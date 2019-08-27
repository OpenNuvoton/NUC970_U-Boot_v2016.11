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

#if defined(CONFIG_NUC970_I2C)
#include <asm/io.h>
#include <asm-generic/gpio.h>
#endif

#include <asm/io.h>
#include <i2c.h>

#define    GCR_BA    0xB0000000 /* Global Control */
#define    CLK_BA    0xB0000200 /* Clock Control */
#define    REG_PCLKEN1    (CLK_BA+0x01C)  /*  APB IP Clock Enable Control Register 1 */

#define    REG_MFP_GPG_L  (GCR_BA+0x0A0)  /* GPIOG Low Byte Multiple Function Control Register */

#define    I2C0_BA   0xB8006000 /* I2C 0 Control */
#define    I2C1_BA   0xB8006100 /* I2C 1 Control */

#define    I2CNUM_0 0  
#define    I2CNUM_1 1 
#define    RETRY    100


#define i2c_out(dev, byte, addr)    __raw_writel(byte, (dev)->base + addr)
#define i2c_in(dev, addr)           __raw_readl((dev)->base + addr)

#define i2cDisable(dev)    i2c_out(dev, 0x00, I2C_CSR)  /* Disable i2c core and interrupt */
#define i2cEnable(dev)     i2c_out(dev, 0x03, I2C_CSR)  /* Enable i2c core and interrupt  */
#define i2cIsBusFree(dev)  ((i2c_in(dev, I2C_SWR) & 0x18) == 0x18 && (i2c_in(dev, I2C_CSR) & 0x0400) == 0) ? 1 : 0

/*-----------------------------------------*/
/* marco, type and constant definitions    */
/*-----------------------------------------*/
#define I2C_NUMBER          2
#define I2C_MAX_BUF_LEN     450

#define I2C_INPUT_CLOCK     33000           /* 33 000 KHz */

/*-----------------------------------------*/
/* global interface variables declarations */
/*-----------------------------------------*/
/* 
    bit map in CMDR
*/
#define I2C_CMD_START   0x10
#define I2C_CMD_STOP    0x08
#define I2C_CMD_READ    0x04
#define I2C_CMD_WRITE   0x02
#define I2C_CMD_NACK    0x01

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
#define I2C_ERR_NACK				(0x03 | I2C_ERR_ID)  /*!< data transfer error             */
#define I2C_ERR_SLAVENACK			(0x04 | I2C_ERR_ID)  /*!< slave not respond after address */
#define I2C_ERR_NODEV				(0x05 | I2C_ERR_ID)  /*!< Wrong device                    */
#define I2C_ERR_BUSY				(0x06 | I2C_ERR_ID)  /*!< Device busy                     */
#define I2C_ERR_IO					(0x07 | I2C_ERR_ID)  /*!< Interface not open              */
#define I2C_ERR_NOTTY				(0x08 | I2C_ERR_ID)  /*!< Command not support             */

/*-----------------------------------------*/
/* global file scope (static) variables    */
/*-----------------------------------------*/
typedef struct{
	int32_t base;    /* i2c bus number */
	int32_t openflag;
	volatile int32_t state;
	int32_t addr;
	uint32_t last_error;

	uint32_t subaddr;
	int32_t subaddr_len;

	uint8_t buffer[I2C_MAX_BUF_LEN];
	volatile uint32_t pos, len;
	int32_t bNackValid;
}i2c_dev;

/**
  * @brief  Set i2c interface speed
  * @param[in] dev i2c device structure pointer
  * @param[in] sp i2c speed
  * @return always 0
  */  
static int32_t _i2cSetSpeed(i2c_dev *dev, int32_t sp)  
{
	uint32_t d;

	/* assume speed above 1000 are Hz-specified */
	if(sp > 1000) sp = sp/1000;
	if(sp > 400) sp = 400;

	d = I2C_INPUT_CLOCK/(sp * 5) -1;

	i2c_out(dev, d & 0xffff, I2C_DIVIDER);

	return 0;
}

