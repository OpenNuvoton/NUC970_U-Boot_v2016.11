/*
 * NUC970 GPIO header file
 * Copyright (c) 2015 Nuvoton Technology Corporation
 * All rights reserved.
 */

#ifndef __NUC970_REGS_GPIO_H
#define __NUC970_REGS_GPIO_H

/* Global control registers */
#define REG_MFP_GPA_L   0xB0000070
#define GPIO_BA		0xB8003000

#define REG_GPIOA_DIR 		(GPIO_BA+0x000)
#define REG_GPIOA_DATAOUT 	(GPIO_BA+0x004)
#define REG_GPIOA_DATAIN 	(GPIO_BA+0x008)
#define REG_GPIOA_IMD 		(GPIO_BA+0x00C)
#define REG_GPIOA_IREN 		(GPIO_BA+0x010)
#define REG_GPIOA_IFEN 		(GPIO_BA+0x014)
#define REG_GPIOA_ISR 		(GPIO_BA+0x018)
#define REG_GPIOA_DBEN 		(GPIO_BA+0x01C)
#define REG_GPIOA_PUEN 		(GPIO_BA+0x020)
#define REG_GPIOA_PDEN 		(GPIO_BA+0x024)
#define REG_GPIOA_ICEN 		(GPIO_BA+0x028)
#define REG_GPIOA_ISEN 		(GPIO_BA+0x02C)
	
#define REG_GPIOB_DIR 		(GPIO_BA+0x040)              
#define REG_GPIOB_DATAOUT 	(GPIO_BA+0x044)            
#define REG_GPIOB_DATAIN 	(GPIO_BA+0x048)              
#define REG_GPIOB_IMD 		(GPIO_BA+0x04C)              
#define REG_GPIOB_IREN 		(GPIO_BA+0x050)              
#define REG_GPIOB_IFEN 		(GPIO_BA+0x054)              
#define REG_GPIOB_ISR 		(GPIO_BA+0x058)              
#define REG_GPIOB_DBEN 		(GPIO_BA+0x05C)              
#define REG_GPIOB_PUEN 		(GPIO_BA+0x060)              
#define REG_GPIOB_PDEN 		(GPIO_BA+0x064)              
#define REG_GPIOB_ICEN 		(GPIO_BA+0x068)              
#define REG_GPIOB_ISEN 		(GPIO_BA+0x06C)              
                                                     
#define REG_GPIOC_DIR 		(GPIO_BA+0x080)              
#define REG_GPIOC_DATAOUT 	(GPIO_BA+0x084)            
#define REG_GPIOC_DATAIN 	(GPIO_BA+0x088)              
#define REG_GPIOC_IMD 		(GPIO_BA+0x08C)              
#define REG_GPIOC_IREN 		(GPIO_BA+0x090)              
#define REG_GPIOC_IFEN 		(GPIO_BA+0x094)              
#define REG_GPIOC_ISR 		(GPIO_BA+0x098)              
#define REG_GPIOC_DBEN 		(GPIO_BA+0x09C)              
#define REG_GPIOC_PUEN 		(GPIO_BA+0x0A0)              
#define REG_GPIOC_PDEN 		(GPIO_BA+0x0A4)              
#define REG_GPIOC_ICEN 		(GPIO_BA+0x0A8)              
#define REG_GPIOC_ISEN 		(GPIO_BA+0x0AC)              
                                                     
#define REG_GPIOD_DIR 		(GPIO_BA+0x0C0)              
#define REG_GPIOD_DATAOUT 	(GPIO_BA+0x0C4)            
#define REG_GPIOD_DATAIN 	(GPIO_BA+0x0C8)              
#define REG_GPIOD_IMD 		(GPIO_BA+0x0CC)              
#define REG_GPIOD_IREN 		(GPIO_BA+0x0D0)              
#define REG_GPIOD_IFEN 		(GPIO_BA+0x0D4)              
#define REG_GPIOD_ISR 		(GPIO_BA+0x0D8)              
#define REG_GPIOD_DBEN 		(GPIO_BA+0x0DC)              
#define REG_GPIOD_PUEN 		(GPIO_BA+0x0E0)              
#define REG_GPIOD_PDEN 		(GPIO_BA+0x0E4)              
#define REG_GPIOD_ICEN 		(GPIO_BA+0x0E8)              
#define REG_GPIOD_ISEN 		(GPIO_BA+0x0EC)              
                                                     
