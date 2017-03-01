/*
 * Copyright (c) 2014 Nuvoton technology corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <common.h>
#include <asm/io.h>
#include <nuc970_keypad.h>

typedef struct
{
  	volatile unsigned int CONF;				        /*!< Offset:  0x0000 Keypad controller configuration Register */
  	volatile unsigned int THREEKCONF;				/*!< Offset:  0x0004 Keypad controller 3-keys configuration register */
  	volatile unsigned int STATUS;				        /*!< Offset:  0x0008 Keypad status register */
  	volatile unsigned int RSTC;			        	/*!< Offset:  0x000C Keypad  Reset Period Controller register */
  	volatile unsigned int KEST;				        /*!< Offset:  0x0010 Keypad Key State Indicator */
        volatile unsigned int RESERVED0;                                /*!< Offset:  0x0014 Reserved */
  	volatile unsigned int KPE;				        /*!< Offset:  0x0018 Press Key Event Indicator */
  	volatile unsigned int RESERVED1;                                /*!< Offset:  0x001C Reserved */
  	volatile unsigned int KRE;				        /*!< Offset:  0x0020 Release Key Event Indicator */
  	volatile unsigned int RESERVED2;                                /*!< Offset:  0x0024 reserved */
  	volatile unsigned int PRESCALDIV;				/*!< Offset:  0x0028 Pre-Scale Divider */
} KPI_TypeDef;

#define GCR_BA    0xB0000000 /* Global Control */
#define REG_MFP_GPA_L	(GCR_BA+0x070)  /* GPIOA Low Byte Multiple Function Control Register */
#define REG_MFP_GPA_H	(GCR_BA+0x074)  /* GPIOA High Byte Multiple Function Control Register */
#define REG_MFP_GPH_L	(GCR_BA+0x0A8)  /* GPIOH Low Byte Multiple Function Control Register */
#define REG_MFP_GPH_H	(GCR_BA+0x0AC)  /* GPIOH High Byte Multiple Function Control Register */
#define rMFP_GPA_L      (*(unsigned int volatile *)REG_MFP_GPA_L)
#define rMFP_GPA_H      (*(unsigned int volatile *)REG_MFP_GPA_H)
#define rMFP_GPH_L      (*(unsigned int volatile *)REG_MFP_GPH_L)
#define rMFP_GPH_H      (*(unsigned int volatile *)REG_MFP_GPH_H)


#define REG_CLKDIV7   (0xB0000200+0x3C)
#define rCLKDIV7      (*(unsigned int volatile *)REG_CLKDIV7)
#define CLKDIV7_KPI_S ((unsigned int)0x00008000)
#define CLKDIV7_KPI_N ((unsigned int)0x00007F00)

#define rCLKPMCON      (*(unsigned int volatile *)(0xB0000200))

#define GPIO_BA                 0xB8003000       /* GPIO Control */
#define REG_GPIOA_DIR 		(GPIO_BA+0x000) /* GPIO Port A Direction Control Register */
#define REG_GPIOA_PUEN 		(GPIO_BA+0x020) /* GPIO Port A Pull-Up Enable Register */
#define REG_GPIOA_PDEN 		(GPIO_BA+0x024) /* GPIO Port A Pull-Down Enable Register */
#define REG_GPIOH_DIR 		(GPIO_BA+0x1C0) /* GPIO Port H Direction Control Register */
#define REG_GPIOH_PUEN 		(GPIO_BA+0x1E0) /* GPIO Port H Pull-Up Enable Register */
#define REG_GPIOH_PDEN 		(GPIO_BA+0x1E4) /* GPIO Port H Pull-Down Enable Register */
#define rGPIOA_DIR              (*(unsigned int volatile *)REG_GPIOA_DIR)
#define rGPIOA_PUEN             (*(unsigned int volatile *)REG_GPIOA_PUEN)
#define rGPIOA_PDEN             (*(unsigned int volatile *)REG_GPIOA_PDEN)
#define rGPIOH_DIR              (*(unsigned int volatile *)REG_GPIOH_DIR)
#define rGPIOH_PUEN             (*(unsigned int volatile *)REG_GPIOH_PUEN)
#define rGPIOH_PDEN             (*(unsigned int volatile *)REG_GPIOH_PDEN)


