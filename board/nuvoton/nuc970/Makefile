#
# Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
#
# (C) Copyright 2011 Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier:	GPL-2.0+
#

ifdef CONFIG_SPL_BUILD
obj-y	+= nand_boot.o
obj-y	+= my_nand_base.o
obj-y	+= my_nuc970_nand.o
obj-y	+= my_nand.o
obj-y	+= my_nand_ecc.o
obj-y	+= my_nand_ids.o
obj-y	+= string.o
else
obj-y	:= nuc970.o
obj-y	+= cmd_decrypt.o
endif
