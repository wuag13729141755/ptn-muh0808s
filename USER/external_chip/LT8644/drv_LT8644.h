#ifndef __DRV_LT8644_H_
#define __DRV_LT8644_H_

// 0,1,1,A3,A2,A1,A0,r/w
#define LT8644_DEV_ADDR_1      0x60
#define LT8644_DEV_ADDR_2      0x62
#define LT8644_DEV_ADDR_3      0x64
#define LT8644_DEV_ADDR_4      0x66
#define LT8644_DEV_ADDR_5      0x68
#define LT8644_DEV_ADDR_6      0x6A
#define LT8644_DEV_ADDR_7      0x6C
#define LT8644_DEV_ADDR_8      0x6E
#define LT8644_DEV_ADDR_9      0x70
#define LT8644_DEV_ADDR_10     0x72
#define LT8644_DEV_ADDR_11     0x74
#define LT8644_DEV_ADDR_12     0x76
#define LT8644_DEV_ADDR_13     0x78
#define LT8644_DEV_ADDR_14     0x7A
#define LT8644_DEV_ADDR_15     0x7C
#define LT8644_DEV_ADDR_16     0x7E

#ifndef LT8644_USE_UPDATE_PIN
#define LT8644_USE_UPDATE_PIN   0
#endif

/*
Table 10. Preemphasis Boost and Overshoot vs. Setting
PE Setting | Main Tap Current(mA) | Delayed Tap Current(mA) | Boost(dB) | Overshoot(%) | DC Swing(mV p-p)|
    0           16                  0                           0.0         0               800
    1           16                  2                           2.0         25              800
    2           16                  5                           4.2         62.5            800
    3           16                  8                           6.0         100             800
    4           11                  8                           7.8         145             550
    5           8                   8                           9.5         200             400
    6           4                   6                           12.0        300             300
    7           4                   6                           12.0        300             300
*/
#ifndef LT8644_PE_VAR
#define LT8644_PE_VAR          3
#endif
typedef void (*StartupLT8644Init)(void);
typedef void (*AppLT8644Process)(void);

extern StartupLT8644Init pLT8644InitFunc;
extern AppLT8644Process  pLT8644Process;

uint8_t write_LT8644(uint8_t u8DevID,  uint8_t u8RegAdd, uint8_t u8Value);
uint8_t read_LT8644(uint8_t u8DevID, uint8_t u8RegAdd);
void LT8644UpdateReg(uint8_t u8DevID);
void LT8644UpdatePin(void);
void InitLT8644(void);
void mapp_LT8644Process(void);
void LT8644PinReset(void);

#endif











