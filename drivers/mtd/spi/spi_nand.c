/*
 * Copyright (c) 2015 The Linux Foundation. All rights reserved.

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#include <common.h>
#include <nand.h>
#include <spi.h>
#include <watchdog.h>
#include <linux/mtd/nand.h>
#include <spi_flash.h>
#include <errno.h>
#include "sf_internal.h"
#include "spi_nand_dev.h"
#include <malloc.h>
#ifdef CONFIG_NUC970
#include "../../spi/nuc970_spi.h"
#endif

//#define CONFIG_SF_DEFAULT_SPEED		(48 * 1000 * 1000)
#define TIMEOUT		5000
#define MFID_GIGA	0xc8
#define MFID_ATO	0x9b
/* Macronix Specific Defines */
#define MFID_MACRONIX   	0xc2
#define MACRONIX_WRAP		((0 & 0x3) << 6)
#define MACRONIX_PLANE		((0 & 0x1) << 4)

#define MACRONIX_NORM_READ_MASK	(MACRONIX_WRAP | MACRONIX_PLANE | 0x0F)

#define spi_print(...)  printf("spi_nand: " __VA_ARGS__)

struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE];
int verify_3bit_ecc(int status);
int verify_2bit_ecc(int status);
int verify_dummy_ecc(int status);
void gigadevice_norm_read_cmd(u8 *cmd, int column);
void macronix_norm_read_cmd(u8 *cmd, int column);
void winbond_norm_read_cmd(u8 *cmd, int column);
extern int spi_nand_flash_cmd_wait_ready(struct spi_flash *flash, u8 status_bit, u8 *status,
        unsigned long timeout);
int nand_register(int devnum, struct mtd_info *mtd);
#define mtd_to_ipq_info(m)	((struct nand_chip *)((m)->priv))->priv

static const struct spi_nand_flash_params spi_nand_flash_tbl[] = {
#if 0
	{
		.id = { 0xc8, 0xb1, 0x48, 0xc8 },
		.page_size = 2048,
		.erase_size = 0x00020000,
		.pages_per_sector = 64,
		.nr_sectors = 1024,
		.oob_size = 64,
		.protec_bpx = 0xC7,
		.norm_read_cmd = gigadevice_norm_read_cmd,
		.verify_ecc = verify_3bit_ecc,
		.name = "GD5F1GQ4XC",
	},
	{
		.id = { 0xff, 0x9b, 0x12 , 0x9b },
		.page_size = 2048,
		.erase_size = 0x00020000,
		.pages_per_sector = 64,
		.nr_sectors = 1024,
		.oob_size = 64,
		.protec_bpx = 0xC7,
		.norm_read_cmd = gigadevice_norm_read_cmd,
		.verify_ecc = verify_dummy_ecc,
		.name = "ATO25D1GA",
	},
	{
		.id = { 0x00, 0xc2, 0x12, 0xc2 },
		.page_size = 2048,
		.erase_size = 0x00020000,
		.pages_per_sector = 64,
		.nr_sectors = 1024,
		.oob_size = 64,
		.protec_bpx = 0xC7,
		.norm_read_cmd = macronix_norm_read_cmd,
		.verify_ecc = verify_2bit_ecc,
		.name = "MX35LFxGE4AB",
	},
#endif
#ifdef CONFIG_SPI_NAND_GD
	{
		.id = { 0xc8, 0xb1, 0x48, 0xc8 },
		.page_size = 2048,
		.erase_size = 0x00020000,
		.pages_per_sector = 64,
		.nr_sectors = 1024,
		.oob_size = 64,
		.protec_bpx = 0xC7,
		.norm_read_cmd = gigadevice_norm_read_cmd,
		.verify_ecc = verify_3bit_ecc,
		.name = "GD5F1GQ4XC",
	},
#else
	{
		.id = { 0xff, 0xef, 0xaa, 0x21 },	// First byte dummy. SPI NAND high-Z, weak pull high in 970
		.page_size = 2048,
		.erase_size = 0x00020000,
		.pages_per_sector = 64,
		.nr_sectors = 1024,
		.oob_size = 64,
		.protec_bpx = 0x87,
		.norm_read_cmd = winbond_norm_read_cmd,
		.verify_ecc = verify_2bit_ecc,
		.name = "W25N01GV",
	},
#endif
};

const struct spi_nand_flash_params *params;
void spinand_internal_ecc(struct mtd_info *mtd, int enable);

void gigadevice_norm_read_cmd(u8 *cmd, int column)
{
	cmd[0] = IPQ40XX_SPINAND_CMD_NORM_READ;
	cmd[1] = 0;
	cmd[2] = (u8)(column >> 8);
	cmd[3] = (u8)(column);

}

void macronix_norm_read_cmd(u8 *cmd, int column)
{
	cmd[0] = IPQ40XX_SPINAND_CMD_NORM_READ;
	cmd[1] = ((u8)(column >> 8) & MACRONIX_NORM_READ_MASK);
	cmd[2] = (u8)(column);
	cmd[3] = 0;

}