#define REG_PCLK0EN   (0xB0000200+0x18)
#define REG_PCLK1EN   (0xB0000200+0x1C)
#define rPCLK0EN      (*(unsigned int volatile *)REG_PCLK0EN)
#define rPCLK1EN      (*(unsigned int volatile *)REG_PCLK1EN)

#define KPI_BA     0xB8008000
#define KPI	   ((KPI_TypeDef *)KPI_BA)

#define rKPI_CONF      (*(unsigned int volatile *)(0xB8008000))


unsigned int u32KPI_INIT_FLAG = 0;

unsigned int u32Store_PCLK0EN;
unsigned int u32Store_PCLK1EN;
unsigned int u32Store_CLKDIV7;
unsigned int u32Store_GPIOx_DIR;
unsigned int u32Store_GPIOx_PDEN;
unsigned int u32Store_GPIOx_PUEN;
unsigned int u32Store_MFPx_L;
unsigned int u32Store_MFPx_H;

void nuc970_KPI_init(void)
{
    unsigned int u32Debounce = 0;
    unsigned int i = 0;

    // Unlock Register
    __raw_writel(0x59, 0xB00001FC);
    __raw_writel(0x16, 0xB00001FC);
    __raw_writel(0x88, 0xB00001FC);
    while(!(__raw_readl(0xB00001FC) & 0x1));

    u32Store_PCLK0EN = rPCLK0EN;
    u32Store_PCLK1EN = rPCLK1EN;
    u32Store_CLKDIV7 = rCLKDIV7;

    rCLKDIV7 = (rCLKDIV7 &~ CLKDIV7_KPI_S) | (0 << 15); // KPI Engine Clock Source; 0:XIN, 1:X32K
    rCLKDIV7 = (rCLKDIV7 &~ CLKDIV7_KPI_N) | (26 << 8);
	//rCLKDIV7 = (rCLKDIV7 &~ CLKDIV7_KPI_N) | (63 << 8);

    rPCLK0EN |= (1 << 3);  // enable GPIO clock
    rPCLK1EN |= (1 << 25); // enable KPI clock

    KPI->CONF |= (1 << 21); // enable KPI debounce

    //set debounce length
    KPI->CONF &=~ (0xf << 16);
    if(CONFIG_KPI_DEBOUNCE > 13)  u32Debounce = 13;
    else u32Debounce = CONFIG_KPI_DEBOUNCE;
	KPI->CONF |=  (u32Debounce << 16);

    KPI->CONF |= (1 << 4); // set Open Drain Enable

    KPI->CONF = (KPI->CONF &~ (0xff << 8)) | (0x80 << 8); // set PRESCALE

    #ifdef CONFIG_KPI_PA_PORT

    u32Store_GPIOx_DIR  = rGPIOA_DIR;
    u32Store_GPIOx_PDEN = rGPIOA_PDEN;
    u32Store_GPIOx_PUEN = rGPIOA_PUEN;
    u32Store_MFPx_L     = rMFP_GPA_L;
    u32Store_MFPx_H     = rMFP_GPA_H;

    for(i = 0; i < CONFIG_KPI_ROW_NUM; i++)
    {
        if(i >= 4) break;

        rGPIOA_DIR &= ~(0x1 << (i+4));
        rGPIOA_PDEN &= ~(0x1 << (i+4));
        rGPIOA_PUEN |= (0x1 << (i+4));
        rMFP_GPA_L = (rMFP_GPA_L &~ (0xf << (16+(i*4))) ) | (0x4 << ((16+(i*4))));

    }

    for(i = 0; i < CONFIG_KPI_COL_NUM; i++)
    {
         if(i >= 8) break;

        rGPIOA_DIR &= ~(0x1 << (i+8));
        rGPIOA_PDEN &= ~(0x1 << (i+8));
        rGPIOA_PUEN |= (0x1 << (i+8));
        rMFP_GPA_H = (rMFP_GPA_H &~ (0xf << (i*4)) ) | (0x4 << ((i*4)));
    }

    #elif CONFIG_KPI_PH_PORT

    u32Store_GPIOx_DIR  = rGPIOH_DIR;
    u32Store_GPIOx_PDEN = rGPIOH_PDEN;
    u32Store_GPIOx_PUEN = rGPIOH_PUEN;
    u32Store_MFPx_L     = rMFP_GPH_L;
    u32Store_MFPx_H     = rMFP_GPH_H;

    for(i = 0; i < CONFIG_KPI_ROW_NUM; i++)
    {
        if(i >= 4) break;

        rGPIOH_DIR &= ~(0x1 << (i+4));
        rGPIOH_PDEN &= ~(0x1 << (i+4));
        rGPIOH_PUEN |= (0x1 << (i+4));
        rMFP_GPH_L = (rMFP_GPH_L &~ (0xf << (16+(i*4))) ) | (0x4 << ((16+(i*4))));

    }

    for(i = 0; i < CONFIG_KPI_COL_NUM; i++)
    {
         if(i >= 8) break;

        rGPIOH_DIR &= ~(0x1 << (i+8));
        rGPIOH_PDEN &= ~(0x1 << (i+8));
        rGPIOH_PUEN |= (0x1 << (i+8));
        rMFP_GPH_H = (rMFP_GPH_H &~ (0xf << (i*4)) ) | (0x4 << ((i*4)));
    }

    #endif

    KPI->CONF |= (1 << 0); // Keypad Scan Enable
}

