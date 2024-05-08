#ifndef __TASK_CEC_H_
#define __TASK_CEC_H_

/* System Tick prescaler computation */
/* Start bit timings (based SysTick end of count event each 100?)  */
#define Sbit_Nom_LD    37 /* Start Bit Nominal Low duration: 37 x 100? = 3.7ms */
#define Sbit_Nom_HD    8  /* Start Bit Nominal High Duration: 8 x 100? = 0.8ms */
#define Sbit_Min_LD    35 /* Start Bit Minimum Low duration: 35 x 100? = 3.5ms */
#define Sbit_Max_LD    39 /* Start Bit Maximum Low duration: 39 x 100? = 3.9ms */
#define Sbit_Min_TD    43 /* Start Bit Minimum Total duration: 43 x 100? = 4.3ms */
#define Sbit_Max_TD    47 /* Start Bit Maximum Total duration: 47 x 100? = 4.7ms */

/* Data bit logical "0" timings  */
#define Dbit0_Nom_LD   15 /* Data Bit "0" Nominal Low duration: 15 x 100? = 1.5ms */
#define Dbit0_Nom_HD   9  /* Data Bit "0" Nominal High Duration: 9 x 100? = 0.9ms */
#define Dbit0_Min_LD   13 /* Data Bit "0" Minimum Low duration: 13 x 100? = 1.3ms */
#define Dbit0_Max_LD   17 /* Data Bit "0" Maximum Low duration: 17 x 100? = 1.7ms */

/* Data bit logical "1" timings  */
#define Dbit1_Nom_LD   6  /* Data Bit (logical "1") Nominal Low duration: 0.6ms  */
#define Dbit1_Nom_HD   18 /* Data Bit (logical "1") Nominal High Duration: 1.8ms */
#define Dbit1_Min_LD   4  /* Data Bit "1" Minimum Low duration: 0.4ms */ //修改HDBT的时候为3，HDMI4
#define Dbit1_Max_LD   8  /* Data Bit "1" Maximum Low duration: 0.8ms */

/* Data bit duration  */
#define DbitX_Min_TD   20 /* Data Bit Minimum Total duration: 2ms   */
#define DbitX_Max_TD   27 /* Data Bit Maximum Total duration: 2.7ms */

//CEC MESSAGE RX BUFFER LENGTH
#define CEC_MSG_RX_BUF_LEN		20

//CEC ADDRESS
#define CEC_ADDR_LOCAL_IN		0x00
#define CEC_ADDR_LOCAL_OUT		0x04
#define CEC_ADDR_PHYSICAL_IN	0x0000
#define CEC_ADDR_PHYSICAL_OUT	0x1000

//OSD NAME
#define CEC_OSD_NAME			"UHD"

#define SIZE_OF_CEC_OPERAND		16

/* CEC define */
typedef enum _CecLogicalAddresses_t
{
    CEC_LOGADDR_TV          	= 0x00,
    CEC_LOGADDR_RECDEV1     	= 0x01,
    CEC_LOGADDR_RECDEV2         = 0x02,
    CEC_LOGADDR_TUNER1          = 0x03,     // STB1 in Spev v1.3
    CEC_LOGADDR_PLAYBACK1       = 0x04,     // DVD1 in Spev v1.3
    CEC_LOGADDR_AUDSYS          = 0x05,
    CEC_LOGADDR_PURE_SWITCH     = 0x06,     // STB2 in Spec v1.3
    CEC_LOGADDR_VIDEO_PROCESSOR = 0x07,     // STB3 in Spec v1.3
    CEC_LOGADDR_PLAYBACK2   	= 0x08,     // DVD2 in Spec v1.3
    CEC_LOGADDR_RECDEV3    	 	= 0x09,
    CEC_LOGADDR_TUNER4      	= 0x0A,     // RES1 in Spec v1.3
    CEC_LOGADDR_PLAYBACK3   	= 0x0B,     // RES2 in Spec v1.3
    CEC_LOGADDR_RES3        	= 0x0C,
    CEC_LOGADDR_RES4        	= 0x0D,
    CEC_LOGADDR_FREEUSE     	= 0x0E,
    CEC_LOGADDR_BROADCAST   	= 0x0F

} CecLogicalAddresses_t;

