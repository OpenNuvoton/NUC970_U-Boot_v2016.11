/*
 * Copyright (c) 2014 Nuvoton technology corporation.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <common.h>

#if defined(CONFIG_NUC980_I2C)
#include <asm/io.h>
#include <asm-generic/gpio.h>
#endif

#include <asm/io.h>
#include <i2c.h>

#define    SYS_BA    0xB0000000 /* Global Control */
#define    CLK_BA    0xB0000200 /* Clock Control */
#define    REG_PCLKEN1		(CLK_BA+0x01C)  /*  APB IP Clock Enable Control Register 1 */

#define    REG_SYS_GPA_MFPL     (SYS_BA+0x070)  /*!< GPIOA Low Byte Multiple Function Control Register */
#define    REG_SYS_GPA_MFPH     (SYS_BA+0x074)  /*!< GPIOA High Byte Multiple Function Control Register */
#define    REG_SYS_GPB_MFPL     (SYS_BA+0x078)  /*!< GPIOB Low Byte Multiple Function Control Register */
#define    REG_SYS_GPB_MFPH     (SYS_BA+0x07C)  /*!< GPIOB High Byte Multiple Function Control Register */
#define    REG_SYS_GPC_MFPL     (SYS_BA+0x080)  /*!< GPIOC Low Byte Multiple Function Control Register */
#define    REG_SYS_GPC_MFPH     (SYS_BA+0x084)  /*!< GPIOC High Byte Multiple Function Control Register */
#define    REG_SYS_GPD_MFPL     (SYS_BA+0x088)  /*!< GPIOD Low Byte Multiple Function Control Register */
#define    REG_SYS_GPD_MFPH     (SYS_BA+0x08C)  /*!< GPIOD High Byte Multiple Function Control Register */
#define    REG_SYS_GPE_MFPL     (SYS_BA+0x090)  /*!< GPIOE Low Byte Multiple Function Control Register */
#define    REG_SYS_GPE_MFPH     (SYS_BA+0x094)  /*!< GPIOE High Byte Multiple Function Control Register */
#define    REG_SYS_GPF_MFPL     (SYS_BA+0x098)  /*!< GPIOF Low Byte Multiple Function Control Register */
#define    REG_SYS_GPF_MFPH     (SYS_BA+0x09C)  /*!< GPIOF High Byte Multiple Function Control Register */
#define    REG_SYS_GPG_MFPL     (SYS_BA+0x0A0)  /*!< GPIOG Low Byte Multiple Function Control Register */
#define    REG_SYS_GPG_MFPH     (SYS_BA+0x0A4)  /*!< GPIOG High Byte Multiple Function Control Register */
#define    REG_SYS_GPH_MFPL     (SYS_BA+0x0A8)  /*!< GPIOH Low Byte Multiple Function Control Register */
#define    REG_SYS_GPH_MFPH     (SYS_BA+0x0AC)  /*!< GPIOH High Byte Multiple Function Control Register */
#define    REG_SYS_GPI_MFPL     (SYS_BA+0x0B0)  /*!< GPIOI Low Byte Multiple Function Control Register */
#define    REG_SYS_GPI_MFPH     (SYS_BA+0x0B4)  /*!< GPIOI High Byte Multiple Function Control Register */
#define    REG_SYS_GPJ_MFPL     (SYS_BA+0x0B8)  /*!< GPIOJ Low Byte Multiple Function Control Register */

#define    I2C0_BA   0xB0080000
#define    I2C1_BA   0xB0081000
#define    I2C2_BA   0xB0082000
#define    I2C3_BA   0xB0083000

#define I2CNUM_0	0  
#define I2CNUM_1	1 
#define I2CNUM_2	2
#define I2CNUM_3	3
#define RETRY		100


#define i2c_out(dev, byte, addr)	__raw_writel(byte, (dev)->base + addr)
#define i2c_in(dev, addr)			__raw_readl((dev)->base + addr)

