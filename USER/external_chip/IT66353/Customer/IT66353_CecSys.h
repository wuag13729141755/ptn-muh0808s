///*****************************************
//  Copyright (C) 2009-2020
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IT6635_CecSys.h>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2020/12/30
//   @fileversion: IT66353_SAMPLE_1.06
//******************************************/
#ifndef _IT6634_CECSYS_H_
#define _IT6634_CECSYS_H_

//#include "IT6635_Typedef.h"


#define CEC_TX_STA_SHIFT		(0)
#define CEC_TX_STA_MASK		(3 << CEC_TX_STA_SHIFT)
#define CEC_TX_STA_ACK			(0 << CEC_TX_STA_SHIFT)
#define CEC_TX_STA_NACK		(1 << CEC_TX_STA_SHIFT)
#define CEC_TX_STA_RETRY		(2 << CEC_TX_STA_SHIFT)
#define CEC_TX_STA_FAIL			(3 << CEC_TX_STA_SHIFT)

#define CEC_TX_DONE_SHIFT		(2)
#define CEC_TX_DONE_MASK		(1 << CEC_TX_DONE_SHIFT)
#define CEC_TX_DONE_SET		(1 << CEC_TX_DONE_SHIFT)
#define CEC_TX_DONE_CLR		(0)

#define CEC_TX_INIT_SHIFT		(3)
#define CEC_TX_INIT_MASK		(1 << CEC_TX_INIT_SHIFT)
#define CEC_TX_INIT_SET			(1 << CEC_TX_INIT_SHIFT)
#define CEC_TX_INIT_CLR			(0)

#define CEC_TX_FIRE_SHIFT		(4)
#define CEC_TX_FIRE_MASK		(1 << CEC_TX_FIRE_SHIFT)
#define CEC_TX_FIRE_SET			(1 << CEC_TX_FIRE_SHIFT)
#define CEC_TX_FIRE_CLR			(0)

#define CEC_TX_POLL_SHIFT		(5)
#define CEC_TX_POLL_MASK		(1 << CEC_TX_POLL_SHIFT)
#define CEC_TX_POLL_SET			(1 << CEC_TX_POLL_SHIFT)
#define CEC_TX_POLL_CLR			(0)

#define CEC_VERISION_2_0		(0x06)

#define CEC_ALL_DEVICE_TYPES_TV								(0x80)
#define CEC_ALL_DEVICE_TYPES_RECORDING_DEVICE				(0x40)
#define CEC_ALL_DEVICE_TYPES_TUNER							(0x20)
#define CEC_ALL_DEVICE_TYPES_PLAYBACK_DEVICE					(0x10)
#define CEC_ALL_DEVICE_TYPES_AUDIO_SYSTEM					(0x08)
#define CEC_ALL_DEVICE_TYPES_CEC_SWITCH						(0x04)

#define CEC_ABORT_REASON_UNRECOGNIXED_OPCODE				(0x00)
#define CEC_ABORT_REASON_NOT_IN_CORRECT_MODE_TO_RESPOND	(0x01)
#define CEC_ABORT_REASON_CAN_NOT_PROVIDE_SOURCE			(0x02)
#define CEC_ABORT_REASON_INVALID_OPERAND					(0x03)
#define CEC_ABORT_REASON_REFUSED								(0x04)
#define CEC_ABORT_REASON_UNABLE_TO_DETEMINE				(0x05)

#define CEC_POWER_STATE_ON				(0x00)
#define CEC_POWER_STATE_STANDBY			(0x01)
#define CEC_POWER_STATE_STANDBY_TO_ON	(0x02)
#define CEC_POWER_STATE_ON_TO_STANDBY	(0x03)

#define CEC_SYS_AUDIO_STATE_OFF			(0x00)
#define CEC_SYS_AUDIO_STATE_ON			(0x01)

#define CEC_AUDIO_MUTE_OFF				(0x00)
#define CEC_AUDIO_MUTE_ON				(0x01)

typedef struct _CEC_QUEUE{
	iTE_u8	u8RPtr;
	iTE_u8	u8WPtr;
	iTE_u8	u8MaxSize;
	iTE_pu8	pu8Buf;
}stCecQ;

