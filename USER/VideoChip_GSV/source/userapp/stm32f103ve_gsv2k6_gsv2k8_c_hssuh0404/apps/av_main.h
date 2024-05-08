/**
 * @file av_main.h
 *
 * @brief sample main entry for audio/video based software
 */

/**
 * @brief  sample main entry for audio/video based software
 * @return never return
 */
#ifndef __av_main_h
#define __av_main_h

#include "Datatype.h"

#include "Gsv_bsp.h"   /* this file includes low level i2c/uart/timer/etc. functions on customer platform */
#include "Gsv_kapi.h"  /* this file includes kernal APIs */
#include "av_uart_cmd.h" /* accept command */
#include "av_key_cmd.h" /* accept key */
#include "av_irda_cmd.h" /* accept ir */
#include "av_edid_manage.h" /* edid manage */
#include "av_event_handler.h" /* routing and event */

#if 0//GSV1K
#include "gsv1k_device.h"
#endif
#if 0//GSV2K0
#include "gsv2k0_device.h"
#endif
#if GSV2K1
#include "gsv2k1_device.h"
#endif
#if GSV2K2
#include "gsv2k2_device.h"
#endif
#if GSV2K6
#include "gsv2k6_device.h"
#endif
#if GSV2K8
#include "gsv2k8_device.h"
#endif
#if GSV5K1
#include "gsv5k1_device.h"
#endif
#if GSV2K11
#include "gsv2k11_device.h"
#endif

typedef enum _emRxForce420CscModeType_t_
{
    emCscMode_ToNone = 0,
    emCscMode_ToYUV444,
    emCscMode_ToYUV422,
    emCscMode_ToRGB,

    emCscMode_Max
}emRxForce420CscModeType_t;

typedef enum _emTxForce420CscModeType_t_
{
    emTxCscMode_ToNone = 0,
    emTxCscMode_To420,
    emTxCscMode_ToOnlyCD8Bit,

    emTxCscMode_Max
}emTxForce420CscModeType_t;

typedef enum _emTxHdmiColorSpaceType_t_
{
    emTxCsType_RGB = 0,
    emTxCsType_YCBCR444,
    emTxCsType_YCBCR422,

    emTxCsType_Max
}emTxHdmiColorSpaceType_t;

#if GSV2K11
typedef struct _stGsvParamChange_T_
{
    u8 u8TxFormatChange;
    u8 u8TxHdmiMode;
    u8 u8TxHpdSta;
    u8 u8TxHdcpChangeFlag;
    u8 u8TxHdcpSta;
    u8 u8TxTimingChangeFlag;
    u8 u8TxTimingID;
    u8 u8TxColorSpaceChangeFlag;
    u8 u8TxColorSpace;

}stGsvParamChange_T;

typedef struct _stGsv2K11ColorSpace_T_
{
    AvVideoCd     Cd;
    AvVideoY      Y;
    AvVideoCs     InCs;
    AvVideoCs     OutCs;
}stGsv2K11ColorSpace_T;
#endif

#if GSV2K6
extern AvDevice AvGsv2k6_devices[AvGsv2k6DeviceNumber];
extern Gsv2k6Device gsv2k6_device[AvGsv2k6DeviceNumber];
extern AvPort gsv2k6Ports[AvGsv2k6DeviceNumber][AvGsv2k6PortNumber];
extern bool g_bGsv2k6HpdForceChange[AvGsv2k6DeviceNumber][2];
extern uint8 g_u8Gsv2k6StartIndex;
#endif

#if GSV2K8
extern AvDevice AvGsv2k8_devices[AvGsv2k8DeviceNumber];
extern Gsv2k8Device gsv2k8_device[AvGsv2k8DeviceNumber];
extern AvPort gsv2k8Ports[AvGsv2k8DeviceNumber][AvGsv2k8PortNumber];
#if AvGsv2k8VideoRgb_422_444To420
extern bool g_bGsv2k8TxChange[AvGsv2k8DeviceNumber][4];
extern uint8 g_u8Gsv2k8ForceCscMode[AvGsv2k8DeviceNumber][4];
extern stAvInfoUartSendMessage g_stTxAvInfo[AvGsv2k8DeviceNumber];
#endif
extern bool g_bGsv2k8HpdForceChange[AvGsv2k8DeviceNumber][4];
extern uint8 g_u8Gsv2k8StartIndex;
#endif

