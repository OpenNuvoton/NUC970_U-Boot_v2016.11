/*
 * (C) Copyright 2014
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * Configuation settings for the NUC970 EV Board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#include <common.h>
#include <asm/io.h>
#include <malloc.h>
#include <mmc.h>
#include "nuc970_mmc.h"

int nuc970_sd_check_ready_busy(void)
{
	int cnt = 10;
	while(cnt-- > 0) {
		writel(readl(REG_SDCSR) | CLK8_OE, REG_SDCSR);
		while(readl(REG_SDCSR) & CLK8_OE);
		if(readl(REG_SDISR) & SDDAT0)
			break;
	}

	if(cnt == 0)
		return(-1);
	else
		return(0);
}

int nuc970_emmc_check_ready_busy(void)
{
	int cnt = 10;

	while(cnt-- > 0) {
		writel(readl(REG_EMMCCTL) | CLK8_OE, REG_EMMCCTL);
		while(readl(REG_EMMCCTL) & CLK8_OE);
		if(readl(REG_EMMCINTSTS) & SDDAT0)
			break;
	}

	if(cnt == 0)
		return(-1);
	else
		return(0);
}

#define REG_MFP_GPH_L   0xB00000A8
#define REG_MFP_GPH_H   0xB00000AC

int nuc970_sd_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	unsigned char *c;
	int i, j, tmp[5];
	unsigned int block_length, blocks;
	unsigned int sdcsr = readl(REG_SDCSR) & DBW;      // keep bus width. config other fields in this function

	/*
	        printf("[%s]REG_SDH_GCTL = 0x%x\n",__FUNCTION__,readl(REG_SDH_GCTL));
	        printf("[%s]REG_SDCSR = 0x%x\n",__FUNCTION__,readl(REG_SDCSR));
	        printf("[%s]REG_SDIER = 0x%x\n",__FUNCTION__,readl(REG_SDIER));
	        printf("[%s]REG_SDISR = 0x%x\n",__FUNCTION__,readl(REG_SDISR));
	        printf("[%s]REG_ECTL = 0x%x\n",__FUNCTION__,readl(REG_ECTL));
	        printf("[%s]cmd->cmdidx = 0x%x\n",__FUNCTION__,cmd->cmdidx);
	        printf("[%s]REG_SDRSP0 = 0x%x\n",__FUNCTION__,readl(REG_SDRSP0));
	        printf("[%s]REG_SDRSP1 = 0x%x\n",__FUNCTION__,readl(REG_SDRSP1));
	        printf("[%s]REG_MFP_GPH_L = 0x%x\n",__FUNCTION__,readl(REG_MFP_GPH_L));
	        printf("[%s]REG_MFP_GPH_H = 0x%x\n",__FUNCTION__,readl(REG_MFP_GPH_H));
		printf("========================================================\n");
	*/

	if((readl(REG_SDCSR) & 0xF) != 0)
		if(nuc970_sd_check_ready_busy() < 0) {
			if ((readl(REG_SDISR) & SDDAT0) == 0)
				return(-ETIMEDOUT);
		}

	if(mmc_resp_type(cmd) != MMC_RSP_NONE) {
		if(mmc_resp_type(cmd) == MMC_RSP_R2) {
			sdcsr |= R2_EN;
		} else {
			sdcsr |= RI_EN;
		}

		writel(RITO_IF, REG_SDISR);
		writel(0xFFFF, REG_SDTMOUT);
	}

	if (mmc->priv == 0) // SD port 0
		sdcsr &= ~0x60000000;                    // Select port 0
	else if (mmc->priv == 1) // SD port 1
		sdcsr = (sdcsr & ~0x60000000) | 0x20000000;                    // Select port 1

	sdcsr |= 0x09010000;                    // Set SDNWR and BLK_CNT to 1, update later
	sdcsr |= (cmd->cmdidx << 8) | CO_EN;

	if (data) {
		block_length = data->blocksize;
		blocks = data->blocks;

		//printf("size %d len %d\n", block_length, blocks);
		writel(block_length - 1, REG_SDBLEN);

		if (block_length <= 0x200) {
			if (blocks < 256)
				sdcsr = (sdcsr & ~0xFF0000) | (blocks << 16);
			else
				printf("NUC970 SD Max block transfer is 255!!\n");
		}
		//printf("sdcsr 0x%x \n", sdcsr);

		if (data->flags == MMC_DATA_READ) {
			writel(DITO_IF, REG_SDISR);
			writel(0xFFFFFF, REG_SDTMOUT);
			sdcsr |= DI_EN;
			writel((unsigned int)data->dest, REG_DMACSAR2);

		} else if (data->flags == MMC_DATA_WRITE) {
			sdcsr |= DO_EN;
			writel((unsigned int)data->src, REG_DMACSAR2);

		}

	} else {
		block_length = 0;
		blocks = 0;
	}

	writel(cmd->cmdarg, REG_SDARG);
	//printf("arg: %x\n", cmd->cmdarg);
	writel(sdcsr, REG_SDCSR);

	while (readl(REG_SDCSR) & CO_EN); //wait 'til command out complete

	if(mmc_resp_type(cmd) != MMC_RSP_NONE) {
		if(mmc_resp_type(cmd) == MMC_RSP_R2) {
			while (readl(REG_SDCSR) & R2_EN);
			c = (unsigned char *)SDH_BA;
			for (i = 0, j = 0; j < 5; i += 4, j++)
				tmp[j] = (*(c + i) << 24) |
				         (*(c + i + 1) << 16) |
				         (*(c + i + 2) << 8) |
				         (*(c + i + 3));
			for (i = 0; i < 4; i++)
				cmd->response[i] = ((tmp[i] & 0x00ffffff) << 8) |
				                   ((tmp[i + 1] & 0xff000000) >> 24);
		} else {
			while (1) {
				if (!(readl(REG_SDCSR) & RI_EN))
					break;
				if (readl(REG_SDISR) & RITO_IF) {
					writel(RITO_IF, REG_SDISR);
					writel(0, REG_SDTMOUT);
					return(-ETIMEDOUT);
				}
			}
			//printf("=>%x %x %x %x\n", sdcsr, readl(REG_SDBLEN), readl(REG_SDRSP0), readl(REG_SDRSP1));
			//printf("[xxxx]REG_SDISR = 0x%x\n",readl(REG_SDISR));
			cmd->response[0] = (readl(REG_SDRSP0) << 8) | (readl(REG_SDRSP1) & 0xff);
			cmd->response[1] = cmd->response[2] = cmd->response[3] = 0;
		}
	}
	if ((readl(REG_SDISR) & CRC_7) == 0) {
		writel(CRC_IF, REG_SDISR);
		if (mmc_resp_type(cmd) & MMC_RSP_CRC) {
			printf("xxxx retrun err!\n");
			return(-EILSEQ);
		}

	}


	if (data) {
		if (data->flags & MMC_DATA_READ) {
			//printf("R\n");
			//printf("**** %x %x %x %x\n", readl(REG_DMACCSR), readl(REG_DMACSAR2), readl(REG_DMACBCR), readl(REG_DMACISR));
			//writel(readl(REG_SDCSR) | DI_EN, REG_SDCSR);

			while(!(readl(REG_SDISR) & BLKD_IF));
			writel(BLKD_IF, REG_SDISR);
			//while(1);

		} else if (data->flags & MMC_DATA_WRITE) {
			//printf("W\n");
			while(!(readl(REG_SDISR) & BLKD_IF));
			writel(BLKD_IF, REG_SDISR);

			writel(readl(REG_SDCSR) | CLK_KEEP0, REG_SDCSR);

			while (!(readl(REG_SDISR) & SDDAT0));
			writel(readl(REG_SDCSR) & ~CLK_KEEP0, REG_SDCSR);

		}
	}

	return(0);

}

