/*
 * cmd_decrypt.c - decrypt image
 *
 * Copyright (c) 2014 Nuvoton technology
 *
 */

#include <common.h>
#include <command.h>

#include <asm/io.h>
#include "register.h"

#define Polling_Loop 0x100000

/* Per the 4 boot mode,rearrange the sequence of key.dat of AES encrypt tool */
u32 otp_key[4][8] = {
{0x43521234, 0x87901ac4, 0x80ae4ca1, 0x9087c3c4, 0x87a12c67, 0xa185f7d4, 0xc5a12f56, 0x98115678},
{0xc5a12f56, 0x98115678, 0x43521234, 0x87901ac4, 0x80ae4ca1, 0x9087c3c4, 0x87a12c67, 0xa185f7d4},
{0x87a12c67, 0xa185f7d4, 0xc5a12f56, 0x98115678, 0x43521234, 0x87901ac4, 0x80ae4ca1, 0x9087c3c4},
{0x80ae4ca1, 0x9087c3c4, 0x87a12c67, 0xa185f7d4, 0xc5a12f56, 0x98115678, 0x43521234, 0x87901ac4}
};

static void SetTimer(unsigned int count)
{
        /* Set timer0 */
        writel(readl(REG_PCLKEN0) | 0x100, REG_PCLKEN0); /* enable timer engine clock */
        writel(0x1, REG_TMR_TISR);
        writel(count, REG_TMR_TICR0);  /* set timer init counter value */
        writel(0xC000000B, REG_TMR_TCSR0); /* one-shot mode, prescale = 12 */
}

static int otp_init(void)
{
	int volatile loop;
	unsigned int volatile reg;
	
	writel(readl(0xB0000004) | 0x10000, 0xB0000004);
	printf("Unlock MTP...\n");
	writel(readl(REG_PCLKEN1) | (1 << 26), REG_PCLKEN1);
	writel(0x59, REG_OTP_PROTECT);
	writel(0x16, REG_OTP_PROTECT);
	writel(0x88, REG_OTP_PROTECT);

	if (readl(REG_OTP_PROTECT) == 0x1)
	{
		printf("MTP is unlocked...\n");
		writel(readl(REG_OTP_KEYEN) | 0x1, REG_OTP_KEYEN);
		for (loop = 0; loop < 0x100000; loop++)
		{
			/* check key valid(STATUS) */
			reg = readl(REG_OTP_STATUS);
			if (reg & 1)
			{
				if ( ((reg >> 16) & 0xf) == 0)
				{
					if (reg & 4)
					{
						printf("MTP enabled, No Key programmed\n");
						return 1;
					}
				}
				else if (reg & 2)
				{
					printf("MTP enabled, and key valid\n");
					return 0;
				}
			}
		}
	}

	printf("MTP enable failed, MTP status is 0x%x\n",reg);
	return -1;
}

static int do_decrypt_aes(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u32 src, dst, len;
	u32 volatile reg;
	int ret;

        if (argc < 3)
                return CMD_RET_USAGE;

	if ((ret = otp_init()) == 0)
	{	
		printf("Init crypto engine...\n");

		/* initial CRYPTO engine */
		writel(readl(REG_HCLKEN) | 0x800000, REG_HCLKEN);// enable CRYPTO engine clock 
		writel(0x3, REG_CRPT_IPSEC_INT_EN);
		writel(0, REG_CRPT_AES_IV0);
		writel(0, REG_CRPT_AES_IV1);
		writel(0, REG_CRPT_AES_IV2);
		writel(0, REG_CRPT_AES_IV3);

		writel(0x00C000D9, REG_CRPT_AES_CTL);
	}
	else if (ret == 1)
	{
		printf("No key programmed, pls program key to MTP\n");
		return 1;
	}

	src = simple_strtoul(argv[1], NULL, 0);
	dst = simple_strtoul(argv[2], NULL, 0);
	len = simple_strtoul(argv[3], NULL, 0);

	printf("decrypt source address is 0x%x\n",src);
	printf("decrypt destination address is 0x%x\n",dst);
	printf("decrypt length is %d\n",len);

	writel(src, REG_CRPT_AES_SADR);
	writel(dst, REG_CRPT_AES_DADR);
	writel(len, REG_CRPT_AES_CNT);

	/* clear interrupt flag */
	writel(0x3, REG_CRPT_IPSEC_INT_FLAG);

	/* SECURE_AES_CTL_START | SECURE_AES_CTL_EXT_KEY | SECURE_AES_CTL_DMA_EN | SECURE_AES_CTL_DMA_CASCADE */
	writel(readl(REG_CRPT_AES_CTL) | 0xd1, REG_CRPT_AES_CTL);

	SetTimer(1000);
	while(!(reg = readl(REG_CRPT_IPSEC_INT_FLAG) & 0x3))
	{
		if (readl(REG_TMR_TISR) & 0x1)
		{
			writel(0x1, REG_TMR_TISR);
			printf("Decrpt Timeout\n");
			break;
		}
	}

	writel(0x1, REG_CRPT_IPSEC_INT_FLAG);

	return 0;
}

