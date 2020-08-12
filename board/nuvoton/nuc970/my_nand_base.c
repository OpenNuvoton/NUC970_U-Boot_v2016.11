/*
 *  Overview:
 *   This is the generic MTD driver for NAND flash devices. It should be
 *   capable of working with almost all NAND chips currently available.
 *
 *	Additional technical information is available on
 *	http://www.linux-mtd.infradead.org/doc/nand.html
 *
 *  Copyright (C) 2000 Steven J. Hill (sjhill@realitydiluted.com)
 *		  2002-2006 Thomas Gleixner (tglx@linutronix.de)
 *
 *  Credits:
 *	David Woodhouse for adding multichip support
 *
 *	Aleph One Ltd. and Toby Churchill Ltd. for supporting the
 *	rework for 2K page size chips
 *
 *  TODO:
 *	Enable cached programming for 2k page size chips
 *	Check, if mtd->ecctype should be set to MTD_ECC_HW
 *	if we have HW ECC support.
 *	BBT table is not serialized, has to be fixed
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <common.h>
#if CONFIG_IS_ENABLED(OF_CONTROL)
#include <fdtdec.h>
#endif
#include <malloc.h>
#include <watchdog.h>
#include <linux/err.h>
#include <linux/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/nand_bch.h>
#ifdef CONFIG_MTD_PARTITIONS
#include <linux/mtd/partitions.h>
#endif
#include <asm/io.h>
#include <linux/errno.h>

extern void sysprintf(char* pcStr,...);
#define printf(fmt, arg...) sysprintf(fmt, ##arg) //CWWeng add

struct nand_buffers g_nand_buffers;
struct nand_buffers *nand_buffers;

/* Define default oob placement schemes for large and small page devices */
static struct nand_ecclayout nand_oob_xxx = {  //CWWeng 2017.2.22 for shrink code size, original is nand_oob_8/16/64/128
	.eccbytes = 3,
	.eccpos = {0, 1, 2},
	.oobfree = {
		{.offset = 3,
		 .length = 2},
		{.offset = 6,
		 .length = 2} }
};


/**
 * nand_read_byte - [DEFAULT] read one byte from the chip
 * @mtd: MTD device structure
 *
 * Default read function for 8bit buswidth
 */
uint8_t nand_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	return readb(chip->IO_ADDR_R);
}

/**
 * nand_read_byte16 - [DEFAULT] read one byte endianness aware from the chip
 * @mtd: MTD device structure
 *
 * Default read function for 16bit buswidth with endianness conversion.
 *
 */
static uint8_t nand_read_byte16(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	return (uint8_t) cpu_to_le16(readw(chip->IO_ADDR_R));
}

/**
 * nand_read_word - [DEFAULT] read one word from the chip
 * @mtd: MTD device structure
 *
 * Default read function for 16bit buswidth without endianness conversion.
 */
static u16 nand_read_word(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	return readw(chip->IO_ADDR_R);
}

/**
 * nand_select_chip - [DEFAULT] control CE line
 * @mtd: MTD device structure
 * @chipnr: chipnumber to select, -1 for deselect
 *
 * Default select function for 1 chip devices.
 */
static void nand_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

	switch (chipnr) {
	case -1:
		chip->cmd_ctrl(mtd, NAND_CMD_NONE, 0 | NAND_CTRL_CHANGE);
		break;
	case 0:
		break;

	default:
		/*BUG();*/
		while(1);
	}
}

#if !defined(CONFIG_BLACKFIN)
static void ioread8_rep(void *addr, uint8_t *buf, int len)
{
	int i;

	for (i = 0; i < len; i++)
		buf[i] = readb(addr);
}

static void ioread16_rep(void *addr, void *buf, int len)
{
	int i;
 	u16 *p = (u16 *) buf;

	for (i = 0; i < len; i++)
		p[i] = readw(addr);
}

#endif

/**
 * nand_read_buf - [DEFAULT] read chip data into buffer
 * @mtd: MTD device structure
 * @buf: buffer to store date
 * @len: number of bytes to read
 *
 * Default read function for 8bit buswidth.
 */
void nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

	ioread8_rep(chip->IO_ADDR_R, buf, len);
}

/**
 * nand_read_buf16 - [DEFAULT] read chip data into buffer
 * @mtd: MTD device structure
 * @buf: buffer to store date
 * @len: number of bytes to read
 *
 * Default read function for 16bit buswidth.
 */