int nuc970_emmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	unsigned char *c;
	int i, j, tmp[5];
	unsigned int block_length, blocks;
	unsigned int emmcctl = readl(REG_EMMCCTL) & DBW;      // keep bus width. config other fields in this function

	/*
		printf("[%s]REG_FMICTL = 0x%x\n",__FUNCTION__,readl(REG_FMICTL));
	        printf("[%s]REG_EMMCCTL = 0x%x\n",__FUNCTION__,readl(REG_EMMCCTL));
	        printf("[%s]REG_EMMCINTEN = 0x%x\n",__FUNCTION__,readl(REG_EMMCINTEN));
	        printf("[%s]REG_EMMCINTSTS = 0x%x\n",__FUNCTION__,readl(REG_EMMCINTSTS));
	        printf("[%s]cmd->cmdidx = 0x%x\n",__FUNCTION__,cmd->cmdidx);
	        printf("[%s]REG_EMMCRESP0 = 0x%x\n",__FUNCTION__,readl(REG_EMMCRESP0));
	        printf("[%s]REG_EMMCRESP1 = 0x%x\n",__FUNCTION__,readl(REG_EMMCRESP1));
		printf("========================================================\n");
	*/

	if((readl(REG_EMMCCTL) & 0xF) != 0)
		if(nuc970_emmc_check_ready_busy() < 0)
			return(-ETIMEDOUT);


	if(mmc_resp_type(cmd) != MMC_RSP_NONE) {
		if(mmc_resp_type(cmd) == MMC_RSP_R2) {
			emmcctl |= R2_EN;
		} else {
			emmcctl |= RI_EN;
		}

		writel(RITO_IF, REG_EMMCINTSTS);
		writel(0xFFFF, REG_EMMCTOUT);
	}

	emmcctl |= 0x09010000;                    // Set SDNWR and BLK_CNT to 1, update later
	emmcctl |= (cmd->cmdidx << 8) | CO_EN;

	if (data) {
		block_length = data->blocksize;
		blocks = data->blocks;

		//printf("size %d len %d\n", block_length, blocks);
		writel(block_length - 1, REG_EMMCBLEN);

		if (block_length <= 0x200) {
			if (blocks < 256)
				emmcctl = (emmcctl & ~0xFF0000) | (blocks << 16);
			else
				printf("NUC970 eMMC Max block transfer is 255!!\n");
		}
		//printf("emmcctl 0x%x \n", emmcctl);

		if (data->flags == MMC_DATA_READ) {
			emmcctl |= DI_EN;
			writel((unsigned int)data->dest, REG_FMIDMASA);

		} else if (data->flags == MMC_DATA_WRITE) {
			emmcctl |= DO_EN;
			writel((unsigned int)data->src, REG_FMIDMASA);

		}

	} else {
		block_length = 0;
		blocks = 0;
	}


	writel(cmd->cmdarg, REG_EMMCCMD);
	//printf("arg: %x\n", cmd->cmdarg);
	writel(emmcctl, REG_EMMCCTL);
	udelay(300);
	while (readl(REG_EMMCCTL) & CO_EN); //wait 'til command out complete



	if(mmc_resp_type(cmd) != MMC_RSP_NONE) {
		if(mmc_resp_type(cmd) == MMC_RSP_R2) {
			while (readl(REG_EMMCCTL) & R2_EN);
			c = (unsigned char *)FMI_BA;
			for (i = 0, j = 0; j < 5; i += 4, j++)
				tmp[j] = (*(c + i) << 24) |
				         (*(c + i + 1) << 16) |
				         (*(c + i + 2) << 8) |
				         (*(c + i + 3));
			for (i = 0; i < 4; i++)
				cmd->response[i] = ((tmp[i] & 0x00ffffff) << 8) |
				                   ((tmp[i + 1] & 0xff000000) >> 24);
		} else {
			while (1) {
				if (!(readl(REG_EMMCCTL) & RI_EN))
					break;
				if (readl(REG_EMMCINTSTS) & RITO_IF) {
					writel(RITO_IF, REG_EMMCINTSTS);
					writel(0, REG_EMMCTOUT);
					return(-ETIMEDOUT);
				}
			}
			//printf("=>%x %x %x %x\n", emmcctl, readl(REG_EMMCBLEN), readl(REG_EMMCRESP0), readl(REG_EMMCRESP1));
			//printf("[xxxx]REG_EMMCINTSTS = 0x%x\n",readl(REG_EMMCINTSTS));
			cmd->response[0] = (readl(REG_EMMCRESP0) << 8) | (readl(REG_EMMCRESP1) & 0xff);
			cmd->response[1] = cmd->response[2] = cmd->response[3] = 0;
		}
	}
	if ((readl(REG_EMMCINTSTS) & CRC_7) == 0) {
		writel(CRC_IF, REG_EMMCINTSTS);
		if (mmc_resp_type(cmd) & MMC_RSP_CRC) {
			printf("xxxx retrun err!\n");
			return(-EILSEQ);
		}

	}


	if (data) {
		if (data->flags & MMC_DATA_READ) {
			//printf("R\n");
			//printf("**** %x %x %x %x\n", readl(REG_DMACCSR), readl(REG_DMACSAR2), readl(REG_DMACBCR), readl(REG_DMACISR));
			//writel(readl(REG_EMMCCTL) | DI_EN, REG_EMMCCTL);


			while(!(readl(REG_EMMCINTSTS) & BLKD_IF));

			writel(BLKD_IF, REG_EMMCINTSTS);
			//while(1);

		} else if (data->flags & MMC_DATA_WRITE) {
			while(!(readl(REG_EMMCINTSTS) & BLKD_IF));
			writel(BLKD_IF, REG_EMMCINTSTS);

			writel(readl(REG_EMMCCTL) | CLK_KEEP0, REG_EMMCCTL);

			while (!(readl(REG_EMMCINTSTS) & SDDAT0));
			writel(readl(REG_EMMCCTL) & ~CLK_KEEP0, REG_EMMCCTL);

		}
	}

	return(0);

}

