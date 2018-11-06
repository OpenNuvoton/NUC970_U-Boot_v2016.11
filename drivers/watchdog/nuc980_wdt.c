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
#include <watchdog.h>
#include <asm/io.h>

#define REG_PCLKEN0     0xB0000218
#define REG_CLKDIVCTL8  0xB0000240
#define REG_WDT_CTL     0xB0040000

void hw_watchdog_reset(void);
void hw_watchdog_init(void);
extern void NUC980_UnLock(void);
extern void NUC980_Lock(void);

void hw_watchdog_reset(void)
{
	NUC980_UnLock();
	writel(readl(REG_WDT_CTL) | 1, REG_WDT_CTL);   // Reset WDT up-counter value
	NUC980_Lock();
}

void hw_watchdog_init(void)
{
	char *s;
	int enable;

	enable = 1;
	s = getenv("watchdog");
	if (s != NULL)
		if ((strncmp(s, "off", 3) == 0) || (strncmp(s, "0", 1) == 0))
			enable = 0;

	if (enable) {
		/* Enable 26 seconds watchdog timer */
		NUC980_UnLock();
		writel((readl(REG_WDT_CTL) & ~0xF00) | 0x882, REG_WDT_CTL);   // Set time-out interval and enable WDT & RSTEN
		NUC980_Lock();

		writel(readl(REG_PCLKEN0) | 1, REG_PCLKEN0);   // Enable WDT clock
		writel(readl((REG_CLKDIVCTL8) & ~0x300) | 0x200, REG_CLKDIVCTL8);   // Set WDT clock source from PCLK/4096
	} else {
		/* Disable watchdog timer */
		NUC980_UnLock();
		writel(readl(REG_WDT_CTL) & ~0x80, REG_WDT_CTL);   // Disable WDT
		NUC980_Lock();

		writel(readl(REG_PCLKEN0) & ~1, REG_PCLKEN0);   // Disable WDT clock
	}
}

