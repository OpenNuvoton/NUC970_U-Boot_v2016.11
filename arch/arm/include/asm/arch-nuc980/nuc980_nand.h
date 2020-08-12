/* linux/include/asm/arch/gnand/nuc970_nand.h
*
* Copyright (c) 2008 Nuvoton technology corporation
* All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* Changelog:
*
*   2008/08/19     jcao add this file for nuvoton all nand driver.
*/

#ifndef _NUC900_NAND_H_
#define _NUC900_NAND_H_

#define GNAND_OK					      0
#define GNERR_READ_L2P_MISS			1 	/* read block not found in L2P        */
#define GNAND_ERR					      0xFFFFC000

/* GENERAL ERRORs */
#define GNERR_GENERAL               (GNAND_ERR+1)		/* general error                      */
#define GNERR_MEMORY_OUT      		  (GNAND_ERR+0x5)		/* memory not enough                  */
#define GNERR_GNAND_FORMAT    		  (GNAND_ERR+0x10) 	/* not GNAND format                   */
#define GNERR_FAT_FORMAT            (GNAND_ERR+0x15) 	/* NAND disk was not formatted as FAT */
#define GNERR_BLOCK_OUT				      (GNAND_ERR+0x20) 	/* there's no available free blocks   */
#define GNERR_P2LN_SYNC				      (GNAND_ERR+0x25)
#define GNERR_READONLY_NAND			(GNAND_ERR+0x26)	/* XtraROM */

/* for NAND driver return value */
#define GNERR_IO_ERR				        (GNAND_ERR+0x30) 	/* NAND read/write/erase access failed*/
#define GNERR_NAND_NOT_FOUND		    (GNAND_ERR+0x40) 	/* NAND driver cannot find NAND disk  */
#define GNERR_UNKNOW_ID				      (GNAND_ERR+0x42) 	/* Not supported NAND ID              */


#define DMA_BLOCK_SIZE		0x200

/* FMI Global Control and Status Register(FMICSR) */
#define FMICSR_SW_RST		(1)
#define FMICSR_SD_EN		(1<<1)
#define FMICSR_MS_EN		(1<<2)
#define FMICSR_SM_EN		(1<<3)
#define FMICSR_CF_EN		(1<<4)

/* FMI Global Interrupt Control Register(FMIIER) */
#define FMIIER_DTA_IE		(1)

/* FMI Global Interrupt Status Register (FMIISR) */
#define FMIISR_DTA_IF		(1)


/* DMAC Control and Status Register (DMACCSR) */
#define DMACCSR_DMACEN		(1)
#define DMACCSR_SW_RST		(1<<1)
#define DMACCSR_SG_EN1		(1<<2)
#define DMACCSR_SG_EN2		(1<<3)
#define DMACCSR_ATA_BUSY	(1<<8)
#define DMACCSR_FMI_BUSY	(1<<9)

/* DMAC Interrupt Enable Register (DMACIER) */
#define DMACIER_TABORT_IE	(1)
#define DMACIER_WEOT_IE		(1<<1)

/* DMAC Interrupt Status Register (DMACISR) */
#define DMACISR_TABORT_IF	(1)
#define DMACISR_WEOT_IF		(1<<1)

/* DMAC BIST Control and Status Register (DMACBIST) */
#define DMACBIST_BIST_EN	(1)
#define DMACBIST_FINISH		(1<<1)
#define DMACBIST_FAILED		(1<<2)


/* SD error code internal using */
#define SD_SUCCESS			0x00
#define SD_FAILED			0x01
#define SD_TIMEOUT			0x02
#define SD_REMOVED			0x03
#define SD_STATE_ERR		0x04

/* sd host operation state */
#define SD_STATE_NOP				0x00
#define SD_STATE_READ				0x10

#define SD_STATE_WRITE				0x20
#define SD_STATE_WRITE_BUS_BUSY	0x21
#define SD_STATE_WRITE_START		0x22

/* time out value ( in jiffies ) */
#define SD_CMD_TIMEOUT		100
#define SD_RESP_TIMEOUT	100
#define SD_BUS_TIMEOUT		10
#define SD_TICKCOUNT 200

#define SD_SHORT_DELAY 20
#define SD_LONG_DELAY  50


/* Driver thread command */
#define SD_EVENT_NONE				0
#define SD_EVENT_ADD				1
#define SD_EVENT_REMOVE			2
#define SD_EVENT_QUIT				-1


/* SCSI Sense Key/Additional Sense Code/ASC Qualifier values */
#define SS_NO_SENSE				0
#define SS_COMMUNICATION_FAILURE		0x040800
#define SS_INVALID_COMMAND			0x052000
#define SS_INVALID_FIELD_IN_CDB			0x052400
#define SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE	0x052100
#define SS_LOGICAL_UNIT_NOT_SUPPORTED		0x052500
#define SS_MEDIUM_NOT_PRESENT			0x023a00
#define SS_MEDIUM_REMOVAL_PREVENTED		0x055302
#define SS_NOT_READY_TO_READY_TRANSITION	0x062800
#define SS_RESET_OCCURRED			0x062900
#define SS_SAVING_PARAMETERS_NOT_SUPPORTED	0x053900
#define SS_UNRECOVERED_READ_ERROR		0x031100
#define SS_WRITE_ERROR				0x030c02
#define SS_WRITE_PROTECTED			0x072700
#define SS_INVALID_MEDIUM			0x033000
#define SS_MEDIUM_CHANGED			0x062800


