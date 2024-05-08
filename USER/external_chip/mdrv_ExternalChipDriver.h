/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     mdrv_ExternalChipDriver.h
**Writer:       wf8421
**Create Date:  2018-12-3
**Rewriter:     wf8421
**Rewrite Date: 2018-12-3
**Description:  外部芯片驱动的文件
**History:
**Version:      V1.0.0
******************************************************/

#ifndef __DRV_EXTERNALCHIPDRIVER_H__
#define __DRV_EXTERNALCHIPDRIVER_H__


#if _ENABLE_CHIP_LM74HC595
#include "LM74HC595/drv_LM74HC595.h"
#endif

#if _EN_CHIP_PCA9539_USE
#include "pca9539/PCA9539.h"
#endif

#if _EN_CHIP_TM1623_USE
#include "tm1623/tm1623.h"
#endif

#if _ENABLE_CHIP_ITE6615
#include "IT6615/iTE6615_CEC_DEFINE.h"
#include "IT6615/iTE6615_CEC_DRV.h"
#include "IT6615/iTE6615_CEC_FETURE.h"
#include "IT6615/iTE6615_CEC_SYS.h"
#include "IT6615/iTE6615_Config.h"
#include "IT6615/iTE6615_Debug.h"
#include "IT6615/iTE6615_DEV_DEFINE.h"
#include "IT6615/iTE6615_DRV_TX.h"
#include "IT6615/iTE6615_EDID_PARSER.h"
#include "IT6615/iTE6615_EVB_Debug.h"
#include "IT6615/iTE6615_Global.h"
#include "IT6615/iTE6615_I2C_RDWR.h"
#include "IT6615/iTE6615_IO.h"
#include "IT6615/iTE6615_Main.h"
#include "IT6615/iTE6615_mcu.h"
//#include "IT6615/iTE6615_Regc51.h"
//#include "IT6615/iTE6615_sha1.h"
#include "IT6615/iTE6615_SYS_FLOW.h"
#include "IT6615/iTE6615_Table_CSC.h"
//#include "IT6615/iTE6615_Table_VM.h"
#include "IT6615/iTE6615_Typedef.h"
#include "IT6615/iTE6615_Utility.h"
#include "IT6615/iTE6615_version.h"
#include "IT6615/iTE6615_HDCP_Repeater.h"
#include "IT6615/iTE6615_HDCP_Repeater_Callback.h"
#include "IT6615/iTE6615_HDCP_Repeater_DEFINE.h"
#endif

#if _ENABLE_CHIP_IDT5V49EE902||_ENABLE_CHIP_IDT5P49V6965
#include "IDT/IDT.h"
#endif

#if _ENABLE_CHIP_ITE66021
#include "IT66021/it6602_config.h"
#include "IT66021/it6602.h"
#include "IT66021/it66021_main.h"
#include "IT66021/it6602_Utility.h"
//#include "IT66021/it6602_sha1.c"
//#include "IT66021/it6602_repeater.h"
#include "IT66021/it6602_Remote.h"
#include "IT66021/it6602_IO.h"
#include "IT66021/it6602_debug.h"
#include "IT66021/it6602_Mcu.h"
#include "IT66021/it6602_mcu_conf.h"
#include "IT66021/it6602_nop.h"
#include "IT66021/it6602_reg.h"
//#include "IT66021/it6602_reg_c51.h"
//#include "IT66021/it6602_REG_MPC82G516.h"
#include "IT66021/it6602_typedef.h"
#include "IT66021/it6602_version.h"
#endif

#if _ENABLE_CHIP_ITE66351
#include "IT6635/chip/IT6635_config.h"
#include "IT6635/chip/it6635.h"
#include "IT6635/Customer/IT6635_CecSys.h"
#include "IT6635/chip/it6635_cec.h"
#include "IT6635/chip/it6635_EQ.h"
#include "IT6635/chip/it6635_IO.h"
#endif

#if _ENABLE_CHIP_ITE66353
#include "IT66353/chip/IT66353_platform.h"
#include "IT66353/chip/IT66353_config.h"
#include "IT66353/chip/it66353.h"
#include "IT66353/Customer/IT66353_CecSys.h"
#include "IT66353/chip/it66353_cec.h"
#include "IT66353/chip/it66353_EQ.h"
#include "IT66353/chip/it66353_IO.h"
#endif

#if _ENABLE_CHIP_LM74HC138
#include "LM74LS138/drv_LM74LS138.h"
#endif

#if _ENABLE_CPLD_FUNCTION
#include "drvCPLD/Drv_CPLD.h"
#endif

#if _ENABLE_CHIP_PT2258
#include "PT2258/drv_PT2258.h"
#endif

#if _EN_CHIP_ADN4604_USE
#include "ADN4604/drv_ADN4604.h"
#endif

#if _ENABLE_SPI_FLASH
#include "exFlash/bsp_spi_flash.h"
#endif

#if _ENABLE_CHIP_ITE66121
#include "IT66121/src/IT66121_typedef.h"
#include "IT66121/src/IT66121_config.h"
#include "IT66121/src/IT66121_hdmitx.h"
//#include "IT66121/src/IT66121_hdmitx_cec.h"
#include "IT66121/src/IT66121_hdmitx_drv.h"
#include "IT66121/src/IT66121_hdmitx_hdcp.h"
#include "IT66121/src/IT66121_hdmitx_input.h"
#include "IT66121/src/IT66121_hdmitx_sys.h"
#include "IT66121/src/IT66121_sha1.h"
#include "IT66121/src/IT66121_debug.h"
#include "IT66121/io/it66121_drv.h"
#include "IT66121/io/IT66121_IO.h"
#endif

#if _ENABLE_CHIP_MS7200
#include "MS7200/inc/ms7200_typedef.h"
#include "MS7200/inc/ms7200_comm.h"
#include "MS7200/inc/ms7200.h"
#include "MS7200/misc/ms7200_config.h"
#include "MS7200/dvout/ms7200_drv_dvout.h"
#include "MS7200/hdmi_rx/ms7200_drv_hdmi_rx.h"
#include "MS7200/hdmi_rx/ms7200_drv_hdmi_rx_config.h"
#include "MS7200/misc/ms7200_drv_misc.h"
#include "MS7200/misc/ms7200_mpi.h"
#include "MS7200/inc/ms7200_loop.h"
#endif

#if _EN_CHIP_LT8644_USE
#include "LT8644/drv_LT8644.h"
#endif

#if ENABLE_SERIES_LCD_FUNCTION
#include "drvLcd/lcd_driver.h"
#endif

#endif  // #ifndef __DRV_EXTERNALCHIPDRIVER_H__

