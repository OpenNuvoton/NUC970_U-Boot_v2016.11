/*
 * (C) Copyright 2017
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * Configuation settings for the NUC980 EV Board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define EXT_CLK	        12000000        /* 12 MHz crystal */

#define CONFIG_SYS_TEXT_BASE		0xE00000

#define CONFIG_SYS_LOAD_ADDR            0xE00000

#define CONFIG_SYS_MEMTEST_START         0xA00000
#define CONFIG_SYS_MEMTEST_END           0xB00000

#define CONFIG_ARCH_CPU_INIT
#undef  CONFIG_USE_IRQ

#define CONFIG_CMDLINE_TAG	1	/* enable passing of ATAGs	*/
#define CONFIG_SETUP_MEMORY_TAGS 1
#define CONFIG_INITRD_TAG	1

/*#define CONFIG_NUC980_HW_CHECKSUM */

#define CONFIG_SYS_USE_SPIFLASH
#define CONFIG_SYS_USE_NANDFLASH
#define CONFIG_ENV_IS_IN_NAND

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

/*#define CONFIG_HW_WATCHDOG*/

#define CONFIG_SYS_BOOTM_LEN		0x1000000 /* 16MB max kernel size */

/*#define CONFIG_BOOTDELAY	3*/

#define CONFIG_SYS_SDRAM_BASE   0
#define CONFIG_NR_DRAM_BANKS    2     /* there are 2 sdram banks for nuc980 */
#define CONFIG_SYS_INIT_SP_ADDR 0xBC004000

#define CONFIG_BAUDRATE         115200
#define CONFIG_SYS_BAUDRATE_TABLE       {115200, 57600, 38400}

/*#define CONFIG_NUC980_EMAC1*/
/*#define CONFIG_CMD_NET */
#define CONFIG_ETHADDR                  00:00:00:11:66:88
#define CONFIG_SYS_RX_ETH_BUFFER        16 // default is 4, set to 16 here.

#undef CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF

/*
 * BOOTP options
 */
#if 1
#define CONFIG_BOOTP_BOOTFILESIZE	1
#define CONFIG_BOOTP_BOOTPATH		1
#define CONFIG_BOOTP_GATEWAY		1
#define CONFIG_BOOTP_HOSTNAME		1
#define CONFIG_BOOTP_SERVERIP /* tftp serverip not overruled by dhcp server */
#endif

/*
 * Command line configuration.
 */


#ifdef CONFIG_SYS_USE_SPIFLASH
#undef CONFIG_CMD_IMLS  /*====================> SPI only */
#undef CONFIG_CMD_JFFS2
#endif

#ifdef CONFIG_SYS_USE_NANDFLASH
#define CONFIG_CMD_NAND		1
#define CONFIG_CMD_UBI         1
#define CONFIG_CMD_UBIFS       1
#define CONFIG_MTD_UBI_WL_THRESHOLD  4096 /* CWWeng 2017.2.13 */
#define CONFIG_MTD_UBI_BEB_LIMIT  20 /* CWWeng 2017.2.13 */
#define CONFIG_CMD_MTDPARTS    1
#define CONFIG_MTD_DEVICE      1
#define CONFIG_MTD_PARTITIONS  1
#define CONFIG_RBTREE          1
#define CONFIG_LZO             1
#define CONFIG_SYS_NAND_ONFI_DETECTION 1

#define MTDIDS_DEFAULT "nand0=nand0"
#define MTDPARTS_DEFAULT "mtdparts=nand0:0x200000@0x0(u-boot),0x1400000@0x200000(kernel),-(user)"
#define MTD_ACTIVE_PART "nand0,2"

/* #define CONFIG_CMD_NAND_YAFFS2 1 */
/* #define CONFIG_YAFFS2          1 */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE	0xB000D000
#define CONFIG_SYS_NAND_ECCBYTES        12
#ifdef CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OFFSET       0x80000
#define CONFIG_ENV_SIZE         0x20000
#define CONFIG_ENV_SECT_SIZE    0x20000
#define CONFIG_ENV_RANGE	        (4 * CONFIG_ENV_SECT_SIZE)  /* Env range : 0x80000 ~ 0x100000 */
#define CONFIG_ENV_OVERWRITE
#endif
#endif

#define CONFIG_SYS_NAND_U_BOOT_OFFS     0x100000   /* Offset to RAM U-Boot image */

#define CONFIG_SPL_TEXT_BASE	0x200
#define CONFIG_SPL_STACK 	0xBC004000

#define CONFIG_SPL_NAND_SIMPLE
#define CONFIG_SPL_NAND_LOAD
#define CONFIG_SPL_NAND_BASE
#define CONFIG_SPL_NAND_DRIVERS
#define CONFIG_SPL_NAND_ECC

#ifdef CONFIG_SPL_BUILD /* CONFIG_NAND_SPL */
/* base address for uboot */
#define CONFIG_SYS_PHY_UBOOT_BASE       (CONFIG_SYS_SDRAM_BASE + 0xE00000)

#define CONFIG_SYS_NAND_U_BOOT_DST      CONFIG_SYS_PHY_UBOOT_BASE       /* NUB load-addr      */
#define CONFIG_SYS_NAND_U_BOOT_START    CONFIG_SYS_NAND_U_BOOT_DST      /* NUB start-addr     */

#define CONFIG_SYS_NAND_U_BOOT_SIZE     (500 * 1024)    /* Size of RAM U-Boot image   */

#endif


/* SPI flash */
#ifdef CONFIG_SYS_USE_SPIFLASH
#define CONFIG_SPI              1
#ifdef CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_OFFSET       0x80000
#define CONFIG_ENV_SIZE         0x20000
#define CONFIG_ENV_SECT_SIZE    0x20000
#define CONFIG_ENV_OVERWRITE
#endif
#endif


/*#define CONFIG_SYS_PROMPT		"U-Boot> "  */
#define CONFIG_SYS_CBSIZE		256
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_LONGHELP		1
#define CONFIG_CMDLINE_EDITING		1
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "


/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN	(1024*1024) /*ROUND(3 * CONFIG_ENV_SIZE + 128*1024, 0x1000)  */

#define CONFIG_STACKSIZE	(32*1024)	/* regular stack */

#endif