void winbond_norm_read_cmd(u8 *cmd, int column)
{

	cmd[0] = IPQ40XX_SPINAND_CMD_NORM_READ;
	cmd[1] = (u8)(column >> 8);
	cmd[2] = (u8)(column);
	cmd[3] = 0;


}

int spi_nand_flash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
                                u8 cmd, u8 poll_bit, u8 *status)
{
	struct spi_slave *spi = flash->spi;
	unsigned long timebase;
	u8 cmd_buf[2];

	cmd_buf[0] = 0x0F;
	cmd_buf[1] = cmd;

	timebase = get_timer(0);
	do {
#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
		WATCHDOG_RESET();
#endif
		spi_flash_cmd_read(spi, cmd_buf, 2, status, 1);
		if ((*status & poll_bit) == 0)
			break;

	} while (get_timer(timebase) < timeout);

	if ((*status & poll_bit) == 0)
		return 0;

	/* Timed out */
	debug("SF: time out!\n");
	return -1;
}

int spi_nand_flash_cmd_wait_ready(struct spi_flash *flash, u8 status_bit, u8 *status,
                                  unsigned long timeout)
{
	return spi_nand_flash_cmd_poll_bit(flash, timeout,
	                                   0xC0, status_bit, status);
}



static int spinand_waitfunc(struct mtd_info *mtd, u8 val, u8 *status)
{
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;
	int ret;

	ret = spi_nand_flash_cmd_wait_ready(flash, val, status, TIMEOUT);
	if (ret) {
		printf("%s Operation Timeout\n", __func__);
		return -1;
	}

	return 0;
}

static int check_offset(struct mtd_info *mtd, loff_t offs)
{
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct nand_chip *chip = info->chip;
	int ret = 0;

	/* Start address must align on block boundary */
	if (offs & ((1 << chip->phys_erase_shift) - 1)) {
		printf("%s: unaligned address\n", __func__);
		ret = -EINVAL;
	}

	return ret;
}

static int spi_nand_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	u8 cmd[8];
	u8 status;
	u32 ret;
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;
	struct nand_chip *chip = info->chip;


	u32 page = (int)(instr->addr >> chip->page_shift);



	if (check_offset(mtd, instr->addr))
		return -1;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf("SF: Unable to claim SPI bus\n");
		return ret;
	}

#ifdef CONFIG_WINBOND_MULTIDIE
	u8 dieid;///die #0, #1
	dieid = (int)(page>>16);


	cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
	cmd[1] = dieid;
	ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
	if (ret) {
		printf("%s  failed for die select :\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}
#endif

	ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WREN, NULL, 0);
	if (ret) {
		printf ("Write enable failed %s\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		goto out;
	}

	cmd[0] = IPQ40XX_SPINAND_CMD_ERASE;
	cmd[1] = (u8)(page >> 16);
	cmd[2] = (u8)(page >> 8);
	cmd[3] = (u8)(page);

	ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
	if (ret) {
		printf("%s  failed for offset:%ld\n", __func__, (long)instr->addr);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}

	if (status & STATUS_E_FAIL) {
		printf("Erase operation failed for 0x%x\n", page);
		ret = -EIO;
		goto out;
	}

	ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WRDI, NULL, 0);

	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Write disable failed\n");
	}

out:
	spi_release_bus(flash->spi);

	return ret;
}

static int spi_nand_block_isbad(struct mtd_info *mtd, loff_t offs)
{
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;
	struct nand_chip *chip = info->chip;
	u8 cmd[8];
	u8 status;
	u32 value = 0xff;
	int page, column, ret;

	page = (int)(offs >> chip->page_shift) & chip->pagemask;
	column = mtd->writesize + chip->badblockpos;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Claim bus failed. %s\n", __func__);
		return -1;
	}

