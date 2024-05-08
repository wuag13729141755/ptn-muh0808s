/**
 *****************************************************************************
  *                                   Update
  * @Version : V1.0
  * @By      : dengzhiqin
  * @Date    : 2017 / 12 / 05
  * @Brief   : project common macro
  *
  *****************************************************************************
**/
#ifndef __PROJECT_MACRO_DEFINE_COMMON_H_
#define __PROJECT_MACRO_DEFINE_COMMON_H_

#define _MACRO_ENABLE                           1
#define _MACRO_DISABLE                          0

#define _MACRO_YES                              1
#define _MACRO_NO                               0

#define _GPIO_PIN_LEVEL_HIGH                    1
#define _GPIO_PIN_LEVEL_LOW                     0

//project type(switcher&spliter, switcher, spliter)
#define _PRJ_SWITCHER_AND_SPLITER_NONE          0
#define _PRJ_SWITCHER_AND_SPLITER               1
#define _PRJ_SWITCHER_ONLY                      2
#define _PRJ_SPLITER_ONLY                       3

//pca port
#define PORT_GPIOA                              1
#define PORT_GPIOB                              2
#define PORT_GPIOC                              3
#define PORT_GPIOD                              4
#define PORT_GPIOE                              5
#define PORT_GPIOF                              6
#define PORT_GPIOG                              7
// 9539
#define PORT_PCA                                8
//595
#define PORT_LM74HC595                          9
#define PORT_LM74HC595_H16                      10
//74ls138
#define PORT_LM74HC138                          11

// 1623 led
#define PORT_TM1623_STR                    (12)
#define PORT_TM1623_END                    ((PORT_TM1623_STR+18)+(TM1623_PORT_TOTAL_NUMBER*DEV_NUM_TM1623))
//#define PORT_TM1623_GRID1_L                     12      // GRID1 LOW(SEG1~SEG8)
//#define PORT_TM1623_GRID1_H                     13      // GRID1 HIGH(SEG9~SEG14)
//#define PORT_TM1623_GRID2_L                     14      // GRID2 LOW(SEG1~SEG8)
//#define PORT_TM1623_GRID2_H                     15      // GRID2 HIGH(SEG9~SEG14)
//#define PORT_TM1623_GRID3_L                     16      // GRID3 LOW(SEG1~SEG8)
//#define PORT_TM1623_GRID3_H                     17      // GRID3 HIGH(SEG9~SEG14)
//#define PORT_TM1623_GRID4_L                     18      // GRID4 LOW(SEG1~SEG8)
//#define PORT_TM1623_GRID4_H                     19      // GRID4 HIGH(SEG9~SEG14)
//#define PORT_TM1623_GRID5_L                     20      // GRID5 LOW(SEG1~SEG8)
//#define PORT_TM1623_GRID5_H                     21      // GRID5 HIGH(SEG9~SEG14)
//#define PORT_TM1623_GRID6_L                     22      // GRID6 LOW(SEG1~SEG8)
//#define PORT_TM1623_GRID6_H                     23      // GRID6 HIGH(SEG9~SEG14)
//#define PORT_TM1623_GRID7_L                     24      // GRID7 LOW(SEG1~SEG8)
//#define PORT_TM1623_GRID7_H                     25      // GRID7 HIGH(SEG9~SEG14)
#define PORT_TM1623_GRID1_L(n)                  ((PORT_TM1623_STR)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID1 LOW(SEG1~SEG8)
#define PORT_TM1623_GRID1_H(n)                  ((PORT_TM1623_STR+1)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID1 HIGH(SEG9~SEG14)
#define PORT_TM1623_GRID2_L(n)                  ((PORT_TM1623_STR+2)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID2 LOW(SEG1~SEG8)
#define PORT_TM1623_GRID2_H(n)                  ((PORT_TM1623_STR+3)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID2 HIGH(SEG9~SEG14)
#define PORT_TM1623_GRID3_L(n)                  ((PORT_TM1623_STR+4)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID3 LOW(SEG1~SEG8)
#define PORT_TM1623_GRID3_H(n)                  ((PORT_TM1623_STR+5)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID3 HIGH(SEG9~SEG14)
#define PORT_TM1623_GRID4_L(n)                  ((PORT_TM1623_STR+6)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID4 LOW(SEG1~SEG8)
#define PORT_TM1623_GRID4_H(n)                  ((PORT_TM1623_STR+7)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID4 HIGH(SEG9~SEG14)
#define PORT_TM1623_GRID5_L(n)                  ((PORT_TM1623_STR+8)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID5 LOW(SEG1~SEG8)
#define PORT_TM1623_GRID5_H(n)                  ((PORT_TM1623_STR+9)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID5 HIGH(SEG9~SEG14)
#define PORT_TM1623_GRID6_L(n)                  ((PORT_TM1623_STR+10)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID6 LOW(SEG1~SEG8)
#define PORT_TM1623_GRID6_H(n)                  ((PORT_TM1623_STR+11)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID6 HIGH(SEG9~SEG14)
#define PORT_TM1623_GRID7_L(n)                  ((PORT_TM1623_STR+12)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID7 LOW(SEG1~SEG8)
#define PORT_TM1623_GRID7_H(n)                  ((PORT_TM1623_STR+13)+(TM1623_PORT_TOTAL_NUMBER*n))      // GRID7 HIGH(SEG9~SEG14)

