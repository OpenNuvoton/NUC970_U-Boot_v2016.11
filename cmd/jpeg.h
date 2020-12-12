/**************************************************************************//**
* @file     jpeg.h
* @version  V1.00
* $Revision: 1 $
* $Date: 15/06/08 11:49a $
* @brief    NUC970 JPEG driver header file
*
* @note
* SPDX-License-Identifier: Apache-2.0
* Copyright (C) 2015 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#ifndef __JPEG_H__
#define __JPEG_H__
#include <common.h>
#include <asm/io.h>

#define outp32(reg, val)    	writel((val), (reg))
#define inp32    		readl

// Define bits mask
#define NVTBIT(start,end) ((0xFFFFFFFFUL >> (31 - start)) & (0xFFFFFFFFUL >>end  << end))

#define BIT0		(1 << 0)
#define BIT1		(1 << 1)
#define BIT2		(1 << 2)
#define BIT3		(1 << 3)
#define BIT4		(1 << 4)
#define BIT5		(1 << 5)
#define BIT6		(1 << 6)
#define BIT7		(1 << 7)
#define BIT8		(1 << 8)
#define BIT9		(1 << 9)
#define BIT10		(1 << 10)
#define BIT11		(1 << 11)
#define BIT12		(1 << 12)
#define BIT13		(1 << 13)
#define BIT14		(1 << 14)
#define BIT15		(1 << 15)
#define BIT16		(1 << 16)
#define BIT17		(1 << 17)
#define BIT18		(1 << 18)
#define BIT19		(1 << 19)
#define BIT20		(1 << 20)
#define BIT21		(1 << 21)
#define BIT22		(1 << 22)
#define BIT23		(1 << 23)
#define BIT24		(1 << 24)
#define BIT25		(1 << 25)
#define BIT26		(1 << 26)
#define BIT27		(1 << 27)
#define BIT28		(1 << 28)
#define BIT29		(1 << 29)
#define BIT30		(1 << 30)
#define BIT31		(1 << 31)

//JMCR
#define RESUMEI		BIT9				/*!< Resume JPEG Operation for Input On-the-Fly Mode          */
#define RESUMEO		BIT8				/*!< Resume JPEG Operation for Output On-the-Fly Mode   */
#define ENC_DEC		BIT7				/*!< JPEG Encode/Decode Mode   */
#define WIN_DEC		BIT6				/*!< JPEG Window Decode Mode   */
#define PRI		BIT5				/*!< Encode Primary Image   */
#define THB		BIT4				/*!< Encode Thumbnail Image   */
#define EY422		BIT3				/*!< Encode Image Format   */
#define QT_BUSY		BIT2				/*!< Quantization-Table Busy Status (Read-Only)   */
#define ENG_RST		BIT1				/*!< Soft Reset JPEG Engine (Except JPEG Control Registers)   */
#define JPG_EN		BIT0				/*!< JPEG Engine Operation Control  */

//JITCR
#define Dec_Scatter_Gather	BIT18
#define DEC_OTF	        	BIT17				/*!< Decoder on the fly with VPE	                                    */
#define ARGB8888		BIT16				/*!< ARGB8888  */
#define PLANAR_ON		BIT15				/*!< Packet On  */
#define ORDER			BIT14				/*!< Decode Packet Data Order  */
#define RGB_555_565		BIT13				/*!< RGB555 & RGB565  */
#define ROTATE			NVTBIT(12,11)		/*!< Encode Image Rotate  */
#define DYUV_MODE		NVTBIT(10,8)		/*!< Decoded Image YUV Color Format (Read-Only)  */
#define EXIF			BIT7				/*!< Encode Quantization-Table & Huffman-Table Header Format Selection  */
#define EY_ONLY			BIT6				/*!< Encode Gray-level (Y-component Only) Image                     */
#define DHEND			BIT5				/*!< Header Decode Complete Stop Enable  */
#define DTHB			BIT4				/*!< Decode Thumbnail Image Only  */
#define E3QTAB			BIT3				/*!< Numbers of Quantization-Table are Used For Encode  */
#define D3QTAB			BIT2				/*!< Numbers of Quantization-Table are Used For Decode (Read-Only)  */
#define ERR_DIS			BIT1				/*!< Decode Error Engine Abort  */
#define PDHTAB			BIT0				/*!< Programmable Huffman-Table Function For Decode  */