/**
  * @brief  Configure i2c command
  * @param[in] dev i2c device structure pointer
  * @param[in] cmd command
  * @return None
  */
static void _i2cCommand(i2c_dev *dev, int32_t cmd)
{
	dev->bNackValid = (cmd & I2C_CMD_WRITE) ? 1 : 0;
	i2c_out(dev, cmd, I2C_CMDR);
}

/**
  * @brief Configure slave address data
  * @param[in] dev i2c device structure pointer
  * @param[in] mode could be write or read
  * @return None
  */
static void _i2cCalcAddr(i2c_dev *dev, int32_t mode)
{
	int32_t i;
	uint32_t subaddr;

	subaddr = dev->subaddr;

	dev->buffer[0] = (((dev->addr << 1) & 0xfe) | I2C_WRITE) & 0xff;

	for(i = dev->subaddr_len; i > 0; i--){
		dev->buffer[i] = subaddr & 0xff;
		subaddr >>= 8;
	}

	if(mode == I2C_STATE_READ){
		i = dev->subaddr_len + 1;
		dev->buffer[i] = (((dev->addr << 1) & 0xfe)) | I2C_READ;
	}
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

/**
  * @brief Interrupt service routine for i2c-0
  * @param[in] None
  * @return None
  */
static void i2c_ISR(i2c_dev *dev)
{
	int32_t csr, val;

	csr = i2c_in(dev, I2C_CSR);

	#if 0 // for debug
	if(csr & 0x04)
	{
		printf("\n csr = 0x%x ", csr);
	}
	else
	{
		return;
	}
	#endif

	csr |= 0x04;

	//i2c_out(dev, csr, I2C_CSR);  /* clear interrupt flag */

	if(dev->state == I2C_STATE_NOP)
		return;

	if((csr & 0x800) && (dev->bNackValid)){	/* NACK only valid in WRITE */
		i2c_out(dev, csr, I2C_CSR);  /* clear interrupt flag */

		dev->last_error = I2C_ERR_NACK;
		_i2cCommand(dev, I2C_CMD_STOP);
		dev->state = I2C_STATE_NOP;
	}
	else if(csr & 0x200){	/* Arbitration lost */
		i2c_out(dev, csr, I2C_CSR);  /* clear interrupt flag */

		printf("Arbitration lost\n");
		dev->last_error = I2C_ERR_LOSTARBITRATION;
		dev->state = I2C_STATE_NOP;
	}
	else if(!(csr & 0x100)){		/* transmit complete */
		i2c_out(dev, csr, I2C_CSR);	/* clear interrupt flag */

		if(dev->pos < dev->subaddr_len + 1){	/* send address state */
			val = dev->buffer[dev->pos++] & 0xff;
			i2c_out(dev, val, I2C_TxR);
			_i2cCommand(dev, I2C_CMD_WRITE);
		}
		else if(dev->state == I2C_STATE_READ){

			/* sub address send over , begin restart a read command */

			if(dev->pos == dev->subaddr_len + 1){
				val = dev->buffer[dev->pos++];
				i2c_out(dev, val, I2C_TxR);
				_i2cCommand(dev, I2C_CMD_START | I2C_CMD_WRITE);

			}
			else{

				dev->buffer[dev->pos++] = i2c_in(dev, I2C_RxR) & 0xff;

				if( dev->pos < dev->len){
					if(dev->pos == dev->len -1 )	/* last character */
						_i2cCommand(dev, I2C_CMD_READ |
										I2C_CMD_STOP |
										I2C_CMD_NACK);
					else
						_i2cCommand(dev, I2C_CMD_READ);
				}
				else{
					dev->state = I2C_STATE_NOP;
				}

			}
		}
		else if(dev->state == I2C_STATE_WRITE){	/* write data */

			if( dev->pos < dev->len){
				val = dev->buffer[dev->pos];

				i2c_out(dev, val, I2C_TxR);

				if(dev->pos == dev->len -1 )	/* last character */
				{
					_i2cCommand(dev, I2C_CMD_WRITE| I2C_CMD_STOP);
					//printf("\n csr22 = 0x%x  ", csr);
				}
				else
					_i2cCommand(dev, I2C_CMD_WRITE);

				dev->pos ++;
			}
			else{
				dev->state = I2C_STATE_NOP;
			}
		}
	}

}

#if 0
/**
  * @brief Interrupt service routine for i2c-1
  * @param[in] None
  * @return None
  */
static void i2c1ISR(i2c_dev *dev)
{
	int32_t csr, val;
	i2c_dev *dev;

    dev = (i2c_dev *) ( (uint32_t)&i2c_device[1] );

	csr = i2c_in(dev, I2C_CSR);
	csr |= 0x04;

	//i2c_out(dev, csr, I2C_CSR);  /* clear interrupt flag */

	if(dev->state == I2C_STATE_NOP)
		return;

	if((csr & 0x800) && (dev->bNackValid)){	/* NACK only valid in WRITE */
		i2c_out(dev, csr, I2C_CSR);  /* clear interrupt flag */

		dev->last_error = I2C_ERR_NACK;
		_i2cCommand(dev, I2C_CMD_STOP);
		dev->state = I2C_STATE_NOP;
	}
	else if(csr & 0x200){	/* Arbitration lost */
		i2c_out(dev, csr, I2C_CSR);  /* clear interrupt flag */

		printf("Arbitration lost\n");
		dev->last_error = I2C_ERR_LOSTARBITRATION;
		dev->state = I2C_STATE_NOP;
	}
	else if(!(csr & 0x100)){	/* transmit complete */
		i2c_out(dev, csr, I2C_CSR);  /* clear interrupt flag */

		if(dev->pos < dev->subaddr_len + 1){	/* send address state */
			val = dev->buffer[dev->pos++] & 0xff;
			i2c_out(dev, val, I2C_TxR);
			_i2cCommand(dev, I2C_CMD_WRITE);
		}
		else if(dev->state == I2C_STATE_READ){

			/* sub address send over , begin restart a read command */

			if(dev->pos == dev->subaddr_len + 1){
				val = dev->buffer[dev->pos++];
				i2c_out(dev, val, I2C_TxR);
				_i2cCommand(dev, I2C_CMD_START | I2C_CMD_WRITE);
			}
			else{

				dev->buffer[dev->pos++] = i2c_in(dev, I2C_RxR) & 0xff;

				if( dev->pos < dev->len){
					if(dev->pos == dev->len -1 )	/* last character */
						_i2cCommand(dev, I2C_CMD_READ |
										I2C_CMD_STOP |
										I2C_CMD_NACK);
					else
						_i2cCommand(dev, I2C_CMD_READ);
				}
				else{
					dev->state = I2C_STATE_NOP;
				}
			}
		}
		else if(dev->state == I2C_STATE_WRITE){	/* write data */

			if( dev->pos < dev->len){
				val = dev->buffer[dev->pos];

				i2c_out(dev, val, I2C_TxR);

				if(dev->pos == dev->len -1 )	/* last character */
					_i2cCommand(dev, I2C_CMD_WRITE| I2C_CMD_STOP);
				else
					_i2cCommand(dev, I2C_CMD_WRITE);

				dev->pos ++;
			}
			else{
				dev->state = I2C_STATE_NOP;
			}
		}
	}
}
#endif

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
	if( (uint32_t)param >= I2C_NUMBER)
		return I2C_ERR_NODEV;

	/* Enable engine clock */
	if((uint32_t)param == 0)
	__raw_writel((__raw_readl(REG_PCLKEN1) | 0x1), REG_PCLKEN1);
	else
	__raw_writel((__raw_readl(REG_PCLKEN1) | 0x2), REG_PCLKEN1);

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
	if(fd != 0 && fd != 1)
		return(I2C_ERR_NODEV);

	return 0;
}