#if GSV2K2
extern AvDevice AvGsv2k2_devices[AvGsv2k2DeviceNumber];
extern Gsv2k2Device gsv2k2_device[AvGsv2k2DeviceNumber];
extern AvPort gsv2k2Ports[AvGsv2k2DeviceNumber][AvGsv2k2PortNumber];
#if AvGsv2k2Video420ToRgb_422_444
extern bool g_bGsv2k2InputIsChange[AvGsv2k2DeviceNumber][2];
extern uint8 g_u8Gsv2k2ForceCscMode[AvGsv2k2DeviceNumber][2];
extern stAvInfoUartSendMessage g_stRxAvInfo[AvGsv2k2DeviceNumber];
#endif
extern bool g_bGsv2k2HpdForceChange[AvGsv2k2DeviceNumber][2];
extern uint8 g_u8Gsv2k2StartIndex;
#endif
#if GSV2K1
extern AvDevice AvGsv2k1_devices[AvGsv2k1DeviceNumber];
extern Gsv2k1Device gsv2k1_device[AvGsv2k1DeviceNumber];
extern AvPort gsv2k1Ports[AvGsv2k1DeviceNumber][AvGsv2k1PortNumber];
extern bool g_bGsv2k1HpdForceChange[AvGsv2k1DeviceNumber][1];
extern uint8 g_u8Gsv2k1StartIndex;
extern bool g_bGsv2k1InitOk[AvGsv2k1DeviceNumber];
#endif
#if GSV2K11
extern AvDevice AvGsv2k11_devices[AvGsv2k11DeviceNumber];
extern Gsv2k11Device gsv2k11_device[AvGsv2k11DeviceNumber];
extern AvPort gsv2k11Ports[AvGsv2k11DeviceNumber][AvGsv2k11PortNumber];
extern bool g_bGsv2k11HpdForceChange[AvGsv2k11DeviceNumber][1];
extern bool g_bGsv2k11InitOk[AvGsv2k11DeviceNumber];
extern bool g_bGsvStandbyFlag[AvGsv2k11DeviceNumber];
extern bool g_bPreGsvStbFlag[AvGsv2k11DeviceNumber];
extern stGsvParamChange_T g_stGsv2k11ParamChg[AvGsv2k11DeviceNumber];
extern const stTimingTable_t res_table[RES_DEFAULT_MAX];
extern uint8 g_u8Gsv2k11StartIndex;
extern stGsv2K11ColorSpace_T g_stGsv2k11HdmiOutColorSpace[AvGsv2k11DeviceNumber];
#endif
#if GSV5K1
extern uint8 TrxAvUpStreamFlag; /* 1 = Tx, 0 = Rx */
//extern uint8 TrxAvEnableDetailTiming; /* 1 = Tx, 0 = Rx */
extern AvDevice AvGsv5k1_devices[AvGsv5k1DeviceNumber];
extern Gsv5k1Device gsv5k1_device[AvGsv5k1DeviceNumber];
extern AvPort gsv5k1Ports[AvGsv5k1DeviceNumber][AvGsv5k1PortNumber];
extern bool g_bGsv5k1HpdForceChange[AvGsv5k1DeviceNumber][1];
extern bool g_bGsv5k1InitOk[AvGsv5k1DeviceNumber];
extern uint8 g_u8Gsv5k1StartIndex;
#endif

extern u8 g_u8ChipDev;
extern u8 g_u8GsvChipType[GsvTotalDevNumber];

void Gsv_Init(void);
int GsvMain(void);
void mapp_GsvPowerManage(void);
void mapp_GsvHdcpManage(AvPort *port, u8 u8HdcpType);
u8 mapp_GsvGetRxHdcpVer(AvPort *port);
void mapp_GsvTxTmdsMute(AvPort *port,bool bMute);

#if GSV2K11
void GSV_GetTxStaFunction(AvPort *port);
void Gsv_StandbyProcess(u8 u8ID);
void mapp_GsvSetStandby(u8 u8ID,bool bOn);
bool mapp_GsvGetStandby(u8 u8ID);
void mapp_GsvSetTimingChange(u8 u8ID);
void mapp_GsvSetTimingID(u8 u8ID,u8 u8ResID);
void GsvSetTxHdcpStatus(u8 u8ID,u8 u8TxMode);
void GsvSetTxMode(u8 u8ID,u8 u8TxMode);
void GsvParamChangeProcess(u8 u8ID);
void mapp_GsvVideoInTxOutputTimingConfig(AvPort *port,u8 u8TimingID);
void mapp_GsvTimingConfig(u8 u8ChipID,u8 u8TimingID);
u8 GsvGetTxHpdStatus(u8 u8ID);
void mapp_GsvGetVideoOutputTiming(AvPort *port,pstTimingTable_t pstRxTiming);
#endif

#endif
