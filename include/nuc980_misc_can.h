

#ifndef _NUC980CAN_H_
#define _NUC980CAN_H_


typedef struct
{
    volatile unsigned int CREQ;         /*!< [0x0020] IFn Command Request */
    volatile unsigned int CMASK;        /*!< [0x0024] IFn Command Mask Register */
    volatile unsigned int MASK1;        /*!< [0x0028] IFn Mask 1 Register       */
    volatile unsigned int MASK2;        /*!< [0x002c] IFn Mask 2 Register       */
    volatile unsigned int ARB1;         /*!< [0x0030] IFn Arbitration 1 Register */
    volatile unsigned int ARB2;         /*!< [0x0034] IFn Arbitration 2 Register */
    volatile unsigned int MCON;         /*!< [0x0038] IFn Message Control Register */
    volatile unsigned int DAT_A1;       /*!< [0x003c] IFn Data A1 Register         */
    volatile unsigned int DAT_A2;       /*!< [0x0040] IFn Data A2 Register         */
    volatile unsigned int DAT_B1;       /*!< [0x0044] IFn Data B1 Register         */
    volatile unsigned int DAT_B2;       /*!< [0x0048] IFn Data B2 Register         */
    volatile unsigned int RESERVE0[13];
} CAN_IF_T;

typedef struct
{
    volatile unsigned int CON;                   /*!< [0x0000] Control Register         */
    volatile unsigned int STATUS;                /*!< [0x0004] Status Register          */
    volatile unsigned int ERR;                   /*!< [0x0008] Error Counter Register   */
    volatile unsigned int BTIME;                 /*!< [0x000c] Bit Timing Register      */
    volatile unsigned int IIDR;                  /*!< [0x0010] Interrupt Identifier Register */
    volatile unsigned int TEST;                  /*!< [0x0014] Test Register                 */
    volatile unsigned int BRPE;                  /*!< [0x0018] Baud Rate Prescaler Extension Register */
    volatile unsigned int RESERVE0[1];
    CAN_IF_T IF[2];
    volatile unsigned int RESERVE2[8];
    volatile unsigned int TXREQ1;                /*!< [0x0100] Transmission Request Register 1  */
    volatile unsigned int TXREQ2;                /*!< [0x0104] Transmission Request Register 2  */
    volatile unsigned int RESERVE3[6];
    volatile unsigned int NDAT1;                 /*!< [0x0120] New Data Register 1              */
    volatile unsigned int NDAT2;                 /*!< [0x0124] New Data Register 2              */
    volatile unsigned int RESERVE4[6];
    volatile unsigned int IPND1;                 /*!< [0x0140] Interrupt Pending Register 1     */
    volatile unsigned int IPND2;                 /*!< [0x0144] Interrupt Pending Register 2     */
    volatile unsigned int RESERVE5[6];
    volatile unsigned int MVLD1;                 /*!< [0x0160] Message Valid Register 1         */
    volatile unsigned int MVLD2;                 /*!< [0x0164] Message Valid Register 2         */
    volatile unsigned int WU_EN;                 /*!< [0x0168] Wake-up Enable Control Register  */
    volatile unsigned int WU_STATUS;             /*!< [0x016c] Wake-up Status Register          */

} CAN_T;

