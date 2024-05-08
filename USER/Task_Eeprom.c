#include "includes.h"

stEepDataSave_t g_EepromDataSave[eEepDataIndex_Max-1];
extern stProjectGlobalAllVar_t g_stPrjGloVar;

#if _SYSTEM_SUPPORT_RTOS
#if _EN_MAIN_TASK_EEPROM
extern QueueHandle_t xQueue_EProcess;

void vTaskEeprom(void *pvParameters)
{
    const TickType_t    xTicksToWait = 20;//0;//20;//
    BaseType_t          xResult;
    pstEepDataSave_t    pEeData, pData1;
    eEepDataIndex_t     i;

    (void)pvParameters;

    memset(g_EepromDataSave, 0, sizeof(stEepDataSave_t)*(eEepDataIndex_Max-1));

    for(;;)
    {
        xResult = xQueueReceive(xQueue_EProcess, (void *)&pEeData, (TickType_t)xTicksToWait);

        if(xResult == pdPASS)
        {
            for(i=(eEepDataIndex_t)1; i<eEepDataIndex_Max; i++)
            {
                pData1 = pEeData + (i-1);

                if(pData1->u16EepSaveAfT > 0)
                {
                    pData1->u16EepSaveAfT --;
                    if(pData1->u16EepSaveAfT == 0)
                    {
                        //dbg_printf("Save pData1          == 0x%08x\r\n",pData1);
                        //dbg_printf("Save u32EepSaveAdr   == 0x%x\r\n",pData1->u32EepSaveAdr);
                        //dbg_printf("Save pvBufferAdr     == 0x%08x\r\n",pData1->pvBufferAdr);
                        //dbg_printf("Save u32EepSaveLen   == %d\r\n",pData1->u32EepSaveLen);
                        eeprom_write(pData1->u32EepSaveAdr, (uint8_t *)pData1->pvBufferAdr, pData1->u32EepSaveLen);
                        vTaskDelay(5);
                        pData1->bEepSaveOkFlag = TRUE;
                    }
                }
            }
        }

        vTaskDelay(10);
    }
}

#endif

