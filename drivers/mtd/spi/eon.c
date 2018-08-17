/*
 * SPI flash interface
 *
 * Copyright (C) 2008 Atmel Corporation
 * Copyright (C) 2010 Reinhard Meyer, EMK Elektronik
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <fdtdec.h>
#include <malloc.h>
#include <spi.h>
#include <spi_flash.h>
#include <watchdog.h>

#ifdef CONFIG_NUC970
#include "../../spi/nuc970_spi.h"
#endif

#include "sf_internal.h"

#define EN25XX_EN4B             0xb7    /* Enter 4-byte mode */

int spi_flash_cmd_read_quad(struct spi_flash *flash, u32 offset, size_t len, void *data);
int eon_set_4byte_mode(struct spi_flash *flash);
int spi_flash_set_4byte_mode(struct spi_flash *flash);

static inline int spi_flash_use_4byte_mode(struct spi_flash *flash)
{
	return NULL != flash->set_4byte_mode;
}

int spi_flash_set_4byte_mode(struct spi_flash *flash)
{
	if (spi_flash_use_4byte_mode(flash)) 
		return flash->set_4byte_mode(flash);

	return 0;
} 

static void spi_flash_addr(struct spi_flash *flash, u32 addr, u8 *cmd, u8 *cmd_len)
{
	/* cmd[0] is actual command */
	if (spi_flash_use_4byte_mode(flash)) { 
		cmd[1] = addr >> 24; 
		cmd[2] = addr >> 16; 
		cmd[3] = addr >> 8; 
		cmd[4] = addr >> 0; 
		*cmd_len = 5; 
	} else { 
		cmd[1] = addr >> 16; 
		cmd[2] = addr >> 8; 
		cmd[3] = addr >> 0; 
		*cmd_len = 4; 
	}
}

static void spi_flash_page_addr(struct spi_flash *flash, u32 page_addr, u32 byte_addr, u8 *cmd, u8 *cmd_len) 
{ 
	/* cmd[0] is actual command */ 
	if (spi_flash_use_4byte_mode(flash)) { 
		cmd[1] = page_addr >> 16; 
		cmd[2] = page_addr >> 8; 
		cmd[3] = page_addr >> 0; 
		cmd[4] = byte_addr; 
		*cmd_len = 5; 
	} else { 
		cmd[1] = page_addr >> 8; 
		cmd[2] = page_addr >> 0; 
		cmd[3] = byte_addr; 
		*cmd_len = 4; 
	} 
} 

int spi_flash_cmd_write_multi(struct spi_flash *flash, u32 offset,
		size_t len, const void *buf)
{
	unsigned long page_addr, byte_addr, page_size;
	size_t chunk_len, actual;
	int ret;
	u8 cmd[5], cmd_len;

	page_size = flash->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;

	ret = spi_claim_bus(flash->spi);
	if (ret) {
		debug("SF: unable to claim SPI bus\n");
		return ret;
	}

	cmd[0] = CMD_PAGE_PROGRAM;
	for (actual = 0; actual < len; actual += chunk_len) {
		chunk_len = min(len - actual, page_size - byte_addr);

		if (flash->spi->max_write_size)
			chunk_len = min(chunk_len, flash->spi->max_write_size);

		spi_flash_page_addr(flash, page_addr, byte_addr, cmd, &cmd_len);

		debug("PP: 0x%p => cmd = { 0x%02x 0x%02x%02x%02x%02x } chunk_len = %zu\n", 
			buf + actual, cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], chunk_len);

		ret = spi_flash_cmd_write_enable(flash);
		if (ret < 0) {
			debug("SF: enabling write failed\n");
			break;
		}

		ret = spi_flash_cmd_write(flash->spi, cmd, cmd_len,
					  buf + actual, chunk_len);
		if (ret < 0) {
			debug("SF: write failed\n");
			break;
		}

		ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
		if (ret)
			break;

		byte_addr += chunk_len;
		if (byte_addr == page_size) {
			page_addr++;
			byte_addr = 0;
		}
	}

	debug("SF: program %s %zu bytes @ %#x\n",
	      ret ? "failure" : "success", len, offset);

	spi_release_bus(flash->spi);
	return ret;
}

int spi_flash_cmd_write_quad(struct spi_flash *flash, u32 offset,
		size_t len, const void *buf)
{
	unsigned long page_addr, byte_addr, page_size;
	size_t chunk_len, actual;
	int ret;
	u8 cmd[4];
	struct spi_slave *spi = flash->spi;
	
	spi->quad_enable = 1;
		
	page_size = flash->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;
	
	ret = spi_claim_bus(flash->spi);
	if (ret) {
		debug("SF: unable to claim SPI bus\n");
		return ret;
	}

	cmd[0] = CMD_QUAD_PAGE_PROGRAM;
	for (actual = 0; actual < len; actual += chunk_len) {
		chunk_len = min(len - actual, page_size - byte_addr);

		if (flash->spi->max_write_size)
			chunk_len = min(chunk_len, flash->spi->max_write_size);

		cmd[1] = page_addr >> 8;
		cmd[2] = page_addr;
		cmd[3] = byte_addr;

		debug("PP: 0x%p => cmd = { 0x%02x 0x%02x%02x%02x } chunk_len = %zu\n",
		      buf + actual, cmd[0], cmd[1], cmd[2], cmd[3], chunk_len);

		ret = spi_flash_cmd_write_enable(flash);
		if (ret < 0) {
			debug("SF: enabling write failed\n");
			break;
		}

		ret = spi_flash_cmd_write(flash->spi, cmd, 4,
					  buf + actual, chunk_len);
		if (ret < 0) {
			debug("SF: write failed\n");
			break;
		}

		ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
		if (ret)
			break;

		byte_addr += chunk_len;
		if (byte_addr == page_size) {
			page_addr++;
			byte_addr = 0;
		}
	}

	debug("SF: program %s %zu bytes @ %#x\n",
	      ret ? "failure" : "success", len, offset);

	spi->quad_enable = 0;

	spi_release_bus(flash->spi);
	return ret;
}

