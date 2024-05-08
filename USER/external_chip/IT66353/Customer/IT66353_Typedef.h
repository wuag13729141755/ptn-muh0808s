///*****************************************
//  Copyright (C) 2009-2020
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE_Typedef.h>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2020/12/30
//   @fileversion: IT66353_SAMPLE_1.06
//******************************************/
#ifndef _ITE6635_TYPEDEF_H_
#define _ITE6635_TYPEDEF_H_

//////////////////////////////////////////////////
// data type
//////////////////////////////////////////////////
/*
#define iTE_FALSE	0
#define iTE_TRUE		1

#define HIGH            1
#define LOW             0

#ifdef _MCU_8051_
	typedef bit iTE_u1 ;
	#define _CODE code
	#define _CODE_3K code
#else
#ifdef WIN32
	typedef int iTE_u1 ;
	#define _CODE
	#define _CODE_3K
#else
	typedef unsigned char iTE_u1 ;
//	#define _CODE
	#define _CODE  __attribute__ ((section ("._OEM_BU1_RODATA ")))
	#define _CODE_3K __attribute__ ((section ("._3K_RODATA ")))
#endif // _MCU_8051_
#endif

typedef enum _iTE_STATUS {
    iTE_SUCCESS = 0,
    iTE_FAIL
//    SYS_RESERVED
} iTE_STATUS;

#ifndef NULL
	#define NULL ((void *) 0)
#endif

typedef char iTE_s8, *iTE_ps8;
typedef unsigned char iTE_u8, *iTE_pu8;
typedef short iTE_s16, *iTE_ps16;
typedef unsigned short iTE_u16, *iTE_pu16;


#ifndef _MCU_8051_				// IT6350
typedef unsigned int iTE_u32, *iTE_pu32;
typedef int iTE_s32, *iTE_ps32;
#else
typedef unsigned long iTE_u32, *iTE_pu32;
typedef long iTE_s32, *iTE_ps32;
#endif
*/

#define HDMI				(0)
#define DVI				(1)
#define MHL				(2)

#define CD8BIT			4
#define CD10BIT	 		5
#define CD12BIT			6
#define CD16BIT			7

#define OUT8B           		0
#define OUT10B          		1
#define OUT12B          		2

#define RGB444			0
#define YCbCr422			1
#define YCbCr444			2
#define YCbCr420        	3

#define RGB444_SDR		0
#define YUV444_SDR		1
#define RGB444_DDR		2
#define YUV444_DDR		3
#define YUV422_EMB_SYNC_SDR	4
#define YUV422_EMB_SYNC_DDR	5
#define YUV422_SEP_SYNC_SDR	6
#define YUV422_SEP_SYNC_DDR	7
#define CCIR656_EMB_SYNC_SDR	8
#define CCIR656_EMB_SYNC_DDR	9
#define CCIR656_SEP_SYNC_SDR	10
#define CCIR656_SEP_SYNC_DDR	11
#define RGB444_HALF_BUS		12
#define YUV444_HALF_BUS		13
#define BTA1004_SDR				14
#define BTA1004_DDR				15


#define IN_RX0			(0x00)
#define IN_RX1			(0x01)
#define IN_RX2			(0x02)
#define IN_RX3			(0x03)
#define IN_RXn			(0x04)

#define OUT_TX0			(0x00)
#define OUT_TX1			(0x01)
#define OUT_TX2			(0x02)
#define OUT_TXn			(0x03)

#define VID8BIT         		(0)
#define VID10BIT        	(1)
#define VID12BIT        	(2)
#define VID16BIT        	(3)

#define DynVESA         	(0)
#define DynCEA          		(1)

#define ITU601          		(1)
#define ITU709          		(2)
#define EXTCOL          		(3)

#define AUD32K          		(0x03)
#define AUD44K          		(0x00)
#define AUD48K          		(0x02)
#define AUD64K          		(0x0B)
#define AUD88K          		(0x08)
#define AUD96K          		(0x0A)
#define AUD128K         	(0x2B)
#define AUD176K         	(0x0C)
#define AUD192K         	(0x0E)
#define AUD256K         	(0x1B)
#define AUD352K         	(0x0D)
#define AUD384K         	(0x05)
#define AUD512K         	(0x3B)
#define AUD705K         	(0x2D)
#define AUD768K         	(0x09)
#define AUD1024K        	(0x35)
#define AUD1411K        	(0x1D)
#define AUD1536K        	(0x15)

#define I2S             		(0)
#define SPDIF           		(1)

#define LPCM            		(0)
#define NLPCM           		(1)
#define HBR             		(2)
#define DSD             		(3)

#define NOCSC           		(0)
#define RGB2YUV         	(2)
#define YUV2RGB         	(3)

#define TDM2CH          	(0x0)
#define TDM4CH          	(0x1)
#define TDM6CH          	(0x2)
#define TDM8CH          	(0x3)

#define DDC75K          		(0)
#define DDC125K         	(1)
#define DDC312K         	(2)

#define FrmPkt          			(0)
#define SbSFull         			(3)
#define TopBtm          			(6)
#define SbSHalf         			(8)

typedef struct _FUNC_TX{
	iTE_u8	bInputRx: 2;
	iTE_u8	bSrcSel: 2;
	iTE_u8	bEn: 1;
	iTE_u8	bBypass: 1;
	iTE_u8	bRev: 1;
}FTx;
typedef struct _FUNC_420CONV_SPLIT{
	iTE_u8	bConvIn: 2;
	iTE_u8	bConvOut: 2;
	iTE_u8	bSplitIn: 2;
	iTE_u8	bConvEn: 1;
	iTE_u8	bSplitEn: 1;
}FConvSplit;

typedef struct _FUNC_HDCP2{
	iTE_u8	bPortSel0: 2;
	iTE_u8	bPortSel1: 2;
	iTE_u8	bEn0: 1;
	iTE_u8	bEn1: 1;
}FHdcp2;
typedef struct _FUNC_AUDIO{
	iTE_u8	bDecSel: 2;
	iTE_u8	bEncSel: 2;
	iTE_u8	bDecEn: 1;
	iTE_u8	bEncEn: 1;
}FAdo;
typedef struct _FUNC_PATGEN{
	iTE_u8	bOutputTx: 3;
	iTE_u8	bInputRx: 2;
	iTE_u8	bEn: 1;
}FPG;
typedef struct _IT6634_FUNC{
	union{
		FTx		sFTx;
		iTE_u8	u8Dat;
	}uTxConf[3];
	union{
		FConvSplit	sFCovSpt;
		iTE_u8	u8Dat;
	}uCovSpt;
	union{
		FHdcp2	sFHdcp2;
		iTE_u8	u8Dat;
	}uRxHdcp;
	union{
		FHdcp2	sFHdcp2;
		iTE_u8	u8Dat;
	}uTxHdcp;
	union{
		FAdo	sFAdo;
		iTE_u8	u8Dat;
	}uAdoConf;
	union{
		FPG		sFPG;
		iTE_u8	u8Dat;
	}uPGCnof;
	iTE_u8	u8AdoTxPort;
}IT6634Func;

enum{
	EDID_DEFAULT_FHD 	= 0x01,
	EDID_DEFAULT_4K2K = 0x02,
	EDID_COPY			= 0x03,
	EDID_COMPOSE_MIN	= 0x04,
	EDID_COMPOSE_J	= 0x05,
};
enum{
	HDMI_TX_A = 0x00,
	HDMI_TX_B = 0x01,
	HDMI_TX_C = 0x02,
	HDMI_TX_D = 0x03,
};

