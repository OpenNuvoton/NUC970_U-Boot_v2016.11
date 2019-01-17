/*
 * (C) Copyright 2014
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * SPI driver for NUC970
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

#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <command.h>

#include "nuc970_spi.h"

#define REG_PCLKEN1     0xB000021C
#define REG_MFP_GPB_L	0xB0000078
#define REG_MFP_GPB_H	0xB000007C
#define REG_MFP_GPG_L	0xB00000A0

static __inline u32 readl_ESwap(u32 addr)
{
	volatile u32 reg,val;
	reg = *(volatile u32 *) (addr);
	val = ((reg & 0xFF) << 24) | ((reg & 0xFF000000) >> 24) | ((reg & 0xFF00) << 8) | ((reg & 0xFF0000) >> 8);
	return val;
}

static __inline void writel_ESwap(u32 val, u32 addr)
{
	*(volatile u32 *) (addr) = (((val & 0xFF) << 24) | ((val & 0xFF000000) >> 24) | ((val & 0xFF00) << 8) | ((val & 0xFF0000) >> 8));
}

void spi_init()
{
	return;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
                                  unsigned int max_hz, unsigned int mode) {
	struct nuc970_spi_slave  *ns;

	ns = malloc(sizeof(struct nuc970_spi_slave));
	if (!ns)
		return NULL;
	memset(ns,0,sizeof(struct nuc970_spi_slave));

	ns->slave.bus = bus;
	ns->slave.cs = cs;
	ns->max_hz = max_hz;
	ns->mode = mode;
	ns->slave.quad_enable = 0;
#ifdef CONFIG_NUC970_SPI_Quad
	ns->slave.mode |= SPI_RX_QUAD;
#endif
	return &ns->slave;
}


void spi_free_slave(struct spi_slave *slave)
{
	struct nuc970_spi_slave *ns = to_nuc970_spi(slave);

	free(ns);

	return;
}

int spi_claim_bus(struct spi_slave *slave)
{
	//char *cp;
	struct nuc970_spi_slave *ns = to_nuc970_spi(slave);

#ifdef USE_NUC970_SPI0
	writel(readl(REG_PCLKEN1) | 0x10, REG_PCLKEN1);       // SPI0 clk
#else
	writel(readl(REG_PCLKEN1) | 0x20, REG_PCLKEN1);       // SPI1 clk
#endif

#ifdef USE_NUC970_SPI0
	//SPI0: B6, B7, B8, B9
	writel(readl(REG_MFP_GPB_L) | 0xBB000000, REG_MFP_GPB_L);
	writel(readl(REG_MFP_GPB_H) | 0x000000BB, REG_MFP_GPB_H);
#else
	//SPI1: B12, B13, B14, B15
	writel(readl(REG_MFP_GPB_H) | 0xBBBB0000, REG_MFP_GPB_H);
#endif

#ifdef CONFIG_NUC970_SPI_Quad
	//cp = getenv("spimode");
	if (1) { //(cp) {
		if (1) { //(*cp == SPI_QUAD_MODE ) {
#ifdef USE_NUC970_SPI0
			//SPI0: B10, B11 = D[2], D[3]
			writel(readl(REG_MFP_GPB_H) | 0x0000BB00,REG_MFP_GPB_H);
#else
			//SPI1: G4, G5 = D[2], D[3]
			writel(readl(REG_MFP_GPG_L) | 0x00BB0000,REG_MFP_GPG_L);
#endif
		}
	}
#endif /* CONFIG_NUC970_SPI_Quad */
	writel(SPI_8BIT, SPICTL);

	if(ns->mode & SPI_CS_HIGH)
		writel(SPI_SS_HIGH, SPISSR);
	else
		writel(0, SPISSR);
	if(ns->mode & SPI_CPOL)
		writel(readl(SPICTL) | SPI_CLKPOL, SPICTL);
	else
		writel(readl(SPICTL) & ~SPI_CLKPOL, SPICTL);

	if(ns->mode & SPI_CPHA)
		writel(readl(SPICTL) | SPI_CLKPHAINV, SPICTL);
	else
		writel(readl(SPICTL) | SPI_CLKPHA, SPICTL);

	spi_set_speed(slave, ns->max_hz);

	return(0);

}