void nand_read_buf16(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	u16 *p = (u16 *) buf;

	ioread16_rep(chip->IO_ADDR_R, p, len >> 1);
}

#define BITS_PER_BYTE 8

/**
 * nand_read_page_hwecc - [REPLACEABLE] hardware ECC based page read function
 * @mtd: mtd info structure
 * @chip: nand chip info structure
 * @buf: buffer to store read data
 * @oob_required: caller requires OOB data read to chip->oob_poi
 * @page: page number to read
 *
 * Not for syndrome calculating ECC controllers which need a special oob layout.
 */
static int nand_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf, int oob_required, int page)
{
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	uint8_t *p = buf;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	uint8_t *ecc_code = chip->buffers->ecccode;
	uint32_t *eccpos = chip->ecc.layout->eccpos;
	unsigned int max_bitflips = 0;

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		chip->read_buf(mtd, p, eccsize);
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);
	}
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	for (i = 0; i < chip->ecc.total; i++)
		ecc_code[i] = chip->oob_poi[eccpos[i]];

	eccsteps = chip->ecc.steps;
	p = buf;

	for (i = 0 ; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		int stat;

		stat = chip->ecc.correct(mtd, p, &ecc_code[i], &ecc_calc[i]);
		if (stat == -EBADMSG &&
		    (chip->ecc.options & NAND_ECC_GENERIC_ERASED_CHECK)) {
			/* check for empty pages with bitflips */
/*
			stat = nand_check_erased_ecc_chunk(p, eccsize,
						&ecc_code[i], eccbytes,
						NULL, 0,
						chip->ecc.strength);
*/
		}

		if (stat < 0) {
			mtd->ecc_stats.failed++;
		} else {
			mtd->ecc_stats.corrected += stat;
			max_bitflips = max_t(unsigned int, max_bitflips, stat);
		}
	}
	return max_bitflips;
}

/**
 * nand_read_page_hwecc_oob_first - [REPLACEABLE] hw ecc, read oob first
 * @mtd: mtd info structure
 * @chip: nand chip info structure
 * @buf: buffer to store read data
 * @oob_required: caller requires OOB data read to chip->oob_poi
 * @page: page number to read
 *
 * Hardware ECC for large page chips, require OOB to be read first. For this
 * ECC mode, the write_page method is re-used from ECC_HW. These methods
 * read/write ECC from the OOB area, unlike the ECC_HW_SYNDROME support with
 * multiple ECC steps, follows the "infix ECC" scheme and reads/writes ECC from
 * the data area, by overwriting the NAND manufacturer bad block markings.
 */
static int nand_read_page_hwecc_oob_first(struct mtd_info *mtd,
	struct nand_chip *chip, uint8_t *buf, int oob_required, int page)
{
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	uint8_t *p = buf;
	uint8_t *ecc_code = chip->buffers->ecccode;
	uint32_t *eccpos = chip->ecc.layout->eccpos;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	unsigned int max_bitflips = 0;

	/* Read the OOB area first */
	chip->cmdfunc(mtd, NAND_CMD_READOOB, 0, page);
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
	chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);

	for (i = 0; i < chip->ecc.total; i++)
		ecc_code[i] = chip->oob_poi[eccpos[i]];

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		int stat;

		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		chip->read_buf(mtd, p, eccsize);
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);

		stat = chip->ecc.correct(mtd, p, &ecc_code[i], NULL);
		if (stat == -EBADMSG &&
		    (chip->ecc.options & NAND_ECC_GENERIC_ERASED_CHECK)) {
			/* check for empty pages with bitflips */
/*
			stat = nand_check_erased_ecc_chunk(p, eccsize,
						&ecc_code[i], eccbytes,
						NULL, 0,
						chip->ecc.strength);
*/
		}

		if (stat < 0) {
			mtd->ecc_stats.failed++;
		} else {
			mtd->ecc_stats.corrected += stat;
			max_bitflips = max_t(unsigned int, max_bitflips, stat);
		}
	}
	return max_bitflips;
}


#define NOTALIGNED(x)	((x & (chip->subpagesize - 1)) != 0)


