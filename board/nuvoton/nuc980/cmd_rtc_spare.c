
/*
 * RTC, spare register support: get rtc spare register data
 */
#include <common.h>
#include <command.h>

#include <asm/io.h>
#include "register.h"

//#define RTC_DEBUG 1

#define RTC_INTSTS_REGWRBUSY_Msk 0x80000000

u32 volatile Wait;

void RTC_Check(void)
{
	unsigned int i = 0;

	Wait = readl(REG_RTC_INTSTS) & RTC_INTSTS_REGWRBUSY_Msk;
    
    while(Wait == RTC_INTSTS_REGWRBUSY_Msk)
    {

		Wait = readl(REG_RTC_INTSTS) & RTC_INTSTS_REGWRBUSY_Msk;
		
    	i++;
		
    	if(i > 0x10000)
    	{
    		printf("Wait Time out\n");
    		break;
    	}
    }  
}

void RTC_AER(void)
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

	RTC_Check();
}

static int do_rtcspare(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u32 i;
	u32 u32PCLK;
	u32 u32TimeOut = 0x10000;
	u32 *buffer;
	const char *str_cmd;

	u32 u32data[16];

	u32PCLK = readl(REG_PCLKEN0);

	if (argc != 3)
	{
show_usage:
        printf("\n fail \n");
		writel(u32PCLK, REG_PCLKEN0);
		return CMD_RET_USAGE;
	}

	/* enable RTC clock */
	writel((readl(REG_PCLKEN0) | 0x4), REG_PCLKEN0);

	str_cmd = argv[1];

    buffer = simple_strtoul(argv[2], NULL, 16);

	#ifdef RTC_DEBUG
	printf("\n buffer address = 0x%x ", buffer);
	printf("\n str_cmd = 0x%x ", *str_cmd);
	#endif

	/* init RTC */
	writel(0xa5eb1357, REG_RTC_INIR);
	while(1)
	{
		if(readl(REG_RTC_INIR) & 0x1)
			break;

		u32TimeOut--;
		if(!u32TimeOut)
			goto show_usage;
	}

	switch (*str_cmd) {
		case 'g':   
			for(i = 0; i < 16; i++)
	    	{
                RTC_AER();
				*(buffer+(i*4)) = readl(REG_RTC_SPARE + (i*4));
	    	}

            #ifdef RTC_DEBUG
			for(i=0; i<16; i++)
			{
				printf("\n SPARE %d = 0x%x ", i, *(buffer+(i*4)));
			}
			printf("\n");
            #endif
		break;

		case 's':     
			for(i = 0; i < 16; i++)
	    	{
				//*(buffer+(i*4)) = i + 0x10;
				RTC_AER();
				writel(*(buffer+(i*4)), (REG_RTC_SPARE + (i*4)));
                RTC_Check();
	    	}

            #ifdef RTC_DEBUG
			for(i = 0; i < 16; i++)
			{
                RTC_AER();
				printf("\n write buffer %d = 0x%x ", i, readl(REG_RTC_SPARE + (i*4)));
			}
			printf("\n");
            #endif
		break;

		default: 
			goto show_usage;
	}

	writel(u32PCLK, REG_PCLKEN0);

	return 0;
}



U_BOOT_CMD(
	rtcspare,	3,	0,	do_rtcspare,
	"get/set rtc spare data",
	"<get|set> <buffer address>\n"
	"get spare register data to buffer or "
	"write buffer data to spare register"
);

