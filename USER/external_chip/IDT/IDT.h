
#ifndef __IDT_H__
#define __IDT_H__

#ifndef DEV_IDT5V_NUM
#define DEV_IDT5V_NUM       1
#endif

#define IDT5V_PCLK_LVTTL    1
#ifndef IDT_SlaveAddr
#define IDT_SlaveAddr       0xD4
#endif

typedef enum _emOutputClockType_
{
    emClk_148_5m = 0,   // 1920x1080_60
    emClk_297m,         // 3840x2160_30
    emClk_74_25m,       // 1280x720_60
    emClk_154m,         // 1920x1200_60
    emClk_65m,          // 1024x768_60
    emClk_85_48m,       // 1360x768_60
    emClk_165m,         // 1600x1200_60

    emClk_max,
}emOutputClockType;

typedef struct _stIDTConfig_t_
{
    bool bDataChange[4];
    u16  u16PClk[4];
    u8   u8Channel;
}stIDTConfig_t,*pstIDTConfig_t;

extern stIDTConfig_t g_stIDTConfigVal[DEV_IDT5V_NUM];
extern vvFuncPoint pvvInitIDTFunc;

void i2c_IDT5V_write_one_byte(u8 u8DevId,u8 u8Addr, u8 u8Data);
u8 i2c_IDT5V_read_one_byte(u8 u8DevId,u8 u8Addr);
void IDT_5V_init(void);
void mapp_IDTProcess(void);
void mapp_IDTSetPclk(u8 u8DevID,u16 u16PClk, u8 u8Channel);
void IDT_SetClockCoef(u16 u16InClk, u16 u16OutClk, u16 *pu16Mx, u8 *pu8Dx);
void IDT_5V_OutProgramClock(u8 u8DevId,u8 u8Ch,u16 u16Clk);    // unit: 10k clock

#endif

