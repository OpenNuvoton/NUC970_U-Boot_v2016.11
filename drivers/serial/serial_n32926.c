/*
 * Copyright (c) 2014	Nuvoton Technology Corp.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Description:   NUC970 UART driver
 */
#include <common.h>
#include <asm/io.h>

#include <serial.h>

//#include "serial_nuc970.h"
#include <asm/arch/serial_n32926.h>


DECLARE_GLOBAL_DATA_PTR;

int n32926_serial_init (void);
void n32926_serial_putc (const char ch);
void n32926_serial_puts (const char *s);
int n32926_serial_getc (void);
int n32926_serial_tstc (void);
void n32926_serial_setbrg (void);

typedef struct
{
  	union
  	{
		volatile unsigned int RBR;		    	        /*!< Offset: 0x0000   Receive Buffer Register			                 */
		volatile unsigned int THR;			            /*!< Offset: 0x0000   Transmit Holding Register 		                 */
  	} x;
  	volatile unsigned int IER;				        /*!< Offset: 0x0004   Interrupt Enable Register 		                 */
  	volatile unsigned int FCR;				        /*!< Offset: 0x0008   FIFO Control Register 			                 */
  	volatile unsigned int LCR;				        /*!< Offset: 0x000C   Line Control Register				                 */
  	volatile unsigned int MCR;			        	/*!< Offset: 0x0010   Modem Control Register 			                 */
  	volatile unsigned int MSR;				        /*!< Offset: 0x0014   Modem Status Register 			                 */
  	volatile unsigned int FSR;				        /*!< Offset: 0x0018   FIFO Status Register 				                 */
  	volatile unsigned int ISR;				        /*!< Offset: 0x001C   Interrupt Status Register 		                 */
  	volatile unsigned int TOR;				        /*!< Offset: 0x0020   Time Out Register 				                 */
  	volatile unsigned int BAUD;				        /*!< Offset: 0x0024   Baud Rate Divisor Register		                 */
} UART_TypeDef;

#define GCR_BA    0xB0000000 /* Global Control */
#define REG_MFP_GPA_H	(GCR_BA+0x084)  /* GPIOA High Byte Multiple Function Control Register */
#define UART1_BA  0xB8008100 /* UART1 Control */
#define UART1	   ((UART_TypeDef *)UART1_BA) 

/*
 * Initialise the serial port with the given baudrate. The settings are always 8n1.
 */

u32 baudrate = CONFIG_BAUDRATE;
u32 ext_clk  = EXT_CLK;

int n32926_serial_init (void)
{
	__raw_writel((__raw_readl(REG_MFP_GPA_H) & 0xffff00ff) | 0x3300, REG_MFP_GPA_H); // UART1 multi-function

	/* UART1 line configuration for (115200,n,8,1) */
	UART1->LCR |=0x07;	
	UART1->BAUD = 0x30000066; /* 12MHz reference clock input, 115200 */
	
	return 0;
}

void n32926_serial_putc (const char ch)
{
	while ((UART1->FSR & 0x800000)); //waits for TX_FULL bit is clear
	UART1->x.THR = ch;
	if(ch == '\n')
	{
		while((UART1->FSR & 0x800000)); //waits for TX_FULL bit is clear
		UART1->x.THR = '\r';
	}
}

void n32926_serial_puts (const char *s)
{
	while (*s) {
		n32926_serial_putc (*s++);
	}
}


int n32926_serial_getc (void)
{
	while (1)
	{
		if (!(UART1->FSR & (1 << 14)))
		{
			return (UART1->x.RBR);
		}
	}
}

int n32926_serial_tstc (void)
{
	return (!((__raw_readl(UART1_BASE + REG_COM_MSR) & RX_FIFO_EMPTY)>>14));
}

void n32926_serial_setbrg (void)
{

        return;
}

static struct serial_device n32926_serial_drv = {
        .name   = "n32926_serial",
        .start  = n32926_serial_init,
        .stop   = NULL,
        .setbrg = n32926_serial_setbrg,
        .putc   = n32926_serial_putc,
        .puts   = n32926_serial_puts,
        .getc   = n32926_serial_getc,
        .tstc   = n32926_serial_tstc,
};

void n32926_serial_initialize(void)
{
        serial_register(&n32926_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
        return &n32926_serial_drv;
}

