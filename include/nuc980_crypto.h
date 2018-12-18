/*************************************************************************
 * (C) Copyright 2018
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * All rights reserved
 *
 * FILENAME
 *     crypto.h
 *
 * REMARK
 *     None
 **************************************************************************/

#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#define REG_HCLKEN	0xB0000210
#define CRPT_BA 	0xB001C000

typedef unsigned int UINT32;

typedef struct {
	UINT32 IPSEC_INT_EN;		/*!< Offset: 0x0000   IPsec interrupt enable                             */
	volatile UINT32 IPSEC_INT_FLAG;	/*!< Offset: 0x0004   IPsec interrupt flag                               */
	UINT32 PRNG_CTL;		/*!< Offset: 0x0008   PRNG  control register                             */
	UINT32 PRNG_SEED;		/*!< Offset: 0x000C   Store  random seed                                 */
	UINT32 PRNG_key0;		/*!< Offset: 0x0010   PRNG  generated  Key0                              */
	UINT32 PRNG_key1;		/*!< Offset: 0x0014   PRNG  generated  Key1                              */
	UINT32 PRNG_key2;		/*!< Offset: 0x0018   PRNG  generated  Key2                              */
	UINT32 PRNG_key3;		/*!< Offset: 0x001C   PRNG  generated  Key3                              */
	UINT32 PRNG_key4;		/*!< Offset: 0x0020   PRNG  generated  Key4                              */
	UINT32 PRNG_key5;		/*!< Offset: 0x0024   PRNG  generated  Key5                              */
	UINT32 PRNG_key6;		/*!< Offset: 0x0028   PRNG  generated  Key6                              */
	UINT32 PRNG_key7;		/*!< Offset: 0x002C   PRNG  generated  Key7                              */
	UINT32 RESERVED1[8];		/*!< Offset: 0x0030 ~ 0x004C   Reserved                                  */
	UINT32 AES_FEEDBACK_0;		/*!< Offset: 0x0050   AES engine 0 Output feedback data after cryptographic operation */
	UINT32 AES_FEEDBACK_1;		/*!< Offset: 0x0054   AES engine 1 Output feedback data after cryptographic operation */
	UINT32 AES_FEEDBACK_2;		/*!< Offset: 0x0058   AES engine 2 Output feedback data after cryptographic operation */
	UINT32 AES_FEEDBACK_3;		/*!< Offset: 0x005C   AES engine 3 Output feedback data after cryptographic operation */
	UINT32 TDES_FEEDBACK_H;		/*!< Offset: 0x0060   Triple DES engine Output higher feedback data after cryptographic operation */
	UINT32 TDES_FEEDBACK_L;		/*!< Offset: 0x0064   Triple DES engine Output lower feedback data after cryptographic operation */
	UINT32 RESERVED2[38];		/*!< Offset: 0x0068 ~ 0x00FC   Reserved                                  */
	UINT32 AES_CTL;			/*!< Offset: 0x0100   AES engine setting                                 */
	UINT32 AES_FLAG;		/*!< Offset: 0x0104   AES engine flag                                    */
	UINT32 AES_DATAIN;		/*!< Offset: 0x0108   AES engine data input port                         */
	UINT32 AES_DATAOUT;		/*!< Offset: 0x010C   AES engine data output port                        */
	UINT32 AES_KEY_0;		/*!< Offset: 0x0110   AES Key Word 0 Register                            */
	UINT32 AES_KEY_1;		/*!< Offset: 0x0114   AES Key Word 1 Register                            */
	UINT32 AES_KEY_2;		/*!< Offset: 0x0118   AES Key Word 2 Register                            */
	UINT32 AES_KEY_3;		/*!< Offset: 0x011C   AES Key Word 3 Register                            */
	UINT32 AES_KEY_4;		/*!< Offset: 0x0120   AES Key Word 4 Register                            */
	UINT32 AES_KEY_5;		/*!< Offset: 0x0124   AES Key Word 5 Register                            */
	UINT32 AES_KEY_6;		/*!< Offset: 0x0128   AES Key Word 6 Register                            */
	UINT32 AES_KEY_7;		/*!< Offset: 0x012C   AES Key Word 7 Register                            */
	UINT32 AES_IV0;			/*!< Offset: 0x0130   AES Initial Vector Word 0 Register                 */
	UINT32 AES_IV1;			/*!< Offset: 0x0134   AES Initial Vector Word 1 Register                 */
	UINT32 AES_IV2;			/*!< Offset: 0x0138   AES Initial Vector Word 2 Register                 */
	UINT32 AES_IV3;			/*!< Offset: 0x013C   AES Initial Vector Word 3 Register                 */
	UINT32 AES_SADR;		/*!< Offset: 0x0140   AES Source Address Register                        */
	UINT32 AES_DADR;		/*!< Offset: 0x0144   AES Destination Address Register                   */
	UINT32 AES_CNT;			/*!< Offset: 0x0148   AES Byte Count Register                            */
	UINT32 AES1_KEY_0;		/*!< Offset: 0x014C   AES1 Key Word 0 Register                           */
	UINT32 AES1_KEY_1;		/*!< Offset: 0x0150   AES1 Key Word 1 Register                           */
	UINT32 AES1_KEY_2;		/*!< Offset: 0x0154   AES1 Key Word 2 Register                           */
	UINT32 AES1_KEY_3;		/*!< Offset: 0x0158   AES1 Key Word 3 Register                           */
	UINT32 AES1_KEY_4;		/*!< Offset: 0x015C   AES1 Key Word 4 Register                           */
	UINT32 AES1_KEY_5;		/*!< Offset: 0x0160   AES1 Key Word 5 Register                           */
	UINT32 AES1_KEY_6;		/*!< Offset: 0x0164   AES1 Key Word 6 Register                           */
	UINT32 AES1_KEY_7;		/*!< Offset: 0x0168   AES1 Key Word 7 Register                           */
	UINT32 AES1_IV0;		/*!< Offset: 0x016C   AES1 Initial Vector Word 0 Register                */
	UINT32 AES1_IV1;		/*!< Offset: 0x0170   AES1 Initial Vector Word 1 Register                */
	UINT32 AES1_IV2;		/*!< Offset: 0x0174   AES1 Initial Vector Word 2 Register                */
	UINT32 AES1_IV3;		/*!< Offset: 0x0178   AES1 Initial Vector Word 3 Register                */
	UINT32 AES1_SADR;		/*!< Offset: 0x017C   AES1 Source Address Register                       */
	UINT32 AES1_DADR;		/*!< Offset: 0x0180   AES1 Destination Address Register                  */
	UINT32 AES1_CNT;		/*!< Offset: 0x0184   AES1 Byte Count Register                           */
	UINT32 AES2_KEY_0;		/*!< Offset: 0x0188   AES2 Key Word 0 Register                           */
	UINT32 AES2_KEY_1;		/*!< Offset: 0x018C   AES2 Key Word 1 Register                           */
	UINT32 AES2_KEY_2;		/*!< Offset: 0x0190   AES2 Key Word 2 Register                           */
	UINT32 AES2_KEY_3;		/*!< Offset: 0x0194   AES2 Key Word 3 Register                           */
	UINT32 AES2_KEY_4;		/*!< Offset: 0x0198   AES2 Key Word 4 Register                           */
	UINT32 AES2_KEY_5;		/*!< Offset: 0x019C   AES2 Key Word 5 Register                           */
	UINT32 AES2_KEY_6;		/*!< Offset: 0x01A0   AES2 Key Word 6 Register                           */
	UINT32 AES2_KEY_7;		/*!< Offset: 0x01A4   AES2 Key Word 7 Register                           */
	UINT32 AES2_IV0;		/*!< Offset: 0x01A8   AES2 Initial Vector Word 0 Register                */
	UINT32 AES2_IV1;		/*!< Offset: 0x01AC   AES2 Initial Vector Word 1 Register                */
	UINT32 AES2_IV2;		/*!< Offset: 0x01B0   AES2 Initial Vector Word 2 Register                */
	UINT32 AES2_IV3;		/*!< Offset: 0x01B4   AES2 Initial Vector Word 3 Register                */
	UINT32 AES2_SADR;		/*!< Offset: 0x01B8   AES2 Source Address Register                       */
	UINT32 AES2_DADR;		/*!< Offset: 0x01BC   AES2 Destination Address Register                  */
	UINT32 AES2_CNT;		/*!< Offset: 0x01C0   AES2 Byte Count Register                           */
	UINT32 AES3_KEY_0;		/*!< Offset: 0x01C4   AES3 Key Word 0 Register                           */
	UINT32 AES3_KEY_1;		/*!< Offset: 0x01C8   AES3 Key Word 1 Register                           */
	UINT32 AES3_KEY_2;		/*!< Offset: 0x01CC   AES3 Key Word 2 Register                           */
	UINT32 AES3_KEY_3;		/*!< Offset: 0x01D0   AES3 Key Word 3 Register                           */
	UINT32 AES3_KEY_4;		/*!< Offset: 0x01D4   AES3 Key Word 4 Register                           */
	UINT32 AES3_KEY_5;		/*!< Offset: 0x01D8   AES3 Key Word 5 Register                           */
	UINT32 AES3_KEY_6;		/*!< Offset: 0x01DC   AES3 Key Word 6 Register                           */
	UINT32 AES3_KEY_7;		/*!< Offset: 0x01E0   AES3 Key Word 7 Register                           */
	UINT32 AES3_IV0;		/*!< Offset: 0x01E4   AES3 Initial Vector Word 0 Register                */
	UINT32 AES3_IV1;		/*!< Offset: 0x01E8   AES3 Initial Vector Word 1 Register                */
	UINT32 AES3_IV2;		/*!< Offset: 0x01EC   AES3 Initial Vector Word 2 Register                */
	UINT32 AES3_IV3;		/*!< Offset: 0x01F0   AES3 Initial Vector Word 3 Register                */
	UINT32 AES3_SADR;		/*!< Offset: 0x01F4   AES3 Source Address Register                       */
	UINT32 AES3_DADR;		/*!< Offset: 0x01F8   AES3 Destination Address Register                  */
	UINT32 AES3_CNT;		/*!< Offset: 0x01FC   AES3 Byte Count Register                           */
	UINT32 TDES_CTL;		/*!< Offset: 0x0200   3DES engine parameter  setting                     */
	UINT32 TDES_FLAG;		/*!< Offset: 0x0204   3DES flag setting                                  */
	UINT32 TDES_KEY1H;		/*!< Offset: 0x0208   DES/3DES Key 1 Higher Word Register                */
	UINT32 TDES_KEY1L;		/*!< Offset: 0x020C   DES/3DES Key 1 Lower Word Register                 */
	UINT32 TDES_KEY2H;		/*!< Offset: 0x0210   3DES Key 2 Higher Word Register                    */
	UINT32 TDES_KEY2L;		/*!< Offset: 0x0214   3DES Key 2 Lower Word Register                     */
	UINT32 TDES_KEY3H;		/*!< Offset: 0x0218   3DES Key 3 Higher Word Register                    */
	UINT32 TDES_KEY3L;		/*!< Offset: 0x021C   3DES Key 3 Lower Word Register                     */
	UINT32 TDES_IVH;		/*!< Offset: 0x0220   3DES Initial Vector Higher Word Register           */
	UINT32 TDES_IVL;		/*!< Offset: 0x0224   3DES Initial Vector Lower Word Register            */
	UINT32 TDES_SADR;		/*!< Offset: 0x0228   DES/3DES Source Address Register                   */
	UINT32 TDES_DADR;		/*!< Offset: 0x022C   DES/3DES Destination Address Register              */
	UINT32 TDES_CNT;		/*!< Offset: 0x0230   DES/3DES Block Count Register                      */
	UINT32 TDES_DATAIN;		/*!< Offset: 0x0234   DES/3DES engine Input Word data Register           */
	UINT32 TDES_DATAOUT;		/*!< Offset: 0x0238   DES/3DES engine Output Word data Register          */
	UINT32 RESERVED3[3];		/*!< Offset: 0x023C ~ 0x0244   Reserved                                  */
	UINT32 TDES1_KEY1H;		/*!< Offset: 0x0248   DES/3DES Key 1 Higher Word Register                */
	UINT32 TDES1_KEY1L;		/*!< Offset: 0x024C   DES/3DES Key 1 Lower Word Register                 */
	UINT32 TDES1_KEY2H;		/*!< Offset: 0x0250   3DES Key 2 Higher Word Register                    */
	UINT32 TDES1_KEY2L;		/*!< Offset: 0x0254   3DES Key 2 Lower Word Register                     */
	UINT32 TDES1_KEY3H;		/*!< Offset: 0x0258   3DES Key 3 Higher Word Register                    */
	UINT32 TDES1_KEY3L;		/*!< Offset: 0x025C   3DES Key 3 Lower Word Register                     */
	UINT32 TDES1_IVH;		/*!< Offset: 0x0260   3DES Initial Vector Higher Word Register           */
	UINT32 TDES1_IVL;		/*!< Offset: 0x0264   3DES Initial Vector Lower Word Register            */
	UINT32 TDES1_SADR;		/*!< Offset: 0x0268   DES/3DES Source Address Register                   */
	UINT32 TDES1_DADR;		/*!< Offset: 0x026C   DES/3DES Destination Address Register              */
	UINT32 TDES1_CNT;		/*!< Offset: 0x0270   DES/3DES Block Count Register                      */
	UINT32 RESERVED4[5];		/*!< Offset: 0x0274 ~ 0x0284   Reserved                                  */
	UINT32 TDES2_KEY1H;		/*!< Offset: 0x0288   DES/3DES Key 1 Higher Word Register                */
	UINT32 TDES2_KEY1L;		/*!< Offset: 0x028C   DES/3DES Key 1 Lower Word Register                 */
	UINT32 TDES2_KEY2H;		/*!< Offset: 0x0290   3DES Key 2 Higher Word Register                    */
	UINT32 TDES2_KEY2L;		/*!< Offset: 0x0294   3DES Key 2 Lower Word Register                     */
	UINT32 TDES2_KEY3H;		/*!< Offset: 0x0298   3DES Key 3 Higher Word Register                    */
	UINT32 TDES2_KEY3L;		/*!< Offset: 0x029C   3DES Key 3 Lower Word Register                     */
	UINT32 TDES2_IVH;		/*!< Offset: 0x02A0   3DES Initial Vector Higher Word Register           */
	UINT32 TDES2_IVL;		/*!< Offset: 0x02A4   3DES Initial Vector Lower Word Register            */
	UINT32 TDES2_SADR;		/*!< Offset: 0x02A8   DES/3DES Source Address Register                   */
	UINT32 TDES2_DADR;		/*!< Offset: 0x02AC   DES/3DES Destination Address Register              */
	UINT32 TDES2_CNT;		/*!< Offset: 0x02B0   DES/3DES Block Count Register                      */
	UINT32 RESERVED5[5];		/*!< Offset: 0x02B4 ~ 0x02C4   Reserved                                  */
	UINT32 TDES3_KEY1H;		/*!< Offset: 0x02C8   DES/3DES Key 1 Higher Word Register                */
	UINT32 TDES3_KEY1L;		/*!< Offset: 0x02CC   DES/3DES Key 1 Lower Word Register                 */
	UINT32 TDES3_KEY2H;		/*!< Offset: 0x02D0   3DES Key 2 Higher Word Register                    */
	UINT32 TDES3_KEY2L;		/*!< Offset: 0x02D4   3DES Key 2 Lower Word Register                     */
	UINT32 TDES3_KEY3H;		/*!< Offset: 0x02D8   3DES Key 3 Higher Word Register                    */
	UINT32 TDES3_KEY3L;		/*!< Offset: 0x02DC   3DES Key 3 Lower Word Register                     */
	UINT32 TDES3_IVH;		/*!< Offset: 0x02E0   3DES Initial Vector Higher Word Register           */
	UINT32 TDES3_IVL;		/*!< Offset: 0x02E4   3DES Initial Vector Lower Word Register            */
	UINT32 TDES3_SADR;		/*!< Offset: 0x02E8   DES/3DES Source Address Register                   */
	UINT32 TDES3_DADR;		/*!< Offset: 0x02EC   DES/3DES Destination Address Register              */
	UINT32 TDES3_CNT;		/*!< Offset: 0x02F0   DES/3DES Block Count Register                      */
	UINT32 RESERVED6[3];		/*!< Offset: 0x02F4 ~ 0x02FC   Reserved                                  */
	UINT32 HMAC_CTL;		/*!< Offset: 0x0300   HMAC/SHA engine parameter setting                  */
	volatile UINT32 HMAC_FLAG;	/*!< Offset: 0x0304   HMAC/SHA status flag                               */
	volatile UINT32 HMAC_H0;	/*!< Offset: 0x0308   HMAC/SHA digest message 0                          */
	UINT32 HMAC_H1;		        /*!< Offset: 0x030C   HMAC/SHA digest message 1                          */
	UINT32 HMAC_H2;		        /*!< Offset: 0x0310   HMAC/SHA digest message 2                          */
	UINT32 HMAC_H3;		        /*!< Offset: 0x0314   HMAC/SHA digest message 3                          */
	UINT32 HMAC_H4;		        /*!< Offset: 0x0318   HMAC/SHA digest message 4                          */
	UINT32 HMAC_H5;		        /*!< Offset: 0x031C   HMAC/SHA digest message 5                          */
	UINT32 HMAC_H6;		        /*!< Offset: 0x0320   HMAC/SHA digest message 6                          */
	UINT32 HMAC_H7;		        /*!< Offset: 0x0324   HMAC/SHA digest message 7                          */
	UINT32 HMAC_H8;		        /*!< Offset: 0x0328   HMAC/SHA digest message 8                          */
	UINT32 HMAC_H9;		        /*!< Offset: 0x032C   HMAC/SHA digest message 9                          */
	UINT32 HMAC_H10;	        /*!< Offset: 0x0330   HMAC/SHA digest message 10                         */
	UINT32 HMAC_H11;	        /*!< Offset: 0x0334   HMAC/SHA digest message 11                         */
	UINT32 HMAC_H12;	        /*!< Offset: 0x0338   HMAC/SHA digest message 12                         */
	UINT32 HMAC_H13;	        /*!< Offset: 0x033C   HMAC/SHA digest message 13                         */
	UINT32 HMAC_H14;	        /*!< Offset: 0x0340   HMAC/SHA digest message 14                         */
	UINT32 HMAC_H15;	        /*!< Offset: 0x0344   HMAC/SHA digest message 15                         */
	UINT32 HMAC_KEY_CNT;		/*!< Offset: 0x0348   HMAC/SHA key byte length                           */
	UINT32 HMAC_SADR;		/*!< Offset: 0x034C   HMAC DMA Source Address Register                   */
	UINT32 HMAC_DMA_CNT;		/*!< Offset: 0x0350   HMAC DMA count                                     */
	UINT32 HMAC_DATAIN;		/*!< Offset: 0x0354   HMAC software write to engine port                 */
} 	SECURE_TypeDef;