void spi_release_bus(struct spi_slave *slave)
{
	//char *cp;

#ifdef USE_NUC970_SPI0
	writel(readl(REG_PCLKEN1) & ~0x10, REG_PCLKEN1);       // SPI0 clk
#else
	writel(readl(REG_PCLKEN1) & ~0x20, REG_PCLKEN1);       // SPI1 clk
#endif

#ifdef USE_NUC970_SPI0
	//SPI0: B6, B7, B8, B9
	writel(readl(REG_MFP_GPB_L) & ~0xBB000000, REG_MFP_GPB_L);
	writel(readl(REG_MFP_GPB_H) & ~0x000000BB, REG_MFP_GPB_H);
#else
	//SPI1: B12, B13, B14, B15
	writel(readl(REG_MFP_GPB_H) & ~0xBBBB0000, REG_MFP_GPB_H);
#endif

#ifdef CONFIG_NUC970_SPI_Quad
	//cp = getenv("spimode");
	if (1) { //(cp) {
		if (1) { //(*cp == SPI_QUAD_MODE ) {
#ifdef USE_NUC970_SPI0
			//SPI0: B10, B11 = D[2], D[3]
			writel(readl(REG_MFP_GPB_H) & ~0x0000BB00,REG_MFP_GPB_H);
#else
			//SPI1: G4, G5 = D[2], D[3]
			writel(readl(REG_MFP_GPG_L) & ~0x00BB0000, REG_MFP_GPG_L);
#endif
		}
	}
#endif /* CONFIG_NUC970_SPI_Quad */	
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
             const void *dout, void *din, unsigned long flags)
{
	unsigned int len;
	unsigned int i;
	unsigned char *tx = (unsigned char *)dout;
	unsigned char *rx = din;

	//printf("entr xfr\n");

	if(bitlen == 0)
		goto out;

	if(bitlen % 8) {
		/* Errors always terminate an ongoing transfer */
		flags |= SPI_XFER_END;
		goto out;
	}

	len = bitlen / 8;

	if(flags & SPI_XFER_BEGIN) {
		spi_cs_activate(slave);
	}

	// handle quad mode
	if (flags & SPI_6WIRE) {
		writel(readl(SPICTL) | SPI_QUAD_EN, SPICTL);
		if(rx)
			writel(readl(SPICTL) & ~SPI_DIR_2QM, SPICTL);
		else
			writel(readl(SPICTL) | SPI_DIR_2QM, SPICTL);
		//printf("QUAD=>(o)(0x%08x)\n", readl(SPICTL));
	} else {
		writel(readl(SPICTL) & ~SPI_QUAD_EN, SPICTL);
		//printf("QUAD=>(x)(0x%08x)\n", readl(SPICTL));
	}

	if (len > 65536) {
		unsigned char NonAlignLen;
		//process non-alignment case
		if ((unsigned int)tx % 4) {
			writel((readl(SPICTL) & ~0xF8) | 0x40, SPICTL); //set bit length to 8 bits
			NonAlignLen = 4 - ((unsigned int)tx % 4);
			for (i = 0; i < NonAlignLen; i++) {
				writel(*tx++, SPITX0);

				writel(readl(SPICTL) | SPI_BUSY, SPICTL);
				while (readl(SPICTL) & SPI_BUSY);
			}

			len -= NonAlignLen;
		}
		if ((unsigned int)rx % 4) {
			writel((readl(SPICTL) & ~0xF8) | 0x40, SPICTL); //set bit length to 8 bits
			NonAlignLen = 4 - ((unsigned int)rx % 4);
			for (i = 0; i < NonAlignLen; i++) {
				writel(readl(SPICTL) | SPI_BUSY, SPICTL);
				while (readl(SPICTL) & SPI_BUSY);

				*rx++ = (unsigned char)readl(SPIRX0);
			}

			len -= NonAlignLen;
		}

		writel(readl(SPICTL) & ~0xF8, SPICTL); //set bit length to 32 bits
		writel(readl(SPICTL) | 0x300, SPICTL); //set tx/rx number to 4 (SPI0/1/2/3)
		for (i = 0; (i+16) <= len; i+=16) {
			if(tx) {
				writel_ESwap(*(unsigned int*)tx, SPITX0);
				writel_ESwap(*(unsigned int*)(tx + 4), SPITX1);
				writel_ESwap(*(unsigned int*)(tx + 8), SPITX2);
				writel_ESwap(*(unsigned int*)(tx + 12), SPITX3);
				tx += 16;
			}

			writel(readl(SPICTL) | SPI_BUSY, SPICTL);
			while (readl(SPICTL) & SPI_BUSY);

			if(rx) {
				*(unsigned int*)rx = readl_ESwap(SPIRX0);
				*(unsigned int*)(rx + 4) = readl_ESwap(SPIRX1);
				*(unsigned int*)(rx + 8) = readl_ESwap(SPIRX2);
				*(unsigned int*)(rx + 12) = readl_ESwap(SPIRX3);
				rx += 16;
			}
		}
		//process rest bytes
		if (i < len) {
			writel((readl(SPICTL) & ~0xF8) | 0x40, SPICTL); //set bit length to 8 bits
			writel(readl(SPICTL) & ~0x300, SPICTL); //set tx/rx number to 1 (SPI0 only)
			for (; i < len; i++) {
				if(tx)
					writel(*tx++, SPITX0);

				writel(readl(SPICTL) | SPI_BUSY, SPICTL);
				while (readl(SPICTL) & SPI_BUSY);

				if(rx)
					*rx++ = (unsigned char)readl(SPIRX0);
			}
		}
	} else {
		for (i = 0; i < len; i++) {
			if(tx)
				writel(*tx++, SPITX0);

			writel(readl(SPICTL) | SPI_BUSY, SPICTL);
			while (readl(SPICTL) & SPI_BUSY);

			if(rx)
				*rx++ = (unsigned char)readl(SPIRX0);
		}
	}

out:
	if (flags & SPI_XFER_END) {
		/*
		 * Wait until the transfer is completely done before
		 * we deactivate CS.
		 */
		while (readl(SPICTL) & SPI_BUSY);

		spi_cs_deactivate(slave);
	}


	return 0;
}

int  spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return(1);
}

void spi_cs_activate(struct spi_slave *slave)
{
	writel(readl(SPISSR) | SPI_SS_ACT, SPISSR);
	return;
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	writel(readl(SPISSR) & ~SPI_SS_ACT, SPISSR);
	return;
}


void spi_set_speed(struct spi_slave *slave, uint hz)
{
	unsigned int div;

	div = SPI_CLK / (hz * 2);

	if((SPI_CLK % (hz * 2)) == 0)
		div--;

	if(div == 0)
		div = 1;  // div should at lease be 1

	if(div > 0xFFFF)
		div = 0xFFFF; // 16 bits only

	writel(div, SPIDIV);

	return;
}