/**
  * @brief Read data from I2C slave.
  * @param[in] fd is interface number.
  * @param[in] buf is receive buffer pointer.
  * @param[in] len is receive buffer length.
  * @return read status.
  * @retval >0 length when success.
  * @retval I2C_ERR_BUSY Interface busy.
  * @retval I2C_ERR_IO Interface not opened.
  * @retval I2C_ERR_NODEV No such device.
  * @retval I2C_ERR_NACK Slave returns an erroneous ACK.
  * @retval I2C_ERR_LOSTARBITRATION arbitration lost happen.
  */
int32_t i2cRead(i2c_dev *dev, int32_t fd, uint8_t* buf, uint32_t len)
{
	uint32_t u32time_out = 0;
	uint32_t WAIT_EEPROM_TIME = 5000;
	uint32_t I2C_TIME_OUT_COUNT = 0x200000;

	if(len == 0)
		return 0;

	if(!i2cIsBusFree(dev))
		return(I2C_ERR_BUSY);

	if(len > I2C_MAX_BUF_LEN - 10)
		len = I2C_MAX_BUF_LEN - 10;

	dev->state = I2C_STATE_READ;
	dev->pos = 1;
	/* Current ISR design will get one garbage byte */
	dev->len = dev->subaddr_len + 1 + len + 2;  /* plus 1 unused char */
	dev->last_error = 0;

	_i2cCalcAddr(dev, I2C_STATE_READ);

	i2cEnable(dev);

	i2c_out(dev, dev->buffer[0] & 0xff, I2C_TxR);

	if(!i2cIsBusFree(dev))
		return(I2C_ERR_BUSY);

	_i2cCommand(dev, I2C_CMD_START | I2C_CMD_WRITE);
	udelay(WAIT_EEPROM_TIME);

	while(dev->state != I2C_STATE_NOP)
	{
		i2c_ISR(dev);
		u32time_out++;
		if(u32time_out > I2C_TIME_OUT_COUNT)
		{
			return 0; // error
		}
	}

	i2cDisable(dev);

	if(dev->last_error)
		return(dev->last_error);

	memcpy(buf, dev->buffer + dev->subaddr_len + 3, len);

	dev->subaddr += len;

	return len;
}