typedef struct {
	UINT32 key0;
	UINT32 key1;
	UINT32 key2;
	UINT32 key3;
	UINT32 key4;
	UINT32 key5;
	UINT32 key6;
	UINT32 key7;
	UINT32 iv0;
	UINT32 iv1;
	UINT32 iv2;
	UINT32 iv3;
	UINT32 src_addr;
	UINT32 dst_addr;
	UINT32 count;
}	AES_TypeDef;


typedef struct {
	UINT32 key1H;
	UINT32 key1L;
	UINT32 key2H;
	UINT32 key2L;
	UINT32 key3H;
	UINT32 key3L;
	UINT32 ivH;
	UINT32 ivL;
	UINT32 src_addr;
	UINT32 dst_addr;
	UINT32 count;
}	TDES_TypeDef;


#define SECURE				((SECURE_TypeDef *) CRPT_BA)
#define AES0				((AES_TypeDef *)(CRPT_BA+0x110))
#define AES1				((AES_TypeDef *)(CRPT_BA+0x14C))
#define AES2				((AES_TypeDef *)(CRPT_BA+0x188))
#define AES3				((AES_TypeDef *)(CRPT_BA+0x1C4))
#define TDES0				((AES_TypeDef *)(CRPT_BA+0x208))
#define TDES1				((AES_TypeDef *)(CRPT_BA+0x248))
#define TDES2				((AES_TypeDef *)(CRPT_BA+0x288))
#define TDES3				((AES_TypeDef *)(CRPT_BA+0x2C8))