#define CAN_CON_INIT_Pos				(0)
#define CAN_CON_INIT_Msk				(0x1ul << CAN_CON_INIT_Pos)
#define CAN_CON_IE_Pos					(1)
#define CAN_CON_IE_Msk					(0x1ul << CAN_CON_IE_Pos)
#define CAN_CON_SIE_Pos					(2)
#define CAN_CON_SIE_Msk					(0x1ul << CAN_CON_SIE_Pos)
#define CAN_CON_EIE_Pos					(3)
#define CAN_CON_EIE_Msk					(0x1ul << CAN_CON_EIE_Pos)
#define CAN_CON_DAR_Pos					(5)
#define CAN_CON_DAR_Msk					(0x1ul << CAN_CON_DAR_Pos)
#define CAN_CON_CCE_Pos					(6)
#define CAN_CON_CCE_Msk					(0x1ul << CAN_CON_CCE_Pos)
#define CAN_CON_TEST_Pos				(7)
#define CAN_CON_TEST_Msk				(0x1ul << CAN_CON_TEST_Pos)
#define CAN_STATUS_LEC_Pos				(0)
#define CAN_STATUS_LEC_Msk				(0x7ul << CAN_STATUS_LEC_Pos)
#define CAN_STATUS_TXOK_Pos				(3)
#define CAN_STATUS_TXOK_Msk 			(0x1ul << CAN_STATUS_TXOK_Pos)
#define CAN_STATUS_RXOK_Pos				(4)
#define CAN_STATUS_RXOK_Msk				(0x1ul << CAN_STATUS_RXOK_Pos)
#define CAN_STATUS_EPASS_Pos			(5)
#define CAN_STATUS_EPASS_Msk			(0x1ul << CAN_STATUS_EPASS_Pos)
#define CAN_STATUS_EWARN_Pos			(6)
#define CAN_STATUS_EWARN_Msk			(0x1ul << CAN_STATUS_EWARN_Pos)
#define CAN_STATUS_BOFF_Pos				(7)
#define CAN_STATUS_BOFF_Msk				(0x1ul << CAN_STATUS_BOFF_Pos)
#define CAN_ERR_TEC_Pos					(0)
#define CAN_ERR_TEC_Msk					(0xfful << CAN_ERR_TEC_Pos)
#define CAN_ERR_REC_Pos					(8)
#define CAN_ERR_REC_Msk					(0x7ful << CAN_ERR_REC_Pos)
#define CAN_ERR_RP_Pos					(15)
#define CAN_ERR_RP_Msk					(0x1ul << CAN_ERR_RP_Pos)
#define CAN_BTIME_BRP_Pos				(0)
#define CAN_BTIME_BRP_Msk				(0x3ful << CAN_BTIME_BRP_Pos)
#define CAN_BTIME_SJW_Pos				(6)
#define CAN_BTIME_SJW_Msk				(0x3ul << CAN_BTIME_SJW_Pos)
#define CAN_BTIME_TSEG1_Pos				(8)
#define CAN_BTIME_TSEG1_Msk				(0xful << CAN_BTIME_TSEG1_Pos)
#define CAN_BTIME_TSEG2_Pos				(12)
#define CAN_BTIME_TSEG2_Msk				(0x7ul << CAN_BTIME_TSEG2_Pos)
#define CAN_IIDR_IntId_Pos				(0)
#define CAN_IIDR_IntId_Msk				(0xfffful << CAN_IIDR_IntId_Pos)
#define CAN_TEST_BASIC_Pos				(2)
#define CAN_TEST_BASIC_Msk				(0x1ul << CAN_TEST_BASIC_Pos)
#define CAN_TEST_SILENT_Pos				(3)
#define CAN_TEST_SILENT_Msk				(0x1ul << CAN_TEST_SILENT_Pos)
#define CAN_TEST_LBACK_Pos				(4)
#define CAN_TEST_LBACK_Msk				(0x1ul << CAN_TEST_LBACK_Pos)
#define CAN_TEST_Tx_Pos					(5)
#define CAN_TEST_Tx_Msk					(0x3ul << CAN_TEST_Tx_Pos)
#define CAN_TEST_Rx_Pos					(7)
#define CAN_TEST_Rx_Msk					(0x1ul << CAN_TEST_Rx_Pos)
#define CAN_BRPE_BRPE_Pos				(0)
#define CAN_BRPE_BRPE_Msk				(0xful << CAN_BRPE_BRPE_Pos)
#define CAN_IF_CREQ_MSGNUM_Pos			(0)
#define CAN_IF_CREQ_MSGNUM_Msk			(0x3ful << CAN_IF_CREQ_MSGNUM_Pos)
#define CAN_IF_CREQ_BUSY_Pos			(15)
#define CAN_IF_CREQ_BUSY_Msk			(0x1ul << CAN_IF_CREQ_BUSY_Pos)
#define CAN_IF_CMASK_DATAB_Pos			(0)
#define CAN_IF_CMASK_DATAB_Msk			(0x1ul << CAN_IF_CMASK_DATAB_Pos)
#define CAN_IF_CMASK_DATAA_Pos			(1)
#define CAN_IF_CMASK_DATAA_Msk			(0x1ul << CAN_IF_CMASK_DATAA_Pos)
#define CAN_IF_CMASK_TXRQSTNEWDAT_Pos	(2)
#define CAN_IF_CMASK_TXRQSTNEWDAT_Msk	(0x1ul << CAN_IF_CMASK_TXRQSTNEWDAT_Pos)
#define CAN_IF_CMASK_CLRINTPND_Pos		(3)
#define CAN_IF_CMASK_CLRINTPND_Msk		(0x1ul << CAN_IF_CMASK_CLRINTPND_Pos)
#define CAN_IF_CMASK_CONTROL_Pos		(4)
#define CAN_IF_CMASK_CONTROL_Msk		(0x1ul << CAN_IF_CMASK_CONTROL_Pos)
#define CAN_IF_CMASK_ARB_Pos			(5)
#define CAN_IF_CMASK_ARB_Msk			(0x1ul << CAN_IF_CMASK_ARB_Pos)
#define CAN_IF_CMASK_MASK_Pos			(6)
#define CAN_IF_CMASK_MASK_Msk			(0x1ul << CAN_IF_CMASK_MASK_Pos)
#define CAN_IF_CMASK_WRRD_Pos			(7)
#define CAN_IF_CMASK_WRRD_Msk			(0x1ul << CAN_IF_CMASK_WRRD_Pos)
#define CAN_IF_MASK1_Msk_Pos			(0)
#define CAN_IF_MASK1_Msk_Msk			(0xfffful << CAN_IF_MASK1_Msk_Pos)
#define CAN_IF_MASK2_Msk_Pos			(0)
#define CAN_IF_MASK2_Msk_Msk			(0x1ffful << CAN_IF_MASK2_Msk_Pos)
#define CAN_IF_MASK2_MDIR_Pos			(14)
#define CAN_IF_MASK2_MDIR_Msk			(0x1ul << CAN_IF_MASK2_MDIR_Pos)
#define CAN_IF_MASK2_MXTD_Pos			(15)
#define CAN_IF_MASK2_MXTD_Msk			(0x1ul << CAN_IF_MASK2_MXTD_Pos)
#define CAN_IF_ARB1_ID_Pos				(0)
#define CAN_IF_ARB1_ID_Msk				(0xfffful << CAN_IF_ARB1_ID_Pos)
#define CAN_IF_ARB2_ID_Pos				(0)
#define CAN_IF_ARB2_ID_Msk				(0x1ffful << CAN_IF_ARB2_ID_Pos)
#define CAN_IF_ARB2_DIR_Pos				(13)
#define CAN_IF_ARB2_DIR_Msk				(0x1ul << CAN_IF_ARB2_DIR_Pos)
#define CAN_IF_ARB2_XTD_Pos				(14)
#define CAN_IF_ARB2_XTD_Msk				(0x1ul << CAN_IF_ARB2_XTD_Pos)
#define CAN_IF_ARB2_MSGVAL_Pos			(15)
#define CAN_IF_ARB2_MSGVAL_Msk			(0x1ul << CAN_IF_ARB2_MSGVAL_Pos)
#define CAN_IF_MCON_DLC_Pos				(0)
#define CAN_IF_MCON_DLC_Msk				(0xful << CAN_IF_MCON_DLC_Pos)
#define CAN_IF_MCON_EOB_Pos				(7)
#define CAN_IF_MCON_EOB_Msk				(0x1ul << CAN_IF_MCON_EOB_Pos)
#define CAN_IF_MCON_TxRqst_Pos			(8)
#define CAN_IF_MCON_TxRqst_Msk			(0x1ul << CAN_IF_MCON_TxRqst_Pos)
#define CAN_IF_MCON_RmtEn_Pos			(9)
#define CAN_IF_MCON_RmtEn_Msk			(0x1ul << CAN_IF_MCON_RmtEn_Pos)
#define CAN_IF_MCON_RXIE_Pos			(10)
#define CAN_IF_MCON_RXIE_Msk			(0x1ul << CAN_IF_MCON_RXIE_Pos)
#define CAN_IF_MCON_TXIE_Pos			(11)
#define CAN_IF_MCON_TXIE_Msk			(0x1ul << CAN_IF_MCON_TXIE_Pos)
#define CAN_IF_MCON_UMASK_Pos			(12)
#define CAN_IF_MCON_UMASK_Msk			(0x1ul << CAN_IF_MCON_UMASK_Pos)
#define CAN_IF_MCON_IntPnd_Pos			(13)
#define CAN_IF_MCON_IntPnd_Msk			(0x1ul << CAN_IF_MCON_IntPnd_Pos)
#define CAN_IF_MCON_MsgLst_Pos			(14)
#define CAN_IF_MCON_MsgLst_Msk			(0x1ul << CAN_IF_MCON_MsgLst_Pos)
#define CAN_IF_MCON_NEWDAT_Pos			(15)
#define CAN_IF_MCON_NEWDAT_Msk			(0x1ul << CAN_IF_MCON_NEWDAT_Pos)
#define CAN_IF_DAT_A1_DATA0_Pos			(0)
#define CAN_IF_DAT_A1_DATA0_Msk			(0xfful << CAN_IF_DAT_A1_DATA0_Pos)
#define CAN_IF_DAT_A1_DATA1_Pos			(8)
#define CAN_IF_DAT_A1_DATA1_Msk			(0xfful << CAN_IF_DAT_A1_DATA1_Pos)
#define CAN_IF_DAT_A2_DATA2_Pos			(0)
#define CAN_IF_DAT_A2_DATA2_Msk			(0xfful << CAN_IF_DAT_A2_DATA2_Pos)
#define CAN_IF_DAT_A2_DATA3_Pos			(8)
#define CAN_IF_DAT_A2_DATA3_Msk			(0xfful << CAN_IF_DAT_A2_DATA3_Pos)
#define CAN_IF_DAT_B1_DATA4_Pos			(0)
#define CAN_IF_DAT_B1_DATA4_Msk			(0xfful << CAN_IF_DAT_B1_DATA4_Pos)
#define CAN_IF_DAT_B1_DATA5_Pos			(8)
#define CAN_IF_DAT_B1_DATA5_Msk			(0xfful << CAN_IF_DAT_B1_DATA5_Pos)
#define CAN_IF_DAT_B2_DATA6_Pos			(0)
#define CAN_IF_DAT_B2_DATA6_Msk			(0xfful << CAN_IF_DAT_B2_DATA6_Pos)
#define CAN_IF_DAT_B2_DATA7_Pos			(8)
#define CAN_IF_DAT_B2_DATA7_Msk			(0xfful << CAN_IF_DAT_B2_DATA7_Pos)
#define CAN_TXREQ1_TXRQST16_1_Pos		(0)
#define CAN_TXREQ1_TXRQST16_1_Msk		(0xfffful << CAN_TXREQ1_TXRQST16_1_Pos)
#define CAN_TXREQ2_TXRQST32_17_Pos		(0)
#define CAN_TXREQ2_TXRQST32_17_Msk		(0xfffful << CAN_TXREQ2_TXRQST32_17_Pos)
#define CAN_NDAT1_NewData16_1_Pos		(0)
#define CAN_NDAT1_NewData16_1_Msk		(0xfffful << CAN_NDAT1_NewData16_1_Pos)
#define CAN_NDAT2_NewData32_17_Pos		(0)
#define CAN_NDAT2_NewData32_17_Msk		(0xfffful << CAN_NDAT2_NewData32_17_Pos)
#define CAN_IPND1_IntPnd16_1_Pos		(0)
#define CAN_IPND1_IntPnd16_1_Msk		(0xfffful << CAN_IPND1_IntPnd16_1_Pos)
#define CAN_IPND2_IntPnd32_17_Pos		(0)
#define CAN_IPND2_IntPnd32_17_Msk		(0xfffful << CAN_IPND2_IntPnd32_17_Pos)
#define CAN_MVLD1_MsgVal16_1_Pos		(0)
#define CAN_MVLD1_MsgVal16_1_Msk		(0xfffful << CAN_MVLD1_MsgVal16_1_Pos)
#define CAN_MVLD2_MsgVal32_17_Pos		(0)
#define CAN_MVLD2_MsgVal32_17_Msk		(0xfffful << CAN_MVLD2_MsgVal32_17_Pos)
#define CAN_WU_EN_WAKUP_EN_Pos			(0)
#define CAN_WU_EN_WAKUP_EN_Msk			(0x1ul << CAN_WU_EN_WAKUP_EN_Pos)
#define CAN_WU_STATUS_WAKUP_STS_Pos		(0)
#define CAN_WU_STATUS_WAKUP_STS_Msk		(0x1ul << CAN_WU_STATUS_WAKUP_STS_Pos)