#define i2cDisable(dev)		i2c_out(dev, 0x00, I2C_CSR)  /* Disable i2c core and interrupt */
#define i2cEnable(dev)		i2c_out(dev, 0x03, I2C_CSR)  /* Enable i2c core and interrupt  */
#define i2cIsBusFree(dev) ((i2c_in(dev, I2C_SWR) & 0x18) == 0x18 && (i2c_in(dev, I2C_CSR) & 0x0400) == 0) ? 1 : 0

/*-----------------------------------------*/
/* marco, type and constant definitions    */
/*-----------------------------------------*/
#define I2C_NUMBER				4
#define I2C_MAX_BUF_LEN			450

#define I2C_INPUT_CLOCK			12000           /* 12 000 KHz */

/*-----------------------------------------*/
/* global interface variables declarations */
/*-----------------------------------------*/
/* 
	bit map in CMDR
*/
#define I2C_CMD_START			0x10
#define I2C_CMD_STOP			0x08
#define I2C_CMD_READ			0x04
#define I2C_CMD_WRITE			0x02
#define I2C_CMD_NACK			0x01

/* 
	for transfer use 
*/
#define I2C_WRITE				0x00
#define I2C_READ				0x01

#define I2C_STATE_NOP			0x00
#define I2C_STATE_READ			0x01
#define I2C_STATE_WRITE			0x02
#define I2C_STATE_PROBE			0x03

/* 
	i2c register offset 
*/
#define     I2C_CSR		(0x00)  /*!< Control and Status Register */
#define     I2C_DIVIDER	(0x04)  /*!< Clock Prescale Register */
#define     I2C_CMDR	(0x08)  /*!< Command Register */
#define     I2C_SWR		(0x0C)  /*!< Software Mode Control Register */
#define     I2C_RxR		(0x10)  /*!< Data Receive Register */
#define     I2C_TxR		(0x14)  /*!< Data Transmit Register */



#define     CTL0        (0x00)     // [0x0000]
#define     ADDR0       (0x04)     // [0x0004]
#define     DAT         (0x08)     // [0x0008]
#define     STATUS0     (0x0C)     // [0x000c]
#define     CLKDIV      (0x10)     // [0x0010]
#define     TOCTL       (0x14)     // [0x0014]
#define     ADDR1       (0x18)     // [0x0018]
#define     ADDR2       (0x1C)     // [0x001c]
#define     ADDR3       (0x20)     // [0x0020]
#define     ADDRMSK0    (0x24)     // [0x0024]
#define     ADDRMSK1    (0x28)     // [0x0028]
#define     ADDRMSK2    (0x2C)     // [0x002c]
#define     ADDRMSK3    (0x30)     // [0x0030]
#define     WKCTL       (0x3C)     // [0x003c]
#define     WKSTS       (0x40)     // [0x0040]
#define     CTL1        (0x44)     // [0x0044]
#define     STATUS1     (0x48)     // [0x0048]
#define     TMCTL       (0x4C)     // [0x004c]
#define     BUSCTL      (0x50)     // [0x0050]
#define     BUSTCTL     (0x54)     // [0x0054]
#define     BUSSTS      (0x58)     // [0x0058]
#define     CLKTOUT     (0x68)     // [0x0068]

#define I2C_CTL_STA_SI            0x28UL /*!< I2C_CTL setting for I2C control bits. It would set STA and SI bits          \hideinitializer */
#define I2C_CTL_STA_SI_AA         0x2CUL /*!< I2C_CTL setting for I2C control bits. It would set STA, SI and AA bits      \hideinitializer */
#define I2C_CTL_STO_SI            0x18UL /*!< I2C_CTL setting for I2C control bits. It would set STO and SI bits          \hideinitializer */
#define I2C_CTL_STO_SI_AA         0x1CUL /*!< I2C_CTL setting for I2C control bits. It would set STO, SI and AA bits      \hideinitializer */
#define I2C_CTL_SI                0x08UL /*!< I2C_CTL setting for I2C control bits. It would set SI bit                   \hideinitializer */
#define I2C_CTL_SI_AA             0x0CUL /*!< I2C_CTL setting for I2C control bits. It would set SI and AA bits           \hideinitializer */
#define I2C_CTL_STA               0x20UL /*!< I2C_CTL setting for I2C control bits. It would set STA bit                  \hideinitializer */
#define I2C_CTL_STO               0x10UL /*!< I2C_CTL setting for I2C control bits. It would set STO bit                  \hideinitializer */
#define I2C_CTL_AA                0x04UL /*!< I2C_CTL setting for I2C control bits. It would set AA bit                   \hideinitializer */