/********************* Bit definition of IPSEC_INT_EN  ************************/
#define SECURE_INT_EN_AES		((UINT32)0x00000001)			/*!<AES DMA finish interrupt flag enable */
#define SECURE_INT_EN_AES_ERR		((UINT32)0x00000002)			/*!<AES error flag enable */
#define SECURE_INT_EN_TDES		((UINT32)0x00000100)			/*!<TDES interrupt source enable */
#define SECURE_INT_EN_TDES_ERR		((UINT32)0x00000200)			/*!<TDES error flag enable */
#define SECURE_INT_EN_PRNG		((UINT32)0x00010000)			/*!<PRNG finish interrupt enable */
#define SECURE_INT_EN_HMAC		((UINT32)0x01000000)			/*!<HMAC interrupt enable */
#define SECURE_INT_EN_HMAC_ERR		((UINT32)0x02000000)			/*!<HMAC error interrupt enable */

/********************* Bit definition of IPSEC_INT_FLAG *******************/
#define SECURE_INT_FLAG_AES_DONE	((UINT32)0x00000001)			/*!<AES finish interrupt flag */
#define SECURE_INT_FLAG_AES_ERR		((UINT32)0x00000002)			/*!<AES error flag */
#define SECURE_INT_FLAG_TDES_DONE	((UINT32)0x00000100)			/*!<TDES finish interrupt flag */
#define SECURE_INT_FLAG_TDES_ERR	((UINT32)0x00000200)			/*!<TDES error flag */
#define SECURE_INT_FLAG_PRNG_DONE	((UINT32)0x00010000)			/*!<PRNG finish interrupt flag */
#define SECURE_INT_FLAG_HMAC_DONE	((UINT32)0x01000000)			/*!<HMAC finish interrupt flag */
#define SECURE_INT_FLAG_HMAC_ERR	((UINT32)0x02000000)			/*!<HMAC error flag */

