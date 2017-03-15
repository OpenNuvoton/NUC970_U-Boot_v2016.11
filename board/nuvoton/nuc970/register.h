/**********************************************************************************************************
 *                                                                          
 * Copyright (c) 2005 - 2007 Winbond Electronics Corp. All rights reserved.      
 *                                                                         
 * FILENAME
 *     register.h
 *
 * VERSION
 *     1.1
 *
 * DESCRIPTION
 *     This file contains the register map. The included H/W Macro functions are
 *     listed below.
 *
 *     0. System Address Map
 *     1. SDRAM Controller
 *     2. Global Controller
 *     3. Clock Controller
 *     4. GPIO
 *     5. Fast Serial Bus
 *     6. Host Bus Interface
 *     7. Flash memory Card Controller (FMI)
 *     8. Audio Interface
 *     9. USB Device
 *    10. LCM
 *    11. Sensor DSP
 *    12. Video Capture Engine
 *    13. JPEG Codec
 *    14. MPEG-4 Video Codec
 *    15. 2-D Graphic Engine
 *    16. Video Processing Engine
 *    17. Motion Estimation Engine
 *    18. High Speed UART
 *    19. UART
 *    20. Timer
 *    21. Interrupt Controller (AIC)
 *    22. Universal Serial Interface
 *    23. External Bus Interface 
 *
 * HISTORY
 *     07/06/2005		 Ver 1.0 Created by PC30 MNCheng
 *
 * REMARK
 *     None
 *     
 **********************************************************************************************************/
#ifndef _REGISTER_H
#define _REGISTER_H

/**********************************************************************************************************
 *                                                               
 * 0. System Address Map - Defines the register base address of each Macro 
 *                         function.
 *
 **********************************************************************************************************/
#define    GCR_BA    0xB0000000 /* Global Control */
#define    CLK_BA    0xB0000200 /* Clock Control */
#define    EBI_BA    0xB0001000 /* EBI Control */
#define    SDIC_BA   0xB0001800 /* SDRAM (SDR/DDR/DDR2) Control */
#define    EMC0_BA   0xB0002000 /* Ethernet MAC 0 Control */
#define    EMC1_BA   0xB0003000 /* Ethernet MAC 1 Control */
#define    GDMA_BA   0xB0004000 /* GDMA control */
#define    USBH_BA   0xB0005000 /* USB Host EHCI Control */
#define    USBD_BA   0xB0006000 /* USB Device Control */
#define    USBO_BA   0xB0007000 /* OHCI USH OHCI Control */
#define    LCM_BA    0xB0008000 /* Display, LCM Interface */
#define    ACTL_BA   0xB0009000 /* Audio Control */
#define    JPEG_BA   0xB000A000 /* JPEG Engine Control */
#define    GE_BA     0xB000B000 /* 2-D Graphic Engine */
#define    FMI_BA    0xB000D000 /* Flash Memory Card Interface */
#define    CAP_BA    0xB000E000 /* Sensor (Capture) Interface Control */
#define    CRPT_BA   0xB000F000 /* Crypto Engine Control */