#ifdef CONFIG_WINBOND_MULTIDIE
	u8 dieid;///die #0, #1
	dieid = (int)(page>>16);


	cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
	cmd[1] = dieid;
	ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
	if (ret) {
		printf("%s  failed for die select :\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}
#endif

	cmd[0] = IPQ40XX_SPINAND_CMD_READ;
	cmd[1] = (u8)(page >> 16);
	cmd[2] = (u8)(page >> 8);
	cmd[3] = (u8)(page);
	ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
	if (ret) {
		printf("%s: write command failed\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}
#ifdef CONFIG_SPI_NAND_MICRON
	column |= page & (1 << 6) ? (1 << 12) : 0;	// This bit is used as plane selection
#endif
	info->params->norm_read_cmd(cmd, column);
	ret = spi_flash_cmd_read(flash->spi, cmd, 4, &value, 1);
	if (ret) {
		printf("%s: read data failed\n", __func__);
		goto out;
	}

	if (value != 0xFF) {
		ret = 1;
		goto out;
	}

out:

	spi_release_bus(flash->spi);
	return ret;
}


static int spinand_write_oob_std(struct mtd_info *mtd, struct nand_chip *chip,  int page)
{
	int column, ret = 0;
	u_char *wbuf;
	u8 cmd[8], status;
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;

	wbuf = chip->oob_poi;
	column = mtd->writesize;

#ifdef CONFIG_WINBOND_MULTIDIE
	u8 dieid;///die #0, #1
	dieid = (int)(page>>16);

	cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
	cmd[1] = dieid;
	ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
	if (ret) {
		printf("%s  failed for die select :\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}
#endif

#ifndef CONFIG_SPI_NAND_GD
	ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WREN, NULL, 0);
	if (ret) {
		printf("Write enable failed in %s\n", __func__);
		goto out;
	}
#endif

	cmd[0] = IPQ40XX_SPINAND_CMD_PLOAD;
#ifdef CONFIG_SPI_NAND_MICRON
	cmd[1] = (u8)(column >> 8) | (page & (1 << 6) ? (1 << 4) : 0);	// This bit is used as plane selection
#else
	cmd[1] = (u8)(column >> 8);
#endif
	cmd[2] = (u8)(column);

	ret = spi_flash_cmd_write(flash->spi, cmd, 3, wbuf, 2);
	if (ret) {
		printf("%s: write command failed\n", __func__);
		ret = 1;
		goto out;
	}

#ifdef CONFIG_SPI_NAND_GD
	ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WREN, NULL, 0);
	if (ret) {
		printf("Write enable failed in %s\n", __func__);
		goto out;
	}
#endif

	cmd[0] = IPQ40XX_SPINAND_CMD_PROG;
	cmd[1] = (u8)(page >> 16);
	cmd[2] = (u8)(page >> 8);
	cmd[3] = (u8)(page);

	ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
	if (ret) {
		printf("PLOG failed\n");
		goto out;
	}

	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		if (status)
			printf("Program failed\n");
	}

	ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WRDI, NULL, 0);
	if (ret)
		printf("Write disable failed in %s\n", __func__);

out:
	spi_release_bus(flash->spi);
	return ret;
}

static void fill_oob_data(struct mtd_info *mtd, uint8_t *oob,
                          size_t len, struct mtd_oob_ops *ops)
{
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct nand_chip *chip = info->chip;

	memset(chip->oob_poi, 0xff, mtd->oobsize);
	memcpy(chip->oob_poi + ops->ooboffs, oob, len);

	return;
}

static int spi_nand_write_oob(struct mtd_info *mtd, loff_t to,
                              struct mtd_oob_ops *ops)
{
	int page;
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct nand_chip *chip = info->chip;

	/* Shift to get page */
	page = (int)(to >> chip->page_shift);

	fill_oob_data(mtd, ops->oobbuf, ops->ooblen, ops);

	return spinand_write_oob_std(mtd, chip, page & chip->pagemask);
}

static int spi_nand_block_markbad(struct mtd_info *mtd, loff_t offs)
{
	uint8_t buf[2]= { 0, 0 };
	int ret;
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;
	struct nand_chip *chip = info->chip;
	struct mtd_oob_ops ops;
	struct erase_info einfo;

	ret = spi_nand_block_isbad(mtd, offs);
	if (ret) {
		if (ret > 0)
			return 0;
		return ret;
	}

	/* Attempt erase before marking OOB */
	memset(&einfo, 0, sizeof(einfo));
	einfo.mtd = mtd;
	einfo.addr = offs;
	einfo.len = 1 << chip->phys_erase_shift;
	spi_nand_erase(mtd, &einfo);

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Claim bus failed. %s\n", __func__);
		return -1;
	}

	ops.datbuf = NULL;
	ops.oobbuf = buf;
	ops.ooboffs = chip->badblockpos;
	ops.len = ops.ooblen = 1;

	ret = spi_nand_write_oob(mtd, offs,&ops);
	if (ret)
		goto out;

out:
	spi_release_bus(flash->spi);
	return ret;
}

int verify_3bit_ecc(int status)
{
	int ecc_status = (status & SPINAND_3BIT_ECC_MASK);

	if (ecc_status == SPINAND_3BIT_ECC_ERROR)
		return ECC_ERR;
	else if (ecc_status)
		return ECC_CORRECTED;
	else
		return 0;
}

int verify_2bit_ecc(int status)
{
	int ecc_status = (status & SPINAND_2BIT_ECC_MASK);

	if ((ecc_status == SPINAND_2BIT_ECC_ERROR) ||
	        (ecc_status == SPINAND_2BIT_ECC_MASK))
		return ECC_ERR;
	else if (ecc_status == SPINAND_2BIT_ECC_CORRECTED)
		return ECC_CORRECTED;
	else
		return 0;
}

int verify_dummy_ecc(int status)
{
	return 0;
}

