/**
  ******************************************************************************
  * @file    /WB_SerialNAND_Sample_Code_LLD.c
  * @author  Winbond FAE Steam Lin
  * @version V1.0.9
  * @date    9-March-2016
  * @brief   This code provide the low level hardware initial and basic Serial operate function based on STM32F205.
  *
  * COPYRIGHT 2015 Winbond.
*/
#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <command.h>
#include "wb_spinand.h"

extern void sysprintf(char* pcStr,...);
#define printf(fmt, arg...) sysprintf(fmt, ##arg) //CWWeng add

#define REG_HCLKEN      0xB0000210
#define REG_PCLKEN1     0xB000021C
#define REG_MFP_GPD_L	0xB0000088
#define SPI_BASE       0xB0060000 //QSPI0
#define SPI_CTL        (SPI_BASE + 0x00)
#define SPI_CLKDIV     (SPI_BASE + 0x04)
#define SPI_SSCTL      (SPI_BASE + 0x08)
#define SPI_PDMACTL    (SPI_BASE + 0x0C)
#define SPI_FIFOCTL    (SPI_BASE + 0x10)
#define SPI_STATUS     (SPI_BASE + 0x14)
#define SPI_TX         (SPI_BASE + 0x20)
#define SPI_RX         (SPI_BASE + 0x30)

/* spi register bit */
#define ENINT           (0x01 << 17)
#define TXNEG           (0x01 << 2)
#define RXNEG           (0x01 << 1)
#define LSB             (0x01 << 13)
#define SELECTLEV       (0x01 << 2)
#define SELECTPOL       (0x01 << 3)
#define SELECTSLAVE0    0x01
#define SELECTSLAVE1    0x02
#define SPIEN           0x01
#define SPIENSTS        (0x01 << 15)
#define TXRXRST         (0x01 << 23)
#define SPI_BUSY        0x00000001
#define SPI_SS_ACT      0x00000001
#define SPI_SS_HIGH     0x00000004
#define SSCR_AUTOSS_Msk (1 << 3)
#define SSCR_SS_LVL_Msk (1 << 2)

/********************
Function: Serial NAND continuous read to buffer
Argument:
addh~addl: Read data address
count: read count
return:
*********************/
void WB_Serial_NAND_Continuous_Normal_Read(uint8_t* buff, uint32_t count)
{
	uint32_t i = 0;
	WB_CS_LOW();
	SPIin(0x03); // dummy
	SPIin(0x00); // dummy
	SPIin(0x00); // dummy
	SPIin(0x00); // dummy
	for( i = 0; i < count; i++) {
		*(buff+i) = SPIin(0x00);
	}
	WB_CS_HIGH();
	return;
}

/********************
Function: W25M series (SPISTACK) die select
Argument:
	Select_die
		0x00  (power on default states)
		0x01
return:
*********************/
void WB_Die_Select(uint8_t select_die)
{

	WB_CS_LOW();
	SPIin(0xC2);		// Software Die Select
	SPIin(select_die);
	WB_CS_HIGH();

	return;
}

/********************
Function: Serial NAND BBM Set LUT
Argument:
  LBA                   Logical block address
  PBA                   Physical block address
  * LBA or PBA          0       1       2
  * Page address        0       0x40   0x80
  return:
*********************/
void WB_Serial_NAND_LUT_Set(uint16_t LBA, uint16_t PBA)
{

	WB_CS_LOW();
	SPIin(0x06);
	WB_CS_HIGH();

	WB_CS_LOW();
	SPIin(0xA1);
	SPIin((LBA/0x100));
	SPIin((LBA%0x100));
	SPIin((PBA/0x100));
	SPIin((PBA%0x100));
	WB_CS_HIGH();
}

/********************
Function: Serial NAND LUT read
Argument:
  uint16_t LBA[20];             Logical block address
  uint16_t PBA[20];             Physical block address
  For Winbond 1Gb NAND, LBA[9:0] & PBA [9:0] are effective block address. LBA[15:14] is used for additional information
return:
*********************/
void WB_Serial_NAND_LUT_Read(uint16_t* LBA, uint16_t* PBA)
{
	uint16_t i, buf1, buf2;
	WB_CS_LOW();
	SPIin(0xA5);
	SPIin(0x00);                  // Dummy
	for(i = 0; i < 20; i++) {
		buf1 = SPIin(0x00);
		buf2 = SPIin(0x00);
		*(LBA+i) = (buf1 << 8) | buf2;
		buf1 = SPIin(0x00);
		buf2 = SPIin(0x00);
		*(PBA+i) = (buf1 << 8) | buf2;
	}
	WB_CS_HIGH();
	return;
}