static int program_otp(u32 key[8], u8 EnSecureBoot) 
{
	u32 volatile reg;
	int  loop = 0;
	u8 key_all_zero = 1;

	//Check key if all 0s
	do {
		if (key[loop++] != 0) {
			key_all_zero = 0;
			break;
		}
	} while (loop < 8);
	
	if (key_all_zero) {
		printf("Key invalid! The keys (256 bits) should not be all 0s!\n");
		return -1;
	}
	
	//Set to program mode
	writel((readl(REG_OTP_MODE) & 0x3) | 0x2, REG_OTP_MODE);
	writel(0x60AE, REG_OTP_CYCLE);

	writel(key[0], REG_OTP_VALUE0);
	writel(key[1], REG_OTP_VALUE1);
	writel(key[2], REG_OTP_VALUE2);
	writel(key[3], REG_OTP_VALUE3);
	writel(key[4], REG_OTP_VALUE4);
	writel(key[5], REG_OTP_VALUE5);
	writel(key[6], REG_OTP_VALUE6);
	writel(key[7], REG_OTP_VALUE7);

	if (EnSecureBoot == 1)
		writel(5, REG_OTP_OPTION); //AES key and enable secure boot
	else
		writel(1, REG_OTP_OPTION); //AES key but disable secure boot
        
	writel(0x1, REG_OTP_START);
        
        for (loop = 0; loop < Polling_Loop; loop++)
        {
                if (readl(REG_OTP_START) == 0)
                        break;
        }
        if (loop >= Polling_Loop)
        {
                printf("MTP_START not cleared!\n");
                return -1;
        }

	if ((reg = readl(REG_OTP_STATUS)) & 0x10) //Program fail
        {
                printf("MTP key program failed! [0x%x]\n", reg);
                return -1;
        }
        
	printf("MTP option is %d\n",readl(REG_OTP_OPTION));
        printf("MTP key program OK, COUNT = %d\n",(reg >> 16) & 0xf);

	return 0;
}

/*
 * Note that before enabling secure boot, you have to burn U-Boot with the same AES key!
 * Otherwise, your system will be locked!!!
*/

static int do_decrypt_program(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	volatile int boot_mode;
	u8 EnSecureBoot;

        if (argc < 3)
                return CMD_RET_USAGE;

	if (otp_init() < 0)
		return -1;

	printf("Program AES key to MTP!\n");

	EnSecureBoot = simple_strtoul(argv[2], NULL, 0);

	if (EnSecureBoot == 1)
		printf("Enable secure boot!\n");
	else 
		printf("Disable secure boot!\n");

	boot_mode = readl(0xB0000004) & 0x3;
	printf("Boot mode is %d\n", boot_mode);

	if (program_otp(otp_key[boot_mode], EnSecureBoot) < 0)
		printf("Program MTP Failed!\n");
	else
		printf("Program MTP Successful!\n");

	return 0;
}

static cmd_tbl_t cmd_decrypt_sub[] ={
U_BOOT_CMD_MKENT(aes, CONFIG_SYS_MAXARGS, 1, do_decrypt_aes, "", ""),
U_BOOT_CMD_MKENT(program, CONFIG_SYS_MAXARGS, 1, do_decrypt_program, "", ""),
};

static char decrypt_help_text[]=
"decrypt aes SrcAddr DstAddr Length - Decrypt the image from SrcAddr to DstAddr with lenth [Length].\n"
"Example : decrypt aes 0x8000 0x10000 0x200- decrypt the image from 0x8000 to 0x10000 and lenth is 0x200\n\n"
"decrypt program aes EnSecure - program AES key to MTP and [Enable/Disable] secure boot.\n"
"Example :  decrypt program aes 1 - program AES key to MTP and Enable secure boot.\n"
"Example :  decrypt program aes 0 - program AES key to MTP but Disable secure boot.\n\n"
"Note that before enabling secure boot, you have to burn U-Boot with the same AES key!\n"
"Otherwise, your system will be locked!!!\n";

int do_decrypt(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

        cmd_tbl_t *cp;

	printf("Decrypt image start!\n");


        /* drop initial "env" arg */
        argc--;
        argv++;

        cp = find_cmd_tbl(argv[0], cmd_decrypt_sub, ARRAY_SIZE(cmd_decrypt_sub));

        if (cp)
                return cp->cmd(cmdtp, flag, argc, argv);

        return CMD_RET_USAGE;
}

U_BOOT_CMD(decrypt, 5, 1, do_decrypt,
	"Decrypt image(kernel)",
	decrypt_help_text);
