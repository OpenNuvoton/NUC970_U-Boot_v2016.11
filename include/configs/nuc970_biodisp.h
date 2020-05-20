/*
 * (C) Copyright 2014
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * Configuation settings for the NUC970 EV Board.
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

#define CONFIG_SYS_TEXT_BASE		0x2000000

#define CONFIG_SYS_LOAD_ADDR            0x8000

#define CONFIG_SYS_HZ			1000
#define CONFIG_SYS_MEMTEST_START         0xA00000
#define CONFIG_SYS_MEMTEST_END           0xB00000

#define CONFIG_ARCH_CPU_INIT
#undef  CONFIG_USE_IRQ

#define CONFIG_CMDLINE_TAG	1	/* enable passing of ATAGs	*/
#define CONFIG_SETUP_MEMORY_TAGS 1
#define CONFIG_INITRD_TAG	1

#define CONFIG_CMD_TIMER

#ifdef  CONFIG_SYS_I2C
#ifndef CONFIG_SYS_I2C_SPEED
#define CONFIG_SYS_I2C_SPEED		50000
#endif
#endif

#ifdef CONFIG_SYS_I2C_SOFT
#define CONFIG_SOFT_I2C_GPIO_SCL	GPIO_PG0
#define CONFIG_SOFT_I2C_GPIO_SDA	GPIO_PG1
#endif

#undef CONFIG_CMD_MEMORY

#define CONFIG_SYS_USE_SPIFLASH
#define CONFIG_SYS_NO_FLASH    // that is, no *NOR* flash
#define CONFIG_ENV_IS_IN_SPI_FLASH
#undef  CONFIG_CMD_IMLS

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_HW_WATCHDOG

#define CONFIG_SYS_BOOTM_LEN		0x1000000 /* 16MB max kernel size */

/*#define CONFIG_DISPLAY_CPUINFO */

/*#define CONFIG_BOOTDELAY	1*/

#define CONFIG_SYS_SDRAM_BASE   0
#define CONFIG_NR_DRAM_BANKS    2     /* there are 2 sdram banks for nuc970 */
#define CONFIG_SYS_INIT_SP_ADDR 0xBC008000

#define CONFIG_BAUDRATE         115200
#define CONFIG_SYS_BAUDRATE_TABLE       {115200, 57600, 38400}

/*#define CONFIG_NUC970_EMAC1*/
/*#define CONFIG_CMD_NET */
#define CONFIG_ETHADDR                  00:00:00:11:66:88
#define CONFIG_SYS_RX_ETH_BUFFER        16 // default is 4, set to 16 here.

/*#define CONFIG_KPI_NUC970*/

/*#undef CONFIG_SYS_ICACHE_OFF */
/*#undef CONFIG_SYS_DCACHE_OFF */
#define CONFIG_SYS_ICACHE_OFF
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
#if 0
#include <config_cmd_default.h>

#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_SOURCE
#endif

#define CONFIG_CMD_PING		1
#define CONFIG_CMD_DHCP		1


/* SPI flash */
#ifdef CONFIG_SYS_USE_SPIFLASH
#define CONFIG_SPI              1
#ifdef CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_OFFSET       0x1F0000
#define CONFIG_ENV_SIZE         0x10000
#define CONFIG_ENV_SECT_SIZE    0x10000
#define CONFIG_ENV_OVERWRITE
#endif
#endif


/*#define CONFIG_SYS_PROMPT		"U-Boot> "*/
#define CONFIG_SYS_CBSIZE		512
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_LONGHELP		1
#define CONFIG_CMDLINE_EDITING		1
#define CONFIG_AUTO_COMPLETE		1
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "


/* Following block is for MMC support */
#ifdef CONFIG_NUC970_MMC
#ifndef CONFIG_CMD_MMC
#define CONFIG_CMD_MMC
#endif
#ifndef CONFIG_CMD_FAT
#define CONFIG_CMD_FAT
#endif
/*#define CONFIG_GENERIC_MMC */
#define CONFIG_DOS_PARTITION
/*#define CONFIG_MMC_TRACE */
/*#define CONFIG_NUC970_SD_PORT0 */
/*#define CONFIG_NUC970_SD_PORT1 */
/*#define CONFIG_NUC970_EMMC */      /* Don't enable eMMC(CONFIG_NUC970_EMMC) and NAND(CONFIG_NAND_NUC970) at the same time! */
#ifdef CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV  1
#define CONFIG_ENV_OFFSET       0x80000
#define CONFIG_ENV_SIZE         /*2048*/0x10000
#define CONFIG_ENV_SECT_SIZE    512
#define CONFIG_ENV_OVERWRITE
#endif
#endif

/* Following block is for EHCI support*/
#if 0
#define CONFIG_CMD_USB
#define CONFIG_CMD_FAT
#define CONFIG_USB_STORAGE
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_NUC970
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_DOS_PARTITION
#endif


/*#define CONFIG_OF_LIBFDT */
/*#define CONFIG_FIT */

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN	(1024*1024) /*ROUND(3 * CONFIG_ENV_SIZE + 128*1024, 0x1000)  */

#define CONFIG_STACKSIZE	(32*1024)	/* regular stack */

#define CONFIG_EXTRA_ENV_SETTINGS \
	"uimage=biodisp.ub\0" \
	"oimage=biodisp.bak\0" \
	"ethact=emac\0" \
	"bootdelay=1\0" \
	"addpanel=if env exists panelid; then setenv bootargs ${bootargs} tftpanel=${panelid}; fi\0" \
	"addmac=setenv bootargs ${bootargs} ethaddr0=${ethaddr}\0" \
	"bootargs=root=/dev/ram0 console=ttyS0,115200n8 rdinit=/sbin/init mem=64M\0" \
	"bootcmd=run addmac; run addpanel; fatload mmc 0 0x7fc0 ${uimage}; bootm 0x7fc0; fatload mmc 0 0x7fc0 ${oimage}; bootm 0x7fc0\0"

#endif