//JINTCR
#define JPG_DOW_INTE	BIT28               /*!< Decoding Output Wait Interrupt Enable  */
#define JPG_DOW_INTS	BIT24               /*!< Status of Decoding Output Wait  */
#define JPG_WAITI	BIT23				/*!< JPEG Input Wait Status (Read-Only)  */
#define JPG_WAITO	BIT22				/*!< JPEG Output Wait Status (Read-Only)  */
#define BAbort		BIT16				/*!< JPEG Memory Access Error Status (Read-Only)  */
#define CER_INTE        BIT15				/*!< Un-complete Capture On-The-Fly Frame Occur Interrupt Enable  */
#define DHE_INTE        BIT14				/*!< JPEG Header Decode End Wait Interrupt Enable  */
#define IPW_INTE        BIT13				/*!< Input Wait Interrupt Enable  */
#define OPW_INTE        BIT12				/*!< Output Wait Interrupt Enable  */
#define ENC_INTE        BIT11				/*!< Encode Complete Interrupt Enable  */
#define DEC_INTE        BIT10				/*!< Decode Complete Interrupt Enable  */
#define DER_INTE        BIT9				/*!< Decode Error Interrupt Enable  */
#define EER_INTE        BIT8				/*!< Encode (On-The-Fly) Error Interrupt Enable  */
#define CER_INTS        BIT7				/*!< Un-complete Capture On-The-Fly Frame Occur Interrupt Status  */
#define DHE_INTS        BIT6				/*!< JPEG  Header Decode End Wait Interrupt Status  */
#define IPW_INTS        BIT5				/*!< Input Wait Interrupt Status  */
#define OPW_INTS        BIT4				/*!< Output Wait Interrupt Status  */
#define ENC_INTS        BIT3				/*!< Encode Complete Interrupt Status  */
#define DEC_INTS        BIT2				/*!< Decode Complete Interrupt Status  */
#define DER_INTS        BIT1				/*!< Decode Error Interrupt Status  */
#define EER_INTS	BIT0				/*!< Encode (On-The-Fly) Error Interrupt Status  */


/*@}*/ /* end of group NUC970_JPEG_EXPORTED_CONSTANTS */

/// @cond HIDDEN_SYMBOLS

//Define for Interrupt Status
#define JPEG_EER_INTS	EER_INTS
#define JPEG_DER_INTS	DER_INTS
#define JPEG_DEC_INTS	DEC_INTS
#define JPEG_ENC_INTS	ENC_INTS
#define JPEG_DHE_INTS	DHE_INTS
#define JPEG_IPW_INTS	IPW_INTS

//Define for Scaling
#define JPEG_DEC_PACKET_DOWNSCALE_MODE			1

//Define for Interrupt Enable
#define JPEG_EER_INTE	ERR_INTE
#define JPEG_DER_INTE	DER_INTE
#define JPEG_DEC_INTE	DEC_INTE
#define JPEG_ENC_INTE	ENC_INTE
#define JPEG_DHE_INTE	DHE_INTE
#define JPEG_IPW_INTE	IPW_INTE

