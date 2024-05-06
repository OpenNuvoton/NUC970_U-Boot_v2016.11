/*
 * (C) Copyright 2024
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * PWM driver for NUC980
 */

#include <common.h>
#include <div64.h>
#include <pwm.h>
#include <asm/io.h>

#define REG_PCLKEN1              (0xB000021C)

#define PWM0_BA                  (0xB0058000)
#define REG_PWM0_PPR             (PWM0_BA+0x00)
#define REG_PWM0_CSR             (PWM0_BA+0x04)
#define REG_PWM0_PCR             (PWM0_BA+0x08)
#define REG_PWM0_CNR0            (PWM0_BA+0x0C)
#define REG_PWM0_CMR0            (PWM0_BA+0x10)
#define REG_PWM0_PDR0            (PWM0_BA+0x14)
#define REG_PWM0_CNR1            (PWM0_BA+0x18)
#define REG_PWM0_CMR1            (PWM0_BA+0x1C)
#define REG_PWM0_PDR1            (PWM0_BA+0x20)
#define REG_PWM0_CNR2            (PWM0_BA+0x24)
#define REG_PWM0_CMR2            (PWM0_BA+0x28)
#define REG_PWM0_PDR2            (PWM0_BA+0x2C)
#define REG_PWM0_CNR3            (PWM0_BA+0x30)
#define REG_PWM0_CMR3            (PWM0_BA+0x34)
#define REG_PWM0_PDR3            (PWM0_BA+0x38)
#define REG_PWM0_PIER            (PWM0_BA+0x3C)
#define REG_PWM0_PIIR            (PWM0_BA+0x40)

#define PWM1_BA                  (0xB0059000)
#define REG_PWM1_PPR             (PWM1_BA+0x00)
#define REG_PWM1_CSR             (PWM1_BA+0x04)
#define REG_PWM1_PCR             (PWM1_BA+0x08)
#define REG_PWM1_CNR0            (PWM1_BA+0x0C)
#define REG_PWM1_CMR0            (PWM1_BA+0x10)
#define REG_PWM1_PDR0            (PWM1_BA+0x14)
#define REG_PWM1_CNR1            (PWM1_BA+0x18)
#define REG_PWM1_CMR1            (PWM1_BA+0x1C)
#define REG_PWM1_PDR1            (PWM1_BA+0x20)
#define REG_PWM1_CNR2            (PWM1_BA+0x24)
#define REG_PWM1_CMR2            (PWM1_BA+0x28)
#define REG_PWM1_PDR2            (PWM1_BA+0x2C)
#define REG_PWM1_CNR3            (PWM1_BA+0x30)
#define REG_PWM1_CMR3            (PWM1_BA+0x34)
#define REG_PWM1_PDR3            (PWM1_BA+0x38)
#define REG_PWM1_PIER            (PWM1_BA+0x3C)
#define REG_PWM1_PIIR            (PWM1_BA+0x40)

int pwm_init(int pwm_id, int div, int invert)
{
	if (pwm_id < 4) {
		writel(readl(REG_PCLKEN1) | 0x4000000, REG_PCLKEN1);
		/* Set clock divider */
		switch (div) {
		case 2 :
			writel((readl(REG_PWM0_CSR) & (~0x7 << (pwm_id*4))), REG_PWM0_CSR);
			break;
		case 4 :
			writel((readl(REG_PWM0_CSR) & (~0x7 << (pwm_id*4))) | (1 << (pwm_id*4)), REG_PWM0_CSR);
			break;
		case 8 :
			writel((readl(REG_PWM0_CSR) & (~0x7 << (pwm_id*4))) | (2 << (pwm_id*4)), REG_PWM0_CSR);
			break;
		case 16 :
			writel((readl(REG_PWM0_CSR) & (~0x7 << (pwm_id*4))) | (3 << (pwm_id*4)), REG_PWM0_CSR);
			break;
		case 1 :
			writel((readl(REG_PWM0_CSR) & (~0x7 << (pwm_id*4))) | (4 << (pwm_id*4)), REG_PWM0_CSR);
			break;
		}
		/* Set output polarity */
		if (pwm_id == 0)
			writel(((readl(REG_PWM0_PCR) & (~0x4)) | (invert << 2)), REG_PWM0_PCR);
		else if (pwm_id == 1)
			writel(((readl(REG_PWM0_PCR) & (~0x400)) | (invert << 10)), REG_PWM0_PCR);
		else if (pwm_id == 2)
			writel(((readl(REG_PWM0_PCR) & (~0x4000)) | (invert << 14)), REG_PWM0_PCR);
		else if (pwm_id == 3)
			writel(((readl(REG_PWM0_PCR) & (~0x40000)) | (invert << 18)), REG_PWM0_PCR);
	} else {
		writel(readl(REG_PCLKEN1) | 0x8000000, REG_PCLKEN1);
		switch (div) {
		case 2 :
			writel((readl(REG_PWM1_CSR) & (~0x7 << (pwm_id*4))), REG_PWM1_CSR);
			break;
		case 4 :
			writel((readl(REG_PWM1_CSR) & (~0x7 << (pwm_id*4))) | (1 << (pwm_id*4)), REG_PWM1_CSR);
			break;
		case 8 :
			writel((readl(REG_PWM1_CSR) & (~0x7 << (pwm_id*4))) | (2 << (pwm_id*4)), REG_PWM1_CSR);
			break;
		case 16 :
			writel((readl(REG_PWM1_CSR) & (~0x7 << (pwm_id*4))) | (3 << (pwm_id*4)), REG_PWM1_CSR);
			break;
		case 1 :
			writel((readl(REG_PWM1_CSR) & (~0x7 << (pwm_id*4))) | (4 << (pwm_id*4)), REG_PWM1_CSR);
			break;
		}
		/* Set output polarity */
		if (pwm_id == 4)
			writel(((readl(REG_PWM1_PCR) & (~0x4)) | (invert << 2)), REG_PWM1_PCR);
		else if (pwm_id == 5)
			writel(((readl(REG_PWM1_PCR) & (~0x400)) | (invert << 10)), REG_PWM1_PCR);
		else if (pwm_id == 6)
			writel(((readl(REG_PWM1_PCR) & (~0x4000)) | (invert << 14)), REG_PWM1_PCR);
		else if (pwm_id == 7)
			writel(((readl(REG_PWM1_PCR) & (~0x40000)) | (invert << 18)), REG_PWM1_PCR);
	}

	return 0;
}

