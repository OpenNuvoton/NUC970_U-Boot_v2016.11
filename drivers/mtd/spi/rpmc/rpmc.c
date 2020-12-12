/**************************************************************************//**
* @file     rpmc.c
* @brief    RPMC library
*
* SPDX-License-Identifier: Apache-2.0
* @copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
******************************************************************************/

#include <common.h>
#include <spi.h>
#include <spi_flash.h>
#include <errno.h>
#include <malloc.h>

#include "sha256.h"
#include "rpmc.h"
#include "../sf_internal.h"

#define DEF_TAG_LEN          (12)
#define DEF_COUNTER_LEN      (4)
#define DEF_SIGNATURE_LEN    (32)
#define DEF_REG_READ_UID     (0x4B)
#define DEF_REG_READ_JEDEC   (0x9F)
#define DEF_RPMC_INSTRUCTION (0x9B)
#define DEF_RPMC_READ_STATUS (0x96)

enum
{
    CMDTYPE_WRITE_ROOTKEY,
    CMDTYPE_UPDATE_HMACKEY,
    CMDTYPE_INCREASE_COUNTER,
    CMDTYPE_REQUEST_COUNTER,
} RPMC_CMDTYPE;

typedef struct
{
    /* Tag data usage for increase counter */
    uint8_t m_au8Tag[DEF_TAG_LEN];

    /* Global usage for RPMC algorithm */
    /* 32-bit counter data */
    uint8_t m_au8Counter[DEF_COUNTER_LEN];

    /* Signature data usage for every instruction output */
    uint8_t m_au8Signature[DEF_SIGNATURE_LEN];
} S_RPMC_CTX;

static S_RPMC_CTX g_sRPMCCtx;

void RPMC_ReadUID(struct spi_flash *flash, uint8_t *pu8Data)
{
    int ret;
    uint8_t au8Cmd[5] = {0};
    /* 1B Cmd and 4B Dummy. */
    au8Cmd[0] = DEF_REG_READ_UID;

    ret = spi_flash_read_common(flash, &au8Cmd[0], sizeof(au8Cmd), (void *)pu8Data, 8);
    if (ret < 0)
    {
        debug("SF: fail to read UID\n");
    }
}

uint32_t RPMC_ReadCounter(void)
{
    return (((((g_sRPMCCtx.m_au8Counter[0] * 0x100) + \
               g_sRPMCCtx.m_au8Counter[1]) * 0x100) + \
             g_sRPMCCtx.m_au8Counter[2]) * 0x100) + \
           g_sRPMCCtx.m_au8Counter[3];
}

uint32_t RPMC_ReadStatus(struct spi_flash *flash, uint32_t u32ChkoutFlag)
{
    int ret;
    uint8_t au8ReceivedBuf[sizeof(S_RPMC_CTX) + 1];
    uint8_t au8Cmd[2] = {0};
    au8Cmd[0] = DEF_RPMC_READ_STATUS;

    ret = spi_flash_read_common(flash, (const u8 *)&au8Cmd[0], sizeof(au8Cmd), (void *)au8ReceivedBuf, (u32ChkoutFlag > 0) ? sizeof(au8ReceivedBuf) : 1);
    if (ret < 0)
    {
        debug("SF: fail to read status\n");
        return -1;
    }

    /* Checkout tag, counter and signature. */
    if (u32ChkoutFlag)
    {
        memcpy((void *)&g_sRPMCCtx, (void *)&au8ReceivedBuf[1], sizeof(g_sRPMCCtx));
    }

    return (uint32_t)au8ReceivedBuf[0];
}


void RPMC_RequestCounter(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8HMACKey, uint8_t *pu8InTag)
{
    int ret;

    uint8_t au8Message[16];

    au8Message[0] = DEF_RPMC_INSTRUCTION;
    au8Message[1] = CMDTYPE_REQUEST_COUNTER;
    au8Message[2] = u32CntAddr - 1;
    au8Message[3] = 0x00;

    memcpy((void *)&au8Message[4], (void *)pu8InTag, sizeof(au8Message) - 4);
    HMAC_SHA256(&pu8HMACKey[0], 32, &au8Message[0], sizeof(au8Message), &g_sRPMCCtx.m_au8Signature[0]);

    /* Send instruction, CMD type, Counter address, Reserve and 96-bit Tag */
    ret = spi_flash_write_common(flash, &au8Message[0], sizeof(au8Message), (void *)&g_sRPMCCtx.m_au8Signature[0], DEF_SIGNATURE_LEN);
    if (ret < 0)
    {
        debug("SF: fail to request counter\n");
    }
}

uint32_t RPMC_WriteRootKey(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8RootKey)
{
    int ret;
    uint32_t u32RPMCStatus;
    uint8_t au8Message[4];
    uint8_t au8SendingBuf[DEF_SIGNATURE_LEN + DEF_SIGNATURE_LEN - 4];

    au8Message[0] = DEF_RPMC_INSTRUCTION;
    au8Message[1] = CMDTYPE_WRITE_ROOTKEY;
    au8Message[2] = u32CntAddr - 1;
    au8Message[3] = 0x00;

    HMAC_SHA256(&pu8RootKey[0], 32, &au8Message[0], sizeof(au8Message), &g_sRPMCCtx.m_au8Signature[0]);

    memcpy(&au8SendingBuf[0], &pu8RootKey[0], DEF_SIGNATURE_LEN);
    memcpy(&au8SendingBuf[DEF_SIGNATURE_LEN], &g_sRPMCCtx.m_au8Signature[4], DEF_SIGNATURE_LEN - 4);

    ret = spi_flash_write_common(flash, &au8Message[0], sizeof(au8Message), (void *)&au8SendingBuf[0], sizeof(au8SendingBuf));
    if (ret < 0)
    {
        debug("SF: fail to write root key\n");
    }

    /* Wait until RPMC operation done */
    while ((u32RPMCStatus = RPMC_ReadStatus(flash, 0)) & AF_REG_STATUS_BUSY);

    return u32RPMCStatus;
}

