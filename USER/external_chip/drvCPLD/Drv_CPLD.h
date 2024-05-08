#ifndef _DRV_CPLD_H_
#define _DRV_CPLD_H_

#if (_ENABLE_CPLD_FUNCTION == _MACRO_ENABLE)
void drvCPLD_WriteData(uint8_t addr,uint8_t data,bool bInvData);
uint8_t drvCPLD_ReadData(uint8_t addr,bool bInvData);
#endif

#endif