// there are 3 bits for divider N0, maximum is 8
#define SD_CLK_DIV0_MAX     8
// there are 8 bits for divider N1, maximum is 256
#define SD_CLK_DIV1_MAX     256

static void nuc970_sd_set_clock(struct mmc *mmc, uint clock)
{

	uint    rate,div0,div1;

	if (clock < (*(mmc->cfg)).f_min)
		clock = (*(mmc->cfg)).f_min;
	if (clock > (*(mmc->cfg)).f_max)
		clock = (*(mmc->cfg)).f_max;

	//printf("clock = %d\n",clock);

	rate = MMC_CLK / clock;
	//printf("MMC_CLK = %d, clock=%d,rate=0x%x\n",MMC_CLK,clock,rate);

	// choose slower clock if system clock cannot divisible by wanted clock
	if (MMC_CLK % clock != 0)
		rate++;

	if (rate >= (SD_CLK_DIV0_MAX * SD_CLK_DIV1_MAX)) // the maximum divider for SD_CLK is (SD_CLK_DIV0_MAX * SD_CLK_DIV1_MAX)
		rate = SD_CLK_DIV0_MAX * SD_CLK_DIV1_MAX;

	for (div0 = SD_CLK_DIV0_MAX; div0 > 0; div0--) {  // choose the maximum value if can exact division
		if (rate % div0 == 0)
			break;
	}
	if (div0 == 0) { // cannot exact division
		// if rate <= SD_CLK_DIV1_MAX, set div0 to 1 since div1 can exactly divide input clock
		div0 = (rate <= SD_CLK_DIV1_MAX) ? 1 : SD_CLK_DIV0_MAX;
	}

	//--- calculate the second divider CLKDIVCTL3[eMMC_N]
	div1 = rate / div0;
	div1 &= 0xFF;


	//printf("bf set CLKDIV : REG_CLKDIVCTL9 = 0x%x\n",readl(REG_CLKDIVCTL9));
	writel((readl(REG_CLKDIVCTL9) & ~0x18) | 0x18, REG_CLKDIVCTL9); //Set SDH clock source from UCLKOut
	writel((readl(REG_CLKDIVCTL9) & ~0x07) | (div0-1), REG_CLKDIVCTL9); //Set SDH source clock divider(SDH_SDIV)
	writel((readl(REG_CLKDIVCTL9) & ~0xFF00) | ((div1-1) << 8), REG_CLKDIVCTL9); //Set SDH clock divider(SDH_N)
	//printf("af set CLKDIV : REG_CLKDIVCTL9 = 0x%x\n",readl(REG_CLKDIVCTL9));
	//printf("set div0 to %d, div1 to %d, clock %d\n", div0, div1, clock);
	//  printf("%x %x %x %x %x\n", readl(REG_CLKEN), readl(REG_CLKEN1), readl(REG_MFSEL), readl(REG_GPIOD_DIR), readl(REG_GPIOD_DATAOUT));

	return;
}