/********************\
Function: Serial NAND Bad block mark check
Argument:
return:
1: Check block is bad block.
0: Check block is not bad block.
update: V.1.0.8 // correct the bad block mark address
*********************/
uint8_t WB_Serial_NAND_bad_block_check(uint32_t page_address, uint32_t page_size)
{
	uint8_t read_buf;
	//uint8_t EPR_status;
	WB_Serial_NAND_PageDataRead((page_address >> 16) & 0xFF,
				    (page_address >> 8) & 0xFF,
				     page_address & 0xFF);    // Read the first page of a block

	/* if build-in ECC algorithm enable
	EPR_status = WB_Check_Embedded_ECC_Flag();
	if(EPR_status > 1){
	  return 1;															// Check ECC status and return fail if (ECC-1, ECC0) = (1,0) or (1, 1)
		  }
	*/
#ifdef CONFIG_SPI_NAND_MICRON
	WB_Serial_NAND_Normal_Read((page_size >> 8) | (page_address & (1 << 6) ? (1 << 4) : 0), 0x0, &read_buf, 1);
#else
	WB_Serial_NAND_Normal_Read((page_size >> 8), 0x0, &read_buf, 1);	// Read bad block mark at 0x800 update at v.1.0.8
#endif
	if(read_buf != 0xFF) {	// update at v.1.0.7
		return 1;
	}
	WB_Serial_NAND_PageDataRead(((page_address + 1) >> 16) & 0xFF,
				    ((page_address + 1) >> 8) & 0xFF,
				     (page_address+1) & 0xFF);	// Read the second page of a block

	/* if build-in ECC algorithm enable
	EPR_status = WB_Check_Embedded_ECC_Flag();
	if(EPR_status > 1){
	  return 1;															// Check ECC status and return fail if (ECC-1, ECC0) = (1,0) or (1, 1)
		  }
	*/
#ifdef CONFIG_SPI_NAND_MICRON
	WB_Serial_NAND_Normal_Read((page_size >> 8) | ((page_address+1) & (1 << 6) ? (1 << 4) : 0), 0x0, &read_buf, 1);
#else
	WB_Serial_NAND_Normal_Read((page_size >> 8), 0x0, &read_buf, 1);	// Read bad block mark at 0x800 update at v.1.0.8
#endif
	if(read_buf != 0xFF) {	// update at v.1.0.7
		return 1;
	}
	return 0;
}


/********************
Function: Do whole Flash protect
Argument:
*********************/
void WB_Protect(void)
{
	uint8_t SR;
	SR = WB_Read_Serial_NAND_StatusRegister(1); 	// Read status register 2
	SR|=0x7C;					// Enable ECC-E bit
	WB_Serial_NAND_StatusRegister_Write_SR1(SR);
	return;
}

/********************
Function: Do whole Flash unprotect
Argument:
*********************/
void WB_Unprotect(void)
{
	uint8_t SR;
	SR = WB_Read_Serial_NAND_StatusRegister(1); 	// Read status register 2
	SR&=0x83;					// Enable ECC-E bit
	WB_Serial_NAND_StatusRegister_Write_SR1(SR);
	return;
}

/********************
Function: Check ECC-E status
Argument:
Comment: Change function name at V1.0.3
*********************/
uint8_t WB_Check_Embedded_ECC_Enable(void)
{
	uint8_t SR;
	SR = WB_Read_Serial_NAND_StatusRegister(2);  // Read status register 2
	return (SR&0x10)>>4;
}

/********************
Function: Check ECC-1, ECC-0 status
Argument:
Comment: Add this function at V1.0.3
*********************/
uint8_t WB_Check_Embedded_ECC_Flag(void)
{
	uint8_t SR;
	SR = WB_Read_Serial_NAND_StatusRegister(3); 	// Read status register 3
	return (SR&0x30)>>4;				// Check ECC-1, ECC0 bit
}

/********************
Function: Enable embedded ECC
Argument:
*********************/
void WB_Enable_Embedded_ECC(void)
{
	uint8_t SR;
	SR = WB_Read_Serial_NAND_StatusRegister(2); 	// Read status register 2
	SR|=0x10;					// Enable ECC-E bit
	WB_Serial_NAND_StatusRegister_Write_SR2(SR);
	return;
}

