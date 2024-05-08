#ifndef __TASK_ITE663XX_H_ 
#define __TASK_ITE663XX_H_

//function 
void vTaskITE663XX(void *pvParameters);
bool isIT663XXProSuspend(void);
void IT663XXProResumeOrSuspend(bool bSta);
void IT6634_AuxFuncWork(void);
void IT663XX_TxOutDisOrEn(void);
void IT663XX_TxAdoDisOrEn(void); 

#endif 














