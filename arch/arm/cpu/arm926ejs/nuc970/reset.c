/*
 *  Copyright (c) 2012~2016 Nuvoton Technology Corp.
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

#define REG_AHBIPRESET       0xB0000060 
#define REG_REGWPCTL       0xB00001FC 
void reset_cpu(ulong ignored)
{
	//UnlockReg
	while(readl(REG_REGWPCTL) != 1) {	
		writel(0x59, REG_REGWPCTL);
		writel(0x16, REG_REGWPCTL);
		writel(0x88, REG_REGWPCTL);
	}

	writel(1, REG_AHBIPRESET); //system reset
	
}