#if 0
typedef enum _CecDeviceTypes_t   // CEC device types
{
    CEC_DT_TV,
    CEC_DT_RECORDING_DEVICE,
    CEC_DT_RSVD,
    CEC_DT_TUNER,
    CEC_DT_PLAYBACK,
    CEC_DT_AUDIO_SYSTEM,
    CEC_DT_PURE_CEC_SWITCH,
    CEC_DT_VIDEO_PROCESSOR,

    CEC_DT_COUNT
} CecDeviceTypes_t;
#endif

typedef enum _CecOpcodes_t       // CEC Messages
{
    CECOP_FEATURE_ABORT             = 0x00,
    CECOP_IMAGE_VIEW_ON             = 0x04,
    CECOP_TUNER_STEP_INCREMENT      = 0x05,     // N/A
    CECOP_TUNER_STEP_DECREMENT      = 0x06,     // N/A
    CECOP_TUNER_DEVICE_STATUS       = 0x07,     // N/A
    CECOP_GIVE_TUNER_DEVICE_STATUS  = 0x08,     // N/A
    CECOP_RECORD_ON                 = 0x09,     // N/A
    CECOP_RECORD_STATUS             = 0x0A,     // N/A
    CECOP_RECORD_OFF                = 0x0B,     // N/A
    CECOP_TEXT_VIEW_ON              = 0x0D,
    CECOP_RECORD_TV_SCREEN          = 0x0F,     // N/A
    CECOP_GIVE_DECK_STATUS          = 0x1A,
    CECOP_DECK_STATUS               = 0x1B,
    CECOP_SET_MENU_LANGUAGE         = 0x32,
    CECOP_CLEAR_ANALOGUE_TIMER      = 0x33,     // Spec 1.3A
    CECOP_SET_ANALOGUE_TIMER        = 0x34,     // Spec 1.3A
    CECOP_TIMER_STATUS              = 0x35,     // Spec 1.3A
    CECOP_STANDBY                   = 0x36,
    CECOP_PLAY                      = 0x41,
    CECOP_DECK_CONTROL              = 0x42,
    CECOP_TIMER_CLEARED_STATUS      = 0x43,     // Spec 1.3A
    CECOP_USER_CONTROL_PRESSED      = 0x44,
    CECOP_USER_CONTROL_RELEASED     = 0x45,
    CECOP_GIVE_OSD_NAME             = 0x46,
    CECOP_SET_OSD_NAME              = 0x47,
    CECOP_SET_OSD_STRING            = 0x64,
    CECOP_SET_TIMER_PROGRAM_TITLE   = 0x67,     // Spec 1.3A
    CECOP_SYSTEM_AUDIO_MODE_REQUEST = 0x70,     // Spec 1.3A
    CECOP_GIVE_AUDIO_STATUS         = 0x71,     // Spec 1.3A
    CECOP_SET_SYSTEM_AUDIO_MODE     = 0x72,     // Spec 1.3A
    CECOP_REPORT_AUDIO_STATUS       = 0x7A,     // Spec 1.3A
    CECOP_GIVE_SYSTEM_AUDIO_MODE_STATUS = 0x7D, // Spec 1.3A
    CECOP_SYSTEM_AUDIO_MODE_STATUS  = 0x7E,     // Spec 1.3A
    CECOP_ROUTING_CHANGE            = 0x80,
    CECOP_ROUTING_INFORMATION       = 0x81,
    CECOP_ACTIVE_SOURCE             = 0x82,
    CECOP_GIVE_PHYSICAL_ADDRESS     = 0x83,
    CECOP_REPORT_PHYSICAL_ADDRESS   = 0x84,
    CECOP_REQUEST_ACTIVE_SOURCE     = 0x85,
    CECOP_SET_STREAM_PATH           = 0x86,
    CECOP_DEVICE_VENDOR_ID          = 0x87,
    CECOP_VENDOR_COMMAND            = 0x89,
    CECOP_VENDOR_REMOTE_BUTTON_DOWN = 0x8A,
    CECOP_VENDOR_REMOTE_BUTTON_UP   = 0x8B,
    CECOP_GIVE_DEVICE_VENDOR_ID     = 0x8C,
    CECOP_MENU_REQUEST              = 0x8D,
    CECOP_MENU_STATUS               = 0x8E,
    CECOP_GIVE_DEVICE_POWER_STATUS  = 0x8F,
    CECOP_REPORT_POWER_STATUS       = 0x90,
    CECOP_GET_MENU_LANGUAGE         = 0x91,
    CECOP_SELECT_ANALOGUE_SERVICE   = 0x92,     // Spec 1.3A    N/A
    CECOP_SELECT_DIGITAL_SERVICE    = 0x93,     //              N/A
    CECOP_SET_DIGITAL_TIMER         = 0x97,     // Spec 1.3A
    CECOP_CLEAR_DIGITAL_TIMER       = 0x99,     // Spec 1.3A
    CECOP_SET_AUDIO_RATE            = 0x9A,     // Spec 1.3A
    CECOP_INACTIVE_SOURCE           = 0x9D,     // Spec 1.3A
    CECOP_CEC_VERSION               = 0x9E,     // Spec 1.3A
    CECOP_GET_CEC_VERSION           = 0x9F,     // Spec 1.3A
    CECOP_VENDOR_COMMAND_WITH_ID    = 0xA0,     // Spec 1.3A
    CECOP_CLEAR_EXTERNAL_TIMER      = 0xA1,     // Spec 1.3A
    CECOP_SET_EXTERNAL_TIMER        = 0xA2,     // Spec 1.3A

    CECOP_REPORT_SHORT_AUDIO    	= 0xA3,     // Spec 1.4
    CECOP_REQUEST_SHORT_AUDIO    	= 0xA4,     // Spec 1.4

    CECOP_GIVE_FEATURES             = 0xA5,     // Spec 1.4
    CECOP_REPORT_FEATURES           = 0xA6,     // Spec 1.4

    CECOP_INITIATE_ARC              = 0xC0,
    CECOP_REPORT_ARC_INITIATED      = 0xC1,
    CECOP_REPORT_ARC_TERMINATED     = 0xC2,

    CECOP_REQUEST_ARC_INITIATION    = 0xC3,
    CECOP_REQUEST_ARC_TERMINATION   = 0xC4,
    CECOP_TERMINATE_ARC             = 0xC5,

    CDCOP_HEADER                    = 0xF8,
    CECOP_ABORT                     = 0xFF,
} CecOpcodes_t;