static void nuc970_emmc_set_clock(struct mmc *mmc, uint clock)
{

	uint    rate,div0,div1;

	if (clock < (*(mmc->cfg)).f_min)
		clock = (*(mmc->cfg)).f_min;
	if (clock > (*(mmc->cfg)).f_max)
		clock = (*(mmc->cfg)).f_max;

	//printf("clock = %d\n",clock);

	rate = MMC_CLK / clock;
	//printf("MMC_CLK = %d, clock=%d,rate=0x%x\n",MMC_CLK,clock,rate);

	// choose slower clock if system clock cannot divisible by wanted clock
	if (MMC_CLK % clock != 0)
		rate++;

	if (rate >= (SD_CLK_DIV0_MAX * SD_CLK_DIV1_MAX)) // the maximum divider for SD_CLK is (SD_CLK_DIV0_MAX * SD_CLK_DIV1_MAX)
		rate = SD_CLK_DIV0_MAX * SD_CLK_DIV1_MAX;

	for (div0 = SD_CLK_DIV0_MAX; div0 > 0; div0--) {  // choose the maximum value if can exact division
		if (rate % div0 == 0)
			break;
	}
	if (div0 == 0) { // cannot exact division
		// if rate <= SD_CLK_DIV1_MAX, set div0 to 1 since div1 can exactly divide input clock
		div0 = (rate <= SD_CLK_DIV1_MAX) ? 1 : SD_CLK_DIV0_MAX;
	}

	//--- calculate the second divider CLKDIVCTL3[eMMC_N]
	div1 = rate / div0;
	div1 &= 0xFF;


	//printf("bf set CLKDIV : REG_CLKDIVCTL3 = 0x%x\n",readl(REG_CLKDIVCTL3));
	writel((readl(REG_CLKDIVCTL3) & ~0x18) | 0x18, REG_CLKDIVCTL3); //Set eMMC clock source from UCLKOut
	writel((readl(REG_CLKDIVCTL3) & ~0x07) | (div0-1), REG_CLKDIVCTL3); //Set eMMC source clock divider(eMMC_SDIV)
	writel((readl(REG_CLKDIVCTL3) & ~0xFF00) | ((div1-1) << 8), REG_CLKDIVCTL3); //Set eMMC clock divider(eMMC_N)
	//printf("af set CLKDIV : REG_CLKDIVCTL3 = 0x%x\n",readl(REG_CLKDIVCTL3));
	//printf("set div0 to %d, div1 to %d, clock %d\n", div0, div1, clock);
	//printf("%x %x %x %x %x\n", readl(REG_CLKEN), readl(REG_CLKEN1), readl(REG_MFSEL), readl(REG_GPIOD_DIR), readl(REG_GPIOD_DATAOUT));

	return;
}