/********************
Function: Disable embedded ECC
Argument:
*********************/
void WB_Disable_Embedded_ECC(void)
{
	uint8_t SR;
	SR = WB_Read_Serial_NAND_StatusRegister(2); 	// Read status register 2
	SR&= 0xEF;					// Disable ECC-E bit
	WB_Serial_NAND_StatusRegister_Write_SR2(SR);
	return;
}

/********************
Function: Enable buffer mode
Argument:
*********************/
void WB_Enable_Buffer_mode(void)
{
	uint8_t SR;
	SR = WB_Read_Serial_NAND_StatusRegister(2); 	// Read status register 2
	SR|=0x08;					// Enable BUF bit
	WB_Serial_NAND_StatusRegister_Write_SR2(SR);
	return;
}

/********************
Function: Disable buffer mode
Argument:
*********************/
void WB_Disable_Buffer_mode(void)
{
	uint8_t SR;
	SR = WB_Read_Serial_NAND_StatusRegister(2); 	// Read status register 2
	SR&= 0xF7;					// Disable BUF bit
	WB_Serial_NAND_StatusRegister_Write_SR2(SR);
	return;
}


/********************
Function: Serial NAND Write Status register 1
Argument:
SR1: program SR1 data
*********************/
void WB_Serial_NAND_StatusRegister_Write_SR1(uint8_t SR1)
{
	WB_CS_LOW();
	//SPIin(0x01); CWWeng : Winbond only
	SPIin(0x1F); //CWWeng : for all
	SPIin(0xA0);
	SPIin(SR1);
	WB_CS_HIGH();
	WB_Serial_NAND_ReadyBusy_Check();
	return;
}

/********************
Function: Serial NAND Write Status register 2
Argument:
SR2: program SR2 data
*********************/
void WB_Serial_NAND_StatusRegister_Write_SR2(uint8_t SR2)
{

	WB_CS_LOW();
	//SPIin(0x01); CWWeng : Winbond only
	SPIin(0x1F); //CWWeng : for all
	SPIin(0xB0);
	SPIin(SR2);
	WB_CS_HIGH();
	WB_Serial_NAND_ReadyBusy_Check();
	return;
}

/********************
Function: Serial NAND Write Status register 3
Argument:
 : select socket
SR3: program SR3 data
*********************/
void WB_Serial_NAND_StatusRegister_Write_SR3(uint8_t SR3)
{
	WB_CS_LOW();
	//SPIin(0x01); CWWeng : Winbond only
	SPIin(0x1F); //CWWeng : for all
	SPIin(0xC0);
	SPIin(SR3);
	WB_CS_HIGH();
	WB_Serial_NAND_ReadyBusy_Check();
	return;
}

/********************
Function: Serial NAND Status register read
Argument:
sr_sel: select register
return: status register value
*********************/
uint8_t WB_Read_Serial_NAND_StatusRegister(uint8_t sr_sel)
{
	uint8_t SR = 0;  // status register data
	switch(sr_sel) {
	case 0x01:
		WB_CS_LOW();
		//SPIin(0x05); CWWeng : Winbond only
		SPIin(0x0F); //CWWeng : for all
		SPIin(0xA0); // SR1
		SR = SPIin(0x00);
		WB_CS_HIGH();
		break;
	case 0x02:
		WB_CS_LOW();
		//SPIin(0x05); CWWeng : Winbond only
		SPIin(0x0F); //CWWeng : for all
		SPIin(0xB0); // SR2
		SR = SPIin(0x00);
		WB_CS_HIGH();
		break;
	case 0x03:
		WB_CS_LOW();
		//SPIin(0x05); CWWeng : Winbond only
		SPIin(0x0F); //CWWeng : for all
		SPIin(0xC0); // SR3
		SR = SPIin(0x00);
		WB_CS_HIGH();
		break;
	default:
		SR = 0xFF;
		break;
	}
	return SR;
}

/********************
Function: SPINAND page data read
Argument:
PA_H, page address
PA_L, page address
return:
*********************/
void WB_Serial_NAND_PageDataRead(uint8_t addr2, uint8_t addr1, uint8_t addr0)
{
	WB_CS_LOW();
	SPIin(0x13); //
	SPIin(addr2); // Page address
	SPIin(addr1); // Page address
	SPIin(addr0); // Page address
	WB_CS_HIGH();
	WB_Serial_NAND_ReadyBusy_Check(); // Need to wait for the data transfer.
	return;
}

