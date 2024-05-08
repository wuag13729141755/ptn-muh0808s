#ifndef __ITE_COMM_API_H_
#define __ITE_COMM_API_H_
#if(IT663XX_VER != IT663XX_NONE)
#include "../USER/video_chip/it663xx_source_h_file.h"
#include "../USER/video_chip/it663xx_customer_h_file.h"
#endif

#ifndef EDID_START_ADDR_SAME_WITH_BOOT
#define EDID_START_ADDR_SAME_WITH_BOOT			0x08038000
#endif

#ifndef SIZE_UART_KEY_VALUE_MAX
#define SIZE_UART_KEY_VALUE_MAX						80
#endif
#ifndef SIZE_PRJ_NAME_CHARS_MAX
#define SIZE_PRJ_NAME_CHARS_MAX						60
#endif

typedef enum
{
    etKEY_Detect,
    #if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
    etKEY_CobomTime,
    #endif
    #if _ENABLE_USER_EDID_LEARNING_FUNCTION
    etUserEdidReceiveTime,
    #endif
    #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
    etUserMsgTimer0,// = 3,
    etUserMsgTimerLast = etUserMsgTimer0+uart_study_key_type_max-1,
    #endif
    #if _ENABLE_SYSTEM_RESET_DELAY_TIME
    etSysResetTimer,
    #endif
    #if _ENABLE_STARTUP_TO_MATCHING_CHIPID
    etSysChipIDErrFuncTimer,
    #endif
    #if _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION
    etUser_CecInitTimer,
    #endif
    #if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
    etUser_SwitchPortTimer,
    #endif
    #if _ENABLE_START_UP_TO_ENABLE_EXTERNAL_AUDIO
    etUser_StartupAudioTimer,
    #endif
	#if  _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD  /*Ìí¼Ó2019.2.14*/
	etDetectDisplayTimer,
	#endif          /*Ìí¼Ó2019.2.14*/
    #if _ENABLE_AUDIO_SOURCE_SWITCH_TIMER
    etUser_AudioSrcSwitchTimer,
    #endif
    #if _ENABLE_IT6615_STARTUP_TODO_HDCP_TIMER
    etUser_It6615StartupTimer,
    #endif
    #if _ENABLE_SWITCH_LED_TIMER
    etUser_SwitchLedTimer,
    #endif
    #if _ENABLE_WIN_CONFIG_UPD_TIMER
    etUser_WinCfgUpdTimer,
    #endif
    etINVALID,  // not remove

}eTIMER;

typedef enum _tx_hdcp_force_t
{
	tx_hdcp_force_off = 0x00,
	tx_hdcp_force_1_4,
	tx_hdcp_force_2_2,
	tx_hdcp_force_bypass,
    #if _ENABLE_HDCP_AUTO_BY_SINK
    tx_hdcp_force_bysink,
    #endif

	tx_hdcp_force_max
}tx_hdcp_force_t;

typedef __packed struct _stHdmiSwitchParam_t_
{
    #if _ENABLE_MULTI_OUTPUT_CHIP
	bool 		bAutoSwitch[Def_outport_num];
    uint8_t     u8SwChannel[Def_outport_num];
    #else
	bool 		bAutoSwitch;
	uint8_t 	u8SwChannel;
    #endif

    #if _ENABLE_AUTO_TURN_TYPE
	eAutoTurnType_t			eAutoTurn;
    #endif
	uint8_t 	u8CheckSum;
}stHdmiSwitchParam_t, *pstHdmiSwitchParam_t;

typedef __packed struct _stUartKeyValue_t_
{
    #if(_ENABLE_UART_KEY_VALUE_MAX_BUG == _MACRO_ENABLE)
	char 		sKeyValueBuffer[SIZE_UART_KEY_VALUE_MAX+1];
    #else
	char 		sKeyValueBuffer[SIZE_UART_KEY_VALUE_MAX];
    #endif
	uint16_t 	u16KeyValueLen;
	uint32_t 	u32BaudRate;
    bool        bUartValueHexOrAsciiFlag;  // TRUE: HEX ; FALSE: ASCII
    u16         u16UartValueSendDelayTime;
    u8          u8UartValueSendMessageNum;
    u16         u16UartValueCounterDelayTime;
    #if _ENABLE_UART_LEARN_TO_MULTI_FUNCTION
    u8          u8SendPort;     // add by wf8421 20190819
    u8          u8SendWhoFollow;     // add by wf8421 20190822
    #endif
	uint8_t 	u8CheckSum;
}stUartKeyValue_t, *pstUartKeyValue_t;