//save eeprom data
void SaveUserDataToEep(eEepDataIndex_t eIndex, uint32_t u32EepAdr, uint8_t *pu8Buffer, uint32_t u32Len, uint16_t u16SaveTime, bool bFromIsr)
{
    #if _EN_MAIN_TASK_EEPROM
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    const TickType_t xTicksToWait = 0;//portMAX_DELAY;
    pstEepDataSave_t pData1 = g_EepromDataSave, pData2;

    pData2 = pData1 + eIndex - 1;
    pData2->pvBufferAdr = (void *)pu8Buffer;
    pData2->u32EepSaveAdr = u32EepAdr;
    pData2->u16EepSaveAfT = u16SaveTime;
    pData2->u32EepSaveLen = u32Len;
    pData2->bEepSaveOkFlag = FALSE;
    //dbg_printf("pData2      == 0x%08x\r\n",pData2);
    //dbg_printf("pvBufferAdr == 0x%08x\r\n",pu8Buffer);
    //dbg_printf("u32EepAdr   == 0x%x\r\n",u32EepAdr);
    //dbg_printf("u16SaveTime == %d\r\n",u16SaveTime);
    //dbg_printf("u32Len      == %d\r\n",u32Len);
    if(bFromIsr)
    {
        xQueueSendFromISR(xQueue_EProcess,(void *)&pData1,&xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        xQueueSend(xQueue_EProcess, (void *)&pData1, xTicksToWait);
    }
    #else
    UNUSED(eIndex);
    UNUSED(u32EepAdr);
    UNUSED(pu8Buffer);
    UNUSED(u32Len);
    UNUSED(u16SaveTime);
    UNUSED(bFromIsr);
    #endif
}
#else

void vTaskEeprom(void *pvParameters)
{
    (void)pvParameters;

}

//save eeprom data
void SaveUserDataToEep(eEepDataIndex_t eIndex, uint32_t u32EepAdr, uint8_t *pu8Buffer, uint32_t u32Len, uint16_t u16SaveTime, bool bFromIsr)
{
    eeprom_write(u32EepAdr, (uint8_t *)pu8Buffer, u32Len);
    UNUSED(eIndex);
    UNUSED(u16SaveTime);
    UNUSED(bFromIsr);
}

#endif

#if _EN_MAIN_TASK_EEPROM
void EepromVarInit(void)
{
    bool bDefaultSet = FALSE;//reset to default value
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 l_u8MainVer[2];
    u16 l_u16MainVer;

    eeprom_read(ADDR_EEPROM_CHECK_MAIN_VER, l_u8MainVer, 2);
    l_u16MainVer = (l_u8MainVer[1]<<8)|l_u8MainVer[0];
    if(l_u16MainVer!=DEF_MAIN_VER)
    {
        l_u16MainVer = DEF_MAIN_VER;
        l_u8MainVer[0] = l_u16MainVer&0xFF;
        l_u8MainVer[1] = (l_u16MainVer>>8)&0xFF;
        eeprom_write(ADDR_EEPROM_CHECK_MAIN_VER, l_u8MainVer, 2);
        app_UserDataToDefault();

        bDefaultSet = TRUE;
        if(bDefaultSet)
        {
            eeprom_write(ADDR_EEPROM_USER_START, (uint8_t *)pstPrjSaveVar, (sizeof(stProjectGlobalSaveVar_t)-sizeof(stPrjSpecVarVector_t)));
        }

        if((pstPrjSaveVar->stSaveSpecVar.pvSpecVar != NULL)&&(pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize != 0))
        {
            //SpecificSaveVarDefault();

            if(bDefaultSet)
            {
                eeprom_write((ADDR_EEPROM_USER_START+((uint8_t *)(&pstPrjSaveVar->stSaveSpecVar)-(uint8_t *)pstPrjSaveVar)), (uint8_t *)pstPrjSaveVar->stSaveSpecVar.pvSpecVar, pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize);
            }
        }
    }
    else
    {
        eeprom_read(ADDR_EEPROM_USER_START, (uint8_t *)pstPrjSaveVar, (sizeof(stProjectGlobalSaveVar_t)-sizeof(stPrjSpecVarVector_t)));

        //hdmi sel & hdmi type switch mode(auto||manu)
        #if ((_EN_EEPROM_FUN_SAVE_HDMI_SEL==_MACRO_ENABLE)||(_EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE==_MACRO_ENABLE))
        if(!mapp_UserData_HdmiSelCheck())
        {
            bDefaultSet = TRUE;
        }

        #if (_EN_EEPROM_FUN_SAVE_HDMI_SEL==_MACRO_DISABLE)
        pstPrjSaveVar->stHdmiSel.u8SwChannel = 0;//the first channel
        #endif

        #if (_EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE==_MACRO_DISABLE)
        pstPrjSaveVar->stHdmiSel.bAutoSwitch = TRUE;//auto switch mode
        #endif

        #endif

        //ir study key
        #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
        if(!mapp_UserDataIrStudyValCheck())
        {
            bDefaultSet = TRUE;
        }
        #endif

        //uart study key
        #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
        if(!mapp_UserDataUartKeyValCheck())
        {
            bDefaultSet = TRUE;
        }
        #endif

        //hdmi tx hdcp status
        #if ((_EN_EEPROM_FUN_SAVE_HDCP_STATUS==_MACRO_ENABLE))
        mapp_UserDataHdcpCheck();
        #endif

        //system edid index
        #if (_EN_EEPROM_FUN_SAVE_SYS_EDID_INDEX == _MACRO_ENABLE)
        if(!mapp_UserDataEdidIndexCheck())
        {
            bDefaultSet = TRUE;
        }
        #endif

        //project name set
        if(!mapp_UserDataProjectNameCheck())
        {
            bDefaultSet = TRUE;
        }
    //set to default value
        if(bDefaultSet)
        {
            eeprom_write(ADDR_EEPROM_USER_START, (uint8_t *)pstPrjSaveVar, (sizeof(stProjectGlobalSaveVar_t)-sizeof(stPrjSpecVarVector_t)));
        }

        if((pstPrjSaveVar->stSaveSpecVar.pvSpecVar != NULL)&&(pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize != 0))
        {
            eeprom_read((ADDR_EEPROM_USER_START+((uint8_t *)(&pstPrjSaveVar->stSaveSpecVar)-(uint8_t *)pstPrjSaveVar)),(uint8_t *)pstPrjSaveVar->stSaveSpecVar.pvSpecVar, pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize);

            if(CheckSaveSpecVarValid()) bDefaultSet = FALSE;
            else bDefaultSet = TRUE;

            if(bDefaultSet)
            {
                eeprom_write((ADDR_EEPROM_USER_START+((uint8_t *)(&pstPrjSaveVar->stSaveSpecVar)-(uint8_t *)pstPrjSaveVar)), (uint8_t *)pstPrjSaveVar->stSaveSpecVar.pvSpecVar, pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize);
            }
        }

    }
}
#endif

// check eeprom save ok
bool mapp_CheckAllDataHasSave(void)
{
    #if _EN_MAIN_TASK_EEPROM
    pstEepDataSave_t pData1 = g_EepromDataSave, pData2;
    eEepDataIndex_t i;
    u8 l_u8SaveCnt = 0;

    for(i=(eEepDataIndex_t)1; i<eEepDataIndex_Max; i++)
    {
        pData2 = pData1 + (i-1);

        if((pData2->bEepSaveOkFlag != TRUE)&&(pData2->pvBufferAdr!=NULL))
        {
            l_u8SaveCnt++;
        }
    }

    if(l_u8SaveCnt != 0)
        return FALSE;
    else
    #endif
        return TRUE;

}

void app_UserDataToDefault(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 l_u8Index;
    mapp_UserData_HdmiSelInit();
#if _ENABLE_MULTI_OUTPUT_CHIP
    for(l_u8Index=0;l_u8Index<Def_outport_num;l_u8Index++)
    {
        pstPrjSaveVar->stHdmiSel.bAutoSwitch[l_u8Index] = Def_AutoSwitchMode;   //auto switch mode
        pstPrjSaveVar->stHdmiSel.u8SwChannel[l_u8Index] = Def_DefaultChannel;      //the first channel
    }
#else
    #if (_EN_EEPROM_FUN_SAVE_HDMI_SEL==_MACRO_DISABLE)
    pstPrjSaveVar->stHdmiSel.u8SwChannel = 0;//the first channel
    #endif
    #if (_EN_EEPROM_FUN_SAVE_TYPE_SWITCH_MODE==_MACRO_DISABLE)
    pstPrjSaveVar->stHdmiSel.bAutoSwitch = TRUE;//auto switch mode
    #endif
#endif //add by wf8421
    mapp_UserDataHdcpInit();
    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    for(l_u8Index=0; l_u8Index<_NUM_IR_STUDY_KEYS; l_u8Index++)
        mapp_UserDataIrStudyValInit(l_u8Index);
    #endif //add by wf8421

    #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
    for(l_u8Index=0; l_u8Index<_NUM_UART_STUDY_KEYS; l_u8Index++)
        mapp_UserDataUartKeyValInit(l_u8Index);
    #endif
    mapp_UserDataEdidIndexInit();
    mapp_UserDataProjectNameInit();

    if((pstPrjSaveVar->stSaveSpecVar.pvSpecVar != NULL)&&(pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize != 0))
    {
        SpecificSaveVarDefault();
    }
    UNUSED(l_u8Index);
}