#define    UART0_BA  0xB8000000 /* UART0 Control (High-Speed UART) */
#define    UART1_BA  0xB8000100 /* UART1 Control (High-Speed UART) */
#define    UART2_BA  0xB8000200 /* UART2 Control (console) */
#define    UART3_BA  0xB8000300 /* UART3 Control (micro-printer) */
#define    UART4_BA  0xB8000400 /* UART4 Control (High-Speed UART) */
#define    UART5_BA  0xB8000500 /* UART5 Control (console) */
#define    UART6_BA  0xB8000600 /* UART6 Control (High-Speed UART) */
#define    UART7_BA  0xB8000700 /* UART7 Control (console) */
#define    UART8_BA  0xB8000800 /* UART8 Control (High-Speed UART) */
#define    TMR_BA    0xB8001000 /* Timer */
#define    WDT_BA    0xB8001800 /* Watch Dog Timer */
#define    WWDT_BA   0xB8001900 /* Windows Watch Dog Timer */
#define    AIC_BA    0xB8002000 /* Interrupt Controller */
#define    GPIO_BA   0xB8003000 /* GPIO Control */
#define    RTC_BA    0xB8004000 /* Real Time Clock Control */
#define    SC0_BA    0xB8005000 /* Smart Card 0 Control */
#define    SC1_BA    0xB8005400 /* Smart Card 1 Control */
#define    I2C0_BA   0xB8006000 /* I2C 0 Control */
#define    I2C1_BA   0xB8006100 /* I2C 1 Control */
#define    USI0_BA   0xB8006200 /* Universal Serial Interface 0 */
#define    USI1_BA   0xB8006300 /* Universal Serial Interface 1 */
#define    PWM_BA    0xB8007000 /* Pulse Width Modulation (PWM) Control */
#define    KPI_BA    0xB8008000 /* Keypad Interface Control */
#define    PS2_BA    0xB8009000 /* PS2 Interface Control */
#define    ADC_BA    0xB800A000 /* ADC Control */
#define    CAN0_BA   0xB800B000 /* CAN 0 Control */
#define    CAN1_BA   0xB800B400 /* CAN 1 Control */
#define    OTP_BA    0xB800C000 /* OTP Control */
#define    IROM_BA   0xFFFF0000 /* Internal Boot ROM Space */


/**********************************************************************************************************
 *                                                               
 * 1. Global Control Registers  
 *
 **********************************************************************************************************/
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


/**********************************************************************************************************
 *                                                               
 * 2. Clock Control Registers  
 *
 **********************************************************************************************************/
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



/**********************************************************************************************************
 *                                                               
 * 3. EBI Control Registers  
 *
 **********************************************************************************************************/



/**********************************************************************************************************
 *                                                               
 * 4. GPIO Control Registers  
 *
 **********************************************************************************************************/


/**********************************************************************************************************
 *                                                               
 * 5. Fast Serial Bus Control Registers  
 *
 **********************************************************************************************************/
 

 
/**********************************************************************************************************
 *                                                               
 * 6. Host Bus Interface Control Registers  
 *
 **********************************************************************************************************/


 
/**********************************************************************************************************
 *                                                               
 * 7. Flash memory Card Control Registers  
 *
 **********************************************************************************************************/
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


 
/**********************************************************************************************************
 *                                                               
 * 8. Audio Interface Control Registers  
 *
 **********************************************************************************************************/

 
/**********************************************************************************************************
 *                                                               
 * 9. USB Device Control Registers  
 *
 **********************************************************************************************************/
