#ifndef __PROJECT_FILE_INCLUDE_CONFIG_H_
#define __PROJECT_FILE_INCLUDE_CONFIG_H_

#if _PROJECT_BASE_LINE == _PROJECT_CURRENT_SELECT



#elif (_PROJECT_SCU41_MV == _PROJECT_CURRENT_SELECT)
    #include "../USER/project_file/SCU41-MV/Project_Var_Fun_Config_SCU41_MV.h"
    #include "../USER/project_file/SCU41-MV/Uart_Response_SCU41_MV.h"
    #include "../USER/project_file/SCU41-MV/Uart_Response_SCU41_MV_User_Func.h"
    #include "../USER/project_file/SCU41-MV/Key_Response_SCU41_MV.h"
    #include "../USER/project_file/SCU41-MV/Project_Board_Config_SCU41_MV.h"

#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_TX)
    #include "../USER/project_file/MHD1616S-TX/Project_Var_Fun_Config_MHD1616S_TX.h"
    #include "../USER/project_file/MHD1616S-TX/Uart_Response_MHD1616S_TX.h"
    #include "../USER/project_file/MHD1616S-TX/Uart_Response_MHD1616S_TX_User_Func.h"
    #include "../USER/project_file/MHD1616S-TX/Key_Response_MHD1616S_TX.h"
    #include "../USER/project_file/MHD1616S-TX/Project_Board_Config_MHD1616S_TX.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_RX)
    #include "../USER/project_file/MHD1616S-RX/Project_Var_Fun_Config_MHD1616S_RX.h"
    #include "../USER/project_file/MHD1616S-RX/Uart_Response_MHD1616S_RX.h"
    #include "../USER/project_file/MHD1616S-RX/Uart_Response_MHD1616S_RX_User_Func.h"
    #include "../USER/project_file/MHD1616S-RX/Key_Response_MHD1616S_RX.h"
    #include "../USER/project_file/MHD1616S-RX/Project_Board_Config_MHD1616S_RX.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_SWITCHBOARD)
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Project_Var_Fun_Config_MHD1616S_SWITCHBOARD.h"
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Uart_Response_MHD1616S_SWITCHBOARD.h"
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Uart_Response_MHD1616S_SWITCHBOARD_User_Func.h"
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Key_Response_MHD1616S_SWITCHBOARD.h"
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Project_Board_Config_MHD1616S_SWITCHBOARD.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD0404S)
    #include "../USER/project_file/MHD0404S/Project_Var_Fun_Config_MHD0404S.h"
    #include "../USER/project_file/MHD0404S/Uart_Response_MHD0404S.h"
    #include "../USER/project_file/MHD0404S/Uart_Response_MHD0404S_User_Func.h"
    #include "../USER/project_file/MHD0404S/Key_Response_MHD0404S.h"
    #include "../USER/project_file/MHD0404S/Project_Board_Config_MHD0404S.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_X5)
    #include "../USER/project_file/X5/Project_Var_Fun_Config_X5.h"
    #include "../USER/project_file/X5/Uart_Response_X5.h"
    #include "../USER/project_file/X5/Uart_Response_X5_User_Func.h"
    #include "../USER/project_file/X5/Key_Response_X5.h"
    #include "../USER/project_file/X5/Project_Board_Config_X5.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2RX)
    #include "../USER/project_file/FIH2RX/Project_Var_Fun_Config_FIH2RX.h"
    #include "../USER/project_file/FIH2RX/Uart_Response_FIH2RX.h"
    #include "../USER/project_file/FIH2RX/Uart_Response_FIH2RX_User_Func.h"
    #include "../USER/project_file/FIH2RX/Key_Response_FIH2RX.h"
    #include "../USER/project_file/FIH2RX/Project_Board_Config_FIH2RX.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2TX)
    #include "../USER/project_file/FIH2TX/Project_Var_Fun_Config_FIH2TX.h"
    #include "../USER/project_file/FIH2TX/Uart_Response_FIH2TX.h"
    #include "../USER/project_file/FIH2TX/Uart_Response_FIH2TX_User_Func.h"
    #include "../USER/project_file/FIH2TX/Key_Response_FIH2TX.h"
    #include "../USER/project_file/FIH2TX/Project_Board_Config_FIH2TX.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAJ)
    #include "../USER/project_file/IMX_25_2VCTAJ/Project_Var_Fun_Config_IMX_25_2VCTAJ.h"
    #include "../USER/project_file/IMX_25_2VCTAJ/Uart_Response_IMX_25_2VCTAJ.h"
    #include "../USER/project_file/IMX_25_2VCTAJ/Uart_Response_IMX_25_2VCTAJ_User_Func.h"
    #include "../USER/project_file/IMX_25_2VCTAJ/Key_Response_IMX_25_2VCTAJ.h"
    #include "../USER/project_file/IMX_25_2VCTAJ/Project_Board_Config_IMX_25_2VCTAJ.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAP)
    #include "../USER/project_file/IMX_25_2VCTAP/Project_Var_Fun_Config_IMX_25_2VCTAP.h"
    #include "../USER/project_file/IMX_25_2VCTAP/Uart_Response_IMX_25_2VCTAP.h"
    #include "../USER/project_file/IMX_25_2VCTAP/Uart_Response_IMX_25_2VCTAP_User_Func.h"
    #include "../USER/project_file/IMX_25_2VCTAP/Key_Response_IMX_25_2VCTAP.h"
    #include "../USER/project_file/IMX_25_2VCTAP/Project_Board_Config_IMX_25_2VCTAP.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2TX_V2)
    #include "../USER/project_file/FIH2TX_V2/Project_Var_Fun_Config_FIH2TX_V2.h"
    #include "../USER/project_file/FIH2TX_V2/Uart_Response_FIH2TX_V2.h"
    #include "../USER/project_file/FIH2TX_V2/Uart_Response_FIH2TX_V2_User_Func.h"
    #include "../USER/project_file/FIH2TX_V2/Key_Response_FIH2TX_V2.h"
    #include "../USER/project_file/FIH2TX_V2/Project_Board_Config_FIH2TX_V2.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_LMX_4OUHS_C)
    #include "../USER/project_file/LMX_4OUHS_C/Project_Var_Fun_Config_4OUHS_C.h"
    #include "../USER/project_file/LMX_4OUHS_C/Uart_Response_4OUHS_C.h"
    #include "../USER/project_file/LMX_4OUHS_C/Uart_Response_4OUHS_C_User_Func.h"
    #include "../USER/project_file/LMX_4OUHS_C/Key_Response_4OUHS_C.h"
    #include "../USER/project_file/LMX_4OUHS_C/Project_Board_Config_4OUHS_C.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_LMX_4IUHS_C)
    #include "../USER/project_file/LMX_4IUHS_C/Project_Var_Fun_Config_4IUHS_C.h"
    #include "../USER/project_file/LMX_4IUHS_C/Uart_Response_4IUHS_C.h"
    #include "../USER/project_file/LMX_4IUHS_C/Uart_Response_4IUHS_C_User_Func.h"
    #include "../USER/project_file/LMX_4IUHS_C/Key_Response_4IUHS_C.h"
    #include "../USER/project_file/LMX_4IUHS_C/Project_Board_Config_4IUHS_C.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAC)
    #include "../USER/project_file/IMX_25_2VCTAC/Project_Var_Fun_Config_IMX_25_2VCTAC.h"
    #include "../USER/project_file/IMX_25_2VCTAC/Uart_Response_IMX_25_2VCTAC.h"
    #include "../USER/project_file/IMX_25_2VCTAC/Uart_Response_IMX_25_2VCTAC_User_Func.h"
    #include "../USER/project_file/IMX_25_2VCTAC/Key_Response_IMX_25_2VCTAC.h"
    #include "../USER/project_file/IMX_25_2VCTAC/Project_Board_Config_IMX_25_2VCTAC.h"

