/*
 * (C) Copyright 2006-2008
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <nand.h>
#include <asm/io.h>

#include "../../../board/nuvoton/nuc970/register.h"

void board_init_f(unsigned long bootflag);

extern int nuc970_serial_init (void);
extern int nuc970_nand_read_page_hwecc_oob_first(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int oob_required, int page);
extern void nuc970_nand_command_lp(struct mtd_info *mtd, unsigned int command, int column, int page_addr);
extern unsigned char nuc970_nand_read_byte(struct mtd_info *mtd);
extern int nand_register(int devnum, struct mtd_info *mtd);
extern int board_nand_postinit(struct mtd_info *mtd);
extern void sysprintf(char* pcStr,...);
#define printf(fmt, arg...) sysprintf(fmt, ##arg) //CWWeng add

//static int nand_ecc_pos[] = CONFIG_SYS_NAND_ECCPOS;

#ifndef CONFIG_SYS_NAND_BASE_LIST
#define CONFIG_SYS_NAND_BASE_LIST { CONFIG_SYS_NAND_BASE }
#endif

static struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE];
static ulong base_address[CONFIG_SYS_MAX_NAND_DEVICE] = CONFIG_SYS_NAND_BASE_LIST;

static int aes_mtp_decrypt(unsigned int image_addr, int image_size);

static int nand_is_bad_block(struct mtd_info *mtd, int block)
{
	int page_addr = 0 + block * (mtd->erasesize / mtd->writesize);

	nuc970_nand_command_lp(mtd, NAND_CMD_READOOB, 0 , page_addr );
	if(nuc970_nand_read_byte(mtd)!=0xff)
		return 1;
	return 0;

}

static int nand_read_page(struct mtd_info *mtd, int block, int page, uchar *dst)
{
	//struct nand_chip *this = mtd->priv;
	struct nand_chip *chip = mtd_to_nand(mtd);
	int real_page;

	real_page = block * (mtd->erasesize / mtd->writesize) + page;

	nuc970_nand_read_page_hwecc_oob_first(mtd, chip, dst, 0, real_page);
	/*
		if (chip->ecc.read_page)
			chip->ecc.read_page(mtd, chip, dst, 0, real_page); //CWWeng : it calls nuc970_nand_read_page_hwecc_oob_first
	*/
	return 0;
}

static int nand_load(struct mtd_info *mtd, unsigned int offs,
                     unsigned int uboot_size, uchar *dst)
{
	unsigned int block, lastblock;
	unsigned int page;
	unsigned int page_count = mtd->erasesize / mtd->writesize;

	/*
	 * offs has to be aligned to a page address!
	 */
	block = offs / mtd->erasesize;
	lastblock = (offs + uboot_size - 1) / mtd->erasesize;
	page = (offs % mtd->erasesize) / mtd->writesize;

	while (block <= lastblock) {
		if (!nand_is_bad_block(mtd, block)) {
			/*
			 * Skip bad blocks
			 */
			while (page < page_count) {
				nand_read_page(mtd, block, page, dst);
				dst += mtd->writesize;
				page++;
			}

			page = 0;
		} else {
			lastblock++;
		}

		block++;
	}

	return 0;
}

/*
 * The main entry for NAND booting. It's necessary that SDRAM is already
 * configured and available since this code loads the main U-Boot image
 * from NAND into SDRAM and starts it from there.
 */
//void nand_boot(void)
void board_init_f(unsigned long bootflag)
{
	struct nand_chip *nand = &nand_chip[0];
	struct mtd_info *mtd = nand_to_mtd(nand);
	ulong base_addr = base_address[0];
	int maxchips = CONFIG_SYS_NAND_MAX_CHIPS;
	__attribute__((noreturn)) void (*uboot)(void);

	nuc970_serial_init();
	printf("NAND boot!\n");

	if (maxchips < 1)
		maxchips = 1;

	nand->IO_ADDR_R = nand->IO_ADDR_W = (void  __iomem *)base_addr;

	if (board_nand_init(nand))
		return;

	nand_scan(mtd, maxchips); /* CWWeng : 2018/1/2 : not return */

	board_nand_postinit(mtd);

	nand_register(0, mtd);

	/*
	 * Load U-Boot image from NAND into RAM
	 */
	nand_load(mtd, CONFIG_SYS_NAND_U_BOOT_OFFS, CONFIG_SYS_NAND_U_BOOT_SIZE,
	          (uchar *)CONFIG_SYS_NAND_U_BOOT_DST);

	aes_mtp_decrypt((unsigned int)CONFIG_SYS_NAND_U_BOOT_DST, CONFIG_SYS_NAND_U_BOOT_SIZE);

#ifdef CONFIG_NAND_ENV_DST
	nand_load(mtd, CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE,
	          (uchar *)CONFIG_NAND_ENV_DST);

#ifdef CONFIG_ENV_OFFSET_REDUND
	nand_load(mtd, CONFIG_ENV_OFFSET_REDUND, CONFIG_ENV_SIZE,
	          (uchar *)CONFIG_NAND_ENV_DST + CONFIG_ENV_SIZE);
#endif
#endif

	if (nand->select_chip)
		nand->select_chip(mtd, -1);

	/*
	 * Jump to U-Boot image
	 */
	uboot = (void *)CONFIG_SYS_NAND_U_BOOT_START;

	(*uboot)();
}

