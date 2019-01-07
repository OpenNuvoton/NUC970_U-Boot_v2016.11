/*
 * NUC970 GPIO header file
 * Copyright (c) 2015 Nuvoton Technology Corporation
 * All rights reserved.
 */

#ifndef __N32926_REGS_GPIO_H
#define __N32926_REGS_GPIO_H

/* Global control registers */
#define REG_MFP_GPA_L   0xB0000080
#define GPIO_BA		0xB8001000

#define REG_GPIOA_DIR 		(GPIO_BA+0x000)
#define REG_GPIOA_PUEN 		(GPIO_BA+0x004)
#define REG_GPIOA_DATAOUT 	(GPIO_BA+0x008)
#define REG_GPIOA_DATAIN 	(GPIO_BA+0x00C)
	
#define REG_GPIOB_DIR 		(GPIO_BA+0x010)              
#define REG_GPIOB_PUEN 		(GPIO_BA+0x014)              
#define REG_GPIOB_DATAOUT 	(GPIO_BA+0x018)            
#define REG_GPIOB_DATAIN 	(GPIO_BA+0x01C)              
                                                  
#define REG_GPIOC_DIR 		(GPIO_BA+0x020)              
#define REG_GPIOC_PUEN 		(GPIO_BA+0x024)              
#define REG_GPIOC_DATAOUT 	(GPIO_BA+0x028)            
#define REG_GPIOC_DATAIN 	(GPIO_BA+0x02C)              
                                                     
#define REG_GPIOD_DIR 		(GPIO_BA+0x030)              
#define REG_GPIOD_PUEN 		(GPIO_BA+0x034)              
#define REG_GPIOD_DATAOUT 	(GPIO_BA+0x038)            
#define REG_GPIOD_DATAIN 	(GPIO_BA+0x03C)              
                                                     
#define REG_GPIOE_DIR 		(GPIO_BA+0x040)              
#define REG_GPIOE_PUEN 		(GPIO_BA+0x044)              
#define REG_GPIOE_DATAOUT 	(GPIO_BA+0x048)            
#define REG_GPIOE_DATAIN 	(GPIO_BA+0x04C)              
                                                     
#define REG_GPIOG_DIR 		(GPIO_BA+0x050)              
#define REG_GPIOG_PUEN 		(GPIO_BA+0x054)              
#define REG_GPIOG_DATAOUT 	(GPIO_BA+0x058)
#define REG_GPIOG_DATAIN 	(GPIO_BA+0x05C)              
                                                     
#define REG_GPIOH_DIR 		(GPIO_BA+0x060)              
#define REG_GPIOH_PUEN 		(GPIO_BA+0x064)              
#define REG_GPIOH_DATAOUT 	(GPIO_BA+0x068)            
#define REG_GPIOH_DATAIN 	(GPIO_BA+0x06C)              
	                                                     
#define GPIO_OFFSET 0x20
#define	DRIVER_NAME "n32926-gpio"
#define NUMGPIO 0x20 * 7	//(PortA~PortH)

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
#define GPIO_PB0	(1*GPIO_OFFSET + 0) 
#define GPIO_PB1	(1*GPIO_OFFSET + 1) 
#define GPIO_PB2	(1*GPIO_OFFSET + 2) 
#define GPIO_PB3	(1*GPIO_OFFSET + 3) 
#define GPIO_PB4	(1*GPIO_OFFSET + 4) 
#define GPIO_PB5	(1*GPIO_OFFSET + 5) 
#define GPIO_PB6	(1*GPIO_OFFSET + 6) 
#define GPIO_PB7	(1*GPIO_OFFSET + 7) 
#define GPIO_PB8	(1*GPIO_OFFSET + 8) 
#define GPIO_PB9	(1*GPIO_OFFSET + 9) 
#define GPIO_PB10	(1*GPIO_OFFSET + 10)
#define GPIO_PB11	(1*GPIO_OFFSET + 11)
#define GPIO_PB12	(1*GPIO_OFFSET + 12)
#define GPIO_PB13	(1*GPIO_OFFSET + 13)
#define GPIO_PB14	(1*GPIO_OFFSET + 14)
#define GPIO_PB15	(1*GPIO_OFFSET + 15)
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
#define GPIO_PG0	(5*GPIO_OFFSET + 0)
#define GPIO_PG1	(5*GPIO_OFFSET + 1)
#define GPIO_PG2	(5*GPIO_OFFSET + 2)
#define GPIO_PG3	(5*GPIO_OFFSET + 3)
#define GPIO_PG4	(5*GPIO_OFFSET + 4)
#define GPIO_PG5	(5*GPIO_OFFSET + 5)
#define GPIO_PG6	(5*GPIO_OFFSET + 6)
#define GPIO_PG7	(5*GPIO_OFFSET + 7)
#define GPIO_PG8	(5*GPIO_OFFSET + 8)
#define GPIO_PG9	(5*GPIO_OFFSET + 9)
#define GPIO_PG10	(5*GPIO_OFFSET + 10)
#define GPIO_PG11	(5*GPIO_OFFSET + 11)
#define GPIO_PG12	(5*GPIO_OFFSET + 12)
#define GPIO_PG13	(5*GPIO_OFFSET + 13)
#define GPIO_PG14	(5*GPIO_OFFSET + 14)
#define GPIO_PG15	(5*GPIO_OFFSET + 15)
#define GPIO_PH0	(6*GPIO_OFFSET + 0)
#define GPIO_PH1	(6*GPIO_OFFSET + 1)
#define GPIO_PH2	(6*GPIO_OFFSET + 2)
#define GPIO_PH3	(6*GPIO_OFFSET + 3)
#define GPIO_PH4	(6*GPIO_OFFSET + 4)
#define GPIO_PH5	(6*GPIO_OFFSET + 5)
#define GPIO_PH6	(6*GPIO_OFFSET + 6)
#define GPIO_PH7	(6*GPIO_OFFSET + 7)
#define GPIO_PH8	(6*GPIO_OFFSET + 8)
#define GPIO_PH9	(6*GPIO_OFFSET + 9)
#define GPIO_PH10	(6*GPIO_OFFSET + 10)
#define GPIO_PH11	(6*GPIO_OFFSET + 11)
#define GPIO_PH12	(6*GPIO_OFFSET + 12)
#define GPIO_PH13	(6*GPIO_OFFSET + 13)
#define GPIO_PH14	(6*GPIO_OFFSET + 14)
#define GPIO_PH15	(6*GPIO_OFFSET + 15)

#endif /*  __N32926_REGS_GPIO_H */
