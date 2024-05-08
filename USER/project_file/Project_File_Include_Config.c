#ifndef __PROJECT_FILE_INCLUDE_CONFIG_C_
#define __PROJECT_FILE_INCLUDE_CONFIG_C_

#if _PROJECT_BASE_LINE == _PROJECT_CURRENT_SELECT

#elif (_PROJECT_SCU41_MV == _PROJECT_CURRENT_SELECT)
    #include "../USER/project_file/SCU41-MV/Project_Var_Fun_Config_SCU41_MV.c"
    #include "../USER/project_file/SCU41-MV/Uart_Response_SCU41_MV.c"
    #include "../USER/project_file/SCU41-MV/Uart_Response_SCU41_MV_User_Func.c"
    #include "../USER/project_file/SCU41-MV/Key_Response_SCU41_MV.c"
    #include "../USER/project_file/SCU41-MV/Project_Board_Config_SCU41_MV.c"

#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_TX)
    #include "../USER/project_file/MHD1616S-TX/Project_Var_Fun_Config_MHD1616S_TX.c"
    #include "../USER/project_file/MHD1616S-TX/Uart_Response_MHD1616S_TX.c"
    #include "../USER/project_file/MHD1616S-TX/Uart_Response_MHD1616S_TX_User_Func.c"
    #include "../USER/project_file/MHD1616S-TX/Key_Response_MHD1616S_TX.c"
    #include "../USER/project_file/MHD1616S-TX/Project_Board_Config_MHD1616S_TX.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_RX)
    #include "../USER/project_file/MHD1616S-RX/Project_Var_Fun_Config_MHD1616S_RX.c"
    #include "../USER/project_file/MHD1616S-RX/Uart_Response_MHD1616S_RX.c"
    #include "../USER/project_file/MHD1616S-RX/Uart_Response_MHD1616S_RX_User_Func.c"
    #include "../USER/project_file/MHD1616S-RX/Key_Response_MHD1616S_RX.c"
    #include "../USER/project_file/MHD1616S-RX/Project_Board_Config_MHD1616S_RX.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD1616S_SWITCHBOARD)
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Project_Var_Fun_Config_MHD1616S_SWITCHBOARD.c"
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Uart_Response_MHD1616S_SWITCHBOARD.c"
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Uart_Response_MHD1616S_SWITCHBOARD_User_Func.c"
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Key_Response_MHD1616S_SWITCHBOARD.c"
    #include "../USER/project_file/MHD1616S-SWITCHBOARD/Project_Board_Config_MHD1616S_SWITCHBOARD.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MHD0404S)
    #include "../USER/project_file/MHD0404S/Project_Var_Fun_Config_MHD0404S.c"
    #include "../USER/project_file/MHD0404S/Uart_Response_MHD0404S.c"
    #include "../USER/project_file/MHD0404S/Uart_Response_MHD0404S_User_Func.c"
    #include "../USER/project_file/MHD0404S/Key_Response_MHD0404S.c"
    #include "../USER/project_file/MHD0404S/Project_Board_Config_MHD0404S.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_X5)
    #include "../USER/project_file/X5/Project_Var_Fun_Config_X5.c"
    #include "../USER/project_file/X5/Uart_Response_X5.c"
    #include "../USER/project_file/X5/Uart_Response_X5_User_Func.c"
    #include "../USER/project_file/X5/Key_Response_X5.c"
    #include "../USER/project_file/X5/Project_Board_Config_X5.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2RX)
    #include "../USER/project_file/FIH2RX/Project_Var_Fun_Config_FIH2RX.c"
    #include "../USER/project_file/FIH2RX/Uart_Response_FIH2RX.c"
    #include "../USER/project_file/FIH2RX/Uart_Response_FIH2RX_User_Func.c"
    #include "../USER/project_file/FIH2RX/Key_Response_FIH2RX.c"
    #include "../USER/project_file/FIH2RX/Project_Board_Config_FIH2RX.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2TX)
    #include "../USER/project_file/FIH2TX/Project_Var_Fun_Config_FIH2TX.c"
    #include "../USER/project_file/FIH2TX/Uart_Response_FIH2TX.c"
    #include "../USER/project_file/FIH2TX/Uart_Response_FIH2TX_User_Func.c"
    #include "../USER/project_file/FIH2TX/Key_Response_FIH2TX.c"
    #include "../USER/project_file/FIH2TX/Project_Board_Config_FIH2TX.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAJ)
    #include "../USER/project_file/IMX_25_2VCTAJ/Project_Var_Fun_Config_IMX_25_2VCTAJ.c"
    #include "../USER/project_file/IMX_25_2VCTAJ/Uart_Response_IMX_25_2VCTAJ.c"
    #include "../USER/project_file/IMX_25_2VCTAJ/Uart_Response_IMX_25_2VCTAJ_User_Func.c"
    #include "../USER/project_file/IMX_25_2VCTAJ/Key_Response_IMX_25_2VCTAJ.c"
    #include "../USER/project_file/IMX_25_2VCTAJ/Project_Board_Config_IMX_25_2VCTAJ.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAP)
    #include "../USER/project_file/IMX_25_2VCTAP/Project_Var_Fun_Config_IMX_25_2VCTAP.c"
    #include "../USER/project_file/IMX_25_2VCTAP/Uart_Response_IMX_25_2VCTAP.c"
    #include "../USER/project_file/IMX_25_2VCTAP/Uart_Response_IMX_25_2VCTAP_User_Func.c"
    #include "../USER/project_file/IMX_25_2VCTAP/Key_Response_IMX_25_2VCTAP.c"
    #include "../USER/project_file/IMX_25_2VCTAP/Project_Board_Config_IMX_25_2VCTAP.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_FIH2TX_V2)
    #include "../USER/project_file/FIH2TX_V2/Project_Var_Fun_Config_FIH2TX_V2.c"
    #include "../USER/project_file/FIH2TX_V2/Uart_Response_FIH2TX_V2.c"
    #include "../USER/project_file/FIH2TX_V2/Uart_Response_FIH2TX_V2_User_Func.c"
    #include "../USER/project_file/FIH2TX_V2/Key_Response_FIH2TX_V2.c"
    #include "../USER/project_file/FIH2TX_V2/Project_Board_Config_FIH2TX_V2.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_LMX_4OUHS_C)
    #include "../USER/project_file/LMX_4OUHS_C/Project_Var_Fun_Config_4OUHS_C.c"
    #include "../USER/project_file/LMX_4OUHS_C/Uart_Response_4OUHS_C.c"
    #include "../USER/project_file/LMX_4OUHS_C/Uart_Response_4OUHS_C_User_Func.c"
    #include "../USER/project_file/LMX_4OUHS_C/Key_Response_4OUHS_C.c"
    #include "../USER/project_file/LMX_4OUHS_C/Project_Board_Config_4OUHS_C.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_LMX_4IUHS_C)
    #include "../USER/project_file/LMX_4IUHS_C/Project_Var_Fun_Config_4IUHS_C.c"
    #include "../USER/project_file/LMX_4IUHS_C/Uart_Response_4IUHS_C.c"
    #include "../USER/project_file/LMX_4IUHS_C/Uart_Response_4IUHS_C_User_Func.c"
    #include "../USER/project_file/LMX_4IUHS_C/Key_Response_4IUHS_C.c"
    #include "../USER/project_file/LMX_4IUHS_C/Project_Board_Config_4IUHS_C.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_IMX_25_2VCTAC)
    #include "../USER/project_file/IMX_25_2VCTAC/Project_Var_Fun_Config_IMX_25_2VCTAC.c"
    #include "../USER/project_file/IMX_25_2VCTAC/Uart_Response_IMX_25_2VCTAC.c"
    #include "../USER/project_file/IMX_25_2VCTAC/Uart_Response_IMX_25_2VCTAC_User_Func.c"
    #include "../USER/project_file/IMX_25_2VCTAC/Key_Response_IMX_25_2VCTAC.c"
    #include "../USER/project_file/IMX_25_2VCTAC/Project_Board_Config_IMX_25_2VCTAC.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MS1826EVB)
    #include "../USER/project_file/MS1826EVB/Project_Var_Fun_Config_MS1826EVB.c"
    #include "../USER/project_file/MS1826EVB/Uart_Response_MS1826EVB.c"
    #include "../USER/project_file/MS1826EVB/Uart_Response_MS1826EVB_User_Func.c"
    #include "../USER/project_file/MS1826EVB/Key_Response_MS1826EVB.c"
    #include "../USER/project_file/MS1826EVB/Project_Board_Config_MS1826EVB.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH0808S)
    #include "../USER/project_file/MUH0808S/Project_Var_Fun_Config_MUH0808S.c"
    #include "../USER/project_file/MUH0808S/Uart_Response_MUH0808S.c"
    #include "../USER/project_file/MUH0808S/Uart_Response_MUH0808S_User_Func.c"
    #include "../USER/project_file/MUH0808S/Key_Response_MUH0808S.c"
    #include "../USER/project_file/MUH0808S/Project_Board_Config_MUH0808S.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH1616S_INPUT)
    #include "../USER/project_file/MUH1616S_INPUT/Project_Var_Fun_Config_MUH1616S_INPUT.c"
    #include "../USER/project_file/MUH1616S_INPUT/Uart_Response_MUH1616S_INPUT.c"
    #include "../USER/project_file/MUH1616S_INPUT/Uart_Response_MUH1616S_INPUT_User_Func.c"
    #include "../USER/project_file/MUH1616S_INPUT/Key_Response_MUH1616S_INPUT.c"
    #include "../USER/project_file/MUH1616S_INPUT/Project_Board_Config_MUH1616S_INPUT.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH1616S_OUTPUT)
    #include "../USER/project_file/MUH1616S_OUTPUT/Project_Var_Fun_Config_MUH1616S_OUTPUT.c"
    #include "../USER/project_file/MUH1616S_OUTPUT/Uart_Response_MUH1616S_OUTPUT.c"
    #include "../USER/project_file/MUH1616S_OUTPUT/Uart_Response_MUH1616S_OUTPUT_User_Func.c"
    #include "../USER/project_file/MUH1616S_OUTPUT/Key_Response_MUH1616S_OUTPUT.c"
    #include "../USER/project_file/MUH1616S_OUTPUT/Project_Board_Config_MUH1616S_OUTPUT.c"
#elif (_PROJECT_CURRENT_SELECT == _PROJECT_MUH0808SPro)
    #include "../USER/project_file/MUH0808SPro/Project_Var_Fun_Config_MUH0808SPro.c"
    #include "../USER/project_file/MUH0808SPro/Uart_Response_MUH0808SPro.c"
    #include "../USER/project_file/MUH0808SPro/Uart_Response_MUH0808SPro_User_Func.c"
    #include "../USER/project_file/MUH0808SPro/Key_Response_MUH0808SPro.c"
    #include "../USER/project_file/MUH0808SPro/Project_Board_Config_MUH0808SPro.c"

#endif

#endif