#define VDO_PCLK						(0)
#define VDO_HTOTAL						(1)
#define VDO_HDEW						(2)
#define VDO_HSW							(3)
#define VDO_HFP							(4)
#define VDO_HBP							(5)
#define VDO_VTOTAL						(6)
#define VDO_VDEW						(7)
#define VDO_VSW							(8)
#define VDO_VFP							(9)
#define VDO_VBP							(10)
#define VDO_HVR2nd						(11)
#define VDO_VFP2nd						(12)
#define VDO_POL							(13)
#define VDO_INTERLANCE					(14)


#define TX0_FUNC_DEFINE				(0)
#define TX1_FUNC_DEFINE				(1)
#define TX2_FUNC_DEFINE				(2)
#define ADO_CONV_SPLT_DEFINE			(3)

#define HPD_L							(0x00)
#define HPD_L2H							(0x01)
#define HPD_H2L							(0x10)
#define HPD_H							(0x11)

#define Channel_B 				(0)
#define Channel_G 				(1)
#define Channel_R 				(2)

#define ADO_PKT_NORMAL			(0)
#define ADO_PKT_PATGEN			(1)
#define ADO_PKT_MUTE			(2)
/************************************  TX_FUNC_DEFINE  *************************************/
#define TX_SRC_SHIFT					(0)
#define TX_SRC_MASK						(0x03 << TX_SRC_SHIFT)
#define TX_SRC_RX0						(0x00 << TX_SRC_SHIFT)
#define TX_SRC_RX1						(0x01 << TX_SRC_SHIFT)
#define TX_SRC_RX2						(0x02 << TX_SRC_SHIFT)
#define TX_SRC_RX3						(0x03 << TX_SRC_SHIFT)

#define TX_FUN_SHIFT					(2)
#define TX_FUN_MASK					(0x0B << TX_FUN_SHIFT)
#define TX_FUN_NORMAL					(0x00 << TX_FUN_SHIFT)
#define TX_FUN_420To444				(0x01 << TX_FUN_SHIFT)
#define TX_FUN_1TO2SPLIT				(0x02 << TX_FUN_SHIFT)
#define TX_FUN_PATGEN					(0x03 << TX_FUN_SHIFT)
#define TX_FUN_BYPASS					(0x08 << TX_FUN_SHIFT)

#define TX_EN_SHIFT						(4)
#define TX_EN_MASK						(0x01 << TX_EN_SHIFT)
#define TX_EN_SET						(0x01 << TX_EN_SHIFT)
#define TX_EN_CLR						(0x00)
//(6)(7)
#define TX_HDCP_ENG_SHIFT				(8)
#define TX_HDCP_ENG_MASK				(0x03 << TX_HDCP_ENG_SHIFT)
#define TX_HDCP_ENG_NONE				(0x00 << TX_HDCP_ENG_SHIFT)
#define TX_HDCP_ENG1					(0x01 << TX_HDCP_ENG_SHIFT)
#define TX_HDCP_ENG2					(0x02 << TX_HDCP_ENG_SHIFT)
#define TX_HDCP_ENG2_0					(0x02 << TX_HDCP_ENG_SHIFT)
#define TX_HDCP_ENG2_1					(0x03 << TX_HDCP_ENG_SHIFT)

#define TX_HDCP_FORCE_SHIFT			(10)
#define TX_HDCP_FORCE_MASK			(0x01 << TX_HDCP_FORCE_SHIFT)
#define TX_HDCP_FORCE_SET				(0x01 << TX_HDCP_FORCE_SHIFT)
#define TX_HDCP_FORCE_CLR				(0x00)

#define TX_NO_EDID_SHIFT				(11)
#define TX_NO_EDID_MASK				(0x01 << TX_NO_EDID_SHIFT)
#define TX_NO_EDID_SET					(0x01 << TX_NO_EDID_SHIFT)
#define TX_NO_EDID_CLR					(0x00)

/*
// TxExtAudSel
#define TX_EXT_ADO_SEL_SHIFT			(11)
#define TX_EXT_ADO_SEL_MASK			(0x01 << TX_EXT_ADO_SEL_SHIFT)
#define TX_EXT_ADO_SEL_SET				(0x01 << TX_EXT_ADO_SEL_SHIFT)
#define TX_EXT_ADO_SEL_CLR				(0x00)
*/
// For switch function
#define TX_SW_CLK_DET_SHIFT			(12)
#define TX_SW_CLK_DET_MASK			(0x0F << TX_SW_CLK_DET_SHIFT)
#define TX_SW_CLK_DET_RX0				(0x01 << TX_SW_CLK_DET_SHIFT)
#define TX_SW_CLK_DET_RX1				(0x02 << TX_SW_CLK_DET_SHIFT)
#define TX_SW_CLK_DET_RX2				(0x04 << TX_SW_CLK_DET_SHIFT)
#define TX_SW_CLK_DET_RX3				(0x08 << TX_SW_CLK_DET_SHIFT)
#define TX_SW_CLK_DET_RX02			(0x05 << TX_SW_CLK_DET_SHIFT)
#define TX_SW_CLK_DET_RX0123			(0x0F << TX_SW_CLK_DET_SHIFT)
#define TX_SW_CLK_DET_RX12			(0x06 << TX_SW_CLK_DET_SHIFT)
#define TX_SW_CLK_DET_RX23			(0x0C << TX_SW_CLK_DET_SHIFT)
/*
#define TX_DET_CLK_SHIFT				(12)
#define TX_DET_CLK_MASK				(0x0F << TX_DET_CLK_SHIFT)
#define TX_DET_CLK_RX0					(0x01 << TX_DET_CLK_SHIFT)
#define TX_DET_CLK_RX1					(0x02 << TX_DET_CLK_SHIFT)
#define TX_DET_CLK_RX2					(0x04 << TX_DET_CLK_SHIFT)
#define TX_DET_CLK_RX3					(0x08 << TX_DET_CLK_SHIFT)
#define TX_DET_CLK_RX0123				(0x0F << TX_DET_CLK_SHIFT)
#define TX_DET_CLK_RX012				(0x07 << TX_DET_CLK_SHIFT)
#define TX_DET_CLK_RX02					(0x05 << TX_DET_CLK_SHIFT)
*/
/************************************  RX_HDCP2_DEFINE  *************************************/
#define RX_HDCP2_ENG0_RX0				(0x10)
#define RX_HDCP2_ENG0_RX1				(0x11)
#define RX_HDCP2_ENG0_RX2				(0x12)
#define RX_HDCP2_ENG0_RX3				(0x13)
#define RX_HDCP2_ENG1_RX0				(0x20)
#define RX_HDCP2_ENG1_RX1				(0x24)
#define RX_HDCP2_ENG1_RX2				(0x28)
#define RX_HDCP2_ENG1_RX3				(0x2C)
/************************************  ADO_CONV_SPLT_DEFINE  *************************************/
#define ADO_DEC_SHIFT					(0)
#define ADO_DEC_MASK					(0x13 << ADO_DEC_SHIFT)
#define ADO_DEC_EN						(0x10 << ADO_DEC_SHIFT)
#define ADO_DEC_PORT_MASK				(0x03 << ADO_DEC_SHIFT)
#define ADO_DEC_RX0					(0x10 << ADO_DEC_SHIFT)
#define ADO_DEC_RX1					(0x11 << ADO_DEC_SHIFT)
#define ADO_DEC_RX2					(0x12 << ADO_DEC_SHIFT)
#define ADO_DEC_RX3					(0x13 << ADO_DEC_SHIFT)

#define ADO_DEC_EN_SHIFT				(0)
#define ADO_DEC_EN_MASK					(0x10 << ADO_DEC_EN_SHIFT)
#define ADO_DEC_EN_SET					(0x10 << ADO_DEC_EN_SHIFT)
#define ADO_DEC_EN_CLR					(0x00)

