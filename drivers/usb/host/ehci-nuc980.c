/*
 *  Copyright (c) 2014 Nuvoton Technology Corp.
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
#include <asm/io.h>

#include "ehci.h"

#define REG_HCLKEN      0xB0000210
#define USBH_CLK	(1 << 18)

#define EHCI_BASE	0xB0015000
#define REG_UCFGR	0xB0015060
#define REG_UPSCR0	0xB0015064
#define REG_UPSCR1	0xB0015068
#define REG_USBPCR0	0xB00150C4
#define REG_USBPCR1	0xB00150C8
#define REG_HcRhSts	0xB0017050
#define REG_OpModEn	0xB0017204

#define REG_GPE_MFP_H	0xB0000094
#define REG_GPF_MFP_H	0xB000009C
#define REG_GPH_MFP_L	0xB00000A8

/*
 * Create the appropriate control structures to manage
 * a new EHCI host controller.
 */
//int ehci_hcd_init(int index, struct ehci_hccr **hccr, struct ehci_hcor **hcor)
int ehci_hcd_init(int index, enum usb_init_type init, struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{

	writel(readl(REG_HCLKEN) | USBH_CLK, REG_HCLKEN);

	writel(0x160, REG_USBPCR0);
	writel(0x520, REG_USBPCR1);

	writel(0x8, REG_OpModEn);	// Over current active low
	writel(0x10000, REG_HcRhSts);	// PWR

	mdelay(100);			// wait 100ms 'til pwr stable

	writel(1, REG_UCFGR);
	writel(0x1000, REG_UPSCR0);
	writel(0x1000, REG_UPSCR1);

	//Set multi-function pin
	//Set PE.10 for USB_OVC, PE.12 for USBH_PWREN
	writel((readl(REG_GPE_MFP_H) & ~0x00000f00) | 0x100, REG_GPE_MFP_H);
	writel((readl(REG_GPE_MFP_H) & ~0x000f0000) | 0x10000, REG_GPE_MFP_H);

	*hccr = (struct ehci_hccr *)(EHCI_BASE);
	*hcor = (struct ehci_hcor *)((uint32_t) *hccr +
	                             HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));

	debug("nuc980-ehci: init hccr %x and hcor %x hc_length %d\n",
	      (uint32_t)*hccr, (uint32_t)*hcor,
	      (uint32_t)HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));

	return 0;
}

/*
 * Destroy the appropriate control structures corresponding
 * the the EHCI host controller.
 */
int ehci_hcd_stop(int index)
{
	return 0;
}

