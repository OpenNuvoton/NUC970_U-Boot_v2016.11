/**************************************************************************//**
* @file     sha256.c
* @brief    HMAC SHA256 library
*
* SPDX-License-Identifier: Apache-2.0
* @copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
******************************************************************************/

#include <common.h>
#include <errno.h>
#include <malloc.h>

#include "sha256.h"

#ifdef CONFIG_NUC970
    #include <nuc970_crypto.h>
#endif

#ifdef CONFIG_NUC980
    #include <nuc980_crypto.h>
#endif

static uint8_t g_au8HMACMsgPool[1024] __attribute__((aligned(32)));
static uint8_t *g_pu8HMACMsg;
void HMAC_SHA256(uint8_t *key, uint32_t key_len, uint8_t *text, uint32_t  text_len, uint8_t *digest)
{
    /* Non-cacheable buffer pointer */
    g_pu8HMACMsg = (uint8_t *)((uint32_t)g_au8HMACMsgPool | 0x80000000);

    /* Copy key to HMAC Msg buffer */
    memcpy((void *)&g_pu8HMACMsg[0], (void *)key, key_len);

    /* Copy text to remaining HMAC Msg buffer */
    memcpy((void *)&g_pu8HMACMsg[(key_len + 3) & 0xfffffffc], (void *)text, text_len);

    /* Enable Crypto engine clock */
    *(volatile uint32_t *)REG_HCLKEN |= 0x800000;

    /* SHA_Open(CRPT, SHA_MODE_SHA256, SHA_IN_OUT_SWAP, key_len); */
    {
        SECURE->HMAC_CTL = (SECURE_HMAC_OP_SHA256) |
                           (SECURE_HMAC_IN_TRANSFORM | SECURE_HMAC_OUT_TRANSFORM);

        if (key_len != 0UL)
        {
            SECURE->HMAC_KEY_CNT = key_len;
            SECURE->HMAC_CTL |= (1UL << 4);
        }
    }

    /* SHA_SetDMATransfer(CRPT, (uint32_t)&g_pu8HMACMsg[0], text_len + ((key_len + 3) & 0xfffffffc)); */
    {
        SECURE->HMAC_SADR = (uint32_t)&g_pu8HMACMsg[0];
        SECURE->HMAC_DMA_CNT = text_len + ((key_len + 3) & 0xfffffffc);
    }

    asm volatile("": : :"memory");

    /* Start HMAC_SHA256 calculation. */
    /* SHA_Start(CRPT, CRYPTO_DMA_ONE_SHOT); */
    {
        SECURE->HMAC_CTL &= ~(0x7UL << 5UL);
        SECURE->HMAC_CTL |= SECURE_HMAC_START | (0x5UL << 0x5UL);
    }

    /* Wait it done. */
    while (1)
    {
        volatile unsigned int int_flag;
        int_flag = (volatile unsigned int)(SECURE->IPSEC_INT_FLAG);
        if (int_flag & SECURE_INT_FLAG_HMAC_DONE)
        {
            SECURE->IPSEC_INT_FLAG = SECURE_INT_FLAG_HMAC_DONE;
            break;
        }
        else if (int_flag & SECURE_INT_FLAG_HMAC_ERR)
        {
            SECURE->IPSEC_INT_FLAG = SECURE_INT_FLAG_HMAC_ERR;
            break;
        }
    }

    while ((volatile unsigned int)(SECURE->HMAC_FLAG) & SECURE_HMAC_BUSY);

    /* void SHA_Read(CRPT, au32OutputDigest); */
    {
        uint32_t  i, reg_addr;
        uint32_t *pu32Digest = (uint32_t *)digest;
        reg_addr = (uint32_t) & (SECURE->HMAC_H0);
        for (i = 0UL; i < 8UL; i++)
        {
            pu32Digest[i] = *(volatile uint32_t *)(reg_addr);
            reg_addr += 4UL;
        }
    }

    /* Disable Crypto engine clock */
    *(volatile uint32_t *)REG_HCLKEN &= ~0x800000;

}
