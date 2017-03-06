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
#include <div64.h>

#define REG_CLKEN       0xB0000218
#define REG_TSCR0       0xB8001000
#define REG_TICR0       0xB8001004
#define REG_TDR0        0xB8001008
#define TIMER_INIT_VAL  0xFFFFFF
#define TMR_DIV         120             /* timer prescaler */  
#define TIMER_CLK       (EXT_CLK / TMR_DIV)


DECLARE_GLOBAL_DATA_PTR;

static inline unsigned long long tick_to_time(unsigned long long tick)
{
        tick *= CONFIG_SYS_HZ;

        do_div(tick, gd->arch.timer_rate_hz);

        return tick;
}

static inline unsigned long long usec_to_tick(unsigned long long usec)
{   
        usec *= gd->arch.timer_rate_hz;
        do_div(usec, 1000000);

        return usec;
}


int timer_init(void)
{
        writel(readl(REG_CLKEN) | 0x100, REG_CLKEN); //enable timer engine clock
        writel(TIMER_INIT_VAL, REG_TICR0);   // set timer init counter value
        writel(0x68000000 |(TMR_DIV - 1), REG_TSCR0);    // start timer counting in periodic mode, prescale = (255 + 1)

        gd->arch.timer_rate_hz = TIMER_CLK;
        gd->arch.tbu = gd->arch.tbl = 0;

        return 0;
}

/*
 * Get the current 64 bit timer tick count
 */
unsigned long long get_ticks(void)
{
        ulong now;
        ulong i = gd->arch.tbl & 0xFFFFFF;
	ulong init = 0;
	ulong tdr;

	tdr = readl(REG_TDR0);
        now = tdr - init;

        /* increment tbu if tbl has rolled over */
        if ((now < i) && ((i- now) > 0xFF0000)) {  // XXX: TDR can float , 'cos timer is not using APB
                if(gd->arch.tbl < 0xFF000000) {
                        //printf("-%d %d\n", now, gd->tbl);                        
                        gd->arch.tbl += 0x1000000;
                } else {
                        gd->arch.tbu++;
                        gd->arch.tbl = 0;
                }                                
        }
        gd->arch.tbl = (gd->arch.tbl & 0xFF000000 ) + now;

        return (((unsigned long long)gd->arch.tbu) << 32) | gd->arch.tbl;
}

void __udelay(unsigned long usec)
{
        unsigned long long start;
        ulong tmo;

        start = get_ticks();            /* get current timestamp */
        tmo = usec_to_tick(usec);       /* convert usecs to ticks */
        while ((get_ticks() - start) < tmo)
                ;                       /* loop till time has passed */
}

/*
 *  Return a value using CONFIG_SYS_HZ as unit
 */
ulong get_timer(ulong base)
{
#if 1
        ulong temp = tick_to_time(get_ticks());


        return(temp - base);
        
#else        
        return tick_to_time(get_ticks()) - base;
#endif
        
}

/*
 * Return the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
        return gd->arch.timer_rate_hz;
}
