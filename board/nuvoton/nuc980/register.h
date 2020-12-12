/*
 * (C) Copyright 2020
 * Nuvoton Technology corp.
 * SPDX-License-Identifier:    GPL-2.0+
 */
#ifndef _REGISTER_H
#define _REGISTER_H

/*!< AHB peripherals */
#define    SYS_BA    0xB0000000 /* Global Control */
#define    GCR_BA    SYS_BA
#define    CLK_BA    0xB0000200 /* Clock Control */
#define    SDIC_BA   0xB0002000
#define    EBI_BA    0xB0010000 /* EBI Control */
#define    GPIO_BA   0xB0004000
#define    PDMA0_BA  0xB0008000
#define    PDMA1_BA  0xB0009000
#define    EMC0_BA   0xB0012000
#define    EHCI_BA   0xB0015000
#define    USBD_BA   0xB0016000
#define    OHCI_BA   0xB0017000
#define    SDH_BA    0xB0018000
#define    FMI_BA    0xB0019000
#define    CRPT_BA   0xB001C000
#define    I2S_BA    0xB0020000
#define    EMC1_BA   0xB0022000
#define    CAP0_BA   0xB0024000
#define    CAP1_BA   0xB0014000

#define    PA_BA     0xB0004000
#define    PB_BA     0xB0004040
#define    PC_BA     0xB0004080
#define    PD_BA     0xB00040C0
#define    PE_BA     0xB0004100
#define    PF_BA     0xB0004140
#define    PG_BA     0xB0004180

/*!< APB peripherals */

#define    WDT_BA    0xB0040000 /* Watch Dog Timer */
#define    WWDT_BA   0xB0040100 /* Windows Watch Dog Timer */
#define    RTC_BA    0xB0041000
#define    AIC_BA    0xB0042000 /* Interrupt Controller */
#define    ADC_BA    0xB0043000
#define    ETMR0_BA  0xB0050000 /* ETimer0 */
#define    ETMR1_BA  0xB0050100 /* ETimer1 */
#define    ETMR2_BA  0xB0051000 /* ETimer2 */
#define    ETMR3_BA  0xB0051100 /* ETimer3 */
#define    ETMR4_BA  0xB0052000 /* ETimer4 */
#define    ETMR5_BA  0xB0052100 /* ETimer5 */
#define    PWM0_BA   0xB0058000
#define    PWM1_BA   0xB0059000
#define    QSPI0_BA  0xB0060000
#define    SPI0_BA   0xB0061000
#define    SPI1_BA   0xB0062000
#define    UART0_BA  0xB0070000 /* UART0 Control */
#define    UART1_BA  0xB0071000 /* UART1 Control */
#define    UART2_BA  0xB0072000
#define    UART3_BA  0xB0073000
#define    UART4_BA  0xB0074000
#define    UART5_BA  0xB0075000
#define    UART6_BA  0xB0076000
#define    UART7_BA  0xB0077000
#define    UART8_BA  0xB0078000
#define    UART9_BA  0xB0079000
#define    I2C0_BA   0xB0080000
#define    I2C1_BA   0xB0081000
#define    I2C2_BA   0xB0082000
#define    I2C3_BA   0xB0083000
#define    SC0_BA    0xB0090000
#define    SC1_BA    0xB0091000
#define    CAN0_BA   0xB00A0000
#define    CAN1_BA   0xB00A1000
#define    CAN2_BA   0xB00A2000
#define    CAN3_BA   0xB00A3000


