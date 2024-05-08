#ifndef _IT66021_MAIN_H_
#define _IT66021_MAIN_H_

typedef enum {
	IT66021_1 = 0,
    #if (IT66021_DEV_NUM >= 2)
	IT66021_2,
	#endif
    #if (IT66021_DEV_NUM >= 3)
    IT66021_3,
    #endif
    #if (IT66021_DEV_NUM >= 4)
    IT66021_4,
    #endif
    #if (IT66021_DEV_NUM >= 5)
    IT66021_5,
    #endif
    #if (IT66021_DEV_NUM >= 6)
    IT66021_6,
    #endif
    #if (IT66021_DEV_NUM >= 7)
    IT66021_7,
    #endif
    #if (IT66021_DEV_NUM >= 8)
    IT66021_8,
    #endif
	IT66021_MAX
} IT66021_ID;

typedef struct {
	unsigned char hdmi;
	unsigned char edid;
    unsigned char m_u8busId;
} IT66021_I2C_ADDR;

typedef enum _emIt66021VideoChange_t_{
    emIt66021_VidChgNone = 0,
    emIt66021_VidChgeVidOn,
    emIt66021_VidChgeVidoff,

    emIt66021_VidChgeMax,
}emIt66021VideoChange_t;

typedef struct {
	unsigned char u8Edid[256];
	unsigned char u8EdidChgFlag;
} stIT66021EdidUpdate_t;

extern IT66021_ID it66021_idx;
extern IT66021_I2C_ADDR it66021_i2c[];

#ifdef __IT66021_MAIN_C__
#define INTERFACE   extern
#else
#define INTERFACE
#endif
INTERFACE void mapp_it66021_SetSrcChangeFlag(u8 u8Id,bool bEn);
INTERFACE bool mapp_it66021_GetSrcChangeFlag(u8 u8Id);
INTERFACE void mapp_it66021_SetVideoChangeSta(u8 u8Id,u8 u8Sta);
INTERFACE u8 mapp_it66021_GetVideoChangeSta(u8 u8Id);
INTERFACE void mapp_it66021_SetInputHpdSta(u8 u8Id,bool bSta);
INTERFACE bool mapp_it66021_GetInputHpdSta(u8 u8Id);

INTERFACE void it66021_id_sel(IT66021_ID id);
INTERFACE void it66021_init(void);
INTERFACE void it66021_process(void);
INTERFACE void it66021_update_edid(unsigned char id,unsigned char *u8EdidBuf);
INTERFACE unsigned char it66021_hdmi_mode(IT66021_ID id);
INTERFACE unsigned char it66021_GetInputTiming(u8 u8Id, u16 *pu16HActive, u16 *pu16VActive, u32 *pu32Pclk);
INTERFACE void it66021_InputTimingChangeProcess(void);
INTERFACE void it66021_GetInputVFreq(u8 u8Id,u8 *u8VFreq);
INTERFACE void it66021EdidManageProcess(u8 u8ID);
INTERFACE void it66021EdidChange(u8 u8ID,u8 *pu8EdidBuf);


#undef INTERFACE
#endif