#define     REG_USBD_IRQ_STAT_L		(USBD_BA+0x00)  /* interrupt status register */
#define     REG_USBD_IRQ_ENB_L		(USBD_BA+0x08)  /* interrupt enable low register */
#define     REG_USBD_IRQ_STAT		(USBD_BA+0x10)  /* usb interrupt status register */
#define     REG_USBD_IRQ_ENB		(USBD_BA+0x14)  /* usb interrupt enable register */
#define     REG_USBD_OPER			(USBD_BA+0x18)  /* usb operation register */
#define     REG_USBD_FRAME_CNT		(USBD_BA+0x1C)	/* usb frame count register */
#define     REG_USBD_ADDR			(USBD_BA+0x20)  /* usb address register */
#define     REG_USBD_CEP_DATA_BUF	(USBD_BA+0x28)  /* control-ep data buffer register */
#define     REG_USBD_CEP_CTRL_STAT	(USBD_BA+0x2C)  /* control-ep control and status register */
#define     REG_USBD_CEP_IRQ_ENB	(USBD_BA+0x30)  /* control-ep interrupt enable register */
#define     REG_USBD_CEP_IRQ_STAT	(USBD_BA+0x34)  /* control-ep interrupt status register */
#define     REG_USBD_IN_TRNSFR_CNT	(USBD_BA+0x38)  /* in-transfer data count register */
#define     REG_USBD_OUT_TRNSFR_CNT	(USBD_BA+0x3C)  /* out-transfer data count register */
#define     REG_USBD_CEP_CNT		(USBD_BA+0x40)  /* control-ep data count register */
#define     REG_USBD_SETUP1_0		(USBD_BA+0x44)  /* setup byte1 & byte0 register */
#define     REG_USBD_SETUP3_2		(USBD_BA+0x48)  /* setup byte3 & byte2 register */
#define     REG_USBD_SETUP5_4		(USBD_BA+0x4C)  /* setup byte5 & byte4 register */
#define     REG_USBD_SETUP7_6		(USBD_BA+0x50)  /* setup byte7 & byte6 register */
#define     REG_USBD_CEP_START_ADDR	(USBD_BA+0x54)  /* control-ep ram start address register */
#define     REG_USBD_CEP_END_ADDR	(USBD_BA+0x58)  /* control-ep ram end address register */
#define     REG_USBD_DMA_CTRL_STS	(USBD_BA+0x5C)  /* dma control and status register */
#define     REG_USBD_DMA_CNT		(USBD_BA+0x60)  /* dma count register */
/* endpoint A */
#define     REG_USBD_EPA_DATA_BUF	(USBD_BA+0x64)  /* endpoint A data buffer register */
#define     REG_USBD_EPA_IRQ_STAT	(USBD_BA+0x68)  /* endpoint A interrupt status register */
#define     REG_USBD_EPA_IRQ_ENB	(USBD_BA+0x6C)  /* endpoint A interrupt enable register */
#define     REG_USBD_EPA_DATA_CNT	(USBD_BA+0x70)  /* data count available in endpoint A buffer */
#define     REG_USBD_EPA_RSP_SC		(USBD_BA+0x74)  /* endpoint A response register set/clear */
#define     REG_USBD_EPA_MPS		(USBD_BA+0x78)  /* endpoint A max packet size register */
#define     REG_USBD_EPA_CNT		(USBD_BA+0x7C)  /* endpoint A transfer count register */
#define     REG_USBD_EPA_CFG		(USBD_BA+0x80)  /* endpoint A configuration register */
#define     REG_USBD_EPA_START_ADDR	(USBD_BA+0x84)  /* endpoint A ram start address register */
#define     REG_USBD_EPA_END_ADDR	(USBD_BA+0x88)  /* endpoint A ram end address register */
/* endpoint B */
#define     REG_USBD_EPB_DATA_BUF	(USBD_BA+0x8C)  /* endpoint B data buffer register */
#define     REG_USBD_EPB_IRQ_STAT	(USBD_BA+0x90)  /* endpoint B interrupt status register */
#define     REG_USBD_EPB_IRQ_ENB	(USBD_BA+0x94)  /* endpoint B interrupt enable register */
#define     REG_USBD_EPB_DATA_CNT	(USBD_BA+0x98)  /* data count available in endpoint B buffer */
#define     REG_USBD_EPB_RSP_SC		(USBD_BA+0x9C)  /* endpoint B response register set/clear */
#define     REG_USBD_EPB_MPS		(USBD_BA+0xA0)  /* endpoint B max packet size register */
#define     REG_USBD_EPB_CNT		(USBD_BA+0xA4)  /* endpoint B transfer count register */
#define     REG_USBD_EPB_CFG		(USBD_BA+0xA8)  /* endpoint B configuration register */
#define     REG_USBD_EPB_START_ADDR	(USBD_BA+0xAC)  /* endpoint B ram start address register */
#define     REG_USBD_EPB_END_ADDR	(USBD_BA+0xB0)  /* endpoint B ram end address register */
/* endpoint C */
#define     REG_USBD_EPC_DATA_BUF	(USBD_BA+0xB4)  /* endpoint C data buffer register */
#define     REG_USBD_EPC_IRQ_STAT	(USBD_BA+0xB8)  /* endpoint C interrupt status register */
#define     REG_USBD_EPC_IRQ_ENB	(USBD_BA+0xBC)  /* endpoint C interrupt enable register */
#define     REG_USBD_EPC_DATA_CNT	(USBD_BA+0xC0)  /* data count available in endpoint C buffer */
#define     REG_USBD_EPC_RSP_SC		(USBD_BA+0xC4)  /* endpoint C response register set/clear */
#define     REG_USBD_EPC_MPS		(USBD_BA+0xC8)  /* endpoint C max packet size register */
#define     REG_USBD_EPC_CNT		(USBD_BA+0xCC)  /* endpoint C transfer count register */
#define     REG_USBD_EPC_CFG		(USBD_BA+0xD0)  /* endpoint C configuration register */
#define     REG_USBD_EPC_START_ADDR	(USBD_BA+0xD4)  /* endpoint C ram start address register */
#define     REG_USBD_EPC_END_ADDR	(USBD_BA+0xD8)  /* endpoint C ram end address register */
/* endpoint D */
#define     REG_USBD_EPD_DATA_BUF	(USBD_BA+0xDC)  /* endpoint D data buffer register */
#define     REG_USBD_EPD_IRQ_STAT	(USBD_BA+0xE0)  /* endpoint D interrupt status register */
#define     REG_USBD_EPD_IRQ_ENB	(USBD_BA+0xE4)  /* endpoint D interrupt enable register */
#define     REG_USBD_EPD_DATA_CNT	(USBD_BA+0xE8)  /* data count available in endpoint D buffer */
#define     REG_USBD_EPD_RSP_SC		(USBD_BA+0xEC)  /* endpoint D response register set/clear */
#define     REG_USBD_EPD_MPS		(USBD_BA+0xF0)  /* endpoint D max packet size register */
#define     REG_USBD_EPD_CNT		(USBD_BA+0xF4)  /* endpoint D transfer count register */
#define     REG_USBD_EPD_CFG		(USBD_BA+0xF8)  /* endpoint D configuration register */
#define     REG_USBD_EPD_START_ADDR	(USBD_BA+0xFC)  /* endpoint D ram start address register */
#define     REG_USBD_EPD_END_ADDR	(USBD_BA+0x100) /* endpoint D ram end address register */
/* endpoint E */
#define     REG_USBD_EPE_DATA_BUF	(USBD_BA+0x104) /* endpoint E data buffer register */
#define     REG_USBD_EPE_IRQ_STAT	(USBD_BA+0x108) /* endpoint E interrupt status register */
#define     REG_USBD_EPE_IRQ_ENB	(USBD_BA+0x10C)	/* endpoint E interrupt enable register */
#define     REG_USBD_EPE_DATA_CNT	(USBD_BA+0x110) /* data count available in endpoint E buffer */
#define     REG_USBD_EPE_RSP_SC		(USBD_BA+0x114) /* endpoint E response register set/clear */
#define     REG_USBD_EPE_MPS		(USBD_BA+0x118) /* endpoint E max packet size register */
#define     REG_USBD_EPE_CNT		(USBD_BA+0x11C) /* endpoint E transfer count register */
#define     REG_USBD_EPE_CFG		(USBD_BA+0x120) /* endpoint E configuration register */
#define     REG_USBD_EPE_START_ADDR	(USBD_BA+0x124) /* endpoint E ram start address register */
#define     REG_USBD_EPE_END_ADDR	(USBD_BA+0x128) /* endpoint E ram end address register */
/* endpoint F */
#define     REG_USBD_EPF_DATA_BUF	(USBD_BA+0x12C) /* endpoint F data buffer register */
#define     REG_USBD_EPF_IRQ_STAT	(USBD_BA+0x130) /* endpoint F interrupt status register */
#define     REG_USBD_EPF_IRQ_ENB	(USBD_BA+0x134) /* endpoint F interrupt enable register */
#define     REG_USBD_EPF_DATA_CNT	(USBD_BA+0x138) /* data count available in endpoint F buffer */
#define     REG_USBD_EPF_RSP_SC		(USBD_BA+0x13C)	/* endpoint F response register set/clear */
#define     REG_USBD_EPF_MPS		(USBD_BA+0x140) /* endpoint F max packet size register */
#define     REG_USBD_EPF_CNT		(USBD_BA+0x144) /* endpoint F transfer count register */
#define     REG_USBD_EPF_CFG		(USBD_BA+0x148) /* endpoint F configuration register */
#define     REG_USBD_EPF_START_ADDR	(USBD_BA+0x14C) /* endpoint F ram start address register */
#define     REG_USBD_EPF_END_ADDR	(USBD_BA+0x150) /* endpoint F ram end address register */
/* endpoint G */
#define     REG_USBD_EPG_DATA_BUF	(USBD_BA+0x154) /* endpoint G data buffer register */
#define     REG_USBD_EPG_IRQ_STAT	(USBD_BA+0x158) /* endpoint G interrupt status register */
#define     REG_USBD_EPG_IRQ_ENB	(USBD_BA+0x15C)	/* endpoint G interrupt enable register */
#define     REG_USBD_EPG_DATA_CNT	(USBD_BA+0x160) /* data count available in endpoint G buffer */
#define     REG_USBD_EPG_RSP_SC		(USBD_BA+0x164) /* endpoint G response register set/clear */
#define     REG_USBD_EPG_MPS		(USBD_BA+0x168) /* endpoint G max packet size register */
#define     REG_USBD_EPG_CNT		(USBD_BA+0x16C) /* endpoint G transfer count register */
#define     REG_USBD_EPG_CFG		(USBD_BA+0x170) /* endpoint G configuration register */
#define     REG_USBD_EPG_START_ADDR	(USBD_BA+0x174) /* endpoint G ram start address register */
#define     REG_USBD_EPG_END_ADDR	(USBD_BA+0x178) /* endpoint G ram end address register */
/* endpoint H */
#define     REG_USBD_EPH_DATA_BUF	(USBD_BA+0x17C) /* endpoint H data buffer register */
#define     REG_USBD_EPH_IRQ_STAT	(USBD_BA+0x180) /* endpoint H interrupt status register */
#define     REG_USBD_EPH_IRQ_ENB	(USBD_BA+0x184) /* endpoint H interrupt enable register */
#define     REG_USBD_EPH_DATA_CNT	(USBD_BA+0x188) /* data count available in endpoint H buffer */
#define     REG_USBD_EPH_RSP_SC		(USBD_BA+0x18C)	/* endpoint H response register set/clear */
#define     REG_USBD_EPH_MPS		(USBD_BA+0x190) /* endpoint H max packet size register */
#define     REG_USBD_EPH_CNT		(USBD_BA+0x194) /* endpoint H transfer count register */
#define     REG_USBD_EPH_CFG		(USBD_BA+0x198) /* endpoint H configuration register */
#define     REG_USBD_EPH_START_ADDR	(USBD_BA+0x19C) /* endpoint H ram start address register */
#define     REG_USBD_EPH_END_ADDR	(USBD_BA+0x1A0) /* endpoint H ram end address register */
/* endpoint I */
#define     REG_USBD_EPI_DATA_BUF	(USBD_BA+0x1A4) /* endpoint I data buffer register */
#define     REG_USBD_EPI_IRQ_STAT	(USBD_BA+0x1A8) /* endpoint I interrupt status register */
#define     REG_USBD_EPI_IRQ_ENB	(USBD_BA+0x1AC)	/* endpoint I interrupt enable register */
#define     REG_USBD_EPI_DATA_CNT	(USBD_BA+0x1B0) /* data count available in endpoint I buffer */
#define     REG_USBD_EPI_RSP_SC		(USBD_BA+0x1B4) /* endpoint I response register set/clear */
#define     REG_USBD_EPI_MPS		(USBD_BA+0x1B8) /* endpoint I max packet size register */
#define     REG_USBD_EPI_CNT		(USBD_BA+0x1BC) /* endpoint I transfer count register */
#define     REG_USBD_EPI_CFG		(USBD_BA+0x1C0) /* endpoint I configuration register */
#define     REG_USBD_EPI_START_ADDR	(USBD_BA+0x1C4) /* endpoint I ram start address register */
#define     REG_USBD_EPI_END_ADDR	(USBD_BA+0x1C8) /* endpoint I ram end address register */
/* endpoint J */
#define     REG_USBD_EPJ_DATA_BUF	(USBD_BA+0x1CC) /* endpoint J data buffer register */
#define     REG_USBD_EPJ_IRQ_STAT	(USBD_BA+0x1D0) /* endpoint J interrupt status register */
#define     REG_USBD_EPJ_IRQ_ENB	(USBD_BA+0x1D4) /* endpoint J interrupt enable register */
#define     REG_USBD_EPJ_DATA_CNT	(USBD_BA+0x1D8) /* data count available in endpoint J buffer */
#define     REG_USBD_EPJ_RSP_SC		(USBD_BA+0x1DC)	/* endpoint J response register set/clear */
#define     REG_USBD_EPJ_MPS		(USBD_BA+0x1E0) /* endpoint J max packet size register */
#define     REG_USBD_EPJ_CNT		(USBD_BA+0x1E4) /* endpoint J transfer count register */
#define     REG_USBD_EPJ_CFG		(USBD_BA+0x1E8) /* endpoint J configuration register */
#define     REG_USBD_EPJ_START_ADDR	(USBD_BA+0x1EC) /* endpoint J ram start address register */
#define     REG_USBD_EPJ_END_ADDR	(USBD_BA+0x1F0) /* endpoint J ram end address register */
/* endpoint K */
#define     REG_USBD_EPK_DATA_BUF	(USBD_BA+0x1F4) /* endpoint K data buffer register */
#define     REG_USBD_EPK_IRQ_STAT	(USBD_BA+0x1F8) /* endpoint K interrupt status register */
#define     REG_USBD_EPK_IRQ_ENB	(USBD_BA+0x1FC)	/* endpoint K interrupt enable register */
#define     REG_USBD_EPK_DATA_CNT	(USBD_BA+0x200) /* data count available in endpoint K buffer */
#define     REG_USBD_EPK_RSP_SC		(USBD_BA+0x204) /* endpoint K response register set/clear */
#define     REG_USBD_EPK_MPS		(USBD_BA+0x208) /* endpoint K max packet size register */
#define     REG_USBD_EPK_CNT		(USBD_BA+0x20C) /* endpoint K transfer count register */
#define     REG_USBD_EPK_CFG		(USBD_BA+0x210) /* endpoint K configuration register */
#define     REG_USBD_EPK_START_ADDR	(USBD_BA+0x214) /* endpoint K ram start address register */
#define     REG_USBD_EPK_END_ADDR	(USBD_BA+0x218) /* endpoint K ram end address register */
/* endpoint L */
#define     REG_USBD_EPL_DATA_BUF	(USBD_BA+0x21C) /* endpoint L data buffer register */
#define     REG_USBD_EPL_IRQ_STAT	(USBD_BA+0x220) /* endpoint L interrupt status register */
#define     REG_USBD_EPL_IRQ_ENB	(USBD_BA+0x224) /* endpoint L interrupt enable register */
#define     REG_USBD_EPL_DATA_CNT	(USBD_BA+0x228) /* data count available in endpoint L buffer */
#define     REG_USBD_EPL_RSP_SC		(USBD_BA+0x22C)	/* endpoint L response register set/clear */
#define     REG_USBD_EPL_MPS		(USBD_BA+0x230) /* endpoint L max packet size register */
#define     REG_USBD_EPL_CNT		(USBD_BA+0x234) /* endpoint L transfer count register */
#define     REG_USBD_EPL_CFG		(USBD_BA+0x238) /* endpoint L configuration register */
#define     REG_USBD_EPL_START_ADDR	(USBD_BA+0x23C) /* endpoint L ram start address register */
#define     REG_USBD_EPL_END_ADDR	(USBD_BA+0x240) /* endpoint L ram end address register */