/**
  * @brief Write data from I2C slave.
  * @param[in] fd is interface number.
  * @param[in] buf is transmit buffer pointer.
  * @param[in] len is transmit buffer length.
  * @return write status.
  * @retval >0 length when success.
  * @retval I2C_ERR_BUSY Interface busy.
  * @retval I2C_ERR_IO Interface not opened.
  * @retval I2C_ERR_NODEV No such device.
  * @retval I2C_ERR_NACK Slave returns an erroneous ACK.
  * @retval I2C_ERR_LOSTARBITRATION arbitration lost happen.
  */
int32_t i2cWrite(i2c_dev *dev, int32_t fd, uint8_t* buf, uint32_t len)
{
	uint32_t u32time_out = 0;
	uint32_t WAIT_EEPROM_TIME = 5000;
	uint32_t I2C_TIME_OUT_COUNT = 0x20000;

	if(len == 0)
		return 0;

	if(!i2cIsBusFree(dev))
		return(I2C_ERR_BUSY);

	if(len > I2C_MAX_BUF_LEN - 10)
		len = I2C_MAX_BUF_LEN - 10;

	memcpy(dev->buffer + dev->subaddr_len + 1 , buf, len);

	dev->state = I2C_STATE_WRITE;
	dev->pos = 1;
	dev->len = dev->subaddr_len + 1 + len;
	dev->last_error = 0;

	_i2cCalcAddr(dev, I2C_STATE_WRITE);

	i2cEnable(dev);

	i2c_out(dev, dev->buffer[0] & 0xff, I2C_TxR);

	if(!i2cIsBusFree(dev))
		return(I2C_ERR_BUSY);

	_i2cCommand(dev, I2C_CMD_START | I2C_CMD_WRITE);

	while(dev->state != I2C_STATE_NOP)
	{
		i2c_ISR(dev);
		u32time_out++;
		if(u32time_out > I2C_TIME_OUT_COUNT)
		{
			printf("\n i2cWrite Time Out! \n");
			return 0; // error
		}
	}

	udelay(WAIT_EEPROM_TIME);

	i2cDisable(dev);

	if(dev->last_error)
		return(dev->last_error);

	dev->subaddr += len;

	return len;
}