/********************* Bit definition of PRNG_CTL *******************/
#define SECURE_PRNG_CTL_START		((UINT32)0x00000001)			/*!<Write ¡§1¡¨ start PRNG engine generate new KEY and store KEYx register. */
#define SECURE_PRNG_CTL_SEED_RELOAD	((UINT32)0x00000002)			/*!1: reload new seed; 0: generate key base original seed. */
#define SECURE_PRNG_CTL_KEY_SIZE_MASK	((UINT32)0x0000000C)			/*!<PRNG KEY SIZE bit mask */
#define SECURE_PRNG_CTL_KEY_SIZE_64	((UINT32)0x00000000)			/*!<PRNG 64 bits key */
#define SECURE_PRNG_CTL_KEY_SIZE_128	((UINT32)0x00000004)			/*!<PRNG 128 bits key */
#define SECURE_PRNG_CTL_KEY_SIZE_192	((UINT32)0x00000008)			/*!<PRNG 192 bits key */
#define SECURE_PRNG_CTL_KEY_SIZE_256	((UINT32)0x0000000C)			/*!<PRNG 256 bits key */
#define SECURE_PRNG_CTL_BUSY		((UINT32)0x00000100)			/*!<read only, 1:  indicate the PRNG engine under generating KEY */


/********************* Bit definition of AES_CTL *******************/
#define SECURE_AES_CTL_START		((UINT32)0x00000001)			/*!<Write ¡§1¡¨, AES engine starting. AES_BUSY flag will be set. */
#define SECURE_AES_CTL_STOP		((UINT32)0x00000002)			/*!<Write ¡§1¡¨ ,  stop  AES engine instantly. */
#define SECURE_AES_CTL_KEY_SIZE_MASK	((UINT32)0x0000000C)			/*!<AES KEY_SIZE bit mask */
#define SECURE_AES_CTL_KEY_SIZE_128	((UINT32)0x00000000)			/*!<AES 128 bits key */
#define SECURE_AES_CTL_KEY_SIZE_192	((UINT32)0x00000004)			/*!<AES 192 bits key */
#define SECURE_AES_CTL_KEY_SIZE_256	((UINT32)0x00000008)			/*!<AES 256 bits key */
#define SECURE_AES_CTL_EXT_KEY		((UINT32)0x00000010)			/*!<1: AES KEY use extend key. 0:  AES KEY source form  AES_KEYx registers. */
#define SECURE_AES_CTL_LAST		((UINT32)0x00000020)			/*!<Index current operation is  last one. */
#define SECURE_AES_CTL_DMA_CASCADE	((UINT32)0x00000040)			/*!<Cascade AES encrypt/decrypt result */
#define SECURE_AES_CTL_DMA_EN		((UINT32)0x00000080)			/*!<enable AES DMA engine */
#define SECURE_AES_CTL_OP_MASK 		((UINT32)0x0000FF00)			/*!<AES engine operation mode bit mask */
#define SECURE_AES_CTL_OP_ECB		((UINT32)0x00000000)			/*!<AES ECB mode */
#define SECURE_AES_CTL_OP_CBC		((UINT32)0x00000100)			/*!<AES CBC mode */
#define SECURE_AES_CTL_OP_CFB		((UINT32)0x00000200)			/*!<AES CFB mode */
#define SECURE_AES_CTL_OP_OFB		((UINT32)0x00000300)			/*!<AES OFB mode */
#define SECURE_AES_CTL_OP_CTR		((UINT32)0x00000400)			/*!<AES CTR mode */
#define SECURE_AES_CTL_OP_CBS_CS1	((UINT32)0x00001000)			/*!<AES CBS CS1 mode */
#define SECURE_AES_CTL_OP_CBS_CS2	((UINT32)0x00001100)			/*!<AES CBS CS2 mode */
#define SECURE_AES_CTL_OP_CBS_CS3	((UINT32)0x00001200)			/*!<AES CBS CS3 mode */
#define SECURE_AES_CTL_OP_CMAC		((UINT32)0x00002000)			/*!<AES CMAC mode */
#define SECURE_AES_CTL_OP_CCM		((UINT32)0x00003000)			/*!<AES CCM mode */
#define SECURE_AES_CTL_OP_GCM		((UINT32)0x00004000)			/*!<AES GCM mode */
#define SECURE_AES_CTL_OP_XTS		((UINT32)0x00005000)			/*!<AES XTS mode */
#define SECURE_AES_CTL_OP_XCBC3		((UINT32)0x00006000)			/*!<AES XCBC3 mode */
#define SECURE_AES_CTL_OP_XCBC1		((UINT32)0x00006100)			/*!<AES XCBC1 mode */
#define SECURE_AES_CTL_ENCRYPT		((UINT32)0x00010000)			/*!<AES engine execute encryption */
#define SECURE_AES_CTL_DECRYPT		((UINT32)0x00000000)			/*!<AES engine execute decryption */
#define SECURE_AES_CTL_OUT_TRANS	((UINT32)0x00400000)			/*!<AES engine output data transform */
#define SECURE_AES_CTL_IN_TRANS		((UINT32)0x00800000)			/*!<AES engine input data transform */
#define SECURE_AES_CTL_CHANNEL_MASK	((UINT32)0x03000000)			/*!<AES working channel bit mask */
#define SECURE_AES_CTL_CHANNEL_0	((UINT32)0x00000000)			/*!<AES Current  control register  setting on channel 0 */
#define SECURE_AES_CTL_CHANNEL_1	((UINT32)0x01000000)			/*!<AES Current  control register  setting on channel 0 */
#define SECURE_AES_CTL_CHANNEL_2	((UINT32)0x02000000)			/*!<AES Current  control register  setting on channel 0 */
#define SECURE_AES_CTL_CHANNEL_3	((UINT32)0x03000000)			/*!<AES Current  control register  setting on channel 0 */
#define SECURE_AES_CTL_KEY_UNPROTECT_MASK	((UINT32)0xFC000000)		/*!<AES unprotect key bit mask */
#define SECURE_AES_CTL_KEY_UNPROTECT	((UINT32)0x58000000)			/*!<Set AES unprotect key */
#define SECURE_AES_CTL_KEY_PROTECT	((UINT32)0x80000000)			/*!<AES key protect */