void nuc970_sd_set_ios(struct mmc *mmc)
{
	if(mmc->clock)
		nuc970_sd_set_clock(mmc, mmc->clock);

	if (mmc->bus_width == 4)
		writel(readl(REG_SDCSR) | DBW, REG_SDCSR);
	else
		writel(readl(REG_SDCSR) & ~DBW, REG_SDCSR);

	return;
}

void nuc970_emmc_set_ios(struct mmc *mmc)
{
	if(mmc->clock)
		nuc970_emmc_set_clock(mmc, mmc->clock);

	if (mmc->bus_width == 4)
		writel(readl(REG_EMMCCTL) | DBW, REG_EMMCCTL);
	else
		writel(readl(REG_EMMCCTL) & ~DBW, REG_EMMCCTL);

	return;
}


static int _nuc970_sd_init(struct mmc *mmc)
{
	int volatile i;

	writel(GCTL_RST, REG_SDH_GCTL);
	writel(DMAC_SW_RST, REG_DMACCSR);
	for(i = 0; i < 10; i++);        // Need few clock delay 'til SW_RST auto cleared.
	writel(SD_EN, REG_SDH_GCTL);
	writel(DMACEN, REG_DMACCSR);

	writel(readl(REG_ECTL) & ~3, REG_ECTL); // SD port 0,1 power enable

	if (mmc->priv == 0) { // SD port 0
		writel(readl(REG_SDCSR) & ~0x60000000, REG_SDCSR); // SD port selection : Select SD0
		writel(readl(REG_SDIER) | 0x40000000, REG_SDIER); // SD port 0 card detect source set to SD0_nCD
	} else if (mmc->priv == 1) { // SD port 1
		writel((readl(REG_SDCSR) & ~0x60000000) | 0x20000000, REG_SDCSR); // SD port selection : Select SD1
		writel(readl(REG_SDIER) | 0x80000000, REG_SDIER); // SD port 0 card detect source set to SD1_nCD
	}

	/*
		printf("[%s]REG_SDH_GCTL = 0x%x\n",__FUNCTION__,readl(REG_SDH_GCTL));
	        printf("[%s]REG_SDCSR = 0x%x\n",__FUNCTION__,readl(REG_SDCSR));
	        printf("[%s]REG_SDIER = 0x%x\n",__FUNCTION__,readl(REG_SDIER));
	        printf("[%s]REG_SDISR = 0x%x\n",__FUNCTION__,readl(REG_SDISR));
	        printf("[%s]REG_ECTL = 0x%x\n",__FUNCTION__,readl(REG_ECTL));
	        printf("[%s]REG_MFP_GPH_L = 0x%x\n",__FUNCTION__,readl(REG_MFP_GPH_L));
	        printf("[%s]REG_MFP_GPH_H = 0x%x\n",__FUNCTION__,readl(REG_MFP_GPH_H));
	*/
	return(0);
}

