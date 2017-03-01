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

#ifndef _NUC970_SPI_H_
#define _NUC970_SPI_H_

#define USE_NUC970_SPI0 
//#define USE_NUC970_SPI1

#ifdef USE_NUC970_SPI0
#define SPI_BASE 0xB8006200 //SPI0
#else
#define SPI_BASE 0xB8006300 //SPI1
#endif

#define SPICTL         (SPI_BASE + 0x00)
#define SPIDIV         (SPI_BASE + 0x04)
#define SPISSR         (SPI_BASE + 0x08)
#define SPIRX0         (SPI_BASE + 0x10)
#define SPITX0         (SPI_BASE + 0x10)
#define SPIRX1         (SPI_BASE + 0x14)
#define SPITX1         (SPI_BASE + 0x14)
#define SPIRX2         (SPI_BASE + 0x18)
#define SPITX2         (SPI_BASE + 0x18)
#define SPIRX3         (SPI_BASE + 0x1C)
#define SPITX3         (SPI_BASE + 0x1C)

#define SPI_BUSY        0x00000001
#define SPI_CLKPOL      0x80000000
#define SPI_CLKPHAMSK   0x00000006
#define SPI_CLKPHAINV   0x00000002
#define SPI_CLKPHA      0x00000004
#define SPI_8BIT        0x00000040
#define SPI_24BIT       0x000000C0
#define SPI_SS_ACT      0x00000001
#define SPI_SS_HIGH     0x00000004
#define SPI_DUAL_EN		0x400000
#define SPI_QUAD_EN		0x200000
#define SPI_DIR_2QM		0x100000

#define SPI_CLK         75000000        // From APB clock

#define SPI_DUAL_MODE '2'
#define SPI_QUAD_MODE '4'

struct nuc970_spi_slave {
        struct spi_slave slave;
        unsigned int max_hz;
        unsigned int mode;
};

static inline struct nuc970_spi_slave *to_nuc970_spi(struct spi_slave *slave)
{
        return container_of(slave, struct nuc970_spi_slave, slave);
}

#endif //_NUC970_SPI_H_