#define REG_GPIOE_DIR 		(GPIO_BA+0x100)              
#define REG_GPIOE_DATAOUT 	(GPIO_BA+0x104)            
#define REG_GPIOE_DATAIN 	(GPIO_BA+0x108)              
#define REG_GPIOE_IMD 		(GPIO_BA+0x10C)              
#define REG_GPIOE_IREN 		(GPIO_BA+0x110)              
#define REG_GPIOE_IFEN 		(GPIO_BA+0x114)              
#define REG_GPIOE_ISR 		(GPIO_BA+0x118)              
#define REG_GPIOE_DBEN 		(GPIO_BA+0x11C)              
#define REG_GPIOE_PUEN 		(GPIO_BA+0x120)              
#define REG_GPIOE_PDEN 		(GPIO_BA+0x124)              
#define REG_GPIOE_ICEN 		(GPIO_BA+0x128)              
#define REG_GPIOE_ISEN 		(GPIO_BA+0x12C)              
                                                     
#define REG_GPIOF_DIR 		(GPIO_BA+0x140)              
#define REG_GPIOF_DATAOUT 	(GPIO_BA+0x144)
#define REG_GPIOF_DATAIN 	(GPIO_BA+0x148)              
#define REG_GPIOF_IMD 		(GPIO_BA+0x14C)              
#define REG_GPIOF_IREN 		(GPIO_BA+0x150)              
#define REG_GPIOF_IFEN 		(GPIO_BA+0x154)              
#define REG_GPIOF_ISR 		(GPIO_BA+0x158)              
#define REG_GPIOF_DBEN 		(GPIO_BA+0x15C)              
#define REG_GPIOF_PUEN 		(GPIO_BA+0x160)             
#define REG_GPIOF_PDEN 		(GPIO_BA+0x164)              
#define REG_GPIOF_ICEN 		(GPIO_BA+0x168)              
#define REG_GPIOF_ISEN 		(GPIO_BA+0x16C)              
	                                                     
#define REG_GPIOG_DIR 		(GPIO_BA+0x180)              
#define REG_GPIOG_DATAOUT 	(GPIO_BA+0x184)
#define REG_GPIOG_DATAIN 	(GPIO_BA+0x188)              
#define REG_GPIOG_IMD 		(GPIO_BA+0x18C)              
#define REG_GPIOG_IREN 		(GPIO_BA+0x190)              
#define REG_GPIOG_IFEN 		(GPIO_BA+0x194)              
#define REG_GPIOG_ISR 		(GPIO_BA+0x198)              
#define REG_GPIOG_DBEN 		(GPIO_BA+0x19C)              
#define REG_GPIOG_PUEN 		(GPIO_BA+0x1A0)              
#define REG_GPIOG_PDEN 		(GPIO_BA+0x1A4)             
#define REG_GPIOG_ICEN 		(GPIO_BA+0x1A8)              
#define REG_GPIOG_ISEN 		(GPIO_BA+0x1AC)
                                                     
#define REG_GPIOH_DIR 		(GPIO_BA+0x1C0)              
#define REG_GPIOH_DATAOUT 	(GPIO_BA+0x1C4)            
#define REG_GPIOH_DATAIN 	(GPIO_BA+0x1C8)              
#define REG_GPIOH_IMD 		(GPIO_BA+0x1CC)              
#define REG_GPIOH_IREN 		(GPIO_BA+0x1D0)              
#define REG_GPIOH_IFEN 		(GPIO_BA+0x1D4)
#define REG_GPIOH_ISR 		(GPIO_BA+0x1D8)              
#define REG_GPIOH_DBEN 		(GPIO_BA+0x1DC)              
#define REG_GPIOH_PUEN 		(GPIO_BA+0x1E0)              
#define REG_GPIOH_PDEN 		(GPIO_BA+0x1E4)              
#define REG_GPIOH_ICEN 		(GPIO_BA+0x1E8)              
#define REG_GPIOH_ISEN 		(GPIO_BA+0x1EC)              
	                                                     