/********************* Bit definition of AES_FLAG *******************/
#define SECURE_AES_FLAG_BUSY		((UINT32)0x00000001)			/*!<AES engine under archiving */
#define SECURE_AES_FLAG_IN_BUF_EMPTY	((UINT32)0x00000100)			/*!<AES input buffer empty */
#define SECURE_AES_FLAG_IN_BUF_FULL	((UINT32)0x00000200)			/*!<AES input buffer full */
#define SECURE_AES_FLAG_IN_BUF_ERR	((UINT32)0x00000400)			/*!<ERROR during feed AES engine data. */
#define SECURE_AES_FLAG_CNT_ERR		((UINT32)0x00001000)			/*!<AES_CNT setting error */
#define SECURE_AES_FLAG_OUT_BUF_EMPTY	((UINT32)0x00010000)			/*!<AES output buffer empty */
#define SECURE_AES_FLAG_OUT_BUF_FULL	((UINT32)0x00020000)			/*!<AES output buffer full */
#define SECURE_AES_FLAG_OUT_BUF_ERR	((UINT32)0x00040000)			/*!<ERROR during get AES engine result */
#define SECURE_AES_FLAG_BUS_ERR		((UINT32)0x00100000)			/*!<AES bus error */

/********************* Bit definition of TDES_CTL *******************/
#define SECURE_TDES_CTL_START		((UINT32)0x00000001)			/*!<Write ¡§1¡¨, TDES engine starting. AES_BUSY flag will be set. */
#define SECURE_TDES_CTL_STOP		((UINT32)0x00000002)			/*!<Write ¡§1¡¨ , stop TDES engine instantly. */
#define SECURE_TDES_CTL_3_MODE 		((UINT32)0x00000004)			/*!<Tripple DES mode */
#define SECURE_TDES_CTL_3KEYS 		((UINT32)0x00000008)			/*!<enable triple KEY in TDES engine */
#define SECURE_TDES_CTL_EXT_KEY		((UINT32)0x00000010)			/*!<TDES KEY use extend key */
#define SECURE_TDES_CTL_LAST_START	((UINT32)0x00000020)			/*!<TDES last DMA cascade round */
#define SECURE_TDES_CTL_CASCADE		((UINT32)0x00000040)			/*!<TDES DMA cascade mode */
#define SECURE_TDES_CTL_DMA_EN		((UINT32)0x00000080)			/*!<enable TDES_DMA engine */
#define SECURE_TDES_CTL_OP_MASK		((UINT32)0x00000700)			/*!<TDES engine operation mode bit mask */
#define SECURE_TDES_CTL_OP_ECB		((UINT32)0x00000000)			/*!<ECB mode */
#define SECURE_TDES_CTL_OP_CBC		((UINT32)0x00000100)			/*!<CBC mode */
#define SECURE_TDES_CTL_OP_CFB		((UINT32)0x00000200)			/*!<CFB mode */
#define SECURE_TDES_CTL_OP_OFB		((UINT32)0x00000300)			/*!<OFB mode */
#define SECURE_TDES_CTL_OP_CTR		((UINT32)0x00000400)			/*!<CTR mode */
#define SECURE_TDES_CTL_ENCRYPT		((UINT32)0x00010000)			/*!<TDES engine execute encryption */
#define SECURE_TDES_CTL_DECRYPT		((UINT32)0x00000000)			/*!<TDES engine execute decryption */
#define SECURE_TDES_CTL_BLOCK_FORMAT ((UINT32)0x00200000)			/*!<TDES engine Swap High/Low word */
#define SECURE_TDES_CTL_OUT_TRANS	((UINT32)0x00400000)			/*!<TDES engine output data transform */
#define SECURE_TDES_CTL_IN_TRANS	((UINT32)0x00800000)			/*!<TDES engine input data transform */
#define SECURE_TDES_CTL_CHANNEL_MASK	((UINT32)0x03000000)			/*!<TDES working channel bit mask */
#define SECURE_TDES_CTL_CHANNEL_0	((UINT32)0x00000000)			/*!<TDES Current  control register  setting on channel 0 */
#define SECURE_TDES_CTL_CHANNEL_1	((UINT32)0x01000000)			/*!<TDES Current  control register  setting on channel 0 */
#define SECURE_TDES_CTL_CHANNEL_2	((UINT32)0x02000000)			/*!<TDES Current  control register  setting on channel 0 */
#define SECURE_TDES_CTL_CHANNEL_3	((UINT32)0x03000000)			/*!<TDES Current  control register  setting on channel 0 */
#define SECURE_TDES_CTL_KEY_UNPROTECT_MASK	((UINT32)0xFC000000)		/*!<TDES unprotect key bit mask */
#define SECURE_TDES_CTL_KEY_UNPROTECT	((UINT32)0x58000000)			/*!<Set TDES unprotect key */
#define SECURE_TDES_CTL_KEY_PROTECT	((UINT32)0x80000000)			/*!<TDES key protect */

