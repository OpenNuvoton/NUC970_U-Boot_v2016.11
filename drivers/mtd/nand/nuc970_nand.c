/*
 * Copyright © 2014 Nuvoton technology corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <common.h>
#include <malloc.h>
#include <ubi_uboot.h>

#include <nand.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

#include "../../../board/nuvoton/nuc970/register.h"

//#define debug printf

#define NAND_EN     0x08
#define READYBUSY   (0x01 << 18)
#define ENDADDR     (0x01 << 31)

/*-----------------------------------------------------------------------------
 * Define some constants for BCH
 *---------------------------------------------------------------------------*/
// define the total padding bytes for 512/1024 data segment
#define BCH_PADDING_LEN_512     32
#define BCH_PADDING_LEN_1024    64
// define the BCH parity code lenght for 512 bytes data pattern
#define BCH_PARITY_LEN_T4  8
#define BCH_PARITY_LEN_T8  15
#define BCH_PARITY_LEN_T12 23
#define BCH_PARITY_LEN_T15 29
// define the BCH parity code lenght for 1024 bytes data pattern
#define BCH_PARITY_LEN_T24 45


#define BCH_T15   0x00400000
#define BCH_T12   0x00200000
#define BCH_T8    0x00100000
#define BCH_T4    0x00080000
#define BCH_T24   0x00040000


struct nuc970_nand_info {
	struct nand_hw_control  controller;
	struct mtd_info         mtd;
	struct nand_chip        chip;
	int                     eBCHAlgo;
	int                     m_i32SMRASize;
	int                     m_ePageSize;
};
struct nuc970_nand_info *nuc970_nand;

static struct nand_ecclayout nuc970_nand_oob;

static const int g_i32BCHAlgoIdx[5] = { BCH_T4, BCH_T8, BCH_T12, BCH_T15, BCH_T24 };
static const int g_i32ParityNum[4][5] = {
	{ 8,    15,     23,     29,     -1  },  // For 512
	{ 32,   60,     92,     116,    90  },  // For 2K
	{ 64,   120,    184,    232,    180 },  // For 4K
	{ 128,  240,    368,    464,    360 },  // For 8K
};

static void nuc970_layout_oob_table ( struct nand_ecclayout* pNandOOBTbl, int oobsize , int eccbytes )
{
	pNandOOBTbl->eccbytes = eccbytes;

	pNandOOBTbl->oobavail = oobsize - 4 - eccbytes ;

	pNandOOBTbl->oobfree[0].offset = 4;  // Bad block marker size

	pNandOOBTbl->oobfree[0].length = oobsize - eccbytes - pNandOOBTbl->oobfree[0].offset ;

	pNandOOBTbl->oobfree[1].offset = 0;

	pNandOOBTbl->oobfree[1].length = 0;
}


static void nuc970_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;

	//debug("hwcontrol(): 0x%02x 0x%02x\n", cmd, ctrl);

	if (ctrl & NAND_CTRL_CHANGE) {
		ulong IO_ADDR_W = (ulong)REG_SMDATA;

		if ((ctrl & NAND_CLE))
			IO_ADDR_W = REG_SMCMD;
		if ((ctrl & NAND_ALE))
			IO_ADDR_W = REG_SMADDR;

		chip->IO_ADDR_W = (void *)IO_ADDR_W;
	}

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, chip->IO_ADDR_W);
}


/* select chip */
static void nuc970_nand_select_chip(struct mtd_info *mtd, int chip)
{
	writel((readl(REG_SMCSR)&(~0x06000000))|0x04000000, REG_SMCSR);
	return;
}


static int nuc970_dev_ready(struct mtd_info *mtd)
{
	int ready;

	ready = (readl(REG_SMISR) & READYBUSY) ? 1 : 0;

	return ready;
}