#define    CAN0_BA   0xB00A0000
#define    CAN1_BA   0xB00A1000
#define    CAN2_BA   0xB00A2000
#define    CAN3_BA   0xB00A3000

#define CAN0  ((CAN_T *)   CAN0_BA)
#define CAN1  ((CAN_T *)   CAN1_BA)
#define CAN2  ((CAN_T *)   CAN2_BA)
#define CAN3  ((CAN_T *)   CAN3_BA)


/*---------------------------------------------------------------------------------------------------------*/
/* CAN Test Mode Constant Definitions                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define    CAN_NORMAL_MODE   0ul    /*!< CAN select normal mode \hideinitializer */
#define    CAN_BASIC_MODE    1ul    /*!< CAN select basic mode \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/* Message ID Type Constant Definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define    CAN_STD_ID    0ul    /*!< CAN select standard ID \hideinitializer */
#define    CAN_EXT_ID    1ul    /*!< CAN select extended ID \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/* Message Frame Type Constant Definitions                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define    CAN_REMOTE_FRAME    0ul    /*!< CAN frame select remote frame \hideinitializer */
#define    CAN_DATA_FRAME    1ul      /*!< CAN frame select data frame \hideinitializer */

/**
  * @details    CAN message structure
  */
typedef struct
{
    uint32_t  IdType;       /*!< ID type */
    uint32_t  FrameType;    /*!< Frame type */
    uint32_t  Id;           /*!< Message ID */
    uint8_t   DLC;          /*!< Data length */
    uint8_t   Data[8];      /*!< Data */
} STR_CANMSG_T;