/********************* Bit definition of TDES_FLAG *******************/
#define SECURE_TDES_FLAG_BUSY		((UINT32)0x00000001)			/*!<TDES engine under archiving */
#define SECURE_TDES_FLAG_IN_BUF_EMPTY	((UINT32)0x00000100)			/*!<TDES input buffer empty */
#define SECURE_TDES_FLAG_IN_BUF_FULL	((UINT32)0x00000200)			/*!<TDES input buffer full */
#define SECURE_TDES_FLAG_IN_BUF_ERR	((UINT32)0x00000400)			/*!<ERROR during feed TDES engine data. */
#define SECURE_TDES_FLAG_OUT_BUF_EMPTY	((UINT32)0x00010000)			/*!<TDES output buffer empty */
#define SECURE_TDES_FLAG_OUT_BUF_FULL	((UINT32)0x00020000)			/*!<TDES output buffer full */
#define SECURE_TDES_FLAG_OUT_BUF_ERR	((UINT32)0x00040000)			/*!<ERROR during get TDES engine result */
#define SECURE_TDES_FLAG_BUS_ERR	((UINT32)0x00100000)			/*!<TDES bus error */

/********************* Bit definition of HMAC_CTL ********************/
#define SECURE_HMAC_START		((UINT32)0x00000001)			/*!<HMAC engine starting */
#define SECURE_HMAC_STOP		((UINT32)0x00000002)			/*!<HMAC engine stop */
#define SECURE_HMAC_EN			((UINT32)0x00000010)			/*!<execute HMAC function */
#define SECURE_HMAC_LAST		((UINT32)0x00000020)			/*!<last DMA cascade round */
#define SECURE_HMAC_DMA_EN		((UINT32)0x00000080)			/*!<enable HMAC_DMA engine */
#define SECURE_HMAC_OP_MASK		((UINT32)0x00000700)			/*!<HMAC engine operation modes mask */
#define SECURE_HMAC_OP_SHA1		((UINT32)0x00000000)			/*!<SHA1 */
#define SECURE_HMAC_OP_SHA224		((UINT32)0x00000500)			/*!<SHA224 */
#define SECURE_HMAC_OP_SHA256		((UINT32)0x00000400)			/*!<SHA256 */
#define SECURE_HMAC_OP_SHA384		((UINT32)0x00000700)			/*!<SHA384 */
#define SECURE_HMAC_OP_SHA512		((UINT32)0x00000600)			/*!<SHA512 */
#define SECURE_HMAC_OUT_TRANSFORM	((UINT32)0x00400000)			/*!<HMAC engine output data transform */
#define SECURE_HMAC_IN_TRANSFORM	((UINT32)0x00800000)			/*!<HMAC engine input data transform */

/********************* Bit definition of HMAC_FLAG *******************/
#define SECURE_HMAC_BUSY		((UINT32)0x00000001)			/*!<HMAC engine busy */
#define SECURE_HMAC_DMA_BUSY		((UINT32)0x00000002)			/*!<HMAC engine is under active */
#define SECURE_HMAC_DMA_ERR		((UINT32)0x00000100)			/*!<HMAC DMA error */
#define SECURE_HMAC_DIN_REQ		((UINT32)0x00010000)			/*!<HMAC_SOFTWARE mode Data input  request */

#endif  /* _CRYPTO_H_ */