// 1623 key
//#define PORT_TM1623_KVAL_0                      26      // KS1(bit0~bit2) & KS2(bit3~bit5)
//#define PORT_TM1623_KVAL_1                      27      // KS3(bit0~bit2) & KS4(bit3~bit5)
//#define PORT_TM1623_KVAL_2                      28      // KS5(bit0~bit2) & KS6(bit3~bit5)
//#define PORT_TM1623_KVAL_3                      29      // KS7(bit0~bit2) & KS8(bit3~bit5)
//#define PORT_TM1623_KVAL_4                      30      // KS9(bit0~bit2) & KS10(bit3~bit5)
#define PORT_TM1623_KVAL_0(n)                   ((PORT_TM1623_STR+14)+(TM1623_PORT_TOTAL_NUMBER*n))      // KS1(bit0~bit2) & KS2(bit3~bit5)
#define PORT_TM1623_KVAL_1(n)                   ((PORT_TM1623_STR+15)+(TM1623_PORT_TOTAL_NUMBER*n))      // KS3(bit0~bit2) & KS4(bit3~bit5)
#define PORT_TM1623_KVAL_2(n)                   ((PORT_TM1623_STR+16)+(TM1623_PORT_TOTAL_NUMBER*n))      // KS5(bit0~bit2) & KS6(bit3~bit5)
#define PORT_TM1623_KVAL_3(n)                   ((PORT_TM1623_STR+17)+(TM1623_PORT_TOTAL_NUMBER*n))      // KS7(bit0~bit2) & KS8(bit3~bit5)
#define PORT_TM1623_KVAL_4(n)                   ((PORT_TM1623_STR+18)+(TM1623_PORT_TOTAL_NUMBER*n))      // KS9(bit0~bit2) & KS10(bit3~bit5)

#define IS_MCU_GPIO(x)                          ((x>=PORT_GPIOA)&&(x<=PORT_GPIOG))
#define IS_9536_GPIO(x)                         (x==PORT_PCA)
#define IS_595_GPIO(x)                          ((x>=PORT_LM74HC595)&&(x<=PORT_LM74HC595_H16))
#define IS_595_GPIO_L16(x)                      (x==PORT_LM74HC595)
#define IS_595_GPIO_H16(x)                      (x==PORT_LM74HC595_H16)
#define IS_138_GPIO(x)                          (x==PORT_LM74HC138)

#define IS_TM1623_GRID(x)                       ((x>=PORT_TM1623_STR)&&(x<=PORT_TM1623_END)&&(((((x-PORT_TM1623_STR)%TM1623_PORT_TOTAL_NUMBER)+PORT_TM1623_STR)>=PORT_TM1623_GRID1_L(0))&&((((x-PORT_TM1623_STR)%TM1623_PORT_TOTAL_NUMBER)+PORT_TM1623_STR)<=PORT_TM1623_GRID7_H(0))))
#define IS_TM1623_KVAL(x)                       ((x>=PORT_TM1623_STR)&&(x<=PORT_TM1623_END)&&(((((x-PORT_TM1623_STR)%TM1623_PORT_TOTAL_NUMBER)+PORT_TM1623_STR)>=PORT_TM1623_KVAL_0(0))&&((((x-PORT_TM1623_STR)%TM1623_PORT_TOTAL_NUMBER)+PORT_TM1623_STR)<=PORT_TM1623_KVAL_4(0))))
#define TM1623_DEV_INDEX(x)                     ((x-PORT_TM1623_STR)/TM1623_PORT_TOTAL_NUMBER)
#define TM1623_PORT_NUM(x)                      ((x-PORT_TM1623_STR)%TM1623_PORT_TOTAL_NUMBER)