typedef enum _CEC_Logic_DEVICE_ID_t
{
	DEVICE_ID_TV=0,
	DEVICE_ID_RECORDING1,		// 1
	DEVICE_ID_RECORDING2,		// 2
	DEVICE_ID_TUNER1,			// 3
	DEVICE_ID_PLAYBACK1,		// 4
	DEVICE_ID_AUDIO,			// 5
	DEVICE_ID_TUNER2,			// 6
	DEVICE_ID_TUNER3,			// 7
	DEVICE_ID_PLAYBACK2,		// 8
	DEVICE_ID_RECORDING3,		// 9
	DEVICE_ID_TUNER4,			// 10
	DEVICE_ID_PLAYBACK3,		// 11
	DEVICE_ID_RESERVED1,		// 12
	DEVICE_ID_RESERVED2,		// 13
	DEVICE_ID_FREEUSE,			// 14
	DEVICE_ID_BROADCAST,		// 15
	DEVICE_ID_MAX
} CEC_Logic_DEVICE_ID_t;

typedef enum _CEC_DEVICE_TYPE
{
	CEC_DEV_TYPE_TV=0,
	CEC_DEV_TYPE_RECORDING_DEV=1,
	CEC_DEV_TYPE_RESERVED=2,
	CEC_DEV_TYPE_TUNER=3,
	CEC_DEV_TYPE_PLAYBACK_DEV=4,
	CEC_DEV_TYPE_AUDIO_SYS=5,
	CEC_DEV_TYPE_PURE_CEC_SWITCH=6,
	CEC_DEV_TYPE_VIDEO_PROCESSOR=7
} CEC_DEVICE_TYPE;


typedef enum _CEC_COMMAND
{
	eFeatureAbort			=0x00,

// One Touch Play Feature
	eActiveSource			=0x82,	// follower:TV, switch	--> Broadcst , Directly address
	eImageViewOn			=0x04,	// follower:TV, switch	--> Broadcst
	eTextViewOn			=0x0D,	// follower:TV

// Routing Control Feature
	eRoutingChange			=0x80,	// follower:TV, switch	--> Broadcst
	eRoutingInformation		=0x81,	// follower:switch		--> Broadcst
	eRequestActiveSource	=0x85,	// follower:switch		--> Broadcst 	, AVR to request active source when Audio ocntrol feature is active
	eSetStreamPath			=0x86,	// follower:switch		--> Broadcst
	eInactiveSource			=0x9D,	//
	eStandBy				=0x36,	// follower:All			--> Broadcst

// One Touch Record Feature
	eRecordOff				=0x0B,
	eRecordOn				=0x09,
	eRecordStatus			=0x0A,
	eRecordTVScreen		=0x0F,

// Timer Programming Feature
	eClearAnalogueTimer	=0x33,
	eClearDigitalTimer		=0x99,
	eClearExternalTimer		=0xA1,
	eSetAnalogueTimer		=0x34,
	eSetDigitalTimer			=0x97,
	eSetExternalTimer		=0xA2,
	eSetTimerProgramTitle	=0x67,
	eTimerClearedStatus		=0x43,
	eTimerStatus			=0x35,

// System Information Feature
	eCECVersioin			=0x9E,
	eGetCECVersion			=0x9F,
	eGivePhysicalAddress	=0x83,	// follower:All
	eGetMenuLanguage		=0x91,	// follower:TV
	eReportPhysicalAddress	=0x84,	// follower:TV
	eSetMenuLanguage		=0x32,	// follower:All,		Initiator:TV
	eReportFeatures			=0xA6,
	eGiveFeatures			=0xA5,

// Deck Control Feature
	eDeckControl			=0x42,
	eDeckStatus				=0x1B,
	eGiveDeckStatus			=0x1A,
	ePlay					=0x41,

// Tuner Control Featuer
	eGiveTunerDeviceStatus	=0x08,
	eSelectAnalogueService	=0x92,
	eSelectDigitalService		=0x93,
	eTunerDeviceStatus		=0x07,
	eTunerStepDecrement	=0x06,
	eTunerStepIncrement	=0x05,

// Vendor Specific Commands Feature
	eDeviceVendorID		=0x87,
	eGiveDeviceVendorID	=0x8C,
	eVendorCommand		=0x89,
	eGiveDevicePowerStatus	=0x8F,
	eReportPowerStatus		=0x90,
	eVendorCommandWithID =0xA0,
	eRemoteButtonDown		=0x8A,
	eRemoteButtonUp		=0x8B,

// OSD Display Feature
	eSetOSDString			=0x64,

// Device OSD Transfer Feature
	eGiveOSDName			=0x46,
	eSetOSDName			=0x47,

// Device Menu Control Feature
	eMenuRequest			=0x8D,
	eMenuStatus				=0x8E,
	eUserControlPressed		=0x44,
	eUserControlReleased	=0x45,

// System Audio Control Feature
	eSystemAudioModeRequest	=0x70,
	eGiveAudioStatus		=0x71,
	eSetSystemAudioMode	=0x72,
	eReportAudioStatus		=0x7A,
	eGiveSystemAudioModeStatus	=0x7D,
	eSystemAudioModeStatus		=0x7E,
	eReportAudioDescriptor 	=0xA3,
	eRequestAudioDescriptor =0xA4,

// Audio Rate Control Feature
	eSetAudioRate			=0x9A,

// ACR
	eInitiateARC				=0xC0,
	eReportARCInitiated		=0xC1,
	eReportARCTerminated	=0xC2,
	eRequestARCInitiation	=0xC3,
	eRequestARCTermination	=0xC4,
	eTerminateARC			=0xC5,

// Dynamic Auto Lipsync Featuer
	eRequestCurrentLatency	=0xA7,
	eReportCurrentLatency	=0xA8,
// HEC
	eCDCMessage			=0xF8,

//Audio Format Feature
	eReportShortAudioDescription	= 0xA3 ,
	eRequestShortAudioDescription	= 0xA4 ,

// Polling Mesage
	ePollingMessage,

	eAbort					=0xff,

	eCECNone

} eCEC_CMD;

