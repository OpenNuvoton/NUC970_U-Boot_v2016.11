/* linux/include/asm-arm/arch-nuc900/nuc900_reg.h
 *
 * Copyright (c) 2008 Nuvoton technology corporation
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Changelog:
 *
 *   2006/08/26     vincen.zswan add this file for nuvoton nuc900 MCU ip REG.
 */

#ifndef __ASM_ARCH_REGS_FMI_H
#define __ASM_ARCH_REGS_FMI_H

#define DMAC_BA   W90X900_VA_DMA /* DMA Control */
#define FMI_BA    W90X900_VA_FMI /* Flash Memory Card Interface */

/* DMAC Control Registers */
#define REG_DMACCSR		(DMAC_BA+0x800)  /* DMAC Control and Status Register */
#define REG_DMACSAR1		(DMAC_BA+0x804)  /* DMAC Transfer Starting Address Register 1 */
#define REG_DMACSAR2		(DMAC_BA+0x808)  /* DMAC Transfer Starting Address Register 2 */
#define REG_DMACBCR		(DMAC_BA+0x80C)  /* DMAC Transfer Byte Count Register */
#define REG_DMACIER		(DMAC_BA+0x810)  /* DMAC Interrupt Enable Register */
#define REG_DMACISR		(DMAC_BA+0x814)  /* DMAC Interrupt Status Register */

/* Flash Memory Card Interface Registers */
#define REG_FMICSR		(FMI_BA+0x000)   /* Global Control and Status Register */
#define REG_FMIIER		(FMI_BA+0x004)   /* Global Interrupt Control Register */
#define REG_FMIISR		(FMI_BA+0x008)   /* Global Interrupt Status Register */

/* Secure Digit Registers */
#define REG_SDCSR		(FMI_BA+0x020)   /* SD control and status register */
#define REG_SDARG		(FMI_BA+0x024)   /* SD command argument register */
#define REG_SDIER		(FMI_BA+0x028)   /* SD interrupt enable register */
#define REG_SDISR		(FMI_BA+0x02C)   /* SD interrupt status register */
#define REG_SDRSP0		(FMI_BA+0x030)   /* SD receive response token register 0 */
#define REG_SDRSP1		(FMI_BA+0x034)   /* SD receive response token register 1 */
#define REG_SDBLEN		(FMI_BA+0x038)   /* SD block length register */
#define REG_SDTMOUT		(FMI_BA+0x03C)   /* SD Response/Data-in timeout register */

/* Memory Stick Registers */
#define REG_MSCSR		(FMI_BA+0x060)   /* MS control and status register */
#define REG_MSIER		(FMI_BA+0x064)   /* MS interrupt control register */
#define REG_MSISR		(FMI_BA+0x068)   /* MS interrupt status register */
#define REG_MSBUF1		(FMI_BA+0x06C)   /* Memory stick register buffer 1 */
#define REG_MSBUF2		(FMI_BA+0x070)   /* Memory stick register buffer 2 */

/* NAND-type Flash Registers */
#define REG_SMCSR		(FMI_BA+0x0A0)   /* NAND Flash Control and Status Register */
#define REG_SMTCR		(FMI_BA+0x0A4)   /* NAND Flash Timing Control Register */
#define REG_SMIER		(FMI_BA+0x0A8)   /* NAND Flash Interrupt Control Register */
#define REG_SMISR		(FMI_BA+0x0AC)   /* NAND Flash Interrupt Status Register */
#define REG_SMCMD		(FMI_BA+0x0B0)   /* NAND Flash Command Port Register */
#define REG_SMADDR		(FMI_BA+0x0B4)   /* NAND Flash Address Port Register */
#define REG_SMDATA		(FMI_BA+0x0B8)   /* NAND Flash Data Port Register */
#define REG_SMECC0		(FMI_BA+0x0BC)   /* NAND Flash Error Correction Code 0 Register */
#define REG_SMECC1		(FMI_BA+0x0C0)   /* NAND Flash Error Correction Code 1 Register */
#define REG_SMECC2		(FMI_BA+0x0C4)   /* NAND Flash Error Correction Code 2 Register */
#define REG_SMECC3		(FMI_BA+0x0C8)   /* NAND Flash a Error Correction Code 3 Register */
#define REG_SMRA_0		(FMI_BA+0x0CC)   /* NAND Flash Redundant Area Register */
#define REG_SMRA_1		(FMI_BA+0x0D0)
#define REG_SMRA_2		(FMI_BA+0x0D4)
#define REG_SMRA_3		(FMI_BA+0x0D8)
#define REG_SMRA_4		(FMI_BA+0x0DC)
#define REG_SMRA_5		(FMI_BA+0x0E0)
#define REG_SMRA_6		(FMI_BA+0x0E4)
#define REG_SMRA_7		(FMI_BA+0x0E8)
#define REG_SMRA_8		(FMI_BA+0x0EC)
#define REG_SMRA_9		(FMI_BA+0x0F0)
#define REG_SMRA_10		(FMI_BA+0x0F4)
#define REG_SMRA_11		(FMI_BA+0x0F8)
#define REG_SMRA_12		(FMI_BA+0x0FC)
#define REG_SMRA_13		(FMI_BA+0x100)
#define REG_SMRA_14		(FMI_BA+0x104)
#define REG_SMRA_15		(FMI_BA+0x108)   /* NAND Flash Redundant Area Register */
#define REG_SMECCAD0		(FMI_BA+0x10C)   /* NAND Flash ECC Correction Address 0 */
#define REG_SMECCAD1		(FMI_BA+0x110)   /* NAND Flash ECC Correction Address 1 */
#define REG_ECC4ST		(FMI_BA+0x114)   /* ECC4 Correction Status */
#define REG_ECC4F1A1		(FMI_BA+0x118)   /* ECC4 Field 1 Error Address 1 */
#define REG_ECC4F1A2		(FMI_BA+0x11C)   /* ECC4 Field 1 Error Address 2 */
#define REG_ECC4F1D		(FMI_BA+0x120)   /* ECC4 Field 1 Error Data */
#define REG_ECC4F2A1		(FMI_BA+0x124)   /* ECC4 Field 2 Error Address 1 */
#define REG_ECC4F2A2		(FMI_BA+0x128)   /* ECC4 Field 2 Error Address 2 */
#define REG_ECC4F2D		(FMI_BA+0x12C)   /* ECC4 Field 2 Error Data */
#define REG_ECC4F3A1		(FMI_BA+0x130)   /* ECC4 Field 3 Error Address 1 */
#define REG_ECC4F3A2		(FMI_BA+0x134)   /* ECC4 Field 3 Error Address 2 */
#define REG_ECC4F3D		(FMI_BA+0x138)   /* ECC4 Field 3 Error Data */
#define REG_ECC4F4A1		(FMI_BA+0x13C)   /* ECC4 Field 4 Error Address 1 */
#define REG_ECC4F4A2		(FMI_BA+0x140)   /* ECC4 Field 4 Error Address 2 */
#define REG_ECC4F4D		(FMI_BA+0x144)   /* ECC4 Field 4 Error Data */


#endif
