#ifndef __ITE6664_FUNC_API_H_
#define __ITE6664_FUNC_API_H_

//#include "IT6664_extern.h"

typedef struct _ite6664_func_api_t_
{
#if(HDCP_HANDLE_FOR_AUDIO_BEFORE_VIDEO == _MACRO_DISABLE)
	void (*TxHdcpForceEffect)(void);
#else
	void (*TxHdcpForceEffect)(iTE_u8);
#endif

#if _ENABLE_IT666X_USER_EDID_FUNCTION
    void (*EdidModeSwitch)(void);
#else
	#if ((USING_ITE_DEMO_CODE==_MACRO_DISABLE)&&((_EN_GPIO_EDID_MODE_SWITCH==_MACRO_ENABLE)||(_EN_GPIO_EDID_SELECT_SWITCH==_MACRO_ENABLE)))
	void (*EdidModeSwitch)(pedid_table_element_t);
	#endif//#if (USING_ITE_DEMO_CODE == _MACRO_DISABLE)
#endif
}ite6664_func_api_t, *pite6664_func_api_t;

extern tx_hdcp_force_t it6664_tx_hdcp_type, pre_it6664_tx_hdcp_type;
extern bool EnableTXHDCP;
extern pite6664_func_api_t pITE6664_STM32_api;
#if _ENABLE_IT666X_USER_EDID_FUNCTION
extern vvFuncPoint pvvIt666xEdidSwitchFunction;
#endif //add by wf8421

#if(HDCP_HANDLE_FOR_AUDIO_BEFORE_VIDEO == _MACRO_DISABLE)
void it6664_tx_hdcp_force_effect(void);
#else
void it6664_tx_hdcp_force_effect(iTE_u8 port);
#endif
#if _ENABLE_IT666X_USER_EDID_FUNCTION
void it666x_UserEdidSwitch(void);
#else
#if ((USING_ITE_DEMO_CODE==_MACRO_DISABLE)&&((_EN_GPIO_EDID_MODE_SWITCH==_MACRO_ENABLE)||(_EN_GPIO_EDID_SELECT_SWITCH==_MACRO_ENABLE)))
void it6664_my_EdidMode_Switch(pedid_table_element_t p_element);
#endif
#endif
#endif



















