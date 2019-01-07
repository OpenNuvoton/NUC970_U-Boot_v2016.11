/*
 * (C) Copyright 2014
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * SPI header for NUC970 
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _N32926_SPI_H_
#define _N32926_SPI_H_

#define USE_N32926_SPI0 
//#define USE_N32926_SPI1

#ifdef USE_N32926_SPI0
#define SPI_BASE 0xB800C000 //SPI0
#else
#define SPI_BASE 0xB800C400 //SPI1
#endif

#define SPICTL         (SPI_BASE + 0x00)
#define SPIDIV         (SPI_BASE + 0x04)
#define SPISSR         (SPI_BASE + 0x08)
#define SPIRX0         (SPI_BASE + 0x20)
#define SPITX0         (SPI_BASE + 0x20)
#define SPIRX1         (SPI_BASE + 0x24)
#define SPITX1         (SPI_BASE + 0x24)
#define SPIRX2         (SPI_BASE + 0x28)
#define SPITX2         (SPI_BASE + 0x28)
#define SPIRX3         (SPI_BASE + 0x2C)
#define SPITX3         (SPI_BASE + 0x2C)

#define SPI_BUSY        0x00000001
#define SPI_CLKPOL      0x00000800
#define SPI_CLKPHAMSK   0x00000006
#define SPI_CLKPHAINV   0x00000002
#define SPI_CLKPHA      0x00000004
#define SPI_8BIT        0x00000040
#define SPI_24BIT       0x000000C0
#define SPI_32BIT       0x00000000
#define SPI_BIT_MASK    0x000000F8
#define SPI_TXN1        0x00000000
#define SPI_TXN4        0x00600000
#define SPI_TXN_MASK    0x00E00000
#define SPI_DUAL_EN	0x00000100
#define SPI_QUAD_EN	0x00000200
#define SPI_DIR_2QM	0x01000000

#define SPI_SS_ACT      0x00000001
#define SPI_SS_HIGH     0x00000004

#define SPI_CLK         75000000        // From APB clock

#define SPI_DUAL_MODE '2'
#define SPI_QUAD_MODE '4'

struct n32926_spi_slave {
        struct spi_slave slave;
        unsigned int max_hz;
        unsigned int mode;
};

static inline struct n32926_spi_slave *to_n32926_spi(struct spi_slave *slave)
{
        return container_of(slave, struct n32926_spi_slave, slave);
}

#endif //_NUC970_SPI_H_