static int _nuc970_emmc_init(struct mmc *mmc)
{
	int volatile i;

	writel(readl(REG_FMICTL) | GCTL_RST, REG_FMICTL);
	writel(DMAC_SW_RST, REG_FMIDMACTL);
	for(i = 0; i < 10; i++);        // Need few clock delay 'til SW_RST auto cleared.
	writel(readl(REG_FMICTL) | EMMC_EN, REG_FMICTL);
	writel(DMACEN, REG_FMIDMACTL);

	/*
	printf("[%s]REG_FMICTL = 0x%x\n",__FUNCTION__,readl(REG_FMICTL));
	printf("[%s]REG_EMMCCTL = 0x%x\n",__FUNCTION__,readl(REG_EMMCCTL));
	printf("[%s]REG_EMMCINTEN = 0x%x\n",__FUNCTION__,readl(REG_EMMCINTEN));
	printf("[%s]REG_EMMCINTSTS = 0x%x\n",__FUNCTION__,readl(REG_EMMCINTSTS));
	*/

	return(0);
}

//int board_mmc_getcd(u8 *cd, struct mmc *mmc)
int board_mmc_getcd(struct mmc *mmc)
{
	u8 cd;

	if ((mmc->priv == 0) || (mmc->priv == 1)) { // SD port 0,1
		//printf("[%s] REG_SDISR = 0x%x\n",__FUNCTION__,readl(REG_SDISR));
		//printf("[%s] REG_SDIER = 0x%x\n",__FUNCTION__,readl(REG_SDIER));

		if (mmc->priv == 0) { // SD port 0
			if (((readl(REG_SDIER) & 0x40000000) >> 30) == 0)  // for DAT3 mode
				cd = ((readl(REG_SDISR) & (1 << 16)) == (1 << 16)) ? 1 : 0;
			else
				cd = ((readl(REG_SDISR) & (1 << 16)) == (1 << 16)) ? 0 : 1;
		} else if (mmc->priv == 1) { // SD port 1
			if (((readl(REG_SDIER) & 0x80000000) >> 31) == 0)  // for DAT3 mode
				cd = ((readl(REG_SDISR) & (1 << 17)) == (1 << 17)) ? 1 : 0;
			else
				cd = ((readl(REG_SDISR) & (1 << 17)) == (1 << 17)) ? 0 : 1;
		}
	} else if (mmc->priv == 2) { // eMMC
		cd = 1; //CWWeng test
	}

	//printf("cd = %d\n",cd);

	//return 0;
	return cd;
}