#define MSG(id)  (id)


uint32_t CAN_SetBaudRate(CAN_T *tCAN, uint32_t u32BaudRate);
uint32_t CAN_Open(CAN_T *tCAN, uint32_t u32BaudRate, uint32_t u32Mode);
void CAN_Close(CAN_T *tCAN);
void CAN_CLR_INT_PENDING_BIT(CAN_T *tCAN, uint8_t u32MsgNum);
void CAN_EnableInt(CAN_T *tCAN, uint32_t u32Mask);
void CAN_DisableInt(CAN_T *tCAN, uint32_t u32Mask);
int32_t CAN_Transmit(CAN_T *tCAN, uint32_t u32MsgNum, STR_CANMSG_T* pCanMsg);
int32_t CAN_Receive(CAN_T *tCAN, uint32_t u32MsgNum, STR_CANMSG_T* pCanMsg);
int32_t CAN_SetMultiRxMsg(CAN_T *tCAN, uint32_t u32MsgNum, uint32_t u32MsgCount, uint32_t u32IDType, uint32_t u32ID);
int32_t CAN_SetRxMsg(CAN_T *tCAN, uint32_t u32MsgNum, uint32_t u32IDType, uint32_t u32ID);
int32_t CAN_SetRxMsgAndMsk(CAN_T *tCAN, uint32_t u32MsgNum, uint32_t u32IDType, uint32_t u32ID, uint32_t u32IDMask);
int32_t CAN_SetTxMsg(CAN_T *tCAN, uint32_t u32MsgNum, STR_CANMSG_T* pCanMsg);
int32_t CAN_TriggerTxMsg(CAN_T  *tCAN, uint32_t u32MsgNum);
int32_t CAN_BasicSendMsg(CAN_T *tCAN, STR_CANMSG_T* pCanMsg);
int32_t CAN_BasicReceiveMsg(CAN_T *tCAN, STR_CANMSG_T* pCanMsg);
void CAN_EnterInitMode(CAN_T *tCAN, uint8_t u8Mask);
void CAN_EnterTestMode(CAN_T *tCAN, uint8_t u8TestMask);
void CAN_LeaveTestMode(CAN_T *tCAN);
uint32_t CAN_GetCANBitRate(CAN_T *tCAN);
uint32_t CAN_IsNewDataReceived(CAN_T *tCAN, uint8_t u8MsgObj);
void CAN_LeaveInitMode(CAN_T *tCAN);
int32_t CAN_SetRxMsgObjAndMsk(CAN_T *tCAN, uint8_t u8MsgObj, uint8_t u8idType, uint32_t u32id, uint32_t u32idmask, uint8_t u8singleOrFifoLast);
int32_t CAN_SetRxMsgObj(CAN_T *tCAN, uint8_t u8MsgObj, uint8_t u8idType, uint32_t u32id, uint8_t u8singleOrFifoLast);
void CAN_WaitMsg(CAN_T *tCAN);
int32_t CAN_ReadMsgObj(CAN_T *tCAN, uint8_t u8MsgObj, uint8_t u8Release, STR_CANMSG_T* pCanMsg);


void CAN_init(CAN_T *tCAN);

#endif