/*
	ioctl commands 
*/
#define I2C_IOC_SET_DEV_ADDRESS		0  /*!< Set device slave address */
#define I2C_IOC_SET_SUB_ADDRESS		1  /*!< Set sub address */
#define I2C_IOC_SET_SPEED			2  /*!< Set I2C interface speed */

/* 
	error code 
*/
#define I2C_ERR_ID					0xFFFF1100	         /*!< I2C library ID                  */
#define I2C_ERR_NOERROR				(0x00)               /*!< No error                        */
#define I2C_ERR_LOSTARBITRATION		(0x01 | I2C_ERR_ID)  /*!< Arbitration lost error          */
#define I2C_ERR_BUSBUSY				(0x02 | I2C_ERR_ID)  /*!< Bus busy error                  */
#define I2C_ERR_NACK				(0x03 | I2C_ERR_ID)	 /*!< data transfer error             */
#define I2C_ERR_SLAVENACK			(0x04 | I2C_ERR_ID)	 /*!< slave not respond after address */
#define I2C_ERR_NODEV				(0x05 | I2C_ERR_ID)  /*!< Wrong device                    */
#define I2C_ERR_BUSY				(0x06 | I2C_ERR_ID)  /*!< Device busy                     */
#define I2C_ERR_IO					(0x07 | I2C_ERR_ID)  /*!< Interface not open              */
#define I2C_ERR_NOTTY				(0x08 | I2C_ERR_ID)  /*!< Command not support             */

#define I2C_MAX_TX_BUF  102
#define I2C_MAX_RX_BUF  100

volatile uint8_t g_u8DeviceAddr;
volatile uint8_t g_au8TxData[I2C_MAX_TX_BUF];
volatile uint8_t g_u8RxData[I2C_MAX_RX_BUF];
volatile uint8_t g_u8DataLen;
volatile uint8_t g_u8EndFlag = 0;
volatile uint32_t I2Cx_BASE = 0xB0080000;
volatile uint32_t g_u32TransferLength;
volatile uint32_t g_u32AddressLength;
volatile uint32_t g_u32RxDataCount;


volatile int32_t WAIT_EEPROM_TIME = 5000;

volatile uint32_t I2C_TIME_OUT_COUNT = 0x20000;

volatile int32_t I2CNUM = I2CNUM_0;
volatile int32_t NUC980_I2C_SPEED = 100;

/*-----------------------------------------*/
/* global file scope (static) variables    */
/*-----------------------------------------*/
typedef struct{
	int32_t base;		/* i2c bus number */
	int32_t openflag;
	volatile int32_t state;
	int32_t addr;
	uint32_t last_error;

	uint32_t subaddr;
	int32_t subaddr_len;

	uint8_t buffer[I2C_MAX_BUF_LEN];
	volatile uint32_t pos, len;

}i2c_dev;


static i2c_dev i2c_device[I2C_NUMBER];

/**
  * @brief  Set i2c interface speed
  * @param[in] dev i2c device structure pointer
  * @param[in] sp i2c speed
  * @return always 0
  */
static int32_t _i2cSetSpeed(int32_t sp)
{
    uint32_t u32Div;

	if(sp > 400) sp = 400;

    u32Div = (uint32_t)(((I2C_INPUT_CLOCK * 10U) / (sp * 4U) + 5U) / 10U - 1U); /* Compute proper divider for I2C clock */

    __raw_writel(u32Div, (I2Cx_BASE + CLKDIV));

	return 0;
}

