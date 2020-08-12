/*
 * Fixed PHY driver
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Copyright 2019 Infomatika LLC.
 * author Valery Gaynullin
 */
#include <phy.h>

static int fixed_config(struct phy_device *phydev)
{
	return 0;
}

static int fixed_startup(struct phy_device *phydev)
{
	phydev->speed = SPEED_100;
	phydev->duplex = DUPLEX_FULL;
	phydev->link = 1;
	return 0;
}

static struct phy_driver FIXED_driver = {
	.name = "Fixed PHY",
	.uid  = 0x00000000,
	.mask = 0x0ffffff0,
	.features = PHY_BASIC_FEATURES,
	.config = &fixed_config,
	.startup = &fixed_startup,
	.shutdown = &fixed_shutdown,
};

int phy_fixed_init(void)
{
	phy_register(&FIXED_driver);

	return 0;
}
