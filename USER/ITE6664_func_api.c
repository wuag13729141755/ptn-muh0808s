/**
 *****************************************************************************
  *                                   Update
  * @Version : V1.0
  * @By      : dengzhiqin
  * @Date    : 2017 / 12 / 05
  * @Brief   : ITE6664 func api
  *
  *****************************************************************************
**/
#include "includes.h"
#if ((IT666X_VER != IT666X_NONE)&&((_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)))

ite6664_func_api_t ite6664_stm32_api =
{
	it6664_tx_hdcp_force_effect,

#if _ENABLE_IT666X_USER_EDID_FUNCTION
    it666x_UserEdidSwitch,
#else
	#if ((USING_ITE_DEMO_CODE==_MACRO_DISABLE)&&\
		((_EN_GPIO_EDID_MODE_SWITCH==_MACRO_ENABLE)||\
		(_EN_GPIO_EDID_SELECT_SWITCH==_MACRO_ENABLE)))
	it6664_my_EdidMode_Switch,
	#endif//#if (USING_ITE_DEMO_CODE == _MACRO_DISABLE)
#endif
};

pite6664_func_api_t pITE6664_STM32_api = &ite6664_stm32_api;
bool EnableTXHDCP = TRUE;
#if (HDCP_HANDLE_FOR_AUDIO_BEFORE_VIDEO == _MACRO_DISABLE)
tx_hdcp_force_t it6664_tx_hdcp_type = _PRJ_TX_HDCP_MODE_ACTIVE, pre_it6664_tx_hdcp_type = tx_hdcp_force_1_4;
#else
extern extern_u8 *gext_u8;
#endif
#if _ENABLE_IT666X_USER_EDID_FUNCTION
vvFuncPoint pvvIt666xEdidSwitchFunction = NULL;
#endif //add by wf8421
extern extern_variables *gext_var;
extern iTE_u8 g_device;
extern iTE_u8 Device_off;
#if(HDCP_HANDLE_FOR_AUDIO_BEFORE_VIDEO == _MACRO_DISABLE)
void it6664_tx_hdcp_force_effect(void)
{
	iTE_u8 i;

	switch(it6664_tx_hdcp_type)
	{
		case tx_hdcp_force_off:
		case tx_hdcp_force_1_4:
		case tx_hdcp_force_2_2:
		case tx_hdcp_force_bypass:
			if(pre_it6664_tx_hdcp_type != it6664_tx_hdcp_type)
			{
                if(it6664_tx_hdcp_type==tx_hdcp_force_off)
				    EnableTXHDCP = FALSE;
                else
				    EnableTXHDCP = TRUE;
				pre_it6664_tx_hdcp_type = it6664_tx_hdcp_type;
				for(i=0; i<4; i++)
				{
					gext_var->HDCPState[i] = Tx_CP_Going;
				}
			}
			break;
		default:
			break;
	}
}
#else
void it6664_tx_hdcp_force_effect(iTE_u8 port)
{
	switch(gext_var->TXHDCPModeCur[port])
	{
		case tx_hdcp_force_off:
		case tx_hdcp_force_1_4:
		case tx_hdcp_force_2_2:
		case tx_hdcp_force_bypass:
			if(gext_var->TXHDCPModePre[port] != gext_var->TXHDCPModeCur[port])
			{
			#if 0//(Project_INT_AS2H1U_TX_POWER_ON_OR_SWITCH_NO_HDCP_OUT_HANDLE == _MACRO_DISABLE)
				if(gext_var->VideoState[port]==Tx_Video_OK)
			#endif
				{
					if((gext_u8->CP_Done & (1<<port)) == FALSE)
					{
                        if(gext_var->TXHDCPModeCur[port] == tx_hdcp_force_off)
                        {
                            EnableTXHDCP = FALSE;
						    //gext_var->HDCPState[port] = Tx_CP_Reset;
                            gext_var->VideoState[port] = Tx_Video_Reset;
                        }
                        else
                        {
    						EnableTXHDCP = TRUE;
						    gext_var->HDCPState[port] = Tx_CP_Going;
                        }
						gext_var->TXHDCPModePre[port] = gext_var->TXHDCPModeCur[port];
					}
				}
			}
			break;
		default:
			break;
	}
}
#endif
#if(SUH4E_H2_DIP_EDID_HANDLE == _MACRO_ENABLE)
void it6664_my_Edid_Copy_Output1ToOver(void);
void it6664_my_Edid_Copy_OutputOverTo1(void);
void it6664_my_Edid_Copy_Inter(iTE_u8 port);
#endif