static void nuc970_nand_command_lp(struct mtd_info *mtd, unsigned int command, int column, int page_addr)
{
	register struct nand_chip *chip = mtd->priv;

	//debug("nuc970_nand_command_lp, 0x%x\n", command);

	if (command == NAND_CMD_READOOB) {
		column += mtd->writesize;
		command = NAND_CMD_READ0;
	}
//debug("cmd 0x%x, page 0x%x, col 0x%x\n", command, page_addr, column);

	writel(command & 0xff, REG_SMCMD);

	if (command == NAND_CMD_READID) {
		writel(ENDADDR, REG_SMADDR);
	} else {
		if (column != -1 || page_addr != -1) {
			if (column != -1) {
				writel(column&0xFF, REG_SMADDR);
				if ( page_addr != -1 )
					writel(column >> 8, REG_SMADDR);
				else
					writel((column >> 8) | ENDADDR, REG_SMADDR);
			}

			if (page_addr != -1) {
				writel(page_addr&0xFF, REG_SMADDR);

				if ( chip->chipsize > (128 << 20) ) {
					writel((page_addr >> 8)&0xFF, REG_SMADDR);
					writel(((page_addr >> 16)&0xFF)|ENDADDR, REG_SMADDR);
				} else {
					writel(((page_addr >> 8)&0xFF)|ENDADDR, REG_SMADDR);
				}
			}
		}
	}

	switch (command) {
	case NAND_CMD_CACHEDPROG:
	case NAND_CMD_PAGEPROG:
	case NAND_CMD_ERASE1:
	case NAND_CMD_ERASE2:
	case NAND_CMD_SEQIN:
	case NAND_CMD_RNDIN:
	case NAND_CMD_STATUS:
		return;

	case NAND_CMD_RESET:
		if (chip->dev_ready)
			break;

		if ( chip->chip_delay )
			udelay(chip->chip_delay);

		writel(NAND_CMD_STATUS, REG_SMCMD);
		writel(command, REG_SMCMD);

		while (!(readl(REG_SMISR) & READYBUSY)) ;
		return;

	case NAND_CMD_RNDOUT:
		writel(NAND_CMD_RNDOUTSTART, REG_SMCMD);
		ndelay(10);
		return;

	case NAND_CMD_READ0:
		writel(NAND_CMD_READSTART, REG_SMCMD);
		break;
	default:
		if (!chip->dev_ready) {
			if ( chip->chip_delay )
				udelay(chip->chip_delay);
			return;
		}
	}

	while (!(readl(REG_SMISR) & READYBUSY)) ;

}

/*
 * nuc970_nand_read_byte - read a byte from NAND controller into buffer
 * @mtd: MTD device structure
 */
static unsigned char nuc970_nand_read_byte(struct mtd_info *mtd)
{
//    unsigned char val;
//    val = (unsigned char)readl(REG_SMDATA);
//debug("nuc970_nand_read_byte <0x%x>\n", val);
//    return val;
	return ((unsigned char)readl(REG_SMDATA));
}

/*
 * nuc970_nand_write_buf - write data from buffer into NAND controller
 * @mtd: MTD device structure
 * @buf: virtual address in RAM of source
 * @len: number of data bytes to be transferred
 */

static void nuc970_nand_write_buf(struct mtd_info *mtd, const unsigned char *buf, int len)
{
	int i;

	//debug("nuc970_nand_write_buf\n");

	for (i = 0; i < len; i++)
		writel(buf[i], REG_SMDATA);
}

/*
 * nuc970_nand_read_buf - read data from NAND controller into buffer
 * @mtd: MTD device structure
 * @buf: virtual address in RAM of source
 * @len: number of data bytes to be transferred
 */
static void nuc970_nand_read_buf(struct mtd_info *mtd, unsigned char *buf, int len)
{
	int i;
//debug("nuc970_nand_read_buf <%d>\n", len);

	for (i = 0; i < len; i++)
		buf[i] = (unsigned char)readl(REG_SMDATA);
}


/*
 * Enable HW ECC : unused on most chips
 */
void nuc970_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
}

/*
 * Calculate HW ECC
 * function called after a write
 * mtd:        MTD block structure
 * dat:        raw data (unused)
 * ecc_code:   buffer for ECC
 */
static int nuc970_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	return 0;
}