static int spi_nand_read_std(struct mtd_info *mtd, loff_t from, struct mtd_oob_ops *ops)
{
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;
	u32 ret;
	u8 cmd[8];
	u8 status;
	int realpage, page, readlen, bytes, column, bytes_oob;
	int ecc_corrected = 0;
	column = mtd->writesize;
	realpage = (int)(from >> 0xB);


	page = realpage & 0xfffff;


	readlen = ops->len;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Claim bus failed. %s\n", __func__);
		return -1;
	}
	while (1) {

#ifdef CONFIG_WINBOND_MULTIDIE
		u8 dieid;///die #0, #1
		dieid = (int)(page>>16);

		cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
		cmd[1] = dieid;
		ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
		if (ret) {
			printf("%s  failed for die select :\n", __func__);
			goto out;
		}
		ret = spinand_waitfunc(mtd, 0x01, &status);
		if (ret) {
			printf("Operation timeout\n");
			goto out;
		}
#endif

		cmd[0] = IPQ40XX_SPINAND_CMD_READ;
		cmd[1] = (u8)(page >> 16);
		cmd[2] = (u8)(page >> 8);
		cmd[3] = (u8)(page);
		ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
		if (ret) {
			printf("%s: write command failed\n", __func__);
			goto out;
		}
		ret = spinand_waitfunc(mtd, 0x01, &status);
		if (ret) {
			goto out;
		}

		ret = info->params->verify_ecc(status);
		if (ret == ECC_ERR) {
			mtd->ecc_stats.failed++;
			printf("ecc err(0x%x) for page read\n", status);
			ret = -EBADMSG;
			goto out;
		} else if (ret == ECC_CORRECTED) {
			mtd->ecc_stats.corrected++;
			ecc_corrected = 1;
		}

		bytes = ((readlen < mtd->writesize) ? readlen : mtd->writesize);
		bytes_oob = ops->ooblen;

		/* Read Data */
		if (bytes) {
			cmd[0] = IPQ40XX_SPINAND_CMD_NORM_READ;
#ifdef CONFIG_SPI_NAND_MICRON
			cmd[1] = page & (1 << 6) ? (1 << 4) : 0;	// This bit is used as plane selection
#else
			cmd[1] = 0;
#endif
			cmd[2] = 0;
			cmd[3] = 0;
			ret = spi_flash_cmd_read(flash->spi, cmd, 4, ops->datbuf, bytes);
			if (ret) {
				printf("%s: read data failed\n", __func__);
				return -1;
			}
			ops->retlen += bytes;
		}

		/* Read OOB */
		if (bytes_oob) {
			info->params->norm_read_cmd(cmd, column);
			ret = spi_flash_cmd_read(flash->spi, cmd, 4, ops->oobbuf, ops->ooblen);
			if (ret) {
				printf("%s: read data failed\n", __func__);
				return -1;
			}
			ops->oobretlen += ops->ooblen;
		}

		readlen -= bytes;
		if (readlen <= 0)
			break;
		ops->datbuf += bytes;
		realpage++;
		page = realpage & 0xfffff;
	}

	if ((ret == 0) && (ecc_corrected))
		ret = -EUCLEAN;
out:
	spi_release_bus(flash->spi);
	return ret;
}

static int spi_nand_read(struct mtd_info *mtd, loff_t from, size_t len,
                         size_t *retlen, u_char *buf)
{
	struct mtd_oob_ops ops = {0};
	u32 ret;

	ops.mode = MTD_OPS_AUTO_OOB;
	ops.len = len;
	ops.datbuf = (uint8_t *)buf;
	ret = spi_nand_read_std(mtd, from, &ops);
	*retlen = ops.retlen;
	return ret;
}

static int spi_nand_read_oob(struct mtd_info *mtd, loff_t from,
                             struct mtd_oob_ops *ops)
{
	u32 ret;

	/* Disable ECC */
	spinand_internal_ecc(mtd, 0);
	ret = spi_nand_read_std(mtd, from,ops);
	/* Enable ECC */
	spinand_internal_ecc(mtd, 1);
	return ret;
}

static int spi_nand_write(struct mtd_info *mtd, loff_t to, size_t len,
                          size_t *retlen, const u_char *buf)
{
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;
	u8 cmd[8];
	u8 status;
	u32 ret;
	const u_char *wbuf;
	int realpage, page, bytes, write_len;
	write_len = len;
	bytes = mtd->writesize;

	/* Check whether page is aligned */
	if (((to & (mtd->writesize -1)) !=0) ||
	        ((len & (mtd->writesize -1)) != 0)) {
		printf("Attempt to write to non page aligned data\n");
		return -EINVAL;
	}

	realpage = (int)(to >> 0xb);
	page = realpage & 0xfffff;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Write enable failed %s\n", __func__);
		return -1;
	}
	while (1) {
		wbuf = buf;

#ifdef CONFIG_WINBOND_MULTIDIE
		u8 dieid;///die #0, #1
		dieid = (int)(page>>16);

		cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
		cmd[1] = dieid;
		ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
		if (ret) {
			printf("%s  failed for die select :\n", __func__);
			goto out;
		}
		ret = spinand_waitfunc(mtd, 0x01, &status);
		if (ret) {
			printf("Operation timeout\n");
			goto out;
		}
#endif

#ifndef CONFIG_SPI_NAND_GD
		ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WREN, NULL, 0);
		if (ret) {
			printf("Write enable failed\n");
			goto out;
		}
#endif

		/* buffer to be transmittted here */
		cmd[0] = IPQ40XX_SPINAND_CMD_PLOAD;
#ifdef CONFIG_SPI_NAND_MICRON
		cmd[1] = page & (1 << 6) ? (1 << 4) : 0;	// This bit is used as plane selection
#else
		cmd[1] = 0;
