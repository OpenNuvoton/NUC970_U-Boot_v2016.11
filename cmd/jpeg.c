/*
 * Copyright (c) 2020 Nuvoton Technology Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 */

#include <common.h>
#include <command.h>
#include <part.h>
#include "jpeg.h"

DECLARE_GLOBAL_DATA_PTR;
// assume min jpeg file size is 8k
#define MIN_JPEG_SIZE	8192
#define REG_CLK_HCLKEN		0xB0000210
#define REG_CLK_DIVCTL3		0xB000022C
#define REG_SYS_AHBIPRST	0xB0000060


void jpegSetDecodeMode(u32 u32OutputFormat)
{
	outp32(REG_JITCR, u32OutputFormat);
	outp32(REG_JMCR,inp32(REG_JMCR) & ~ENC_DEC);
}

void jpegDecodeTrigger(void)
{
	JPEG_CLEAR_INT(DEC_INTS | JPEG_DER_INTS | JPEG_DHE_INTS | JPEG_IPW_INTS | JPG_DOW_INTS);
	outp32(REG_JMCR, JPG_EN | inp32(REG_JMCR));
	outp32(REG_JMCR, ~JPG_EN & inp32(REG_JMCR));
}

void jpegSetDimension(u16 u16Height, u16 u16Width)
{
	outp32(REG_JPRIWH,((u16Height & 0x1FFF)<<16) | (u16Width & 0x1FFF) );
}

int jpegOpen(void)
{
	outp32(REG_CLK_HCLKEN, (inp32(REG_CLK_HCLKEN) | (1 << 29))); // Enable JPEG clock
	outp32(REG_CLK_DIVCTL3, (inp32(REG_CLK_DIVCTL3) & ~0xf0000000) | ( 1 << 28));

	return 0;
}


void jpegInit(void)
{
	/* Set the default values of the JPEG registers */

	outp32(REG_JPRIQC, 0x000000F4);
	outp32(REG_JTHBQC, 0x000000F4);
	outp32(REG_JPRST, 0x00000004);
	outp32(REG_JTRST, 0x00000004);
	outp32(REG_JITCR, 0x00000000);
	outp32(REG_JINTCR, 0x00000000);
	outp32(REG_JDOWFBS,0xFFFFFFFF);

	// Disable the Primary Up-scaling & Scaling-down
	outp32(REG_JPSCALU, 0x00000000);
	outp32(REG_JPSCALD, 0x00000000);

	// Reset JUPRAT and JSRCH
	outp32(REG_JUPRAT, 0x00000000);
	outp32(REG_JSRCH, 0x00000FFF);
	//-------------------------------------------

	/* Reset JPEG (JMCR [1]) */
	outp32(REG_JMCR,0x00000002);
	outp32(REG_JMCR,0x00000000);
	outp32(REG_JMACR, 0x00400000);  //Can't use single buffer
}



void jpegClose(void)
{

	outp32(REG_JMCR,0x00000002);
	outp32(REG_JMCR,0x00000000);
}


static u32 start = 0;
static u32 end = 0;
static u32 dly = 0;
static u32 h = 0;
static u32 w = 0;
static u32 playing = 0;
static u32 t = 0;

void find_soi(void)
{
	while(1) {
		if(start + MIN_JPEG_SIZE >= end) {
			playing = 0;
			break;
		}
		if(*(u8 *)start == 0xFF)
			if(*(u8 *)(start + 1) == 0xD9)
			{
				start += 2;
				break;
			}
		start++;
	}

}


bool jpegIsStop(void)
{
	return playing == 0;
}

void JPEG_TRIGGER(void)
{
	if (!playing)
		return;
	if (get_timer(0) - t < dly)
		return;
	t = get_timer(0);
	jpegInit();
	jpegSetDecodeMode(JPEG_DEC_PRIMARY_PACKET_RGB565);
	JPEG_SET_YADDR((u32)gd->fb_base);
	jpegSetDimension(h, w);

	JPEG_SET_BITSTREAM_ADDR(start);
	jpegDecodeTrigger();
	find_soi();
}

int do_jpeg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

	if (argc != 6) {
		cmd_usage(cmdtp);
		return 1;
	}

	start = simple_strtoul(argv[1], NULL, 16);
	end = start + simple_strtoul(argv[2], NULL, 16);
	h = simple_strtoul(argv[3], NULL, 16);
	w = simple_strtoul(argv[4], NULL, 16);
	dly = 1000 / simple_strtoul(argv[5], NULL, 16);

	jpegOpen();
	t = get_timer(0);

	jpegInit();
	jpegSetDecodeMode(JPEG_DEC_PRIMARY_PACKET_RGB565);
	JPEG_SET_YADDR((u32)gd->fb_base);
	jpegSetDimension(h, w);

	JPEG_SET_BITSTREAM_ADDR(start);
	jpegDecodeTrigger();

	playing = 1;

	start += MIN_JPEG_SIZE;
	find_soi();
	return 0;
}

U_BOOT_CMD(
    jpeg,	6,	0,	do_jpeg,
    "Decode JPEG image",
    "<start> <size> <height> <width> <fps>"
);