int spi_flash_cmd_read_fast(struct spi_flash *flash, u32 offset,
		size_t len, void *data)
{
	u8 cmd[6], cmd_len;

	/* Handle memory-mapped SPI */
	if (flash->memory_map)
		memcpy(data, flash->memory_map + offset, len);

	cmd[0] = CMD_READ_ARRAY_FAST;
	spi_flash_addr(flash, offset, cmd, &cmd_len);  
        cmd[cmd_len] = 0x00; 

	return spi_flash_read_common(flash, cmd, cmd_len + 1, data, len);
}

static int EON_en_quad_mode(struct spi_flash *flash)
{
	int ret;

	spi_claim_bus(flash->spi);
	ret = spi_flash_cmd(flash->spi, CMD_QUAD_ENABLE, NULL, 0);
        if (ret) {
                printf("SF: Failed issue enable quad command (CMD_QUAD_ENABLE)\n");
        }
	spi_release_bus(flash->spi);

	return ret;
}

static int EON_disable_quad_mode(struct spi_flash *flash)
{
	int ret;

	spi_claim_bus(flash->spi);
	ret = spi_flash_cmd(flash->spi, CMD_QUAD_RESET, NULL, 0);
        if (ret) {
                printf("SF: Failed issue reset quad command (CMD_QUAD_RESET)\n");
        }
	spi_release_bus(flash->spi);

	return ret;
}

int spi_flash_cmd_read_quad(struct spi_flash *flash, u32 offset,
               size_t len, void *data)
{
	struct spi_slave *spi = flash->spi;
	
	unsigned long page_addr, byte_addr, page_size;
	size_t chunk_len, actual;
	int ret = 0;
	u8 cmd[8], cmd_len;

	EON_en_quad_mode(flash);

	spi->quad_enable = 1;

	/* Handle memory-mapped SPI */
	if (flash->memory_map)
		memcpy(data, flash->memory_map + offset, len);
	
	page_size = flash->page_size;
	page_addr = offset / page_size;
	byte_addr = offset % page_size;

	if (spi_flash_use_4byte_mode(flash)) 
		cmd_len = 8;
	else
		cmd_len = 7;
	
	cmd[0] = CMD_READ_QUAD_EON;

	for (actual = 0; actual < len; actual += chunk_len) {
		//chunk_len = min(len - actual, page_size - byte_addr);
		chunk_len = len; //CWWeng 2015.10.21

		if (cmd_len == 7) { //3-byte address
			cmd[1] = page_addr >> 8;
			cmd[2] = page_addr;
			cmd[3] = byte_addr;
			cmd[4] = 0x0;
			cmd[5] = 0x0;
			cmd[6] = 0x0;
		} else { //4-byte address
			cmd[1] = page_addr >> 16;
			cmd[2] = page_addr >> 8;
			cmd[3] = page_addr;
			cmd[4] = byte_addr;
			cmd[5] = 0x0;
			cmd[6] = 0x0;
			cmd[7] = 0x0;
		}
		
		//ret = spi_flash_read_common(flash, cmd, sizeof(cmd),
		ret = spi_flash_read_common(flash, cmd, cmd_len,
		       data + actual, chunk_len);
		if (ret < 0) {
			debug("SF: read failed");
			break;
		}
		
		byte_addr += chunk_len;
		if (byte_addr == page_size) {
			page_addr++;
			byte_addr = 0;
		}
	}
	
	EON_disable_quad_mode(flash);

	spi->quad_enable = 0;

	return ret;
}

int spi_flash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
			   u8 cmd, u8 poll_bit)
{
	struct spi_slave *spi = flash->spi;
	unsigned long timebase;
	int ret;
	u8 status;
/*
	printf("\n[cmd]>>");
        printf("0x%02x ", cmd);
        printf("\n");
*/
	ret = spi_xfer(spi, 8, &cmd, NULL, SPI_XFER_BEGIN);
	if (ret) {
		debug("SF: Failed to send command %02x: %d\n", cmd, ret);
		return ret;
	}

	timebase = get_timer(0);
	do {
		WATCHDOG_RESET();

		//printf("spi_xfer\n");
		ret = spi_xfer(spi, 8, NULL, &status, 0);
		if (ret)
			return -1;

		if ((status & poll_bit) == 0)
			break;

	} while (get_timer(timebase) < timeout);

	spi_xfer(spi, 0, NULL, NULL, SPI_XFER_END);

	if ((status & poll_bit) == 0)
		return 0;

	/* Timed out */
	debug("SF: time out!\n");
	return -1;
}

int spi_flash_cmd_wait_ready(struct spi_flash *flash, unsigned long timeout)
{
	return spi_flash_cmd_poll_bit(flash, timeout,
		CMD_READ_STATUS, STATUS_WIP);
}

int eon_set_4byte_mode(struct spi_flash *flash)
{
        struct spi_slave *spi = flash->spi;

        return spi_flash_cmd(spi, EN25XX_EN4B, NULL, 0);
}


