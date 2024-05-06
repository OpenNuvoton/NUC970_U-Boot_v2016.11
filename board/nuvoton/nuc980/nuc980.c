/*
 * (C) Copyright 2016 Nuvoton, Inc.
 *
 */

#include <common.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <watchdog.h>
#include <pwm.h>

#define REG_HCLKEN      0xB0000210
#define REG_PCLKEN0     0xB0000218
#define REG_PCLKEN1     0xB000021C
#define REG_PWRON       0xB0000004
#define REG_SDIC_SIZE0  0xB0002010
#define REG_SDIC_SIZE1  0xB0002014
#define REG_CLKDIVCTL8  0xB0000240
#define REG_MFP_GPA_L	0xB0000070
#define REG_MFP_GPA_H	0xB0000074
#define REG_MFP_GPB_L	0xB0000078
#define REG_MFP_GPB_H	0xB000007C
#define REG_MFP_GPC_L	0xB0000080
#define REG_MFP_GPC_H	0xB0000084
#define REG_MFP_GPD_L	0xB0000088
#define REG_MFP_GPD_H	0xB000008C
#define REG_MFP_GPE_L	0xB0000090
#define REG_MFP_GPE_H	0xB0000094
#define REG_MFP_GPF_L	0xB0000098
#define REG_MFP_GPF_H	0xB000009C
#define REG_MFP_GPG_L	0xB00000A0
#define REG_MFP_GPG_H	0xB00000A4
#define REG_MFP_GPH_L	0xB00000A8
#define REG_MFP_GPH_H	0xB00000AC
#define REG_MFP_GPI_L	0xB00000B0
#define REG_MFP_GPI_H	0xB00000B4
#define SYS_RSTDEBCTL	0xB000010C
#define SYS_REGWPCTL    0xB00001FC

void NUC980_UnLock(void);
void NUC980_Lock(void);
int NUC980_cleanup(void);
extern int spi_flash_reset(void);

DECLARE_GLOBAL_DATA_PTR;

void NUC980_UnLock(void)
{
	do {
		writel(0x59, SYS_REGWPCTL);
		writel(0x16, SYS_REGWPCTL);
		writel(0x88, SYS_REGWPCTL);
		//wait for write-protection disabled indicator raised
	} while(!(readl(SYS_REGWPCTL) & 1));
}

void NUC980_Lock(void)
{
	writel(0x0, SYS_REGWPCTL);
}

static unsigned int sdram_size(unsigned int config)
{
	unsigned int size = 0;

	config &= 0x7;

	switch (config) {
	case 0:
		size = 0;
		break;
	case 1:
		size = 0x200000;
		break;
	case 2:
		size = 0x400000;
		break;
	case 3:
		size = 0x800000;
		break;
	case 4:
		size = 0x1000000;
		break;
	case 5:
		size = 0x2000000;
		break;
	case 6:
		size = 0x4000000;
		break;
	case 7:
		size = 0x8000000;
		break;
	}

	return(size);
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].size = sdram_size(readl(REG_SDIC_SIZE0));
	gd->bd->bi_dram[0].start = gd->bd->bi_dram[0].size == 0 ? 0 : (readl(REG_SDIC_SIZE0) & 0x1FE00000);

	gd->bd->bi_dram[1].size = sdram_size(readl(REG_SDIC_SIZE1));
	gd->bd->bi_dram[1].start = gd->bd->bi_dram[1].size == 0 ? 0 : (readl(REG_SDIC_SIZE1) & 0x1FE00000);
}

int dram_init(void)
{
	gd->ram_size = sdram_size(readl(REG_SDIC_SIZE0)) + sdram_size(readl(REG_SDIC_SIZE1));

	return(0);
}


int board_early_init_f(void)
{
	writel(readl(REG_PCLKEN0) | 0x10000, REG_PCLKEN0);   // UART clk
	writel(readl(REG_PCLKEN0) | 0x100, REG_PCLKEN0);   // Timer clk
#ifdef CONFIG_NUC980_GPIO
	writel(readl(REG_HCLKEN) | 0x800, REG_HCLKEN);   // GPIO clk
#endif

	return 0;
}

