/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     drv_PT2258.h
**Writer:       wf8421
**Create Date:  2020-09-07
**Rewriter:     wf8421
**Rewrite Date: 2020-09-07
**Description:  PT2258Çý¶¯
**History:
**Version:      V1.0.0
******************************************************/

typedef enum
{
    PT2258Ch_All        = 0,
    PT2258Ch_Audio1,
    PT2258Ch_Audio2,
    PT2258Ch_Audio3,

    PT2258Ch_Max,
}emPT2258ChType;

void Init_PT2258(void);
void PT2258_VolumeMute(u8 uAudioCh,bool tbMute,u8 u8Vloume);
void PT2258_VolumeAdjust(u8 uAudioCh,u8 uVolume);
void PT2258_AdjustFunction(u8 uCh, u8 uValue);
void PT2258_Mute_6CH_Function(bool bMute);