#define ADO_ENC_SHIFT					(2)
#define ADO_ENC_MASK					(0x0B << ADO_ENC_SHIFT)
#define ADO_ENC_EN						(0x08 << ADO_ENC_SHIFT)
#define ADO_ENC_PORT_MASK				(0x03 << ADO_ENC_SHIFT)
#define ADO_ENC_TX0					(0x08 << ADO_ENC_SHIFT)
#define ADO_ENC_TX1					(0x09 << ADO_ENC_SHIFT)
#define ADO_ENC_TX2					(0x0A << ADO_ENC_SHIFT)
// (6)
#define ADO_ENC_CONF_SHIFT			(7)
#define ADO_ENC_CONF_MASAK			(0x01 << ADO_ENC_CONF_SHIFT)
#define ADO_ENC_CONF_SET				(0x01 << ADO_ENC_CONF_SHIFT)
#define ADO_ENC_CONF_CLR				(0x00)
/************************************  CONV_SPLT_DEFINE  *************************************/
#define Y420TO444_IN_SHIFT				(0)
#define Y420TO444_IN_MASK				(0x03 << Y420TO444_IN_SHIFT)
#define Y420TO444_IN_RX0				(0x00 << Y420TO444_IN_SHIFT)
#define Y420TO444_IN_RX1				(0x01 << Y420TO444_IN_SHIFT)
#define Y420TO444_IN_RX2				(0x02 << 420TO444_IN_SHIFT)
#define Y420TO444_IN_RX3				(0x03 << 420TO444_IN_SHIFT)

#define Y420TO444_OUT_SHIFT			(2)
#define Y420TO444_OUT_MASK			(0x03 << Y420TO444_OUT_SHIFT)
#define Y420TO444_OUT_TX0				(0x00 << Y420TO444_OUT_SHIFT)
#define Y420TO444_OUT_TX1				(0x01 << Y420TO444_OUT_SHIFT)
#define Y420TO444_OUT_TX2				(0x02 << Y420TO444_OUT_SHIFT)
#define Y420TO444_OUT_REV				(0x03 << Y420TO444_OUT_SHIFT)

#define SPLIT_IN_SHIFT					(4)
#define SPLIT_IN_MASK					(0x03 << SPLIT_IN_SHIFT)
#define SPLIT_IN_RX0					(0x00 << SPLIT_IN_SHIFT)
#define SPLIT_IN_RX1					(0x01 << SPLIT_IN_SHIFT)
#define SPLIT_IN_RX2					(0x02 << SPLIT_IN_SHIFT)
#define SPLIT_IN_RX3					(0x03 << SPLIT_IN_SHIFT)

#define Y420TO444_EN_SHIFT				(6)
#define Y420TO444_EN_MASK				(0x01 << Y420TO444_EN_SHIFT)
#define Y420TO444_EN_SET				(0x01 << Y420TO444_EN_SHIFT)
#define Y420TO444_EN_CLR				(0x00)

#define SPLIT_EN_SHIFT					(7)
#define SPLIT_EN_MASK					(0x01 << SPLIT_EN_SHIFT)
#define SPLIT_EN_SET					(0x01 << SPLIT_EN_SHIFT)
#define SPLIT_EN_CLR					(0x00)
/************************************  RX EDID MODE   *************************************/
#define RX_EDID_MODE_SHIFT				(0)
#define RX_EDID_MODE_MASK				(0x1F << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_TYPE_MASK		(0x1C << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_TYPE_SPECIAL		(0x00 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_TYPE_COPY_RX	(0x04 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_TYPE_COPY_TX	(0x08 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_TYPE_COMPOSE	(0x18 << RX_EDID_MODE_SHIFT)

#define RX_EDID_MODE_EXTERNAL			(0x00 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_DEFAULT_0		(0x01 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_DEFAULT_1		(0x02 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_BYPASS				(0x03 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COPY_RX0			(0x04 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COPY_RX1			(0x05 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COPY_RX2			(0x06 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COPY_RX3			(0x07 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COPY_TX0			(0x08 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COPY_TX1			(0x09 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COPY_TX2			(0x0A << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COMPOSE_0		(0x18 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COMPOSE_1		(0x19 << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COMPOSE_2		(0x1A << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COMPOSE_3		(0x1B << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COMPOSE_4		(0x1C << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COMPOSE_5		(0x1D << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COMPOSE_6		(0x1E << RX_EDID_MODE_SHIFT)
#define RX_EDID_MODE_COMPOSE_7		(0x1F << RX_EDID_MODE_SHIFT)

#define RX_EDID_SPECIFIC_PA_SHIFT		(5)
#define RX_EDID_SPECIFIC_PA_MASK		(0x03 << RX_EDID_SPECIFIC_PA_SHIFT)
#define RX_EDID_SPECIFIC_PA_DEFAULT	(0x01 << RX_EDID_SPECIFIC_PA_SHIFT)
#define RX_EDID_SPECIFIC_PA_INC		(0x02 << RX_EDID_SPECIFIC_PA_SHIFT)
//#define RX_EDID_SPECIFIC_PA_SWITCH	(0x03 << RX_EDID_SPECIFIC_PA_SHIFT)
#define RX_EDID_SPECIFIC_PA_CLR		(0x00)

#define RX_EDID_DEFAULT_ADO_SHIFT	(7)
#define RX_EDID_DEFAULT_ADO_MASK		(0x01 << RX_EDID_DEFAULT_ADO_SHIFT)
#define RX_EDID_DEFAULT_ADO_SET		(0x01 << RX_EDID_DEFAULT_ADO_SHIFT)
#define RX_EDID_DEFAULT_ADO_CLR		(0x00)

/************************************  RX FUN  *************************************/
#define RX_FUN_HDCP_SHIFT				(0)
#define RX_FUN_HDCP_MASK				(0x03 << RX_FUN_HDCP_SHIFT)
#define RX_FUN_HDCP_AUTO				(0x00)
#define RX_FUN_HDCP_2					(0x03 << RX_FUN_HDCP_SHIFT)
#define RX_FUN_HDCP_1					(0x02 << RX_FUN_HDCP_SHIFT)
#define RX_FUN_HDCP_NONE				(0x01 << RX_FUN_HDCP_SHIFT)

#define RX_FUN_EQ_SHIFT					(3)
#define RX_FUN_EQ_MASK					(0x07 << RX_FUN_EQ_SHIFT)
#define RX_FUN_EQ_AUTO					(0x00)
#define RX_FUN_EQ_STEP_1				(0x01 << RX_FUN_EQ_SHIFT)
#define RX_FUN_EQ_STEP_2				(0x02 << RX_FUN_EQ_SHIFT)
#define RX_FUN_EQ_STEP_3				(0x03 << RX_FUN_EQ_SHIFT)
#define RX_FUN_EQ_STEP_4				(0x04 << RX_FUN_EQ_SHIFT)
#define RX_FUN_EQ_STEP_5				(0x05 << RX_FUN_EQ_SHIFT)
#define RX_FUN_EQ_STEP_6				(0x06 << RX_FUN_EQ_SHIFT)
#define RX_FUN_EQ_STEP_7				(0x07 << RX_FUN_EQ_SHIFT)

#define RX_FUN_FORCE_ON_SHIFT			(6)
#define RX_FUN_FORCE_ON_MASK			(0x03 << RX_FUN_FORCE_ON_SHIFT)
#define RX_FUN_FORCE_ON_FULL				(0x02 << RX_FUN_FORCE_ON_SHIFT)
#define RX_FUN_FORCE_ON_HALF			(0x01 << RX_FUN_FORCE_ON_SHIFT)
#define RX_FUN_FORCE_ON_CLR				(0x00)

