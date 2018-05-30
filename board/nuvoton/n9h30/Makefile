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
obj-y	+= my_n9h30_nand.o
obj-y	+= my_nand.o
obj-y	+= my_nand_ecc.o
obj-y	+= my_nand_ids.o
obj-y	+= string.o

obj-y	+= serial_n9h30.o
obj-y	+= serial.o
obj-y	+= n9h30_sysprintf.o
obj-y	+= vsprintf.o
obj-y	+= div64.o
obj-y	+= console.o
else
obj-y	:= n9h30.o
obj-y	+= cmd_decrypt.o
obj-y	+= cmd_rtc_spare.o
endif