#ifdef CONFIG_NUC980_ETH
extern int nuc980_eth_register(void);

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_NUC980_EMAC0
	writel(readl(REG_HCLKEN) | 0x10000, REG_HCLKEN);   // EMAC0 clk
	writel(readl(REG_CLKDIVCTL8) | 0x10, REG_CLKDIVCTL8); //MII management interface clock
	//Init multi-function pin for RMII
	writel(0x11111111, REG_MFP_GPE_L);	// pin E0~E7 for RMII0
	writel((readl(REG_MFP_GPE_H) & ~0xff) | 0x11, REG_MFP_GPE_H);// pin E8~E9 for RMII0
#else //CONFIG_NUC980_EMAC1
	writel(readl(REG_HCLKEN) | 0x20000, REG_HCLKEN);   // EMAC1 clk
	writel(readl(REG_CLKDIVCTL8) | 0x10, REG_CLKDIVCTL8); //MII management interface clock
	//Init multi-function pin for RMII
	writel(0x11111111, REG_MFP_GPF_L);	// pin F0~F7 for RMII1
	writel((readl(REG_MFP_GPF_H) & ~0xff) | 0x11, REG_MFP_GPF_H);// pin F8~F9 for RMII1
#endif

	return(nuc980_eth_register());
}

#endif

#ifdef CONFIG_NUC980_MMC
#define REG_CLKDIVCTL3  0xB000022C
#define REG_CLKDIVCTL9  0xB0000244
#define FMI_BA    	0xB0019000 /* Flash Memory Card Interface */
#define SDH_BA          0xB0018000 /* SD Host */
#define REG_SDH_GCTL    (SDH_BA+0x800)   /* Global Control and Status Register */
#define REG_ECTL        (SDH_BA+0x840)   /* SD Host extend control register */
#define REG_SDCSR       (SDH_BA+0x820)   /* SD control and status register */
#define REG_FMICTL      (FMI_BA+0x800)  /* FMI control and status register */
#define REG_EMMCCTL     (FMI_BA+0x820)  /* eMMC control register */
#define GCTL_RST        0x1
#define SD_EN           0x2
#define EMMC_EN         0x2
#define CLK74_OE        0x20

int board_mmc_init(bd_t *bd)
{

	volatile int i;

#ifdef CONFIG_NUC980_SD_PORT0
	writel(readl(REG_HCLKEN) | 0x40000000, REG_HCLKEN);   // SDH clk
#endif
#if defined(CONFIG_NUC980_SD_PORT0)
	writel((readl(REG_CLKDIVCTL9) & ~0x18), REG_CLKDIVCTL9); //Set SDH clock source from XIN
	//writel((readl(REG_CLKDIVCTL9) & ~0xFF00) | (0x1d << 8), REG_CLKDIVCTL9); //Set SDH clock divider => 400 KHz
	writel((readl(REG_CLKDIVCTL9) & ~0xFF00) | (0x3b << 8), REG_CLKDIVCTL9); //Set SDH clock divider => 200 KHz

	writel(GCTL_RST, REG_SDH_GCTL);
	for(i = 0; i < 10; i++);        // Need few clock delay 'til SW_RST auto cleared.
	writel(SD_EN, REG_SDH_GCTL);

	writel(readl(REG_SDCSR) | CLK74_OE, REG_SDCSR);
	while(readl(REG_SDCSR) & CLK74_OE);
#endif

#ifdef CONFIG_NUC980_SD_PORT0

	writel((readl(REG_MFP_GPF_L) & ~0x0fffffff) | 0x02222222, REG_MFP_GPF_L); // pin F0~F6 for SD1

	nuc980_mmc_init(0); // init for SD port
#endif

#ifdef CONFIG_NUC980_EMMC
#ifdef CONFIG_NAND_NUC980
# error Do not enable eMMC(CONFIG_NUC980_EMMC) and NAND(CONFIG_NAND_NUC980) at the same time!
#endif

	writel(readl(REG_HCLKEN) | 0x00700000, REG_HCLKEN);   // eMMC & NAND & FMI clk
	writel((readl(REG_CLKDIVCTL3) & ~0x18), REG_CLKDIVCTL3); //Set eMMC clock source from XIN
	writel((readl(REG_CLKDIVCTL3) & ~0xFF00) | (0x3b << 8), REG_CLKDIVCTL3); //Set eMMC clock divider => 200 KHz
	writel(SD_EN, REG_FMICTL);

	writel(readl(REG_EMMCCTL) | CLK74_OE, REG_EMMCCTL);
	while(readl(REG_EMMCCTL) & CLK74_OE);

	// Set MFP the same port as NAND according to power-on setting register
	writel((readl(REG_MFP_GPC_L) & ~0xfff00000) | 0x66600000, REG_MFP_GPC_L); // pin C5~C7 for eMMC0
	writel((readl(REG_MFP_GPC_H) & ~0x000f0fff) | 0x00060666, REG_MFP_GPC_H); // pin C8~C10 for eMMC0

	nuc980_mmc_init(1); // init for eMMC
#endif

	return 0;

}
#endif

