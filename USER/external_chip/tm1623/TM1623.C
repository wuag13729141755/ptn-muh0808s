#include "includes.h"

#if (_EN_CHIP_TM1623_USE==_MACRO_ENABLE)

const uint8_t cg_u8TM1623DevTable[DEV_NUM_TM1623] =
{
    _SPI_TABLE_INDEX_TM1623,
#if (DEV_NUM_TM1623>1)
    _SPI_TABLE_INDEX_TM1623_1,
#endif
#if (DEV_NUM_TM1623>2)
    _SPI_TABLE_INDEX_TM1623_2,
#endif
#if (DEV_NUM_TM1623>3)
    _SPI_TABLE_INDEX_TM1623_3,
#endif
#if (DEV_NUM_TM1623>4)
    _SPI_TABLE_INDEX_TM1623_4,
#endif
};

uint8_t TM1623_ksg_reg_val[DEV_NUM_TM1623][TM1623_PORT_TOTAL_NUMBER]={{0x00}}, pre_TM1623_seg_reg_val[DEV_NUM_TM1623][TM1623_PORT_SEG_NUMBER]={{0x00}};
uint32_t g_u32TM1623SegUpdate[DEV_NUM_TM1623] = {0x00};//if Seg value will be update
StartupLedStaInit pTm1623LedStaInitFunc = NULL;

extern SemaphoreHandle_t xSemaphore_SPI[NUM_OF_SPI_BUS_MAX];

void TM1623_Read_Key(uint8_t u8DevId, uint8_t *keyValue)
{
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_SPI[cg_u8TM1623DevTable[u8DevId]], portMAX_DELAY);
#endif
	drv_spi_bus_read(cg_u8TM1623DevTable[u8DevId],0x42,5,keyValue,eSpiU8Addr);       // must be read 5 byte data
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreGive(xSemaphore_SPI[cg_u8TM1623DevTable[u8DevId]]);
#endif
}

void TM1623_Write_Cmd(uint8_t u8DevId, uint8_t cmd)
{
	drv_spi_bus_write(cg_u8TM1623DevTable[u8DevId],cmd,0,NULL,eSpiU8Addr);
}

void TM1623_Write_Data(uint8_t u8DevId, uint8_t addr, uint8_t dat)
{
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_SPI[cg_u8TM1623DevTable[u8DevId]], portMAX_DELAY);
#endif
	TM1623_Write_Cmd(cg_u8TM1623DevTable[u8DevId],0x44);
	drv_spi_bus_write(cg_u8TM1623DevTable[u8DevId], 0xC0|addr, 1, &dat, eSpiU8Addr);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreGive(xSemaphore_SPI[cg_u8TM1623DevTable[u8DevId]]);
#endif
}

uint8_t TM1623_PwOn_SelfTest(uint8_t u8DevId)
{
	uint8_t tSuccess=1,i,u8KeyReg[5]={0xFF,0xFF,0xFF,0xFF,0xFF};

	TM1623_Read_Key(u8DevId,u8KeyReg);

	for(i=0; i<5; i++)
	{
		if(u8KeyReg[i] != 0)
		{
			tSuccess = 0;//error
			break;
		}
	}

	return tSuccess;
}

uint8_t TM1623_Init(void)
{
	uint8_t i=0, tError=0;
    uint8_t u8DevId = 0;

    for(u8DevId = 0;u8DevId<DEV_NUM_TM1623;u8DevId++)
    {
#if _SYSTEM_SUPPORT_RTOS
    	xSemaphoreTake(xSemaphore_SPI[cg_u8TM1623DevTable[u8DevId]], portMAX_DELAY);
#endif
    	TM1623_Write_Cmd(u8DevId,0x03);		//设置为7位11段模式
    	TM1623_Write_Cmd(u8DevId,LEVEL_1);	//调节亮度
#if _SYSTEM_SUPPORT_RTOS
    	xSemaphoreGive(xSemaphore_SPI[cg_u8TM1623DevTable[u8DevId]]);
#endif
    	for(i=0; i<14; i++)
    	{
    		TM1623_Write_Data(u8DevId,i,0x00);
    	}
    }

    #if 1
    if(pTm1623LedStaInitFunc != NULL)
    {
        pTm1623LedStaInitFunc();
    }
    #endif
    for(u8DevId = 0;u8DevId<DEV_NUM_TM1623;u8DevId++)
    {
    	if(!TM1623_PwOn_SelfTest(u8DevId))
    	{
    		tError = 1;
    	}
    }
	return tError;
}

void TM1623_SegLed_Write(uint8_t u8DevId,uint8_t SegPort, uint8_t SegVal)
{
	TM1623_Write_Data(u8DevId,SegPort, SegVal);
}

bool isTM1623_SegVal_Same(uint8_t u8DevId)
{
	uint8_t i;
	bool isSameRet = TRUE;

	for(i=0; i<TM1623_PORT_SEG_NUMBER; i++)
	{
		if(TM1623_ksg_reg_val[u8DevId][i] != pre_TM1623_seg_reg_val[u8DevId][i])
		{
			g_u32TM1623SegUpdate[u8DevId] |= (1<<i);
			isSameRet = FALSE;
		}
	}

	return isSameRet;
}

void assignTM1623_SegVal_Same(uint8_t u8DevId)
{
	uint8_t i;

	for(i=0; i<TM1623_PORT_SEG_NUMBER; i++)
	{
		pre_TM1623_seg_reg_val[u8DevId][i] = TM1623_ksg_reg_val[u8DevId][i];
	}
	g_u32TM1623SegUpdate[u8DevId] = 0x00;
}

#endif//#if (_EN_CHIP_TM1623_USE==_MACRO_ENABLE)









