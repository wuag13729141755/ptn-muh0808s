#ifndef __PCA9539_H_
#define __PCA9539_H_

#define ADN4604_DEV_ADDR_1      0x90
#define ADN4604_DEV_ADDR_2      0x92
#define ADN4604_DEV_ADDR_3      0x94
#define ADN4604_DEV_ADDR_4      0x96

#ifndef ADN4604_MAP_SELECT
#define ADN4604_MAP_SELECT      1       // 0 select XPT map 0 ; 1 select XPT map 1
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
#ifndef ADN4604_PE_VAR
#define ADN4604_PE_VAR          3
#endif
typedef void (*StartupADN4604Init)(void);
typedef void (*AppADN4604Process)(void);

extern StartupADN4604Init pADN4604InitFunc;
extern AppADN4604Process  pADN4604Process;

uint8_t write_4604(uint8_t u8DevID,  uint8_t u8RegAdd, uint8_t u8Value);
uint8_t read_4604(uint8_t u8DevID, uint8_t u8RegAdd);
void InitAdn4604(void);
void mapp_ADN604Process(void);

#endif