/************************************  RX STA  *************************************/
//u16RxSta
#define RX_CS_SHIFT						(0)
#define RX_CS_MASK						(0x03 << RX_CS_SHIFT)
#define RX_CS_RGB						(0x00 << RX_CS_SHIFT)
#define RX_CS_YUV422					(0x01 << RX_CS_SHIFT)
#define RX_CS_YUV444					(0x02 << RX_CS_SHIFT)
#define RX_CS_YUV420					(0x03 << RX_CS_SHIFT)

#define RX_SCDT_SHIFT					(2)
#define RX_SCDT_MASK					(0x01 << RX_SCDT_SHIFT)
#define RX_SCDT_SET						(0x01 << RX_SCDT_SHIFT)
#define RX_SCDT_CLR						(0x00)

#define RX_MHL_SHIFT					(3)
#define RX_MHL_MASK						(0x01 << RX_MHL_SHIFT)
#define RX_MHL_SET						(0x01 << RX_MHL_SHIFT)
#define RX_MHL_CLR						(0x00)

#define RX_HPD_SHIFT					(4)
#define RX_HPD_MASK						(0x01 << RX_HPD_SHIFT)
#define RX_HPD_SET						(0x01 << RX_HPD_SHIFT)
#define RX_HPD_CLR						(0x00)

#define RX_PWR_ON_SHIFT					(5)
#define RX_PWR_ON_MASK					(0x01 << RX_PWR_ON_SHIFT)
#define RX_PWR_ON_SET					(0x01 << RX_PWR_ON_SHIFT)
#define RX_PWR_ON_CLR					(0x00)

#define RX_MAIN_SEL_SHIFT				(6)
#define RX_MAIN_SEL_MASK				(0x01 << RX_MAIN_SEL_SHIFT)
#define RX_MAIN_SEL_SET					(0x01 << RX_MAIN_SEL_SHIFT)
#define RX_MAIN_SEL_CLR					(0x00)

#define RX_ADO_INFORFRAME_SHIFT			(7)
#define RX_ADO_INFORFRAME_MASK			(0x01 << RX_ADO_INFORFRAME_SHIFT)
#define RX_ADO_INFORFRAME_SET			(0x01 << RX_ADO_INFORFRAME_SHIFT)
#define RX_ADO_INFORFRAME_CLR			(0x00)

#define RX_HDMI_SHIFT					(8)
#define RX_HDMI_MASK					(0x01 << RX_HDMI_SHIFT)
#define RX_HDMI_SET						(0x01 << RX_HDMI_SHIFT)
#define RX_HDMI_CLR						(0x00)

#define RX_HDR_SHIFT					(9)
#define RX_HDR_MASK						(0x01 << RX_HDR_SHIFT)
#define RX_HDR_SET						(0x01 << RX_HDR_SHIFT)
#define RX_HDR_CLR						(0x00)

#define RX_GENPKT_SHIFT					(10)
#define RX_GENPKT_MASK					(0x01 << RX_GENPKT_SHIFT)
#define RX_GENPKT_SET					(0x01 << RX_GENPKT_SHIFT)
#define RX_GENPKT_CLR					(0x00)

#define RX_AV_MUTE_SHIFT				(11)
#define RX_AV_MUTE_MASK					(0x01 << RX_AV_MUTE_SHIFT)
#define RX_AV_MUTE_SET					(0x01 << RX_AV_MUTE_SHIFT)
#define RX_AV_MUTE_CLR					(0x00)

#define RX_HPD_CUR_STATE_SHIFT			(12)
#define RX_HPD_CUR_STATE_MASK			(0x01 << RX_HPD_CUR_STATE_SHIFT)
#define RX_HPD_CUR_STATE_HI				(0x01 << RX_HPD_CUR_STATE_SHIFT)
#define RX_HPD_CUR_STATE_LO				(0x00)
//(12)
#define RX_ACPPKT_SHIFT					(13)
#define RX_ACPPKT_MASK					(0x01 << RX_ACPPKT_SHIFT)
#define RX_ACPPKT_SET					(0x01 << RX_ACPPKT_SHIFT)
#define RX_ACPPKT_CLR					(0x00)

#define RX_SIGNAL_DET_SHIFT				(14)
#define RX_SIGNAL_DET_MASK				(0x01 << RX_SIGNAL_DET_SHIFT)
#define RX_SIGNAL_DET_SET				(0x01 << RX_SIGNAL_DET_SHIFT)
#define RX_SIGNAL_DET_CLR				(0x00)
/************************************  RX HDCP STA  *************************************/
//u8RxHdcpSta
#define RXHDCP_SHIFT					(0)
#define RXHDCP_MASK						(0x03 << RXHDCP_SHIFT)
#define RXHDCP_2						(0x02 << RXHDCP_SHIFT)
#define RXHDCP_1						(0x01 << RXHDCP_SHIFT)
#define RXHDCP_NONE						(0x00)

#define RXHDCP_KSV_READY_SHIFT			(2)
#define RXHDCP_KSV_READY_MASK			(0x01 << RXHDCP_KSV_READY_SHIFT)
#define RXHDCP_KSV_READY_SET			(0x01 << RXHDCP_KSV_READY_SHIFT)
#define RXHDCP_KSV_READY_CLR			(0x00)

#define RXHDCP_ENC_CHANGE_SHIFT			(3)
#define RXHDCP_ENC_CHANGE_MASK			(0x01 << RXHDCP_ENC_CHANGE_SHIFT)
#define RXHDCP_ENC_CHANGE_SET			(0x01 << RXHDCP_ENC_CHANGE_SHIFT)
#define RXHDCP_ENC_CHANGE_CLR			(0x00)

#define RXHDCP_ENC_SHIFT				(4)
#define RXHDCP_ENC_MASK					(0x01 << RXHDCP_ENC_SHIFT)
#define RXHDCP_ENC_SET					(0x01 << RXHDCP_ENC_SHIFT)
#define RXHDCP_ENC_CLR					(0x00)

#define RXHDCP_SUP_SHIFT				(5)
#define RXHDCP_SUP_MASK					(0x03 << RXHDCP_SUP_SHIFT)
#define RXHDCP_SUP_2					(0x03 << RXHDCP_SUP_SHIFT)
#define RXHDCP_SUP_1					(0x02 << RXHDCP_SUP_SHIFT)
#define RXHDCP_SUP_NONE					(0x01 << RXHDCP_SUP_SHIFT)

#define RXHDCP_TYPE_SHIFT				(7)
#define RXHDCP_TYPE_MASK				(0x01 << RXHDCP_TYPE_SHIFT)
#define RXHDCP_TYPE_0					(0x00)
#define RXHDCP_TYPE_1					(0x01 << RXHDCP_TYPE_SHIFT)
/************************************  RX COUNT  *************************************/
//u32RxCnt
#define RXCNT_SCDT_STAB_SHIFT			(0)
#define RXCNT_SCDT_STAB_MASK			(0x0F << RXCNT_SCDT_STAB_SHIFT)
#define RXCNT_SCDT_STAB_0				(0x00)
#define RXCNT_SCDT_STAB_1				(0x01 << RXCNT_SCDT_STAB_SHIFT)
#define RXCNT_SCDT_STAB_2				(0x02 << RXCNT_SCDT_STAB_SHIFT)
#define RXCNT_SCDT_STAB_5				(0x05 << RXCNT_SCDT_STAB_SHIFT)
#define RXCNT_SCDT_STAB_10				(0x0A << RXCNT_SCDT_STAB_SHIFT)