#if _ENABLE_IT666X_USER_EDID_FUNCTION
void it666x_UserEdidSwitch(void)
{
    if(pvvIt666xEdidSwitchFunction != NULL)
    {
        pvvIt666xEdidSwitchFunction();
    }
}
#endif
#if ((USING_ITE_DEMO_CODE==_MACRO_DISABLE)&&((_EN_GPIO_EDID_MODE_SWITCH==_MACRO_ENABLE)||(_EN_GPIO_EDID_SELECT_SWITCH==_MACRO_ENABLE)))
void it6664_my_EdidMode_Switch(pedid_table_element_t p_element)
{
	iTE_u8 hpdsts[2]={0}, i, k=0, g_device_save = g_device, device;

	iTE6664_MsgTX((" \r\n"));
	iTE6664_MsgTX(("~~~~~~~~~~~~~~~EdidMode_Switch~~~~~~~~~~~~~~~~~ \r\n"));

	#if (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8)
	i = 0;k = 2;
	#elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_4)
	i = 0;k = 1;
	#elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_2)
	i = 0;k = 1;
	#endif

	for(i=0; i<k; i++)
	{
		#if (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8)
		if(i==0)
		{
			device = IT6664_A;
			Device_off = 0;
		}
		else
		{
			device = IT6664_B;
			Device_off = 1;
		}
		#else
		device = g_device;
		Device_off = 0;
		#endif

		iTE6664_MsgTX(("g_device = %02x \r\n",device));
		IT6664_DeviceSelect(device);

		hpdsts[i] = it6664_Check_HPDsts();
		if(hpdsts[i])
		{
			it6664_Edid_Parse(hpdsts[i]);
		}
	}
#if(_EN_GPIO_EDID_MODE_SWITCH==_MACRO_ENABLE)
	switch(p_element->edid_mode)
	{
		case kval_edidm_l_res:
			/*if(hpdsts[0]||hpdsts[1])*/ it6664_my_Edid_Copy_Lowest();
			break;
		case kval_edidm_h_res:
			/*if(hpdsts[0]||hpdsts[1])*/ it6664_my_Edid_Copy_Highest();
			break;
		case kval_edidm_preset:
			#if (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8)
			if(p_element->edid_select <= kval_edids_select7)
			#elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_4)
			if(p_element->edid_select <= kval_edids_select3)
			#elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_2)
			if(p_element->edid_select <= kval_edids_select1)
			#endif
			{
				/*if(hpdsts[0]||hpdsts[1])*/ it6664_my_Edid_Copy_Output((iTE_u8)(p_element->edid_select-1));
			}
			else
			{
				#if (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8)
				if(hpdsts[0]||hpdsts[1]) it6664_my_Edid_Copy_Eeprom((iTE_u8)p_element->edid_select);
				#elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_4)
				if(hpdsts[0]||hpdsts[1]) it6664_my_Edid_Copy_Eeprom((iTE_u8)p_element->edid_select);
				#elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_2)
				if(hpdsts[0]||hpdsts[1]) it6664_my_Edid_Copy_Eeprom((iTE_u8)p_element->edid_select);
				#endif
			}
			break;
		default:
			break;
	}
#elif (_EN_GPIO_EDID_SELECT_SWITCH==_MACRO_ENABLE)
#if(SUH4E_H2_DIP_EDID_HANDLE == _MACRO_ENABLE)
	switch(p_element->edid_select)
	{
		case kval_edids_select0:
				it6664_my_Edid_Copy_Output1ToOver();
				it6664_my_Edid_Copy_Output1ToOver();
				it6664_tx_hdcp_type = tx_hdcp_force_bypass;
				break;
		case kval_edids_select1:
				it6664_my_Edid_Copy_OutputOverTo1();
				it6664_my_Edid_Copy_OutputOverTo1();
				it6664_tx_hdcp_type = tx_hdcp_force_bypass;
				break;
		case kval_edids_select2:
		case kval_edids_select3:
		case kval_edids_select4:
		case kval_edids_select5:
		case kval_edids_select6:
		case kval_edids_select7:
				it6664_my_Edid_Copy_Inter((iTE_u8)p_element->edid_select -kval_edids_select2 );
				it6664_tx_hdcp_type = tx_hdcp_force_bypass;
			break;

		case kval_edids_select8:
				it6664_my_Edid_Copy_Output1ToOver();
				it6664_my_Edid_Copy_Output1ToOver();
				it6664_tx_hdcp_type = tx_hdcp_force_off;
				break;
		case kval_edids_select9:
				it6664_my_Edid_Copy_OutputOverTo1();
				it6664_my_Edid_Copy_OutputOverTo1();
				it6664_tx_hdcp_type = tx_hdcp_force_off;
				break;
		case kval_edids_selectA:
		case kval_edids_selectB:
		case kval_edids_selectC:
		case kval_edids_selectD:
		case kval_edids_selectE:
		case kval_edids_selectF:
				it6664_my_Edid_Copy_Inter((iTE_u8)p_element->edid_select -kval_edids_selectA );
				it6664_tx_hdcp_type = tx_hdcp_force_off;
			break;

		default:
			break;
	}
	p_element->edid_select = kval_edids_none;
#endif

#endif

	iTE6664_MsgTX(("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \r\n"));
	iTE6664_MsgTX((" \r\n"));
	IT6664_DeviceSelect(g_device_save);
}
#endif//#if (USING_ITE_DEMO_CODE == _MACRO_DISABLE)



#endif //#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)




