/* Set default functions */
static void nand_set_defaults(struct nand_chip *chip, int busw)
{
	/* check for proper chip_delay setup, set 20us if not */
	if (!chip->chip_delay)
		chip->chip_delay = 20;

	if (!chip->select_chip)
		chip->select_chip = nand_select_chip;


	/* If called twice, pointers that depend on busw may need to be reset */
	if (!chip->read_byte || chip->read_byte == nand_read_byte)
		chip->read_byte = busw ? nand_read_byte16 : nand_read_byte;
	if (!chip->read_word)
		chip->read_word = nand_read_word;
	if (!chip->read_buf || chip->read_buf == nand_read_buf)
		chip->read_buf = busw ? nand_read_buf16 : nand_read_buf;

	if (!chip->controller) {
		chip->controller = &chip->hwcontrol;
		spin_lock_init(&chip->controller->lock);
		init_waitqueue_head(&chip->controller->wq);
	}

}

/*
 * nand_id_has_period - Check if an ID string has a given wraparound period
 * @id_data: the ID string
 * @arrlen: the length of the @id_data array
 * @period: the period of repitition
 *
 * Check if an ID string is repeated within a given sequence of bytes at
 * specific repetition interval period (e.g., {0x20,0x01,0x7F,0x20} has a
 * period of 3). This is a helper function for nand_id_len(). Returns non-zero
 * if the repetition has a period of @period; otherwise, returns zero.
 */
static int nand_id_has_period(u8 *id_data, int arrlen, int period)
{
	int i, j;
	for (i = 0; i < period; i++)
		for (j = i + period; j < arrlen; j += period)
			if (id_data[i] != id_data[j])
				return 0;
	return 1;
}

/*
 * nand_id_len - Get the length of an ID string returned by CMD_READID
 * @id_data: the ID string
 * @arrlen: the length of the @id_data array

 * Returns the length of the ID string, according to known wraparound/trailing
 * zero patterns. If no pattern exists, returns the length of the array.
 */
static int nand_id_len(u8 *id_data, int arrlen)
{
	int last_nonzero, period;

	/* Find last non-zero byte */
	for (last_nonzero = arrlen - 1; last_nonzero >= 0; last_nonzero--)
		if (id_data[last_nonzero])
			break;

	/* All zeros */
	if (last_nonzero < 0)
		return 0;

	/* Calculate wraparound period */
	for (period = 1; period < arrlen; period++)
		if (nand_id_has_period(id_data, arrlen, period))
			break;

	/* There's a repeated pattern */
	if (period < arrlen)
		return period;

	/* There are trailing zeros */
	if (last_nonzero < arrlen - 1)
		return last_nonzero + 1;

	/* No pattern detected */
	return arrlen;
}

/* Extract the bits of per cell from the 3rd byte of the extended ID */
static int nand_get_bits_per_cell(u8 cellinfo)
{
	int bits;

	bits = cellinfo & NAND_CI_CELLTYPE_MSK;
	bits >>= NAND_CI_CELLTYPE_SHIFT;
	return bits + 1;
}

/*
 * Many new NAND share similar device ID codes, which represent the size of the
 * chip. The rest of the parameters must be decoded according to generic or
 * manufacturer-specific "extended ID" decoding patterns.
 */