#define     REG_USBD_AHB_DMA_ADDR	(USBD_BA+0x700) /* AHB_DMA address register */
#define     REG_USBD_PHY_CTL		(USBD_BA+0x704) /* USB PHY control register */


/**********************************************************************************************************
 *                                                               
 * 10. LCM Control Registers  
 *
 **********************************************************************************************************/


/**********************************************************************************************************
 *                                                               
 * 11. Sensor DSP Control Registers  
 *
 **********************************************************************************************************/
 
 
/**********************************************************************************************************
 *                                                               
 * 12. Video Capture Engine Control Registers  
 *
 **********************************************************************************************************/

 
/**********************************************************************************************************
 *                                                               
 * 13. JPEG Codec Control Registers  
 *
 **********************************************************************************************************/
 
 
/**********************************************************************************************************
 *                                                               
 * 14. MPEG-4 Video Codec Control Registers  
 *
 **********************************************************************************************************/
 
 
/**********************************************************************************************************
 *                                                               
 * 15. 2-D Graphic Engine Control Registers  
 *
 **********************************************************************************************************/
 
 
/**********************************************************************************************************
 *                                                               
 * 16. Video Processing Engine Control Registers  
 *
 **********************************************************************************************************/
 
 
/**********************************************************************************************************
 *                                                               
 * 17. Motion Estimation Engine Control Registers  
 *
 **********************************************************************************************************/
   
 
