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

#define SYS_REGWPCTL    0xB00001FC
#define REG_CLKDIVCTL8  0xB0000240
#define REG_WDT_CTL     0xB8001800

void hw_watchdog_reset(void);
void hw_watchdog_init(void);

// UnLock register write protect
static void Unlock_RegWriteProtect(void)
{
    do {
        writel(0x59, SYS_REGWPCTL);
        writel(0x16, SYS_REGWPCTL);
        writel(0x88, SYS_REGWPCTL);
    //wait for write-protection disabled indicator raised 
    } while(!(readl(SYS_REGWPCTL) & 1));
}

// Lock register write protect
static void Lock_RegWriteProtect(void)
{
    writel(0x0, SYS_REGWPCTL);
}

void hw_watchdog_reset(void)
{
    Unlock_RegWriteProtect();
    writel(readl(REG_WDT_CTL) | 1, REG_WDT_CTL);   // Reset WDT up-counter value
    Lock_RegWriteProtect();
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
        /* Enable 14 seconds watchdog timer */
        Unlock_RegWriteProtect();
        writel(readl(REG_WDT_CTL) | 0x80, REG_WDT_CTL);   // Enable WDT
        Lock_RegWriteProtect();

        writel(readl((REG_CLKDIVCTL8) & ~0x300) | 0x200, REG_CLKDIVCTL8);   // Set WDT clock source from PCLK/4096
    }
    else {
        /* Disable watchdog timer */
        Unlock_RegWriteProtect();
        writel(readl(REG_WDT_CTL) & ~0x80, REG_WDT_CTL);   // Disable WDT
        Lock_RegWriteProtect();
    }
}