typedef enum {
	eDirectly = 0x80,
	eBroadcast_1_4 = 0x40,
	eBroadcast_2_0 = 0x20,
	eBroadcast = (eBroadcast_1_4 | eBroadcast_2_0),
	eBoth = (eDirectly | eBroadcast)
}HeaderType;

typedef enum{
	eCDCFeature = 0,
	eOTPFeature,
	eRoutingCtlFeature,
	eStandByFeature,
	eOTRFeature,
	eTimerProgFeature,
	eSysInfoFeature,
	eDeckCtrlFeature,
	eTunerCtrlFeature,
	eVenSpecCmdFeature,
	eOSDDisplayFeature,
	eDevOSDTransferFeature,
	eDevMenuCtrlFeature,
	eRemoteCtlPassthroughFeature,
	ePowerStatusFeature,
	eGeneralProtocolMessage,
	eSysAudioCtlFeature,
	eAudioRateCtrlFeature,
	eARCFeature,
	eDynAutoLipsyncFeature,
	eHECFeature,
	eAudioFormatFeature ,
	eNoneFeature
}CECOpCodeType;


typedef enum {
	eSelect		= 0x00 ,
	eUp			= 0x01	,
	eDown		= 0x02	,
	eLeft		= 0x03 ,
	eRight		= 0x04 ,
	eRightUp	= 0x05 ,
	eRightDown	= 0x06 ,
	eLeftUp		= 0x07 ,
	eLeftDown	= 0x08 ,
	eRootMenu	= 0x09 ,
	eSetupMenu	= 0x0A ,
	eContentsMenu = 0x0B ,
	eFavoriteMenu = 0x0C ,
	eExit		= 0x0D ,
	eMediaTopMenu = 0x10 ,
	eMediaContentSensitiveMenu = 0x11 ,
	eNumberEntryMode = 0x1D ,
	eDot		= 0x2A ,
	eEnter		= 0x2B ,
	eClear		= 0x2C ,
	eNextFavorite = 0x2F ,
	eChannelUp	= 0x30 ,
	eChannelDown= 0x31 ,
	ePreViousChannel = 0x32 ,
	eSoundSelect= 0x33 ,
	eInputSelect= 0x34 ,
	eDisplayInformation = 0x35 ,
	eHelp		= 0x36,
	ePageUp		= 0x37,
	ePageDown	= 0x38,
	ePower 		= 0x40,
	eVolumeUp 	= 0x41 ,
	eVolumeDown = 0x42 ,
	eMute		= 0x43 ,
	ePlayCmd	= 0x44 ,
	eStop		= 0x45 ,
	ePause		= 0x46 ,
	eRecord		= 0x47 ,
	eRewind		= 0x48 ,
	eFastForward= 0x49 ,
	eEject		= 0x4A ,
	eForward	= 0x4B ,
	eBackward	= 0x4C ,
	eStopRecord = 0x4D ,
	ePauseRecord= 0x4E ,
	eAngle		= 0x50 ,
	eSubPicture = 0x51 ,
	eVideoOnDemand = 0x52 ,
	eElectronicProgramGuide = 0x53 ,
	etimerProgramming 		= 0x54 ,
	einitialConfiguration 	= 0x55 ,
	eSelectBroadcastType 	= 0x56 ,
	eSelectSoundPresentation = 0x57 ,
	ePlayFunction 	= 0x60 ,
	ePausePlayFunction 		= 0x61 ,
	eRecordFunction = 0x62 ,
	ePauseRecordFunction 	= 0x63 ,
	eStopFunction	= 0x64 ,
	eMuteFunction	= 0x65 ,
	eRestoreVolumnFunction 	= 0x66 ,
	eTuneFunction	= 0x67 ,
	eSelectMediaFunction 	= 0x68 ,
	eSelectAVInputFunction 	= 0x69 ,
	eSelectAudioInputFunction = 0x6A ,
	ePowerToggleFunction	= 0x6B ,
	ePowerOffFunction		= 0x6C ,
	ePowerOnFunction		= 0x6D ,
}CECUICmd ;