#define    REG_PDID     	(GCR_BA+0x000)  /* Product Identifier Register */
#define    REG_PWRON		(GCR_BA+0x004)  /* Power-On Setting Register */
#define    REG_ARBCON		(GCR_BA+0x008)  /* Arbitration Control Register */
#define    REG_LVRDCR 		(GCR_BA+0x020)  /* Low Voltage Reset & Detect Control Register */
#define    REG_MISCFCR  	(GCR_BA+0x030)  /* Miscellaneous Function Control Register */
#define    REG_MISCIER 		(GCR_BA+0x040)  /* Miscellaneous Interrupt Enable Register */
#define    REG_MISCISR 		(GCR_BA+0x044)  /* Miscellaneous Interrupt Status Register */
#define    REG_ROMSUM0 		(GCR_BA+0x048)  /* Internal ROM BIST Checksum Register 0 */
#define    REG_ROMSUM1 		(GCR_BA+0x04C)  /* Internal ROM BIST Checksum Register 1 */
#define    REG_WKUPSER		(GCR_BA+0x058)  /* System Wakeup Source Enable Register */
#define    REG_WKUPSSR		(GCR_BA+0x05C)  /* System Wakeup Source Status Register */
#define    REG_AHBIPRST		(GCR_BA+0x060)  /* AHB IP Reset Control Register */
#define    REG_APBIPRST0	(GCR_BA+0x064)  /* APB IP Reset Control Register 0 */
#define    REG_APBIPRST1	(GCR_BA+0x068)  /* APB IP Reset Control Register 1 */
#define    REG_RSTSTS		(GCR_BA+0x06C)  /* Reset Source Active Status Register */
#define    REG_MFP_GPA_L	(GCR_BA+0x070)  /* GPIOA Low Byte Multiple Function Control Register */
#define    REG_MFP_GPA_H	(GCR_BA+0x074)  /* GPIOA High Byte Multiple Function Control Register */
#define    REG_MFP_GPB_L	(GCR_BA+0x078)  /* GPIOB Low Byte Multiple Function Control Register */
#define    REG_MFP_GPB_H	(GCR_BA+0x07C)  /* GPIOB High Byte Multiple Function Control Register */
#define    REG_MFP_GPC_L	(GCR_BA+0x080)  /* GPIOC Low Byte Multiple Function Control Register */
#define    REG_MFP_GPC_H	(GCR_BA+0x084)  /* GPIOC High Byte Multiple Function Control Register */
#define    REG_MFP_GPD_L	(GCR_BA+0x088)  /* GPIOD Low Byte Multiple Function Control Register */
#define    REG_MFP_GPD_H	(GCR_BA+0x08C)  /* GPIOD High Byte Multiple Function Control Register */
#define    REG_MFP_GPE_L	(GCR_BA+0x090)  /* GPIOE Low Byte Multiple Function Control Register */
#define    REG_MFP_GPE_H	(GCR_BA+0x094)  /* GPIOE High Byte Multiple Function Control Register */
#define    REG_MFP_GPF_L	(GCR_BA+0x098)  /* GPIOF Low Byte Multiple Function Control Register */
#define    REG_MFP_GPF_H	(GCR_BA+0x09C)  /* GPIOF High Byte Multiple Function Control Register */
#define    REG_MFP_GPG_L	(GCR_BA+0x0A0)  /* GPIOG Low Byte Multiple Function Control Register */
#define    REG_MFP_GPG_H	(GCR_BA+0x0A4)  /* GPIOG High Byte Multiple Function Control Register */
#define    REG_MFP_GPH_L	(GCR_BA+0x0A8)  /* GPIOH Low Byte Multiple Function Control Register */
#define    REG_MFP_GPH_H	(GCR_BA+0x0AC)  /* GPIOH High Byte Multiple Function Control Register */
#define    REG_MFP_GPI_L	(GCR_BA+0x0B0)  /* GPIOI Low Byte Multiple Function Control Register */
#define    REG_MFP_GPI_H	(GCR_BA+0x0B4)  /* GPIOI High Byte Multiple Function Control Register */
#define    REG_DDR_DS_CR	(GCR_BA+0x0E0)  /* DDR I/O Driving Strength Control Register */


