
#include "includes.h"
#ifndef __SII9630_SIDATATYPES_H__
#define __SII9630_SIDATATYPES_H__
#include "Sii9630_si_lib_log.h"
#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#define msleep(x)   delay_ms(x)
#define usleep(x)   delay_ms((x/1000)+1)


#define ssize_t     int

#define SIL9630_USE_TIMER_PROCESS           1

#define SII_PLATFORM_DEBUG_ASSERT(x)        //

#if 1
#define SII_DEBUG
#endif

#ifdef SII_DEBUG
#define sii_log_err(...) 		sii_log_generic_dbg(DBG_MSG_CRIT_ERR, __VA_ARGS__)
#define sii_log_warn(...) 		sii_log_generic_dbg(DBG_MSG_WARN, __VA_ARGS__)
#define sii_log_info(...) 		sii_log_generic_dbg(DBG_MSG_INFO, __VA_ARGS__)
#define sii_log_debug(...) 		sii_log_generic_dbg(DBG_MSG_DEBUG, __VA_ARGS__)
#define sii_log_print(...) 		sii_log_generic_dbg_print(DBG_MSG_DEBUG, __VA_ARGS__)
#define pr_err(...)             sii_log_generic_dbg_print(DBG_MSG_WARN, __VA_ARGS__)
#define pr_info(...)            sii_log_generic_dbg_print(DBG_MSG_INFO, __VA_ARGS__)
#define pr_debug(...)           sii_log_generic_dbg_print(DBG_MSG_DEBUG, __VA_ARGS__)
#else
#define sii_log_err(...) 		//sii_log_generic_dbg(DBG_MSG_CRIT_ERR, __VA_ARGS__)
#define sii_log_warn(...) 		//sii_log_generic_dbg(DBG_MSG_WARN, __VA_ARGS__)
#define sii_log_info(...) 		//sii_log_generic_dbg(DBG_MSG_INFO, __VA_ARGS__)
#define sii_log_debug(...) 		//sii_log_generic_dbg(DBG_MSG_DEBUG, __VA_ARGS__)
#define sii_log_print(...) 		//sii_log_generic_dbg_print(DBG_MSG_DEBUG, __VA_ARGS__)
#define pr_err(...)             //sii_log_generic_dbg_print(DBG_MSG_WARN, __VA_ARGS__)
#define pr_info(...)            //sii_log_generic_dbg_print(DBG_MSG_INFO, __VA_ARGS__)
#define pr_debug(...)           //sii_log_generic_dbg_print(DBG_MSG_DEBUG, __VA_ARGS__)
#endif

#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))

#define CONFIG_SII9630_HDMI_TX          _MACRO_ENABLE
//#define CONFIG_SII8630_MHL_TX           _MACRO_DISABLE
//#define CONFIG_SII8630_HDMI_MHL_TX      _MACRO_DISABLE
#define CONFIG_TX_HDCP2X_EN             _MACRO_ENABLE
#define CONFIG_DSC_EN                   _MACRO_ENABLE
#define CONFIG_AUTO_SW_RST_EN           _MACRO_ENABLE
//#define CONFIG_POWER_DOWN_EN            _MACRO_DISABLE
//#define CONFIG_CAPTURE_REGISTER_DUMP    _MACRO_DISABLE
#define CONFIG_DEFINED_MACRO            _MACRO_ENABLE
//#define CONFIG_PLL_MANUAL_ZONE          _MACRO_DISABLE
#define CONFIG_HANDLE_BOARD_RESET       _MACRO_ENABLE
//#define CONFIG_DSC_HDMI_VSIF            _MACRO_DISABLE
#define CONFIG_ENHANCED_MODE_PROT       _MACRO_ENABLE
#define CONFIG_EDID_SWITCH              _MACRO_ENABLE
//#define CONFIG_FPGA_PLATFORM            _MACRO_DISABLE
//#define CONFIG_MHL_RBP_EN               _MACRO_DISABLE
//#define CONFIG_MHL3_EN                  _MACRO_DISABLE
//#define CONFIG_EMSC_EN                  _MACRO_DISABLE
//#define CONFIG_MDT_EN                   _MACRO_DISABLE
//#define CONFIG_RX_HDCP2X_EN             _MACRO_ENABLE
//#define CONFIG_SWAY_CONFIG_POST_CONV    _MACRO_DISABLE
//#define CONFIG_HDCP_REPEATER_EN         _MACRO_DISABLE
//#define CONFIG_TX_TMDS_MODE_CTRL        _MACRO_ENABLE

#define REMOVE_SCDC_DEPENDENCY_ON_EM_PROTOCOL
#define AVIF_INSTEAD_OF_VSIF_FOR_EMM

#define SII_PRODUCT_NUM                 "9630"
#define SII_BUILD_NUM                   "0.92.01.01"
#define BUILD_NUM_STRING                "0.7.0.0"
#define SII_DEVICE_NAME                 "sii9630"

/*! DDC bus access error codes*/
enum ddc_com_err {
	SI_TX_DDC_ERROR_CODE_NO_ERROR = 0x00, /*!< success */
	SI_TX_DDC_ERROR_CODE_TIMEOUT = 0x01,
	/*!< DDC bus is not granted within timeout */
	SI_TX_DDC_ERROR_CODE_NO_ACK = 0x02, /*!< no ACK from DDC device */
	SI_TX_DDC_ERROR_CODE_BUSY = 0x03, /*!< DDC bus is busy */
	SI_TX_DDC_ERROR_CODE_TX_HW = 0x04,
	SI_TX_DDC_ERROR_CODE_LIM_EXCEED = 0x05,
    SI_TX_DDC_ERROR_CODE_FIFO_EMPTY = 0x06,
};

#endif  //#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#endif