/*
 * HW ECC Correction
 * function called after a read
 * mtd:        MTD block structure
 * dat:        raw data read from the chip
 * read_ecc:   ECC from the chip (unused)
 * isnull:     unused
 */
static int nuc970_nand_correct_data(struct mtd_info *mtd, u_char *dat,
                                    u_char *read_ecc, u_char *calc_ecc)
{
	return 0;
}


/*-----------------------------------------------------------------------------
 * Correct data by BCH alrogithm.
 *      Support 8K page size NAND and BCH T4/8/12/15/24.
 *---------------------------------------------------------------------------*/
void fmiSM_CorrectData_BCH(u8 ucFieidIndex, u8 ucErrorCnt, u8* pDAddr)
{
	u32 uaData[24], uaAddr[24];
	u32 uaErrorData[6];
	u8  ii, jj;
	u32 uPageSize;
	u32 field_len, padding_len, parity_len;
	u32 total_field_num;
	u8  *smra_index;

	//--- assign some parameters for different BCH and page size
	switch (readl(REG_SMCSR) & 0x007C0000) {
	case BCH_T24:
		field_len   = 1024;
		padding_len = BCH_PADDING_LEN_1024;
		parity_len  = BCH_PARITY_LEN_T24;
		break;
	case BCH_T15:
		field_len   = 512;
		padding_len = BCH_PADDING_LEN_512;
		parity_len  = BCH_PARITY_LEN_T15;
		break;
	case BCH_T12:
		field_len   = 512;
		padding_len = BCH_PADDING_LEN_512;
		parity_len  = BCH_PARITY_LEN_T12;
		break;
	case BCH_T8:
		field_len   = 512;
		padding_len = BCH_PADDING_LEN_512;
		parity_len  = BCH_PARITY_LEN_T8;
		break;
	case BCH_T4:
		field_len   = 512;
		padding_len = BCH_PADDING_LEN_512;
		parity_len  = BCH_PARITY_LEN_T4;
		break;
	default:
		return;
	}

	uPageSize = readl(REG_SMCSR) & 0x00030000;
	switch (uPageSize) {
	case 0x30000:
		total_field_num = 8192 / field_len;
		break;
	case 0x20000:
		total_field_num = 4096 / field_len;
		break;
	case 0x10000:
		total_field_num = 2048 / field_len;
		break;
	case 0x00000:
		total_field_num =  512 / field_len;
		break;
	default:
		return;
	}

	//--- got valid BCH_ECC_DATAx and parse them to uaData[]
	// got the valid register number of BCH_ECC_DATAx since one register include 4 error bytes
	jj = ucErrorCnt/4;
	jj ++;
	if (jj > 6)
		jj = 6;     // there are 6 BCH_ECC_DATAx registers to support BCH T24

	for(ii=0; ii<jj; ii++) {
		uaErrorData[ii] = readl(REG_BCH_ECC_DATA0 + ii*4);
	}

	for(ii=0; ii<jj; ii++) {
		uaData[ii*4+0] = uaErrorData[ii] & 0xff;
		uaData[ii*4+1] = (uaErrorData[ii]>>8) & 0xff;
		uaData[ii*4+2] = (uaErrorData[ii]>>16) & 0xff;
		uaData[ii*4+3] = (uaErrorData[ii]>>24) & 0xff;
	}

	//--- got valid REG_BCH_ECC_ADDRx and parse them to uaAddr[]
	// got the valid register number of REG_BCH_ECC_ADDRx since one register include 2 error addresses
	jj = ucErrorCnt/2;
	jj ++;
	if (jj > 12)
		jj = 12;    // there are 12 REG_BCH_ECC_ADDRx registers to support BCH T24

	for(ii=0; ii<jj; ii++) {
		uaAddr[ii*2+0] = readl(REG_BCH_ECC_ADDR0 + ii*4) & 0x07ff;   // 11 bits for error address
		uaAddr[ii*2+1] = (readl(REG_BCH_ECC_ADDR0 + ii*4)>>16) & 0x07ff;
	}

	//--- pointer to begin address of field that with data error
	pDAddr += (ucFieidIndex-1) * field_len;

	//--- correct each error bytes
	for(ii=0; ii<ucErrorCnt; ii++) {
		// for wrong data in field
		if (uaAddr[ii] < field_len) {
			*(pDAddr+uaAddr[ii]) ^= uaData[ii];
		}
		// for wrong first-3-bytes in redundancy area
		else if (uaAddr[ii] < (field_len+3)) {
			uaAddr[ii] -= field_len;
			uaAddr[ii] += (parity_len*(ucFieidIndex-1));    // field offset
			*((u8 *)REG_SMRA0 + uaAddr[ii]) ^= uaData[ii];
		}
		// for wrong parity code in redundancy area
		else {
			// BCH_ERR_ADDRx = [data in field] + [3 bytes] + [xx] + [parity code]
			//                                   |<--     padding bytes      -->|
			// The BCH_ERR_ADDRx for last parity code always = field size + padding size.
			// So, the first parity code = field size + padding size - parity code length.
			// For example, for BCH T12, the first parity code = 512 + 32 - 23 = 521.
			// That is, error byte address offset within field is
			uaAddr[ii] = uaAddr[ii] - (field_len + padding_len - parity_len);

			// smra_index point to the first parity code of first field in register SMRA0~n
			smra_index = (u8 *)
			             (REG_SMRA0 + (readl(REG_SMREACTL) & 0x1ff) - // bottom of all parity code -
			              (parity_len * total_field_num)                             // byte count of all parity code
			             );

			// final address = first parity code of first field +
			//                 offset of fields +
			//                 offset within field
			*((u8 *)smra_index + (parity_len * (ucFieidIndex-1)) + uaAddr[ii]) ^= uaData[ii];
		}
	}   // end of for (ii<ucErrorCnt)
}