#define    REG_PMCON		(CLK_BA+0x000)  /*  Power Management Control Register */
#define    REG_HCLKEN		(CLK_BA+0x010)  /*  AHB IP Clock Enable Control Register */
#define    REG_PCLKEN0		(CLK_BA+0x018)  /*  APB IP Clock Enable Control Register 0 */
#define    REG_PCLKEN1		(CLK_BA+0x01C)  /*  APB IP Clock Enable Control Register 1 */
#define    REG_CLKDIV0		(CLK_BA+0x020)  /*  Clock Divider Control Register 0 */
#define    REG_CLKDIV1		(CLK_BA+0x024)  /*  Clock Divider Control Register 1 */
#define    REG_CLKDIV2		(CLK_BA+0x028)  /*  Clock Divider Control Register 2 */
#define    REG_CLKDIV3		(CLK_BA+0x02C)  /*  Clock Divider Control Register 3 */
#define    REG_CLKDIV4		(CLK_BA+0x030)  /*  Clock Divider Control Register 4 */
#define    REG_CLKDIV5		(CLK_BA+0x034)  /*  Clock Divider Control Register 5 */
#define    REG_CLKDIV6		(CLK_BA+0x038)  /*  Clock Divider Control Register 6 */
#define    REG_CLKDIV7		(CLK_BA+0x03C)  /*  Clock Divider Control Register 7 */
#define    REG_CLKDIV8		(CLK_BA+0x040)  /*  Clock Divider Control Register 8 */
#define    REG_CLKDIV9		(CLK_BA+0x044)  /*  Clock Divider Control Register 9 */
#define    REG_APLLCON		(CLK_BA+0x060)  /*  APLL Control Register */
#define    REG_UPLLCON		(CLK_BA+0x064)  /*  UPLL Control Register */
#define    REG_CLKTREG		(CLK_BA+0x080)  /*  Test Clock Control Register */


/* DMAC Control Registers*/
#define     REG_FB0			(FMI_BA+0x000)  /* DMAC Control and Status Register */
#define     REG_DMACCSR		(FMI_BA+0x400)  /* DMAC Control and Status Register */
#define     REG_DMACSAR		(FMI_BA+0x408)  /* DMAC Transfer Starting Address Register */
#define     REG_DMACBCR		(FMI_BA+0x40C)  /* DMAC Transfer Byte Count Register */
#define     REG_DMACIER		(FMI_BA+0x410)  /* DMAC Interrupt Enable Register */
#define     REG_DMACISR		(FMI_BA+0x414)  /* DMAC Interrupt Status Register */

#define     REG_FMICSR		(FMI_BA+0x800)   /* Global Control and Status Register */
#define     REG_FMIIER		(FMI_BA+0x804)   /* Global Interrupt Control Register */
#define     REG_FMIISR		(FMI_BA+0x808)   /* Global Interrupt Status Register */

/* Secure Digit Registers */
#define     REG_SDCSR		(FMI_BA+0x820)   /* SD control and status register */
#define     REG_SDARG		(FMI_BA+0x824)   /* SD command argument register */
#define     REG_SDIER		(FMI_BA+0x828)   /* SD interrupt enable register */
#define     REG_SDISR		(FMI_BA+0x82C)   /* SD interrupt status register */
#define     REG_SDRSP0		(FMI_BA+0x830)   /* SD receive response token register 0 */
#define     REG_SDRSP1		(FMI_BA+0x834)   /* SD receive response token register 1 */
#define     REG_SDBLEN		(FMI_BA+0x838)   /* SD block length register */
#define     REG_SDTMOUT		(FMI_BA+0x83C)   /* SD block length register */
 