#endif
		cmd[2] = 0;
		ret = spi_flash_cmd_write(flash->spi, cmd, 3, wbuf, bytes);
		if (ret) {
			printf("%s: write command failed\n", __func__);
			goto out;
		}

#ifdef CONFIG_SPI_NAND_GD
		ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WREN, NULL, 0);
		if (ret) {
			printf("Write enable failed\n");
			goto out;
		}
#endif

		cmd[0] = IPQ40XX_SPINAND_CMD_PROG;
		cmd[1] = (u8)(page >> 16);
		cmd[2] = (u8)(page >> 8);
		cmd[3] = (u8)(page);

		ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
		if (ret) {
			printf("PLOG failed\n");
			goto out;
		}

		ret = spinand_waitfunc(mtd, 0x01, &status);
		if (ret) {
			printf("Operation timeout\n");
			goto out;
		}

		if (status & STATUS_P_FAIL) {
			printf("Program failure (0x%x)\n", status);
			ret = -EIO;
			goto out;
		}

		ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WRDI, NULL, 0);
		if (ret) {
			printf("Write disable failed\n");
			goto out;
		}

		write_len -= bytes;
		if (!write_len)
			break;
		buf += bytes;
		realpage++;
		page = realpage & 0xfffff;

	}

out:
	spi_release_bus(flash->spi);
	return ret;
}

int spi_nand_write_raw(struct spi_flash *flash, u32 offset, size_t len, const void *buf)
{
	u8 cmd[8];
	u8 status;
	u32 ret;
	const u_char *wbuf;
	int page, bytes, write_len;

	write_len = len;
	bytes = flash->page_size;

	/* Check whether page is aligned */
	if (offset % flash->page_size || len % flash->page_size) {
		printf("Attempt to write to non page aligned data\n");
		return -1;
	}

	page = offset / flash->page_size;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Claim bus failed. %s\n", __func__);
		return -1;
	}
	while (1) {
		wbuf = buf;

#ifndef CONFIG_SPI_NAND_GD
		ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WREN, NULL, 0);
		if (ret) {
			printf("Write enable failed\n");
			goto out;
		}
#endif

		ret = spi_nand_flash_cmd_wait_ready(flash, 0x01, &status, TIMEOUT);
		if (ret) {
			printf("%s Operation Timeout\n", __func__);
			goto out;
		}

		/* buffer to be transmitted here */
		cmd[0] = IPQ40XX_SPINAND_CMD_PLOAD;
#ifdef CONFIG_SPI_NAND_MICRON
		cmd[1] = page & (1 << 6) ? (1 << 4) : 0;	// This bit is used as plane selection
#else
		cmd[1] = 0;
#endif
		cmd[2] = 0;
		ret = spi_flash_cmd_write(flash->spi, cmd, 3, wbuf, bytes);
		if (ret) {
			printf("%s: write command failed\n", __func__);
			goto out;
		}

#ifdef CONFIG_SPI_NAND_GD
		ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WREN, NULL, 0);
		if (ret) {
			printf("Write enable failed\n");
			goto out;
		}
#endif

		cmd[0] = IPQ40XX_SPINAND_CMD_PROG;
		cmd[1] = (u8)(page >> 16);
		cmd[2] = (u8)(page >> 8);
		cmd[3] = (u8)(page);

		ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
		if (ret) {
			printf("PLOG failed\n");
			goto out;
		}

		ret = spi_nand_flash_cmd_wait_ready(flash, 0x01, &status, TIMEOUT);
		if (ret) {
			printf("Operation timeout\n");
			goto out;
		}

		if (status & STATUS_P_FAIL) {
			printf("Program failure (0x%x)\n", status);
			ret = -EIO;
			goto out;
		}

		ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WRDI, NULL, 0);
		if (ret) {
			printf("Write disable failed\n");
			goto out;
		}

		write_len -= bytes;
		if (!write_len)
			break;
		buf += bytes;
		page++;

	}

out:
	spi_release_bus(flash->spi);
	return ret;

}

static void spinand_set_quad(struct spi_flash *flash, int enable)
{
#if defined(CONFIG_SPI_NAND_XTX) || defined(CONFIG_SPI_NAND_MACRONIX) || defined(CONFIG_SPI_NAND_MK) || defined(CONFIG_SPI_NAND_ATO) || defined(CONFIG_SPI_NAND_GD)
	u8 status;
	int ret;
	u8 cmd[3];

	cmd[0] = IPQ40XX_SPINAND_CMD_GETFEA;
	cmd[1] = IPQ40XX_SPINAND_FEATURE_REG;

	ret = spi_flash_cmd_read(flash->spi, cmd, 2, &status, 1);
	if (ret) {
		printf("%s: read data failed\n", __func__);
		goto out;
	}

	cmd[0] = IPQ40XX_SPINAND_CMD_SETFEA;
	cmd[1] = IPQ40XX_SPINAND_FEATURE_REG;
	if (enable) {
		cmd[2] = status | IPQ40XX_SPINAND_FEATURE_QE;
	} else {
		cmd[2] = status & ~(IPQ40XX_SPINAND_FEATURE_QE);
	}

	ret = spi_flash_cmd_write(flash->spi, cmd, 3, NULL, 0);
	if (ret) {
		printf("Quad mode enable failed\n");
	}

out:
	return;
#endif
}