int fmiSMCorrectData (struct mtd_info *mtd, unsigned long uDAddr )
{
	int uStatus, ii, jj, i32FieldNum=0;
	volatile int uErrorCnt = 0;

	if ( readl ( REG_SMISR ) & 0x4 ) {
		if ( ( readl(REG_SMCSR) & 0x7C0000) == BCH_T24 )
			i32FieldNum = mtd->writesize / 1024;    // Block=1024 for BCH
		else
			i32FieldNum = mtd->writesize / 512;

		if ( i32FieldNum < 4 )
			i32FieldNum  = 1;
		else
			i32FieldNum /= 4;

		for ( jj=0; jj<i32FieldNum; jj++ ) {
			uStatus = readl ( REG_SMECC_ST0+jj*4 );
//printk(">> 0x%x, 0x%x, <0x%x>\n", uStatus, readl (REG_SMISR), readl(REG_SMECC_ST0+jj*4));
			if ( !uStatus )
				continue;

			for ( ii=1; ii<5; ii++ ) {
				if ( !(uStatus & 0x03) ) { // No error

					uStatus >>= 8;
					continue;

				} else if ( (uStatus & 0x03)==0x01 ) { // Correctable error

					uErrorCnt = (uStatus >> 2) & 0x1F;
//printk(">>> 0x%x\n", uErrorCnt);
					fmiSM_CorrectData_BCH(jj*4+ii, uErrorCnt, (u8 *)uDAddr);

				} else { // uncorrectable error or ECC error
//printk("uncorrectable\n");
					return -1;
				}
				uStatus >>= 8;
			}
		} //jj
	}
	return uErrorCnt;
}