typedef enum _CecPowerstatus_t   // Operands for <Power Status> Opcode
{
    CEC_POWERSTATUS_ON              = 0x00,
    CEC_POWERSTATUS_STANDBY         = 0x01,
    CEC_POWERSTATUS_STANDBY_TO_ON   = 0x02,
    CEC_POWERSTATUS_ON_TO_STANDBY   = 0x03,
} CecPowerstatus_t;

#if 0
typedef enum _CecDevState_t
{
    CEC_DEVSTATE_ON                 = 1,
    CEC_DEVSTATE_STANDBY            = 2,
    CEC_DEVSTATE_OFF                = 3,
    CEC_DEVSTATE_IMAGE_DISPLAY      = 4,
    CEC_DEVSTATE_MENU_DISPLAY       = 5,
    CEC_DEVSTATE_TEXT_DISPLAY       = 6,
    CEC_DEVSTATE_MENU_ACTIVE        = 7,
    CEC_DEVSTATE_MENU_INACTIVE      = 8,
    CEC_DEVSTATE_RECORDING          = 9,
    CEC_DEVSTATE_NOT_RECORDING      = 10,
    CEC_DEVSTATE_DECK_ACTIVE        = 11,
    CEC_DEVSTATE_DECK_INACTIVE      = 12
} CecDevState_t;

typedef enum _CecAbortReason_t   // Operands for <Feature Abort> Opcode
{
    CECAR_UNRECOG_OPCODE            = 0x00,
    CECAR_NOT_CORRECT_MODE,
    CECAR_CANT_PROVIDE_SOURCE,
    CECAR_INVALID_OPERAND,
    CECAR_REFUSED,
    CECAR_UNABLE_TO_DETERMINE
} CecAbortReason_t;