static void nand_decode_ext_id(struct mtd_info *mtd, struct nand_chip *chip,
				u8 id_data[8], int *busw)
{
	int extid, id_len;
	/* The 3rd id byte holds MLC / multichip data */
	chip->bits_per_cell = nand_get_bits_per_cell(id_data[2]);
	/* The 4th id byte is the important one */
	extid = id_data[3];

	id_len = nand_id_len(id_data, 8);

	/*
	 * Field definitions are in the following datasheets:
	 * Old style (4,5 byte ID): Samsung K9GAG08U0M (p.32)
	 * New Samsung (6 byte ID): Samsung K9GAG08U0F (p.44)
	 * Hynix MLC   (6 byte ID): Hynix H27UBG8T2B (p.22)
	 *
	 * Check for ID length, non-zero 6th byte, cell type, and Hynix/Samsung
	 * ID to decide what to do.
	 */
	if (id_len == 6 && id_data[0] == NAND_MFR_SAMSUNG &&
			!nand_is_slc(chip) && id_data[5] != 0x00) {
		/* Calc pagesize */
		mtd->writesize = 2048 << (extid & 0x03);
		extid >>= 2;
		/* Calc oobsize */
		switch (((extid >> 2) & 0x04) | (extid & 0x03)) {
		case 1:
			mtd->oobsize = 128;
			break;
		case 2:
			mtd->oobsize = 218;
			break;
		case 3:
			mtd->oobsize = 400;
			break;
		case 4:
			mtd->oobsize = 436;
			break;
		case 5:
			mtd->oobsize = 512;
			break;
		case 6:
			mtd->oobsize = 640;
			break;
		case 7:
		default: /* Other cases are "reserved" (unknown) */
			mtd->oobsize = 1024;
			break;
		}
		extid >>= 2;
		/* Calc blocksize */
		mtd->erasesize = (128 * 1024) <<
			(((extid >> 1) & 0x04) | (extid & 0x03));
		*busw = 0;
	} else if (id_len == 6 && id_data[0] == NAND_MFR_HYNIX &&
			!nand_is_slc(chip)) {
		unsigned int tmp;

		/* Calc pagesize */
		mtd->writesize = 2048 << (extid & 0x03);
		extid >>= 2;
		/* Calc oobsize */
		switch (((extid >> 2) & 0x04) | (extid & 0x03)) {
		case 0:
			mtd->oobsize = 128;
			break;
		case 1:
			mtd->oobsize = 224;
			break;
		case 2:
			mtd->oobsize = 448;
			break;
		case 3:
			mtd->oobsize = 64;
			break;
		case 4:
			mtd->oobsize = 32;
			break;
		case 5:
			mtd->oobsize = 16;
			break;
		default:
			mtd->oobsize = 640;
			break;
		}
		extid >>= 2;
		/* Calc blocksize */
		tmp = ((extid >> 1) & 0x04) | (extid & 0x03);
		if (tmp < 0x03)
			mtd->erasesize = (128 * 1024) << tmp;
		else if (tmp == 0x03)
			mtd->erasesize = 768 * 1024;
		else
			mtd->erasesize = (64 * 1024) << tmp;
		*busw = 0;
	} else {
		/* Calc pagesize */
		mtd->writesize = 1024 << (extid & 0x03);
		extid >>= 2;
		/* Calc oobsize */
		mtd->oobsize = (8 << (extid & 0x01)) *
			(mtd->writesize >> 9);
		extid >>= 2;
		/* Calc blocksize. Blocksize is multiples of 64KiB */
		mtd->erasesize = (64 * 1024) << (extid & 0x03);
		extid >>= 2;
		/* Get buswidth information */
		*busw = (extid & 0x01) ? NAND_BUSWIDTH_16 : 0;

		/*
		 * Toshiba 24nm raw SLC (i.e., not BENAND) have 32B OOB per
		 * 512B page. For Toshiba SLC, we decode the 5th/6th byte as
		 * follows:
		 * - ID byte 6, bits[2:0]: 100b -> 43nm, 101b -> 32nm,
		 *                         110b -> 24nm
		 * - ID byte 5, bit[7]:    1 -> BENAND, 0 -> raw SLC
		 */
		if (id_len >= 6 && id_data[0] == NAND_MFR_TOSHIBA &&
				nand_is_slc(chip) &&
				(id_data[5] & 0x7) == 0x6 /* 24nm */ &&
				!(id_data[4] & 0x80) /* !BENAND */) {
			mtd->oobsize = 32 * mtd->writesize >> 9;
		}

	}
}

/*
 * Old devices have chip data hardcoded in the device ID table. nand_decode_id
 * decodes a matching ID table entry and assigns the MTD size parameters for
 * the chip.
 */
static void nand_decode_id(struct mtd_info *mtd, struct nand_chip *chip,
				struct nand_flash_dev *type, u8 id_data[8],
				int *busw)
{
	int maf_id = id_data[0];

	mtd->erasesize = type->erasesize;
	mtd->writesize = type->pagesize;
	mtd->oobsize = mtd->writesize / 32;
	*busw = type->options & NAND_BUSWIDTH_16;

	/* All legacy ID NAND are small-page, SLC */
	chip->bits_per_cell = 1;

	/*
	 * Check for Spansion/AMD ID + repeating 5th, 6th byte since
	 * some Spansion chips have erasesize that conflicts with size
	 * listed in nand_ids table.
	 * Data sheet (5 byte ID): Spansion S30ML-P ORNAND (p.39)
	 */
	if (maf_id == NAND_MFR_AMD && id_data[4] != 0x00 && id_data[5] == 0x00
			&& id_data[6] == 0x00 && id_data[7] == 0x00
			&& mtd->writesize == 512) {
		mtd->erasesize = 128 * 1024;
		mtd->erasesize <<= ((id_data[3] & 0x03) << 1);
	}
}

