/*
 * Copyright (c) 2018 Nuvoton Technology Corp.
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
 * Description:   NUC980 MAC driver source file
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <miiphy.h>
#include <malloc.h>
#include <linux/mii.h>
#include "nuc980_eth.h"


struct eth_descriptor volatile rx_desc[RX_DESCRIPTOR_NUM] __attribute__ ((aligned(32)));
struct eth_descriptor volatile tx_desc[TX_DESCRIPTOR_NUM] __attribute__ ((aligned(32)));


struct eth_descriptor volatile *tx_desc_ptr, *rx_desc_ptr;


int nuc980_eth_mii_write(uchar addr, uchar reg, ushort val)
{

	writel(val, MIID);
	writel((addr << 8) | reg | PHYBUSY | PHYWR | MDCCR, MIIDA);

	while (readl(MIIDA) & PHYBUSY);

	return(0);
}


int nuc980_eth_mii_read(uchar addr, uchar reg, ushort *val)
{
	writel((addr << 8) | reg | PHYBUSY | MDCCR, MIIDA);
	while (readl(MIIDA) & PHYBUSY);

	*val = (ushort)readl(MIID);

	return(0);
}

int nuc980_reset_phy(void)
{

	unsigned short reg;
	int delay;

	nuc980_eth_mii_write(CONFIG_NUC980_PHY_ADDR, MII_BMCR, BMCR_RESET);

	delay = 2000;
	while(delay-- > 0) {
		nuc980_eth_mii_read(CONFIG_NUC980_PHY_ADDR, MII_BMCR, &reg);
		if((reg & BMCR_RESET) == 0)
			break;

	}

	if(delay == 0) {
		printf("Reset phy failed\n");
		return(-1);
	}

	nuc980_eth_mii_write(CONFIG_NUC980_PHY_ADDR, MII_ADVERTISE, ADVERTISE_CSMA |
	                     ADVERTISE_10HALF |
	                     ADVERTISE_10FULL |
	                     ADVERTISE_100HALF |
	                     ADVERTISE_100FULL);

	nuc980_eth_mii_read(CONFIG_NUC980_PHY_ADDR, MII_BMCR, &reg);
	nuc980_eth_mii_write(CONFIG_NUC980_PHY_ADDR, MII_BMCR, reg | BMCR_ANRESTART);

	delay = 20000;
	while(delay-- > 0) {
		nuc980_eth_mii_read(CONFIG_NUC980_PHY_ADDR, MII_BMSR, &reg);
		if((reg & (BMSR_ANEGCOMPLETE | BMSR_LSTATUS)) == (BMSR_ANEGCOMPLETE | BMSR_LSTATUS))
			break;
	}

	if(delay == 0) {
		printf("AN failed. Set to 100 FULL\n");
		writel(readl(MCMDR) | MCMDR_OPMOD | MCMDR_FDUP, MCMDR);
		return(-1);
	} else {
		nuc980_eth_mii_read(CONFIG_NUC980_PHY_ADDR, MII_LPA, &reg);

		if(reg | ADVERTISE_100FULL) {
			writel(readl(MCMDR) | MCMDR_OPMOD | MCMDR_FDUP, MCMDR);
		} else if(reg | ADVERTISE_100HALF) {
			writel((readl(MCMDR) | MCMDR_OPMOD) & ~MCMDR_FDUP, MCMDR);
		} else if(reg | ADVERTISE_10FULL) {
			writel((readl(MCMDR) | MCMDR_FDUP) & ~MCMDR_OPMOD, MCMDR);
		} else {
			writel(readl(MCMDR) & ~(MCMDR_OPMOD | MCMDR_FDUP), MCMDR);
		}
	}
	return(0);
}


void init_tx_desc(void)
{
	int i;

	writel((unsigned int)(&tx_desc[0]), TXDLSA);
	tx_desc_ptr = &tx_desc[0];

	for(i = 0; i < TX_DESCRIPTOR_NUM; i++) {
		tx_desc[i].status1 = PaddingMode | CRCMode/* | MACTxIntEn*/;
		tx_desc[i].buf = NULL;
		tx_desc[i].status2 = 0;
		tx_desc[i].next = (struct eth_descriptor *)(&tx_desc[(i + 1) % TX_DESCRIPTOR_NUM]);
	}

	return;
}

