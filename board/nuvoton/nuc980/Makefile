#
# Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
#
# (C) Copyright 2011 Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier:	GPL-2.0+
#

ifdef CONFIG_SPL_BUILD
ifdef CONFIG_SYS_USE_SPIFLASH
obj-y	+= spinand_boot.o
obj-y	+= wb_spinand.o
else
obj-y	+= nand_boot.o
obj-y	+= my_nand_base.o
obj-y	+= my_nuc980_nand.o
obj-y	+= my_nand.o
obj-y	+= my_nand_ecc.o
obj-y	+= my_nand_ids.o
endif
obj-y	+= string.o

obj-y   += serial_nuc980.o
obj-y   += serial.o
obj-y   += nuc980_sysprintf.o
obj-y   += vsprintf.o
obj-y   += div64.o
obj-y   += console.o
else
obj-y	:= nuc980.o
obj-y	+= cmd_rtc_spare.o
endif
