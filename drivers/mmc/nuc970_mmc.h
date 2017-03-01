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


#ifndef _NUC970_MMC_
#define _NUC970_MMC_

//#define DMAC_BA   0xB000C000 /* DMA Control */
#define FMI_BA		0xB000D000 /* Flash Memory Card Interface */
#define SDH_BA		0xB000C000 /* SD Host */

/* DMAC Control Registers */
#define REG_DMACCSR             (SDH_BA+0x400)  /* DMAC Control and Status Register */
#define DMACEN           0x1
#define DMAC_SW_RST      0x2
//#define REG_DMACSAR1            (SDH_BA+0x404)  /* DMAC Transfer Starting Address Register 1 */
#define REG_DMACSAR2            (SDH_BA+0x408)  /* DMAC Transfer Starting Address Register 2 */
#define REG_DMACBCR             (SDH_BA+0x40C)  /* DMAC Transfer Byte Count Register */
#define REG_DMACIER             (SDH_BA+0x410)  /* DMAC Interrupt Enable Register */
#define REG_DMACISR             (SDH_BA+0x414)  /* DMAC Interrupt Status Register */

/* Flash Memory Card Interface Registers */
#define REG_SDH_GCTL            (SDH_BA+0x800)   /* Global Control and Status Register */
#define GCTL_RST      0x1
#define SD_EN         0x2
#define EMMC_EN       0x2

#define REG_GINTEN              (SDH_BA+0x804)   /* Global Interrupt Control Register */
#define REG_GINTSTS             (SDH_BA+0x808)   /* Global Interrupt Status Register */

/* Secure Digit Registers */
#define REG_SDCSR               (SDH_BA+0x820)   /* SD control and status register */
#define CO_EN           0x01
#define RI_EN           0x02
#define DI_EN           0x04
#define DO_EN           0x08
#define R2_EN           0x10
#define CLK74_OE        0x20
#define CLK8_OE         0x40
#define CLK_KEEP0       0x80
#define DBW             0x8000
#define REG_SDARG               (SDH_BA+0x824)   /* SD command argument register */
#define REG_SDIER               (SDH_BA+0x828)   /* SD interrupt enable register */
#define REG_SDISR               (SDH_BA+0x82C)   /* SD interrupt status register */
#define BLKD_IF         0x01
#define CRC_IF          0x02
#define CRC_7           0x04
#define CRC_16          0x08
#define SDDAT0          0x80
#define RITO_IF         0x1000
#define DITO_IF         0x2000
#define REG_SDRSP0              (SDH_BA+0x830)   /* SD receive response token register 0 */
#define REG_SDRSP1              (SDH_BA+0x834)   /* SD receive response token register 1 */
#define REG_SDBLEN              (SDH_BA+0x838)   /* SD block length register */
#define REG_SDTMOUT             (SDH_BA+0x83C)   /* SD Response/Data-in timeout register */
#define REG_ECTL                (SDH_BA+0x840)   /* SD Host extend control register */

#define REG_CLKDIVCTL3             0xB000022C
#define REG_CLKDIVCTL9             0xB0000244

#define MMC_CLK                 300000000  /* UCLKout */
//#define MMC_CLK                 12000000  /* Xin 12M */

// copy from linux/mmc/core.h
#define mmc_resp_type(cmd)      ((cmd)->resp_type & (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC|MMC_RSP_BUSY|MMC_RSP_OPCODE))

#define REG_FMIDMACTL		(FMI_BA+0x400)	/* FMI DMA control register */
#define REG_FMIDMASA		(FMI_BA+0x408)	/* FMI DMA transfer starting address register */
#define REG_FMIDMABCNT		(FMI_BA+0x40C)	/* FMI DMA transfer byte count register */
#define REG_FMIDMAINTEN		(FMI_BA+0x410)	/* FMI DMA interrupt enable register */
#define REG_FMIDMAINTSTS	(FMI_BA+0x414)	/* FMI DMA interrupt status register */

#define REG_FMICTL		(FMI_BA+0x800)	/* FMI control and status register */
#define REG_FMIINTEN		(FMI_BA+0x804)	/* FMI interrupt enable register */
#define REG_FMIINTSTS		(FMI_BA+0x808)	/* FMI interrupt status register */
#define REG_EMMCCTL		(FMI_BA+0x820)	/* eMMC control register */
#define REG_EMMCCMD		(FMI_BA+0x824)	/* eMMC command argument register */
#define REG_EMMCINTEN		(FMI_BA+0x828)	/* eMMC interrupt enable register */
#define REG_EMMCINTSTS		(FMI_BA+0x82C)	/* eMMC interrupt status register */
#define REG_EMMCRESP0		(FMI_BA+0x830)	/* eMMC receiving response register 0 */
#define REG_EMMCRESP1		(FMI_BA+0x834)	/* eMMC receiving response register 1 */
#define REG_EMMCBLEN		(FMI_BA+0x838)	/* eMMC block length register */
#define REG_EMMCTOUT		(FMI_BA+0x83C)	/* eMMC Response/Data-in time-out register */

#endif

