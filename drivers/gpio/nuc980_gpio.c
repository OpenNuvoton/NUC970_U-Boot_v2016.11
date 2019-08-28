/*
 * NUC980 GPIO driver
 *
 * Copyright (c) 2019 Nuvoton Technology Corporation
 * All rights reserved.
 *
 */
#include <common.h>
#include <asm/io.h>
#include <linux/errno.h>

#include <asm-generic/gpio.h>
#include "nuc980_gpio.h"

//#define GPIO_DEBUG_ENABLE_ENTER_LEAVE
#ifdef GPIO_DEBUG_ENABLE_ENTER_LEAVE
#define ENTRY()                 printf("[%-20s] : Enter...\n", __FUNCTION__)
#define LEAVE()                 printf("[%-20s] : Leave...\n", __FUNCTION__)
#else
#define ENTRY()
#define LEAVE()
#endif

#define GPIO_PMD_INPUT              0x0UL                  /*!< Input Mode */
#define GPIO_PMD_OUTPUT             0x1UL                  /*!< Output Mode */
#define GPIO_PMD_OPEN_DRAIN         0x2UL                  /*!< Open-Drain Mode */
#define GPIO_PMD_QUASI              0x3UL                  /*!< Quasi-bidirectional Mode */
#define GPIO_PMD_MODE(pin, mode)    ((mode) << ((pin)<<1)) /*!< Generate the PMD mode setting for each pin  */

struct gpio_port {
	volatile unsigned int * dir;
	volatile unsigned int * out;
	volatile unsigned int * in;
};

static const struct gpio_port port_class[] = {
	{
		(volatile unsigned int *)REG_GPIOA_MODE, (volatile unsigned int *)REG_GPIOA_DOUT,
		(volatile unsigned int *)REG_GPIOA_PIN
	},
	{
		(volatile unsigned int *)REG_GPIOB_MODE, (volatile unsigned int *)REG_GPIOB_DOUT,
		(volatile unsigned int *)REG_GPIOB_PIN
	},
	{
		(volatile unsigned int *)REG_GPIOC_MODE, (volatile unsigned int *)REG_GPIOC_DOUT,
		(volatile unsigned int *)REG_GPIOC_PIN
	},
	{
		(volatile unsigned int *)REG_GPIOD_MODE, (volatile unsigned int *)REG_GPIOD_DOUT,
		(volatile unsigned int *)REG_GPIOD_PIN
	},
	{
		(volatile unsigned int *)REG_GPIOE_MODE, (volatile unsigned int *)REG_GPIOE_DOUT,
		(volatile unsigned int *)REG_GPIOE_PIN
	},
	{
		(volatile unsigned int *)REG_GPIOF_MODE, (volatile unsigned int *)REG_GPIOF_DOUT,
		(volatile unsigned int *)REG_GPIOF_PIN
	},
	{
		(volatile unsigned int *)REG_GPIOG_MODE, (volatile unsigned int *)REG_GPIOG_DOUT,
		(volatile unsigned int *)REG_GPIOG_PIN
	},
	{},
};

static const struct gpio_port *nuc980_gpio_cla_port(unsigned gpio_num,
        unsigned *group,unsigned *num)
{
	*group = gpio_num / GPIO_OFFSET;
	*num = gpio_num % GPIO_OFFSET;
	return &port_class[*group];
}

/**
 * Set value of the specified gpio
 */
int gpio_set_value(unsigned gpio_num, int val)
{
	unsigned int port_num,group_num;
	const struct gpio_port *port;
	ENTRY();
	port = nuc980_gpio_cla_port(gpio_num, &group_num, &port_num);
	GPIO_PIN_DATA(group_num,port_num)=val;
	LEAVE();

	return 0;
}

/**
 * Get value of the specified gpio
 */
int gpio_get_value(unsigned gpio_num)
{
	unsigned int port_num,group_num;
	const struct gpio_port *port;
	ENTRY();
	port = nuc980_gpio_cla_port(gpio_num, &group_num, &port_num);
	LEAVE();
	return GPIO_PIN_DATA(group_num,port_num);
}

/**
 * Set gpio direction as input
 */
int gpio_direction_input(unsigned gpio_num)
{
	unsigned int port_num,group_num;
	unsigned long value;
	const struct gpio_port *port =
	    nuc980_gpio_cla_port(gpio_num, &group_num, &port_num);
	ENTRY();
	value = readl(port->dir);
	value &= ~GPIO_PMD_MODE(port_num,GPIO_PMD_QUASI);
	value |= GPIO_PMD_MODE(port_num,GPIO_PMD_INPUT);
	writel(value, port->dir);
	LEAVE();

	return 0;
}

/**
 * Set gpio direction as output
 */
int gpio_direction_output(unsigned gpio_num, int val)
{
	unsigned int port_num,group_num;
	unsigned long value;
	const struct gpio_port *port =
	    nuc980_gpio_cla_port(gpio_num, &group_num, &port_num);
	ENTRY();
	value = readl(port->dir);
	value &= ~GPIO_PMD_MODE(port_num,GPIO_PMD_QUASI);
	value |= GPIO_PMD_MODE(port_num,GPIO_PMD_OUTPUT);
	writel(value, port->dir);
	gpio_set_value(gpio_num, val);
	LEAVE();

	return 0;
}

/**
 * Request a gpio before using it.
 *
 * NOTE: Argument 'label' is unused.
 */
int gpio_request(unsigned gpio, const char *label)
{
	unsigned int group,num,reg,value;
	group = gpio / GPIO_OFFSET;
	num   = gpio % GPIO_OFFSET;
	reg   = (unsigned int)REG_MFP_GPA_L + (group* 0x08);
	if (num > 7) {
		num -= 8;
		reg = reg + 0x04 ;
	}

	value =	( readl((volatile unsigned int *)reg) & (0xf<<(num*4)))>>(num*4);
	if(value>0 && value<0xf) {
		printf("[%s] Please Check GPIO pin [%d], multi-function pins = 0x%x \n",__FUNCTION__,gpio,value);
		return -EINVAL;
	}
	return 0;
}

/**
 * Reset and free the gpio after using it.
 */
int gpio_free(unsigned gpio)
{
	return 0;
}