typedef enum _CecDeckControl_t   // Operands for <Deck Control> Opcode
{
    CEC_DECKCONTROL_SKIP_FWD        = 0x01,
    CEC_DECKCONTROL_SKIP_BACK       = 0x02,
    CEC_DECKCONTROL_STOP            = 0x03,
    CEC_DECKCONTROL_EJECT           = 0x04
}CecDeckControl_t;

typedef enum _CecDeckStatus_t
{
        /* Operands for <Give Deck Status> Opcode   */

    CEC_DECKSTATUS_ON               = 0x01,
    CEC_DECKSTATUS_OFF              = 0x02,
    CEC_DECKSTATUS_ONCE             = 0x03,

        /* Operands for <Deck Status> Opcode        */

    CEC_DECKSTATUS_PLAY             = 0x11,
    CEC_DECKSTATUS_RECORD           = 0x12,
    CEC_DECKSTATUS_PLAY_REV         = 0x13,
    CEC_DECKSTATUS_STILL            = 0x14,
    CEC_DECKSTATUS_SLOW             = 0x15,
    CEC_DECKSTATUS_SLOW_REV         = 0x16,
    CEC_DECKSTATUS_SEARCH_FWD       = 0x17,
    CEC_DECKSTATUS_SEARCH_REV       = 0x18,
    CEC_DECKSTATUS_NO_MEDIA         = 0x19,
    CEC_DECKSTATUS_STOP             = 0x1A,
    CEC_DECKSTATUS_WIND             = 0x1B,
    CEC_DECKSTATUS_REWIND           = 0x1C,
    CEC_DECKSTATUS_INDEX_SEARCH_FWD = 0x1D,
    CEC_DECKSTATUS_INDEX_SEARCH_REV = 0x1E,
    CEC_DECKSTATUS_OTHER_STATUS     = 0x1F
}CecDeckStatus_t;


typedef enum _CecPlayModes_t     // Operand for <Play> Opcode
{
    CEC_PLAY_SCAN_FWD_MIN           = 0x05,
    CEC_PLAY_SCAN_FWD_MED           = 0x06,
    CEC_PLAY_SCAN_FWD_MAX           = 0x07,
    CEC_PLAY_SCAN_REV_MIN           = 0x09,
    CEC_PLAY_SCAN_REV_MED           = 0x0A,
    CEC_PLAY_SCAN_REV_MAX           = 0x0B,
    CEC_PLAY_SLOW_FWD_MIN           = 0x15,
    CEC_PLAY_SLOW_FWD_MED           = 0x16,
    CEC_PLAY_SLOW_FWD_MAX           = 0x17,
    CEC_PLAY_SLOW_REV_MIN           = 0x19,
    CEC_PLAY_SLOW_REV_MED           = 0x1A,
    CEC_PLAY_SLOW_REV_MAX           = 0x1B,
    CEC_PLAY_REVERSE                = 0x20,
    CEC_PLAY_FORWARD                = 0x24,
    CEC_PLAY_STILL                  = 0x25
} CecPlayModes_t;

typedef enum _CecMenuRequests_t
{
    CEC_MENUREQ_ACTIVATE            = 0x00,
    CEC_MENUREQ_DEACTIVATE          = 0x01,
    CEC_MENUREQ_QUERY               = 0x02
} CecMenuRequests_t;

typedef enum _CecMenuStates_t
{
    CEC_MENUSTATE_ACTIVATED         = 0x00,
    CEC_MENUSTATE_DEACTIVATED       = 0x01
} CecMenuStates_t;
#endif