//switcher type
#define _SWITCHER_TYPE_NONE                     0       //NONE
#define _SWITCHER_TYPE_4_TO_1                   1       //USE IT66341
#define _SWITCHER_TYPE_2_TO_1                   2       //USE IT66321
#define _SWITCHER_TYPE_4_TO_3                   3       //USE IT6634
#define _SWITCHER_TYPE_6_TO_1                   4       //USE TWO PIECES OF IT66341
#define _SWITCHER_TYPE_4_TO_1_SEAMLESS          5       //USE FOUR PIECES OF IT66321
#define _SWITCHER_TYPE_6_TO_1_SEAMLESS          6       //USE FOUR PIECES OF IT66321

//spliter type
#define _SPLITER_TYPE_NONE                      0       //NONE
#define _SPLITER_TYPE_1_TO_2                    1       //USE IT6663 ONLY
#define _SPLITER_TYPE_1_TO_4                    2       //USE IT6664 ONLY
#define _SPLITER_TYPE_1_TO_8                    3       //USE IT6663 + IT6664 * 2
#define _SPLITER_TYPE_1_TO_24                   4       //USE IT6664*8

//ir function
#define _IR_FUNCTION_NONE                       0
#define _IR_FUNCTION_REMOTE                     1<<0
#define _IR_FUNCTION_KEY_STUDY                  1<<1
#define _IR_FUNCTION_BYPASS                     1<<2

//ir sensor pin
#define _IR_SENSOR_IN_GPIOB9                    0
#define _IR_SENSOR_IN_GPIOB8                    1
#define _IR_SENSOR_IN_GPIOB0                    2
#define _IR_SENSOR_IN_GPIOA1                    3
#define _IR_SENSOR_IN_GPIOA0                    4
#define _IR_SENSOR_IN_GPIOA15                   5
#define _IR_SENSOR_IN_GPIOA5                    6

//hdmi input port bit index
#define _HDMI_INPUT_NONE_BIT_INDEX              (0xFF)
#define _HDMI_INPUT_1_BIT_INDEX                 (1<<0)
#define _HDMI_INPUT_2_BIT_INDEX                 (1<<1)
#define _HDMI_INPUT_3_BIT_INDEX                 (1<<2)
#define _HDMI_INPUT_4_BIT_INDEX                 (1<<3)
#define _HDMI_INPUT_5_BIT_INDEX                 (1<<4)
#define _HDMI_INPUT_6_BIT_INDEX                 (1<<5)
#define _HDMI_INPUT_7_BIT_INDEX                 (1<<6)
#define _HDMI_INPUT_8_BIT_INDEX                 (1<<7)
#define _HDMI_INPUT_9_BIT_INDEX                 (1<<8)
#define _HDMI_INPUT_10_BIT_INDEX                (1<<9)
#define _HDMI_INPUT_11_BIT_INDEX                (1<<10)
#define _HDMI_INPUT_12_BIT_INDEX                (1<<11)
#define _HDMI_INPUT_13_BIT_INDEX                (1<<12)
#define _HDMI_INPUT_14_BIT_INDEX                (1<<13)
#define _HDMI_INPUT_15_BIT_INDEX                (1<<14)
#define _HDMI_INPUT_16_BIT_INDEX                (1<<15)

//hdmi input port physic index
#define _HDMI_INPUT_1_PHYSIC_INDEX              0
#define _HDMI_INPUT_2_PHYSIC_INDEX              1
#define _HDMI_INPUT_3_PHYSIC_INDEX              2
#define _HDMI_INPUT_4_PHYSIC_INDEX              3
#define _HDMI_INPUT_PHYSIC_INDEX_INVALID        0xFF

#define _HDMI_INPUT_PHYSIC_SHIFT                2
#define _HDMI_INPUT_PHYSIC_MASK                 0x3

//hdmi output port physic index
#define _HDMI_OUTPUT_1_PHYSIC_INDEX             0
#define _HDMI_OUTPUT_2_PHYSIC_INDEX             1
#define _HDMI_OUTPUT_3_PHYSIC_INDEX             2
#define _HDMI_OUTPUT_4_PHYSIC_INDEX             3
#define _HDMI_OUTPUT_5_PHYSIC_INDEX             4
#define _HDMI_OUTPUT_6_PHYSIC_INDEX             5
#define _HDMI_OUTPUT_7_PHYSIC_INDEX             6
#define _HDMI_OUTPUT_8_PHYSIC_INDEX             7