/* NAND-type Flash Registers */
#define     REG_SMCSR		(FMI_BA+0x8A0)   /* NAND Flash Control and Status Register */
#define     REG_SMTCR		(FMI_BA+0x8A4)   /* NAND Flash Timing Control Register */
#define     REG_SMIER		(FMI_BA+0x8A8)   /* NAND Flash Interrupt Control Register */
#define     REG_SMISR		(FMI_BA+0x8AC)   /* NAND Flash Interrupt Status Register */
#define     REG_SMCMD		(FMI_BA+0x8B0)   /* NAND Flash Command Port Register */
#define     REG_SMADDR		(FMI_BA+0x8B4)   /* NAND Flash Address Port Register */
#define     REG_SMDATA		(FMI_BA+0x8B8)   /* NAND Flash Data Port Register */
#define     REG_SMREACTL	(FMI_BA+0x8BC)   /* NAND Flash Smart-Media Redundant Area Control Register */
#define     REG_NFECR       (FMI_BA+0x8C0)   /* NAND Flash Extend Control Regsiter */
#define     REG_SMECC_ST0	(FMI_BA+0x8D0)	 /* Smart-Media ECC Error Status 0 */
#define     REG_SMECC_ST1	(FMI_BA+0x8D4)	 /* Smart-Media ECC Error Status 1 */
#define     REG_SMECC_ST2	(FMI_BA+0x8D8)	 /* Smart-Media ECC Error Status 2 */
#define     REG_SMECC_ST3	(FMI_BA+0x8DC)	 /* Smart-Media ECC Error Status 3 */
#define     REG_SMPROT_ADDR0 (FMI_BA+0x8E0)  /* Smart-Media Protect region end address 0 */
#define     REG_SMPROT_ADDR1 (FMI_BA+0x8E4)  /* Smart-Media Protect region end address 1 */

/* NAND-type Flash BCH Error Address Registers */
#define     REG_BCH_ECC_ADDR0	(FMI_BA+0x900)  /* BCH error byte address 0 */
#define     REG_BCH_ECC_ADDR1	(FMI_BA+0x904)  /* BCH error byte address 1 */
#define     REG_BCH_ECC_ADDR2	(FMI_BA+0x908)  /* BCH error byte address 2 */
#define     REG_BCH_ECC_ADDR3	(FMI_BA+0x90C)  /* BCH error byte address 3 */
#define     REG_BCH_ECC_ADDR4	(FMI_BA+0x910)  /* BCH error byte address 4 */
#define     REG_BCH_ECC_ADDR5	(FMI_BA+0x914)  /* BCH error byte address 5 */
#define     REG_BCH_ECC_ADDR6	(FMI_BA+0x918)  /* BCH error byte address 6 */
#define     REG_BCH_ECC_ADDR7	(FMI_BA+0x91C)  /* BCH error byte address 7 */
#define     REG_BCH_ECC_ADDR8	(FMI_BA+0x920)  /* BCH error byte address 8 */
#define     REG_BCH_ECC_ADDR9	(FMI_BA+0x924)  /* BCH error byte address 9 */
#define     REG_BCH_ECC_ADDR10	(FMI_BA+0x928)  /* BCH error byte address 10 */
#define     REG_BCH_ECC_ADDR11	(FMI_BA+0x92C)  /* BCH error byte address 11 */

/* NAND-type Flash BCH Error Data Registers */
#define     REG_BCH_ECC_DATA0	(FMI_BA+0x960)  /* BCH error byte data 0 */
#define     REG_BCH_ECC_DATA1	(FMI_BA+0x964)  /* BCH error byte data 1 */
#define     REG_BCH_ECC_DATA2	(FMI_BA+0x968)  /* BCH error byte data 2 */
#define     REG_BCH_ECC_DATA3	(FMI_BA+0x96C)  /* BCH error byte data 3 */
#define     REG_BCH_ECC_DATA4	(FMI_BA+0x970)  /* BCH error byte data 4 */
#define     REG_BCH_ECC_DATA5	(FMI_BA+0x974)  /* BCH error byte data 5 */

/* NAND-type Flash Redundant Area Registers */
#define     REG_SMRA0			(FMI_BA+0xA00)  /* Smart-Media Redundant Area Register */
#define     REG_SMRA1			(FMI_BA+0xA04)  /* Smart-Media Redundant Area Register */

