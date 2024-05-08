#ifndef __PROJECT_FACTORYDEFAULTVALUE_H__
#define __PROJECT_FACTORYDEFAULTVALUE_H__

#ifndef Def_Uart_CounterDelayTime
#define Def_Uart_CounterDelayTime                   500
#endif

#ifndef Def_Uart_MessageCounter
#define Def_Uart_MessageCounter                     1
#endif

#ifndef Def_Uart_MessageSendDelayTime
#define Def_Uart_MessageSendDelayTime               0
#endif

#ifndef Def_UserEdidMode
#define Def_UserEdidMode                            6
#endif

#ifndef Def_InDetMode
#define Def_InDetMode                               emTMDS
#endif

#ifndef Def_OutputHdcpMode
#if (IT663XX_VER != IT663XX_NONE)
#define Def_OutputHdcpMode                          tx_hdcp_force_bypass
#else
#define Def_OutputHdcpMode                          0
#endif
#endif

#ifndef Def_AutoSwitchMode
#define Def_AutoSwitchMode                          TRUE
#endif

#ifndef Def_DefaultChannel
#define Def_DefaultChannel                          0
#endif

#ifndef Def_UserEdidMode0
#define Def_UserEdidMode0                           0
#endif
#ifndef Def_UserEdidMode1
#define Def_UserEdidMode1                           0
#endif
#ifndef Def_UserEdidMode2
#define Def_UserEdidMode2                           0
#endif
#ifndef Def_UserEdidMode3
#define Def_UserEdidMode3                           0
#endif
#ifndef Def_UserEdidMode4
#define Def_UserEdidMode4                           0
#endif
#ifndef Def_UserEdidMode5
#define Def_UserEdidMode5                           0
#endif
#ifndef Def_UserEdidMode6
#define Def_UserEdidMode6                           0
#endif
#ifndef Def_UserEdidMode7
#define Def_UserEdidMode7                           0
#endif

#endif  //#ifndef __PROJECT_FACTORYDEFAULTVALUE_H__