//uart port
#define _UART_PORT_NONE                         0
#define _UART_PORT_1                            1
#define _UART_PORT_2                            2
#define _UART_PORT_3                            3
#define _UART_PORT_4                            4       // PC10 PC11
#define _UART_PORT_5                            5       // PC12 PD2
#define _UART_PORT_6                            6       // PC6 PC7      // n32g45x pc0 pc1
#if ((defined(STM32F2XX_HD))||defined(GD32F4XX))
#define _UART_PORT_3_PD8_9                      7       // PD8 PD9
#define _UART_PORT_2_PD5_6                      8       // PD5 PD6
#define _UART_PORT_4_PC10_11                    9       // PD5 PD6
#define _UART_PORT_MAX                          _UART_PORT_4_PC10_11
#elif (defined(N32G45X))
#define _UART_PORT_7                            7       // PC4 PC5
#define _UART_PORT_7_PC2_3                      8       // PC2 PC3
#define _UART_PORT_MAX                          _UART_PORT_7_PC2_3
#else
#define _UART_PORT_MAX                          _UART_PORT_3
#endif


//eeprom chip type
#define _EEPROM_CHIP_24CM01                     0
#define _EEPROM_CHIP_24C32                      1
#define _EEPROM_CHIP_NO_USE                     2

//project name
#define _PROJECT_BASE_LINE                      0
#define _PROJECT_SCU41_MV                       10
#define _PROJECT_MHD1616S_TX                    29
#define _PROJECT_MHD1616S_RX                    30
#define _PROJECT_MHD1616S_SWITCHBOARD           31
#define _PROJECT_MHD0404S                       32
#define _PROJECT_X5                             33
#define _PROJECT_FIH2RX                         34
#define _PROJECT_FIH2TX                         35
#define _PROJECT_IMX_25_2VCTAJ                  36      // 机动平台视频处理单元图形工作站连接板 IMX_25_2VCTAJ0
#define _PROJECT_IMX_25_2VCTAP                  37      // 机动平台视频处理单元按键板 IMX_25_2VCTAP0
#define _PROJECT_FIH2TX_V2                      38      // 光矩TX第二版
#define _PROJECT_LMX_4OUHS_C                    39      // ms1826版本的LMX矩阵输出卡
#define _PROJECT_LMX_4IUHS_C                    40      // ms1826版本的LMX矩阵输入卡
#define _PROJECT_IMX_25_2VCTAC                  41      // 机动平台视频处理单元SDI卡 IMX_25_2VCTAC1
#define _PROJECT_MS1826EVB                      42      // ms1826DEMO板程序
#define _PROJECT_MUH0808S                       43      // MUH0808S AA0板程序
#define _PROJECT_MUH1616S_INPUT                 44      // MUH1616S AA0板程序 GD32F427
#define _PROJECT_MUH1616S_OUTPUT                45      // MUH1616S AB0板程序 N32G45X
#define _PROJECT_MUH0808SPro                    46      // MUH0808S AA1板程序 Pro程序


#ifndef _PROJECT_CURRENT_SELECT
#ifdef STM32F2XX_HD
#define _PROJECT_CURRENT_SELECT                 _PROJECT_MHD1616S_SWITCHBOARD
#elif defined(STM32F10X_RB)
#define _PROJECT_CURRENT_SELECT                 _PROJECT_X5
#elif defined(N32G45X)
#define _PROJECT_CURRENT_SELECT                 _PROJECT_MHD1616S_TX//_PROJECT_MHD1616S_TX
#elif (defined(GD32F4XX))
#define _PROJECT_CURRENT_SELECT                 _PROJECT_MUH0808S
#else
#define _PROJECT_CURRENT_SELECT                 _PROJECT_IMX_25_2VCTAJ
#endif
#endif