typedef enum _CecUiCommand_t         // Remote Control Pass-through and UI command codes
{
    CEC_RC_SELECT                       = 0x00,
    CEC_RC_UP                           = 0x01,
    CEC_RC_DOWN                         = 0x02,
    CEC_RC_LEFT                         = 0x03,
    CEC_RC_RIGHT                        = 0x04,
    CEC_RC_RIGHT_UP                     = 0x05,
    CEC_RC_RIGHT_DOWN                   = 0x06,
    CEC_RC_LEFT_UP                      = 0x07,
    CEC_RC_LEFT_DOWN                    = 0x08,
    CEC_RC_ROOT_MENU                    = 0x09,
    CEC_RC_SETUP_MENU                   = 0x0A,
    CEC_RC_CONTENTS_MENU                = 0x0B,
    CEC_RC_FAVORITE_MENU                = 0x0C,
    CEC_RC_EXIT                         = 0x0D,
                                        // 0x0E - 0x1F Reserved
    CEC_RC_0                            = 0x20,
    CEC_RC_1                            = 0x21,
    CEC_RC_2                            = 0x22,
    CEC_RC_3                            = 0x23,
    CEC_RC_4                            = 0x24,
    CEC_RC_5                            = 0x25,
    CEC_RC_6                            = 0x26,
    CEC_RC_7                            = 0x27,
    CEC_RC_8                            = 0x28,
    CEC_RC_9                            = 0x29,
    CEC_RC_DOT                          = 0x2A,
    CEC_RC_ENTER                        = 0x2B,
    CEC_RC_CLEAR                        = 0x2C,
                                        // 0x2D - 0x2F Reserved
    CEC_RC_CHANNEL_UP                   = 0x30,
    CEC_RC_CHANNEL_DOWN                 = 0x31,
    CEC_RC_PREVIOUS_CHANNEL             = 0x32,
    CEC_RC_SOUND_SELECT                 = 0x33,
    CEC_RC_INPUT_SELECT                 = 0x34,
    CEC_RC_DISPLAY_INFORMATION          = 0x35,
    CEC_RC_HELP                         = 0x36,
    CEC_RC_PAGE_UP                      = 0x37,
    CEC_RC_PAGE_DOWN                    = 0x38,
                                        // 0x39 - 0x3F Reserved
    CEC_RC_POWER                        = 0x40,
    CEC_RC_VOLUME_UP                    = 0x41,
    CEC_RC_VOLUME_DOWN                  = 0x42,
    CEC_RC_MUTE                         = 0x43,
    CEC_RC_PLAY                         = 0x44,
    CEC_RC_STOP                         = 0x45,
    CEC_RC_PAUSE                        = 0x46,
    CEC_RC_RECORD                       = 0x47,
    CEC_RC_REWIND                       = 0x48,
    CEC_RC_FAST_FORWARD                 = 0x49,
    CEC_RC_EJECT                        = 0x4A,
    CEC_RC_FORWARD                      = 0x4B,
    CEC_RC_BACKWARD                     = 0x4C,
    CEC_RC_STOP_RECORD                  = 0x4D,
    CEC_RC_PAUSE_RECORD                 = 0x4E,
                                        // 0x4F Reserved
    CEC_RC_ANGLE                        = 0x50,
    CEC_RC_SUB_PICTURE                  = 0x51,
    CEC_RC_VIDEO_ON_DEMAND              = 0x52,
    CEC_RC_ELECTRONIC_PROGRAM_GUIDE     = 0x53,
    CEC_RC_TIMER_PGRMING                = 0x54,
    CEC_RC_INITIAL_CONFIGURATION        = 0x55,
    CEC_RC_SELECT_BROADCAST_TYPE        = 0x56,
    CEC_RC_SELECT_SOUND_PRESENTATION    = 0x57,
                                        // 0x58 - 0x5F Reserved
    CEC_RC_PLAY_FUNCTION                = 0x60,
    CEC_RC_PAUSE_PLAY_FUNCTION          = 0x61,
    CEC_RC_RECORD_FUNCTION              = 0x62,
    CEC_RC_PAUSE_RECORD_FUNCTION        = 0x63,
    CEC_RC_STOP_FUNCTION                = 0x64,
    CEC_RC_MUTE_FUNCTION                = 0x65,
    CEC_RC_RESTORE_VOLUME_FUNCTION      = 0x66,
    CEC_RC_TUNE_FUNCTION                = 0x67,
    CEC_RC_SELECT_DISK_FUNCTION         = 0x68,
    CEC_RC_SELECT_AV_INPUT_FUNCTION     = 0x69,
    CEC_RC_SELECT_AUDIO_INPUT_FUNCTION  = 0x6A,
    CEC_RC_POWER_TOGGLE_FUNCTION        = 0x6B,
    CEC_RC_POWER_OFF_FUNCTION           = 0x6C,
    CEC_RC_POWER_ON_FUNCTION            = 0x6D,
                                        // 0x6E - 0x70 Reserved
    CEC_RC_F1_BLUE                      = 0x71,
    CEC_RC_F2_RED                       = 0x72,
    CEC_RC_F3_GREEN                     = 0x73,
    CEC_RC_F4_YELLOW                    = 0x74,
    CEC_RC_F5                           = 0x75,
    CEC_RC_DATA                         = 0x76
                                        // 0x77 - 0x7F Reserved
} CecUiCommand_t;