int spi_nand_read_raw(struct spi_flash *flash, u32 offset, size_t len, void *data)
{
	u32 ret;
	u8 cmd[8];
	u8 status;
	int page, readlen, bytes, quad = 0;
	int ecc_corrected = 0;
	struct spi_slave *spi = flash->spi;

	readlen = len;
	bytes = flash->page_size;

	/* Check whether page is aligned */
	if (offset % flash->page_size) {
		debug("Read offset not multiple of page size\n");
		return -1;
	}

	page = offset / flash->page_size;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Claim bus failed. %s\n", __func__);
		return -1;
	}

	if (flash->read_cmd == CMD_READ_QUAD_OUTPUT_FAST) {
		quad = 1;
	}

	while (1) {
		cmd[0] = IPQ40XX_SPINAND_CMD_READ;
		cmd[1] = (u8)(page >> 16);
		cmd[2] = (u8)(page >> 8);
		cmd[3] = (u8)(page);
		ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
		if (ret) {
			printf("%s: write command failed\n", __func__);
			goto out;
		}
		ret = spi_nand_flash_cmd_wait_ready(flash, 0x01, &status, TIMEOUT);
		if (ret) {
			printf("Operation timeout\n");
			goto out;
		}

#ifdef CONFIG_SPI_NAND_GD
		ret = verify_3bit_ecc(status);
#else
		ret = verify_2bit_ecc(status);
#endif

		if (ret == ECC_ERR) {
			printf("ecc err(0x%x) for page read\n", status);
			ret = -EBADMSG;
			goto out;
		} else if (ret == ECC_CORRECTED) {
			ecc_corrected = 1;
		}

		bytes = ((readlen < flash->page_size) ? readlen : flash->page_size);

		/* Read Data only, ignore OOB */
		if (bytes) {
			cmd[0] = quad == 0 ? IPQ40XX_SPINAND_CMD_NORM_READ : IPQ40XX_SPINAND_CMD_FAST_READ_QUAD;
#ifdef CONFIG_SPI_NAND_MICRON
			cmd[1] = page & (1 << 6) ? (1 << 4) : 0;	// This bit is used as plane selection
#else
			cmd[1] = 0;
#endif
			cmd[2] = 0;
			cmd[3] = 0;
#ifdef CONFIG_SPI_NAND_GD
			if (cmd[0] == IPQ40XX_SPINAND_CMD_FAST_READ_QUAD)
				cmd[4] = 0;
#endif
			spi->quad_enable = quad;

			if (quad)
				spinand_set_quad(flash, 1);

			ret = spi_flash_cmd_read(flash->spi, cmd, 4, data, bytes);
			spi->quad_enable = 0;

			if (quad)
				spinand_set_quad(flash, 0);

			if (ret) {
				printf("%s: read data failed\n", __func__);
				return -1;
			}
		}

		readlen -= bytes;
		if (readlen <= 0)
			break;
		data += bytes;
		page++;
	}

	if ((ret == 0) && (ecc_corrected))
		ret = -EUCLEAN;  //--
out:
	spi_release_bus(flash->spi);
	return ret;
}

int spi_nand_erase_raw(struct spi_flash *flash, u32 offset, size_t len)
{
	u32 start, end, page;
	int ret;
	u8 cmd[5], status;
	printf("erase\n");
	page = (int)(offset >> (ffs(flash->page_size) - 1));

	if (offset % flash->sector_size || len % flash->sector_size) {
		printf("SF: Erase offset/length not multiple of erase size\n");
		return -1;
	}

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf("SF: Unable to claim SPI bus\n");
		return ret;
	}

	ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WREN, NULL, 0);
	if (ret) {
		printf ("Write enable failed %s\n", __func__);
		goto out;
	}
	ret = spi_nand_flash_cmd_wait_ready(flash, 0x01, &status, TIMEOUT);
	if (ret) {
		printf("%s Operation Timeout\n", __func__);
		goto out;
	}

	start = offset;
	end = start + len;

	cmd[0] = IPQ40XX_SPINAND_CMD_ERASE;

	while (offset < end) {

		cmd[1] = (u8)(page >> 16);
		cmd[2] = (u8)(page >> 8);
		cmd[3] = (u8)(page);
		offset += flash->sector_size;
		page += 64;  // 64 page per block...
		//printf("SF: erase %2x %2x %2x %2x %2x (%x)\n", cmd[0], cmd[1],
		//		cmd[2], cmd[3], cmd[4], offset);

		ret = spi_flash_cmd_write(flash->spi, cmd, 4, NULL, 0);
		if (ret)
			goto out;

		ret = spi_nand_flash_cmd_wait_ready(flash, 0x01, &status, SPI_FLASH_PAGE_ERASE_TIMEOUT);
		if (ret) {
			printf("Operation timeout\n");
			goto out;
		}

		if (status & STATUS_E_FAIL) {
			printf("Erase operation failed for 0x%x\n", page);
			ret = -EIO;
			goto out;
		}
	}

	ret = spi_flash_cmd(flash->spi, IPQ40XX_SPINAND_CMD_WRDI, NULL, 0);

	ret = spi_nand_flash_cmd_wait_ready(flash, 0x01, &status, TIMEOUT);
	if (ret) {
		printf("Write disable failed\n");
	}