typedef __packed struct _stHdcpStatusOutPort_t_
{
	uint8_t 	u8HdcpSta[_HDMI_O_PORT_NUM];
	uint8_t 	u8CheckSum;
}stHdcpStatusOutPort_t, *pstHdcpStatusOutPort_t;

typedef __packed struct _stEdidSelIndex_t_
{
	uint8_t 	u8EdidFlashIndex;
    #if _ENABLE_SAVE_CURRENT_EDID
    uint8_t     u8CurEdidbuf[256];
    #endif
    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
	uint8_t 	u8EdidChannelMode[_HDMI_INPUT_PORT_USED_NUMBER];
    #endif
	uint8_t 	u8CheckSum;
}stEdidSelIndex_t, *pstEdidSelIndex_t;

typedef __packed struct _stProjectNameSet_t_
{
	char 		sPrjName[SIZE_PRJ_NAME_CHARS_MAX];
    uint8_t     m_u8Len;
	uint8_t 	u8CheckSum;
}stProjectNameSet_t, *pstProjectNameSet_t;

typedef struct _stPrjSpecVarVector_t_
{
	void *					pvSpecVar;
	uint16_t				u16SpecVarSize;
}stPrjSpecVarVector_t, *pstPrjSpecVarVector_t;

typedef struct _stProjectGlobalSaveVar_t_
{
	stHdmiSwitchParam_t		stHdmiSel;
#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
	stIrKeyWave_t			stIrStudyKeyData[_NUM_IR_STUDY_KEYS];
#endif //add by wf8421
    #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
	stUartKeyValue_t		stUartStudyKeyData[_NUM_UART_STUDY_KEYS];
    #endif
	stHdcpStatusOutPort_t	stHdcpStatusOp;
	stEdidSelIndex_t		stEdidSelIndex;
	stProjectNameSet_t		stPrjNameSet;

	stPrjSpecVarVector_t	stSaveSpecVar;//The project specific var one should be the last one
}stProjectGlobalSaveVar_t, *pstProjectGlobalSaveVar_t;

typedef struct _stProjectGlobalAllVar_t_
{
	stProjectGlobalSaveVar_t	stPrjSaveVar;
}stProjectGlobalAllVar_t, *pstProjectGlobalAllVar_t;

typedef enum _eIT663xxGetEdidSrc_t_
{
	eIT663xxGetEdidFrom_Tx = 0,
	eIT663xxGetEdidFrom_Buffer,

	eIT663xxGetEdidFrom_Max,
}eIT663xxGetEdidSrc_t;

typedef struct
{
    uint16_t u16Year;
    uint8_t  u8Month;
    uint8_t  u8Date;
    uint8_t  u8Hour;
    uint8_t  u8minute;
    uint8_t  u8Second;
}stVersionType;

extern iTE_u8  			*System_Default_Edid_table;
extern iTE_u8			g_pu8It663xxEdidBuffer[256];
extern uint8_t *g_edid_tabel_user_def;
#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
extern iTE_u8   g_u8SingleChannelEdidBuffer[_HDMI_INPUT_PORT_USED_NUMBER][256];
#endif
extern const unsigned char DataStr[];
extern const unsigned char TimeStr[];
extern stVersionType g_stTestVersion;
extern stProjectGlobalAllVar_t g_stPrjGloVar;

uint8_t CRC8Calculate(void *pBuf ,unsigned pBufSize);
iTE_u8 CalEdidCheckSum(iTE_u8 edid[],iTE_u8 block);
bool is_edid_valid(iTE_u8 *edid_table);
#if (FUNCTION_MACRO_EDID_MODIFY_TO_STANDARD == _MACRO_ENABLE)
void ModifyStandard4kEdid(unsigned char *edid_table);
#endif
bool is_edid_compare_same(iTE_pu8 arry1,iTE_pu8 arry2);
void LimitedVSDBMaxPixel(uint8_t* pEdidBlk1, uint8_t MaxPixelC);
void SetAutoSwDetectTime(uint32_t u32SetTime);
void AutoSwDetectTime1MsLoop(void);
bool PollAutoSwDetectStatus(void);
#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
eIT663xxGetEdidSrc_t GetIT663xxEdidSource(uint8_t u8ChipIndex,u8 u8RxPort);
void SetIT663xxEdidSource(uint8_t u8ChipIndex,u8 u8RxPort, eIT663xxGetEdidSrc_t eEdidSrc);
#else
eIT663xxGetEdidSrc_t GetIT663xxEdidSource(uint8_t u8ChipIndex);
void SetIT663xxEdidSource(uint8_t u8ChipIndex, eIT663xxGetEdidSrc_t eEdidSrc);
#endif
void FillIT663xxEdidData(iTE_u8 *pBuffer);
#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
void FillIT663xxEdidDataForSingleChannel(iTE_u8 *pBuffer,u8 u8InPort);
void GetIT663xxEdidDataForSingleChannel(iTE_u8 *pu8GetBuffer,u8 u8InPort);
#endif

