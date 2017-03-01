/*
 * Copyright (c) 2014 Nuvoton Technology Corp.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Description:   NUC970 LCD driver source file
 */

#ifndef  _NUC970_FB_
#define  _NUC970_FB_

#define LCM_BASE 0xB0008000

#define REG_LCM_DCCS                    (LCM_BASE  + 0x00)
#define LCM_DCCS_ENG_RST                (1 << 0)

#define REG_LCM_DEV_CTRL                (LCM_BASE  + 0x04)

#define REG_LCM_MPU_CMD                 (LCM_BASE  + 0x08)
#define REG_LCM_INT_CS                  (LCM_BASE  + 0x0c)
#define REG_LCM_CRTC_SIZE               (LCM_BASE  + 0x10)
#define LCM_CRTC_SIZE_VTTVAL(x)         ((x) << 16)
#define LCM_CRTC_SIZE_HTTVAL(x)         ((x) << 0)
#define REG_LCM_CRTC_DEND               (LCM_BASE  + 0x14)
#define LCM_CRTC_DEND_VDENDVAL(x)       ((x) << 16)
#define LCM_CRTC_DEND_HDENDVAL(x)       ((x) << 0)
#define REG_LCM_CRTC_HR                 (LCM_BASE  + 0x18)
#define LCM_CRTC_HR_EVAL(x)             ((x) << 16)
#define LCM_CRTC_HR_SVAL(x)             ((x) << 0)
#define REG_LCM_CRTC_HSYNC              (LCM_BASE  + 0x1C)
#define LCM_CRTC_HSYNC_SHIFTVAL(x)      ((x) << 30)
#define LCM_CRTC_HSYNC_EVAL(x)          ((x) << 16)
#define LCM_CRTC_HSYNC_SVAL(x)          ((x) << 0)
#define REG_LCM_CRTC_VR                 (LCM_BASE  + 0x20)
#define LCM_CRTC_VR_EVAL(x)             ((x) << 16)
#define LCM_CRTC_VR_SVAL(x)             ((x) << 0)
#define REG_LCM_VA_BADDR0               (LCM_BASE  + 0x24)
#define REG_LCM_VA_BADDR1               (LCM_BASE  + 0x28)
#define REG_LCM_VA_FBCTRL               (LCM_BASE  + 0x2C)
#define REG_LCM_VA_SCALE                (LCM_BASE  + 0x30)
#define REG_LCM_VA_WIN                  (LCM_BASE  + 0x38)
#define REG_LCM_VA_STUFF                (LCM_BASE  + 0x3C)
#define REG_LCM_OSD_WINS                (LCM_BASE  + 0x40)
#define REG_LCM_OSD_WINE                (LCM_BASE  + 0x44)
#define REG_LCM_OSD_BADDR               (LCM_BASE  + 0x48)
#define REG_LCM_OSD_FBCTRL              (LCM_BASE  + 0x4c)
#define REG_LCM_OSD_OVERLAY             (LCM_BASE  + 0x50)
#define REG_LCM_OSD_CKEY                (LCM_BASE  + 0x54)
#define REG_LCM_OSD_CMASK               (LCM_BASE  + 0x58)
#define REG_LCM_OSD_SKIP1               (LCM_BASE  + 0x5C)
#define REG_LCM_OSD_SKIP2               (LCM_BASE  + 0x60)
#define REG_LCM_OSD_SCALE               (LCM_BASE  + 0x64)
#define REG_LCM_MPU_VSYNC               (LCM_BASE  + 0x68)
#define REG_LCM_HC_CTRL                 (LCM_BASE  + 0x6C)
#define REG_LCM_HC_POS                  (LCM_BASE  + 0x70)
#define REG_LCM_HC_WBCTRL               (LCM_BASE  + 0x74)
#define REG_LCM_HC_BADDR                (LCM_BASE  + 0x78)
#define REG_LCM_HC_COLOR0               (LCM_BASE  + 0x7C)
#define REG_LCM_HC_COLOR1               (LCM_BASE  + 0x80)
#define REG_LCM_HC_COLOR2               (LCM_BASE  + 0x84)
#define REG_LCM_HC_COLOR3               (LCM_BASE  + 0x88)

#define GPIO_BASE 0xB8003000

#define REG_GPIOG_DIR                   (GPIO_BASE  + 0x180)
#define REG_GPIOG_DOUT                  (GPIO_BASE  + 0x184)


typedef struct
{
        unsigned int clk;
        unsigned int hight;
        unsigned int width;
        unsigned int left_margin;
        unsigned int right_margin;
        unsigned int hsync_len;
        unsigned int upper_margin;
        unsigned int lower_margin;
        unsigned int vsync_len; 
        unsigned int dccs;
        unsigned int devctl;
        unsigned int fbctrl;
        unsigned int scale;                
}vpost_cfg_t;

#endif