out:
	spi_release_bus(flash->spi);
	return ret;
}


void spinand_enable_internal_ecc(struct spi_flash *flash)
{
	int ret;
	u8 cmd[3];
	u8 reg;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Write enable failed %s %d\n", __func__, __LINE__);
		return;
	}

	cmd[0] = IPQ40XX_SPINAND_CMD_GETFEA;
	cmd[1] = IPQ40XX_SPINAND_FEATURE_REG;

	ret = spi_flash_cmd_read(flash->spi, cmd, 2, &reg, 1);
	if (ret) {
		printf("%s: read data failed\n", __func__);
		goto out;
	}

	cmd[0] = IPQ40XX_SPINAND_CMD_SETFEA;
	cmd[1] = IPQ40XX_SPINAND_FEATURE_REG;
	cmd[2] = reg | IPQ40XX_SPINAND_FEATURE_ECC_EN;

	ret = spi_flash_cmd_write(flash->spi, cmd, 3, NULL, 0);
	if (ret) {
		printf("Internal ECC enable failed\n");
	}

	cmd[0] = IPQ40XX_SPINAND_CMD_GETFEA;
	cmd[1] = IPQ40XX_SPINAND_PROTEC_REG;

	ret = spi_flash_cmd_read(flash->spi, cmd, 2, &reg, 1);
	if (ret) {
		printf("Failed to read protect register");
		goto out;
	}

	cmd[0] = IPQ40XX_SPINAND_CMD_SETFEA;
	cmd[1] = IPQ40XX_SPINAND_PROTEC_REG;
	cmd[2] = reg & 0x87;
	ret = spi_flash_cmd_write(flash->spi, cmd, 3, NULL, 0);
	if (ret) {
		printf("Failed to unblock sectors #0\n");
	}

out:
	spi_release_bus(flash->spi);
	return;
}
static int spinand_unlock_protect(struct mtd_info *mtd)
{
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;
	u8 status;
	int ret;
	u8 cmd[3];

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Write enable failed %s %d\n", __func__, __LINE__);
		return -1;
	}