/**********************************************************************************************************
 *                                                               
 * 18. High Speed UART Control Registers  
 *
 **********************************************************************************************************/
 
 
/**********************************************************************************************************
 *                                                               
 * 19. UART0 Control Registers  
 *
 **********************************************************************************************************/
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
 
/**********************************************************************************************************
 *                                                               
 * 20. Timer Control Registers  
 *
 **********************************************************************************************************/
#define     REG_TMR_TCSR0		(TMR_BA+0x00)  /* Timer Control and Status Register 0 */
#define     REG_TMR_TICR0		(TMR_BA+0x04)  /* Timer Initial Control Register 0 */
#define     REG_TMR_TDR0		(TMR_BA+0x08)  /* Timer Data Register 0 */
#define     REG_TMR_TCSR1		(TMR_BA+0x10)  /* Timer Control and Status Register 1 */
#define     REG_TMR_TICR1		(TMR_BA+0x14)  /* Timer Initial Control Register 1 */
#define     REG_TMR_TDR1		(TMR_BA+0x18)  /* Timer Data Register 1 */
#define     REG_TMR_TCSR2		(TMR_BA+0x20)  /* Timer Control and Status Register 2 */
#define     REG_TMR_TICR2		(TMR_BA+0x24)  /* Timer Initial Control Register 2 */
#define     REG_TMR_TDR2		(TMR_BA+0x28)  /* Timer Data Register 2 */
#define     REG_TMR_TCSR3		(TMR_BA+0x30)  /* Timer Control and Status Register 3 */
#define     REG_TMR_TICR3		(TMR_BA+0x34)  /* Timer Initial Control Register 3 */
#define     REG_TMR_TDR3		(TMR_BA+0x38)  /* Timer Data Register 3 */
#define     REG_TMR_TCSR4		(TMR_BA+0x40)  /* Timer Control and Status Register 4 */
#define     REG_TMR_TICR4		(TMR_BA+0x44)  /* Timer Initial Control Register 4 */
#define     REG_TMR_TDR4		(TMR_BA+0x48)  /* Timer Data Register 4 */
#define     REG_TMR_TISR		(TMR_BA+0x60)  /* Timer Interrupt Status Register */

