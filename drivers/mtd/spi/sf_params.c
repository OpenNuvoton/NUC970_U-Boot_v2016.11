/*
 * SPI flash Params table
 *
 * Copyright (C) 2013 Jagannadha Sutradharudu Teki, Xilinx Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <spi.h>
#include <spi_flash.h>

#include "sf_internal.h"

/* SPI/QSPI flash device params structure */
const struct spi_flash_params spi_flash_params_table[] = {
#ifdef CONFIG_SPI_FLASH_ATMEL		/* ATMEL */
	{"AT45DB011D",	   0x1f2200, 0x0,	64 * 1024,     4, SECT_4K},
	{"AT45DB021D",	   0x1f2300, 0x0,	64 * 1024,     8, SECT_4K},
	{"AT45DB041D",	   0x1f2400, 0x0,	64 * 1024,     8, SECT_4K},
	{"AT45DB081D",	   0x1f2500, 0x0,	64 * 1024,    16, SECT_4K},
	{"AT45DB161D",	   0x1f2600, 0x0,	64 * 1024,    32, SECT_4K},
	{"AT45DB321D",	   0x1f2700, 0x0,	64 * 1024,    64, SECT_4K},
	{"AT45DB641D",	   0x1f2800, 0x0,	64 * 1024,   128, SECT_4K},
	{"AT25DF321A",     0x1f4701, 0x0,	64 * 1024,    64, SECT_4K},
	{"AT25DF321",      0x1f4700, 0x0,	64 * 1024,    64, SECT_4K},
	{"AT26DF081A",     0x1f4501, 0x0,	64 * 1024,    16, SECT_4K},
#endif
#ifdef CONFIG_SPI_FLASH_EON		/* EON */
	{"EN25Q32B",	   0x1c3016, 0x0,	64 * 1024,    64, 0},
	{"EN25Q64",	   0x1c3017, 0x0,	64 * 1024,   128, SECT_4K},
	{"EN25Q128B",	   0x1c3018, 0x0,       64 * 1024,   256, 0},
	{"EN25S64",	   0x1c3817, 0x0,	64 * 1024,   128, 0},
	{"EN25Q64",	   0x1c7017, 0x0,	64 * 1024,   128, SECT_4K},
	{"EN25QH16-104HIP",0x1c7015, 0x0,	64 * 1024,   64, 0},
	{"EN25QH128",	   0x1c7018, 0x0,	64 * 1024,   256, RD_QUAD | SECT_4K},
	{"EN25QH256",	   0x1c7019, 0x0,	64 * 1024,   512, RD_QUAD | SECT_4K},
#endif
#ifdef CONFIG_SPI_FLASH_GIGADEVICE	/* GIGADEVICE */
	{"GD25B256E",	   0xc84019, 0x0,	64 * 1024,   512, SECT_4K},
	{"GD25Q128",	   0xc84018, 0x0,	64 * 1024,   256, SECT_4K},
	{"GD25Q32",	   0xc84016, 0x0,	64 * 1024,   64, SECT_4K},
	{"GD25Q64B",	   0xc84017, 0x0,	64 * 1024,   128, SECT_4K},
	{"GD25LQ32",	   0xc86016, 0x0,	64 * 1024,    64, SECT_4K},
#endif
#ifdef CONFIG_SPI_FLASH_ISSI		/* ISSI */
	{"IS25LP032",	   0x9d6016, 0x0,	64 * 1024,    64, 0},
	{"IS25LP064",	   0x9d6017, 0x0,	64 * 1024,   128, 0},
	{"IS25LP128",	   0x9d6018, 0x0,	64 * 1024,   256, 0},
#endif
#ifdef CONFIG_SPI_FLASH_MACRONIX	/* MACRONIX */
	{"MX25L2006E",	   0xc22012, 0x0,	64 * 1024,     4, 0},
	{"MX25L4005",	   0xc22013, 0x0,	64 * 1024,     8, 0},
	{"MX25L8005",	   0xc22014, 0x0,	64 * 1024,    16, 0},
	{"MX25L1605D",	   0xc22015, 0x0,	64 * 1024,    32, 0},
	{"MX25L3205D",	   0xc22016, 0x0,	64 * 1024,    64, 0},
	{"MX25L6405D",	   0xc22017, 0x0,	64 * 1024,   128, 0},
	{"MX25L12805",	   0xc22018, 0x0,	64 * 1024,   256, RD_FULL | WR_QPP},
	{"MX25L25635F",	   0xc22019, 0x0,	64 * 1024,   512, RD_FULL | WR_QPP},
	{"MX25L51235F",	   0xc2201a, 0x0,	64 * 1024,  1024, RD_FULL | WR_QPP},
	{"MX25L12855E",	   0xc22618, 0x0,	64 * 1024,   256, RD_FULL | WR_QPP},
#endif
#ifdef CONFIG_SPI_FLASH_SPANSION	/* SPANSION */
	{"S25FL008A",	   0x010213, 0x0,	64 * 1024,    16, 0},
	{"S25FL016A",	   0x010214, 0x0,	64 * 1024,    32, 0},
	{"S25FL032A",	   0x010215, 0x0,	64 * 1024,    64, 0},
	{"S25FL064A",	   0x010216, 0x0,	64 * 1024,   128, 0},
	{"S25FL116K",	   0x014015, 0x0,	64 * 1024,   128, 0},
	{"S25FL164K",	   0x014017, 0x0140,	64 * 1024,   128, 0},
	{"S25FL128P_256K", 0x012018, 0x0300,   256 * 1024,    64, RD_FULL | WR_QPP},
	{"S25FL128P_64K",  0x012018, 0x0301,    64 * 1024,   256, RD_FULL | WR_QPP},
	{"S25FL032P",	   0x010215, 0x4d00,    64 * 1024,    64, RD_FULL | WR_QPP},
	{"S25FL064P",	   0x010216, 0x4d00,    64 * 1024,   128, RD_FULL | WR_QPP},
	{"S25FL128S_256K", 0x012018, 0x4d00,   256 * 1024,    64, RD_FULL | WR_QPP},
	{"S25FL128S_64K",  0x012018, 0x4d01,    64 * 1024,   256, RD_FULL | WR_QPP},
	{"S25FL256S_256K", 0x010219, 0x4d00,   256 * 1024,   128, RD_FULL | WR_QPP},
	{"S25FL256S_64K",  0x010219, 0x4d01,	64 * 1024,   512, RD_FULL | WR_QPP},
	{"S25FS512S",      0x010220, 0x4D00,   128 * 1024,   512, RD_FULL | WR_QPP},
	{"S25FL512S_256K", 0x010220, 0x4d00,   256 * 1024,   256, RD_FULL | WR_QPP},
	{"S25FL512S_64K",  0x010220, 0x4d01,    64 * 1024,  1024, RD_FULL | WR_QPP},
	{"S25FL512S_512K", 0x010220, 0x4f00,   256 * 1024,   256, RD_FULL | WR_QPP},
#endif
#ifdef CONFIG_SPI_FLASH_STMICRO		/* STMICRO */
	{"M25P10",	   0x202011, 0x0,	32 * 1024,     4, 0},
	{"M25P20",	   0x202012, 0x0,       64 * 1024,     4, 0},
	{"M25P40",	   0x202013, 0x0,       64 * 1024,     8, 0},
	{"M25P80",	   0x202014, 0x0,       64 * 1024,    16, 0},
	{"M25P16",	   0x202015, 0x0,       64 * 1024,    32, 0},
	{"M25PE16",	   0x208015, 0x1000,    64 * 1024,    32, 0},
	{"M25PX16",	   0x207115, 0x1000,    64 * 1024,    32, RD_QUAD | RD_DUAL},
	{"M25P32",	   0x202016, 0x0,       64 * 1024,    64, 0},
	{"M25P64",	   0x202017, 0x0,       64 * 1024,   128, 0},
	{"M25P128",	   0x202018, 0x0,      256 * 1024,    64, 0},
	{"M25PX64",	   0x207117, 0x0,       64 * 1024,   128, SECT_4K},
	{"N25Q016A",       0x20bb15, 0x0,	64 * 1024,    32, SECT_4K},
	{"N25Q32",	   0x20ba16, 0x0,       64 * 1024,    64, RD_FULL | WR_QPP | SECT_4K},
	{"N25Q32A",	   0x20bb16, 0x0,       64 * 1024,    64, RD_FULL | WR_QPP | SECT_4K},
	{"N25Q64",	   0x20ba17, 0x0,       64 * 1024,   128, RD_FULL | WR_QPP | SECT_4K},
	{"N25Q64A",	   0x20bb17, 0x0,       64 * 1024,   128, RD_FULL | WR_QPP | SECT_4K},
	{"N25Q128",	   0x20ba18, 0x0,       64 * 1024,   256, RD_FULL | WR_QPP},
	{"N25Q128A",	   0x20bb18, 0x0,       64 * 1024,   256, RD_FULL | WR_QPP},
	{"N25Q256",	   0x20ba19, 0x0,       64 * 1024,   512, RD_FULL | WR_QPP | SECT_4K},
	{"N25Q256A",	   0x20bb19, 0x0,       64 * 1024,   512, RD_FULL | WR_QPP | SECT_4K},
	{"N25Q512",	   0x20ba20, 0x0,       64 * 1024,  1024, RD_FULL | WR_QPP | E_FSR | SECT_4K},
	{"N25Q512A",	   0x20bb20, 0x0,       64 * 1024,  1024, RD_FULL | WR_QPP | E_FSR | SECT_4K},
	{"N25Q1024",	   0x20ba21, 0x0,       64 * 1024,  2048, RD_FULL | WR_QPP | E_FSR | SECT_4K},
	{"N25Q1024A",	   0x20bb21, 0x0,       64 * 1024,  2048, RD_FULL | WR_QPP | E_FSR | SECT_4K},
#endif
#ifdef CONFIG_SPI_FLASH_SST		/* SST */
	{"SST25VF040B",	   0xbf258d, 0x0,	64 * 1024,     8, SECT_4K | SST_WR},
	{"SST25VF080B",	   0xbf258e, 0x0,	64 * 1024,    16, SECT_4K | SST_WR},
	{"SST25VF016B",	   0xbf2541, 0x0,	64 * 1024,    32, SECT_4K | SST_WR},
	{"SST25VF032B",	   0xbf254a, 0x0,	64 * 1024,    64, SECT_4K | SST_WR},
	{"SST25VF064C",	   0xbf254b, 0x0,	64 * 1024,   128, SECT_4K},
	{"SST25WF512",	   0xbf2501, 0x0,	64 * 1024,     1, SECT_4K | SST_WR},
	{"SST25WF010",	   0xbf2502, 0x0,	64 * 1024,     2, SECT_4K | SST_WR},
	{"SST25WF020",	   0xbf2503, 0x0,	64 * 1024,     4, SECT_4K | SST_WR},
	{"SST25WF040",	   0xbf2504, 0x0,	64 * 1024,     8, SECT_4K | SST_WR},
	{"SST25WF040B",	   0x621613, 0x0,	64 * 1024,     8, SECT_4K},
	{"SST25WF080",	   0xbf2505, 0x0,	64 * 1024,    16, SECT_4K | SST_WR},
#endif
#ifdef CONFIG_SPI_FLASH_WINBOND		/* WINBOND */
	{"W25P80",	   0xef2014, 0x0,	64 * 1024,    16, 0},
	{"W25P16",	   0xef2015, 0x0,	64 * 1024,    32, 0},
	{"W25P32",	   0xef2016, 0x0,	64 * 1024,    64, 0},
	{"W25X40",	   0xef3013, 0x0,	64 * 1024,     8, SECT_4K},
	{"W25X16",	   0xef3015, 0x0,	64 * 1024,    32, SECT_4K},
	{"W25X32",	   0xef3016, 0x0,	64 * 1024,    64, SECT_4K},
	{"W25X64",	   0xef3017, 0x0,	64 * 1024,   128, SECT_4K},
	{"W25Q80BL",	   0xef4014, 0x0,	64 * 1024,    16, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q16CL",	   0xef4015, 0x0,	64 * 1024,    32, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q32BV",	   0xef4016, 0x0,	64 * 1024,    64, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q64CV",	   0xef4017, 0x0,	64 * 1024,   128, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q128BV",	   0xef4018, 0x0,	64 * 1024,   256, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q256",	   0xef4019, 0x0,	64 * 1024,   512, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q256JV",	   0xef7019, 0x0,	64 * 1024,   512, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q512JVF",	   0xef4020, 0x0,	64 * 1024,   1024, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q80BW",	   0xef5014, 0x0,	64 * 1024,    16, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q16DW",	   0xef6015, 0x0,	64 * 1024,    32, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q32DW",	   0xef6016, 0x0,	64 * 1024,    64, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q64DW",	   0xef6017, 0x0,	64 * 1024,   128, RD_FULL | WR_QPP | SECT_4K},
	{"W25Q128FW",	   0xef6018, 0x0,	64 * 1024,   256, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_FLASH_XTX		/* XTX */
	{"XT25F08BSSIGU",  0x0b4014, 	0x0,	64 * 1024,   16, RD_FULL | WR_QPP | SECT_4K},
	{"XT25F16BSSIGU",  0x0b4015, 	0x0,	64 * 1024,   32, RD_FULL | WR_QPP | SECT_4K},
	{"XT25F32BSSIGU",  0x0b4016, 	0x0,	64 * 1024,   64, RD_FULL | WR_QPP | SECT_4K},
	{"XT25F64BSSIGU",  0x0b4017, 	0x0,	64 * 1024,   128, RD_FULL | WR_QPP | SECT_4K},
	{"XT25F128BSSIGU",  0x0b4018, 	0x0,	64 * 1024,   256, RD_FULL | WR_QPP | SECT_4K},
	{"XT25F256BSSIGU",  0x0b4019, 	0x0,	64 * 1024,   512, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_FLASH_XMC		/* XMC */
	{"XM25QH256C",  0x204019, 	0x0,	64 * 1024,   512, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_NAND_WINBOND		/* WINBOND SPI NAND */
	{"W25N01GV",	   0xefaa21, 0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"W25N01KV",	   0xefae21, 0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"W25N02KV",	   0xefaa22, 0x0,	64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
	{"W25M02GV",	   0xefab21, 0x0,	64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
	{"W25N02JW",	   0xefbf22, 0x0,	64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
	{"W25N04KV",	   0xefaa23, 0x0,	64 * 2048,  4096, RD_FULL | WR_QPP | SECT_4K},
	{"GD5F1GQ4xExxG",  0xc8d1c8, 0x0,       64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},  //suppurt GD & WB in one uboot
	{"GD5F1GQ5xExxG",  0xc851c8, 0x0,       64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},  //suppurt GD & WB in one uboot
	{"GD5F2GQ5xExxG",  0xc852c8, 0x0,       64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
	{"TC58CVG0S3HRAIJ",0x98e240, 0x0,       64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},  //suppurt GD & WB & KIOXIA in one uboot
	{"FM25S01A",  	   0xa1e47f, 0x0,       64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},	//suppurt GD & WB & KIOXIA & FM25 in one uboot
	{"S35ML04G03", 	   0x013501, 0x0,       64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_NAND_MACRONIX		/* MACRONIX SPI NAND */
	{"MX35LF1GE4AB",   0xc212,   0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"MX35LF2GE4AB",   0xc222,   0x0,	64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_NAND_XTX		/* XTX SPI NAND */
	{"XT26G01A",	   0x0be1,   0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"XT26G02A",	   0x0be2,   0x0,	64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
	{"XT26G01BWSEGA",  0x0bf1,   0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"XT26G02BWSIGA",  0x0bf2,   0x0,	64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
	{"XT26G01CWSIGA",  0x0b11,   0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"XT26G11CWSIGA",  0x0b15,   0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
        {"XT26G04CWSIGA",  0x0b13,   0x0,	64 * 2048,  4096, RD_FULL | WR_QPP | SECT_4K},
 	{"XT26G02CWSIGA",  0x0b12,   0x0,	64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_NAND_MK		/* MK SPI NAND */
	{"MKSV1GCL-DE",	   0xd51c,   0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"MKSV1GCW-BE",	   0xd511,   0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"MKSV4GIL-DE",	   0xd50b,   0x0,	64 * 2048,  4096, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_NAND_ATO		/* ATO SPI NAND */
	{"ATO25D1GA",	   0x9b12,   0x0,	64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_NAND_GD              /* GigaDevice SPI NAND */
	{"GD5F1GQ4UCxxG",  0xc8b148,   0x0,     64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
	{"GD5F1GQ4RCxxG",  0xc8a148,   0x0,     64 * 2048,  1024, RD_FULL | WR_QPP | SECT_4K},
#endif
#ifdef CONFIG_SPI_NAND_MICRON		/* MICRON SPI NAND */
	{"MT29F2G01",	   0x2c24,   0x0,	64 * 2048,  2048, RD_FULL | WR_QPP | SECT_4K},
#endif
	{},	/* Empty entry to terminate the list */
	/*
	 * Note:
	 * Below paired flash devices has similar spi_flash params.
	 * (S25FL129P_64K, S25FL128S_64K)
	 * (W25Q80BL, W25Q80BV)
	 * (W25Q16CL, W25Q16DV)
	 * (W25Q32BV, W25Q32FV_SPI)
	 * (W25Q64CV, W25Q64FV_SPI)
	 * (W25Q128BV, W25Q128FV_SPI)
	 * (W25Q32DW, W25Q32FV_QPI)
	 * (W25Q64DW, W25Q64FV_QPI)
	 * (W25Q128FW, W25Q128FV_QPI)
	 */
};