/**
  * @brief Support some I2C driver commands for application.
  * @param[in] fd is interface number.
  * @param[in] cmd is command.
  * @param[in] arg0 is the first argument of command.
  * @param[in] arg1 is the second argument of command.
  * @return command status.
  * @retval 0 Success.
  * @retval I2C_ERR_IO Interface not opened.
  * @retval I2C_ERR_NODEV No such device.
  * @retval I2C_ERR_NOTTY Command not support, or parameter error.
  */
int32_t i2cIoctl(i2c_dev *dev, int32_t fd, uint32_t cmd, uint32_t arg0, uint32_t arg1)
{
	if(fd != 0 && fd != 1)
		return(I2C_ERR_NODEV);

	switch(cmd){
		case I2C_IOC_SET_DEV_ADDRESS:
			dev->addr = arg0;
			break;

		case I2C_IOC_SET_SPEED:

			return(_i2cSetSpeed(dev, (int32_t)arg0));

		case I2C_IOC_SET_SUB_ADDRESS:

			if(arg1 > 4){
				return(I2C_ERR_NOTTY);
			}

			dev->subaddr = arg0;
			dev->subaddr_len = arg1;
			break;

		default:
			return(I2C_ERR_NOTTY);
	}

	return (0);
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

#if 0
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
	else
	{
		memset((void *)&i2c_device[1], 0, sizeof(i2c_dev));
	}

	return(0);
}
#endif

/*-----------------------------------------------------------------------
 * Initialization
 */
static void nuc970_i2c_init(struct i2c_adapter *adap, int speed, int slaveaddr)
{
	printf("\n soft_i2c_init  0x%x \n", adap->hwadapnr);
	i2c_dev dev;

	#if defined (CONFIG_ENABLE_NUC970_I2C0)
	if (adap->hwadapnr == 0)
	{
		dev.base = I2C0_BA;

		/* Configure multi function pins to I2C0 */
		#if defined (CONFIG_NUC970_I2C0_PG)
		__raw_writel((__raw_readl(REG_MFP_GPG_L) & ~0xff) | 0x88, REG_MFP_GPG_L);
		#endif

		i2cOpen((uint32_t)I2CNUM_0);
		i2cIoctl(&dev, I2CNUM_0, I2C_IOC_SET_SPEED, CONFIG_NUC970_I2C0_SPEED, 0);
	}
	#endif

	#if defined (CONFIG_ENABLE_NUC970_I2C1)
	if (adap->hwadapnr == 1)
	{
		dev.base = I2C1_BA;

		dev.base = I2C1_BA;

		i2cOpen((uint32_t)I2CNUM_1);
		i2cIoctl(&dev, I2CNUM_1, I2C_IOC_SET_SPEED, CONFIG_NUC970_I2C1_SPEED, 0);
	}
	#endif
}

/*-----------------------------------------------------------------------
 * Probe to see if a chip is present.
 */
static int nuc970_i2c_probe(struct i2c_adapter *adap, uint8_t addr)
{
	return 1;
}

/*-----------------------------------------------------------------------
 * Read bytes
 */
