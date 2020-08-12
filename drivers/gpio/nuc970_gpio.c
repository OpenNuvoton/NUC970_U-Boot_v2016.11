/*
 * NUC970 GPIO driver
 *
 * Copyright (c) 2015 Nuvoton Technology Corporation
 * All rights reserved.
 *
 */
#include <common.h>
#include <asm/io.h>
#include <linux/errno.h>

#include <asm-generic/gpio.h>
#include "nuc970_gpio.h"

struct gpio_port {
	volatile unsigned int * dir;
	volatile unsigned int * out;
	volatile unsigned int * in;
};

static const struct gpio_port port_class[] = {
	{(volatile unsigned int *)REG_GPIOA_DIR, (volatile unsigned int *)REG_GPIOA_DATAOUT,
	 (volatile unsigned int *)REG_GPIOA_DATAIN},
	{(volatile unsigned int *)REG_GPIOB_DIR, (volatile unsigned int *)REG_GPIOB_DATAOUT,
	 (volatile unsigned int *)REG_GPIOB_DATAIN},
	{(volatile unsigned int *)REG_GPIOC_DIR, (volatile unsigned int *)REG_GPIOC_DATAOUT,
	 (volatile unsigned int *)REG_GPIOC_DATAIN},
	{(volatile unsigned int *)REG_GPIOD_DIR, (volatile unsigned int *)REG_GPIOD_DATAOUT,
	 (volatile unsigned int *)REG_GPIOD_DATAIN},
	{(volatile unsigned int *)REG_GPIOE_DIR, (volatile unsigned int *)REG_GPIOE_DATAOUT,
	 (volatile unsigned int *)REG_GPIOE_DATAIN},
	{(volatile unsigned int *)REG_GPIOF_DIR, (volatile unsigned int *)REG_GPIOF_DATAOUT,
	 (volatile unsigned int *)REG_GPIOF_DATAIN},
	{(volatile unsigned int *)REG_GPIOG_DIR, (volatile unsigned int *)REG_GPIOG_DATAOUT,
	 (volatile unsigned int *)REG_GPIOG_DATAIN},
	{(volatile unsigned int *)REG_GPIOH_DIR, (volatile unsigned int *)REG_GPIOH_DATAOUT,
	 (volatile unsigned int *)REG_GPIOH_DATAIN},
	{(volatile unsigned int *)REG_GPIOI_DIR, (volatile unsigned int *)REG_GPIOI_DATAOUT,
	 (volatile unsigned int *)REG_GPIOI_DATAIN},
	{(volatile unsigned int *)REG_GPIOJ_DIR, (volatile unsigned int *)REG_GPIOJ_DATAOUT,
	 (volatile unsigned int *)REG_GPIOJ_DATAIN},
	{},
};

static const struct gpio_port *nuc970_gpio_cla_port(unsigned gpio,
						    int *num)
{
	int group;
	group = gpio / GPIO_OFFSET;
	*num = gpio % GPIO_OFFSET;
	return &port_class[group];
}

/**
 * Set value of the specified gpio
 */
int gpio_set_value(unsigned gpio, int val)
{
	int port_num, value;
	const struct gpio_port *port =
	nuc970_gpio_cla_port(gpio, &port_num);


	value = readl(port->out);
	if (val)
		value |= (1 << port_num);
	else
		value &= ~(1 << port_num);
	writel(value, port->out);

	return 0;
}

/**
 * Get value of the specified gpio
 */
int gpio_get_value(unsigned gpio)
{
	int port_num, value;
	const struct gpio_port *port;
	port = nuc970_gpio_cla_port(gpio, &port_num);
	value = 0;

	if ((readl(port->dir) & (1 << port_num))) {	//GPIO OUT
		value = (readl(port->out) >> port_num) & 0x1;

	} else {		//GPIO IN
		value = (readl(port->in) >> port_num) & 0x1;
	}

	return value;
}

/**
 * Set gpio direction as input
 */
int gpio_direction_input(unsigned gpio)
{
	int port_num;
	unsigned long value;
	const struct gpio_port *port =
	    nuc970_gpio_cla_port(gpio, &port_num);

	value = readl(port->dir);
	value &= ~(1 << port_num);
	writel(value, port->dir);

	return 0;
}

/**
 * Set gpio direction as output
 */
int gpio_direction_output(unsigned gpio, int val)
{
	int port_num;
	unsigned long value;
	const struct gpio_port *port =
	    nuc970_gpio_cla_port(gpio, &port_num);

	value = readl(port->dir);
	value |= (1 << port_num);
	writel(value, port->dir);
	gpio_set_value(gpio, val);

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
	if (num > 7)
	{
		num -= 8;
		reg = reg + 0x04 ;
	}

	value =	( readl((volatile unsigned int *)reg) & (0xf<<(num*4)))>>(num*4);
	if(value>0 && value<0xf)
	{
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