//Register
#define     JPEG_BASE		(0xB000A000UL)
#define     REG_JMCR		(JPEG_BASE + 0x000)
#define     REG_JHEADER		(JPEG_BASE + 0x004)
#define     REG_JITCR		(JPEG_BASE + 0x008)
#define     REG_JPRIQC		(JPEG_BASE + 0x010)
#define     REG_JTHBQC		(JPEG_BASE + 0x014)
#define     REG_JPRIWH		(JPEG_BASE + 0x018)
#define     REG_JTHBWH		(JPEG_BASE + 0x01C)
#define     REG_JPRST		(JPEG_BASE + 0x020)
#define     REG_JTRST		(JPEG_BASE + 0x024)
#define     REG_JDECWH		(JPEG_BASE + 0x028)
#define     REG_JINTCR		(JPEG_BASE + 0x02C)
#define     REG_JDOWFBS		(JPEG_BASE + 0x03C)
#define     REG_JWINDEC0	(JPEG_BASE + 0x044)
#define     REG_JWINDEC1	(JPEG_BASE + 0x048)
#define     REG_JWINDEC2	(JPEG_BASE + 0x04C)
#define     REG_JMACR		(JPEG_BASE + 0x050)
#define     REG_JPSCALU		(JPEG_BASE + 0x054)
#define     REG_JPSCALD		(JPEG_BASE + 0x058)
#define     REG_JTSCALD		(JPEG_BASE + 0x05C)
#define     REG_JDBCR		(JPEG_BASE + 0x060)
#define     REG_JRESERVE	(JPEG_BASE + 0x070)
#define     REG_JOFFSET		(JPEG_BASE + 0x074)
#define     REG_JFSTRIDE	(JPEG_BASE + 0x078)
#define     REG_JYADDR0		(JPEG_BASE + 0x07C)
#define     REG_JUADDR0		(JPEG_BASE + 0x080)
#define     REG_JVADDR0		(JPEG_BASE + 0x084)
#define     REG_JYADDR1		(JPEG_BASE + 0x088)
#define     REG_JUADDR1		(JPEG_BASE + 0x08C)
#define     REG_JVADDR1		(JPEG_BASE + 0x090)
#define     REG_JYSTRIDE	(JPEG_BASE + 0x094)
#define     REG_JUSTRIDE	(JPEG_BASE + 0x098)
#define     REG_JVSTRIDE	(JPEG_BASE + 0x09C)
#define     REG_JIOADDR0	(JPEG_BASE + 0x0A0)
#define     REG_JIOADDR1	(JPEG_BASE + 0x0A4)
#define     REG_JPRI_SIZE	(JPEG_BASE + 0x0A8)
#define     REG_JTHB_SIZE	(JPEG_BASE + 0x0AC)
#define     REG_JUPRAT		(JPEG_BASE + 0x0B0)
#define     REG_JBSFIFO		(JPEG_BASE + 0x0B4)
#define     REG_JSRCH		(JPEG_BASE + 0x0B8)
#define     REG_JQTAB0		(JPEG_BASE + 0x100)
#define     REG_JQTAB1		(JPEG_BASE + 0x140)
#define     REG_JQTAB2		(JPEG_BASE + 0x180)

#define JPEG_DEC_PRIMARY_PACKET_RGB565		0x06001  /*!< JPEG decode output Primary Packet RGB565 */
#define JPEG_DEC_PRIMARY_PACKET_RGB888		0x14001  /*!< JPEG decode Primary Packet RGB888 */

//Export functions
#define JPEG_SET_YADDR(u32Address)			outp32(REG_JYADDR0, u32Address)
#define JPEG_SET_BITSTREAM_ADDR(u32Address)		outp32(REG_JIOADDR0,u32Address)
#define JPEG_SET_ENC_DEC(u8Mode)			outp32(REG_JMCR, (inp32(REG_JMCR) & ~ENC_DEC) | (u8Mode << 7));

//Decode
#define JPEG_DEC_DISABLE_DOWNSCALING()			outp32(REG_JPSCALD,~PSX_ON)

#define JPEG_GET_INT_STATUS()				(inp32(REG_JINTCR) & 0x010000FF)
#define JPEG_CLEAR_INT(u32Intflag)			outp32(REG_JINTCR, (inp32 (REG_JINTCR) & ~0xFF) | u32Intflag)

int jpegOpen(void);
void jpegClose(void);
void jpegInit(void);
bool jpegIsStop(void);
void jpegSetDimension(u16 u16Height, u16 u16Width);

void jpegSetDecodeMode(u32 u8OutputFormat);
void jpegDecodeTrigger(void);

#endif