bool isRxHpdDetNull(uint8_t u8RxIndex);
bool isGetHpdDetConnect(uint8_t u8RxIndex);

void mapp_UserData_HdmiSelInit(void);
bool mapp_UserData_HdmiSelCheck(void);
#if 1//_ENABLE_MULTI_OUTPUT_CHIP
void SetUserData_HdmiSel(void);
#else
void SetUserData_HdmiSel(uint8_t HdmiRxSel, bool isAutoMode);
#endif

void mapp_UserDataUartKeyValInit(u8 u8Index);
bool mapp_UserDataUartKeyValCheck(void);
void SetUserData_UartKeyVal(uint8_t u8KeyIndex);
void SetUserData_UartKeyValSaveAll(void);

void mapp_UserDataIrStudyValInit(u8 u8Index);
bool mapp_UserDataIrStudyValCheck(void);
void SetUserData_IrStudyVal(uint8_t u8KeyIndex, bool bIsr);
void SetUserData_IrStudyValSaveAll(bool bIsr);

void mapp_UserDataEdidIndexInit(void);
bool mapp_UserDataEdidIndexCheck(void);
void SetUserData_EdidIndexSave(void);

void mapp_UserDataProjectNameInit(void);
bool mapp_UserDataProjectNameCheck(void);
void SetUserData_ProjectName(char *strPrjName, uint8_t len);

void mapp_UserDataHdcpInit(void);
bool mapp_UserDataHdcpCheck(void);
void SetUserData_HDCP(void);

#if 1
bool mapp_EdidLimitTo4K30(u8 *u8pInEdid,
                                    u8 *u8pOutEdid,
                                    u16 u16len,
                                    bool bEnInterlace,
                                    bool bOnlyPcm20);
bool mapp_EdidLimitTo300M_Block0(u8 *u8pInEdid,
                                    u8 *u8pOutEdid,
                                    u16 u16len,
                                    bool bIsLimitTiming);
bool mapp_EdidLimitTo300M_Block1(u8 *u8pInEdid,
                                    u8 *u8pOutEdid,
                                    u16 u16len,
                                    bool bRemoveVDB6G,
                                    bool bEnInterlace,
                                    bool bOnlyPcm20,
                                    bool bRemove420);
bool mapp_EdidLimitTo1080p60(u8 *u8pInEdid,
                                    u8 *u8pOutEdid,
                                    u16 u16len);
#endif
u16 mapp_GetEdidPhysicalAddress(u8 *pu8EdidBuf);
bool mapp_CopyEdidPhysicalAddress(u8 *pu8InEdidBuf,u8 *pu8OutEdidBuf);

u8 HexAndAsciiChange(u8 cvalue,u8 cmode);
bool isNumberCmd(char *cmd, uint16_t len);

u32 PlatformTimerGet(uint8_t eTimer);
void PlatformTimerSet( uint8_t index, u32 m_sec );
void PlatformTimerInit (void);
void PlatformTimerTask(void);
uint8_t PlatformTimerExpired( uint8_t timer );

char *my_StrStr(char *inBuffer,char *inSerchStr);
char *my_CaseStrStr(char *inBuffer,char *inSerchStr);
char *FindChar(char *buf,char Taildata,uint16_t Bitwide);
unsigned short StrToNum(char *Str,unsigned char cursor);
unsigned short NumToStr(char* buf,char* Str,unsigned char member);
short GetNumofStr(char *buf,char Taildata,unsigned char Bitwide);
unsigned char GetLenOfStr(char *buf,char Taildata,unsigned char Bitwide);
void bubble_sort(u8* a, u8 n,u8 mode);
void mapp_RevisionMake(void);
bool buffer_compare(pu8 arry1,pu8 arry2,u16 u16Len);
u16 U16ToU8MsbCovert(pu16 arry1,pu8 arry2,u16 u16Len);
u16 U8ToU16MsbCovert(pu8 arry1,pu16 arry2,u16 u16Len);

#endif


































