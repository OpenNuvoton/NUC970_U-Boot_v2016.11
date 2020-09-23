/**************************************************************************//**
* @file     rpmc.h
* @brief    RPMC library header file
*
* SPDX-License-Identifier: Apache-2.0
* @copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
******************************************************************************/
#ifndef __RPMC_H__
#define __RPMC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define QSPI_FLASH_PORT  QSPI0

#define AF_REG_STATUS_BUSY           (1<<0)

/* Root Keys Overwrite or Root Keys length mismatch or TruncatedSig Mismatch */
#define AF_REG_STATUS_BIT1           (1<<1)

/* Signature Mismatch or Counter Address out of range or Write_Mode out of range */
#define AF_REG_STATUS_BIT2           (1<<2)

/* HMAC Key Reg Uninitialized */
#define AF_REG_STATUS_BIT3           (1<<3)

/* Monotonic_Counter_Data Mismatch */
#define AF_REG_STATUS_BIT4           (1<<4)

#define AF_REG_STATUS_FATAL_ERROR    (1<<5)
#define AF_REG_STATUS_SUCCESS        (1<<7)

void RPMC_ReadUID(struct spi_flash *flash, uint8_t *pu8Data);
uint32_t RPMC_ReadCounter(void);
uint32_t RPMC_ReadStatus(struct spi_flash *flash, uint32_t u32ChkoutFlag);
void RPMC_RequestCounter(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8HMACKey, uint8_t *pu8InTag);
uint32_t RPMC_WriteRootKey(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8RootKey);
uint32_t RPMC_UpdateHMACKey(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8RootKey, uint8_t *pu8HMAC, uint8_t *pu8HMACKey);
uint32_t RPMC_IncreaseCounter(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8HMACKey, uint8_t *pu8InTag);
int32_t RPMC_Challenge(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8HMACKey, uint8_t* pu8InTag);
void RPMC_CreateRootKey(uint8_t *pu8ID, uint32_t u32IDLen, uint8_t *pu8Rootkey);

#ifdef __cplusplus
}
#endif

#endif //__RPMC_H__
