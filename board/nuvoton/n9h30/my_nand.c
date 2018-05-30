/*
 * (C) Copyright 2005
 * 2N Telekomunikace, a.s. <www.2n.cz>
 * Ladislav Michl <michl@2n.cz>
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#include <common.h>
#include <nand.h>
#include <errno.h>
#include <linux/mtd/concat.h>

#ifndef CONFIG_SYS_NAND_BASE_LIST
#define CONFIG_SYS_NAND_BASE_LIST { CONFIG_SYS_NAND_BASE }
#endif

DECLARE_GLOBAL_DATA_PTR;

int nand_curr_device = -1;


struct mtd_info *nand_info[CONFIG_SYS_MAX_NAND_DEVICE];


static char dev_name[CONFIG_SYS_MAX_NAND_DEVICE][8];

static unsigned long total_nand_size; /* in kiB */

int nand_mtd_to_devnum(struct mtd_info *mtd)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(nand_info); i++) {
		if (mtd && nand_info[i] == mtd)
			return i;
	}

	return -ENODEV;
}

/* Register an initialized NAND mtd device with the U-Boot NAND command. */
int nand_register(int devnum, struct mtd_info *mtd)
{
	if (devnum >= CONFIG_SYS_MAX_NAND_DEVICE)
		return -EINVAL;

	nand_info[devnum] = mtd;

/* CWWeng 2017.2.21
	sprintf(dev_name[devnum], "nand%d", devnum);
*/
	mtd->name = dev_name[devnum];

#ifdef CONFIG_MTD_DEVICE
	/*
	 * Add MTD device so that we can reference it later
	 * via the mtdcore infrastructure (e.g. ubi).
	 */
/* CWWeng 2017.2.21
	add_mtd_device(mtd);
*/
#endif

	total_nand_size += mtd->size / 1024;

	if (nand_curr_device == -1)
		nand_curr_device = devnum;

	return 0;
}

void nand_init(void)
{
}