#define     REG_UART0_RBR		(UART0_BA+0x00)  /* Receive Buffer Register */
#define     REG_UART0_THR		(UART0_BA+0x00)  /* Transmit Holding Register */
#define     REG_UART0_IER		(UART0_BA+0x04)  /* Interrupt Enable Register */
#define     REG_UART0_FCR		(UART0_BA+0x08)  /* FIFO Control Register */
#define     REG_UART0_LCR		(UART0_BA+0x0C)  /* Line Control Register */
#define     REG_UART0_MCR		(UART0_BA+0x10)  /* Modem Control Register */
#define     REG_UART0_MSR		(UART0_BA+0x14)  /* MODEM Status Register */
#define     REG_UART0_FSR		(UART0_BA+0x18)  /* FIFO Status Register */
#define     REG_UART0_ISR		(UART0_BA+0x1C)  /* Interrupt Status Register */
#define     REG_UART0_TOR		(UART0_BA+0x20)  /* Time Out Register */
#define     REG_UART0_BAUD		(UART0_BA+0x24)  /* Baud Rate Divisor Register */
#define     REG_UART0_IRCR		(UART0_BA+0x28)  /* IrDA Control Register */
#define     REG_UART0_ALT_CSR	(UART0_BA+0x2C)  /* Alternate Control/Status Register */
#define     REG_UART0_FUN_SEL	(UART0_BA+0x30)  /* Function Select Register */
#define     REG_UART0_LIN_CTL	(UART0_BA+0x34)  /* LIN Control Register */
#define     REG_UART0_LIN_SR	(UART0_BA+0x38)  /* LIN Status Register */
#define     REG_UART0_DEBUG		(UART0_BA+0x3C)  /* Debug Register */
#define     REG_UART0_SC_CTL	(UART0_BA+0x40)  /* SC Control Register */
#define     REG_UART0_SC_FSR	(UART0_BA+0x44)  /* SC Flag Status Register */
 


#define REG_WDT_CR			(WDT_BA + 0x00)	/* WDT control register */
#define REG_WDT_CRALT			(WDT_BA + 0x04)	/* WDT alternative control register */


#define     REG_AIC_SCR1    (AIC_BA+0x00)    /* Source control register 1 */
#define     REG_AIC_SCR2    (AIC_BA+0x04)    /* Source control register 2 */
#define     REG_AIC_SCR3    (AIC_BA+0x08)    /* Source control register 3 */
#define     REG_AIC_SCR4    (AIC_BA+0x0C)    /* Source control register 4 */
#define     REG_AIC_SCR5    (AIC_BA+0x10)    /* Source control register 5 */
#define     REG_AIC_SCR6    (AIC_BA+0x14)    /* Source control register 6 */
#define     REG_AIC_SCR7    (AIC_BA+0x18)    /* Source control register 7 */
#define     REG_AIC_SCR8    (AIC_BA+0x1C)    /* Source control register 8 */
#define     REG_AIC_SCR9    (AIC_BA+0x20)    /* Source control register 9 */
#define     REG_AIC_SCR10   (AIC_BA+0x24)    /* Source control register 10 */
#define     REG_AIC_SCR11   (AIC_BA+0x28)    /* Source control register 11 */
#define     REG_AIC_SCR12   (AIC_BA+0x2C)    /* Source control register 12 */
#define     REG_AIC_SCR13   (AIC_BA+0x30)    /* Source control register 13 */
#define     REG_AIC_SCR14   (AIC_BA+0x34)    /* Source control register 14 */
#define     REG_AIC_SCR15   (AIC_BA+0x38)    /* Source control register 15 */
#define     REG_AIC_IRSR    (AIC_BA+0x100)   /* Interrupt raw status register */
#define     REG_AIC_IRSRH   (AIC_BA+0x104)   /* Interrupt raw status register (Hign) */
#define     REG_AIC_IASR    (AIC_BA+0x108)   /* Interrupt active status register */
#define     REG_AIC_IASRH   (AIC_BA+0x10C)   /* Interrupt active status register (Hign) */
#define     REG_AIC_ISR     (AIC_BA+0x110)   /* Interrupt status register */
#define     REG_AIC_ISRH    (AIC_BA+0x114)   /* Interrupt status register (High) */
#define     REG_AIC_IPER    (AIC_BA+0x118)   /* Interrupt priority encoding register */
#define     REG_AIC_ISNR    (AIC_BA+0x120)   /* Interrupt source number register */
#define     REG_AIC_OISR    (AIC_BA+0x124)   /* Output interrupt status register */
#define     REG_AIC_IMR     (AIC_BA+0x128)   /* Interrupt mask register */
#define     REG_AIC_IMRH    (AIC_BA+0x12C)   /* Interrupt mask register (High) */
#define     REG_AIC_MECR    (AIC_BA+0x130)   /* Mask enable command register */
#define     REG_AIC_MECRH   (AIC_BA+0x134)   /* Mask enable command register (High) */
#define     REG_AIC_MDCR    (AIC_BA+0x138)   /* Mask disable command register */
#define     REG_AIC_MDCRH   (AIC_BA+0x13C)   /* Mask disable command register (High) */
#define     REG_AIC_SSCR    (AIC_BA+0x140)   /* Source Set Command Register */
#define     REG_AIC_SSCRH   (AIC_BA+0x144)   /* Source Set Command Register (High) */
#define     REG_AIC_SCCR    (AIC_BA+0x148)   /* Source Clear Command Register */
#define     REG_AIC_SCCRH   (AIC_BA+0x14C)   /* Source Clear Command Register (High) */
#define     REG_AIC_EOSCR   (AIC_BA+0x150)   /* End of service command register */
 