static inline bool is_full_id_nand(struct nand_flash_dev *type)
{
	return type->id_len;
}

static bool find_full_id_nand(struct mtd_info *mtd, struct nand_chip *chip,
		   struct nand_flash_dev *type, u8 *id_data, int *busw)
{
	if (!strncmp((char *)type->id, (char *)id_data, type->id_len)) {
		mtd->writesize = type->pagesize;
		mtd->erasesize = type->erasesize;
		mtd->oobsize = type->oobsize;

		chip->bits_per_cell = nand_get_bits_per_cell(id_data[2]);
		chip->chipsize = (uint64_t)type->chipsize << 20;
		chip->options |= type->options;
		chip->ecc_strength_ds = NAND_ECC_STRENGTH(type);
		chip->ecc_step_ds = NAND_ECC_STEP(type);
		chip->onfi_timing_mode_default =
					type->onfi_timing_mode_default;

		*busw = type->options & NAND_BUSWIDTH_16;

		if (!mtd->name)
			mtd->name = type->name;

		return true;
	}
	return false;
}

/*
 * Get the flash and manufacturer id and lookup if the type is supported.
 */
static struct nand_flash_dev *nand_get_flash_type(struct mtd_info *mtd,
						  struct nand_chip *chip,
						  int *maf_id, int *dev_id,
						  struct nand_flash_dev *type)
{
	int busw;
	int i, maf_idx;
	u8 id_data[8];

	/* Select the device */
	chip->select_chip(mtd, 0);

	/*
	 * Reset the chip, required by some chips (e.g. Micron MT29FxGxxxxx)
	 * after power-up.
	 */
	chip->cmdfunc(mtd, NAND_CMD_RESET, -1, -1);

	/* Send the command for reading device ID */
	chip->cmdfunc(mtd, NAND_CMD_READID, 0x00, -1);

	/* Read manufacturer and device IDs */
	*maf_id = chip->read_byte(mtd);
	*dev_id = chip->read_byte(mtd);

	/*
	 * Try again to make sure, as some systems the bus-hold or other
	 * interface concerns can cause random data which looks like a
	 * possibly credible NAND flash to appear. If the two results do
	 * not match, ignore the device completely.
	 */

	chip->cmdfunc(mtd, NAND_CMD_READID, 0x00, -1);

	/* Read entire ID string */
	for (i = 0; i < 8; i++)
		id_data[i] = chip->read_byte(mtd);

	if (id_data[0] != *maf_id || id_data[1] != *dev_id) {
		pr_info("second ID read did not match %02x,%02x against %02x,%02x\n",
			*maf_id, *dev_id, id_data[0], id_data[1]);
		return ERR_PTR(-ENODEV);
	}

	if (!type)
		type = nand_flash_ids;

	for (; type->name != NULL; type++) {
		if (is_full_id_nand(type)) {
			if (find_full_id_nand(mtd, chip, type, id_data, &busw))
				goto ident_done;
		} else if (*dev_id == type->dev_id) {
			break;
		}
	}

	chip->onfi_version = 0;

	if (!type->name)
		return ERR_PTR(-ENODEV);

	if (!mtd->name)
		mtd->name = type->name;

	chip->chipsize = (uint64_t)type->chipsize << 20;

	if (!type->pagesize) {
		/* Decode parameters from extended ID */
		nand_decode_ext_id(mtd, chip, id_data, &busw);
	} else {
		nand_decode_id(mtd, chip, type, id_data, &busw);
	}
	/* Get chip options */
	chip->options |= type->options;

	/*
	 * Check if chip is not a Samsung device. Do not clear the
	 * options for chips which do not have an extended id.
	 */
	if (*maf_id != NAND_MFR_SAMSUNG && !type->pagesize)
		chip->options &= ~NAND_SAMSUNG_LP_OPTIONS;
ident_done:

	/* Try to identify manufacturer */
	for (maf_idx = 0; nand_manuf_ids[maf_idx].id != 0x0; maf_idx++) {
		if (nand_manuf_ids[maf_idx].id == *maf_id)
			break;
	}

	if (chip->options & NAND_BUSWIDTH_AUTO) {
		WARN_ON(chip->options & NAND_BUSWIDTH_16);
		chip->options |= busw;
		nand_set_defaults(chip, busw);
	} else if (busw != (chip->options & NAND_BUSWIDTH_16)) {
		/*
		 * Check, if buswidth is correct. Hardware drivers should set
		 * chip correct!
		 */
		pr_info("device found, Manufacturer ID: 0x%02x, Chip ID: 0x%02x\n",
			*maf_id, *dev_id);
		pr_info("%s %s\n", nand_manuf_ids[maf_idx].name, mtd->name);
		pr_warn("bus width %d instead %d bit\n",
			   (chip->options & NAND_BUSWIDTH_16) ? 16 : 8,
			   busw ? 16 : 8);
		return ERR_PTR(-EINVAL);
	}


	/* Calculate the address shift from the page size */
	chip->page_shift = ffs(mtd->writesize) - 1;
	/* Convert chipsize to number of pages per chip -1 */
	chip->pagemask = (chip->chipsize >> chip->page_shift) - 1;

	chip->bbt_erase_shift = chip->phys_erase_shift =
		ffs(mtd->erasesize) - 1;
	if (chip->chipsize & 0xffffffff)
		chip->chip_shift = ffs((unsigned)chip->chipsize) - 1;
	else {
		chip->chip_shift = ffs((unsigned)(chip->chipsize >> 32));
		chip->chip_shift += 32 - 1;
	}

	chip->badblockbits = 8;


	pr_info("device found, Manufacturer ID: 0x%02x, Chip ID: 0x%02x\n",
		*maf_id, *dev_id);

#ifdef CONFIG_SYS_NAND_ONFI_DETECTION
	if (chip->onfi_version)
		pr_info("%s %s\n", nand_manuf_ids[maf_idx].name,
				chip->onfi_params.model);
	else if (chip->jedec_version)
		pr_info("%s %s\n", nand_manuf_ids[maf_idx].name,
				chip->jedec_params.model);
	else
		pr_info("%s %s\n", nand_manuf_ids[maf_idx].name,
				type->name);
#else
	if (chip->jedec_version)
		pr_info("%s %s\n", nand_manuf_ids[maf_idx].name,
				chip->jedec_params.model);
	else
		pr_info("%s %s\n", nand_manuf_ids[maf_idx].name,
				type->name);

	pr_info("%s %s\n", nand_manuf_ids[maf_idx].name,
		type->name);
#endif

	pr_info("%d MiB, %s, erase size: %d KiB, page size: %d, OOB size: %d\n",
		(int)(chip->chipsize >> 20), nand_is_slc(chip) ? "SLC" : "MLC",
		mtd->erasesize >> 10, mtd->writesize, mtd->oobsize);
	return type;
}

/**
 * nand_scan_ident - [NAND Interface] Scan for the NAND device
 * @mtd: MTD device structure
 * @maxchips: number of chips to scan for
 * @table: alternative NAND ID table
 *
 * This is the first phase of the normal nand_scan() function. It reads the
 * flash ID and sets up MTD fields accordingly.
 *
 */
int nand_scan_ident(struct mtd_info *mtd, int maxchips,
		    struct nand_flash_dev *table)
{
	int i, nand_maf_id, nand_dev_id;
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct nand_flash_dev *type;

	/* Set the default functions */
	nand_set_defaults(chip, chip->options & NAND_BUSWIDTH_16);

	/* Read the flash type */
	type = nand_get_flash_type(mtd, chip, &nand_maf_id,
				   &nand_dev_id, table);

	if (IS_ERR(type)) {
		if (!(chip->options & NAND_SCAN_SILENT_NODEV))
			pr_warn("No NAND device found\n");
		chip->select_chip(mtd, -1);
		return PTR_ERR(type);
	}

	chip->select_chip(mtd, -1);