typedef enum _CecAbortReason_t_
{
    CecAbortReason_UnrecognizedOpcode = 0,
    CecAbortReason_NotInCorrectModeToRespond,
    CecAbortReason_CannotProvideSource,
    CecAbortReason_InvalidOperand,
    CecAbortReason_Refused,
    CecAbortReason_UnableToDetermine,
}CecAbortReason_t;

typedef enum _CecVersion_t_
{
    CecVersion_Unused = 0x00,
    CecVersion_V1_3 = 0x04,
    CecVersion_V1_4 = 0x05,
    CecVersion_V2_0 = 0x06,
}CecVersion_t;

typedef enum _emCecRelinkState_t_
{
    // tx hdmi port
    emCecRelinkSta_None = 0,
    emCecRelinkSta_PollingMsg,
    emCecRelinkSta_SetPhyAddrMsg,
    emCecRelinkSta_SetOsdName,
    emCecRelinkSta_AudioInit,
    emCecRelinkSta_ArcInit,

    // rx hdmi port
    emCecRelinkSta_RxPollingMsg,
    emCecRelinkSta_GetPhyAddrMsg,
    emCecRelinkSta_GetOsdName,
    emCecRelinkSta_RequestArcInit,
    emCecRelinkSta_ReportArcInit,
    emCecRelinkSta_GetCecVersion,

    emCecRelinkSta_UserControlReleased,

    emCecRelinkSta_Max,
}emCecRelinkState_t;

#define CEC_OPERATE_OUTPUT			0xFE
#define CEC_OPERATE_INPUT			0xFF

typedef struct _stCecMsg_t_
{
	uint8_t ucHeader;
	uint8_t ucOpCode;
	uint8_t ucOperand[SIZE_OF_CEC_OPERAND];
	uint8_t ucLen;
}stCecMsg_t, *pstCecMsg_t;

typedef struct _stCecTxMsg_t_
{
	uint8_t 	cecPort;
	stCecMsg_t 	msgContent[NUM_OF_CEC_PINS_MAX];
}stCecTxMsg_t, *pstCecTxMsg_t;

typedef struct _stCecRxMsg_t_
{
	stCecMsg_t msgElm[CEC_MSG_RX_BUF_LEN];
	uint8_t push_cur;
	uint8_t pop_cur;
	uint8_t addr_loc;
	uint8_t addr_destination;
	uint16_t addr_physical;
}stCecRxMsg_t, *pstCecRxMsg_t;

typedef enum _cec_msg_state_t_
{
	cec_state_none=0,

	cec_state_start,
	cec_state_header,
	cec_state_opcode,
	cec_state_operand1,
	cec_state_operand2,
	cec_state_operand3,
	cec_state_operand4,
	cec_state_operand5,
	cec_state_operand6,
	cec_state_operand7,
	cec_state_operand8,
	cec_state_operand9,
	cec_state_operand10,
	cec_state_operand11,
	cec_state_operand12,
	cec_state_operand13,
	cec_state_operand14,
	cec_state_operand15,
	cec_state_operand16,

	cec_state_max,

	cec_state_err_start,
	cec_state_err_header,
	cec_state_err_opcode,
	cec_state_err_operand1,
	cec_state_err_operand2,
	cec_state_err_operand3,
	cec_state_err_operand4,
	cec_state_err_operand5,
	cec_state_err_operand6,
	cec_state_err_operand7,
	cec_state_err_operand8,
	cec_state_err_operand9,
	cec_state_err_operand10,
	cec_state_err_operand11,
	cec_state_err_operand12,
	cec_state_err_operand13,
	cec_state_err_operand14,
	cec_state_err_operand15,
	cec_state_err_operand16,

	cec_state_err_max,
}cec_msg_state_t;