/*
  Watch Dog Timer Registers 
*/  
#define REG_WDT_CR				(WDT_BA + 0x00)	/* WDT control register */
#define REG_WDT_CRALT			(WDT_BA + 0x04)	/* WDT alternative control register */

 
/**********************************************************************************************************
 *                                                               
 * 21. Interrupt Control Registers  
 *
 **********************************************************************************************************/
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

  
/**********************************************************************************************************
 *                                                               
 * 22. Universal Serial Interface Control Registers  
 *
 **********************************************************************************************************/
#define     REG_USI_CNTRL	(USI0_BA+0x00)  /* Control and Status Register */
#define     REG_USI_DIVIDER	(USI0_BA+0x04)  /* Clock Divider Register */
#define     REG_USI_SSR		(USI0_BA+0x08)  /* Slave Select Register */
#define     REG_USI_Rx0		(USI0_BA+0x10)  /* Data Receive Register 0 */
#define     REG_USI_Rx1		(USI0_BA+0x14)  /* Data Receive Register 1 */
#define     REG_USI_Rx2		(USI0_BA+0x18)  /* Data Receive Register 2 */
#define     REG_USI_Rx3		(USI0_BA+0x1C)  /* Data Receive Register 3 */
#define     REG_USI_Tx0		(USI0_BA+0x10)  /* Data Transmit Register 0 */
#define     REG_USI_Tx1		(USI0_BA+0x14)  /* Data Transmit Register 1 */
#define     REG_USI_Tx2		(USI0_BA+0x18)  /* Data Transmit Register 2 */
#define     REG_USI_Tx3		(USI0_BA+0x1C)  /* Data Transmit Register 3 */
 
 
/**********************************************************************************************************
 *                                                               
 * 23. External Bus Interface Control Registers  
 *
 **********************************************************************************************************/