#ifdef CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
int overwrite_console(void)
{

	return 1;  // force serial interface as stdin/stdout/stderr even lcd interface enabled
}
#endif

void enable_caches(void)
{
#ifndef CONFIG_SYS_ICACHE_OFF
	icache_enable();
#endif
#ifndef CONFIG_SYS_DCACHE_OFF
	dcache_enable();
#endif

	return;
}

/* Enable reset debounce and set debounce counter to 0xFFF */
static void enable_ResetDebounce(void)
{
	NUC980_UnLock();
	writel(0x80000FFF, SYS_RSTDEBCTL);
	NUC980_Lock();
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	enable_ResetDebounce();

	return 0;
}

int board_late_init(void)
{
#ifdef CONFIG_HW_WATCHDOG
	hw_watchdog_init();
#endif

#ifdef CONFIG_PWM_NUC980
	/* Set multi-function pin PB.13 for PWM0 channel 2 */
	writel((readl(REG_MFP_GPB_H) & ~0xF00000) | 0x400000, REG_MFP_GPB_H);

	/* enable PWM0 channel 2, div 1, no invert */
	pwm_init(2, 1, 0);
	pwm_enable(2);
	/* duty cycle 32000000ns, period: 65000000ns */
	pwm_config(2, 32000000, 65000000);
#endif

	return 0;
}

int NUC980_cleanup(void)
{

#ifdef CONFIG_SYS_USE_SPIFLASH
	spi_flash_reset();
#endif

	//Reset multi-function pins to GPIO, except PG[15:11] while JTAGSEL(SYS_PWRON[4]) = 1, or PA[6:2] while JTAGSEL(SYS_PWRON[4]) = 0
	if (readl(REG_PWRON) & 0x00000010)
		writel(0, REG_MFP_GPA_L);
	else
		writel((readl(REG_MFP_GPA_L) & 0x0FFFFF00), REG_MFP_GPA_L);

	writel(0, REG_MFP_GPA_H);
	writel(0, REG_MFP_GPB_L);
#ifdef CONFIG_PWM_NUC980
	writel(0x400000, REG_MFP_GPB_H); /* Keep PB.13 for PWM0 channel 2 works */
#else
	writel(0, REG_MFP_GPB_H);
#endif
	writel(0, REG_MFP_GPC_L);
	writel(0, REG_MFP_GPC_H);
	writel(0, REG_MFP_GPD_L);
	writel(0, REG_MFP_GPD_H);
	writel(0, REG_MFP_GPE_L);
	writel(0, REG_MFP_GPE_H);
	writel(0, REG_MFP_GPF_L);
	writel(0, REG_MFP_GPF_H);
	writel(0, REG_MFP_GPG_L);
	if (readl(REG_PWRON) & 0x00000010)
		writel((readl(REG_MFP_GPG_H) & 0xFFFFF000), REG_MFP_GPG_H);
	else
		writel(0, REG_MFP_GPG_H);
	writel(0, REG_MFP_GPH_L);
	writel(0, REG_MFP_GPH_H);
	writel(0, REG_MFP_GPI_L);
	writel(0, REG_MFP_GPI_H);

	return 0;
}

int checkboard(void)
{
	puts("Board: NUC980\n");

	return 0;
}

/* Lowlevel init isn't used on nuc980, so just provide a dummy one here */
void lowlevel_init(void) {}