typedef struct {
	eCEC_CMD cmd;
//	HeaderType header;
	iTE_u8	size;
	CECOpCodeType eType;
	iTE_ps8	CmdString;
} CECcmdFormat;

// for check Invalid CEC Cmd
#ifdef _CEC_LOOKUP_TAB_
#if (IT6635_EN_CEC == iTE_TRUE)

iTE_u16	/*_CODE_3K*/const	u16PaMask[6] = {0x0000, 0xF000, 0xFF00, 0xFFF0, 0xFFFF, 0x0000};

iTE_u8	/*_CODE_3K*/const	u8SwitchLA[0x10] = {
								DEVICE_ID_FREEUSE,						// DEVICE_ID_TV
								DEVICE_ID_RECORDING2,					// DEVICE_ID_RECORDING1
								DEVICE_ID_RECORDING3,					// DEVICE_ID_RECORDING2
								DEVICE_ID_TUNER2,						// DEVICE_ID_TUNER1
								DEVICE_ID_PLAYBACK2,					// DEVICE_ID_PLAYBACK1
								DEVICE_ID_FREEUSE,						// DEVICE_ID_AUDIO
								DEVICE_ID_TUNER3,						// DEVICE_ID_TUNER2
								DEVICE_ID_TUNER4,						// DEVICE_ID_TUNER3
								DEVICE_ID_PLAYBACK3,					// DEVICE_ID_PLAYBACK2
								DEVICE_ID_RESERVED1,					// DEVICE_ID_RECORDING3
								DEVICE_ID_RESERVED1,						// DEVICE_ID_TUNER4
								DEVICE_ID_RESERVED1,						// DEVICE_ID_PLAYBACK3
								DEVICE_ID_RESERVED2,					// DEVICE_ID_RESERVED1
								DEVICE_ID_BROADCAST,					// DEVICE_ID_RESERVED2
								DEVICE_ID_BROADCAST,					// DEVICE_ID_FREEUSE
								DEVICE_ID_BROADCAST					// DEVICE_ID_BROADCAST
							};