/********************
Function: Serial NAND Normal read to buffer
Argument:
addh~addl: Read data address
count: read count
return:
*********************/
void WB_Serial_NAND_Normal_Read(uint8_t addh, uint8_t addl, uint8_t* buff, uint32_t count)
{
	uint32_t i = 0;
	WB_CS_LOW();
	SPIin(0x03);
#ifdef	CONFIG_SPI_NAND_GD
	SPIin(0x00); // dummy
#endif
	SPIin(addh);
	SPIin(addl);
#ifndef	CONFIG_SPI_NAND_GD
	SPIin(0x00); // dummy
#endif
	for( i = 0; i < count; i++) {
		*(buff+i) = SPIin(0x00);
	}
	WB_CS_HIGH();
	return;
}

/********************
Function: Serial NAND reset command
Argument:
return:
*********************/
void WB_NAND_Reset(void)
{
	WB_CS_LOW();
	SPIin(0xFF);
	WB_CS_HIGH();
	WB_Serial_NAND_ReadyBusy_Check();
}


/********************
Function: SPI NAND Ready busy check
Argument:
return:
*********************/
void WB_Serial_NAND_ReadyBusy_Check(void)
{
	uint8_t SR = 0xFF;
	//while((SR & 0x3) != 0x00){ CWWeng
	while((SR & 0x1) != 0x00) {
		WB_CS_LOW();
		//SPIin(0x05); Winbond only
		SPIin(0x0F); //CWWeng : for all
		SPIin(0xC0);
		SR = SPIin(0x00);
		WB_CS_HIGH();
	}
	return;
}

/********************
Function: read JEDEC ID
Argument: cs sel
return: JEDEC ID
*********************/
uint32_t WB_NAND_Read_JEDEC_ID(void)
{
	uint32_t JEDECID = 0;
	WB_CS_LOW();
	SPIin(0x9F);
	SPIin(0x00); // dummy
	JEDECID += SPIin(0x00);
	JEDECID <<= 8;
	JEDECID += SPIin(0x00);
	JEDECID <<= 8;
	JEDECID += SPIin(0x00);
	WB_CS_HIGH();
	return JEDECID;
}

/********************
Function: CS select low
Argument:
return:
*********************/
void WB_CS_LOW(void)
{
	// /CS: active
	writel((readl(SPI_SSCTL) & ~(SSCR_AUTOSS_Msk|SSCR_SS_LVL_Msk))| SPI_SS_ACT, SPI_SSCTL);
}

/********************
Function: CS select high
Argument:
return:
*********************/
void WB_CS_HIGH(void)
{
	// /CS: de-active
	writel(readl(SPI_SSCTL) & ~(SSCR_AUTOSS_Msk|SSCR_SS_LVL_Msk|SPI_SS_ACT), SPI_SSCTL);
}

/********************
Function: SPI hardware command send
Argument: DI: DI data
return: DO data
*********************/
uint8_t SPIin(uint8_t DI)
{
	writel(DI, SPI_TX);
	while ((readl(SPI_STATUS) & 0x100)); //RXEMPTY
	return ((unsigned char)readl(SPI_RX) & 0xff);
}

void nuc980_spi_init(void)
{

	writel(readl(REG_PCLKEN1) | 0x10, REG_PCLKEN1);       // QSPI0 clk
	writel(readl(REG_MFP_GPD_L) | 0x00111100, REG_MFP_GPD_L); //QSPI0: D2, D3, D4, D5

	writel(0, SPI_SSCTL); /* AUTOSS=0; low-active; de-select all SS pins. */
	writel((readl(SPI_CTL) & ~0x1F00) | 0x800, SPI_CTL); //Data Width = 8 bit
	writel(4, SPI_CLKDIV); //150/5 = 30 MHz

	writel(readl(SPI_FIFOCTL) | 0x3, SPI_FIFOCTL); //TX/RX reset
	while ((readl(SPI_STATUS) & TXRXRST));

	writel((readl(SPI_CTL) & ~0xFF)|5, SPI_CTL);

	writel(readl(SPI_CTL) | SPIEN, SPI_CTL);
	while ((readl(SPI_STATUS) & SPIENSTS) == 0);

	WB_NAND_Reset();
}