static const struct mmc_ops nuc970_sd_ops = {
	.send_cmd	= nuc970_sd_send_cmd,
	.set_ios	= nuc970_sd_set_ios,
	.init		= _nuc970_sd_init,
};

static const struct mmc_ops nuc970_emmc_ops = {
	.send_cmd	= nuc970_emmc_send_cmd,
	.set_ios	= nuc970_emmc_set_ios,
	.init		= _nuc970_emmc_init,
};

static struct mmc_config nuc970_sd_cfg = {
	.name		= "NUC970 SD",
	.ops		= &nuc970_sd_ops,
	//.host_caps	= MMC_MODE_4BIT | MMC_MODE_HS | MMC_MODE_HC,
	.host_caps	= MMC_MODE_4BIT | MMC_MODE_HS,
	.voltages	= MMC_VDD_33_34 | MMC_VDD_32_33| MMC_VDD_31_32| MMC_VDD_30_31| MMC_VDD_29_30| MMC_VDD_28_29| MMC_VDD_27_28,
	.b_max		= 255,
};

static struct mmc_config nuc970_emmc_cfg = {
	.name		= "NUC970 eMMC",
	.ops		= &nuc970_emmc_ops,
	//.host_caps	= MMC_MODE_4BIT | MMC_MODE_HS | MMC_MODE_HC,
	.host_caps	= MMC_MODE_4BIT | MMC_MODE_HS,
	.voltages	= MMC_VDD_33_34 | MMC_VDD_32_33| MMC_VDD_31_32| MMC_VDD_30_31| MMC_VDD_29_30| MMC_VDD_28_29| MMC_VDD_27_28,
	.b_max		= 255,
};

int nuc970_mmc_init(int priv)
{
	struct mmc *mmc;

	//mmc->priv = host;
	if ((priv == 0) || (priv == 1)) { // SD
		nuc970_sd_cfg.ops = &nuc970_sd_ops;
		nuc970_sd_cfg.f_min = 400000;
		nuc970_sd_cfg.f_max = 50000000;
		mmc = mmc_create(&nuc970_sd_cfg, priv);
		if (mmc == NULL)
			return -1;
		mmc->priv = priv;
	} else if (priv == 2) { //eMMC
		nuc970_emmc_cfg.ops = &nuc970_emmc_ops;
		nuc970_emmc_cfg.f_min = 300000;
		nuc970_emmc_cfg.f_max = 20000000;
		mmc = mmc_create(&nuc970_emmc_cfg, priv);
		if (mmc == NULL)
			return -1;
		mmc->priv = priv;
	}

	return 0;
}