/**
  * @brief Reset some variables
  * @param[in] dev i2c device structure pointer
  * @return None
  */
static void _i2cReset(i2c_dev *dev)
{
	dev->addr = -1;
	dev->last_error = 0;
	dev->subaddr = 0;
	dev->subaddr_len = 0;
}

void I2C_MasterRx(uint32_t u32Status)
{
    if (u32Status == 0x08) {                    /* START has been transmitted and prepare SLA+W */
        __raw_writel((g_u8DeviceAddr << 1), (I2Cx_BASE + DAT)); /* Write SLA+W to Register I2CDAT */
        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI_AA, (I2Cx_BASE + CTL0));
    } else if (u32Status == 0x18) {             /* SLA+W has been transmitted and ACK has been received */
        __raw_writel(g_au8TxData[g_u8DataLen++], (I2Cx_BASE + DAT));
        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI_AA, (I2Cx_BASE + CTL0));
    } else if (u32Status == 0x20) {             /* SLA+W has been transmitted and NACK has been received */
        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | (I2C_CTL_STO | I2C_CTL_SI), (I2Cx_BASE + CTL0));
    } else if (u32Status == 0x28) {             /* DATA has been transmitted and ACK has been received */
        if (g_u8DataLen < g_u32AddressLength) {
            __raw_writel(g_au8TxData[g_u8DataLen++], (I2Cx_BASE + DAT));
            __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI_AA, (I2Cx_BASE + CTL0));
        } else {
            __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | (I2C_CTL_STA | I2C_CTL_SI_AA), (I2Cx_BASE + CTL0));
        }
    } else if (u32Status == 0x10) {             /* Repeat START has been transmitted and prepare SLA+R */
        __raw_writel(((g_u8DeviceAddr << 1) | 0x01), (I2Cx_BASE + DAT)); /* Write SLA+R to Register I2CDAT */
        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI_AA, (I2Cx_BASE + CTL0));
    } else if (u32Status == 0x40) {             /* SLA+R has been transmitted and ACK has been received */
        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI_AA, (I2Cx_BASE + CTL0));
    } else if (u32Status == 0x48) {             /* Slave Address NACK */
        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_STO_SI, (I2Cx_BASE + CTL0));  /* Clear SI and send STOP */
    } else if (u32Status == 0x50) {            /* DATA has been received and ACK has been returned */
        g_u8RxData[g_u32RxDataCount++] = __raw_readl(I2Cx_BASE + DAT);
         if(g_u32RxDataCount < (g_u32TransferLength - 1u))
        {
            __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI_AA, (I2Cx_BASE + CTL0));
        }
        else
        {
            __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI, (I2Cx_BASE + CTL0));
        }
    }else if (u32Status == 0x58) {             /* DATA has been received and NACK has been returned */
        g_u8RxData[g_u32RxDataCount++] = __raw_readl(I2Cx_BASE + DAT);
        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | (I2C_CTL_STO | I2C_CTL_SI), (I2Cx_BASE + CTL0));
        g_u8EndFlag = 1;
    } else {
        /* TO DO */
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

void I2C_MasterTx(uint32_t u32Status)
{
    if (u32Status == 0x08) {                    /* START has been transmitted */
        __raw_writel((g_u8DeviceAddr << 1), (I2Cx_BASE + DAT)); /* Write SLA+W to Register I2CDAT */

        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI_AA, (I2Cx_BASE + CTL0));
    } else if (u32Status == 0x18) {             /* SLA+W has been transmitted and ACK has been received */
        __raw_writel(g_au8TxData[g_u8DataLen++], (I2Cx_BASE + DAT));

        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | I2C_CTL_SI_AA, (I2Cx_BASE + CTL0));
    } else if (u32Status == 0x20) {             /* SLA+W has been transmitted and NACK has been received */
        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | (I2C_CTL_STA | I2C_CTL_STO | I2C_CTL_SI_AA), I2Cx_BASE + CTL0);
    } else if (u32Status == 0x28) {             /* DATA has been transmitted and ACK has been received */
        if (g_u8DataLen != (g_u32TransferLength + g_u32AddressLength)) { // for EEPROM need transfer high-byte and low-byte address
            __raw_writel(g_au8TxData[g_u8DataLen++], (I2Cx_BASE + DAT));

            __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | (I2C_CTL_SI_AA), I2Cx_BASE + CTL0);
        } else {
            __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | (I2C_CTL_STO | I2C_CTL_SI), I2Cx_BASE + CTL0);
            g_u8EndFlag = 1;
        }
    } else {
        /* TO DO */
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

/// @endcond /* HIDDEN_SYMBOLS */
    
/**
  * @brief This function reset the i2c interface and enable interrupt.
  * @param[in] param is interface number.
  * @return open status.
  * @retval 0 success.
  * @retval I2C_ERR_BUSY Interface already opened.
  * @retval I2C_ERR_NODEV Interface number out of range.
  */
int32_t i2cOpen(uint32_t param)
{
	i2c_dev *dev;

	if( (uint32_t)param >= I2C_NUMBER)
		return I2C_ERR_NODEV;

	dev = (i2c_dev *)((uint32_t)&i2c_device[(uint32_t)param] );

	if( dev->openflag != 0 )		/* a card slot can open only once */
		return(I2C_ERR_BUSY);
			
	/* Enable engine clock */
	if((uint32_t)param == 0)
	__raw_writel((__raw_readl(REG_PCLKEN1) | 0x1), REG_PCLKEN1);
	else if((uint32_t)param == 1)
	__raw_writel((__raw_readl(REG_PCLKEN1) | 0x2), REG_PCLKEN1);
	else if((uint32_t)param == 2)
	__raw_writel((__raw_readl(REG_PCLKEN1) | 0x4), REG_PCLKEN1);
	else if((uint32_t)param == 3)
	__raw_writel((__raw_readl(REG_PCLKEN1) | 0x8), REG_PCLKEN1);

	memset(dev, 0, sizeof(i2c_dev));

	if((uint32_t)param == 0)
		dev->base = I2C0_BA;
    else if((uint32_t)param == 1)
		dev->base = I2C1_BA;
	else if((uint32_t)param == 2)
		dev->base = I2C2_BA;
	else if((uint32_t)param == 3)
		dev->base = I2C3_BA;

	_i2cReset(dev);

	dev->openflag = 1;
	
	return 0;
}

/**
  * @brief Disable I2C interrupt. And initialize some parameters.
  * @param[in] fd is interface number.
  * @return close status.
  * @retval 0 success.
  * @retval I2C_ERR_NODEV Interface number is out of range.
  */
int32_t i2cClose(int32_t fd)  
{
	i2c_dev *dev;

	if(fd != 0 && fd != 1)
		return(I2C_ERR_NODEV);
		
	dev = (i2c_dev *)( (uint32_t)&i2c_device[fd] );	

	dev->openflag = 0;	

    __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ (0x40)), I2Cx_BASE + CTL0); // Disable I2C

	return 0;
}


