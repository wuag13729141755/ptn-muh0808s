#ifndef __TM1623_H_
#define __TM1623_H_

#ifndef DEV_NUM_TM1623
#define DEV_NUM_TM1623  1
#endif

typedef void (*StartupLedStaInit)(void);

//显示亮度级别
#define LEVEL_OFF   				0x80
#define LEVEL_1     				0x88
#define LEVEL_2     				0x89
#define LEVEL_4    					0x8a
#define LEVEL_10    				0x8b
#define LEVEL_11    				0x8c
#define LEVEL_12    				0x8d
#define LEVEL_13    				0x8e
#define LEVEL_14    				0x8f

//port address
#define TM1623_PORT_G1xS18			0x00 //GRID1 x SEG1-SEG8
#define TM1623_PORT_G1xS9E			0x01 //GRID1 x SEG9-SEG14
#define TM1623_PORT_G2xS18			0x02 //GRID2 x SEG1-SEG8
#define TM1623_PORT_G2xS9E			0x03 //GRID2 x SEG9-SEG14
#define TM1623_PORT_G3xS18			0x04 //GRID3 x SEG1-SEG8
#define TM1623_PORT_G3xS9E			0x05 //GRID3 x SEG9-SEG14
#define TM1623_PORT_G4xS18			0x06 //GRID4 x SEG1-SEG8
#define TM1623_PORT_G4xS9E			0x07 //GRID4 x SEG9-SEG14
#define TM1623_PORT_G5xS18			0x08 //GRID5 x SEG1-SEG8
#define TM1623_PORT_G5xS9E			0x09 //GRID5 x SEG9-SEG14
#define TM1623_PORT_G6xS18			0x0A //GRID6 x SEG1-SEG8
#define TM1623_PORT_G6xS9E			0x0B //GRID6 x SEG9-SEG14
#define TM1623_PORT_G7xS18			0x0C //GRID7 x SEG1-SEG8
#define TM1623_PORT_G7xS9E			0x0D //GRID7 x SEG9-SEG14
#define TM1623_PORT_KS1				0x0E //KEY SCAN BYTE1
#define TM1623_PORT_KS2				0x0F //KEY SCAN BYTE2
#define TM1623_PORT_KS3				0x10 //KEY SCAN BYTE3
#define TM1623_PORT_KS4				0x11 //KEY SCAN BYTE4
#define TM1623_PORT_KS5				0x12 //KEY SCAN BYTE5

#define TM1623_PORT_SEG_NUMBER		0x0E
#define TM1623_PORT_KS_NUMBER		0x05

#ifndef TM1623_PORT_TOTAL_NUMBER
#define TM1623_PORT_TOTAL_NUMBER	0x13
#endif

extern uint32_t g_u32TM1623SegUpdate[DEV_NUM_TM1623];
extern uint8_t TM1623_ksg_reg_val[DEV_NUM_TM1623][TM1623_PORT_TOTAL_NUMBER];
extern StartupLedStaInit pTm1623LedStaInitFunc;


void TM1623_Read_Key(uint8_t u8DevId,uint8_t *keyValue);
uint8_t TM1623_Init(void);
void TM1623_SegLed_Write(uint8_t u8DevId,uint8_t SegPort, uint8_t SegVal);
bool isTM1623_SegVal_Same(uint8_t u8DevId);
void assignTM1623_SegVal_Same(uint8_t u8DevId);


#endif



