static _CODE  CECcmdFormat  LookUpTable_CEC[]=
//static _CODE  LookUpTable_CEC[][3]=
 {
	// One Touch Play Feature
	{eActiveSource,			eBroadcast	|	4,	eOTPFeature, "Active Source"},			// Header + [0x82] + [Physical Address]
	{eImageViewOn,			eDirectly		|	2,	eOTPFeature, "Image View On"},			// Header + [0x04]
	{eTextViewOn,			eDirectly 		|	2,	eOTPFeature, "Text View On"},			// Header + [0x0D]

	// Routing Control Feature
	{eInactiveSource,			eDirectly 		| 	4,	eRoutingCtlFeature, "Inactive Source"},			// Header + [0x9D]
	{eRequestActiveSource,		eBroadcast 	| 	2,	eRoutingCtlFeature, "Request Active Source"},			// Header + [0x85]
	{eRoutingChange,			eBroadcast 	|	6,	eRoutingCtlFeature, "Routing Change"},			// Header + [0x80] + [Org Address] + [New Address]
	{eRoutingInformation,		eBroadcast 	| 	4,	eRoutingCtlFeature, "Routing Information"},			// Header + [0x80] + [Org Address] + [New Address]
	{eSetStreamPath,			eBroadcast 	| 	4,	eRoutingCtlFeature, "Set Stream Path"},			// Header + [0x86] + [Physical Address]

	{eStandBy,				eBoth 		| 	2,	eStandByFeature, "Standby"},			// Header + [0x36]

	// One Touch Record Feature
	{eRecordOff,				eDirectly 		| 	2,	eOTRFeature, "Record OFF"},
	{eRecordOn,				eDirectly 		| 	3,	eOTRFeature, "Record ON"},
	{eRecordStatus,			eDirectly 		| 	3,	eOTRFeature, "Record Status"},
	{eRecordTVScreen,		eDirectly 		| 	2,	eOTRFeature, "Record TV Screen"},

	// Timer Programming Feature
	{eClearAnalogueTimer,		eDirectly 		| 	13,	eTimerProgFeature, "Clear Analogue Timer"},
	{eClearDigitalTimer,		eDirectly 		| 	16,	eTimerProgFeature, "Clear Digital Timer"},
	{eClearExternalTimer,		eDirectly 		| 	13,	eTimerProgFeature, "Clear External Timer"},
	{eSetAnalogueTimer,		eDirectly 		| 	13,	eTimerProgFeature, "Set Analogue Timer"},
	{eSetDigitalTimer,			eDirectly 		| 	16,	eTimerProgFeature, "Set Digital Timer"},
	{eSetExternalTimer,		eDirectly		| 	13,	eTimerProgFeature, "Set External Timer"},
	{eSetTimerProgramTitle,	eDirectly 		| 	2,	eTimerProgFeature, "Set Timer Program Title"},
	{eTimerClearedStatus,		eDirectly 		| 	3,	eTimerProgFeature, "Timer Cleared Status"},
	{eTimerStatus	,			eDirectly 		| 	3,	eTimerProgFeature, "Timer Status"},

	// System Information Feature
	{eCECVersioin,			eDirectly 		| 	3,	eSysInfoFeature, "CEC Version"},			// Header + [0x82] + [CEC Version]
	{eGetCECVersion,			eDirectly 		|	2,	eSysInfoFeature, "Get CEC Version"},			// Header + [0x9F]
	{eGivePhysicalAddress,		eDirectly 		|	2,	eSysInfoFeature, "Give Physical Address"},			// Header + [0x83]
	{eGetMenuLanguage,		eDirectly 		|	2,	eSysInfoFeature, "Get Menu Language"},			// Header + [0x91]
	{eReportPhysicalAddress,	eBroadcast 	|	5,	eSysInfoFeature, "Report Physical Address"},			// Header + [0x84] + [Physical Address]+ [Device Type]
	{eSetMenuLanguage,		eBroadcast 	|	5,	eSysInfoFeature, "Set Menu Language"},			// Header + [0x32] + [Language]
	{eReportFeatures,			eBroadcast 	|	6,	eSysInfoFeature, "Report Features"},
	{eGiveFeatures,			eDirectly 		|	2,	eSysInfoFeature, "Give Features"},

	// Deck Control Feature
	{eDeckControl,			eDirectly 		|	3,	eDeckCtrlFeature, "Deck Control"},			// Header + [0x41] + [Play Mode]
	{eDeckStatus,				eDirectly 		|	3,	eDeckCtrlFeature, "Deck Status"},			// Header + [0x41] + [Play Mode]
	{eGiveDeckStatus,			eDirectly 		|	3,	eDeckCtrlFeature, "Give Deck Status"},		// Header + [0x41] + [Play Mode]
	{ePlay,					eDirectly 		|	3,	eDeckCtrlFeature, "Play"},						// Header + [0x41] + [Play Mode]

	// Tuner Control Featuer
	{eGiveTunerDeviceStatus,	eDirectly 		|	3,	eTunerCtrlFeature, "Give Tuner Device Status"},
	{eSelectAnalogueService,	eDirectly 		|	6,	eTunerCtrlFeature, "Select Analogue Service"},
	{eSelectDigitalService,		eDirectly 		|	9,	eTunerCtrlFeature,	"Select Digital Service"},
	{eTunerDeviceStatus,		eDirectly 		|	7,	eTunerCtrlFeature, "Tuner Device Status"},
	{eTunerStepDecrement,	eDirectly 		|	2,	eTunerCtrlFeature, "Tuner Step Decrement"},
	{eTunerStepIncrement,		eDirectly 		|	2,	eTunerCtrlFeature, "Tuner Step Increment"},

	// Vendor Specific Commands Feature
	{eDeviceVendorID,		eBroadcast 	|	5,	eVenSpecCmdFeature, "Device Vendor ID"},			// Header + [0x87] + [Vendor ID]
	{eGiveDeviceVendorID,		eDirectly 		|	2,	eVenSpecCmdFeature, "Give Device Vendor ID"},			// Header + [0x8C]
	{eVendorCommand,		eDirectly 		|	2,	eVenSpecCmdFeature, "Vendor Command"},			// Header + [0x89] + [Vendor Specific Data]
	{eVendorCommandWithID,	eBoth 		|	5,	eVenSpecCmdFeature, "Vendor Command With ID"},			// Header + [0xA0] + [Vendor ID]+ [Vendor Specific Data]
	{eRemoteButtonDown,		eBoth 		|	2,	eVenSpecCmdFeature, "Vendor Remote Button Down"},			// Header + [0x8A] + [Vendor Specific RC code]
	{eRemoteButtonUp,		eBoth 		|	2,	eVenSpecCmdFeature, "Vendor Remote Button Up"},			// Header + [0x8B] + [Vendor Specific RC code]

	// OSD Display Feature
	{eSetOSDString,			eDirectly 		|	3,	eOSDDisplayFeature, "Set OSD String"},

	// Device OSD Transfer Feature
	{eGiveOSDName,			eDirectly 		|	2,	eDevOSDTransferFeature, "Give OSD Name"},
	{eSetOSDName,			eDirectly 		|	2,	eDevOSDTransferFeature, "Set OSD Name"},

	// Device Menu Contro Feature
	{eMenuRequest,			eDirectly 		|	3,	eDevMenuCtrlFeature, "Menu Request"},
	{eMenuStatus,			eDirectly 		|	3,	eDevMenuCtrlFeature, "Menu Status"},

	// Remote Control PassThrough Feature
	{eUserControlPressed,		eDirectly 		|	3,	eRemoteCtlPassthroughFeature, "User Control Pressed"},			// Header + [0x44] + [UI command]
	{eUserControlReleased,		eDirectly 		|	2,	eRemoteCtlPassthroughFeature, "User Control Released"},			// Header + [0x45]

	// Power Status Feature
	{eGiveDevicePowerStatus,	eDirectly 		|	2,	ePowerStatusFeature, "Give Device Power Status"},			// Header + [0x8F]
	{eReportPowerStatus,		eDirectly 	| eBroadcast_2_0 |	3,	ePowerStatusFeature, "Report Power Status"},			// Header + [0x90] + [Power Status]

	// General Protocol Messages
	{eFeatureAbort,			eDirectly 		|	4,	eGeneralProtocolMessage, "Feature Abort"},			// [Header] + [0x00] + [Feature OpCode] + [Abort Reason]
	{eAbort,					eDirectly 		|	2,	eGeneralProtocolMessage, "Abort"},					// [Header] + [0xFF]
	// System Audio Control Feature
	{eGiveAudioStatus,		eDirectly 		|	2,	eSysAudioCtlFeature, "Give Audio Status"},			// Header + [0x71]
	{eGiveSystemAudioModeStatus,eDirectly 	|	2,	eSysAudioCtlFeature, "Give System Audio Mode Status"},			// Header + [0x7D]
	{eReportAudioStatus,		eDirectly 		|	3,	eSysAudioCtlFeature, "Report Audio Status"},			// Header + [0x7A] + [Audio Status]
	{eReportAudioDescriptor,	eDirectly 		|	2,	eSysAudioCtlFeature, "Report Audio Descriptor"},			// Header + [0xA3] + [Audio Descriptor]
	{eRequestAudioDescriptor,	eDirectly 		|	2,	eSysAudioCtlFeature, "Request Short Audio Descriptor"},			// Header + [0xA4] + [Audio Format ID and Code]
	{eSetSystemAudioMode	,	eBoth 		|	3,	eSysAudioCtlFeature, "Set System Audio Mode"},			// Header + [0x72] + [System Audio Status]
	//{eSystemAudioModeRequest, eDirectly 	|	2,	eSysAudioCtlFeature, "System Audio Mode Request"},			// Header + [0x70] + [Physical Address]
	{eSystemAudioModeRequest, eDirectly 	|	4,	eSysAudioCtlFeature, "System Audio Mode Request"},			// Header + [0x70] + [Physical Address]
	{eSystemAudioModeStatus,	eDirectly 		|	3,	eSysAudioCtlFeature, "System Audio Mode Status"},			// Header + [0x7E] + [System Audio Status]

	// Audio Rate Control Feature
	{eSetAudioRate,			eDirectly 		|	3,	eAudioRateCtrlFeature, "Set Audio Rate"},

	// ACR
	{eInitiateARC,				eDirectly 		|	2,	eARCFeature, "Initiate ARC"},			// Header + [0xC0]
	{eReportARCInitiated,		eDirectly 		|	2,	eARCFeature, "Report ARC Initiated"},			// Header + [0xC1]
	{eReportARCTerminated,	eDirectly 		|	2,	eARCFeature, "Report ARC Terminated"},			// Header + [0xC2]
	{eRequestARCInitiation,		eDirectly 		|	2,	eARCFeature, "Request ARC Initiation"},			// Header + [0xC3]
	{eRequestARCTermination,	eDirectly 		|	2,	eARCFeature, "Request ARC Termination"},			// Header + [0xC4]
	{eTerminateARC,			eDirectly 		|	2,	eARCFeature, "Termiate ARC"},			// Header + [0xC5]

	// Dynamic Auto Lipsync Featuer
	{eRequestCurrentLatency,	eBroadcast 	|	4,	eDynAutoLipsyncFeature, "Request Current Latency"},
	{eReportCurrentLatency,	eBroadcast 	|	7,	eDynAutoLipsyncFeature, "Report Current Latency"},

	// Capability Discovery and Control Feature (HEC)
	{eCDCMessage,			eDirectly 		|	2,	eHECFeature, "CDC Message"},			// Header + [0xF8]

	// Polling Mesage
	//ePollingMessage,
//	{eCDC+1,				eBoth,		0,	eNoneFeature},			//end of 	lookup table !!!
 };

#define     SizeOfLookUpTable_CEC    (sizeof(LookUpTable_CEC)/sizeof(CECcmdFormat))
#endif
#endif

typedef struct _CecSys{
	iTE_u8		u8Sta;
	iTE_u8		u8PaAB, u8PaCD;
	iTE_u8		u8La;
	iTE_u8		u8TxReFireCnt;
	iTE_u8		u8Type;
	iTE_u8		u8Stage;
	iTE_u8		u8SelPort;
}CecSys;

void CecSys_Init(iTE_u8 u8PaAB, iTE_u8 u8PaCD, iTE_u8 u8SelPort);
void CecSys_TxHandler(void);
void CecSys_RxHandler(void);
void CecSys_TxTest(iTE_u8 u8CecSysCmd);


#endif
