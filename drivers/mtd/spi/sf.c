/*
 * SPI flash interface
 *
 * Copyright (C) 2008 Atmel Corporation
 * Copyright (C) 2010 Reinhard Meyer, EMK Elektronik
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <spi.h>

#ifdef CONFIG_SPI_FLASH_EON
static int spi_flash_read_write(struct spi_slave *spi,
                                const u8 *cmd, size_t cmd_len,
                                const u8 *data_out, u8 *data_in,
                                size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

#ifdef CONFIG_SF_DUAL_FLASH
	if (spi->flags & SPI_XFER_U_PAGE)
		flags |= SPI_XFER_U_PAGE;
#endif
	if (data_len == 0)
		flags |= SPI_XFER_END;

	if (spi->quad_enable)
		flags |= SPI_6WIRE;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send command (%zu bytes): %d\n",
		      cmd_len, ret);
	} else if (data_len != 0) {
		if (spi->quad_enable) {
			ret = spi_xfer(spi, data_len * 8, data_out, data_in,
			               SPI_6WIRE | SPI_XFER_END);
		} else {
			ret = spi_xfer(spi, data_len * 8, data_out, data_in,
			               SPI_XFER_END);
		}
		if (ret)
			debug("SF: Failed to transfer %zu bytes of data: %d\n",
			      data_len, ret);
	}

	return ret;
}
#else
static int spi_flash_read_write(struct spi_slave *spi,
                                const u8 *cmd, size_t cmd_len,
                                const u8 *data_out, u8 *data_in,
                                size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

#ifdef CONFIG_SF_DUAL_FLASH
	if (spi->flags & SPI_XFER_U_PAGE)
		flags |= SPI_XFER_U_PAGE;
#endif
	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send command (%zu bytes): %d\n",
		      cmd_len, ret);
	} else if (data_len != 0) {
		if ((cmd[0] == 0x6b/*CMD_READ_QUAD_OUTPUT_FAST*/) || (cmd[0] == 0x32/*CMD_QUAD_PAGE_PROGRAM*/)) {
			ret = spi_xfer(spi, data_len * 8, data_out, data_in,
			               SPI_6WIRE | SPI_XFER_END);
		} else {
			ret = spi_xfer(spi, data_len * 8, data_out, data_in,
			               SPI_XFER_END);
		}
		if (ret)
			debug("SF: Failed to transfer %zu bytes of data: %d\n",
			      data_len, ret);
	}

	return ret;
}
#endif

int spi_flash_cmd_read(struct spi_slave *spi, const u8 *cmd,
                       size_t cmd_len, void *data, size_t data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, NULL, data, data_len);
}

int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, size_t len)
{
	return spi_flash_cmd_read(spi, &cmd, 1, response, len);
}

int spi_flash_cmd_write(struct spi_slave *spi, const u8 *cmd, size_t cmd_len,
                        const void *data, size_t data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, data, NULL, data_len);
}