#define REG_GPIOI_DIR 		(GPIO_BA+0x200)              
#define REG_GPIOI_DATAOUT 	(GPIO_BA+0x204)            
#define REG_GPIOI_DATAIN 	(GPIO_BA+0x208)              
#define REG_GPIOI_IMD 		(GPIO_BA+0x20C)              
#define REG_GPIOI_IREN 		(GPIO_BA+0x210)              
#define REG_GPIOI_IFEN 		(GPIO_BA+0x214)              
#define REG_GPIOI_ISR 		(GPIO_BA+0x218)              
#define REG_GPIOI_DBEN 		(GPIO_BA+0x21C)              
#define REG_GPIOI_PUEN 		(GPIO_BA+0x220)              
#define REG_GPIOI_PDEN 		(GPIO_BA+0x224)              
#define REG_GPIOI_ICEN 		(GPIO_BA+0x228)              
#define REG_GPIOI_ISEN 		(GPIO_BA+0x22C)  


#define REG_GPIOJ_DIR 		(GPIO_BA+0x240)              
#define REG_GPIOJ_DATAOUT	(GPIO_BA+0x244)            
#define REG_GPIOJ_DATAIN 	(GPIO_BA+0x248)              
#define REG_GPIOJ_IMD 		(GPIO_BA+0x24C)              
#define REG_GPIOJ_IREN 		(GPIO_BA+0x250)              
#define REG_GPIOJ_IFEN 		(GPIO_BA+0x254)              
#define REG_GPIOJ_ISR 		(GPIO_BA+0x258)              
#define REG_GPIOJ_DBEN 		(GPIO_BA+0x25C)              
#define REG_GPIOJ_PUEN 		(GPIO_BA+0x260)              
#define REG_GPIOJ_PDEN 		(GPIO_BA+0x264)              
#define REG_GPIOJ_ICEN 		(GPIO_BA+0x268)              
#define REG_GPIOJ_ISEN 		(GPIO_BA+0x26C) 
                                               
#define REG_GPIO_DBNCECON	(GPIO_BA+0x3F0)              
#define REG_GPIO_ISR		  (GPIO_BA+0x3FC)

#define GPIO_OFFSET 0x20
#define	DRIVER_NAME "nuc970-gpio"
#define NUMGPIO 0x20 * 10	//(PortA~Portj)