static inline int _nuc970_nand_dma_transfer(struct mtd_info *mtd, const u_char *addr, unsigned int len, int is_write)
{
	struct nuc970_nand_info *nand = nuc970_nand;

	//debug("_nuc970_nand_dma_transfer 0x%x, %d, %d\n", addr, len, mtd->writesize);
	// For save, wait DMAC to ready
	while ( readl(REG_DMACCSR) & 0x200 );

	// Reinitial dmac
	// DMAC enable
	writel( readl(REG_DMACCSR) | 0x3, REG_DMACCSR);
	while (readl(REG_DMACCSR) & 0x2);

	// Clear DMA finished flag
	writel( readl(REG_SMISR) | 0x1, REG_SMISR);

	// Disable Interrupt
	writel(readl(REG_SMIER) & ~(0x1), REG_SMIER);

	// Fill dma_addr
	writel((unsigned long)addr, REG_DMACSAR);

	// Enable target abort interrupt generation during DMA transfer.
	writel( 0x1, REG_DMACIER);

	// Clear Ready/Busy 0 Rising edge detect flag
	writel(0x400, REG_SMISR);

	// Set which BCH algorithm
	if ( nand->eBCHAlgo >= 0 ) {
		// Set BCH algorithm
		writel( (readl(REG_SMCSR) & (~0x7C0000)) | g_i32BCHAlgoIdx[nand->eBCHAlgo], REG_SMCSR);
		// Enable H/W ECC, ECC parity check enable bit during read page
		writel( readl(REG_SMCSR) | 0x00800080, REG_SMCSR);
	} else  {
		// Disable H/W ECC / ECC parity check enable bit during read page
		writel( readl(REG_SMCSR) & (~0x00800080), REG_SMCSR);
	}

	writel( nand->m_i32SMRASize , REG_SMREACTL );

	writel( readl(REG_SMIER) & (~0x4), REG_SMIER );

	writel ( 0x4, REG_SMISR );

	// Enable SM_CS0
	writel((readl(REG_SMCSR)&(~0x06000000))|0x04000000, REG_SMCSR);
	/* setup and start DMA using dma_addr */

	if ( is_write ) {
		register char *ptr= (char *)REG_SMRA0;
		// To mark this page as dirty.
		if ( ptr[3] == 0xFF )
			ptr[3] = 0;
		if ( ptr[2] == 0xFF )
			ptr[2] = 0;

		writel ( readl(REG_SMCSR) | 0x4, REG_SMCSR );
		while ( !(readl(REG_SMISR) & 0x1) );

	} else {
		// Blocking for reading
		// Enable DMA Read

		writel ( readl(REG_SMCSR) | 0x2, REG_SMCSR);

		if ( readl(REG_SMCSR) & 0x80 ) {
			do {
				int stat=0;
				if ( (stat=fmiSMCorrectData ( mtd,  (unsigned long)addr)) < 0 ) {
					mtd->ecc_stats.failed++;
					writel(0x4, REG_SMISR );
					writel(0x3, REG_DMACCSR);          // reset DMAC
					writel(readl(REG_SMCSR)|0x1, REG_SMCSR);    // reset SM controller
					break;
				} else if ( stat > 0 ) {
					//mtd->ecc_stats.corrected += stat; //Occure: MLC UBIFS mount error
					writel(0x4, REG_SMISR );
				}

			} while (!(readl(REG_SMISR) & 0x1) || (readl(REG_SMISR) & 0x4));
		} else
			while (!(readl(REG_SMISR) & 0x1));
	}

	// Clear DMA finished flag
	writel(readl(REG_SMISR) | 0x1, REG_SMISR);

	return 0;
}


/**
 * nand_write_page_hwecc - [REPLACABLE] hardware ecc based page write function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @buf:        data buffer
 */
//static void nuc970_nand_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf) //CWWeng 2017.2.14
static int nuc970_nand_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf, int oob_required, int page)
{
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	uint32_t hweccbytes=chip->ecc.layout->eccbytes;
	register char * ptr=(char *)REG_SMRA0;

	//debug("nuc970_nand_write_page_hwecc\n");
	memset ( (void*)ptr, 0xFF, mtd->oobsize );
	memcpy ( (void*)ptr, (void*)chip->oob_poi,  mtd->oobsize - chip->ecc.total );

	_nuc970_nand_dma_transfer( mtd, buf, mtd->writesize , 0x1);

	// Copy parity code in SMRA to calc
	memcpy ( (void*)ecc_calc,  (void*)( REG_SMRA0 + ( mtd->oobsize - chip->ecc.total ) ), chip->ecc.total );

	// Copy parity code in calc to oob_poi
	memcpy ( (void*)(chip->oob_poi+hweccbytes), (void*)ecc_calc, chip->ecc.total);

	return 0; //CWWeng 2017.2.14 : TODO: should return the right thing
}

