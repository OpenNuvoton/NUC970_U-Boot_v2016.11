/*
 * (C) Copyright 2018
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * SPI driver for NUC980
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

#include "nuc980_spi.h"

#define REG_HCLKEN      0xB0000210
#define REG_PCLKEN1     0xB000021C
#define REG_MFP_GPB_H	0xB000007C
#define REG_MFP_GPD_L	0xB0000088
#define REG_MFP_GPD_H	0xB000008C
#define PDMA_BA		0xB0008000
#define REG_PDMA_DSCT0_CTL      (PDMA_BA+0x000)
#define REG_PDMA_DSCT0_SA       (PDMA_BA+0x004)
#define REG_PDMA_DSCT0_DA       (PDMA_BA+0x008)
#define REG_PDMA_CHCTL          (PDMA_BA+0x400)
#define REG_PDMA_TDSTS          (PDMA_BA+0x424)
#define REG_PDMA_REQSEL0_3      (PDMA_BA+0x480)
#define TXCNT_Pos		(16)
#define TXWIDTH_32		(0x2000)
#define SRC_FIXED		(0x300)
#define TBINTDIS		(0x80)
#define TX_SINGLE		(0x4)
#define MODE_BASIC		(0x1)

void spi_init()
{
	return;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
                                  unsigned int max_hz, unsigned int mode) {
	struct nuc980_spi_slave  *ns;

	ns = malloc(sizeof(struct nuc980_spi_slave));
	if (!ns)
		return NULL;
	memset(ns,0,sizeof(struct nuc980_spi_slave));

	ns->slave.bus = bus;
	ns->slave.cs = cs;
	ns->max_hz = max_hz;
	ns->mode = mode;
	ns->slave.quad_enable = 0;
#ifdef CONFIG_NUC980_SPI_Quad
	ns->slave.mode |= SPI_RX_QUAD;
#endif


	return &ns->slave;
}


void spi_free_slave(struct spi_slave *slave)
{
	struct nuc980_spi_slave *ns = to_nuc980_spi(slave);

	free(ns);

	return;
}

int spi_claim_bus(struct spi_slave *slave)
{
	struct nuc980_spi_slave *ns = to_nuc980_spi(slave);

#if defined(CONFIG_USE_NUC980_QSPI0)
	writel(readl(REG_PCLKEN1) | 0x10, REG_PCLKEN1);       // QSPI0 clk
#elif defined(CONFIG_USE_NUC980_SPI0)
	writel(readl(REG_PCLKEN1) | 0x20, REG_PCLKEN1);       // SPI0 clk
#elif defined(CONFIG_USE_NUC980_SPI1)
	writel(readl(REG_PCLKEN1) | 0x40, REG_PCLKEN1);       // SPI1 clk
#endif

#if defined(CONFIG_USE_NUC980_QSPI0)
	//QSPI0: D2, D3, D4, D5
	writel(readl(REG_MFP_GPD_L) | 0x00111100, REG_MFP_GPD_L);
#elif defined(CONFIG_USE_NUC980_SPI0)
	//SPI0: D8, D9, D10, D11
	writel(readl(REG_MFP_GPD_H) | 0x00001111, REG_MFP_GPD_H);
#elif defined(CONFIG_USE_NUC980_SPI1)
	//SPI1: B9, B10, B11, B12
	writel(readl(REG_MFP_GPB_H) | 0x00001111, REG_MFP_GPB_H);
#endif

	writel((readl(SPI_CTL) & ~0x1F00) | 0x800, SPI_CTL); //Data Width = 8 bit

	if(ns->mode & SPI_CS_HIGH)
		writel(SPI_SS_HIGH, SPI_SSCTL);
	else
		writel(0, SPI_SSCTL);

	if(ns->mode & SPI_CPOL)
		writel(readl(SPI_CTL) | SELECTPOL, SPI_CTL);
	else
		writel(readl(SPI_CTL) & ~SELECTPOL, SPI_CTL);

	if(ns->mode & SPI_CPHA)
		writel(readl(SPI_CTL) | RXNEG, SPI_CTL);
	else
		writel(readl(SPI_CTL) | TXNEG, SPI_CTL);

	spi_set_speed(slave, ns->max_hz);

	writel(readl(SPI_FIFOCTL) | 0x3, SPI_FIFOCTL); //TX/RX reset
	while ((readl(SPI_STATUS) & TXRXRST));

	writel((readl(SPI_CTL) & ~0xFF)|5, SPI_CTL);

	writel(readl(SPI_CTL) | SPIEN, SPI_CTL);
	while ((readl(SPI_STATUS) & SPIENSTS) == 0);

	return(0);

}


void spi_release_bus(struct spi_slave *slave)
{

#if defined(CONFIG_USE_NUC980_QSPI0)
	writel(readl(REG_PCLKEN1) & ~0x10, REG_PCLKEN1);       // QSPI0 clk
#elif defined(CONFIG_USE_NUC980_SPI0)
	writel(readl(REG_PCLKEN1) & ~0x20, REG_PCLKEN1);       // SPI0 clk
#elif defined(CONFIG_USE_NUC980_SPI1)
	writel(readl(REG_PCLKEN1) & ~0x40, REG_PCLKEN1);       // SPI1 clk
#endif

#if defined(CONFIG_USE_NUC980_QSPI0)
	//QSPI0: D2, D3, D4, D5
	writel(readl(REG_MFP_GPD_L) & ~0x00111100, REG_MFP_GPD_L);
#elif defined(CONFIG_USE_NUC980_SPI0)
	//SPI0: D8, D9, D10, D11
	writel(readl(REG_MFP_GPD_H) & ~0x00001111, REG_MFP_GPD_H);
#elif defined(CONFIG_USE_NUC980_SPI1)
	//SPI1: B9, B10, B11, B12
	writel(readl(REG_MFP_GPB_H) & ~0x00001111, REG_MFP_GPB_H);
#endif

#if defined(CONFIG_USE_NUC980_QSPI0)
	//SPI0: D6, D7 = D[2], D[3]
	writel(readl(REG_MFP_GPD_L) & ~0x11000000, REG_MFP_GPD_L);
#endif

}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
             const void *dout, void *din, unsigned long flags)
{
	unsigned int len;
	unsigned int i;
	unsigned char *tx = (unsigned char *)dout;
	unsigned char *rx = din;

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
		//SPI0: D6, D7 = D[2], D[3]
		writel(readl(REG_MFP_GPD_L) | 0x11000000, REG_MFP_GPD_L);

		writel(readl(SPI_CTL) | SPI_QUAD_EN, SPI_CTL);
		if(rx)
			writel(readl(SPI_CTL) & ~SPI_DIR_2QM, SPI_CTL);
		else
			writel(readl(SPI_CTL) | SPI_DIR_2QM, SPI_CTL);
		//printf("QUAD=>(o)(0x%08x)\n", readl(SPI_CTL));
	} else {
		writel(readl(SPI_CTL) & ~SPI_QUAD_EN, SPI_CTL);
		//printf("QUAD=>(x)(0x%08x)\n", readl(SPI_CTL));
	}

	writel(readl(SPI_FIFOCTL) | 0x3, SPI_FIFOCTL); //TX/RX reset
	while ((readl(SPI_STATUS) & TXRXRST));

	if (rx && (len > 65536)) {
		//process non-alignment case
		if(rx) {
			u32 loop,count;

			writel((readl(SPI_CTL) & ~0x1), SPI_CTL); //Disable SPIEN
			while ((readl(SPI_STATUS) & 0x8000)); //SPIENSTS

			writel((readl(SPI_CTL) & ~0x1F00), SPI_CTL); //Data Width = 32 bit
			writel((readl(SPI_CTL) & ~0xF0), SPI_CTL); //Suspend interval = 0
			writel((readl(SPI_CTL) | 0x80000), SPI_CTL); //Byte reorder

			writel(readl(SPI_FIFOCTL) | 0x3, SPI_FIFOCTL); //TX/RX reset
			while ((readl(SPI_STATUS) & TXRXRST));

			writel((readl(SPI_CTL) | 0x1), SPI_CTL); //Enable SPIEN
			while (!(readl(SPI_STATUS) & 0x8000)); //SPIENSTS

			writel((readl(REG_HCLKEN) | 0x1000), REG_HCLKEN); /* enable PDMA0 clock */
			/* initial PDMA channel 0 */
			writel(0x1, REG_PDMA_CHCTL);
			writel(21, REG_PDMA_REQSEL0_3);  /* QSPI0 RX */

			/* set PDMA */
			writel(SPI_RX, REG_PDMA_DSCT0_SA);

			len = (len + 3) >> 2;
			loop = len / 0x10000;
			for (i=0; i<loop; i++) {
				writel((unsigned int)rx, REG_PDMA_DSCT0_DA);
				writel((0xFFFF << TXCNT_Pos)|TXWIDTH_32|SRC_FIXED|TBINTDIS|TX_SINGLE|MODE_BASIC, REG_PDMA_DSCT0_CTL);
				writel(readl(SPI_PDMACTL)|0x2, SPI_PDMACTL);
				while(1) {
					if (readl(REG_PDMA_TDSTS) & 0x1) {
						writel(0x1, REG_PDMA_TDSTS);
						break;
					}
				}
				rx += 0x40000;
			}
			count = len % 0x10000;
			{
				writel((unsigned int)rx, REG_PDMA_DSCT0_DA);
				writel((count << TXCNT_Pos)|TXWIDTH_32|SRC_FIXED|TBINTDIS|TX_SINGLE|MODE_BASIC, REG_PDMA_DSCT0_CTL);
				writel(readl(SPI_PDMACTL)|0x2, SPI_PDMACTL);
				while(1) {
					if (readl(REG_PDMA_TDSTS) & 0x1) {
						writel(0x1, REG_PDMA_TDSTS);
						break;
					}
				}
			}
			writel(readl(SPI_CTL) & ~(0x80000), SPI_CTL); //Disable Byte reorder
			writel((readl(SPI_CTL) & ~0x1F00)| (0x800), SPI_CTL); //Data length 8 bit
			writel(0, SPI_PDMACTL);     // disable RX
			writel((readl(REG_HCLKEN) & ~0x1000), REG_HCLKEN); /* disable PDMA0 clock */
		}
	} else if (rx && (len >= 2048)) {
		u32 count;
		writel((readl(SPI_CTL) & ~0x1), SPI_CTL); //Disable SPIEN
		while ((readl(SPI_STATUS) & 0x8000)); //SPIENSTS

		writel((readl(SPI_CTL) & ~0x1F00), SPI_CTL); //Data Width = 32 bit
		writel((readl(SPI_CTL) & ~0xF0), SPI_CTL); //Suspend interval = 0
		writel((readl(SPI_CTL) | 0x80000), SPI_CTL); //Byte reorder

		writel(readl(SPI_FIFOCTL) | 0x3, SPI_FIFOCTL); //TX/RX reset
		while ((readl(SPI_STATUS) & TXRXRST));

		writel((readl(SPI_CTL) | 0x1), SPI_CTL); //Enable SPIEN
		while (!(readl(SPI_STATUS) & 0x8000)); //SPIENSTS

		writel((readl(REG_HCLKEN) | 0x1000), REG_HCLKEN); /* enable PDMA0 clock */
		/* initial PDMA channel 0 */
		writel(0x1, REG_PDMA_CHCTL);
		writel(21, REG_PDMA_REQSEL0_3);  /* QSPI0 RX */

		count = (len + 3) >> 2;
		/* set PDMA */
		writel(SPI_RX, REG_PDMA_DSCT0_SA);
		writel((unsigned int)rx, REG_PDMA_DSCT0_DA);
		writel(((count-1)<<TXCNT_Pos)|TXWIDTH_32|SRC_FIXED|TBINTDIS|TX_SINGLE|MODE_BASIC, REG_PDMA_DSCT0_CTL);
		writel(readl(SPI_PDMACTL)|0x2, SPI_PDMACTL);
		while(1) {
			if (readl(REG_PDMA_TDSTS) & 0x1) {
				writel(0x1, REG_PDMA_TDSTS);
				break;
			}
		}
		writel(readl(SPI_CTL) & ~(0x80000), SPI_CTL); //Disable Byte reorder
		writel((readl(SPI_CTL) & ~0x1F00)| (0x800), SPI_CTL); //Data length 8 bit
		writel(0, SPI_PDMACTL);     // disable RX
		writel((readl(REG_HCLKEN) & ~0x1000), REG_HCLKEN); /* disable PDMA0 clock */
	} else {
		for (i = 0; i < len; i++) {
			if(tx) {
				while ((readl(SPI_STATUS) & 0x20000)); //TXFULL
				writel(*tx++, SPI_TX);
			}

			if(rx) {
				while ((readl(SPI_STATUS) & 0x20000)); //TXFULL
				writel(0, SPI_TX);
				while ((readl(SPI_STATUS) & 0x100)); //RXEMPTY
				*rx++ = (unsigned char)readl(SPI_RX);
			}
		}

		while (readl(SPI_STATUS) & SPI_BUSY);
	}

out:
	if (flags & SPI_XFER_END) {
		/*
		 * Wait until the transfer is completely done before
		 * we deactivate CS.
		 */
		while (readl(SPI_STATUS) & SPI_BUSY);

		spi_cs_deactivate(slave);
	}

	if (flags & SPI_6WIRE) {
		writel(readl(SPI_CTL) & ~SPI_QUAD_EN, SPI_CTL); //Disable Quad mode
		writel(readl(REG_MFP_GPD_L) & ~(0x11000000), REG_MFP_GPD_L);
	}

	return 0;
}

int  spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return(1);
}

void spi_cs_activate(struct spi_slave *slave)
{
	writel(readl(SPI_SSCTL) | SELECTSLAVE0, SPI_SSCTL);
	return;
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	writel(readl(SPI_SSCTL) & ~SELECTSLAVE0, SPI_SSCTL);
	return;
}


void spi_set_speed(struct spi_slave *slave, uint hz)
{
	unsigned int div;

	div = PCLK_CLK / hz;

	if (div)
		div--;

	if(div == 0)
		div = 1;

	if(div > 0x1FF)
		div = 0x1FF;

	writel(div, SPI_CLKDIV);

	return;
}