/**
  * @brief exit function.
  * @return always 0.
  * @retval 0 Success.
  */
int32_t i2cExit(void)
{
	return(0);
}

/**
  * @brief Install ISR.
  * @param[in] fd is interface number.
  * @return always 0.
  * @retval 0 Success.
  */
int32_t i2cInit(int32_t fd)  
{
	if(fd == 0)
	{
		memset((void *)&i2c_device[0], 0, sizeof(i2c_dev));
	}
	else if(fd == 1)
	{
		memset((void *)&i2c_device[1], 0, sizeof(i2c_dev));
	}
	else if(fd == 2)
	{
		memset((void *)&i2c_device[2], 0, sizeof(i2c_dev));
	}
	else if(fd == 3)
	{
		memset((void *)&i2c_device[3], 0, sizeof(i2c_dev));
	}

	return(0);
}

/*-----------------------------------------------------------------------
 * Initialization
 */
static void nuc980_i2c_init(struct i2c_adapter *adap, int speed, int slaveaddr)
{
	if (adap->hwadapnr == 0)
	{
		I2CNUM = I2CNUM_0;

		#if defined (CONFIG_ENABLE_NUC980_I2C0)
		NUC980_I2C_SPEED = CONFIG_NUC980_I2C0_SPEED;
		I2Cx_BASE = I2C0_BA;
		#endif

		/* Configure multi function pins to I2C0 */
		#if defined (CONFIG_NUC980_I2C0_PA_1)
		__raw_writel((__raw_readl(REG_SYS_GPA_MFPL) & ~0xff) | 0x33, REG_SYS_GPA_MFPL);

		#endif
	}
	else if (adap->hwadapnr == 1)
	{
		I2CNUM = I2CNUM_1;

		#if defined (CONFIG_ENABLE_NUC980_I2C1)
		//NUC980_I2C_SPEED = CONFIG_NUC980_I2C1_SPEED;
		I2Cx_BASE = I2C1_BA;
		#endif
	}
	else if (adap->hwadapnr == 2)
	{
		I2CNUM = I2CNUM_2;

		#if defined (CONFIG_ENABLE_NUC980_I2C2)
		//NUC980_I2C_SPEED = CONFIG_NUC980_I2C1_SPEED;
		I2Cx_BASE = I2C2_BA;
		#endif
	}
	else if (adap->hwadapnr == 3)
	{
		I2CNUM = I2CNUM_3;

		#if defined (CONFIG_ENABLE_NUC980_I2C3)
		//NUC980_I2C_SPEED = CONFIG_NUC980_I2C1_SPEED;
		I2Cx_BASE = I2C3_BA;
		#endif
	}
	else
	{
		printf("\n I2C number not cprrect !! \n");
	}


	i2cInit(I2CNUM);

	i2cOpen((uint32_t)I2CNUM);

	_i2cSetSpeed((int32_t)NUC980_I2C_SPEED);
	
	__raw_writel((__raw_readl(I2Cx_BASE + CTL0) | (0x40)), I2Cx_BASE + CTL0); // Enable I2C
}