	/* Check for a chip array */
	for (i = 1; i < maxchips; i++) {
		chip->select_chip(mtd, i);
		/* See comment in nand_get_flash_type for reset */
		chip->cmdfunc(mtd, NAND_CMD_RESET, -1, -1);
		/* Send the command for reading device ID */
		chip->cmdfunc(mtd, NAND_CMD_READID, 0x00, -1);
		/* Read manufacturer and device IDs */
		if (nand_maf_id != chip->read_byte(mtd) ||
		    nand_dev_id != chip->read_byte(mtd)) {
			chip->select_chip(mtd, -1);
			break;
		}
		chip->select_chip(mtd, -1);
	}

#ifdef DEBUG
	if (i > 1)
		pr_info("%d chips detected\n", i);
#endif

	/* Store the number of chips and calc total size for mtd */
	chip->numchips = i;
	mtd->size = i * chip->chipsize;

	return 0;
}
EXPORT_SYMBOL(nand_scan_ident);

/**
 * nand_scan_tail - [NAND Interface] Scan for the NAND device
 * @mtd: MTD device structure
 *
 * This is the second phase of the normal nand_scan() function. It fills out
 * all the uninitialized function pointers with the defaults and scans for a
 * bad block table if appropriate.
 */
int nand_scan_tail(struct mtd_info *mtd)
{
	int i;
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct nand_ecc_ctrl *ecc = &chip->ecc;
	struct nand_buffers *nbuf;

	/* New bad blocks should be marked in OOB, flash-based BBT, or both */
/*
	BUG_ON((chip->bbt_options & NAND_BBT_NO_OOB_BBM) &&
			!(chip->bbt_options & NAND_BBT_USE_FLASH));
*/

	if (!(chip->options & NAND_OWN_BUFFERS)) {
		//nbuf = kzalloc(sizeof(struct nand_buffers), GFP_KERNEL);
		nbuf = &g_nand_buffers; //CWWeng 
    		memset((void*)nbuf,0,sizeof(struct nand_buffers)); //CWWeng
		chip->buffers = nbuf;
	} else {
		if (!chip->buffers)
			return -ENOMEM;
	}

	/* Set the internal oob buffer location, just after the page data */
	chip->oob_poi = chip->buffers->databuf + mtd->writesize;

	/*
	 * If no default placement scheme is given, select an appropriate one.
	 */
	if (!ecc->layout && (ecc->mode != NAND_ECC_SOFT_BCH)) {
		switch (mtd->oobsize) {
		case 8:
			ecc->layout = &nand_oob_xxx;
			break;
		case 16:
			ecc->layout = &nand_oob_xxx;
			break;
		case 64:
			ecc->layout = &nand_oob_xxx;
			break;
		case 128:
			ecc->layout = &nand_oob_xxx;
			break;
		default:
			pr_warn("No oob scheme defined for oobsize %d\n",
				   mtd->oobsize);
			/*BUG();*/while(1);
		}
	}


	/*
	 * Check ECC mode, default to software if 3byte/512byte hardware ECC is
	 * selected and we have 256 byte pagesize fallback to software ECC
	 */

	switch (ecc->mode) {
	case NAND_ECC_HW_OOB_FIRST:
		/* Similar to NAND_ECC_HW, but a separate read_page handle */
		if (!ecc->calculate || !ecc->correct || !ecc->hwctl) {
			pr_warn("No ECC functions supplied; hardware ECC not possible\n");
			/*BUG();*/while(1);
		}
		if (!ecc->read_page)
			ecc->read_page = nand_read_page_hwecc_oob_first;

	case NAND_ECC_HW:
		/* Use standard hwecc read page function? */
		if (!ecc->read_page)
			ecc->read_page = nand_read_page_hwecc;

	case NAND_ECC_SOFT:
		ecc->correct = nand_correct_data;
		if (!ecc->size)
			ecc->size = 256;
		ecc->bytes = 3;
		ecc->strength = 1;
		break;

	case NAND_ECC_SOFT_BCH:
		if (!mtd_nand_has_bch()) {
			pr_warn("CONFIG_MTD_NAND_ECC_BCH not enabled\n");
			/*BUG();*/while(1);
		}
		/*
		 * Board driver should supply ecc.size and ecc.strength values
		 * to select how many bits are correctable. Otherwise, default
		 * to 4 bits for large page devices.
		 */
		if (!ecc->size && (mtd->oobsize >= 64)) {
			ecc->size = 512;
			ecc->strength = 4;
		}

		/* See nand_bch_init() for details. */
		ecc->bytes = 0;
		ecc->priv = nand_bch_init(mtd);
		if (!ecc->priv) {
			pr_warn("BCH ECC initialization failed!\n");
			/*BUG();*/while(1);
		}
		break;

	case NAND_ECC_NONE:
		pr_warn("NAND_ECC_NONE selected by board driver. This is not recommended!\n");
		ecc->size = mtd->writesize;
		ecc->bytes = 0;
		ecc->strength = 0;
		break;

	default:
		pr_warn("Invalid NAND_ECC_MODE %d\n", ecc->mode);
		/*BUG(); */ while(1);
	}

	/* For many systems, the standard OOB write also works for raw */
	if (!ecc->read_oob_raw)
		ecc->read_oob_raw = ecc->read_oob;

	/*
	 * The number of bytes available for a client to place data into
	 * the out of band area.
	 */
	mtd->oobavail = 0;
	if (ecc->layout) {
		for (i = 0; ecc->layout->oobfree[i].length; i++)
			mtd->oobavail += ecc->layout->oobfree[i].length;
	}

	/*
	 * Set the number of read / write steps for one page depending on ECC
	 * mode.
	 */
	ecc->steps = mtd->writesize / ecc->size;
	if (ecc->steps * ecc->size != mtd->writesize) {
		pr_warn("Invalid ECC parameters\n");
		/*BUG(); */while(1);
	}
	ecc->total = ecc->steps * ecc->bytes;

	/* Allow subpage writes up to ecc.steps. Not possible for MLC flash */
	if (!(chip->options & NAND_NO_SUBPAGE_WRITE) && nand_is_slc(chip)) {
		switch (ecc->steps) {
		case 2:
			mtd->subpage_sft = 1;
			break;
		case 4:
		case 8:
		case 16:
			mtd->subpage_sft = 2;
			break;
		}
	}
	chip->subpagesize = mtd->writesize >> mtd->subpage_sft;

	/* Initialize state */
	chip->state = FL_READY;

	/* Invalidate the pagebuffer reference */
	chip->pagebuf = -1;

	/* Large page NAND with SOFT_ECC should support subpage reads */
	switch (ecc->mode) {
	case NAND_ECC_SOFT:
	case NAND_ECC_SOFT_BCH:
		if (chip->page_shift > 9)
			chip->options |= NAND_SUBPAGE_READ;
		break;

	default:
		break;
	}

	/* Fill in remaining MTD driver data */
	mtd->type = nand_is_slc(chip) ? MTD_NANDFLASH : MTD_MLCNANDFLASH;
	mtd->flags = (chip->options & NAND_ROM) ? MTD_CAP_ROM :
						MTD_CAP_NANDFLASH;
	mtd->_lock = NULL;
	mtd->_unlock = NULL;
	mtd->writebufsize = mtd->writesize;

	/* propagate ecc info to mtd_info */
	mtd->ecclayout = ecc->layout;
	mtd->ecc_strength = ecc->strength;
	mtd->ecc_step_size = ecc->size;
	/*
	 * Initialize bitflip_threshold to its default prior scan_bbt() call.
	 * scan_bbt() might invoke mtd_read(), thus bitflip_threshold must be
	 * properly set.
	 */
	if (!mtd->bitflip_threshold)
		mtd->bitflip_threshold = DIV_ROUND_UP(mtd->ecc_strength * 3, 4);

	return 0;
}
EXPORT_SYMBOL(nand_scan_tail);

/**
 * nand_scan - [NAND Interface] Scan for the NAND device
 * @mtd: MTD device structure
 * @maxchips: number of chips to scan for
 *
 * This fills out all the uninitialized function pointers with the defaults.
 * The flash ID is read and the mtd/chip structures are filled with the
 * appropriate values.
 */
int nand_scan(struct mtd_info *mtd, int maxchips)
{
	int ret;

	ret = nand_scan_ident(mtd, maxchips, NULL);
/* CWWeng 2018/1/2
	if (!ret)
		ret = nand_scan_tail(mtd);
*/
	ret = nand_scan_tail(mtd);
	return ret;
}
EXPORT_SYMBOL(nand_scan);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Steven J. Hill <sjhill@realitydiluted.com>");
MODULE_AUTHOR("Thomas Gleixner <tglx@linutronix.de>");
MODULE_DESCRIPTION("Generic NAND flash driver code");
