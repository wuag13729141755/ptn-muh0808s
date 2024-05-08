#ifndef __TASK_ITE6664_H_
#define __TASK_ITE6664_H_

#if(IT663XX_VER == IT663XX_VER_1_21)
#include "../USER/video_chip/IT663xx_1.21/IT6634_Customer/iTE_Typedef.h"
#elif(IT663XX_VER == IT663XX_VER_1_07)
#include "../USER/video_chip/IT663xx_1.07/IT6634_Customer/iTE_Typedef.h"
#elif(IT663XX_VER == IT663XX_VER_1_30)
#include "../USER/video_chip/IT663xx_1.30/IT6634_Customer/iTE_Typedef.h"
#endif

void vTaskITE6664(void *pvParameters);
void IT666X_AuxFuncWork(void);

void mDelay(iTE_u16 Delay_Count);
void mSleep(iTE_u16 Delay_Count);

#endif



