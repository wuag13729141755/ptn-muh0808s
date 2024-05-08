#ifndef __ITE6664_COMM_API_H_
#define __ITE6664_COMM_API_H_

#define I2C_SMBusE						(iTE_u8)_I2C_TABLE_INDEX_IT666X_0
#define I2C_SMBusF						(iTE_u8)_I2C_TABLE_INDEX_IT666X_1

#define IT6664_EDID_RETRY_TIMES					2

#define IT6664_RX_EDID_UPDATE_AFTER_TIME		600//unit is 5ms

#define IT6664_Initial							it6664

#define IT6664_REMOVE_WARNING_EN		_MACRO_ENABLE

typedef enum _edid_copy_from_t_
{

	edid_copy_from_none=0,

	edid_copy_from_lowest,
	edid_copy_from_highest,
	edid_copy_from_output,
	edid_copy_from_eeprom,

	edid_copy_from_max,
}edid_copy_from_t;

#if (USING_1to8==_MACRO_ENABLE)
extern EDIDBlock0 EDID0data[8];
extern EDIDBlock1 EDID1data[8];
extern iTE_u8 HPD[8];
#elif (_ENABLE_IT666X_MULTI_CHIP_USE == _MACRO_ENABLE)
extern EDIDBlock0 EDID0data[NUM_OF_IT666X][4];//0x6
extern EDIDBlock1 EDID1data[NUM_OF_IT666X][4];//0x4a
//extern iTE_u8 SupportVesaDTD[NUM_OF_IT666X][4];
extern iTE_u8 HPD[NUM_OF_IT666X][4];
#else
extern EDIDBlock0 EDID0data[4];
extern EDIDBlock1 EDID1data[4];
extern iTE_u8 HPD[4];
#endif
extern iTE_u8 it6664_u8I2CAdr;
extern iTE_u8 Compose_block0[128];
extern iTE_u8 Compose_block1[128];
extern iTE_u8 Device_off;
extern uint16_t g_update_rx_edid_cnt;

#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
extern uint16_t g_cpt_ds_manu_sw_cnt;//detect signal after this time when manual switch
#endif

void IT6664_MainLoop(void);

#if(USING_ITE_DEMO_CODE == _MACRO_DISABLE)
void it6664_my_Edid_Copy_Lowest(void);
void it6664_my_Edid_Copy_Highest(void);
void it6664_my_Edid_Copy_Output(iTE_u8 port);
void it6664_my_Edid_Copy_Eeprom(iTE_u8 port);
void it6664_my_Default_Edid(void);
bool it6664_my_Edid_Copy_Port(uint8_t iport);
#endif//#if(USING_ITE_DEMO_CODE == _MACRO_DISABLE)
void it6664_edid_update(void);
void it6664_led_work(void);
void IT6664_SetTx5VOnOff(bool on_flag);
#if _ENABLE_IT666X_READ_TX_EDID
typedef u8 (*pu8It666xGetHdcpSta)(u8);
extern pu8It666xGetHdcpSta pIt666xGetHdcpStaFunc;
void it666X_ReadTxEdidToBuffer(void);
void it666X_SetReadTxEdidFlag(u8 u8Port,bool bEn);
bool it666X_GetReadTxEdidFlag(u8 u8Port);
void it666X_GetTxPortEdid(u8 u8Port,u16 u16Len,u8 *pu8Edid);
bool it666X_GetTxHpdSta(u8 u8TxPort);
void it666x_TxEdidCopy(u8 u8TxPort,u8 *pu8EdidBlk0,u8 *pu8EdidBlk1);
#endif
#if _ENABLE_IT666X_FORCE_SET_OUTPUT_HDCP
void it666X_SetHdcpMode(u8 u8TxPort,u8 u8HdcpMode);
#endif
#if _ENABLE_IT666X_MANUAL_TO_TX_VIDEO_RESET
void it666X_SetVideoReset(u8 u8DevId);
#endif

#endif




