#define GPIO_PA0	0
#define GPIO_PA1	1
#define GPIO_PA2	2
#define GPIO_PA3	3
#define GPIO_PA4	4
#define GPIO_PA5	5
#define GPIO_PA6	6
#define GPIO_PA7	7
#define GPIO_PA8	8
#define GPIO_PA9	9
#define GPIO_PA10	10
#define GPIO_PA11	11
#define GPIO_PA12	12
#define GPIO_PA13	13
#define GPIO_PA14	14
#define GPIO_PA15	15
#define GPIO_PB0	32
#define GPIO_PB1	33
#define GPIO_PB2	34
#define GPIO_PB3	35
#define GPIO_PB4	36
#define GPIO_PB5	37
#define GPIO_PB6	38
#define GPIO_PB7	39
#define GPIO_PB8	40
#define GPIO_PB9	41
#define GPIO_PB10	42
#define GPIO_PB11	43
#define GPIO_PB12	44
#define GPIO_PB13	45
#define GPIO_PB14	46
#define GPIO_PB15	47
#define GPIO_PC0	(2*GPIO_OFFSET + 0)
#define GPIO_PC1	(2*GPIO_OFFSET + 1)
#define GPIO_PC2	(2*GPIO_OFFSET + 2)
#define GPIO_PC3	(2*GPIO_OFFSET + 3)
#define GPIO_PC4	(2*GPIO_OFFSET + 4)
#define GPIO_PC5	(2*GPIO_OFFSET + 5)
#define GPIO_PC6	(2*GPIO_OFFSET + 6)
#define GPIO_PC7	(2*GPIO_OFFSET + 7)
#define GPIO_PC8	(2*GPIO_OFFSET + 8)
#define GPIO_PC9	(2*GPIO_OFFSET + 9)
#define GPIO_PC10	(2*GPIO_OFFSET + 10)
#define GPIO_PC11	(2*GPIO_OFFSET + 11)
#define GPIO_PC12	(2*GPIO_OFFSET + 12)
#define GPIO_PC13	(2*GPIO_OFFSET + 13)
#define GPIO_PC14	(2*GPIO_OFFSET + 14)
#define GPIO_PC15	(2*GPIO_OFFSET + 15)
#define GPIO_PD0	(3*GPIO_OFFSET + 0)
#define GPIO_PD1	(3*GPIO_OFFSET + 1)
#define GPIO_PD2	(3*GPIO_OFFSET + 2)
#define GPIO_PD3	(3*GPIO_OFFSET + 3)
#define GPIO_PD4	(3*GPIO_OFFSET + 4)
#define GPIO_PD5	(3*GPIO_OFFSET + 5)
#define GPIO_PD6	(3*GPIO_OFFSET + 6)
#define GPIO_PD7	(3*GPIO_OFFSET + 7)
#define GPIO_PD8	(3*GPIO_OFFSET + 8)
#define GPIO_PD9	(3*GPIO_OFFSET + 9)
#define GPIO_PD10	(3*GPIO_OFFSET + 10)
#define GPIO_PD11	(3*GPIO_OFFSET + 11)
#define GPIO_PD12	(3*GPIO_OFFSET + 12)
#define GPIO_PD13	(3*GPIO_OFFSET + 13)
#define GPIO_PD14	(3*GPIO_OFFSET + 14)
#define GPIO_PD15	(3*GPIO_OFFSET + 15)
#define GPIO_PE0	(4*GPIO_OFFSET + 0)
#define GPIO_PE1	(4*GPIO_OFFSET + 1)
#define GPIO_PE2	(4*GPIO_OFFSET + 2)
#define GPIO_PE3	(4*GPIO_OFFSET + 3)
#define GPIO_PE4	(4*GPIO_OFFSET + 4)
#define GPIO_PE5	(4*GPIO_OFFSET + 5)
#define GPIO_PE6	(4*GPIO_OFFSET + 6)
#define GPIO_PE7	(4*GPIO_OFFSET + 7)
#define GPIO_PE8	(4*GPIO_OFFSET + 8)
#define GPIO_PE9	(4*GPIO_OFFSET + 9)
#define GPIO_PE10	(4*GPIO_OFFSET + 10)
#define GPIO_PE11	(4*GPIO_OFFSET + 11)
#define GPIO_PE12	(4*GPIO_OFFSET + 12)
#define GPIO_PE13	(4*GPIO_OFFSET + 13)
#define GPIO_PE14	(4*GPIO_OFFSET + 14)
#define GPIO_PE15	(4*GPIO_OFFSET + 15)
#define GPIO_PF0	(5*GPIO_OFFSET + 0)
#define GPIO_PF1	(5*GPIO_OFFSET + 1)
#define GPIO_PF2	(5*GPIO_OFFSET + 2)
#define GPIO_PF3	(5*GPIO_OFFSET + 3)
#define GPIO_PF4	(5*GPIO_OFFSET + 4)
#define GPIO_PF5	(5*GPIO_OFFSET + 5)
#define GPIO_PF6	(5*GPIO_OFFSET + 6)
#define GPIO_PF7	(5*GPIO_OFFSET + 7)
#define GPIO_PF8	(5*GPIO_OFFSET + 8)
#define GPIO_PF9	(5*GPIO_OFFSET + 9)
#define GPIO_PF10	(5*GPIO_OFFSET + 10)
#define GPIO_PF11	(5*GPIO_OFFSET + 11)
#define GPIO_PF12	(5*GPIO_OFFSET + 12)
#define GPIO_PF13	(5*GPIO_OFFSET + 13)
#define GPIO_PF14	(5*GPIO_OFFSET + 14)
#define GPIO_PF15	(5*GPIO_OFFSET + 15)
#define GPIO_PG0	(6*GPIO_OFFSET + 0)
#define GPIO_PG1	(6*GPIO_OFFSET + 1)
#define GPIO_PG2	(6*GPIO_OFFSET + 2)
#define GPIO_PG3	(6*GPIO_OFFSET + 3)
#define GPIO_PG4	(6*GPIO_OFFSET + 4)
#define GPIO_PG5	(6*GPIO_OFFSET + 5)
#define GPIO_PG6	(6*GPIO_OFFSET + 6)
#define GPIO_PG7	(6*GPIO_OFFSET + 7)
#define GPIO_PG8	(6*GPIO_OFFSET + 8)
#define GPIO_PG9	(6*GPIO_OFFSET + 9)
#define GPIO_PG10	(6*GPIO_OFFSET + 10)
#define GPIO_PG11	(6*GPIO_OFFSET + 11)
#define GPIO_PG12	(6*GPIO_OFFSET + 12)
#define GPIO_PG13	(6*GPIO_OFFSET + 13)
#define GPIO_PG14	(6*GPIO_OFFSET + 14)
#define GPIO_PG15	(6*GPIO_OFFSET + 15)
#define GPIO_PH0	(7*GPIO_OFFSET + 0)
#define GPIO_PH1	(7*GPIO_OFFSET + 1)
#define GPIO_PH2	(7*GPIO_OFFSET + 2)
#define GPIO_PH3	(7*GPIO_OFFSET + 3)
#define GPIO_PH4	(7*GPIO_OFFSET + 4)
#define GPIO_PH5	(7*GPIO_OFFSET + 5)
#define GPIO_PH6	(7*GPIO_OFFSET + 6)
#define GPIO_PH7	(7*GPIO_OFFSET + 7)
#define GPIO_PH8	(7*GPIO_OFFSET + 8)
#define GPIO_PH9	(7*GPIO_OFFSET + 9)
#define GPIO_PH10	(7*GPIO_OFFSET + 10)
#define GPIO_PH11	(7*GPIO_OFFSET + 11)
#define GPIO_PH12	(7*GPIO_OFFSET + 12)
#define GPIO_PH13	(7*GPIO_OFFSET + 13)
#define GPIO_PH14	(7*GPIO_OFFSET + 14)
#define GPIO_PH15	(7*GPIO_OFFSET + 15)
#define GPIO_PI0	(8*GPIO_OFFSET + 0)
#define GPIO_PI1	(8*GPIO_OFFSET + 1)
#define GPIO_PI2	(8*GPIO_OFFSET + 2)
#define GPIO_PI3	(8*GPIO_OFFSET + 3)
#define GPIO_PI4	(8*GPIO_OFFSET + 4)
#define GPIO_PI5	(8*GPIO_OFFSET + 5)
#define GPIO_PI6	(8*GPIO_OFFSET + 6)
#define GPIO_PI7	(8*GPIO_OFFSET + 7)
#define GPIO_PI8	(8*GPIO_OFFSET + 8)
#define GPIO_PI9	(8*GPIO_OFFSET + 9)
#define GPIO_PI10	(8*GPIO_OFFSET + 10)
#define GPIO_PI11	(8*GPIO_OFFSET + 11)
#define GPIO_PI12	(8*GPIO_OFFSET + 12)
#define GPIO_PI13	(8*GPIO_OFFSET + 13)
#define GPIO_PI14	(8*GPIO_OFFSET + 14)
#define GPIO_PI15	(8*GPIO_OFFSET + 15)
#define GPIO_PJ0	(9*GPIO_OFFSET + 0)
#define GPIO_PJ1	(9*GPIO_OFFSET + 1)
#define GPIO_PJ2	(9*GPIO_OFFSET + 2)
#define GPIO_PJ3	(9*GPIO_OFFSET + 3)
#define GPIO_PJ4	(9*GPIO_OFFSET + 4)
#define GPIO_PJ5	(9*GPIO_OFFSET + 5)
#define GPIO_PJ6	(9*GPIO_OFFSET + 6)
#define GPIO_PJ7	(9*GPIO_OFFSET + 7)
#define GPIO_PJ8	(9*GPIO_OFFSET + 8)
#define GPIO_PJ9	(9*GPIO_OFFSET + 9)
#define GPIO_PJ10	(9*GPIO_OFFSET + 10)
#define GPIO_PJ11	(9*GPIO_OFFSET + 11)
#define GPIO_PJ12	(9*GPIO_OFFSET + 12)
#define GPIO_PJ13	(9*GPIO_OFFSET + 13)
#define GPIO_PJ14	(9*GPIO_OFFSET + 14)
#define GPIO_PJ15	(9*GPIO_OFFSET + 15)

#endif /*  __NUC970_REGS_GPIO_H */
