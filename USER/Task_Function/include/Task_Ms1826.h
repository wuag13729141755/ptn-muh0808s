#ifndef __TASK_MS1826_H_
#define __TASK_MS1826_H_

#if (_ENABLE_CHIP_MS1826 == _MACRO_ENABLE)
#include "ms1826_app.h"

#ifndef _MS1826_WORK_MODE_0
#define _MS1826_WORK_MODE_0         MATRIX_SWITCHER
#endif
#ifndef _MS1826_WORK_MODE_1
#define _MS1826_WORK_MODE_1         MATRIX_SWITCHER
#endif
#ifndef _MS1826_WORK_MODE_2
#define _MS1826_WORK_MODE_2         MATRIX_SWITCHER
#endif
#ifndef _MS1826_WORK_MODE_3
#define _MS1826_WORK_MODE_3         MATRIX_SWITCHER
#endif
#ifndef _MS1826_WORK_MODE_4
#define _MS1826_WORK_MODE_4         MATRIX_SWITCHER
#endif
#ifndef _MS1826_WORK_MODE_5
#define _MS1826_WORK_MODE_5         MATRIX_SWITCHER
#endif

typedef struct _stMS1826Config_T_
{
    u8 u8I2cIndex;
    u8 u8WorkMode;
}stMS1826Config_T;

extern vvFuncPoint      pvvFuncMS1826UserFunctionInit;
extern vvFuncPoint      pvvFuncMS1826UserFunctionProcess;
extern const stMS1826Config_T       cg_stMs1826DevCfgTable[];

void vTask_Ms1826(void *pvParameters);
void vTaskCreate_MS1826(void);

#endif

#endif




