void nuc970_KPI_close(void)
{
    // Unlock Register
    __raw_writel(0x59, 0xB00001FC);
    __raw_writel(0x16, 0xB00001FC);
    __raw_writel(0x88, 0xB00001FC);
    while(!(__raw_readl(0xB00001FC) & 0x1));

    #ifdef CONFIG_KPI_PA_PORT

    rGPIOA_DIR = u32Store_GPIOx_DIR;
    rGPIOA_PDEN = u32Store_GPIOx_PDEN;
    rGPIOA_PUEN = u32Store_GPIOx_PUEN;
    rMFP_GPA_L = u32Store_MFPx_L;
    rMFP_GPA_H = u32Store_MFPx_H;

    #elif CONFIG_KPI_PH_PORT
    
    rGPIOH_DIR = u32Store_GPIOx_DIR;
    rGPIOH_PDEN = u32Store_GPIOx_PDEN;
    rGPIOH_PUEN = u32Store_GPIOx_PUEN;
    rMFP_GPH_L = u32Store_MFPx_L;
    rMFP_GPH_H = u32Store_MFPx_H;
    
    #endif

    KPI->CONF &= ~(1 << 0); // Keypad Scan Disable
    rPCLK0EN = u32Store_PCLK0EN;
    rPCLK1EN = u32Store_PCLK1EN;
    rCLKDIV7 = u32Store_CLKDIV7;
}

// return value 0: no key be press or release
// return value 1 ~ 32: press key
// return value 33 ~ 64: release key
unsigned int nuc970_KPI_Read(void)
{
    unsigned int i = 0;
    unsigned int u32Tmp = 0;
    unsigned int u32Tmp1 = 0;

    if(u32KPI_INIT_FLAG == 0)
    {
        nuc970_KPI_init();
        u32KPI_INIT_FLAG = 1;
    }

    if(KPI->KPE)
    {
        u32Tmp = 1;
        u32Tmp1 = KPI->KPE;
    }
    else if(KPI->KRE)
    {
        u32Tmp = 33;
        u32Tmp1 = KPI->KRE;
    }

    if(u32Tmp > 0)
    {
        for(i = 0; i < 32; i++)
        {
            if(u32Tmp1 & (0x1 << i))
            {
                u32Tmp = u32Tmp + i;
                if(u32Tmp > 32)
                {
                    KPI->KRE = (0x1 << i);  // clear release key
                }
                else
                {
                    KPI->KPE = (0x1 << i);  //clear press key
                }
                break;
            }
        }
    }

    return u32Tmp;

}