#define RXCNT_CLK_OFF_CHK_SHIFT		(4)
#define RXCNT_CLK_OFF_CHK_MASK		(0x03 << RXCNT_CLK_OFF_CHK_SHIFT)
#define RXCNT_CLK_OFF_CHK_0			(0x00)
#define RXCNT_CLK_OFF_CHK_1			(0x01 << RXCNT_CLK_OFF_CHK_SHIFT)
#define RXCNT_CLK_OFF_CHK_2			(0x02 << RXCNT_CLK_OFF_CHK_SHIFT)
#define RXCNT_CLK_OFF_CHK_3			(0x03 << RXCNT_CLK_OFF_CHK_SHIFT)

#define RXCNT_SYMLOCK_CHK_SHIFT		(6)
#define RXCNT_SYMLOCK_CHK_MASK		(0x03 << RXCNT_SYMLOCK_CHK_SHIFT)
#define RXCNT_SYMLOCK_CHK_0			(0x00)
#define RXCNT_SYMLOCK_CHK_1			(0x01 << RXCNT_SYMLOCK_CHK_SHIFT)
#define RXCNT_SYMLOCK_CHK_2			(0x02 << RXCNT_SYMLOCK_CHK_SHIFT)
#define RXCNT_SYMLOCK_CHK_3			(0x03 << RXCNT_SYMLOCK_CHK_SHIFT)

#define RXCNT_HPD_CHK_SHIFT			(8)
#define RXCNT_HPD_CHK_MASK			(0x1F << RXCNT_HPD_CHK_SHIFT)
#define RXCNT_HPD_CHK_0				(0x00)
#define RXCNT_HPD_CHK_1				(0x01 << RXCNT_HPD_CHK_SHIFT)
#define RXCNT_HPD_CHK_5				(0x05 << RXCNT_HPD_CHK_SHIFT)
#define RXCNT_HPD_CHK_10				(0x0A << RXCNT_HPD_CHK_SHIFT)
#define RXCNT_HPD_CHK_15				(0x0F << RXCNT_HPD_CHK_SHIFT)
#define RXCNT_HPD_CHK_16				(0x10 << RXCNT_HPD_CHK_SHIFT)
#define RXCNT_HPD_CHK_20				(0x14 << RXCNT_HPD_CHK_SHIFT)

#define RXCNT_ECC_SYMRST_SHIFT		(13)
#define RXCNT_ECC_SYMRST_MASK			(0x03 << RXCNT_ECC_SYMRST_SHIFT)
#define RXCNT_ECC_SYMRST_0				(0x00)
#define RXCNT_ECC_SYMRST_1				(0x01 << RXCNT_ECC_SYMRST_SHIFT)
#define RXCNT_ECC_SYMRST_2				(0x02 << RXCNT_ECC_SYMRST_SHIFT)
#define RXCNT_ECC_SYMRST_3				(0x03 << RXCNT_ECC_SYMRST_SHIFT)
 // (15)
/************************************  TX HDCP STA  *************************************/
//  u8TxHdcpSta
#define TXHDCP_HDCP_SHIFT				(0)
#define TXHDCP_HDCP_MASK				(0x03 << TXHDCP_HDCP_SHIFT)
#define TXHDCP_HDCP_NONE				(0x00 << TXHDCP_HDCP_SHIFT)
#define TXHDCP_HDCP_FIRE				(0x01 << TXHDCP_HDCP_SHIFT)
#define TXHDCP_HDCP_GOING				(0x02 << TXHDCP_HDCP_SHIFT)
#define TXHDCP_HDCP_DONE				(0x03 << TXHDCP_HDCP_SHIFT)

#define TXHDCP_KSV_CHK_SHIFT			(2)
#define TXHDCP_KSV_CHK_MASK				(0x01 << TXHDCP_KSV_CHK_SHIFT)
#define TXHDCP_KSV_CHK_SET				(0x01 << TXHDCP_KSV_CHK_SHIFT)
#define TXHDCP_KSV_CHK_CLR				(0x00)

#define TXHDCP_ENC_SHIFT				(3)
#define TXHDCP_ENC_MASK					(0x01 << TXHDCP_ENC_SHIFT)
#define TXHDCP_ENC_SET					(0x01 << TXHDCP_ENC_SHIFT)
#define TXHDCP_ENC_CLR					(0x00)

#define TXHDCP_FHDCP_SHIFT				(4)
#define TXHDCP_FHDCP_MASK				(0x01 << TXHDCP_FHDCP_SHIFT)
#define TXHDCP_FHDCP_SET				(0x01 << TXHDCP_FHDCP_SHIFT)
#define TXHDCP_FHDCP_CLR				(0x00)

#define TXHDCP_RPT_SHIFT				(5)
#define TXHDCP_RPT_MASK					(0x01 << TXHDCP_RPT_SHIFT)
#define TXHDCP_RPT_SET					(0x01 << TXHDCP_RPT_SHIFT)
#define TXHDCP_RPT_CLR					(0x00)

#define TXHDCP_KSV_READY_SHIFT			(6)
#define TXHDCP_KSV_READY_MASK			(0x01 << TXHDCP_KSV_READY_SHIFT)
#define TXHDCP_KSV_READY_SET			(0x01 << TXHDCP_KSV_READY_SHIFT)
#define TXHDCP_KSV_READY_CLR			(0x00)

#define TXHDCP_VER_CHK_SHIFT			(7)
#define TXHDCP_VER_CHK_MASK				(0x01 << TXHDCP_VER_CHK_SHIFT)
#define TXHDCP_VER_CHK_SET				(0x01 << TXHDCP_VER_CHK_SHIFT)
#define TXHDCP_VER_CHK_CLR				(0x00)
/************************************  TX EDID STA  *************************************/
//  u8TxEdidSta
#define TXEDID_CHECK_SHIFT				(0)
#define TXEDID_CHECK_MASK				(0x03 << TXEDID_CHECK_SHIFT)
#define TXEDID_CHECK_SET				(0x01 << TXEDID_CHECK_SHIFT)
#define TXEDID_CHECK_DONE				(0x02 << TXEDID_CHECK_SHIFT)
#define TXEDID_CHECK_CLR				(0x00)
// Det_HFVSDB
#define TXEDID_DET_HFVSDB_SHIFT		(2)
#define TXEDID_DET_HFVSDB_MASK		(0x01 << TXEDID_DET_HFVSDB_SHIFT)
#define TXEDID_DET_HFVSDB_SET			(0x01 << TXEDID_DET_HFVSDB_SHIFT)
#define TXEDID_DET_HFVSDB_CLR			(0x00)
// RGBQCap
#define TXEDID_RGBQCAP_SHIFT			(3)
#define TXEDID_RGBQCAP_MASK				(0x01 << TXEDID_RGBQCAP_SHIFT)
#define TXEDID_RGBQCAP_SET				(0x01 << TXEDID_RGBQCAP_SHIFT)
#define TXEDID_RGBQCAP_CLR				(0x00)
// YCCQCap
#define TXEDID_YCCQCAP_SHIFT			(4)
#define TXEDID_YCCQCAP_MASK				(0x01 << TXEDID_YCCQCAP_SHIFT)
#define TXEDID_YCCQCAP_SET				(0x01 << TXEDID_YCCQCAP_SHIFT)
#define TXEDID_YCCQCAP_CLR				(0x00)
//
#define TXEDID_HDMI_SHIFT				(5)
#define TXEDID_HDMI_MASK				(0x01 << TXEDID_HDMI_SHIFT)
#define TXEDID_HDMI_SET					(0x01 << TXEDID_HDMI_SHIFT)
#define TXEDID_HDMI_CLR					(0x00)
//
#define TXEDID_HDCP_SUP_SHIFT			(6)
#define TXEDID_HDCP_SUP_MASK			(0x03 << TXEDID_HDCP_SUP_SHIFT)
#define TXEDID_HDCP_SUP_2				(0x02 << TXEDID_HDCP_SUP_SHIFT)
#define TXEDID_HDCP_SUP_1				(0x01 << TXEDID_HDCP_SUP_SHIFT)
#define TXEDID_HDCP_SUP_NONE			(0x00 << TXEDID_HDCP_SUP_SHIFT)

