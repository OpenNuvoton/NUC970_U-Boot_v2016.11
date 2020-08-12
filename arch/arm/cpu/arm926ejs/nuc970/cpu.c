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

#define REG_PWRON         0xB0000004

int print_cpuinfo (void)
{
        unsigned int id;

        id = (readl(REG_PWRON) & 0x0F000000) >> 24;

        printf("CPU: ");

        switch(id) {
		case 0:
                        printf("N9H30F\n");
                        break;
                case 1:
                        printf("NUC971\n");
                        break;
                case 2:
                        printf("NUC972\n");
                        break;
                case 3:
                        printf("NUC973\n");
                        break;
                case 8:
                        printf("NUC978\n");
                        break;
                case 9:
                        printf("N9H30K\n");
                        break;
                case 0xd:
                        printf("NUC975\n");
                        break;
                case 0xe:
                        printf("NUC976\n");
                        break;
                case 0xf:
                        printf("NUC977\n");
                        break;
                default:
                        printf("Unknown\n");
                        break;

        }

        return(0);
}

int arch_cpu_init(void)
{
        // do nothing...

        return 0;
}

