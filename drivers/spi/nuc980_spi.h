/*
 * (C) Copyright 2018
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * SPI header for NUC980
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

#ifndef _NUC980_SPI_H_
#define _NUC980_SPI_H_

#if defined(CONFIG_USE_NUC980_QSPI0)
#define SPI_BASE 0xB0060000 //QSPI0
#elif defined(CONFIG_USE_NUC980_SPI0)
#define SPI_BASE 0xB0061000 //SPI0
#elif defined(CONFIG_USE_NUC980_SPI1)
#define SPI_BASE 0xB0062000 //SPI1
#endif

#define SPI_CTL        (SPI_BASE + 0x00)
#define SPI_CLKDIV     (SPI_BASE + 0x04)
#define SPI_SSCTL      (SPI_BASE + 0x08)
#define SPI_PDMACTL    (SPI_BASE + 0x0C)
#define SPI_FIFOCTL    (SPI_BASE + 0x10)
#define SPI_STATUS     (SPI_BASE + 0x14)
#define SPI_TX         (SPI_BASE + 0x20)
#define SPI_RX         (SPI_BASE + 0x30)

/* spi register bit */
#define ENINT           (0x01 << 17)
#define TXNEG           (0x01 << 2)
#define RXNEG           (0x01 << 1)
#define LSB             (0x01 << 13)
#define SELECTLEV       (0x01 << 2)
#define SELECTPOL       (0x01 << 3)
#define SELECTSLAVE0    0x01
#define SELECTSLAVE1    0x02
#define SPIEN           0x01
#define SPIENSTS        (0x01 << 15)
#define TXRXRST         (0x01 << 23)
#define SPI_BUSY        0x00000001
#define SPI_SS_ACT      0x00000001
#define SPI_SS_HIGH     0x00000004
#define SPI_QUAD_EN     0x400000
#define SPI_DIR_2QM     0x100000

#define PCLK_CLK         150000000        // From APB clock


struct nuc980_spi_slave {
	struct spi_slave slave;
	unsigned int max_hz;
	unsigned int mode;
};

static inline struct nuc980_spi_slave *to_nuc980_spi(struct spi_slave *slave) {
	return container_of(slave, struct nuc980_spi_slave, slave);
}

#endif //_NUC980_SPI_H_