/************************************  TX PORT STA  *************************************/
//  u16TxPortSta
// TxVidStbFlag
#define TXPORT_FORCE_DISOUT_SHIFT		(0)
#define TXPORT_FORCE_DISOUT_MASK		(0x01 << TXPORT_FORCE_DISOUT_SHIFT)
#define TXPORT_FORCE_DISOUT_SET			(0x01 << TXPORT_FORCE_DISOUT_SHIFT)
#define TXPORT_FORCE_DISOUT_CLR			(0x00)
//(2)
#define TXPORT_420TO444_SHIFT			(1)
#define TXPORT_420TO444_MASK			(0x01 << TXPORT_420TO444_SHIFT)
#define TXPORT_420TO444_SET				(0x01 << TXPORT_420TO444_SHIFT)
#define TXPORT_420TO444_CLR				(0x00)
//
#define TXPORT_POWER_ON_SHIFT			(2)
#define TXPORT_POWER_ON_MASK			(0x01 << TXPORT_POWER_ON_SHIFT)
#define TXPORT_POWER_ON_SET				(0x01 << TXPORT_POWER_ON_SHIFT)
#define TXPORT_POWER_ON_CLR				(0x00)
// EnTx2VSync
#define TXPORT_TX2VSYNC_EN_SHIFT		(3)
#define TXPORT_TX2VSYNC_EN_MASK			(0x01 << TXPORT_TX2VSYNC_EN_SHIFT)
#define TXPORT_TX2VSYNC_EN_SET			(0x01 << TXPORT_TX2VSYNC_EN_SHIFT)
#define TXPORT_TX2VSYNC_EN_CLR			(0x00)
// ForceRx2TxPCLK, Auto set by FW in 1-to-2 split mode, set TRUE to force function enabled
#define TXPORT_RX2TXPCLK_EN_SHIFT		(4)
#define TXPORT_RX2TXPCLK_EN_MASK		(0x01 << TXPORT_RX2TXPCLK_EN_SHIFT)
#define TXPORT_RX2TXPCLK_EN_SET			(0x01 << TXPORT_RX2TXPCLK_EN_SHIFT)
#define TXPORT_RX2TXPCLK_EN_CLR			(0x00)
// EnDDCMasterSel
#define TXPORT_DDC_SEL_SHIFT			(5)
#define TXPORT_DDC_SEL_MASK				(0x01 << TXPORT_DDC_SEL_SHIFT)
#define TXPORT_DDC_SEL_FW				(0x01 << TXPORT_DDC_SEL_SHIFT)
#define TXPORT_DDC_SEL_HW				(0x00)
// EnH2Enc
#define TXPORT_HDMI2_ENC_EN_SHIFT		(6)
#define TXPORT_HDMI2_ENC_EN_MASK		(0x01 << TXPORT_HDMI2_ENC_EN_SHIFT)
#define TXPORT_HDMI2_ENC_EN_SET			(0x01 << TXPORT_HDMI2_ENC_EN_SHIFT)
#define TXPORT_HDMI2_ENC_EN_CLR			(0x00)
// EnTxPatGen
#define TXPORT_PATGEN_EN_SHIFT			(7)
#define TXPORT_PATGEN_EN_MASK			(0x01 << TXPORT_PATGEN_EN_SHIFT)
#define TXPORT_PATGEN_EN_SET			(0x01 << TXPORT_PATGEN_EN_SHIFT)
#define TXPORT_PATGEN_EN_CLR			(0x00)
// EnExtraPkt
#define TXPORT_EXTRA_PKT_EN_SHIFT		(8)
#define TXPORT_EXTRA_PKT_EN_MASK		(0x01 << TXPORT_EXTRA_PKT_EN_SHIFT)
#define TXPORT_EXTRA_PKT_EN_SET			(0x01 << TXPORT_EXTRA_PKT_EN_SHIFT)
#define TXPORT_EXTRA_PKT_EN_CLR			(0x00)
// EnTxChSwap
#define TXPORT_CH_SWAP_SHIFT			(9)
#define TXPORT_CH_SWAP_MASK				(0x01 << TXPORT_CH_SWAP_SHIFT)
#define TXPORT_CH_SWAP_SET				(0x01 << TXPORT_CH_SWAP_SHIFT)
#define TXPORT_CH_SWAP_CLR				(0x00)
// EnTxPNSwap
#define TXPORT_PN_SWAP_SHIFT			(10)
#define TXPORT_PN_SWAP_MASK				(0x01 << TXPORT_PN_SWAP_SHIFT)
#define TXPORT_PN_SWAP_SET				(0x01 << TXPORT_PN_SWAP_SHIFT)
#define TXPORT_PN_SWAP_CLR				(0x00)

#define TXPORT_SCDT_SHIFT				(11)
#define TXPORT_SCDT_MASK				(0x01 << TXPORT_SCDT_SHIFT)
#define TXPORT_SCDT_SET					(0x01 << TXPORT_SCDT_SHIFT)
#define TXPORT_SCDT_CLR					(0x00)

#define TXPORT_FLAG_POLL_SHIFT			(12)
#define TXPORT_FLAG_POLL_MASK			(0x01 << TXPORT_FLAG_POLL_SHIFT)
#define TXPORT_FLAG_POLL_SET			(0x01 << TXPORT_FLAG_POLL_SHIFT)
#define TXPORT_FLAG_POLL_CLR			(0x00)

#define TXPORT_RR_SHIFT					(13)
#define TXPORT_RR_MASK					(0x01 << TXPORT_RR_SHIFT)
#define TXPORT_RR_SET					(0x01 << TXPORT_RR_SHIFT)
#define TXPORT_RR_CLR					(0x00)