static int  nuc970_i2c_read(struct i2c_adapter *adap, uchar chip, uint addr,
			int alen, uchar *buffer, int len)
{
	int i, j;
	i2c_dev dev;
	int32_t I2CNUM;


	if (adap->hwadapnr == 0){
		memset((void *)&dev, 0, sizeof(i2c_dev));
		_i2cReset(&dev);
		dev.base = I2C0_BA;
		I2CNUM = I2CNUM_0;
	}
	else if (adap->hwadapnr == 1){
		memset((void *)&dev, 0, sizeof(i2c_dev));
		_i2cReset(&dev);
		dev.base = I2C1_BA;
		I2CNUM = I2CNUM_1;
	}
	else
		return(I2C_ERR_NODEV);

	i2cIoctl(&dev, I2CNUM, I2C_IOC_SET_DEV_ADDRESS, chip, 0);  /* On 970 EV board, set 0x50 for I2C0 */

	/* Rx porcess */
	//printf("Start Rx --> %d %d 0x%x 0x%x \n", len, alen, addr, chip);

	for(i = 0 ; i < len; i++){
		i2cIoctl(&dev, I2CNUM, I2C_IOC_SET_SUB_ADDRESS, addr+i, alen);
		j = RETRY;

		while(j-- > 0){
			if(i2cRead(&dev, I2CNUM, buffer+i, 1) == 1)
			{
				break;
			}
		}

		if(j <= 0)
			printf("Read ERROR [%d]!\n", i);
	}

	return(0);
}

/*-----------------------------------------------------------------------
 * Write bytes
 */
static int  nuc970_i2c_write(struct i2c_adapter *adap, uchar chip, uint addr,
			int alen, uchar *buffer, int len)
{
	int i, j;
	//int tmp;
	i2c_dev dev;
	int32_t I2CNUM;

	if (adap->hwadapnr == 0){
		memset((void *)&dev, 0, sizeof(i2c_dev));
		_i2cReset(&dev);
		dev.base = I2C0_BA;
		I2CNUM = I2CNUM_0;
	}
	else if (adap->hwadapnr == 1){
		memset((void *)&dev, 0, sizeof(i2c_dev));
		_i2cReset(&dev);
		dev.base = I2C1_BA;
		I2CNUM = I2CNUM_1;
	}
	else
		return(I2C_ERR_NODEV);

	i2cIoctl(&dev, I2CNUM, I2C_IOC_SET_DEV_ADDRESS, chip, 0);  /* On 970 EV board, set 0x50 for I2C0 */

	/* Tx porcess */
	//tmp = len;
	//printf("Start Tx --> %d %d 0x%x 0x%x \n", len, alen, addr, chip);
	for(i = 0 ; i < len ; i++){
		i2cIoctl(&dev, I2CNUM, I2C_IOC_SET_SUB_ADDRESS, addr+i, alen);
		j = RETRY;

		while(j-- > 0){
			if(i2cWrite(&dev, I2CNUM, buffer+i, 1) == 1)
			{
				break;
			}
		}

		if(j <= 0){
			printf("\n WRITE ERROR [%d]! \n", i);
			return (1);
		}
	}

	return(0);
}

/*
 * Register soft i2c adapters
 */
U_BOOT_I2C_ADAP_COMPLETE(I2CNUM_0, nuc970_i2c_init, nuc970_i2c_probe,
			 nuc970_i2c_read, nuc970_i2c_write, NULL/* _set_speed */,
			 CONFIG_NUC970_I2C0_SPEED, CONFIG_SYS_I2C_SLAVE,
			 0)

#if defined (CONFIG_ENABLE_NUC970_I2C1)
U_BOOT_I2C_ADAP_COMPLETE(I2CNUM_1, nuc970_i2c_init, nuc970_i2c_probe,
			 nuc970_i2c_read, nuc970_i2c_write, NULL,
			 CONFIG_NUC970_I2C1_SPEED,
			 CONFIG_SYS_I2C_SLAVE,
			 1)
#endif