/*-----------------------------------------------------------------------
 * Probe to see if a chip is present.
 */
static int nuc980_i2c_probe(struct i2c_adapter *adap, uint8_t addr)
{
	return 1;
}

/*-----------------------------------------------------------------------
 * Read bytes
 */
static int  nuc980_i2c_read(struct i2c_adapter *adap, uchar chip, uint addr,
			int alen, uchar *buffer, int len)
{
    int i;
    uint32_t u32Status;
    uint32_t u32time_out;

    u32time_out = 0;

    g_u8DataLen = 0;
    g_u8DeviceAddr = chip;
    g_u32TransferLength = len;
    
    g_u32RxDataCount = 0;

    if(len > I2C_MAX_RX_BUF)
    {
        printf("\n Rx max. length is %d \n", I2C_MAX_RX_BUF);
        return 1;
    }

    //for(i = 0 ; i < len; i++)
    {
    	g_u8EndFlag = 0;
    	g_u32AddressLength = 2; // for EEPROM address length is 2 bytes
    	g_au8TxData[0] = (uint8_t)(((addr) & 0xFF00) >> 8); // High byte address
        g_au8TxData[1] = (uint8_t)((addr) & 0x00FF);  // Low byte address

    	__raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | (I2C_CTL_STA), I2Cx_BASE + CTL0);
    	
    	while(1)
    	{
    		if(__raw_readl(I2Cx_BASE + CTL0) & I2C_CTL_SI)
        	{
        		u32time_out = 0;
                u32Status = __raw_readl(I2Cx_BASE + STATUS0);
                //printf("\n i2c read status = 0x%x ", u32Status);
                I2C_MasterRx(u32Status);
            }
            
            if(g_u8EndFlag) 
            {
            	for(i = 0 ; i < len; i++)
            	{
            	    buffer[i] = g_u8RxData[i];
            	}
                break;
            }

            u32time_out++;
		    if(u32time_out > I2C_TIME_OUT_COUNT)
		    {
			    printf("\n i2cWrite Time Out! \n");
			    return 1; // error
		    }
        }

    }

	return(0);
}