#define TXPORT_ADO_MUTE_SHIFT			(14)
#define TXPORT_ADO_MUTE_MASK			(0x01 << TXPORT_ADO_MUTE_SHIFT)
#define TXPORT_ADO_MUTE_SET				(0x01 << TXPORT_ADO_MUTE_SHIFT)
#define TXPORT_ADO_MUTE_CLR				(0x00)
/************************************  TX PORT STA  *************************************/
//  u16TxVdoSta
// OutColorMode
#define TXVDO_OUT_COLOR_SHIFT			(0)
#define TXVDO_OUT_COLOR_MASK			(0x03 << TXVDO_OUT_COLOR_SHIFT)
#define TXVDO_OUT_COLOR_RGB			(RGB444 << TXVDO_OUT_COLOR_SHIFT)
#define TXVDO_OUT_COLOR_Y422			(YCbCr422 << TXVDO_OUT_COLOR_SHIFT)
#define TXVDO_OUT_COLOR_Y444			(YCbCr444 << TXVDO_OUT_COLOR_SHIFT)
#define TXVDO_OUT_COLOR_Y420			(YCbCr420 << TXVDO_OUT_COLOR_SHIFT)
//
#define TXVDO_OUT_COLOR_FORCE_SHIFT	(2)
#define TXVDO_OUT_COLOR_FORCE_MASK	(0x01 << TXVDO_OUT_COLOR_FORCE_SHIFT)
#define TXVDO_OUT_COLOR_FORCE_SET	(0x01 << TXVDO_OUT_COLOR_FORCE_SHIFT)
#define TXVDO_OUT_COLOR_FORCE_CLR	(0x00 << TXVDO_OUT_COLOR_FORCE_SHIFT)
// ColorDepth
#define TXVDO_COLOR_DEPTH_SHIFT		(3)
#define TXVDO_COLOR_DEPTH_MASK		(0x03 << TXVDO_COLOR_DEPTH_SHIFT)
#define TXVDO_COLOR_DEPTH_8			(VID8BIT << TXVDO_COLOR_DEPTH_SHIFT)
#define TXVDO_COLOR_DEPTH_10			(VID10BIT << TXVDO_COLOR_DEPTH_SHIFT)
#define TXVDO_COLOR_DEPTH_12			(VID12BIT << TXVDO_COLOR_DEPTH_SHIFT)
#define TXVDO_COLOR_DEPTH_16			(VID16BIT << TXVDO_COLOR_DEPTH_SHIFT)
// InColorMode
#define TXVDO_IN_COLOR_SHIFT			(5)
#define TXVDO_IN_COLOR_MASK			(0x03 << TXVDO_IN_COLOR_SHIFT)
#define TXVDO_IN_COLOR_RGB				(RGB444 << TXVDO_IN_COLOR_SHIFT)
#define TXVDO_IN_COLOR_Y422			(YCbCr422 << TXVDO_IN_COLOR_SHIFT)
#define TXVDO_IN_COLOR_Y444			(YCbCr444 << TXVDO_IN_COLOR_SHIFT)
#define TXVDO_IN_COLOR_Y420			(YCbCr420 << TXVDO_IN_COLOR_SHIFT)
// YCbCrCoef
#define TXVDO_YUV_COEF_SHIFT			(7)
#define TXVDO_YUV_COEF_MASK			(0x03 <<TXVDO_YUV_COEF_SHIFT)
#define TXVDO_YUV_COEF_ITU601			(ITU601 << TXVDO_YUV_COEF_SHIFT)
#define TXVDO_YUV_COEF_ITU709			(ITU709 << TXVDO_YUV_COEF_SHIFT)
#define TXVDO_YUV_COEF_EXTCOL			(EXTCOL <<TXVDO_YUV_COEF_SHIFT)
// DynRange
#define TXVDO_DYN_RANGE_SHIFT			(9)
#define TXVDO_DYN_RANGE_MASK			(0x01 << TXVDO_DYN_RANGE_SHIFT)
#define TXVDO_DYN_RANGE_CEA			(0x01 << TXVDO_DYN_RANGE_SHIFT)
#define TXVDO_DYN_RANGE_VESA			(0x00)
// En3D
#define TXVDO_EN_3D_SHIFT				(10)
#define TXVDO_EN_3D_MASK				(0x01 << TXVDO_EN_3D_SHIFT)
#define TXVDO_EN_3D_SET				(0x01 << TXVDO_EN_3D_SHIFT)
#define TXVDO_EN_3D_CLR				(0x00)
// Sel3DFmt
#define TXVDO_3D_FMT_SHIFT			(11)
#define TXVDO_3D_FMT_MASK				(0x0F << TXVDO_3D_FMT_SHIFT)
#define TXVDO_3D_FMT_FRM_PKT			(FrmPkt << TXVDO_3D_FMT_SHIFT)
#define TXVDO_3D_FMT_SBS_FULL			(SbSFull << TXVDO_3D_FMT_SHIFT)
#define TXVDO_3D_FMT_TOP_BTM			(TopBtm << TXVDO_3D_FMT_SHIFT)
#define TXVDO_3D_FMT_SBS_HALF			(SbSHalf << TXVDO_3D_FMT_SHIFT)
//15
#define TXVDO_DCTO422_SHIFT			(15)
#define TXVDO_DCTO422_MASK			(0x01 << TXVDO_DCTO422_SHIFT)
#define TXVDO_DCTO422_SET				(0x01 << TXVDO_DCTO422_SHIFT)
#define TXVDO_DCTO422_CLR				(0x00)
/************************************  TX COUNT  *************************************/
//u16TxCnt
//(0)

#define TXCNT_HPD_L2H_SHIFT				(2)
#define TXCNT_HPD_L2H_MASK				(0x1F << TXCNT_HPD_L2H_SHIFT)
#define TXCNT_HPD_L2H_20				(0x14 << TXCNT_HPD_L2H_SHIFT)
#define TXCNT_HPD_L2H_10				(0x0A << TXCNT_HPD_L2H_SHIFT)
#define TXCNT_HPD_L2H_1					(0x01 << TXCNT_HPD_L2H_SHIFT)
#define TXCNT_HPD_L2H_0					(0x00)

#define TXCNT_SINK_STAB_SHIFT			(7)
#define TXCNT_SINK_STAB_MASK			(0x1F << TXCNT_SINK_STAB_SHIFT)
#define TXCNT_SINK_STAB_20				(0x14 << TXCNT_SINK_STAB_SHIFT)
#define TXCNT_SINK_STAB_10				(0x0A << TXCNT_SINK_STAB_SHIFT)
#define TXCNT_SINK_STAB_1				(0x01 << TXCNT_SINK_STAB_SHIFT)
#define TXCNT_SINK_STAB_0				(0x00)

#define TXCNT_RETRY_ENOUT_SHIFT			(12)
#define TXCNT_RETRY_ENOUT_MASK			(0x01 << TXCNT_RETRY_ENOUT_SHIFT)
#define TXCNT_RETRY_ENOUT_1				(0x01 << TXCNT_RETRY_ENOUT_SHIFT)
#define TXCNT_RETRY_ENOUT_0				(0x00)

#define TXCNT_HDCP_VER_CHK_SHIFT		(13)
#define TXCNT_HDCP_VER_CHK_MASK			(0x07 << TXCNT_HDCP_VER_CHK_SHIFT)
#define TXCNT_HDCP_VER_CHK_7			(0x07 << TXCNT_HDCP_VER_CHK_SHIFT)
#define TXCNT_HDCP_VER_CHK_3			(0x03 << TXCNT_HDCP_VER_CHK_SHIFT)
#define TXCNT_HDCP_VER_CHK_2			(0x02 << TXCNT_HDCP_VER_CHK_SHIFT)
#define TXCNT_HDCP_VER_CHK_1			(0x01 << TXCNT_HDCP_VER_CHK_SHIFT)
#define TXCNT_HDCP_VER_CHK_0			(0x00)
/************************************  ADOIO SPDIF INTERFACE *************************************/

#define ADO_SPDIF_SAMPLE_FREQ_32K		((32 + 44) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_44K		((44 + 48) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_48K		((48 + 64) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_64K		((64 + 88) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_88K		((88 + 96) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_96K		((96 + 128) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_128K		((128 + 176) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_176K		((176 + 192) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_192K		((192 + 256) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_256K		((256 + 352) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_352K		((352 + 384) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_384K		((384 + 512) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_512K		((512 + 705) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_705K		((705 + 768) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_768K		((768 + 1024) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_1024K 	((1024 + 1411) / 2)
#define ADO_SPDIF_SAMPLE_FREQ_1411K 	((1411 + 1536) / 2)

