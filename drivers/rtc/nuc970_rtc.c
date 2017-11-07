/*
 * Copyright (c) 2014 Nuvoton technology corporation.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <common.h>
#include <command.h>

#include <asm/io.h>

#include <rtc.h>


#if defined(CONFIG_CMD_DATE)

#define    RTC_BA    0xB8004000 /* Real Time Clock Control */
#define    CLK_BA    0xB0000200 /* Clock Control */

#define    REG_PCLKEN0		(CLK_BA+0x018)  /*  APB IP Clock Enable Control Register 0 */

#define		REG_RTC_INIR			(RTC_BA+0x0000)	/* Initial register */
#define		REG_RTC_AER			(RTC_BA+0x0004)	/* Access Enable Register*/
#define         REG_RTC_TIME                    (RTC_BA+0x0C)   /* Time Loading Register */
#define         REG_RTC_CAL                     (RTC_BA+0x10)   /* Calendar Loading Register */
#define         REG_RTC_TIMEFMT                 (RTC_BA+0x14)   /* Time Format Selection Register */
#define         REG_RTC_WEEKDAY                 (RTC_BA+0x18)   /* Day of the Week Register */
#define		REG_RTC_INTSTS			(RTC_BA+0x002C)	/* Interrupt Status */
#define		REG_RTC_SPARE			(RTC_BA+0x0040)	/* Spare register */

#define RTC_INTSTS_REGWRBUSY_Msk 0x80000000

void RTC_WaitBusy(void)
{
 unsigned int i = 0;

    while(readl(REG_RTC_INTSTS) & RTC_INTSTS_REGWRBUSY_Msk)
    {
    	i++;
		
    	if(i > 0x10000)
    	{
    		printf("Wait Time out\n");
    		break;
    	}
    }
}

void RTC_WriteEnable(void)
{
	u32 u32TimeOut = 0x10000;

	/* Check Access Enable */
	while(1)
	{
		writel(0xA965, REG_RTC_AER);

		if(readl(REG_RTC_AER) & 0x10000)
			break;

		u32TimeOut--;
		if(!u32TimeOut)
		{
			printf("RTC AER Time out\n");
			break;
		}
	}
	
	RTC_WaitBusy();
}

void rtc_reg_write(int value, int reg)
{
 RTC_WriteEnable();
	writel(value, reg);
	RTC_WaitBusy();
}

/*****************************************************************************
 * get time
 *****************************************************************************/
int rtc_get (struct rtc_time *tmp)
{
    uint32_t u32Time;
    uint32_t u32Cal;
    uint32_t u32Wday;

      while(!(readl(REG_RTC_INIR) & 0x1))
      {
      	unsigned int i = 0;
      /* enable RTC clock */
      __raw_writel((readl(REG_PCLKEN0) | 0x4), REG_PCLKEN0);
      /* init RTC */
      __raw_writel(0xa5eb1357, REG_RTC_INIR);

      i++;

    	if(i > 0x10000)
    	{
    		printf("RTC INIT Time-out \n");
    		return -1;
    	}
      }

      u32Cal  = readl(REG_RTC_CAL);
      u32Wday = readl(REG_RTC_WEEKDAY);
      u32Time = readl(REG_RTC_TIME);

       tmp->tm_year	= 2000+ (((u32Cal >> 20) & 0xf) * 10) + ((u32Cal >> 16) & 0xf);
       tmp->tm_mon	= (((u32Cal >> 12) & 0xf) * 10) + ((u32Cal >> 8) & 0xf);
       tmp->tm_mday	= (((u32Cal >> 4) & 0xf) * 10) + ((u32Cal >> 0) & 0xf);
       tmp->tm_wday	= u32Wday;
       tmp->tm_hour	= (((u32Time >> 20) & 0xf) * 10) + ((u32Time >> 16) & 0xf);
       tmp->tm_min	= (((u32Time >> 12) & 0xf) * 10) + ((u32Time >> 8) & 0xf);
       tmp->tm_sec	= (((u32Time >> 4) & 0xf) * 10) + ((u32Time >> 0) & 0xf);

       return 0;
}

/*****************************************************************************
 * set time
 *****************************************************************************/
int rtc_set (struct rtc_time *tmp)
{
    uint32_t u32Time;
    uint32_t u32Cal;
    uint32_t u32Wday;

//    printf("\n Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d \n",
//		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
//		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

      while(!(readl(REG_RTC_INIR) & 0x1))
      {
      	unsigned int i = 0;
      /* enable RTC clock */
      __raw_writel((readl(REG_PCLKEN0) | 0x4), REG_PCLKEN0);
      /* init RTC */
      __raw_writel(0xa5eb1357, REG_RTC_INIR);

      i++;

    	if(i > 0x10000)
    	{
    		printf("RTC INIT Time-out \n");
    		return -1;
    	}
      }

      u32Time = ((tmp->tm_sec%10) << 0 ) | ((tmp->tm_sec/10) << 4 ) |
                ((tmp->tm_min%10) << 8 ) | ((tmp->tm_min/10) << 12) |
                ((tmp->tm_hour%10)<< 16) | ((tmp->tm_hour/10)<< 20);

      rtc_reg_write(u32Time, REG_RTC_TIME);

      u32Cal  = ((tmp->tm_mday%10) << 0 ) | ((tmp->tm_mday/10) << 4 ) |
                ((tmp->tm_mon %10) << 8 ) | ((tmp->tm_mon /10) << 12) |
                ((tmp->tm_year%10)<< 16) | (((tmp->tm_year/10)%10)<< 20);

      rtc_reg_write(u32Cal, REG_RTC_CAL);

      u32Wday = (tmp->tm_wday & 0x7);

      rtc_reg_write(u32Wday, REG_RTC_WEEKDAY);

	return 0;
}

/*****************************************************************************
 * reset rtc
 *****************************************************************************/
void rtc_reset (void)
{
	return;	/* nothing to do */
}

#endif