#if _PROJECT_BASE_LINE == _PROJECT_CURRENT_SELECT
#include "../USER/project_file/BASE_LINE/Project_Macro_Define_Management_Base_Line.h"
#elif (_PROJECT_SCU41_MV == _PROJECT_CURRENT_SELECT)
#include "../USER/project_file/SCU41-MV/Project_Macro_Define_Management_SCU41_MV.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_TX)
#include "../USER/project_file/MHD1616S-TX/Project_Macro_Define_Management_MHD1616S_TX.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_RX)
#include "../USER/project_file/MHD1616S-RX/Project_Macro_Define_Management_MHD1616S_RX.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_SWITCHBOARD)
#include "../USER/project_file/MHD1616S-SWITCHBOARD/Project_Macro_Define_Management_MHD1616S_SWITCHBOARD.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD0404S)
#include "../USER/project_file/MHD0404S/Project_Macro_Define_Management_MHD0404S.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_X5)
#include "../USER/project_file/X5/Project_Macro_Define_Management_X5.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2RX)
#include "../USER/project_file/FIH2RX/Project_Macro_Define_Management_FIH2RX.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2TX)
#include "../USER/project_file/FIH2TX/Project_Macro_Define_Management_FIH2TX.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAJ)
#include "../USER/project_file/IMX_25_2VCTAJ/Project_Macro_Define_Management_IMX_25_2VCTAJ.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAP)
#include "../USER/project_file/IMX_25_2VCTAP/Project_Macro_Define_Management_IMX_25_2VCTAP.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2TX_V2)
#include "../USER/project_file/FIH2TX_V2/Project_Macro_Define_Management_FIH2TX_V2.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_LMX_4OUHS_C)
#include "../USER/project_file/LMX_4OUHS_C/Project_Macro_Define_Management_4OUHS_C.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_LMX_4IUHS_C)
#include "../USER/project_file/LMX_4IUHS_C/Project_Macro_Define_Management_4IUHS_C.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAC)
#include "../USER/project_file/IMX_25_2VCTAC/Project_Macro_Define_Management_IMX_25_2VCTAC.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MS1826EVB)
#include "../USER/project_file/MS1826EVB/Project_Macro_Define_Management_MS1826EVB.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH0808S)
#include "../USER/project_file/MUH0808S/Project_Macro_Define_Management_MUH0808S.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH1616S_INPUT)
#include "../USER/project_file/MUH1616S_INPUT/Project_Macro_Define_Management_MUH1616S_INPUT.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH1616S_OUTPUT)
#include "../USER/project_file/MUH1616S_OUTPUT/Project_Macro_Define_Management_MUH1616S_OUTPUT.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH0808SPro)
#include "../USER/project_file/MUH0808SPro/Project_Macro_Define_Management_MUH0808SPro.h"

#else
#error "Add your project file"
#endif

//hdmi input number & hdmi output number
#ifndef _HDMI_I_PORT_NUM
#if (_PRJ_TYPE_SWITCH_SPLITER == _PRJ_SPLITER_ONLY)
    #define _HDMI_I_PORT_NUM        1
#else
    #if (_SWITCHER_TYPE_IN_OUT == _SWITCHER_TYPE_2_TO_1)
    #define _HDMI_I_PORT_NUM        2
    #elif (_SWITCHER_TYPE_IN_OUT == _SWITCHER_TYPE_4_TO_1)
    #define _HDMI_I_PORT_NUM        4
    #elif (_SWITCHER_TYPE_IN_OUT == _SWITCHER_TYPE_4_TO_3)
    #define _HDMI_I_PORT_NUM        4
    #elif (_SWITCHER_TYPE_IN_OUT == _SWITCHER_TYPE_6_TO_1)
    #define _HDMI_I_PORT_NUM        6
    #elif (_SWITCHER_TYPE_IN_OUT == _SWITCHER_TYPE_4_TO_1_SEAMLESS)
    #define _HDMI_I_PORT_NUM        4
    #elif (_SWITCHER_TYPE_IN_OUT == _SWITCHER_TYPE_6_TO_1_SEAMLESS)
    #define _HDMI_I_PORT_NUM        6
    #endif
#endif
#endif

#ifndef _HDMI_O_PORT_NUM
#if (_PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_ONLY)
    #define _HDMI_O_PORT_NUM        1
#else
    #if (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8)
    #define _HDMI_O_PORT_NUM        8
    #elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_4)
    #define _HDMI_O_PORT_NUM        4
    #elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_2)
    #define _HDMI_O_PORT_NUM        2
    #elif (_SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_24)
    #define _HDMI_O_PORT_NUM        24
    #endif
#endif
#endif

#include "Project_Macro_Define_Common_Management.h"
#include "Project_FactoryDefaultValue.h"

#endif