/************************************  Tx ADOIO ENCODER CONFIG *************************************/
//g_u32TxAdoEncConf
// AudCh
#define ADO_CONF_CH_SHIFT						(0)
#define ADO_CONF_CH_MASK						(0x0F << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_0							(0x00 << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_2							(0x02 << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_3							(0x03 << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_4							(0x04 << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_5							(0x05 << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_6							(0x06 << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_7							(0x07 << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_8							(0x08 << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_10							(0x0A << ADO_CONF_CH_SHIFT)
#define ADO_CONF_CH_12							(0x0C << ADO_CONF_CH_SHIFT)
// AudFmt
#define ADO_CONF_SAMPLE_FREQ_SHIFT			(4)
#define ADO_CONF_SAMPLE_FREQ_MASK			(0x3F << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_32K				(0x03 << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_44K				(0x00 << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_48K				(0x02 << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_64K				(0x0B << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_88K				(0x08 << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_96K				(0x0A << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_128K			(0x2B << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_176K			(0x0C << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_192K			(0x0E << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_256K			(0x1B << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_352K			(0x0D << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_384K			(0x05 << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_512K			(0x3B << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_705K			(0x2D << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_768K			(0x09 << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_1024K			(0x35 << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_1411K			(0x1D << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_1536K			(0x15 << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_REFER2HEADER	(0x3E << ADO_CONF_SAMPLE_FREQ_SHIFT)
#define ADO_CONF_SAMPLE_FREQ_ERROR			(0x3F << ADO_CONF_SAMPLE_FREQ_SHIFT)
// AudSel
#define ADO_CONF_SEL_SHIFT						(10)
#define ADO_CONF_SEL_MASK						(0x01 << ADO_CONF_SEL_SHIFT)
#define ADO_CONF_SEL_I2S						(0x00 << ADO_CONF_SEL_SHIFT)
#define ADO_CONF_SEL_SPDIF						(0x01 << ADO_CONF_SEL_SHIFT)
// AudType
#define ADO_CONF_TYPE_SHIFT					(11)
#define ADO_CONF_TYPE_MASK					(0x03 << ADO_CONF_TYPE_SHIFT)
#define ADO_CONF_TYPE_LPCM					(0x00 << ADO_CONF_TYPE_SHIFT)
#define ADO_CONF_TYPE_NLPCM					(0x01 << ADO_CONF_TYPE_SHIFT)
#define ADO_CONF_TYPE_HBR						(0x02 << ADO_CONF_TYPE_SHIFT)
#define ADO_CONF_TYPE_DSD						(0x03 << ADO_CONF_TYPE_SHIFT)
// AudTypSWL
#define ADO_CONF_BITS_SHIFT					(13)
#define ADO_CONF_BITS_MASK					(0x03 << ADO_CONF_BITS_SHIFT)
#define ADO_CONF_BITS_16						(0x00 << ADO_CONF_BITS_SHIFT)
#define ADO_CONF_BITS_18						(0x01 << ADO_CONF_BITS_SHIFT)
#define ADO_CONF_BITS_20						(0x02 << ADO_CONF_BITS_SHIFT)
#define ADO_CONF_BITS_24						(0x03 << ADO_CONF_BITS_SHIFT)
// AudTypFmt
#define ADO_CONF_FMT_SHIFT					(15)
#define ADO_CONF_FMT_MASK						(0x1F << ADO_CONF_FMT_SHIFT)
//#define ADO_CONF_FMT_I2S_STANDARD			(0x00 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_I2S_32BIT				(0x01 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_JUSTIFIED_LEFT			(0x00 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_JUSTIFIED_RIGHT			(0x02 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_1T_DELAY_WS			(0x00 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_NO_DELAY_WS			(0x04 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_WS_CH_LEFT				(0x00 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_WS_CH_RIGHT			(0x08 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_SHIFT_FIRST_MSB			(0x00 << ADO_CONF_FMT_SHIFT)
#define ADO_CONF_FMT_SHIFT_FIRST_LSB			(0x10 << ADO_CONF_FMT_SHIFT)
// EnMSAud		//only for LPCM/NLPCM and DSD when AudCh is less than or equal to 8
#define ADO_CONF_MULTI_STREAM_EN_SHIFT		(20)
#define ADO_CONF_MULTI_STREAM_EN_MASK		(0x01 << ADO_CONF_MULTI_STREAM_EN_SHIFT)
#define ADO_CONF_MULTI_STREAM_EN_SET		(0x01 << ADO_CONF_MULTI_STREAM_EN_SHIFT)
#define ADO_CONF_MULTI_STREAM_EN_CLR		(0x00 << ADO_CONF_MULTI_STREAM_EN_SHIFT)
// EnTDM		// if TRUE, AudSel must be I2S and AudType cannot be DSD and EnAudGen must be TRUE for Internal AudGen
#define ADO_CONF_TDM_EN_SHFIT				(21)
#define ADO_CONF_TDM_EN_MASK					(0x01 << ADO_CONF_TDM_EN_SHFIT)
#define ADO_CONF_TDM_EN_SET					(0x01 << ADO_CONF_TDM_EN_SHFIT)
#define ADO_CONF_TDM_EN_CLR					(0x00 << ADO_CONF_TDM_EN_SHFIT)
// TDMCh		// defautl set TDMCh base on AudCh, but TDM6CH is not allowed for Internal AudGen
#define ADO_CONF_TDM_CH_SHIFT				(22)
#define ADO_CONF_TDM_CH_MASK					(0x03 << ADO_CONF_TDM_CH_SHIFT)
#define ADO_CONF_TDM_CH_2						(0x00 << ADO_CONF_TDM_CH_SHIFT)
#define ADO_CONF_TDM_CH_4						(0x01 << ADO_CONF_TDM_CH_SHIFT)
#define ADO_CONF_TDM_CH_6						(0x02 << ADO_CONF_TDM_CH_SHIFT)
#define ADO_CONF_TDM_CH_8						(0x03 << ADO_CONF_TDM_CH_SHIFT)
// RecChStSel
#define ADO_CONF_CH_STA_SEL_SHIFT			(24)
#define ADO_CONF_CH_STA_SEL_MASK				(0x01 << ADO_CONF_CH_STA_SEL_SHIFT)
#define ADO_CONF_CH_STA_SEL_USER				(0x00 << ADO_CONF_CH_STA_SEL_SHIFT)
#define ADO_CONF_CH_STA_SEL_SPDIF			(0x01 << ADO_CONF_CH_STA_SEL_SHIFT)
// EnDV3D
#define ADO_CONF_DUAL_VIEW_3D_SHIFT			(25)
#define ADO_CONF_DUAL_VIEW_3D_MASK			(0x01 << ADO_CONF_DUAL_VIEW_3D_SHIFT)
#define ADO_CONF_DUAL_VIEW_3D_SET			(0x01 << ADO_CONF_DUAL_VIEW_3D_SHIFT)
#define ADO_CONF_DUAL_VIEW_3D_CLR			(0x00 << ADO_CONF_DUAL_VIEW_3D_SHIFT)
//
#define ADO_CONF_ACTIVE_SHIFT					(26)
#define ADO_CONF_ACTIVE_MASK					(0x01 << ADO_CONF_ACTIVE_SHIFT)
#define ADO_CONF_ACTIVE_SET					(0x01 << ADO_CONF_ACTIVE_SHIFT)
#define ADO_CONF_ACTIVE_CLR					(0x00)

#define ADO_CONF_INPUT_CHK_SHIFT			(27)
#define ADO_CONF_INPUT_CHK_MASK				(0x01 << ADO_CONF_INPUT_CHK_SHIFT)
#define ADO_CONF_INPUT_CHK_SET				(0x01 << ADO_CONF_INPUT_CHK_SHIFT)
#define ADO_CONF_INPUT_CHK_CLR				(0x00)

#define ADO_CONF_NO_INFO_SHIFT				(28)
#define ADO_CONF_NO_INFO_MASK				(0x01 << ADO_CONF_NO_INFO_SHIFT)
#define ADO_CONF_NO_INFO_SET				(0x01 << ADO_CONF_NO_INFO_SHIFT)
#define ADO_CONF_NO_INFO_CLR				(0x00)
#endif
