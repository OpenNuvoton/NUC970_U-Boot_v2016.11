/*
 * IC Plus PHY drivers
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Copyright 2019 Infomatika LLC.
 * author Valery Gaynullin
 */
#include <phy.h>

/* ICPlus IP101 */
static int ip101_config(struct phy_device *phydev)
{

	genphy_config_aneg(phydev);

	return 0;
}

static int ip101_startup(struct phy_device *phydev)
{
	int ret;

	ret = genphy_update_link(phydev);
	return ret;

}

static struct phy_driver IP101_driver = {
	.name = "IC Plus IP101A/G",
	.uid = 0x02430c54,
	.mask = 0x0ffffff0,
	.features = PHY_BASIC_FEATURES,
	.config = &ip101_config,
	.startup = &ip101_startup,
	.shutdown = &genphy_shutdown,
};

int phy_icplus_init(void)
{
	phy_register(&IP101_driver);

	return 0;
}