#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MS1826EVB)
    #include "../USER/project_file/MS1826EVB/Project_Var_Fun_Config_MS1826EVB.h"
    #include "../USER/project_file/MS1826EVB/Uart_Response_MS1826EVB.h"
    #include "../USER/project_file/MS1826EVB/Uart_Response_MS1826EVB_User_Func.h"
    #include "../USER/project_file/MS1826EVB/Key_Response_MS1826EVB.h"
    #include "../USER/project_file/MS1826EVB/Project_Board_Config_MS1826EVB.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH0808S)
    #include "../USER/project_file/MUH0808S/Project_Var_Fun_Config_MUH0808S.h"
    #include "../USER/project_file/MUH0808S/Uart_Response_MUH0808S.h"
    #include "../USER/project_file/MUH0808S/Uart_Response_MUH0808S_User_Func.h"
    #include "../USER/project_file/MUH0808S/Key_Response_MUH0808S.h"
    #include "../USER/project_file/MUH0808S/Project_Board_Config_MUH0808S.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH1616S_INPUT)
    #include "../USER/project_file/MUH1616S_INPUT/Project_Var_Fun_Config_MUH1616S_INPUT.h"
    #include "../USER/project_file/MUH1616S_INPUT/Uart_Response_MUH1616S_INPUT.h"
    #include "../USER/project_file/MUH1616S_INPUT/Uart_Response_MUH1616S_INPUT_User_Func.h"
    #include "../USER/project_file/MUH1616S_INPUT/Key_Response_MUH1616S_INPUT.h"
    #include "../USER/project_file/MUH1616S_INPUT/Project_Board_Config_MUH1616S_INPUT.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH1616S_OUTPUT)
    #include "../USER/project_file/MUH1616S_OUTPUT/Project_Var_Fun_Config_MUH1616S_OUTPUT.h"
    #include "../USER/project_file/MUH1616S_OUTPUT/Uart_Response_MUH1616S_OUTPUT.h"
    #include "../USER/project_file/MUH1616S_OUTPUT/Uart_Response_MUH1616S_OUTPUT_User_Func.h"
    #include "../USER/project_file/MUH1616S_OUTPUT/Key_Response_MUH1616S_OUTPUT.h"
    #include "../USER/project_file/MUH1616S_OUTPUT/Project_Board_Config_MUH1616S_OUTPUT.h"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH0808SPro)
    #include "../USER/project_file/MUH0808SPro/Project_Var_Fun_Config_MUH0808SPro.h"
    #include "../USER/project_file/MUH0808SPro/Uart_Response_MUH0808SPro.h"
    #include "../USER/project_file/MUH0808SPro/Uart_Response_MUH0808SPro_User_Func.h"
    #include "../USER/project_file/MUH0808SPro/Key_Response_MUH0808SPro.h"
    #include "../USER/project_file/MUH0808SPro/Project_Board_Config_MUH0808SPro.h"
#endif

#endif













