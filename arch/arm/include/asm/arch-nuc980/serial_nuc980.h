/*
 * Copyright (c) 2012 Nuvoton Technology Corp.
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
 * Description:   NUC900 UART register definition
 */
#ifndef _SERIAL_NUC900_H
#define _SERIAL_NUC900_H

#define WB_DATA_BITS_5    0x00
#define WB_DATA_BITS_6    0x01
#define WB_DATA_BITS_7    0x02
#define WB_DATA_BITS_8    0x03

#define WB_STOP_BITS_1    0x00
#define WB_STOP_BITS_2    0x04

#define WB_PARITY_NONE    0x00
#define WB_PARITY_ODD     0x00
#define WB_PARITY_EVEN    0x10

#define LEVEL_1_BYTE      0x00
#define LEVEL_4_BYTES     0x40
#define LEVEL_8_BYTES     0x80
#define LEVEL_14_BYTES    0xC0


#define TX_RX_FIFO_RESET    0x06
#define ENABLE_DLAB         0x80    // Enable Divider Latch Access
#define DISABLE_DLAB        0x7F
#define ENABLE_TIME_OUT     (0x80+0x20)
#define THR_EMPTY           0x20    // Transmit Holding Register Empty
#define RX_DATA_READY       0x01
#define RX_FIFO_EMPTY       0x4000




#define REG_COM_TX     (0x0)    /* (W) TX buffer */
#define REG_COM_RX     (0x0)    /* (R) RX buffer */
#define REG_COM_LSB    (0x0)    /* Divisor latch LSB */
#define REG_COM_MSB    (0x04)   /* Divisor latch MSB */
#define REG_COM_IER    (0x04)   /* Interrupt enable register */
#define REG_COM_IIR    (0x08)   /* (R) Interrupt ident. register */
#define REG_COM_FCR    (0x08)   /* (W) FIFO control register */
#define REG_COM_LCR    (0x0C)   /* Line control register */
#define REG_COM_MCR    (0x10)   /* Modem control register */
#define	REG_COM_LSR    (0x14)   /* (R) Line status register */
#define REG_COM_MSR    (0x18)   /* (R) Modem status register */
#define	REG_COM_TOR    (0x1C)   /* (R) Time out register */
#define REG_MFSEL      0xB000000C
#define UART0_BASE     0xB0070000
#define UART1_BASE     0xB0071000
#define UART2_BASE     0xB0072000
#define UART3_BASE     0xB0073000

#endif  // _SERIAL_NUC900_H
