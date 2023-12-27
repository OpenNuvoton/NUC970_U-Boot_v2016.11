/**
  ******************************************************************************
  * @file    /WB_SerialNAND_Sample_Code_LLD.h
  * @author  Winbond FAE Steam Lin
  * @version V1.0.3
  * @date    9-March-2016
  * @brief   This code provide the low level hardware initial and basic Serial operate function based on STM32F205.
  *
  * COPYRIGHT 2015 Winbond.
*/

/* CWWeng
#include "stm32f2xx.h"
#include "WB_SPI_Code.h"
*/

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

/* status check */
uint8_t WB_Check_Embedded_ECC(void);
uint8_t WB_Read_Serial_NAND_StatusRegister(uint8_t sr_sel);
void WB_Serial_NAND_ReadyBusy_Check(void);
uint32_t WB_NAND_Read_JEDEC_ID(void);
uint8_t WB_Serial_NAND_bad_block_check(uint32_t page_address, uint32_t page_size);
void WB_Serial_NAND_LUT_Read(uint16_t* LBA, uint16_t* PBA);

/* Stack function for W25M series */
void WB_Die_Select(uint8_t select_die);

/* status set */
void WB_Enable_Embedded_ECC(void);
void WB_Disable_Embedded_ECC(void);
void WB_Enable_Buffer_mode(void);
void WB_Disable_Buffer_mode(void);
void WB_Serial_NAND_StatusRegister_Write_SR1(uint8_t SR1);
void WB_Serial_NAND_StatusRegister_Write_SR2(uint8_t SR2);
void WB_Serial_NAND_StatusRegister_Write_SR3(uint8_t SR3);
void WB_NAND_Reset(void);
void WB_Protect(void);
void WB_Unprotect(void);
void WB_Serial_NAND_LUT_Set(uint16_t LBA, uint16_t PBA);

/* read function */
void WB_Serial_NAND_PageDataRead(uint8_t addr2, uint8_t addr1, uint8_t addr0);
void WB_Serial_NAND_Normal_Read(uint8_t addh, uint8_t addl, uint8_t* buff, uint32_t count);
void WB_Serial_NAND_Continuous_Normal_Read(uint8_t* buff, uint32_t count);

/* Hardware Control */
void WB_CS_LOW(void);
void WB_CS_HIGH(void);
uint8_t SPIin(uint8_t DI);

/* CWWeng add */
void nuc980_spi_init(void);
void Initial_Serial_NAND(void);
uint8_t Check_JEDEC_ID(void);
uint8_t Check_Serial_NAND_Read_Write(void);