int pwm_config(int pwm_id, int duty_ns, int period_ns)
{
	unsigned long period, duty, prescale;

	/* Set PCLK to 75 MHz, calculate valid parameter range. */
	prescale = 75000000 / 1000000 - 1;

	/* now pwm time unit is 1000ns. */
	period = (period_ns + 500) / 1000;
	duty = (duty_ns + 500) / 1000;

	/* don't want the minus 1 below change the value to -1 (0xFFFF) */
	if(period == 0)
		period = 1;
	if(duty == 0)
		duty = 1;

	/* Set prescale for all pwm channels */
	if (pwm_id < 4)
		writel(prescale | (prescale << 8), REG_PWM0_PPR);
	else
		writel(prescale | (prescale << 8), REG_PWM1_PPR);

	if (pwm_id == 0) {
		writel(period - 1, REG_PWM0_CNR0);
		writel(duty - 1, REG_PWM0_CMR0);
	} else if(pwm_id == 1) {
		writel(period - 1, REG_PWM0_CNR1);
		writel(duty - 1, REG_PWM0_CMR1);
	} else if (pwm_id == 2) {
		writel(period - 1, REG_PWM0_CNR2);
		writel(duty - 1, REG_PWM0_CMR2);
	} else if (pwm_id == 3) {
		writel(period - 1, REG_PWM0_CNR3);
		writel(duty - 1, REG_PWM0_CMR3);
	} else if (pwm_id == 4) {
		writel(period - 1, REG_PWM1_CNR0);
		writel(duty - 1, REG_PWM1_CMR0);
	} else if (pwm_id == 5) {
		writel(period - 1, REG_PWM1_CNR1);
		writel(duty - 1, REG_PWM1_CMR1);
	} else if (pwm_id == 6) {
		writel(period - 1, REG_PWM1_CNR2);
		writel(duty - 1, REG_PWM1_CMR2);
	} else if (pwm_id == 7) {
		writel(period - 1, REG_PWM1_CNR3);
		writel(duty - 1, REG_PWM1_CMR3);
	}

	return 0;
}

int pwm_enable(int pwm_id)
{
	if (pwm_id == 0)
		writel(readl(REG_PWM0_PCR) | 0x9, REG_PWM0_PCR);
	else if (pwm_id == 1)
		writel(readl(REG_PWM0_PCR) | 0x900, REG_PWM0_PCR);
	else if (pwm_id == 2)
		writel(readl(REG_PWM0_PCR) | 0x9000, REG_PWM0_PCR);
	else if (pwm_id == 3)
		writel(readl(REG_PWM0_PCR) | 0x90000, REG_PWM0_PCR);
	else if (pwm_id == 4)
		writel(readl(REG_PWM1_PCR) | 0x9, REG_PWM1_PCR);
	else if (pwm_id == 5)
		writel(readl(REG_PWM1_PCR) | 0x900, REG_PWM1_PCR);
	else if (pwm_id == 6)
		writel(readl(REG_PWM1_PCR) | 0x9000, REG_PWM1_PCR);
	else if (pwm_id == 7)
		writel(readl(REG_PWM1_PCR) | 0x90000, REG_PWM1_PCR);

	return 0;
}

void pwm_disable(int pwm_id)
{
	if (pwm_id == 0)
		writel((readl(REG_PWM0_PCR) & (~0x9)), REG_PWM0_PCR);
	else if (pwm_id == 1)
		writel((readl(REG_PWM0_PCR) & (~0x900)), REG_PWM0_PCR);
	else if (pwm_id == 2)
		writel((readl(REG_PWM0_PCR) & (~0x9000)), REG_PWM0_PCR);
	else if (pwm_id == 3)
		writel((readl(REG_PWM0_PCR) & (~0x90000)), REG_PWM0_PCR);
	else if (pwm_id == 4)
		writel((readl(REG_PWM1_PCR) & (~0x9)), REG_PWM1_PCR);
	else if (pwm_id == 5)
		writel((readl(REG_PWM0_PCR) & (~0x900)), REG_PWM1_PCR);
	else if (pwm_id == 6)
		writel((readl(REG_PWM0_PCR) & (~0x9000)), REG_PWM1_PCR);
	else if (pwm_id == 7)
		writel((readl(REG_PWM0_PCR) & (~0x90000)), REG_PWM1_PCR);
}