uint32_t RPMC_UpdateHMACKey(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8RootKey, uint8_t *pu8HMAC, uint8_t *pu8HMACKey)
{
    int ret;
    uint8_t au8Message[8] = {0};
    uint32_t u32RPMCStatus;

    au8Message[0] = DEF_RPMC_INSTRUCTION;
    au8Message[1] = CMDTYPE_UPDATE_HMACKEY;
    au8Message[2] = u32CntAddr - 1;
    au8Message[3] = 0x00;
    memcpy((void *)&au8Message[4], (void *)&pu8HMAC[0], 4);

    /* Use rootkey generate HMAC key by SHA256 */
    HMAC_SHA256(&pu8RootKey[0], 32, &pu8HMAC[0], 4, &pu8HMACKey[0]);

    /* Calculate signature using SHA256 */
    HMAC_SHA256(&pu8HMACKey[0], 32, &au8Message[0], sizeof(au8Message), &g_sRPMCCtx.m_au8Signature[0]);

    ret = spi_flash_write_common(flash, &au8Message[0], sizeof(au8Message), (void *)&g_sRPMCCtx.m_au8Signature[0], DEF_SIGNATURE_LEN);
    if (ret < 0)
    {
        debug("SF: fail to update HMAC key\n");
    }

    /* Wait until RPMC operation done */
    while ((u32RPMCStatus = RPMC_ReadStatus(flash, 0))& AF_REG_STATUS_BUSY);

    return u32RPMCStatus;
}

uint32_t RPMC_IncreaseCounter(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8HMACKey, uint8_t *pu8InTag)
{
    int ret;
    uint8_t au8Message[8] = {0};
    uint32_t u32RPMCStatus;

    RPMC_RequestCounter(flash, u32CntAddr, pu8HMACKey, pu8InTag);

    /* Wait until RPMC operation done */
    while (RPMC_ReadStatus(flash, 0) & AF_REG_STATUS_BUSY);

    /* Checkout all. */
    RPMC_ReadStatus(flash, 1);

    au8Message[0] = DEF_RPMC_INSTRUCTION;
    au8Message[1] = CMDTYPE_INCREASE_COUNTER;
    au8Message[2] = u32CntAddr - 1;
    au8Message[3] = 0x00;
    memcpy((void *)&au8Message[4], &g_sRPMCCtx.m_au8Counter[0], DEF_COUNTER_LEN);

    /* Calculate signature by SHA256 */
    HMAC_SHA256(pu8HMACKey, 32, au8Message, sizeof(au8Message), &g_sRPMCCtx.m_au8Signature[0]);

    ret = spi_flash_write_common(flash, &au8Message[0], sizeof(au8Message), (void *)&g_sRPMCCtx.m_au8Signature[0], DEF_SIGNATURE_LEN);
    if (ret < 0)
    {
        debug("SF: fail to update HMAC key\n");
    }

    /* Wait until RPMC operation done */
    while ((u32RPMCStatus = RPMC_ReadStatus(flash, 0))& AF_REG_STATUS_BUSY);

    return u32RPMCStatus;
}

int32_t RPMC_Challenge(struct spi_flash *flash, uint32_t u32CntAddr, uint8_t *pu8HMACKey, uint8_t *pu8InTag)
{
    uint8_t au8Message[16] = {0};
    uint8_t au8VerifySignature[32];

    RPMC_RequestCounter(flash, u32CntAddr, pu8HMACKey, pu8InTag);

    /* Wait until RPMC operation done */
    while ((RPMC_ReadStatus(flash, 0)&AF_REG_STATUS_BUSY));

    /* Checkout all. */
    RPMC_ReadStatus(flash, 1);

    memcpy((void *)&au8Message[0],  &g_sRPMCCtx.m_au8Tag[0], DEF_TAG_LEN);
    memcpy((void *)&au8Message[12], &g_sRPMCCtx.m_au8Counter[0], DEF_COUNTER_LEN);

    /* Verification signature should as same as security output */
    HMAC_SHA256(pu8HMACKey, 32, au8Message, sizeof(au8Message), &au8VerifySignature[0]);

    /* Compare Verification signature (computed by controller) and internal signature (return from security Flash by request counter operation) */
    return memcmp(&au8VerifySignature[0], &g_sRPMCCtx.m_au8Signature[0], sizeof(au8VerifySignature));
}

/**
  * @brief A create Rootkey reference.
  * @param[in] spi flash UID.
  * @param[in] UID length.
  * @param[out] 32-byte outputted Rootkey buffer.
  * @return None.
  */
void RPMC_CreateRootKey(uint8_t *pu8ID, uint32_t u32IDLen, uint8_t *pu8Rootkey)
{
    uint8_t au8RootKeyTag[32] = {0};

    int copied_len = strlen(CONFIG_RPMC_ROOTKEY_TAG);

    if (!copied_len)
        printf("Rootkey Tag string is empty!!\n");
    else
        memcpy(&au8RootKeyTag[0], CONFIG_RPMC_ROOTKEY_TAG, (copied_len > sizeof(au8RootKeyTag)) ? sizeof(au8RootKeyTag) : copied_len);

    HMAC_SHA256(pu8ID, u32IDLen, &au8RootKeyTag[0], sizeof(au8RootKeyTag), pu8Rootkey);
}