/**
 * nuc970_nand_read_page_hwecc_oob_first - hardware ecc based page write function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @buf:        buffer to store read data
 * @page:       page number to read
 */
//static int nuc970_nand_read_page_hwecc_oob_first(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int page) //CWWeng 2017.2.14
static int nuc970_nand_read_page_hwecc_oob_first(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int oob_required, int page)
{
	int eccsize = chip->ecc.size;
	uint8_t *p = buf;
	char * ptr= (char *)REG_SMRA0;

	//debug("nuc970_nand_read_page_hwecc_oob_first\n");
	/* At first, read the OOB area  */
	nuc970_nand_command_lp(mtd, NAND_CMD_READOOB, 0, page);
	nuc970_nand_read_buf(mtd, chip->oob_poi, mtd->oobsize);

	// Second, copy OOB data to SMRA for page read
	memcpy ( (void*)ptr, (void*)chip->oob_poi, mtd->oobsize );

#if 0
	if (1) {
		int volatile i;
		for(i=0; i<mtd->oobsize; i++) {
			printf("%02X ",  *(ptr+i) );
			if ( i % 32 == 31)
				printf("\n");
		}
		printf("\n");
	}
#endif

	// Third, read data from nand
	nuc970_nand_command_lp(mtd, NAND_CMD_READ0, 0, page);
//printk("===========> 0x%x, %d, 0x%x\n", p, eccsize, page);
	_nuc970_nand_dma_transfer(mtd, p, eccsize, 0x0);
#if 0
	for(i=0; i<512; i++) {
		printk("%02X ",  *(p+i) );
		if ( i % 32 == 31)
			printk("\n");
	}
	printk("\n");
#endif

	// Fouth, restore OOB data from SMRA
	memcpy ( (void*)chip->oob_poi, (void*)ptr, mtd->oobsize );

	return 0;
}

int (*read_oob)(struct mtd_info *mtd, struct nand_chip *chip, int page);
/**
 * nuc970_nand_read_oob_hwecc - [REPLACABLE] the most common OOB data read function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @page:       page number to read
 * @sndcmd:     flag whether to issue read command or not
 */
//static int nuc970_nand_read_oob_hwecc(struct mtd_info *mtd, struct nand_chip *chip, int page, int sndcmd) //CWWeng 2017.2.14
static int nuc970_nand_read_oob_hwecc(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	char * ptr=(char *)REG_SMRA0;

	//debug("nuc970_nand_read_oob_hwecc, page %d, %d\n", page, sndcmd);
	/* At first, read the OOB area  */
	//if ( sndcmd ) { //CWWeng 2017.2.14
	if (1) { //CWWeng 2017.2.14
		nuc970_nand_command_lp(mtd, NAND_CMD_READOOB, 0, page);
		//sndcmd = 0; //CWWeng 2017.2.14
	}

	nuc970_nand_read_buf(mtd, chip->oob_poi, mtd->oobsize);

	// Second, copy OOB data to SMRA for page read
	memcpy ( (void*)ptr, (void*)chip->oob_poi, mtd->oobsize );

#if 0
	// Third, read data from nand
	nuc970_nand_command_lp(mtd, NAND_CMD_READ0, 0, page);
	_nuc970_nand_dma_transfer(mtd, NULL, mtd->writesize, 0x0);

	// Fouth, recovery OOB data for SMRA
	memcpy ( (void*)chip->oob_poi, (void*)ptr, mtd->oobsize );
#endif

	//return sndcmd; //CWWeng 2017.2.14
	return 0; //CWWeng 2017.2.14
}