#ifdef CONFIG_WINBOND_MULTIDIE
	cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
	cmd[1] = 0x00;
	ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
	if (ret) {
		printf("%s  failed for die select :\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}
#endif

	cmd[0] = IPQ40XX_SPINAND_CMD_GETFEA;
	cmd[1] = IPQ40XX_SPINAND_PROTEC_REG;

	ret = spi_flash_cmd_read(flash->spi, cmd, 2, &status, 1);
	if (ret) {
		printf("Failed to read status register");
		goto out;
	}


	status &= (info->params->protec_bpx);
	cmd[0] = IPQ40XX_SPINAND_CMD_SETFEA;
	cmd[1] = IPQ40XX_SPINAND_PROTEC_REG;
	cmd[2] = (u8)status;
	ret = spi_flash_cmd_write(flash->spi, cmd, 3, NULL, 0);
	if (ret) {
		printf("Failed to unblock sectors #0\n");
	}

#ifdef CONFIG_WINBOND_MULTIDIE
	cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
	cmd[1] = 0x01;
	ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
	if (ret) {
		printf("%s  failed for die select :\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}
#endif

	status &= (info->params->protec_bpx);
	cmd[0] = IPQ40XX_SPINAND_CMD_SETFEA;
	cmd[1] = IPQ40XX_SPINAND_PROTEC_REG;
	cmd[2] = (u8)status;
	ret = spi_flash_cmd_write(flash->spi, cmd, 3, NULL, 0);
	if (ret) {
		printf("Failed to unblock sectors #0\n");
	}

out:
	spi_release_bus(flash->spi);
	return ret;
}

void spinand_internal_ecc(struct mtd_info *mtd, int enable)
{
	struct ipq40xx_spinand_info *info = mtd_to_ipq_info(mtd);
	struct spi_flash *flash = info->flash;
	u8 status;
	int ret;
	u8 cmd[3];

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		printf ("Write enable failed %s %d\n", __func__, __LINE__);
		return;
	}

#ifdef CONFIG_WINBOND_MULTIDIE
	cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
	cmd[1] = 0x00;
	ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
	if (ret) {
		printf("%s  failed for die select :\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}
#endif

	cmd[0] = IPQ40XX_SPINAND_CMD_GETFEA;
	cmd[1] = IPQ40XX_SPINAND_FEATURE_REG;

	ret = spi_flash_cmd_read(flash->spi, cmd, 2, &status, 1);
	if (ret) {
		printf("%s: read data failed\n", __func__);
		goto out;
	}



	cmd[0] = IPQ40XX_SPINAND_CMD_SETFEA;
	cmd[1] = IPQ40XX_SPINAND_FEATURE_REG;
	if (enable) {
		cmd[2] = status | IPQ40XX_SPINAND_FEATURE_ECC_EN;
	} else {
		cmd[2] = status & ~(IPQ40XX_SPINAND_FEATURE_ECC_EN);
	}

	ret = spi_flash_cmd_write(flash->spi, cmd, 3, NULL, 0);
	if (ret) {
		printf("Internal ECC enable failed\n");
	}

#ifdef CONFIG_WINBOND_MULTIDIE
	cmd[0] = IPQ40XX_SPINAND_CMD_DIESELECT;
	cmd[1] = 0x01;
	ret = spi_flash_cmd_write(flash->spi, cmd, 2, NULL, 0);
	if (ret) {
		printf("%s  failed for die select :\n", __func__);
		goto out;
	}
	ret = spinand_waitfunc(mtd, 0x01, &status);
	if (ret) {
		printf("Operation timeout\n");
		goto out;
	}
#endif

	cmd[0] = IPQ40XX_SPINAND_CMD_GETFEA;
	cmd[1] = IPQ40XX_SPINAND_FEATURE_REG;

	ret = spi_flash_cmd_read(flash->spi, cmd, 2, &status, 1);
	if (ret) {
		printf("%s: read data failed\n", __func__);
		goto out;
	}

	//printf("####debug die 1 SR: %X,",status);

	cmd[0] = IPQ40XX_SPINAND_CMD_SETFEA;
	cmd[1] = IPQ40XX_SPINAND_FEATURE_REG;
	if (enable) {
		cmd[2] = status | IPQ40XX_SPINAND_FEATURE_ECC_EN;
	} else {
		cmd[2] = status & ~(IPQ40XX_SPINAND_FEATURE_ECC_EN);
	}
	//printf(" SRafter:%X \n", cmd[2]);
	ret = spi_flash_cmd_write(flash->spi, cmd, 3, NULL, 0);
	if (ret) {
		printf("Internal ECC enable failed\n");
	}

out:
	spi_release_bus(flash->spi);
	return;
}

static int spi_nand_scan_bbt_nop(struct mtd_info *mtd)
{
	return 0;
}

int spi_nand_init(void)
{
	struct spi_flash *flash;
	struct mtd_info *mtd;
	struct nand_chip *chip;
	struct ipq40xx_spinand_info *info;
	int ret;

	info = (struct ipq40xx_spinand_info *)malloc(
	           sizeof(struct ipq40xx_spinand_info));
	if (!info) {
		printf ("Error in allocating mem\n");
		return -ENOMEM;
	}
	memset(info, '0', sizeof(struct ipq40xx_spinand_info));

	flash = spi_flash_probe(0, 0, 30000000, 3);

	if (!flash) {
		free(info);
		spi_print("Id could not be mapped\n");
		return -ENODEV;
	}

	//mtd = nand_info[0];
	mtd = (struct mtd_info *)malloc(
	          sizeof(struct mtd_info));
	if (!mtd) {
		printf ("Error in allocating mem\n");
		return -ENOMEM;
	}
	memset(info, '0', sizeof(struct ipq40xx_spinand_info));
	chip = &nand_chip[0];

	mtd->priv = chip;
	mtd->writesize = flash->page_size;
	mtd->erasesize = spi_nand_flash_tbl[0].erase_size;
	mtd->oobsize = spi_nand_flash_tbl[0].oob_size;
	mtd->size = flash->size;
	mtd->type = MTD_NANDFLASH;
	mtd->flags = MTD_CAP_NANDFLASH;
#ifndef __UBOOT__
	mtd->_point = NULL;
	mtd->_unpoint = NULL;
#endif
	mtd->_read = spi_nand_read;
	mtd->_write = spi_nand_write;
	mtd->_erase = spi_nand_erase;
	mtd->_read_oob = spi_nand_read_oob;
	mtd->_write_oob = spi_nand_write_oob;
	mtd->_block_isbad = spi_nand_block_isbad;
	mtd->_block_markbad = spi_nand_block_markbad;

	chip->page_shift = ffs(mtd->writesize) - 1;
	chip->phys_erase_shift = ffs(mtd->erasesize) - 1;
	chip->chipsize = flash->size;
	chip->pagemask = (chip->chipsize >> chip->page_shift) - 1;
	chip->badblockpos = 0;
	chip->buffers = kmalloc(sizeof(*chip->buffers), GFP_KERNEL);
	chip->oob_poi = chip->buffers->databuf + mtd->writesize;

	/* One of the NAND layer functions that the command layer
	 * tries to access directly.
	 */
	chip->scan_bbt = spi_nand_scan_bbt_nop;

	// an ugly hack. cannot register our own probe function to set params, so assign correct one here.
	info->params = &spi_nand_flash_tbl[0];//params;
	info->flash = flash;
	info->mtd = mtd;
	info->chip = chip;
	chip->priv = info;

	if ((ret = nand_register(/*CONFIG_IPQ_SPI_NAND_INFO_IDX*/0, mtd)) < 0) {
		free(info);
		spi_print("Failed to register with MTD subsystem\n");
		return ret;
	}

	ret = spinand_unlock_protect(mtd);
	if (ret) {
		free(info);
		printf("Failed to unlock blocks\n");
		return -1;
	}

	spinand_internal_ecc(mtd, 1);

	return 0;
}