typedef struct _FLAG_SETTING {
        int bWriteProtect;
        int bCrcCheck;
        int bInitSuccess;
        int bMediaChanged;
        volatile int bCardExist;
        volatile int bBusBusy;
        volatile int sdRemove;


        int bWriteProtect1;
        int bCrcCheck1;
        int bInitSuccess1;
        int bMediaChanged1;
        volatile int bCardExist1;
        volatile int bBusBusy1;
        volatile int sdRemove1;

        volatile int curCard;
        volatile int update;
        volatile int needReset;
} NAND_FLAG_SETTING, *PFLAG_SETTING;




struct nand_hostdata {
        int irq;
        struct clk *fmi_clk, *dmac_clk;

        int myID;
        volatile unsigned int state, sense;

        int curDBuf, curSBuf, curCount, bufCount, bakCount;

        struct scatterlist *firstList;
        unsigned int nTotalLists, curOffset, curList;
        unsigned int DMAvaddr, DMApaddr;

        unsigned int nSectorSize;
        unsigned int nTotalSectors;
        unsigned int nCapacityInByte;
/*
        // for SCSI sense
        uint8_t senseKey;
        uint8_t ASCkey;
        uint8_t ASCQkey;

        struct Scsi_Host *shost;
        struct scsi_cmnd *cmd;
        struct device dev;
*/
};

/******************************************/
#define FMI_ERR_ID	0xFFFF0100

#define FMI_TIMEOUT				(FMI_ERR_ID|0x01)
/* NAND error */
#define FMI_SM_INIT_ERROR		(FMI_ERR_ID|0x20)
#define FMI_SM_RB_ERR			(FMI_ERR_ID|0x21)
#define FMI_SM_STATE_ERROR		(FMI_ERR_ID|0x22)
#define FMI_SM_ECC_ERROR		(FMI_ERR_ID|0x23)
#define FMI_SM_STATUS_ERR		(FMI_ERR_ID|0x24)
#define FMI_SM_ID_ERR			(FMI_ERR_ID|0x25)
#define FMI_SM_INVALID_BLOCK	(FMI_ERR_ID|0x26)

#define NAND_TYPE_SLC		0x01
#define NAND_TYPE_MLC		0x00

typedef struct fmi_sm_info_t {
        u32	uSectorPerFlash;
        u32	uBlockPerFlash;
        u32	uPagePerBlock;
        u32	uSectorPerBlock;
        u32	uLibStartBlock;
        int	bIsMulticycle;
        int	bIs2KPageSize;
        int	bIsMLCNand;
} FMI_SM_INFO_T;
extern FMI_SM_INFO_T *pSM0, *pSM1;

#define nuc900_nand_read(reg)		__raw_readl(reg)
#define nuc900_nand_write(reg, val)	__raw_writel((val), (reg))

#define Swap32(val)				((val << 24) |\
								((val << 8) & 0xff0000) |\
								((val >> 8) & 0xff00) |\
								(val >> 24))

#define Make32(c4, c3, c2, c1)		(((c4<<24)  & 0xff000000) |\
								((c3 << 16) & 0x00ff0000) |\
								((c2 << 8) & 0x0000ff00) |\
								(c1 & 0x000000ff))

// Used to stuff Cmnd->result
#define SD_CMD_RESULT( x, y, c )     ( ( ( x )<< 16 )|( ( y )<< 8 ) | ( c ) )

#define SD_COMPUTE_START_LBA(cmd)		(((u32)cmd->cmnd[2] << 24 ) + \
					                                   ((u32)cmd->cmnd[3] << 16 ) + \
		       			                            ((u32)cmd->cmnd[4] << 8)   + \
		                            			       ((u32)cmd->cmnd[5] ))

#define SD_COMPUTE_BLK_COUNT(cmd)	((((u32)cmd->cmnd[7]<<8) + \((u32)cmd->cmnd[8])) / DMA_BLOCK_SIZE)

// IOCTL

#define SD_IOC_MAGIC		's'
#define SD_IOC_NR			2

#define SD_IOC_GET_CARD_TYPE		_IOR(SD_IOC_MAGIC, 0, int)
#define SD_IOC_IO_READ_WRITE		_IOR(SD_IOC_MAGIC, 1, int)



int fmiSMCheckRB(void);
int fmiSM_Reset(void);
int fmiSM_ReadID(FMI_SM_INFO_T *pSM );

int fmiSMCorrectData_512(u32 uDAddr);
int fmiSM_Read_512(FMI_SM_INFO_T *pSM, u32 uSector, u32 uDAddr);
int fmiSM_Write_512(FMI_SM_INFO_T *pSM, u32 uSector, u32 uSAddr);
int fmiSMCorrectData_2K(u32 uDAddr);
int fmiSM_Read_2K(FMI_SM_INFO_T *pSM, u32 uPage, u32 uDAddr);
int fmiSM_Read_2K_RA(FMI_SM_INFO_T *pSM, u32 uPage, u32 ucColAddr);
int fmiSM_Read_2Plane(FMI_SM_INFO_T *pSM, u32 PBA, u32 uPage, u32 uDAddr);
int fmiSM_Read_2Plane_RA(FMI_SM_INFO_T *pSM, u32 uPage, u32 ucColAddr);
int fmiSM_Write_2Plane(FMI_SM_INFO_T *pSM, u32 uSector, u32 ucColAddr, u32 uSAddr);
int fmiSM_Write_2K(FMI_SM_INFO_T *pSM, u32 uSector, u32 ucColAddr, u32 uSAddr);
int fmiCheckInvalidBlock(FMI_SM_INFO_T *pSM, u32 BlockNo);

#endif