/**********************************************************************************************************
 *                                                               
 * 24. OTP Control Registers
 *
 **********************************************************************************************************/
#define     REG_OTP_KEYEN	(OTP_BA+0x00)  /* Key Enable Register */
#define     REG_OTP_OPTION	(OTP_BA+0x0C)  /* OTP Option Register */
#define     REG_OTP_VALUE0	(OTP_BA+0x10)  /* OTP Value 0 Register */
#define     REG_OTP_VALUE1	(OTP_BA+0x14)  /* OTP Value 1 Register */
#define     REG_OTP_VALUE2	(OTP_BA+0x18)  /* OTP Value 2 Register */
#define     REG_OTP_VALUE3	(OTP_BA+0x1C)  /* OTP Value 3 Register */
#define     REG_OTP_VALUE4	(OTP_BA+0x20)  /* OTP Value 4 Register */
#define     REG_OTP_VALUE5	(OTP_BA+0x24)  /* OTP Value 5 Register */
#define     REG_OTP_VALUE6	(OTP_BA+0x28)  /* OTP Value 6 Register */
#define     REG_OTP_VALUE7	(OTP_BA+0x2C)  /* OTP Value 7 Register */
#define     REG_OTP_CYCLE	(OTP_BA+0x30)  /* Cycle Count Register */
#define     REG_OTP_MODE	(OTP_BA+0x34)  /* OTP Mode Register */
#define     REG_OTP_START	(OTP_BA+0x38)  /* OTP Program Start Register */
#define     REG_OTP_STATUS	(OTP_BA+0x40)  /* OTP Status Register */
#define     REG_OTP_PROTECT	(OTP_BA+0x50)  /* OTP Protect Register */


/**********************************************************************************************************
 *                                                               
 * 25. Crypto Engine Control Registers
 *
 **********************************************************************************************************/
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


/**********************************************************************************************************
 *
 * 26. RTC Control Registers
 *
 **********************************************************************************************************/
#define		REG_RTC_INIR			(RTC_BA+0x0000)	/* Initial register */
#define		REG_RTC_AER			(RTC_BA+0x0004)	/* Access Enable Register*/
#define		REG_RTC_INTSTS			(RTC_BA+0x002C)	/* Interrupt Status */
#define		REG_RTC_SPARE			(RTC_BA+0x0040)	/* Spare register */

 
#endif /* _REGISTER_H */
