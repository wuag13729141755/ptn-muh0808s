/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     mdrv_ExternalChipDriver.c
**Writer:       wf8421
**Create Date:  2018-12-3
**Rewriter:     wf8421
**Rewrite Date: 2018-12-3
**Description:  外部芯片驱动的文件
**History:
**Version:      V1.0.0
******************************************************/

#define __DRV_EXTERNALCHIPDRIVER_C__
#include "includes.h"

#if _ENABLE_CHIP_LM74HC595
#include "LM74HC595/drv_LM74HC595.c"
#endif

#if _EN_CHIP_PCA9539_USE
#include "pca9539/PCA9539.c"
#endif

#if _EN_CHIP_TM1623_USE
#include "tm1623/tm1623.c"
#endif

#if _ENABLE_CHIP_ITE6615
#include "IT6615/iTE6615_CEC_DRV.c"
#include "IT6615/iTE6615_CEC_FETURE.c"
#include "IT6615/iTE6615_CEC_SYS.c"
#include "IT6615/iTE6615_DRV_TX.c"
#include "IT6615/iTE6615_EDID_PARSER.c"
#include "IT6615/iTE6615_EVB_Debug.c"
#include "IT6615/iTE6615_I2C_RDWR.c"
#include "IT6615/iTE6615_IO.c"
#include "IT6615/iTE6615_Main.c"
//#include "IT6615/iTE6615_sha1.c"
#include "IT6615/iTE6615_SYS_FLOW.c"
#include "IT6615/iTE6615_Utility.c"
#include "IT6615/iTE6615_Table_VM.h"
#include "IT6615/iTE6615_HDCP_Repeater.c"
#include "IT6615/iTE6615_HDCP_Repeater_Callback.c"

#endif

#if _ENABLE_CHIP_IDT5V49EE902||_ENABLE_CHIP_IDT5P49V6965
#include "IDT/IDT.c"
#endif

#if _ENABLE_CHIP_ITE66021
#include "IT66021/it6602.c"
#include "IT66021/it66021_main.c"
#include "IT66021/it6602_Utility.c"
//#include "IT66021/it6602_sha1.c"
#include "IT66021/it6602_repeater.c"
#include "IT66021/it6602_Remote.c"
#include "IT66021/it6602_IO.c"
#include "IT66021/it6602_cec.c"
#endif

#if _ENABLE_CHIP_ITE66351
#include "IT6635/chip/it6635.c"
#include "IT6635/Customer/IT6635_CecSys.c"
#include "IT6635/Customer/IT6635_CecSys_TV.c"
#include "IT6635/chip/it6635_cec.c"
#include "IT6635/chip/it6635_EQ.c"
#include "IT6635/chip/it6635_IO.c"
#endif

#if _ENABLE_CHIP_ITE66353
#include "IT66353/chip/it66353.c"
#include "IT66353/Customer/IT66353_CecSys.c"
#include "IT66353/Customer/IT66353_CecSys_TV.c"
#include "IT66353/chip/it66353_cec.c"
#include "IT66353/chip/it66353_EQ.c"
#include "IT66353/chip/it66353_IO.c"
#include "IT66353/chip/it66353_opts.c"
#endif

#if _ENABLE_CHIP_LM74HC138
#include "LM74LS138/drv_LM74LS138.c"
#endif

#if _ENABLE_CPLD_FUNCTION
#include "drvCPLD/Drv_CPLD.c"
#endif

#if _ENABLE_CHIP_PT2258
#include "PT2258/drv_PT2258.c"
#endif

#if _EN_CHIP_ADN4604_USE
#include "ADN4604/drv_ADN4604.c"
#endif

#if _ENABLE_SPI_FLASH
#include "exFlash/bsp_spi_flash.c"
#endif

#if _ENABLE_CHIP_ITE66121
#include "IT66121/src/IT66121_csc.c"
//#include "IT66121/src/IT66121_hdmitx_cec.c"
#include "IT66121/src/IT66121_hdmitx_drv.c"
#include "IT66121/src/IT66121_hdmitx_hdcp.c"
#include "IT66121/src/IT66121_hdmitx_input.c"
#include "IT66121/src/IT66121_hdmitx_sys.c"
#include "IT66121/src/IT66121_sha1.c"
#include "IT66121/io/it66121_drv.c"
#include "IT66121/io/IT66121_IO.c"
#endif

#if _ENABLE_CHIP_MS7200
#include "MS7200/dvout/ms7200_drv_dvout.c"
#include "MS7200/hdmi_rx/ms7200_drv_hdmi_rx.c"
#include "MS7200/misc/ms7200_drv_misc.c"
#include "MS7200/misc/ms7200_mpi.c"
#include "MS7200/ms7200.c"
#include "MS7200/ms7200_loop.c"
#endif

#if _EN_CHIP_LT8644_USE
#include "LT8644/drv_LT8644.c"
#endif

#if ENABLE_SERIES_LCD_FUNCTION
#include "drvLcd/lcd_driver.c"
#endif


