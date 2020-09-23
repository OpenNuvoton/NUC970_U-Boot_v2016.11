/**************************************************************************//**
* @file     sha256.h
* @brief    SHA256 library header file
*
* SPDX-License-Identifier: Apache-2.0
* @copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
******************************************************************************/
#ifndef __SHA256_H__
#define __SHA256_H__

#ifdef __cplusplus
extern "C"
{
#endif

void HMAC_SHA256(uint8_t *key, uint32_t  key_len, uint8_t *text, uint32_t  text_len, uint8_t *digest);

#ifdef __cplusplus
}
#endif

#endif //__SHA256_H__