typedef struct _stCecPinFun_t_
{
	bool cecMsg_tx_flag;//TRUE=tx, FALSE=rx

	cec_msg_state_t cecMsg_state;
	uint32_t timer_cnt;
	uint8_t bit_cnt;
	uint8_t get_cnt;

	pstCecMsg_t cecMsg_content;
    bool    bPollingFlag;
}stCecPinFun_t, *pstCecPinFun_t;

typedef struct _stCecFrameDetect_t_
{
	bool en_detect_mode;//'true' enable detect mode
	ePinLevel_t cec_pin_cur_state;
	ePinLevel_t cec_pin_pre_state;
}stCecFrameDetect_t, *pstCecFrameDetect_t;

typedef struct _stCecRelinkAction_t_
{
    u8 u8CecActionSta[15];  // 执行的功能
    u16 u16CecLoopDelayTime;  // 执行多个功能之间延时时间
    u8 u8CecActionCnt;      // 执行次数
    u8 u8CecRunningCnt;     // 执行中的次数
    u8 u8RelinkSuccessFlag; // 执行完成标志
}stCecRelinkAction_t,*pstCecRelinkAction_t;

typedef void (*funcPrj1MsLoop)(void);

extern funcPrj1MsLoop pFuncPrj1MsLoopHook;

void vTaskCEC(void *pvParameters);
#if (!_SYSTEM_SUPPORT_RTOS)
void appCecInit(void);
#endif
void cec_pin_fun_register(uint8_t u8CecIndex);
void hdmi_cec_init(void);

void cec_frame_detect(pstCecDeclare_t pCecPort);
void cec_tx_process(pstCecDeclare_t pCecPort);
void cec_rx_process(pstCecDeclare_t pCecPort);
void cec_tx_send_frame(pstCecTxMsg_t pMsg);
void cec_cmd_send(uint8_t u8CecPort, pstCecMsg_t pMsg, bool bFromIsr);
void cec_cmd_tv_poweron(uint8_t u8CecPort, bool bFromIsr);
void cec_cmd_tv_poweroff(uint8_t u8CecPort, bool bFromIsr);
void cec_cmd_tv_volup(uint8_t u8CecPort, bool bFromIsr);
void cec_cmd_tv_voldown(uint8_t u8CecPort, bool bFromIsr);
void cec_cmd_tv_volmute(uint8_t u8CecPort, bool bFromIsr);
void cec_cmd_tv_source(uint8_t u8CecPort, bool bFromIsr);
void cec_cmd_send_to_src(uint8_t u8CecPort, uint8_t u8Operand0, bool bFromIsr);
void cec_get_display_port_phyaddr(uint8_t u8Port, uint16_t u16Phyaddr);
void cec_get_input_port_phyaddr(uint8_t u8Port, uint16_t u16Phyaddr);
void mapi_CEC_SetDestinationAddress(uint8_t u8Port,u8 u8Addr);
u8 mapi_CEC_GetDestinationAddress(uint8_t u8Port);
void cec_msg_parse(pstCecDeclare_t pCecPort);
void cec_cmd_send_to_device(u8 u8DevID,
                                    u8 u8Addr,
                                    u8 u8Opcode,
                                    u8 *pu8Operand,
                                    u8 u8Size,
                                    bool from_isr);
#if _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION
void cec_init_to_function(bool from_isr);
void cec_set_init_function_couter(uint8_t u8Cnt);
#endif
#if 1
void TxPollingMsg(u8 u8Port,u8 u8Head);
void cec_init_port(void);
void mapp_DisplayConnectToInitCecPort(u8 u8Port,u16 u16DelayMs);
void mapp_CecRelinkMessage(u8 u8Port,emCecRelinkState_t *peState,u8 u8StaCnt,u16 u16DelayTimeMs);
void mapp_InputConnectToInitCecPort(u8 u8Port,u16 u16DelayMs);
#endif
void mapp_CecUserCtrlReleasedPort(u8 u8Port,u16 u16DelayMs);

#if (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA1)
void TIMER3_Configuration(u16 arr, u16 psc);
#else
void TIMER2_Configuration(u16 arr, u16 psc);
#endif
void Sys1MsClockRun(void);
void drv_TimerFuncInit(void);

#endif


