#define     REG_CRPT_IPSEC_INT_EN	(CRPT_BA+0x0000)  /* IPsec interrupt enable */
#define     REG_CRPT_IPSEC_INT_FLAG	(CRPT_BA+0x0004)  /* IPsec interrupt flag */
#define     REG_CRPT_AES_CTL		(CRPT_BA+0x0100)  /* AES engine setting */
#define     REG_CRPT_AES_KEY0		(CRPT_BA+0x0110)  /* AES Key Word 0 Register */
#define     REG_CRPT_AES_KEY1		(CRPT_BA+0x0114)  /* AES Key Word 1 Register */
#define     REG_CRPT_AES_KEY2		(CRPT_BA+0x0118)  /* AES Key Word 2 Register */
#define     REG_CRPT_AES_KEY3		(CRPT_BA+0x011C)  /* AES Key Word 3 Register */
#define     REG_CRPT_AES_KEY4		(CRPT_BA+0x0120)  /* AES Key Word 4 Register */
#define     REG_CRPT_AES_KEY5		(CRPT_BA+0x0124)  /* AES Key Word 5 Register */
#define     REG_CRPT_AES_KEY6		(CRPT_BA+0x0128)  /* AES Key Word 6 Register */
#define     REG_CRPT_AES_KEY7		(CRPT_BA+0x012C)  /* AES Key Word 7 Register */
#define     REG_CRPT_AES_IV0		(CRPT_BA+0x0130)  /* AES Initial Vector Word 0 Register */
#define     REG_CRPT_AES_IV1		(CRPT_BA+0x0134)  /* AES Initial Vector Word 1 Register */
#define     REG_CRPT_AES_IV2		(CRPT_BA+0x0138)  /* AES Initial Vector Word 2 Register */
#define     REG_CRPT_AES_IV3		(CRPT_BA+0x013C)  /* AES Initial Vector Word 3 Register */
#define     REG_CRPT_AES_SADR		(CRPT_BA+0x0140)  /* AES Source Address Register */
#define     REG_CRPT_AES_DADR		(CRPT_BA+0x0144)  /* AES Destination Address Register */
#define     REG_CRPT_AES_CNT		(CRPT_BA+0x0148)  /* AES Byte Count Register */


#define		REG_RTC_INIR			(RTC_BA+0x0000)	/* Initial register */
#define		REG_RTC_AER			(RTC_BA+0x0004)	/* Access Enable Register*/
#define		REG_RTC_INTSTS			(RTC_BA+0x002C)	/* Interrupt Status */
#define		REG_RTC_SPARE			(RTC_BA+0x0040)	/* Spare register */

 
#endif /* _REGISTER_H */