/*-----------------------------------------------------------------------
 * Write bytes
 */
static int  nuc980_i2c_write(struct i2c_adapter *adap, uchar chip, uint addr,
			int alen, uchar *buffer, int len)
{
	int i;
	uint32_t u32Status;
	uint32_t u32time_out;

    g_u8DeviceAddr = chip;
    g_u32TransferLength = len;

	//for(i = 0 ; i < len ; i++)
	{
		g_u8DataLen = 0;
        u32time_out = 0;

        g_u32AddressLength = 2; // for EEPROM address length is 2 bytes
		g_au8TxData[0] = (uint8_t)(((addr) & 0xFF00) >> 8); // High byte address
        g_au8TxData[1] = (uint8_t)((addr) & 0x00FF);  // Low byte address
        
        if(len > (I2C_MAX_TX_BUF -2))
        {
            printf("\n Tx max. length is %d \n", (I2C_MAX_TX_BUF -2));
            return 1;
        }

        for(i = 0; i < len; i++)
            g_au8TxData[2+i] = (uint8_t)(buffer[i]);

        g_u8EndFlag = 0;

        __raw_writel((__raw_readl(I2Cx_BASE + CTL0) &~ 0x3C) | (I2C_CTL_STA), I2Cx_BASE + CTL0);

        while(1)
        {
        	if(__raw_readl(I2Cx_BASE + CTL0) & I2C_CTL_SI)
        	{
        		u32time_out = 0;
                u32Status = __raw_readl(I2Cx_BASE + STATUS0);
                //printf("\n i2c write starus: 0x%x ", __raw_readl(I2Cx_BASE + STATUS0));
                I2C_MasterTx(u32Status);
            }
            
            if(g_u8EndFlag)
            {
            	//printf("\n g_u8DataLen = %d ", g_u8DataLen);
            	break;
            }
            u32time_out++;
		    if(u32time_out > I2C_TIME_OUT_COUNT)
		    {
			    printf("\n i2cWrite Time Out! \n");
			    return 1; // error
		    }
        }
	}

	return(0);
}

/*
 * Register soft i2c adapters
 */
U_BOOT_I2C_ADAP_COMPLETE(I2CNUM_0, nuc980_i2c_init, nuc980_i2c_probe,
			 nuc980_i2c_read, nuc980_i2c_write, NULL/* _set_speed */,
			 CONFIG_NUC980_I2C0_SPEED, CONFIG_SYS_I2C_SLAVE,
			 0)

#if defined (CONFIG_ENABLE_NUC980_I2C1)
U_BOOT_I2C_ADAP_COMPLETE(I2CNUM_1, nuc980_i2c_init, nuc980_i2c_probe,
			 nuc980_i2c_read, nuc980_i2c_write, NULL,
			 CONFIG_NUC980_I2C1_SPEED,
			 CONFIG_SYS_I2C_SLAVE,
			 1)
#endif

#if defined (CONFIG_ENABLE_NUC980_I2C2)
U_BOOT_I2C_ADAP_COMPLETE(I2CNUM_2, nuc980_i2c_init, nuc980_i2c_probe,
			 nuc980_i2c_read, nuc980_i2c_write, NULL,
			 CONFIG_NUC980_I2C2_SPEED,
			 CONFIG_SYS_I2C_SLAVE,
			 1)
#endif

#if defined (CONFIG_ENABLE_NUC980_I2C3)
U_BOOT_I2C_ADAP_COMPLETE(I2CNUM_3, nuc980_i2c_init, nuc980_i2c_probe,
			 nuc980_i2c_read, nuc980_i2c_write, NULL,
			 CONFIG_NUC980_I2C3_SPEED,
			 CONFIG_SYS_I2C_SLAVE,
			 1)
#endif