void init_rx_desc(void)
{
	int i;

	writel((unsigned int)&rx_desc[0], RXDLSA);
	rx_desc_ptr = &rx_desc[0];
	for(i = 0; i < RX_DESCRIPTOR_NUM; i++) {
		rx_desc[i].status1 = RXfOwnership_DMA;
		rx_desc[i].buf = (unsigned char *)net_rx_packets[i];
		rx_desc[i].status2 = 0;
		rx_desc[i].next = (struct eth_descriptor *)(&rx_desc[(i + 1) % RX_DESCRIPTOR_NUM]);
	}

	return;
}

int nuc980_eth_write_hwaddr(struct eth_device *dev)
{

	writel((dev->enetaddr[0] << 24) |
	       (dev->enetaddr[1] << 16) |
	       (dev->enetaddr[2] << 8) |
	       dev->enetaddr[3] , CAM0M);

	writel((dev->enetaddr[4] << 24) |
	       (dev->enetaddr[5] << 16) , CAM0L);

	writel(CAM_ECMP | CAM_ABP, CAMCMR);
	writel(1, CAMEN);
	return(0);
}


int nuc980_eth_init(struct eth_device *dev, bd_t *bis)
{
	// Reset MAC
	writel(MCMDR_SWR, MCMDR);
	while(readl(MCMDR) &MCMDR_SWR);

	init_tx_desc();
	init_rx_desc();

	nuc980_eth_write_hwaddr(dev);  // need to reconfigure hardware address 'cos we just RESET emc...

	writel(MCMDR_SPCRC | MCMDR_RXON | MCMDR_TXON, MCMDR);
	writel(0, RSDR);

	return(nuc980_reset_phy());

}



void  nuc980_eth_halt(struct eth_device *dev)
{
	writel(readl(MCMDR) & ~(MCMDR_RXON|MCMDR_TXON), MCMDR);
}

int nuc980_eth_recv (struct eth_device *dev)
{

	while(1) {
		if(rx_desc_ptr->status1 & RXfOwnership_DMA) {
			break;
		}

		if(rx_desc_ptr->status1 & RXFD_RXGD) {
			net_process_received_packet(rx_desc_ptr->buf, rx_desc_ptr->status1 & 0xFFFF);;
		}
		rx_desc_ptr->status1 = RXfOwnership_DMA;
		rx_desc_ptr = rx_desc_ptr->next;
	}

	writel(0, RSDR);

	return 0;

}

int nuc980_eth_send(struct eth_device *dev, void *packet, int length)
{
	tx_desc_ptr->buf = (unsigned char *)packet;
	tx_desc_ptr->status2 = (unsigned int)length;
	tx_desc_ptr->status1 |= TXfOwnership_DMA;

	writel(0, TSDR);

	// wait 'til transfer complete. (e.g. ownership again set to CPU)
	while(tx_desc_ptr->status1 & TXfOwnership_DMA);

	tx_desc_ptr = tx_desc_ptr->next;

	return 0 ;
}




int nuc980_eth_register(void)
{
	struct eth_device *dev;

	dev = malloc(sizeof(*dev));
	if (dev == NULL)
		return(-1);

	memset(dev, 0, sizeof(*dev));
	sprintf(dev->name, "emac");

	dev->init = nuc980_eth_init;
	dev->halt = nuc980_eth_halt;
	dev->send = nuc980_eth_send;
	dev->recv = nuc980_eth_recv;
	dev->write_hwaddr = nuc980_eth_write_hwaddr;

	eth_register(dev);

	return(0);
}
