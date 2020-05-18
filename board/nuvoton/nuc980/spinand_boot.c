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

void board_init_f(unsigned long bootflag);

extern void nuc980_spi_init(void);
extern uint8_t WB_Serial_NAND_bad_block_check(uint32_t page_address);
extern void WB_Serial_NAND_PageDataRead(uint8_t addr2, uint8_t addr1, uint8_t addr0);
extern void WB_Serial_NAND_Normal_Read(uint8_t addh, uint8_t addl, uint8_t* buff, uint32_t count);
extern int nuc980_serial_init (void);
extern void sysprintf(char* pcStr,...);
#define printf(fmt, arg...) sysprintf(fmt, ##arg) //CWWeng add

static struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE];

static int spinand_is_bad_block(struct mtd_info *mtd, int block)
{
	int page_addr = 0 + block * (mtd->erasesize / mtd->writesize);

	if (WB_Serial_NAND_bad_block_check(page_addr))
		return 1;
	return 0;

}

static int spinand_read_page(struct mtd_info *mtd, int block, int page, uchar *dst)
{
	int real_page;

	real_page = block * (mtd->erasesize / mtd->writesize) + page;

	WB_Serial_NAND_PageDataRead((real_page >> 16) & 0xFF, (real_page >> 8) & 0xFF, real_page & 0xFF);
#ifdef CONFIG_SPI_NAND_MICRON
	WB_Serial_NAND_Normal_Read(real_page & (1 << 6) ? (1 << 4) : 0, 0, dst, mtd->writesize);
#else
	WB_Serial_NAND_Normal_Read(0, 0, dst, mtd->writesize);
#endif
	return 0;
}

static int spinand_load(struct mtd_info *mtd, unsigned int offs,
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
		if (!spinand_is_bad_block(mtd, offs)) {
			/*
			 * Skip bad blocks
			 */
			while (page < page_count) {
				spinand_read_page(mtd, block, page, dst);
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
 * The main entry for SPI NAND booting. It's necessary that SDRAM is already
 * configured and available since this code loads the main U-Boot image
 * from SPI NAND Flash into SDRAM and starts it from there.
 */
//void nand_boot(void)
void board_init_f(unsigned long bootflag)
{
	struct nand_chip *nand = &nand_chip[0];
	struct mtd_info *mtd = nand_to_mtd(nand);
	int maxchips = CONFIG_SYS_NAND_MAX_CHIPS;
	__attribute__((noreturn)) void (*uboot)(void);

	nuc980_serial_init();
	printf("\n SPL load main U-Boot from SPI NAND Flash! (%s %s)\n",__DATE__,__TIME__);

	if (maxchips < 1)
		maxchips = 1;

	nuc980_spi_init();

	mtd->writesize = 2048;
	mtd->erasesize = 64 * (mtd->writesize);

	/*
	 * Load U-Boot image from SPI NAND into RAM
	 */
	spinand_load(mtd, CONFIG_SYS_NAND_U_BOOT_OFFS, CONFIG_SYS_NAND_U_BOOT_SIZE,
	             (uchar *)CONFIG_SYS_NAND_U_BOOT_DST);

#ifdef CONFIG_NAND_ENV_DST
	spinand_load(mtd, CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE,
	             (uchar *)CONFIG_NAND_ENV_DST);

#ifdef CONFIG_ENV_OFFSET_REDUND
	spinand_load(mtd, CONFIG_ENV_OFFSET_REDUND, CONFIG_ENV_SIZE,
	             (uchar *)CONFIG_NAND_ENV_DST + CONFIG_ENV_SIZE);
#endif
#endif

	/*
	 * Jump to U-Boot image
	 */
	uboot = (void *)CONFIG_SYS_NAND_U_BOOT_START;

	(*uboot)();
}

/* Lowlevel init isn't used on nuc980, so just provide a dummy one here */
void lowlevel_init(void) {}

/*
 * Called in case of an exception.
 */
void hang(void)
{
	/* Loop forever */
	while (1) ;
}