int board_nand_init(struct nand_chip *nand)
{
	struct mtd_info *mtd;

	nuc970_nand = kzalloc(sizeof(struct nuc970_nand_info), GFP_KERNEL);
	if (!nuc970_nand)
		return -1;

	mtd=&nuc970_nand->mtd;
	nuc970_nand->chip.controller = &nuc970_nand->controller;

	/* initialize nand_chip data structure */
	nand->IO_ADDR_R = (void *)REG_SMDATA;
	nand->IO_ADDR_W = (void *)REG_SMDATA;

	/* read_buf and write_buf are default */
	/* read_byte and write_byte are default */
#ifdef CONFIG_NAND_SPL
	nand->read_buf = nand_read_buf;
#endif

	/* hwcontrol always must be implemented */
	nand->cmd_ctrl = nuc970_hwcontrol;
	nand->cmdfunc = nuc970_nand_command_lp;
	nand->dev_ready = nuc970_dev_ready;
	nand->select_chip = nuc970_nand_select_chip;

	nand->read_byte = nuc970_nand_read_byte;
	nand->write_buf = nuc970_nand_write_buf;
	nand->read_buf = nuc970_nand_read_buf;
	//nand->verify_buf = nuc970_verify_buf;
	nand->chip_delay = 50;

	nand->controller = &nuc970_nand->controller;

	nand->ecc.mode      = NAND_ECC_HW_OOB_FIRST;
	nand->ecc.hwctl     = nuc970_nand_enable_hwecc;
	nand->ecc.calculate = nuc970_nand_calculate_ecc;
	nand->ecc.correct   = nuc970_nand_correct_data;
	nand->ecc.write_page= nuc970_nand_write_page_hwecc;
	nand->ecc.read_page = nuc970_nand_read_page_hwecc_oob_first;
	nand->ecc.read_oob  = nuc970_nand_read_oob_hwecc;
	nand->ecc.layout    = &nuc970_nand_oob;
	nand->ecc.strength  = 1; //CWWeng 2017.2.14
	mtd = nand_to_mtd(nand); //CWWeng 2017.2.14

	mtd->priv = nand;

	/* initial NAND controller */
	writel( (readl(REG_HCLKEN)|(0x300000)), REG_HCLKEN);

	if (readl(REG_PWRON) & 0x08000000) {
		writel( 0x55555550, REG_MFP_GPI_L);
		writel( 0x55555555, REG_MFP_GPI_H);
	} else {
		writel( 0x55555555, REG_MFP_GPC_L);
		writel( 0x05555555, REG_MFP_GPC_H);
	}

	// Enable SM_EN
	writel(NAND_EN, REG_FMICSR);
	writel(0x20305, REG_SMTCR);

	// Enable SM_CS0
	writel((readl(REG_SMCSR)&(~0x06000000))|0x04000000, REG_SMCSR);
	writel(0x1, REG_NFECR); /* un-lock write protect */

	// NAND Reset
	writel(readl(REG_SMCSR) | 0x1, REG_SMCSR);    // software reset
	while (readl(REG_SMCSR) & 0x1);

	/* Detect NAND chips */
	/* first scan to find the device and get the page size */
	//if (nand_scan_ident(&(nuc970_nand->mtd), 1, NULL)) { //CWWeng 2017.2.14
	if (nand_scan_ident(mtd, 1, NULL)) { //CWWeng 2017.2.14
		printf("NAND Flash not found !\n");
		//return -1;
	}

	//Set PSize bits of SMCSR register to select NAND card page size
	switch (mtd->writesize) {
	case 2048:
		writel( (readl(REG_SMCSR)&(~0x30000)) + 0x10000, REG_SMCSR);
		nuc970_nand->eBCHAlgo = 0; /* T4 */
		nuc970_nand->m_ePageSize = 1;
		break;

	case 4096:
		writel( (readl(REG_SMCSR)&(~0x30000)) + 0x20000, REG_SMCSR);
		nuc970_nand->eBCHAlgo = 1; /* T8 */
		nuc970_nand->m_ePageSize = 2;
		break;

	case 8192:
		writel( (readl(REG_SMCSR)&(~0x30000)) + 0x30000, REG_SMCSR);
		nuc970_nand->eBCHAlgo = 2; /* T12 */
		nuc970_nand->m_ePageSize = 3;
		break;

	/* Not support now. */
	case 512:
	//writel( (readl(REG_SMCSR)&(~0x30000)) + 0, REG_SMCSR);
	//nuc970_nand->m_ePageSize = ePageSize_512;
	//break;

	default:
		printf("NUC970 NAND CONTROLLER IS NOT SUPPORT THE PAGE SIZE. (%d, %d)\n", mtd->writesize, mtd->oobsize );
	}
	nuc970_layout_oob_table(&nuc970_nand_oob,mtd->oobsize,g_i32ParityNum[nuc970_nand->m_ePageSize][nuc970_nand->eBCHAlgo]);

	/* check power on setting */
	if ((readl(REG_PWRON) & 0xc0) != 0xc0) {
		/* page size */
		switch ((readl(REG_PWRON) & 0xc0)) {
		case 0x00: // 2KB
			mtd->writesize = 2048;
			writel( (readl(REG_SMCSR)&(~0x30000)) + 0x10000, REG_SMCSR);
			nuc970_nand->eBCHAlgo = 0; /* T4 */
			nuc970_nand->m_ePageSize = 1;
			mtd->oobsize = 64;
			break;

		case 0x40: // 4KB
			mtd->writesize = 4096;
			writel( (readl(REG_SMCSR)&(~0x30000)) + 0x20000, REG_SMCSR);
			nuc970_nand->eBCHAlgo = 1; /* T8 */
			nuc970_nand->m_ePageSize = 2;
			mtd->oobsize = 128;
			break;

		case 0x80: // 8KB
			mtd->writesize = 8192;
			writel( (readl(REG_SMCSR)&(~0x30000)) + 0x30000, REG_SMCSR);
			nuc970_nand->eBCHAlgo = 2; /* T12 */
			nuc970_nand->m_ePageSize = 3;
			mtd->oobsize = 376;
			break;

		default:
			printf("WRONG NAND page Power-On-Setting (0x%x)\n", readl(REG_PWRON));
		}
	}

	if ((readl(REG_PWRON) & 0x300) != 0x300) {
		/* ECC */
		switch ((readl(REG_PWRON) & 0x300)) {
		case 0x000: // T12
			nuc970_nand->eBCHAlgo = 2;
			break;

		case 0x100: // T15
			nuc970_nand->eBCHAlgo = 3;
			break;

		case 0x200: // T24
			nuc970_nand->eBCHAlgo = 4;
			break;

		default:
			printf("WRONG ECC Power-On-Setting (0x%x)\n", readl(REG_PWRON));
		}
		mtd->oobsize = g_i32ParityNum[nuc970_nand->m_ePageSize][nuc970_nand->eBCHAlgo] + 8;
	}
	nuc970_layout_oob_table(&nuc970_nand_oob, mtd->oobsize, g_i32ParityNum[nuc970_nand->m_ePageSize][nuc970_nand->eBCHAlgo]);

	nuc970_nand->m_i32SMRASize  = mtd->oobsize;
	nand->ecc.bytes = nuc970_nand_oob.eccbytes;
	nand->ecc.size  = mtd->writesize;

	nand->options = 0;
#ifdef CONFIG_SYS_NAND_USE_FLASH_BBT
	nand->bbt_options |= NAND_BBT_USE_FLASH;
#endif

	// Redundant area size
	writel( nuc970_nand->m_i32SMRASize , REG_SMREACTL );

	// Protect redundant 3 bytes
	// because we need to implement write_oob function to partial data to oob available area.
	// Please note we skip 4 bytes
	writel( readl(REG_SMCSR) | 0x100, REG_SMCSR);

	// To read/write the ECC parity codes automatically from/to NAND Flash after data area field written.
	writel( readl(REG_SMCSR) | 0x10, REG_SMCSR);
	// Set BCH algorithm
	writel( (readl(REG_SMCSR) & (~0x007C0000)) | g_i32BCHAlgoIdx[nuc970_nand->eBCHAlgo], REG_SMCSR);
	// Enable H/W ECC, ECC parity check enable bit during read page
	writel( readl(REG_SMCSR) | 0x00800080, REG_SMCSR);

	//printf("end of nand_init 0x%x\n", readl(REG_SMCSR));
	return 0;
}