/* Lowlevel init isn't used on nuc970, so just provide a dummy one here */
void lowlevel_init(void) {}

/*
 * Called in case of an exception.
 */
void hang(void)
{
	/* Loop forever */
	while (1) ;
}



/*-------------------------------------------------------------------*/
/*  AES decrypt image with MTP NAND-boot key                         */
/*  Return value:                                                    */
/*  0:  Is secure boot with AES encrypted. uboot decrypted success.  */
/*  -1: Crypto H/W error                                             */
/*  -2: Is not secure boot with AES encrypted.                       */
/*  -3: Register unlock failed                                       */
/*-------------------------------------------------------------------*/

#define REG_MTP_KEYEN		0xB800C000
#define REG_MTP_USERDATA	0xB800C00C
#define REG_MTP_STATUS		0xB800C040
#define REG_MTP_REGLCTL		0xB800C050
#define REG_CRPT_INTEN		0xB000F000
#define REG_CRPT_INTSTS		0xB000F004

static int aes_mtp_decrypt(unsigned int image_addr, int image_size)
{
	int ret;
	int volatile loop;

	int  i;
	unsigned char  *ptr = (unsigned char *)image_addr;
	
	/* enable OTP */
	__raw_writel(__raw_readl(REG_PCLKEN1)|(1 << 26), REG_PCLKEN1);
	__raw_writel(0x59, REG_MTP_REGLCTL);
	__raw_writel(0x16, REG_MTP_REGLCTL);
	__raw_writel(0x88, REG_MTP_REGLCTL);

	if (__raw_readl(REG_MTP_REGLCTL) != 0x1) {
		ret = -3;
		goto ret_out;
	}

	__raw_writel(__raw_readl(REG_MTP_KEYEN)|0x1, REG_MTP_KEYEN);

	/* check if secure boot */
	for (loop = 0; loop < 0x100000; loop++)
	{
		if ((__raw_readl(REG_MTP_STATUS) & 0x3) == 0x3)
		{
			if ((__raw_readl(REG_MTP_USERDATA) & 0x5) == 0x5)
			{
				printk("Is secure boot with AES encrypt.\n");

				/* initial CRYPTO engine */
				__raw_writel(__raw_readl(REG_HCLKEN)|(1<<23), REG_HCLKEN);
				__raw_writel(0x3, REG_CRPT_INTEN);
				__raw_writel(0, REG_CRPT_AES_IV0);
				__raw_writel(0, REG_CRPT_AES_IV1);
				__raw_writel(0, REG_CRPT_AES_IV2);
				__raw_writel(0, REG_CRPT_AES_IV3);

				__raw_writel(image_addr, REG_CRPT_AES_SADR);
				__raw_writel(image_addr, REG_CRPT_AES_DADR);
				__raw_writel(image_size, REG_CRPT_AES_CNT);

				/* clear interrupt flag */
				__raw_writel(0x3, REG_CRPT_INTSTS);

				__raw_writel(0x00C000B9, REG_CRPT_AES_CTL);

				while ((__raw_readl(REG_CRPT_INTSTS) & 0x3) == 0);

				if (__raw_readl(REG_CRPT_INTSTS) & 0x2) {
					ret = -1;
					goto ret_out;
				}

				__raw_writel(0x3, REG_CRPT_INTSTS);

				ret = 0;
				goto ret_out;

			}
			else {
				ret = -2;
				goto ret_out;
			}
		}

		/* No Key */
		if ((__raw_readl(REG_MTP_STATUS) & 0x5) == 0x5) {
			// dbg_printf("No Key\n");
			ret = -2;
			goto ret_out;
		}
	}

ret_out:
	__raw_writel(0, REG_CRPT_AES_CTL);
	__raw_writel(0, REG_CRPT_INTEN);
	__raw_writel(__raw_readl(REG_HCLKEN)& ~(1<<23), REG_HCLKEN);

	__raw_writel(__raw_readl(REG_MTP_KEYEN)& ~0x1, REG_MTP_KEYEN);
	__raw_writel(__raw_readl(REG_PCLKEN1)& ~(1 << 26), REG_PCLKEN1);

	return ret;
}









