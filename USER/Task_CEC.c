/**
 *****************************************************************************
  * Update
  * @Version : V1.0.1
  * @By      : dengzhiqin
  * @Date    : 2018 / 01 / 04
  * @Brief   : task of cec
  * @Rewriter:     wf8421
  * @Rewrite Date: 2020-8-6
  * @History :  更改CEC接收协议，更改CEC接收命令
  *****************************************************************************
**/
#include "includes.h"

#if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)

#ifndef DBG_CEC_RX
#define DBG_CEC_RX              0
#endif

#if DBG_CEC_RX
#define CEC_DBG_PRINT(x)        dbg_printf x
#else
#define CEC_DBG_PRINT(x)
#endif
#ifndef ENABLE_CEC_USER2
#define ENABLE_CEC_USER2            1
#endif //add by wf8421

#ifndef ENABLE_USER_ARC_RX
#define ENABLE_USER_ARC_RX          0
#endif //add by wf8421

#ifndef ENABLE_USER_ARC_TX
#define ENABLE_USER_ARC_TX          0
#endif //add by wf8421

#ifndef ENABLE_USER_CTRL_RELEASED
#define ENABLE_USER_CTRL_RELEASED   1
#endif //add by wf8421

#define TIMES_USER_CTRL_RELEASED    300

#define MAX_OSD_NAME_LEN    15
stCecPinFun_t g_stCecPinFunTable[NUM_OF_CEC_PINS_MAX];
stCecTxMsg_t g_tx_cecMsg;
stCecRxMsg_t g_rx_cecMsg[NUM_OF_CEC_PINS_MAX];
stCecFrameDetect_t g_cec_frame_dct[NUM_OF_CEC_PINS_MAX];
#if _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION
uint8_t g_u8SWSrcInitCecCnt = 0;
#endif
#if DBG_CEC_RX
bool g_bRxDbgRecevieFlag[NUM_OF_CEC_PINS_MAX];
stCecMsg_t g_stCecRxDbgMsg[NUM_OF_CEC_PINS_MAX];
stCecMsg_t g_stCecTxDbgMsg[NUM_OF_CEC_PINS_MAX];
bool g_bCecTxDbgFlag[NUM_OF_CEC_PINS_MAX];
#endif
u8 g_u8OsdName[NUM_OF_CEC_PINS_MAX][MAX_OSD_NAME_LEN];
u8 g_u8CecDisplayInitSta[NUM_OF_CEC_PINS_MAX];
u8 g_u8CecInitPollingCounter[NUM_OF_CEC_PINS_MAX];
stCecRelinkAction_t g_stCecRelinkAction[NUM_OF_CEC_PINS_MAX];

static u8 sg_u8VendorID[3] = {0x55,0x48,0x44};
static u32 sg_u32CecTimerCnt[NUM_OF_CEC_PINS_MAX];
extern QueueHandle_t            xQueue_CecTx;
extern stProjectGlobalAllVar_t  g_stPrjGloVar;
extern const stCecDeclare_t     stProjectCecWorkTable[];
extern const stUartDeclare_t stProjectUartWorkTable[];

#if ENABLE_CEC_USER2
#define CEC_REV_CNT     5

typedef enum _emCecState_t_
{
    emCecSta_None = 0,
    emCecSta_Recevie,
    emCecSta_RevOk,
    emCecSta_SendAck,
    emCecSta_Max,
}emCecState_t;

typedef enum _emCecAckType_t_
{
    emCecAckType_None = 0,
    emCecAckType_0,
    emCecAckType_1,

    emCecAckType_Max,
}emCecAckType_t;

typedef struct _stCecRevType_t_
{
    u8 u8Ware[150];
    u16 u16BitCnt;
    emCecState_t eCecSta;
    u16 u16HighSta;
    u16 u16LowSta;
    u8 u8PrevLineSta;
    u8 u8CurLineSta;
    u8 u8SetAck;
}stCecRevType_t,*pstCecRevType_t;

stCecRevType_t g_stCecRevData[NUM_OF_CEC_PINS_MAX];
#define CEC_RX_CNT     5
bool g_bRxRecevieFlag[NUM_OF_CEC_PINS_MAX][CEC_RX_CNT];
stCecMsg_t g_stCecRxMsg[NUM_OF_CEC_PINS_MAX][CEC_RX_CNT];
u8 g_u8CecRxCnt[NUM_OF_CEC_PINS_MAX];

#define SCAN_CEC_END        50
#endif

#if ENABLE_CEC_USER2
void CecPinExtiEnable(void);
u8 cec_receive_logic(u8 u8Low,u8 u8High);
u8 cec_receive_logic_start(u8 u8Low,u8 u8High);
static void mapp_CecRxCmdAction(void);
//****************************************************************************
static void Cec_Cmd_OTPFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_RoutingCtlFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_StandByFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_OTRFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_SysInfoFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_DeckCtrlFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_VenSpecCmdFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_DevOSDTransferFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_RemoteCtlPassthroughFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_PowerStatusFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_GeneralProtocolMessage(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_SysAudioCtlFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
static void Cec_Cmd_ARCFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr);
#endif
#if ENABLE_USER_ARC_RX
void mapp_CecArcInit(uint8_t u8Port);
#endif
static void mapp_CecRelinkActionProcess(void);
static void mapp_CecTimerInit (void);

#if _SYSTEM_SUPPORT_RTOS
void vTaskCEC(void *pvParameters)
{
    pstCecTxMsg_t       ptCecMsg;
    #if 1//(_ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION||DBG_CEC_RX)
    const TickType_t    xTicksToWait = 20;//
    #else
    const TickType_t    xTicksToWait = portMAX_DELAY;//0;//20;//
    #endif
    BaseType_t          xResult;

    (void)pvParameters;

    hdmi_cec_init();
    cec_init_port();
    for(;;)
    {
        xResult = xQueueReceive(xQueue_CecTx, (void *)&ptCecMsg,\
                                    (TickType_t)xTicksToWait);
        if(xResult == pdPASS)
        {
            cec_tx_send_frame(ptCecMsg);
        }

        #if _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION
        cec_init_to_function(FALSE);
        #endif
        //mapp_CecInitPort();
        mapp_CecRelinkActionProcess();

        #if (DBG_CEC_RX&&(!ENABLE_CEC_USER2))
        {
            u8 cec_index;
            u8 i;
            static u8 sl_u8PreOpcode[NUM_OF_CEC_PINS_MAX];

            for(cec_index=0; cec_index<NUM_OF_CEC_PINS_MAX; cec_index++)
            {
                if(g_bRxDbgRecevieFlag[cec_index] == TRUE)
                {
                    if(sl_u8PreOpcode[cec_index]!=g_stCecRxDbgMsg[cec_index].ucOpCode)
                    {
                        sl_u8PreOpcode[cec_index]=g_stCecRxDbgMsg[cec_index].ucOpCode;
                        CEC_DBG_PRINT(("=============start %d======================\n",cec_index));
                        CEC_DBG_PRINT(("ucHeader              ==0x%x\n",g_stCecRxDbgMsg[cec_index].ucHeader));
                        CEC_DBG_PRINT(("ucOpCode              ==0x%x\n",g_stCecRxDbgMsg[cec_index].ucOpCode));
                        CEC_DBG_PRINT(("ucLen                 ==%d\n",g_stCecRxDbgMsg[cec_index].ucLen));
                        CEC_DBG_PRINT(("ucOperand:start\n"));
                        for(i=0;i<g_stCecRxDbgMsg[cec_index].ucLen;i++)
                        {
                            CEC_DBG_PRINT(("0x%02x ",g_stCecRxDbgMsg[cec_index].ucOperand[i]));
                        }
                        CEC_DBG_PRINT(("\n"));
                        CEC_DBG_PRINT(("ucOperand:end\n"));
                        CEC_DBG_PRINT(("=============end %d========================\n",cec_index));
                    }
                    g_bRxDbgRecevieFlag[cec_index]= FALSE;
                }
            }
        }
        #endif

        #if ENABLE_CEC_USER2
        mapp_CecRxCmdAction();
        #endif

        vTaskDelay(10);
    }
}
#else
void appCecInit(void)
{
    hdmi_cec_init();
    cec_init_port();
}

void vTaskCEC(void *pvParameters)
{
    (void)pvParameters;

    #if _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION
    cec_init_to_function(FALSE);
    #endif
    //mapp_CecInitPort();
    mapp_CecRelinkActionProcess();

    #if (DBG_CEC_RX&&(!ENABLE_CEC_USER2))
    {
        u8 cec_index;
        u8 i;
        static u8 sl_u8PreOpcode[NUM_OF_CEC_PINS_MAX];

        for(cec_index=0; cec_index<NUM_OF_CEC_PINS_MAX; cec_index++)
        {
            if(g_bRxDbgRecevieFlag[cec_index] == TRUE)
            {
                if(sl_u8PreOpcode[cec_index]!=g_stCecRxDbgMsg[cec_index].ucOpCode)
                {
                    sl_u8PreOpcode[cec_index]=g_stCecRxDbgMsg[cec_index].ucOpCode;
                    CEC_DBG_PRINT(("=============start %d======================\n",cec_index));
                    CEC_DBG_PRINT(("ucHeader              ==0x%x\n",g_stCecRxDbgMsg[cec_index].ucHeader));
                    CEC_DBG_PRINT(("ucOpCode              ==0x%x\n",g_stCecRxDbgMsg[cec_index].ucOpCode));
                    CEC_DBG_PRINT(("ucLen                 ==%d\n",g_stCecRxDbgMsg[cec_index].ucLen));
                    CEC_DBG_PRINT(("ucOperand:start\n"));
                    for(i=0;i<g_stCecRxDbgMsg[cec_index].ucLen;i++)
                    {
                        CEC_DBG_PRINT(("0x%02x ",g_stCecRxDbgMsg[cec_index].ucOperand[i]));
                    }
                    CEC_DBG_PRINT(("\n"));
                    CEC_DBG_PRINT(("ucOperand:end\n"));
                    CEC_DBG_PRINT(("=============end %d========================\n",cec_index));
                }
                g_bRxDbgRecevieFlag[cec_index]= FALSE;
            }
        }
    }
    #endif

    #if ENABLE_CEC_USER2
    mapp_CecRxCmdAction();
    #endif
}

#endif

void hdmi_cec_init(void)
{
    pstCecDeclare_t pstCecPort, pstCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    uint8_t         u8Tmp;

    memset(&g_rx_cecMsg[0], 0, (sizeof(stCecRxMsg_t)*NUM_OF_CEC_PINS_MAX));
    memset(&g_cec_frame_dct[0], 0, (sizeof(stCecFrameDetect_t)*NUM_OF_CEC_PINS_MAX));
    memset(g_u8OsdName, 0, (NUM_OF_CEC_PINS_MAX)*MAX_OSD_NAME_LEN);
    memset(g_u8CecDisplayInitSta,emCecRelinkSta_None,NUM_OF_CEC_PINS_MAX);
    memset(g_u8CecInitPollingCounter,0,NUM_OF_CEC_PINS_MAX);
    memset(g_stCecRelinkAction,0,sizeof(stCecRelinkAction_t)*NUM_OF_CEC_PINS_MAX);
    mapp_CecTimerInit();

    pstCecPort = pstCecPortOrg;
    while(pstCecPort->pstCecGpioLoc != NULL)
    {
        u8Tmp = pstCecPort-pstCecPortOrg;
        if(pstCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Src)
        {
            g_rx_cecMsg[u8Tmp].addr_loc = pstCecPort->stCecCnntType.u8LocalAddr;
            g_rx_cecMsg[u8Tmp].addr_physical = CEC_ADDR_PHYSICAL_IN;
            g_rx_cecMsg[u8Tmp].addr_destination = CEC_LOGADDR_PLAYBACK1;//CEC_LOGADDR_BROADCAST;
        }
        else if(pstCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Dis)
        {
            g_rx_cecMsg[u8Tmp].addr_loc = pstCecPort->stCecCnntType.u8LocalAddr;
            g_rx_cecMsg[u8Tmp].addr_physical = 0x0000;//CEC_ADDR_PHYSICAL_OUT;
            g_rx_cecMsg[u8Tmp].addr_destination = CEC_LOGADDR_BROADCAST;
        }

        cec_pin_fun_register(u8Tmp);
        pstCecPort++;
    }


    #if ENABLE_CEC_USER2
    memset(g_bRxRecevieFlag,FALSE,NUM_OF_CEC_PINS_MAX*CEC_RX_CNT);
    memset(g_stCecRxMsg,0,sizeof(stCecMsg_t)*NUM_OF_CEC_PINS_MAX*CEC_RX_CNT);
    memset(g_u8CecRxCnt,0,NUM_OF_CEC_PINS_MAX);
    memset(g_stCecRevData,0,sizeof(stCecRevType_t)*NUM_OF_CEC_PINS_MAX);
    #endif
}

static void mapp_CecTimerProcess(void)
{
    int i;

    static volatile uint8_t     svu8TimerIntCnt = 0;

    // unit = 100ns
    svu8TimerIntCnt ++;
    if(svu8TimerIntCnt >= 10)
    {
        for(i=0;i<NUM_OF_CEC_PINS_MAX;i++)
        {
            if(sg_u32CecTimerCnt[i] > 0)
            {
                sg_u32CecTimerCnt[i]--;
            }
        }
        svu8TimerIntCnt = 0;
    }
}

static void mapp_CecTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<NUM_OF_CEC_PINS_MAX)
            {
                sg_u32CecTimerCnt[index] = m_sec;
            }
            break;
    }
}

//static u32 mapp_CecTimerGet(uint8_t eTimer)
//{
//    return(sg_u32CecTimerCnt[eTimer]);
//}

static void mapp_CecTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < NUM_OF_CEC_PINS_MAX; i++ )
    {
        sg_u32CecTimerCnt[ i ] = 0;
    }
}

static uint8_t mapp_CecTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < NUM_OF_CEC_PINS_MAX )
    {
        return( sg_u32CecTimerCnt[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}

void SetCecPinHigh(pstCecDeclare_t   pCecPort)
{
//    #ifdef STM32F2XX_HD
//    STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA]->BSRRL = pCecPort->pstCecGpioLoc->u16GpioPin;
//    #else
//    STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA]->BSRR = pCecPort->pstCecGpioLoc->u16GpioPin;
//    #endif

    GPIO_SetBits(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA],pCecPort->pstCecGpioLoc->u16GpioPin);
}

void SetCecPinLow(pstCecDeclare_t pCecPort)
{
//    #ifdef STM32F2XX_HD
//    STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA]->BSRRH = pCecPort->pstCecGpioLoc->u16GpioPin;
//    #else
//    STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA]->BRR = pCecPort->pstCecGpioLoc->u16GpioPin;
//    #endif
    GPIO_ResetBits(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA],pCecPort->pstCecGpioLoc->u16GpioPin);
}

uint8_t GetCecPinStatus(pstCecDeclare_t pCecPort)
{
//    if((STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA]->IDR&pCecPort->pstCecGpioLoc->u16GpioPin) != 0)
    if(GPIO_ReadInputDataBit(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA],pCecPort->pstCecGpioLoc->u16GpioPin) != Bit_RESET)
    {
        return (uint8_t)Bit_SET;
    }
    else
    {
        return (uint8_t)Bit_RESET;
    }
}

#if (defined(GD32F4XX))
void SetCecPinOutMode(pstCecDeclare_t pCecPort)
{
    gpio_mode_set(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA],
                GPIO_Mode_OUT,
                GPIO_PuPd_UP,
                pCecPort->pstCecGpioLoc->u16GpioPin);
    gpio_output_options_set(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA],
                    GPIO_OType_OD,
                    GPIO_OSPEED_50MHZ,
                    pCecPort->pstCecGpioLoc->u16GpioPin);
}

void SetCecPinInputMode(pstCecDeclare_t pCecPort)
{
    gpio_mode_set(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA],
                GPIO_Mode_IN,
                GPIO_PuPd_UP,
                pCecPort->pstCecGpioLoc->u16GpioPin);
}

#else
void SetCecPinOutMode(pstCecDeclare_t pCecPort)
{
#ifdef N32G45X
    GPIO_InitType    GPIO_InitStructure;
#else
	GPIO_InitTypeDef GPIO_InitStructure;
#endif

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef N32G45X
    GPIO_InitStructure.Pin        = pCecPort->pstCecGpioLoc->u16GpioPin;
#else
    GPIO_InitStructure.GPIO_Pin   = pCecPort->pstCecGpioLoc->u16GpioPin;
#endif

    #ifdef STM32F2XX_HD
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    #else
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    #endif
#ifdef N32G45X
    GPIO_InitPeripheral(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
#else
    GPIO_Init(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
#endif
}

void SetCecPinInputMode(pstCecDeclare_t pCecPort)
{
#ifdef N32G45X
    GPIO_InitType    GPIO_InitStructure;
#else
    GPIO_InitTypeDef GPIO_InitStructure;
#endif

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef N32G45X
    GPIO_InitStructure.Pin        = pCecPort->pstCecGpioLoc->u16GpioPin;
#else
    GPIO_InitStructure.GPIO_Pin   = pCecPort->pstCecGpioLoc->u16GpioPin;
#endif
    #ifdef STM32F2XX_HD
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    #else
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    #endif
#ifdef N32G45X
    GPIO_InitPeripheral(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
#else
    GPIO_Init(STM32_GPIO_PORT[pCecPort->pstCecGpioLoc->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
#endif

}
#endif

void cec_pin_fun_register(uint8_t u8CecIndex)
{
    pstCecPinFun_t pstFun = &g_stCecPinFunTable[u8CecIndex];

    pstFun->cecMsg_tx_flag = FALSE;

    pstFun->cecMsg_state = cec_state_none;
    pstFun->timer_cnt = 0;
    pstFun->bit_cnt = 0;
    pstFun->get_cnt = 0;

    memset(&pstFun->cecMsg_content, 0, sizeof(pstFun->cecMsg_content));
}

void cec_frame_detect(pstCecDeclare_t pCecPort)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    pstCecFrameDetect_t     pstDetect;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];
    pstDetect = &g_cec_frame_dct[pCecPort-pCecPortOrg];
    if(pstDetect->en_detect_mode)
    {
        pstDetect->cec_pin_cur_state = (GetCecPinStatus(pCecPort)==Bit_RESET)?ePinLevel_Low : ePinLevel_High;
        if((pstDetect->cec_pin_pre_state==ePinLevel_High)&&\
            (pstDetect->cec_pin_cur_state==ePinLevel_Low))
        {
            pstDetect->en_detect_mode = FALSE;

            pstFun->cecMsg_state = cec_state_start;
            //pstFun->cecMsg_tx_flag = FALSE;
            pstFun->timer_cnt = 0;
            pstFun->bit_cnt = 0;
            pstFun->get_cnt = 0;
        }
        else
        {
            pstDetect->cec_pin_pre_state = pstDetect->cec_pin_cur_state;
        }
    }
}

bool cec_send_start(pstCecDeclare_t pCecPort)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    bool                    bBreakFlag = FALSE;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    if(pstFun->timer_cnt == 0)
    {
        SetCecPinLow(pCecPort);
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }
    else if(pstFun->timer_cnt == Sbit_Nom_LD)
    {
        SetCecPinHigh(pCecPort);
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }
    else if(pstFun->timer_cnt==(Sbit_Nom_LD+Sbit_Nom_HD))
    {
        pstFun->cecMsg_state = cec_state_header;
        pstFun->timer_cnt = 0;
    }
    else
    {
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }

    return bBreakFlag;
}

bool cec_receive_data_bit(pstCecDeclare_t pCecPort, uint8_t* pu8Rbit);
bool cec_send_byte_by_state(pstCecDeclare_t pCecPort, bool bEom)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    bool                    bBreakFlag = FALSE;
    uint8_t                 u8SendByte;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    u8SendByte = ((pstFun->cecMsg_state==cec_state_header) ? pstFun->cecMsg_content->ucHeader:\
                ((pstFun->cecMsg_state==cec_state_opcode) ? pstFun->cecMsg_content->ucOpCode:\
                (pstFun->cecMsg_content->ucOperand[pstFun->cecMsg_state-cec_state_operand1])));

    if(pstFun->timer_cnt==0)
    {
        SetCecPinLow(pCecPort);
        pstFun->timer_cnt++;
        pstFun->bit_cnt = 0;
        pstFun->get_cnt = 0;

        bBreakFlag = TRUE;
    }
    //send bit 1
    else if((pstFun->timer_cnt==(Dbit1_Nom_LD+(pstFun->bit_cnt*(Dbit1_Nom_LD+Dbit1_Nom_HD))))&&\
        (((pstFun->bit_cnt<=7)&&((u8SendByte&(1<<(7-pstFun->bit_cnt)))>0))||\
        ((pstFun->bit_cnt==8)&&(bEom))))
    {
        SetCecPinHigh(pCecPort);
        pstFun->timer_cnt++;

        bBreakFlag = TRUE;
    }
    //send bit 1
    else if((pstFun->timer_cnt==((pstFun->bit_cnt+1)*(Dbit1_Nom_LD+Dbit1_Nom_HD)))&&\
        (((pstFun->bit_cnt<=7)&&((u8SendByte&(1<<(7-pstFun->bit_cnt)))>0))||\
        ((pstFun->bit_cnt==8)&&(bEom))))
    {
        SetCecPinLow(pCecPort);
        pstFun->timer_cnt++;
        pstFun->bit_cnt++;

        bBreakFlag = TRUE;
    }
    //send bit 0
    else if((pstFun->timer_cnt==(Dbit0_Nom_LD+(pstFun->bit_cnt*(Dbit0_Nom_LD+Dbit0_Nom_HD))))&&\
        (((pstFun->bit_cnt<=7)&&((u8SendByte&(1<<(7-pstFun->bit_cnt)))==0))||\
        ((pstFun->bit_cnt==8)&&(!bEom))))
    {
        SetCecPinHigh(pCecPort);
        pstFun->timer_cnt++;

        bBreakFlag = TRUE;
    }
    //send bit 0
    else if((pstFun->timer_cnt==((pstFun->bit_cnt+1)*(Dbit0_Nom_LD+Dbit0_Nom_HD)))&&\
        (((pstFun->bit_cnt<=7)&&((u8SendByte&(1<<(7-pstFun->bit_cnt)))==0))||\
        ((pstFun->bit_cnt==8)&&(!bEom))))
    {
        SetCecPinLow(pCecPort);
        pstFun->timer_cnt++;
        pstFun->bit_cnt++;

        bBreakFlag = TRUE;
    }
#if 0
    //ack
    else if(pstFun->bit_cnt==9)
    {
        if(pstFun->get_cnt==0)
        {
        }
        if(GetCecPinStatus(pCecPort)== 0)
        {
            pstFun->get_cnt++;
            pstFun->timer_cnt++;
            bBreakFlag = TRUE;
            if(pstFun->get_cnt>Dbit0_Max_LD)
            {
                SetCecPinHigh(pCecPort);
            }
        }
        else
        {
            pstFun->get_cnt++;
            pstFun->timer_cnt++;
            if(pstFun->get_cnt>=(Dbit0_Min_LD+Dbit0_Nom_HD))
            {
                SetCecPinHigh(pCecPort);
                pstFun->cecMsg_state = (bEom)?cec_state_none:(cec_msg_state_t)(pstFun->cecMsg_state+1);
                pstFun->timer_cnt = 0;
                if(bEom) bBreakFlag = TRUE;
            }
            else
                bBreakFlag = TRUE;
        }
    }
#else
    //ack
    else if((pstFun->timer_cnt==(Dbit1_Nom_LD+(pstFun->bit_cnt*(Dbit1_Nom_LD+Dbit1_Nom_HD))))&&\
        (pstFun->bit_cnt==9))
    {
        if(pstFun->get_cnt==0)
        {
            //SetCecPinHigh(pCecPort);
            pstFun->get_cnt += Dbit1_Nom_LD;

            bBreakFlag = TRUE;
        }
        else
        {
            if(GetCecPinStatus(pCecPort)== 0)
            {
                pstFun->get_cnt++;
                if(pstFun->get_cnt>=Dbit0_Max_LD)
                {
                    SetCecPinHigh(pCecPort);
                    pstFun->timer_cnt++;
                }
            }
            else
            {
                SetCecPinHigh(pCecPort);
                pstFun->timer_cnt++;
            }

            bBreakFlag = TRUE;
        }
    }
    //ack
    else if((pstFun->timer_cnt==(((pstFun->bit_cnt+1)*(Dbit1_Nom_LD+Dbit1_Nom_HD))-(pstFun->get_cnt-Dbit1_Nom_LD)))&&\
        (pstFun->bit_cnt==9))
    {
        if(pstFun->get_cnt >= Dbit0_Min_LD)
        {
            pstFun->cecMsg_state = (bEom)?cec_state_none:(cec_msg_state_t)(pstFun->cecMsg_state+1);
            pstFun->timer_cnt = 0;

            if(bEom) bBreakFlag = TRUE;
        }
        else
        {
            pstFun->cecMsg_state = (cec_msg_state_t)(cec_state_err_header+(pstFun->cecMsg_state-cec_state_header));
            pstFun->timer_cnt = 0;

            bBreakFlag = TRUE;
        }
    }
#endif //add by wf8421
    else
    {
        pstFun->timer_cnt++;

        bBreakFlag = TRUE;
    }

    return bBreakFlag;
}

void cec_tx_process(pstCecDeclare_t pCecPort)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    //send the cec start bit
    switch(pstFun->cecMsg_state)
    {
        case cec_state_start:
            if(cec_send_start(pCecPort))
                break;

        case cec_state_header:
            if(cec_send_byte_by_state(pCecPort, (bool)(pstFun->cecMsg_content->ucLen <= 1)))
                break;

        case cec_state_opcode:
            if(cec_send_byte_by_state(pCecPort, (bool)(pstFun->cecMsg_content->ucLen <= 2)))
                break;

        case cec_state_operand1:
        case cec_state_operand2:
        case cec_state_operand3:
        case cec_state_operand4:
        case cec_state_operand5:
        case cec_state_operand6:
        case cec_state_operand7:
        case cec_state_operand8:
        case cec_state_operand9:
        case cec_state_operand10:
        case cec_state_operand11:
        case cec_state_operand12:
        case cec_state_operand13:
        case cec_state_operand14:
        case cec_state_operand15:
        case cec_state_operand16:
            if(pstFun->cecMsg_content->ucLen < 2)
                break;
            if(cec_send_byte_by_state(pCecPort,
                    (bool)((pstFun->cecMsg_content->ucLen-2) == (pstFun->cecMsg_state-cec_state_operand1+1))))
                break;
        default:
            break;
    }
}

bool cec_receive_start(pstCecDeclare_t pCecPort)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    bool                    bBreakFlag = FALSE;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    if(pstFun->timer_cnt == 0)
    {
        SetCecPinHigh(pCecPort);
        if(GetCecPinStatus(pCecPort)==0)
        {
            pstFun->timer_cnt ++;
            pstFun->bit_cnt = 0;
        }
        else
        {
            pstFun->cecMsg_state = cec_state_err_start;
            pstFun->timer_cnt = 0;
            pstFun->bit_cnt = 0;
            pstFun->get_cnt = 0;
        }

        bBreakFlag = TRUE;
    }
    else
    {
        if(pstFun->bit_cnt == 0)
        {
            if(GetCecPinStatus(pCecPort)==0)
            {
                pstFun->timer_cnt++;
                if(pstFun->timer_cnt > (Sbit_Max_LD+1))
                {
                    pstFun->cecMsg_state = cec_state_err_start;
                    pstFun->timer_cnt = 0;
                    pstFun->bit_cnt = 0;
                    pstFun->get_cnt = 0;
                }
                bBreakFlag = TRUE;
            }
            else
            {
                if(pstFun->timer_cnt < (Sbit_Min_LD-1))
                {
                    //low level below Sbit_Min_LD,error
                    pstFun->cecMsg_state = cec_state_err_start;
                    pstFun->timer_cnt = 0;
                    pstFun->bit_cnt = 0;
                    pstFun->get_cnt = 0;
                    bBreakFlag = TRUE;
                }
                else
                {
                    //normal, detect high level now
                    pstFun->bit_cnt = 1;
                    pstFun->timer_cnt++;
                    bBreakFlag = TRUE;
                }
            }
        }
        else if(pstFun->bit_cnt == 1)
        {
            if(GetCecPinStatus(pCecPort)!=0)
            {
                pstFun->timer_cnt++;
                if(pstFun->timer_cnt > (Sbit_Max_TD+1))
                {
                    pstFun->cecMsg_state = cec_state_err_start;
                    pstFun->timer_cnt = 0;
                    pstFun->bit_cnt = 0;
                    pstFun->get_cnt = 0;
                }
                bBreakFlag = TRUE;
            }
            else
            {
                if(pstFun->timer_cnt < (Sbit_Min_TD-1))
                {
                    pstFun->cecMsg_state = cec_state_err_start;
                    pstFun->timer_cnt = 0;
                    pstFun->bit_cnt = 0;
                    pstFun->get_cnt = 0;
                    bBreakFlag = TRUE;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_header;
                    pstFun->timer_cnt = 0;
                    pstFun->get_cnt = 0;
                }
            }
        }
    }

    return bBreakFlag;
}

bool cec_receive_data_bit(pstCecDeclare_t pCecPort, uint8_t* pu8Rbit)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    bool                    bBreakFlag = FALSE;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    if(pstFun->timer_cnt == 0)
    {
        if(GetCecPinStatus(pCecPort)==0)
        {
            pstFun->timer_cnt ++;
            pstFun->bit_cnt = 0;
        }
        else
        {
            pstFun->cecMsg_state = (cec_msg_state_t)(cec_state_err_start+(pstFun->cecMsg_state-cec_state_start));
            pstFun->timer_cnt = 0;
            pstFun->bit_cnt = 0;
            pstFun->get_cnt = 0;
        }
        bBreakFlag = TRUE;
    }
    else
    {
        if(pstFun->bit_cnt == 0)
        {
            if(GetCecPinStatus(pCecPort)==0)
            {
                pstFun->timer_cnt++;
                if(pstFun->timer_cnt > (Dbit0_Max_LD+1))
                {
                    pstFun->cecMsg_state = (cec_msg_state_t)(cec_state_err_start+(pstFun->cecMsg_state-cec_state_start));
                    pstFun->timer_cnt = 0;
                    pstFun->bit_cnt = 0;
                    pstFun->get_cnt = 0;
                }
                bBreakFlag = TRUE;
            }
            else
            {
                if(pstFun->timer_cnt >= (Dbit0_Min_LD-1))
                {
                    *pu8Rbit = 0;
                    pstFun->bit_cnt = 1;
                    pstFun->timer_cnt++;
                    bBreakFlag = TRUE;
                }
                else
                {
                    if(pstFun->timer_cnt > (Dbit1_Max_LD+1))
                    {
                        pstFun->cecMsg_state = (cec_msg_state_t)(cec_state_err_start+(pstFun->cecMsg_state-cec_state_start));
                        pstFun->timer_cnt = 0;
                        pstFun->bit_cnt = 0;
                        pstFun->get_cnt = 0;
                        bBreakFlag = TRUE;
                    }
                    else if(pstFun->timer_cnt >= (Dbit1_Min_LD-1))
                    {
                        *pu8Rbit = 1;
                        pstFun->bit_cnt = 1;
                        pstFun->timer_cnt++;
                        bBreakFlag = TRUE;
                    }

                    else
                    {
                        pstFun->cecMsg_state = (cec_msg_state_t)(cec_state_err_start+(pstFun->cecMsg_state-cec_state_start));
                        pstFun->timer_cnt = 0;
                        pstFun->bit_cnt = 0;
                        pstFun->get_cnt = 0;
                        bBreakFlag = TRUE;
                    }
                }
            }
        }
        else
        {
            if(GetCecPinStatus(pCecPort)!=0)
            {
                pstFun->timer_cnt++;
                if(pstFun->timer_cnt > (DbitX_Max_TD+1))
                {
                    *pu8Rbit = 0xFF;
                    pstFun->cecMsg_state = (cec_msg_state_t)(cec_state_err_start+(pstFun->cecMsg_state-cec_state_start));
                    pstFun->timer_cnt = 0;
                    pstFun->bit_cnt = 0;
                    pstFun->get_cnt = 0;
                }
                bBreakFlag = TRUE;
            }
            else
            {
                if(pstFun->timer_cnt < (DbitX_Min_TD-1))
                {
                    *pu8Rbit = 0xFF;
                    pstFun->cecMsg_state = (cec_msg_state_t)(cec_state_err_start+(pstFun->cecMsg_state-cec_state_start));
                    pstFun->timer_cnt = 0;
                    pstFun->bit_cnt = 0;
                    pstFun->get_cnt = 0;
                    bBreakFlag = TRUE;
                }
                else
                {
                    pstFun->timer_cnt = 0;//go to next bit
                }
            }
        }
    }

    return bBreakFlag;
}

bool cec_send_data_bit(pstCecDeclare_t pCecPort, uint8_t u8Sbit)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    bool                    bBreakFlag = FALSE;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    if(pstFun->timer_cnt == 0)
    {
        SetCecPinLow(pCecPort);
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }
    //s_bit = 1;
    else if((pstFun->timer_cnt==Dbit1_Nom_LD)&&(u8Sbit==1))
    {
        SetCecPinHigh(pCecPort);
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }
    else if((pstFun->timer_cnt==(Dbit1_Nom_HD+Dbit1_Nom_LD))&&(u8Sbit==1))
    {
        pstFun->timer_cnt=0;//send over
        SetCecPinHigh(pCecPort);
    }
    else if((pstFun->timer_cnt==Dbit0_Nom_LD)&&(u8Sbit==0))
    {
        SetCecPinHigh(pCecPort);
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }
    else if((pstFun->timer_cnt==(Dbit0_Nom_HD+Dbit0_Nom_LD))&&(u8Sbit==0))
    {
        pstFun->timer_cnt=0;//send over
        SetCecPinHigh(pCecPort);
    }
    else
    {

        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }

    return bBreakFlag;
}

bool cec_send_ack_bit(pstCecDeclare_t pCecPort, uint8_t u8Eom)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    bool                    bBreakFlag = FALSE;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    //send ack bit
    if(u8Eom == 0)
    {
        if(pstFun->bit_cnt==0)
        {
            if(cec_send_data_bit(pCecPort, 0)==FALSE)
            {
                pstFun->bit_cnt = 1;
                SetCecPinLow(pCecPort);
            }
            bBreakFlag = TRUE;
        }
        else
        {
            SetCecPinHigh(pCecPort);
        }
    }
    else
    {
        bBreakFlag = cec_send_data_bit(pCecPort, 0);
    }

    return bBreakFlag;
}

bool cec_receive_byte(pstCecDeclare_t pCecPort, uint8_t* pu8Rbyte, uint8_t* pu8Eom)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    bool                    bBreakFlag = FALSE;
    static uint8_t          su8Rbit = 0xFF;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    switch(pstFun->get_cnt)
    {
        case 0:
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Rbyte = (*pu8Rbyte&(~0x80))|((su8Rbit)?0x80:0x00);
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt ++;
                pstFun->bit_cnt = 0;
            }
        case 1:
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Rbyte = (*pu8Rbyte&(~0x40))|((su8Rbit)?0x40:0x00);
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt ++;
                pstFun->bit_cnt = 0;
            }
        case 2:
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Rbyte = (*pu8Rbyte&(~0x20))|((su8Rbit)?0x20:0x00);
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt ++;
                pstFun->bit_cnt = 0;
            }
        case 3:
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Rbyte = (*pu8Rbyte&(~0x10))|((su8Rbit)?0x10:0x00);
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt ++;
                pstFun->bit_cnt = 0;
            }
        case 4:
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Rbyte = (*pu8Rbyte&(~0x08))|((su8Rbit)?0x08:0x00);
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt ++;
                pstFun->bit_cnt = 0;
            }
        case 5:
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Rbyte = (*pu8Rbyte&(~0x04))|((su8Rbit)?0x04:0x00);
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt ++;
                pstFun->bit_cnt = 0;
            }
        case 6:
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Rbyte = (*pu8Rbyte&(~0x02))|((su8Rbit)?0x02:0x00);
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt ++;
                pstFun->bit_cnt = 0;
            }
        case 7:
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Rbyte = (*pu8Rbyte&(~0x01))|((su8Rbit)?0x01:0x00);
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt++;
                pstFun->bit_cnt = 0;
            }
        case 8://eom
            if(cec_receive_data_bit(pCecPort, &su8Rbit))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                if(su8Rbit!=0xFF)
                {
                    *pu8Eom = su8Rbit;
                    su8Rbit = 0xFF;
                }
                pstFun->get_cnt++;
                pstFun->bit_cnt = 0;
            }
        case 9://ack
            if(cec_send_ack_bit(pCecPort, *pu8Eom))
            {
                bBreakFlag = TRUE;
                break;
            }
            else
            {
                pstFun->get_cnt = 0;//receive over
                pstFun->timer_cnt = 0;
                pstFun->bit_cnt = 0;
            }
        default:
            break;
    }

    return bBreakFlag;
}

void cec_msg_push(pstCecMsg_t pcec_msg, pstCecDeclare_t pCecPort)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecMsg_t             p_rx_cecMsgC;
    pstCecRxMsg_t           p_rx_cecMsg;

    p_rx_cecMsgC = g_rx_cecMsg[pCecPort-pCecPortOrg].msgElm;
    p_rx_cecMsg = &g_rx_cecMsg[pCecPort-pCecPortOrg];

    if((p_rx_cecMsg->addr_loc == (pcec_msg->ucHeader&0x0F))||\
        (0x0F==(pcec_msg->ucHeader&0x0F)))
    {
        p_rx_cecMsgC += p_rx_cecMsg->push_cur;
        memcpy(p_rx_cecMsgC, pcec_msg, sizeof(stCecMsg_t));
        p_rx_cecMsg->push_cur++;
        if(p_rx_cecMsg->push_cur>=CEC_MSG_RX_BUF_LEN)
        {
            p_rx_cecMsg->push_cur = 0;
        }
    }
}

pstCecMsg_t cec_msg_pop(pstCecDeclare_t pCecPort)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecRxMsg_t           p_rx_cecMsg;
    pstCecMsg_t             p_rx_cecMsgC;

    p_rx_cecMsg = &g_rx_cecMsg[pCecPort-pCecPortOrg];
    p_rx_cecMsgC = g_rx_cecMsg[pCecPort-pCecPortOrg].msgElm;

    if(p_rx_cecMsg->pop_cur != p_rx_cecMsg->push_cur)
    {
        p_rx_cecMsgC += p_rx_cecMsg->pop_cur;
        p_rx_cecMsg->pop_cur ++;
        if(p_rx_cecMsg->pop_cur>=CEC_MSG_RX_BUF_LEN)
        {
            p_rx_cecMsg->pop_cur = 0;
        }

        return p_rx_cecMsgC;
    }

    return NULL;
}

void cec_rx_process(pstCecDeclare_t pCecPort)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    static uint8_t          eom=0xFF;
    static stCecMsg_t       cec_msg_temp;
    pstCecMsg_t             p_cec = &cec_msg_temp;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    switch(pstFun->cecMsg_state)
    {
        case cec_state_start:
            if(cec_receive_start(pCecPort))
            {
                break;
            }
            else
            {
                memset(p_cec, 0, sizeof(stCecMsg_t));
                eom = 0xFF;
                pstFun->get_cnt = 0;
                pstFun->timer_cnt = 0;
                pstFun->bit_cnt = 0;
            }
        case cec_state_header:
            if(cec_receive_byte(pCecPort, &p_cec->ucHeader, &eom))
            {
                break;
            }
            else
            {
                //pstFun->cecMsg_state = cec_state_opcode;
                //eom=0xFF;

                if(eom != 1)
                {
                    pstFun->cecMsg_state = cec_state_opcode;
                    eom = 0xFF;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 0;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                }
            }
        case cec_state_opcode:
            if(cec_receive_byte(pCecPort, &p_cec->ucOpCode, &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand1;
                }
                else
                    {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 0;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand1:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[0], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand2;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 1;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand2:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[1], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand3;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 2;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand3:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[2], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand4;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 3;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand4:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[3], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand5;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 4;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand5:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[4], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand6;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 5;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand6:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[5], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand7;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 6;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand7:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[6], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand8;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 7;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand8:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[7], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand9;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 8;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand9:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[8], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand10;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 9;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand10:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[9], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand11;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 10;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand11:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[10], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand12;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 11;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand12:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[11], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand13;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 12;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand13:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[12], &eom))
            {
                break;
            }
            else
            {
                if(eom!=1)
                {
                    eom=0xFF;
                    pstFun->cecMsg_state = cec_state_operand14;
                }
                else
                {
                    pstFun->cecMsg_state = cec_state_none;
                    cec_msg_temp.ucLen = 13;
                    cec_msg_push(&cec_msg_temp, pCecPort);
                    break;
                }
            }
        case cec_state_operand14:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[13], &eom))
            {
                break;
            }
            else
            {
                pstFun->cecMsg_state = cec_state_none;
                cec_msg_temp.ucLen = 14;
                cec_msg_push(&cec_msg_temp, pCecPort);
                break;
            }
        case cec_state_operand15:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[14], &eom))
            {
                break;
            }
            else
            {
                pstFun->cecMsg_state = cec_state_none;
                cec_msg_temp.ucLen = 15;
                cec_msg_push(&cec_msg_temp, pCecPort);
                break;
            }
        case cec_state_operand16:
            if(cec_receive_byte(pCecPort, &p_cec->ucOperand[15], &eom))
            {
                break;
            }
            else
            {
                pstFun->cecMsg_state = cec_state_none;
                cec_msg_temp.ucLen = 16;
                cec_msg_push(&cec_msg_temp, pCecPort);
                break;
            }
        default:
            break;
    }
}

#if ENABLE_CEC_USER2
bool cec_send_data_bit2(pstCecDeclare_t pCecPort, uint8_t u8Sbit)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t          pstFun;
    bool                    bBreakFlag = FALSE;

    pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

    if(pstFun->timer_cnt == 0)
    {
        SetCecPinLow(pCecPort);
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }
    //s_bit = 1;
    else if((pstFun->timer_cnt==Dbit1_Nom_LD)&&(u8Sbit==1))
    {
        SetCecPinHigh(pCecPort);
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }
    else if((pstFun->timer_cnt==(Dbit1_Nom_HD+Dbit1_Nom_LD))&&(u8Sbit==1))
    {
        pstFun->timer_cnt=0;//send over
        SetCecPinHigh(pCecPort);
    }
    else if((pstFun->timer_cnt==Dbit0_Nom_LD)&&(u8Sbit==0))
    {
        SetCecPinHigh(pCecPort);
        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }
    else if((pstFun->timer_cnt==(Dbit0_Nom_HD+Dbit0_Nom_LD))&&(u8Sbit==0))
    {
        pstFun->timer_cnt=0;//send over
        SetCecPinHigh(pCecPort);
    }
    else
    {

        pstFun->timer_cnt++;
        bBreakFlag = TRUE;
    }

    return bBreakFlag;
}


u8 cec_receive_logic_start(u8 u8Low,u8 u8High)
{
    u8 l_u8BitVal = 0xFF;

    if((u8Low>=Sbit_Min_LD)&&(u8Low<=Sbit_Max_LD))
    {
        if((u8High>=Sbit_Nom_HD-1)&&(u8High<=Sbit_Nom_HD+1))
            l_u8BitVal = 0;
    }

    return l_u8BitVal;
}

u8 cec_receive_logic(u8 u8Low,u8 u8High)
{
    u8 l_u8BitVal = 0xFF;
    if((u8Low>=Dbit0_Min_LD)&&(u8Low<=Dbit0_Max_LD))
    {
        l_u8BitVal = 0;
    }
    else if((u8Low>=Dbit1_Min_LD)&&(u8Low<=Dbit1_Max_LD))
    {
        l_u8BitVal = 1;
    }
    u8High = u8High;
    return l_u8BitVal;
}

bool cec_receive_start2(pstCecDeclare_t pCecPort,u8 cec_pin)
{
    bool break_flag = FALSE;
    pstCecRevType_t l_pstCecRev = &g_stCecRevData[cec_pin];

    {
        if(GetCecPinStatus(pCecPort)==0)
        {
            if(l_pstCecRev->u8CurLineSta== ePinLevel_High)
            {
                l_pstCecRev->u8PrevLineSta = l_pstCecRev->u8CurLineSta;
                l_pstCecRev->u8CurLineSta = ePinLevel_Low;
                l_pstCecRev->u16LowSta = 0;
            }

            #if 1
            if(l_pstCecRev->eCecSta==emCecSta_None)
            {
                if((l_pstCecRev->u8PrevLineSta==ePinLevel_High)&&(l_pstCecRev->u8CurLineSta==ePinLevel_Low))
                {
                    if((l_pstCecRev->u16LowSta>=Sbit_Min_LD)&&(l_pstCecRev->u16LowSta<=Sbit_Max_LD))
                    {
                        l_pstCecRev->eCecSta = emCecSta_Recevie;
                        l_pstCecRev->u16BitCnt =  0;
                    }
                }
            }
            #endif

            l_pstCecRev->u16LowSta++;
            if((l_pstCecRev->u16HighSta > 0)&&(l_pstCecRev->u16HighSta < SCAN_CEC_END))
            {
                l_pstCecRev->u8Ware[l_pstCecRev->u16BitCnt] = l_pstCecRev->u16HighSta;
                l_pstCecRev->u16BitCnt++;
                l_pstCecRev->u16HighSta = 0;
                if(l_pstCecRev->u16BitCnt>=149)
                {
                    l_pstCecRev->eCecSta = emCecSta_RevOk;
                }
            }

            #if 1
            if(l_pstCecRev->u16BitCnt>=20)
            {
                //if((sl_u16HighSta>=(Dbit0_Nom_HD-1))&&(sl_u16HighSta<=(Dbit0_Nom_HD+1)))
                {
                    if((l_pstCecRev->u16BitCnt==20)
                        ||((l_pstCecRev->u16BitCnt>20)
                        &&((l_pstCecRev->u16BitCnt-20)%20==0)))
                    {
                        u8 l_u8Temp = 0;
                        l_u8Temp |= cec_receive_logic(l_pstCecRev->u8Ware[2] ,l_pstCecRev->u8Ware[3] )?BIT7:0;
                        l_u8Temp |= cec_receive_logic(l_pstCecRev->u8Ware[4] ,l_pstCecRev->u8Ware[5] )?BIT6:0;
                        l_u8Temp |= cec_receive_logic(l_pstCecRev->u8Ware[6] ,l_pstCecRev->u8Ware[7] )?BIT5:0;
                        l_u8Temp |= cec_receive_logic(l_pstCecRev->u8Ware[8] ,l_pstCecRev->u8Ware[9] )?BIT4:0;
                        l_u8Temp |= cec_receive_logic(l_pstCecRev->u8Ware[10],l_pstCecRev->u8Ware[11])?BIT3:0;
                        l_u8Temp |= cec_receive_logic(l_pstCecRev->u8Ware[12],l_pstCecRev->u8Ware[13])?BIT2:0;
                        l_u8Temp |= cec_receive_logic(l_pstCecRev->u8Ware[14],l_pstCecRev->u8Ware[15])?BIT1:0;
                        l_u8Temp |= cec_receive_logic(l_pstCecRev->u8Ware[16],l_pstCecRev->u8Ware[17])?BIT0:0;

                        if(cec_receive_logic(l_pstCecRev->u8Ware[l_pstCecRev->u16BitCnt-2],
                                        l_pstCecRev->u8Ware[l_pstCecRev->u16BitCnt-1])==0)
                        {// EOM = 0  还有消息
                            // 广播地址，为ack为0时表示拒绝接受消息，反之表示接收消息
                            if((l_u8Temp&0x0F) == CEC_LOGADDR_BROADCAST)
                            {
                                l_pstCecRev->u8SetAck = emCecAckType_1;
                            }
                            // 本端口地址，为ack为0时表示接收消息成功，反之表示发来的是非本机地址
                            else if((l_u8Temp&0x0F) == g_rx_cecMsg[cec_pin].addr_loc)
                            {
                                l_pstCecRev->u8SetAck = emCecAckType_0;
                            }
                            else
                            {
                                //l_pstCecRev->u8SetAck = emCecAckType_1;
                            }
                        }
                        else if(cec_receive_logic(l_pstCecRev->u8Ware[l_pstCecRev->u16BitCnt-2],
                                        l_pstCecRev->u8Ware[l_pstCecRev->u16BitCnt-1])==1)
                        {// EOM = 1  消息结束
                            // 广播地址，为ack为0时表示拒绝接受消息，反之表示接收消息
                            if((l_u8Temp&0x0F) == CEC_LOGADDR_BROADCAST)
                            {
                                l_pstCecRev->u8SetAck = emCecAckType_1;
                            }
                            // 本端口地址，为ack为0时表示接收消息成功，反之表示发来的是非本机地址
                            else if((l_u8Temp&0x0F) == g_rx_cecMsg[cec_pin].addr_loc)
                            {
                                l_pstCecRev->u8SetAck = emCecAckType_0;
                            }
                            else
                            {
                                //l_pstCecRev->u8SetAck = emCecAckType_1;
                            }
                        }
                    }
                }
            }
            #endif
        }
        else
        {
            if(l_pstCecRev->u8CurLineSta == ePinLevel_Low)
            {
                l_pstCecRev->u8PrevLineSta = l_pstCecRev->u8CurLineSta;
                l_pstCecRev->u8CurLineSta = ePinLevel_High;
                l_pstCecRev->u16HighSta = 0;
            }
            l_pstCecRev->u16HighSta++;
            if((l_pstCecRev->u16LowSta > 0)&&(l_pstCecRev->u16LowSta < SCAN_CEC_END))
            {
                if(l_pstCecRev->eCecSta == emCecSta_Recevie)
                {
                    l_pstCecRev->u8Ware[l_pstCecRev->u16BitCnt] = l_pstCecRev->u16LowSta;
                    l_pstCecRev->u16BitCnt++;
                    l_pstCecRev->u16LowSta = 0;
                    if(l_pstCecRev->u16BitCnt>=149)
                    {
                        l_pstCecRev->eCecSta = emCecSta_RevOk;
                    }
                }
            }

            if(l_pstCecRev->u16HighSta>=SCAN_CEC_END)
            {
                if(l_pstCecRev->eCecSta == emCecSta_Recevie)
                {
                    l_pstCecRev->eCecSta = emCecSta_RevOk;
                }
                l_pstCecRev->u16HighSta = 0;
                l_pstCecRev->u16LowSta = 0;
            }
        }
    }

    if(l_pstCecRev->u8SetAck != emCecAckType_None)
    {
        if(l_pstCecRev->u8SetAck == emCecAckType_0)
        {
            if(cec_send_data_bit2(pCecPort,0)==FALSE)
            {
                l_pstCecRev->u8SetAck = emCecAckType_None;
            }
        }
        else
        {
            if(cec_send_data_bit2(pCecPort,1)==FALSE)
            {
                l_pstCecRev->u8SetAck = emCecAckType_None;
            }
        }
    }

    return break_flag;
}

void cec_rx_process2(pstCecDeclare_t pCecPort, u8 cec_pin)
{
    {
        cec_receive_start2(pCecPort,cec_pin);
    }
    cec_pin = cec_pin;
}

static void mapp_CecRxGetCmd(void)
{
    u8 cec_index;
    u8 l_u8Cnt = 0;

    for(cec_index=0; cec_index<NUM_OF_CEC_PINS_MAX; cec_index++)
    {
        pstCecRevType_t l_pstCecRev = &g_stCecRevData[cec_index];
        //for(l_u8Cnt=0;l_u8Cnt<=g_u8CecRevDataCnt[cec_index];l_u8Cnt++)
        {
            if(l_pstCecRev->eCecSta == emCecSta_RevOk)
            {
                //l_pstCecRev->eCecSta = emCecSta_None;
                {
                    u16 i;
                    u16 l_u16BitLen = (l_pstCecRev->u16BitCnt%2)?l_pstCecRev->u16BitCnt-1 : l_pstCecRev->u16BitCnt;
                    u8 l_u8Header = 0;
                    u8 l_u8OpCode = 0;
                    u8 l_u8Data[16] = {0};
                    u8 l_u8DataIndex = 0;
                    u8 l_u8BitIndex = 0;
                    u16 l_u16Eom = 0x0;
                    u16 l_u16Ack = 0x0;
                    bool l_bSetOk = FALSE;

                    for(i=2;i<l_u16BitLen;i=i+2)
                    {
                        if(cec_receive_logic_start(l_pstCecRev->u8Ware[0],l_pstCecRev->u8Ware[1]) != 0xFF)
                        {
                            l_bSetOk = TRUE;
                            if(cec_receive_logic(l_pstCecRev->u8Ware[i],l_pstCecRev->u8Ware[i+1]) != 0xFF)
                            {
                                if(l_u8BitIndex<8)
                                {
                                    if(l_u8DataIndex==0)
                                    {
                                        l_u8Header = (l_u8Header<<1)|
                                            (cec_receive_logic(l_pstCecRev->u8Ware[i],l_pstCecRev->u8Ware[i+1]));
                                    }
                                    else if(l_u8DataIndex==1)
                                    {
                                        l_u8OpCode = (l_u8OpCode<<1)|
                                            (cec_receive_logic(l_pstCecRev->u8Ware[i],l_pstCecRev->u8Ware[i+1]));
                                    }
                                    else
                                    {
                                        l_u8Data[l_u8DataIndex-2] = (l_u8Data[l_u8DataIndex-2]<<1)|
                                            (cec_receive_logic(l_pstCecRev->u8Ware[i],l_pstCecRev->u8Ware[i+1]));
                                    }
                                }
                                else if(l_u8BitIndex==8)
                                {
                                    l_u16Eom = (l_u16Eom<<1)|
                                            (cec_receive_logic(l_pstCecRev->u8Ware[i],l_pstCecRev->u8Ware[i+1]));
                                    l_u8DataIndex++;
                                }
                                else if(l_u8BitIndex==9)
                                {
                                    l_u16Ack = (l_u16Ack<<1)|
                                            (cec_receive_logic(l_pstCecRev->u8Ware[i],l_pstCecRev->u8Ware[i+1]));

                                    if(l_u16Eom>=0x01)  // 如果在EOM指示后，消息包含额外的数据块，则跟随者应忽略附加块。
                                    {
                                        break;
                                    }
                                }
                            }

                            l_u8BitIndex++;
                        #if 1
                            if(l_u8BitIndex>=10)
                            {
                                l_u8BitIndex = 0;
                            }
                        #endif
                        }
                    }

                    if(l_u16Eom == 0x00)    // 如果EOM为零且没有收到更多的数据块，跟随者应忽略整个消息。
                    {
                        l_bSetOk = FALSE;
                    }

                    if(l_bSetOk)
                    {
                        if(l_u8DataIndex>0)
                        {
                            if(((l_u8Header&0x0F) == g_rx_cecMsg[cec_index].addr_loc)
                                ||((l_u8Header&0x0F) == 0x0F)
                                //||((l_u8Header&0x0F) == 0x05)
                                )
                            {
                                l_u8Cnt = g_u8CecRxCnt[cec_index];
                                g_stCecRxMsg[cec_index][l_u8Cnt].ucHeader = l_u8Header;
                                g_stCecRxMsg[cec_index][l_u8Cnt].ucOpCode = l_u8OpCode;
                                g_stCecRxMsg[cec_index][l_u8Cnt].ucLen    = l_u8DataIndex;//(l_u8DataIndex>2)?(l_u8DataIndex-2):0;
                                if(g_stCecRxMsg[cec_index][l_u8Cnt].ucLen>2)
                                {
                                    for(i=0;i<g_stCecRxMsg[cec_index][l_u8Cnt].ucLen;i++)
                                    {
                                        g_stCecRxMsg[cec_index][l_u8Cnt].ucOperand[i] = l_u8Data[i];
                                    }
                                }
                                g_bRxRecevieFlag[cec_index][l_u8Cnt]= TRUE;
                                g_u8CecRxCnt[cec_index] += 1;
                            }
                            //CEC_DBG_PRINT(("l_u16Eom==0x%04x\n",l_u16Eom));
                            //CEC_DBG_PRINT(("l_u16Ack==0x%04x\n",l_u16Ack));
                        }
                    }
                }
                memset(l_pstCecRev,0,sizeof(stCecRevType_t));
                l_pstCecRev->eCecSta = emCecSta_SendAck;//emCecSta_None;
            }
        }
    }
}

static void mapp_CecRxCmdAction(void)
{
    u8 cec_index;
    u8 l_u8Cnt;
    #if DBG_CEC_RX
    u8 i;
    #endif

    mapp_CecRxGetCmd();

    for(cec_index=0; cec_index<NUM_OF_CEC_PINS_MAX; cec_index++)
    {
        for(l_u8Cnt=0;l_u8Cnt<g_u8CecRxCnt[cec_index];l_u8Cnt++)
        {
            if(g_bRxRecevieFlag[cec_index][l_u8Cnt] == TRUE)
            {
                #if DBG_CEC_RX
                CEC_DBG_PRINT(("=============start %d======================\n",cec_index));
                CEC_DBG_PRINT(("ucHeader              ==0x%x\n",g_stCecRxMsg[cec_index][l_u8Cnt].ucHeader));
                CEC_DBG_PRINT(("ucOpCode              ==0x%x\n",g_stCecRxMsg[cec_index][l_u8Cnt].ucOpCode));
                CEC_DBG_PRINT(("ucLen                 ==%d\n",g_stCecRxMsg[cec_index][l_u8Cnt].ucLen));
                CEC_DBG_PRINT(("ucOperand:start\n"));
                for(i=0;i<((g_stCecRxMsg[cec_index][l_u8Cnt].ucLen>2)?(g_stCecRxMsg[cec_index][l_u8Cnt].ucLen-2):0);i++)
                {
                    CEC_DBG_PRINT(("0x%02x ",g_stCecRxMsg[cec_index][l_u8Cnt].ucOperand[i]));
                }
                CEC_DBG_PRINT(("\n"));
                CEC_DBG_PRINT(("ucOperand:end\n"));
                CEC_DBG_PRINT(("=============end %d========================\n",cec_index));
                #endif
                g_bRxRecevieFlag[cec_index][l_u8Cnt]= FALSE;
                if(((g_stCecRxMsg[cec_index][l_u8Cnt].ucHeader&0x0F) == g_rx_cecMsg[cec_index].addr_loc)
                    ||((g_stCecRxMsg[cec_index][l_u8Cnt].ucHeader&0x0F) == 0x0F))
                {
                    Cec_Cmd_SysInfoFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                    if((g_stCecRxMsg[cec_index][l_u8Cnt].ucHeader&0x0F) == g_rx_cecMsg[cec_index].addr_loc)
                    {
                        //Cec_Cmd_SysInfoFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_OTPFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_RoutingCtlFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_StandByFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_OTRFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_DeckCtrlFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_VenSpecCmdFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_DevOSDTransferFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_RemoteCtlPassthroughFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_PowerStatusFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_GeneralProtocolMessage(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_SysAudioCtlFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                        Cec_Cmd_ARCFeature(cec_index,&g_stCecRxMsg[cec_index][l_u8Cnt],FALSE);
                    }
                }
                memset(&g_stCecRxMsg[cec_index][l_u8Cnt],0,sizeof(stCecMsg_t));


            }
            if((g_u8CecRxCnt[cec_index]-1)==l_u8Cnt)
                g_u8CecRxCnt[cec_index] = 0;
        }
        if(g_stCecRevData[cec_index].eCecSta == emCecSta_SendAck)
            g_stCecRevData[cec_index].eCecSta = emCecSta_None;

    #if DBG_CEC_RX
        if(g_bCecTxDbgFlag[cec_index])
        {
            g_bCecTxDbgFlag[cec_index] = FALSE;

            CEC_DBG_PRINT(("=============Tx start %d======================\n",cec_index));
            CEC_DBG_PRINT(("ucHeader              ==0x%x\n",g_stCecTxDbgMsg[cec_index].ucHeader));
            CEC_DBG_PRINT(("ucOpCode              ==0x%x\n",g_stCecTxDbgMsg[cec_index].ucOpCode));
            CEC_DBG_PRINT(("ucLen                 ==%d\n",g_stCecTxDbgMsg[cec_index].ucLen));
            CEC_DBG_PRINT(("ucOperand:start\n"));
            for(i=0;i<((g_stCecTxDbgMsg[cec_index].ucLen>2)?(g_stCecTxDbgMsg[cec_index].ucLen-2):0);i++)
            {
                CEC_DBG_PRINT(("0x%02x ",g_stCecTxDbgMsg[cec_index].ucOperand[i]));
            }
            CEC_DBG_PRINT(("\n"));
            CEC_DBG_PRINT(("ucOperand:end\n"));
            CEC_DBG_PRINT(("=============Tx end %d========================\n",cec_index));
        }
    #endif //add by wf8421
    }
}
#endif

void DisableCecDetect(uint8_t u8CecPinIndex)
{
    pstCecDeclare_t         pCecPort, pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecFrameDetect_t     pstDetect;

    if(u8CecPinIndex == CEC_OPERATE_OUTPUT)
    {
        pCecPort = pCecPortOrg;
        while(pCecPort->pstCecGpioLoc != NULL)
        {
            if(pCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Dis)
            {
                pstDetect = &g_cec_frame_dct[pCecPort-pCecPortOrg];

                pstDetect->en_detect_mode = FALSE;
                pstDetect->cec_pin_cur_state = ePinLevel_High;
                pstDetect->cec_pin_pre_state = ePinLevel_High;
            }
            pCecPort ++;
        }
    }
    else if(u8CecPinIndex == CEC_OPERATE_INPUT)
    {
        pCecPort = pCecPortOrg;
        while(pCecPort->pstCecGpioLoc != NULL)
        {
            if(pCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Src)
            {
                pstDetect = &g_cec_frame_dct[pCecPort-pCecPortOrg];

                pstDetect->en_detect_mode = FALSE;
                pstDetect->cec_pin_cur_state = ePinLevel_High;
                pstDetect->cec_pin_pre_state = ePinLevel_High;
            }
            pCecPort ++;
        }
    }
    else
    {
        pCecPort = pCecPortOrg + u8CecPinIndex;

        if(pCecPort->pstCecGpioLoc != NULL)
        {
            pstDetect = &g_cec_frame_dct[u8CecPinIndex];

            pstDetect->en_detect_mode = FALSE;
            pstDetect->cec_pin_cur_state = ePinLevel_High;
            pstDetect->cec_pin_pre_state = ePinLevel_High;
        }
    }
}

void cec_tx_send_frame(pstCecTxMsg_t pMsg)
{
    pstCecDeclare_t     pCecPort, pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t      pstFun;

    if(pMsg->cecPort == CEC_OPERATE_OUTPUT)//send to hdmi output
    {
        DisableCecDetect(CEC_OPERATE_OUTPUT);

        pCecPort = pCecPortOrg;
        while(pCecPort->pstCecGpioLoc != NULL)
        {
            if(pCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Dis)
            {
                pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

                pstFun->cecMsg_content = &pMsg->msgContent[pCecPort-pCecPortOrg];
                pstFun->cecMsg_tx_flag = TRUE;
                pstFun->timer_cnt = 0;
                pstFun->bit_cnt = 0;
                pstFun->get_cnt = 0;
                pstFun->cecMsg_state = cec_state_start;
            }
            pCecPort ++;
        }
    }
    else if(pMsg->cecPort == CEC_OPERATE_INPUT)
    {
        DisableCecDetect(CEC_OPERATE_INPUT);

        pCecPort = pCecPortOrg;
        while(pCecPort->pstCecGpioLoc != NULL)
        {
            if(pCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Src)
            {
                pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];

                pstFun->cecMsg_content = &pMsg->msgContent[pCecPort-pCecPortOrg];
                pstFun->cecMsg_tx_flag = TRUE;
                pstFun->timer_cnt = 0;
                pstFun->bit_cnt = 0;
                pstFun->get_cnt = 0;
                pstFun->cecMsg_state = cec_state_start;
            }
            pCecPort ++;
        }
    }
    else
    {
        DisableCecDetect(pMsg->cecPort);

        pstFun = &g_stCecPinFunTable[pMsg->cecPort];

        pstFun->cecMsg_content = &pMsg->msgContent[pMsg->cecPort];
        pstFun->cecMsg_tx_flag = TRUE;
        pstFun->timer_cnt = 0;
        pstFun->bit_cnt = 0;
        pstFun->get_cnt = 0;
        pstFun->cecMsg_state = cec_state_start;
    }
}

void cec_cmd_send(uint8_t u8CecPort, pstCecMsg_t pMsg, bool bFromIsr)
{
    pstCecDeclare_t         pCecPort, pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
#if _SYSTEM_SUPPORT_RTOS
    const TickType_t        xTicksToWait = 0;//portMAX_DELAY;
    BaseType_t              xHigherPriorityTaskWoken = pdFALSE;
#endif
    pstCecMsg_t             ptMsg, ptMsgOrg= &g_tx_cecMsg.msgContent[0];
    pstCecTxMsg_t           ptTxMsg = &g_tx_cecMsg;

    ptTxMsg->cecPort = u8CecPort;
    if(u8CecPort==CEC_OPERATE_INPUT)
    {
        pCecPort = pCecPortOrg;
        while(pCecPort->pstCecGpioLoc != NULL)
        {
            if(pCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Src)
            {
                ptMsg = ptMsgOrg + (pCecPort-pCecPortOrg);
                memcpy(ptMsg, pMsg, sizeof(stCecMsg_t));
            #if ENABLE_USER_CTRL_RELEASED
                if(pMsg->ucOpCode == CECOP_USER_CONTROL_PRESSED)
                {
                    mapp_CecUserCtrlReleasedPort(pCecPort->stCecCnntType.u8PortIndex,TIMES_USER_CTRL_RELEASED);
                }
            #endif //add by wf8421
            }
            pCecPort ++;
        }
    }
    else if(u8CecPort == CEC_OPERATE_OUTPUT)
    {
        pCecPort = pCecPortOrg;
        while(pCecPort->pstCecGpioLoc != NULL)
        {
            if(pCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Dis)
            {
                ptMsg = ptMsgOrg + (pCecPort-pCecPortOrg);
                memcpy(ptMsg, pMsg, sizeof(stCecMsg_t));
            #if ENABLE_USER_CTRL_RELEASED
                if(pMsg->ucOpCode == CECOP_USER_CONTROL_PRESSED)
                {
                    mapp_CecUserCtrlReleasedPort(pCecPort->stCecCnntType.u8PortIndex,TIMES_USER_CTRL_RELEASED);
                }
            #endif //add by wf8421
            }
            pCecPort ++;
        }
    }
    else
    {
        ptMsg = ptMsgOrg + u8CecPort;
        memcpy(ptMsg, pMsg, sizeof(stCecMsg_t));

        #if ENABLE_USER_CTRL_RELEASED
        if(pMsg->ucOpCode == CECOP_USER_CONTROL_PRESSED)
        {
            mapp_CecUserCtrlReleasedPort(u8CecPort,TIMES_USER_CTRL_RELEASED);
        }
        #endif //add by wf8421
    }

#if _SYSTEM_SUPPORT_RTOS
    if(!bFromIsr)
    {
        xQueueSend(xQueue_CecTx, (void *)&ptTxMsg, xTicksToWait);
    }
    else
    {
        xQueueSendFromISR(xQueue_CecTx, (void *)&ptTxMsg, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
#else
    cec_tx_send_frame(ptTxMsg);
#endif
}

void cec_cmd_tv_poweron(uint8_t u8CecPort, bool bFromIsr)
{
    stCecMsg_t cecMsg;

    //if(CEC_OPERATE_OUTPUT != u8CecPort)
    //    cecMsg.ucHeader = (g_rx_cecMsg[u8CecPort].addr_loc<<4)|CEC_LOGADDR_TV;
    ///else
    {
        cecMsg.ucHeader = (CEC_ADDR_LOCAL_OUT<<4)|CEC_LOGADDR_TV;
    }
    cecMsg.ucOpCode = CECOP_IMAGE_VIEW_ON;
    cecMsg.ucLen = 2;

    cec_cmd_send(u8CecPort, &cecMsg, bFromIsr);
    mapp_DisplayConnectToInitCecPort(u8CecPort,400);
}

void cec_cmd_tv_poweroff(uint8_t u8CecPort, bool bFromIsr)
{
    stCecMsg_t cecMsg;

    cecMsg.ucHeader = 0xF0;//(g_rx_cecMsg[u8CecPort].addr_loc<<4)|CEC_LOGADDR_TV;
    cecMsg.ucOpCode = CECOP_STANDBY;
    cecMsg.ucLen = 2;

    cec_cmd_send(u8CecPort, &cecMsg, bFromIsr);
    //mapp_DisplayConnectToInitCecPort(u8CecPort,4);
}

void cec_cmd_tv_volup(uint8_t u8CecPort, bool bFromIsr)
{
    stCecMsg_t cecMsg;

    if(CEC_OPERATE_OUTPUT != u8CecPort)
        cecMsg.ucHeader = (g_rx_cecMsg[u8CecPort].addr_loc<<4)|CEC_LOGADDR_TV;
    else
    {
        cecMsg.ucHeader = (CEC_ADDR_LOCAL_OUT<<4)|CEC_LOGADDR_TV;
    }
    cecMsg.ucOpCode = CECOP_USER_CONTROL_PRESSED;
    cecMsg.ucOperand[0] = CEC_RC_VOLUME_UP;
    cecMsg.ucLen = 3;

    cec_cmd_send(u8CecPort, &cecMsg, bFromIsr);
}

void cec_cmd_tv_voldown(uint8_t u8CecPort, bool bFromIsr)
{
    stCecMsg_t cecMsg;

    if(CEC_OPERATE_OUTPUT != u8CecPort)
        cecMsg.ucHeader = (g_rx_cecMsg[u8CecPort].addr_loc<<4)|CEC_LOGADDR_TV;
    else
    {
        cecMsg.ucHeader = (CEC_ADDR_LOCAL_OUT<<4)|CEC_LOGADDR_TV;
    }
    cecMsg.ucOpCode = CECOP_USER_CONTROL_PRESSED;
    cecMsg.ucOperand[0] = CEC_RC_VOLUME_DOWN;
    cecMsg.ucLen = 3;

    cec_cmd_send(u8CecPort, &cecMsg, bFromIsr);
}

void cec_cmd_tv_volmute(uint8_t u8CecPort, bool bFromIsr)
{
    stCecMsg_t cecMsg;

    if(CEC_OPERATE_OUTPUT != u8CecPort)
        cecMsg.ucHeader = (g_rx_cecMsg[u8CecPort].addr_loc<<4)|CEC_LOGADDR_TV;
    else
    {
        cecMsg.ucHeader = (CEC_ADDR_LOCAL_OUT<<4)|CEC_LOGADDR_TV;
    }
    cecMsg.ucOpCode = CECOP_USER_CONTROL_PRESSED;
    cecMsg.ucOperand[0] = CEC_RC_MUTE;
    cecMsg.ucLen = 3;

    cec_cmd_send(u8CecPort, &cecMsg, bFromIsr);
}

void cec_cmd_tv_source(uint8_t u8CecPort, bool bFromIsr)
{
    stCecMsg_t cecMsg;

    cecMsg.ucHeader = 0x4F;//(CEC_ADDR_LOCAL_OUT<<4)+0x00;
    cecMsg.ucOpCode = CECOP_ACTIVE_SOURCE;
    cecMsg.ucOperand[0] = (g_rx_cecMsg[u8CecPort].addr_physical>>8)&0xFF;
    cecMsg.ucOperand[1] = (g_rx_cecMsg[u8CecPort].addr_physical)&0xFF;
    cecMsg.ucLen = 4;

    cec_cmd_send(u8CecPort, &cecMsg, bFromIsr);
}

void cec_cmd_send_to_src(uint8_t u8CecPort, uint8_t u8Operand0, bool bFromIsr)
{
    stCecMsg_t  stCecSendMsg;

    memset(stCecSendMsg.ucOperand,0,SIZE_OF_CEC_OPERAND);
    if(CEC_OPERATE_INPUT != u8CecPort)
        stCecSendMsg.ucHeader = ((g_rx_cecMsg[u8CecPort].addr_loc<<4)|(g_rx_cecMsg[u8CecPort].addr_destination));
    else
    {
        stCecSendMsg.ucHeader = (CEC_ADDR_LOCAL_OUT);
    }
    //stCecSendMsg.ucHeader = 0x04;
    stCecSendMsg.ucOpCode = CECOP_USER_CONTROL_PRESSED;
    stCecSendMsg.ucOperand[0] = u8Operand0;
    stCecSendMsg.ucLen = 3;
    cec_cmd_send(u8CecPort, &stCecSendMsg, bFromIsr);
}

void cec_cmd_send_to_device(u8 u8DevID,
                                    u8 u8Addr,
                                    u8 u8Opcode,
                                    u8 *pu8Operand,
                                    u8 u8Size,
                                    bool from_isr)
{
    stCecMsg_t cecMsg;
    u8 i;
    memset(cecMsg.ucOperand,0,SIZE_OF_CEC_OPERAND);
    cecMsg.ucHeader = u8Addr;
    cecMsg.ucOpCode = u8Opcode;
    if(u8Size>14)
    {
        u8Size = 14;
    }

    for(i=0;i<u8Size;i++)
    {
        cecMsg.ucOperand[i] = pu8Operand[i];
    }
    if((u8Size==0)&&(u8Opcode==0))
        cecMsg.ucLen = 1;
    else
        cecMsg.ucLen = u8Size+2;

    cec_cmd_send(u8DevID, &cecMsg, from_isr);

#if 0
    if((cecMsg.ucOpCode == CECOP_IMAGE_VIEW_ON)
        //||(cecMsg.ucOpCode == CECOP_STANDBY)
        )
    {
        mapp_DisplayConnectToInitCecPort(u8DevID,4);
    }
#endif //add by wf8421
}

void cec_get_input_port_phyaddr(uint8_t u8Port, uint16_t u16Phyaddr)
{
    pstCecDeclare_t pstCecPort = (pstCecDeclare_t)&stProjectCecWorkTable[u8Port];

    if(pstCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Src)
    {
        g_rx_cecMsg[u8Port].addr_physical = u16Phyaddr;
    }
}

void mapi_CEC_SetDestinationAddress(uint8_t u8Port,u8 u8Addr)
{
    g_rx_cecMsg[u8Port].addr_destination = u8Addr;
}

u8 mapi_CEC_GetDestinationAddress(uint8_t u8Port)
{
    return g_rx_cecMsg[u8Port].addr_destination;
}

#if ENABLE_CEC_USER2
const u8 g_u8DefaultAdb[] = {

    0x38,
    0x0F, 0x7F, 0x07,
    0x15, 0x07, 0x50,
    0x35, 0x06, 0x3C,
    0x3E, 0x1E, 0xC0,
    0x4D, 0x02, 0x00,
    0x57, 0x06, 0x00,
    0x5F, 0x7E, 0x01,
    0x67, 0x7E, 0x00
//    0x32,
//    0x0F,0x1F,0x07,     // LPCM 8CH
//    0x15,0x07,0x50,     // dolby 6CH
//    0x3D,0x1E,0xC0,     // DTS 6CH
//    0x57,0x06,0x01,     // DD+ 8CH
//    0x5F,0x1E,0x01,     // DTS-HD 8CH
//    0x67,0x1E,0x00,     // Dolby TrueHD
};

//****************************************************************************
static void Cec_Cmd_OTPFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    //pstCecRxMsg_t p_rx_cecMsg;
    //stCecMsg_t cec_msg_send;

    //p_rx_cecMsg = &g_rx_cecMsg[cec_index];
    switch(pcec_msg->ucHeader){
        case    CECOP_ACTIVE_SOURCE:
            mapi_CEC_SetDestinationAddress(cec_index,(pcec_msg->ucHeader&0x0F)>>4);
            // ToDo
            break;
        case    CECOP_IMAGE_VIEW_ON:
            mapi_CEC_SetDestinationAddress(cec_index,(pcec_msg->ucHeader&0x0F)>>4);
                // ToDo
            break;
        case    CECOP_TEXT_VIEW_ON:
            mapi_CEC_SetDestinationAddress(cec_index,(pcec_msg->ucHeader&0x0F)>>4);
                // ToDo
            break;
    }
}
//****************************************************************************
static void Cec_Cmd_RoutingCtlFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucHeader){
        case CECOP_ROUTING_CHANGE:
            {
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_ROUTING_CHANGE;
                cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
#if 0
        case CECOP_ROUTING_INFORMATION:
            {
                if((pcec_msg->ucOperand[0] == (p_rx_cecMsg->addr_physical>>8)&0xFF)
                    &&(pcec_msg->ucOperand[1] == (p_rx_cecMsg->addr_physical)&0xFF))
                {
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | DEVICE_ID_BROADCAST;
                    cec_msg_send.ucOpCode = CECOP_ROUTING_INFORMATION;
                    cec_msg_send.ucOperand[0] = (p_rx_cecMsg->addr_physical>>8)&0xFF;
                    cec_msg_send.ucOperand[1] = (p_rx_cecMsg->addr_physical)&0xFF;
                }
                #if 0
                if((u8CecCmd[2] == pstCecSta->u8PaAB) && (u8CecCmd[3] == pstCecSta->u8PaCD)){
                    iTE_u8  u8Temp = pstCecSta->u8SelPort;

                    u8CecCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
                    u8CecCmd[1] = eRoutingInformation;
                    u8CecCmd[2] = pstCecSta->u8PaAB;
                    u8CecCmd[3] = pstCecSta->u8PaCD;
                    switch(pstCecSta->u8Stage){
                        case    0:
                                u8CecCmd[2] |= u8Temp << 4;
                            break;
                        case    1:
                                u8CecCmd[2] |= u8Temp;
                            break;
                        case    2:
                                u8CecCmd[3] |= u8Temp << 4;
                            break;
                        case    3:
                                u8CecCmd[4] |= u8Temp;
                            break;
                        default:
                                u8Temp = u8CecCmd[0] >> 4;
                                u8CecCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
                                u8CecCmd[1] = eFeatureAbort;
                                u8CecCmd[2] = eRecordTVScreen;
                                u8CecCmd[3] = CEC_ABORT_REASON_REFUSED;
                            return;
                    }
                    CecSys_PushToQueue(u8CecCmd, 4, &stTxQ);
                }
                #endif //add by wf8421
                l_bSendFlag = TRUE;
            }
            break;
#endif
        case CECOP_REQUEST_ACTIVE_SOURCE:
            {
                //u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                cec_msg_send.ucOpCode = CECOP_ACTIVE_SOURCE;
                cec_msg_send.ucOperand[0] = (p_rx_cecMsg->addr_physical>>8)&0xFF;
                cec_msg_send.ucOperand[1] = (p_rx_cecMsg->addr_physical)&0xFF;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
#if 1
        case CECOP_SET_STREAM_PATH:
            {
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                cec_msg_send.ucOpCode = CECOP_ACTIVE_SOURCE;
                cec_msg_send.ucOperand[0] = (p_rx_cecMsg->addr_physical>>8)&0xFF;
                cec_msg_send.ucOperand[1] = (p_rx_cecMsg->addr_physical)&0xFF;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
#endif
        case CECOP_INACTIVE_SOURCE:
            {
                //u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                cec_msg_send.ucOpCode = CECOP_ACTIVE_SOURCE;
                cec_msg_send.ucOperand[0] = (p_rx_cecMsg->addr_physical>>8)&0xFF;
                cec_msg_send.ucOperand[1] = (p_rx_cecMsg->addr_physical)&0xFF;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}
//****************************************************************************
static void Cec_Cmd_StandByFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    switch(pcec_msg->ucHeader){
        case    CECOP_STANDBY:
            break;
    }
}
//****************************************************************************
static void Cec_Cmd_OTRFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucHeader){
        case    CECOP_RECORD_OFF:
                // ToDo
            break;
        case    CECOP_RECORD_ON:
                // ToDo
            break;
        case    CECOP_RECORD_STATUS:
                // ToDo
            break;
        case    CECOP_RECORD_TV_SCREEN:
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_RECORD_TV_SCREEN;
                cec_msg_send.ucOperand[1] = CecAbortReason_CannotProvideSource;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}

//****************************************************************************
static void Cec_Cmd_SysInfoFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucOpCode){
        case CECOP_CEC_VERSION:
            {
                mapi_CEC_SetDestinationAddress(cec_index,(pcec_msg->ucHeader&0x0F)>>4);
            }
            break;
        case CECOP_GET_CEC_VERSION:
            {
                mapi_CEC_SetDestinationAddress(cec_index,(pcec_msg->ucHeader&0x0F)>>4);
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_CEC_VERSION;
                cec_msg_send.ucOperand[0] = CecVersion_V2_0;//CecVersion_V1_4;
                cec_msg_send.ucLen = 3;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
        case CECOP_GIVE_PHYSICAL_ADDRESS:
            {
                if(p_rx_cecMsg->addr_physical == 0x0000)
                    break;
                mapi_CEC_SetDestinationAddress(cec_index,(pcec_msg->ucHeader&0x0F)>>4);
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                cec_msg_send.ucOpCode = CECOP_REPORT_PHYSICAL_ADDRESS;
                cec_msg_send.ucOperand[0] = (p_rx_cecMsg->addr_physical)>>8;
                cec_msg_send.ucOperand[1] = (p_rx_cecMsg->addr_physical)&0xFF;
                cec_msg_send.ucOperand[2] = (p_rx_cecMsg->addr_loc);
                cec_msg_send.ucLen = 5;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
#if 0//(EN_CEC_TV == iTE_TRUE)
        case CECOP_GET_MENU_LANGUAGE:
            {
                u8CecCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
                u8CecCmd[1] = eSetMenuLanguage;
                u8CecCmd[2] = 'c';
                u8CecCmd[3] = 'h';
                u8CecCmd[4] = 'i';
                CecSys_PushToQueue(u8CecCmd, 5, &stTxQ);
                l_bSendFlag = TRUE;
            }
            break;
        case CECOP_REPORT_PHYSICAL_ADDRESS:
            {
                //u8CecList[u8CecCmd[0] >> 4][0] = u8CecCmd[2];
                //u8CecList[u8CecCmd[0] >> 4][1] = u8CecCmd[3];
                //u8CecList[u8CecCmd[0] >> 4][2] = u8CecCmd[4];
                l_bSendFlag = TRUE;
            }
            break;
#else
        case CECOP_GET_MENU_LANGUAGE:
            {
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_GET_MENU_LANGUAGE;
                cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
        case CECOP_REPORT_PHYSICAL_ADDRESS:
            break;
#endif
        case CECOP_SET_MENU_LANGUAGE:
            // ToDo
            break;
        case CECOP_REPORT_FEATURES:
            // ToDo
            break;
        case CECOP_GIVE_FEATURES:
            {
                /*  All Device Types data 1 byte description:
                    TV Bit 7
                    Recording Device                                Bit 6
                    Tuner                                           Bit 5
                    Playback Device                                 Bit 4
                    Audio System                                    Bit 3
                    CEC Switch                                      Bit 2
                    Reserved                                        Bit 1-0

                    [RC Profile Extension]                          Bit 7
                    If bit=0, [RC Profile TV] in bits 5-0
                    If bit=1, [RC Profile Source] in bits 5-0       Bit 6
                    [RC Profile TV] or [RC Profile Source],
                    as determined by bit 6                          Bit 5-0

                    Device Features data 1 byte description:
                    Device Features Extension                       Bit 7
                    "TV supports <Record TV Screen>"                Bit 6
                    "TV supports <Set OSD String>"                  Bit 5
                    "supports being controlled by Deck Control"     Bit 4
                    "Source supports <Set Audio Rate>"              Bit 3
                    "Sink supports ARC Tx"                          Bit 2
                    "Source supports ARC Rx"                        Bit 1
                    reserved                                        Bit 0
                    */
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                cec_msg_send.ucOpCode = CECOP_REPORT_FEATURES;
                cec_msg_send.ucOperand[0] = CecVersion_V1_4;//CecVersion_V2_0;    // CEC VERSION
                cec_msg_send.ucOperand[1] = 0x08;               // All Device Types  --Audio System
                cec_msg_send.ucOperand[2] = 0x00;               // RC Profile
                cec_msg_send.ucOperand[3] = 0x20|(ENABLE_USER_ARC_TX?BIT2:0)|(ENABLE_USER_ARC_RX?BIT1:0);   // Device Features
                cec_msg_send.ucLen = 6;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}
//****************************************************************************
static void Cec_Cmd_DeckCtrlFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucOpCode){
        case    CECOP_DECK_CONTROL:
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_DECK_CONTROL;
                cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            break;
        case    CECOP_DECK_STATUS:
            break;
        case    CECOP_GIVE_DECK_STATUS:
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_GIVE_DECK_STATUS;
                cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            break;
        case    CECOP_PLAY:
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_PLAY;
                cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}

//****************************************************************************
static void Cec_Cmd_VenSpecCmdFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];
    switch(pcec_msg->ucOpCode){
        case    CECOP_GIVE_DEVICE_VENDOR_ID:
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                //cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                //cec_msg_send.ucOperand[0] = CECOP_GIVE_DEVICE_VENDOR_ID;
                //cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                //cec_msg_send.ucLen = 4;
                cec_msg_send.ucOpCode = CECOP_DEVICE_VENDOR_ID;
                cec_msg_send.ucOperand[0] = sg_u8VendorID[0];
                cec_msg_send.ucOperand[1] = sg_u8VendorID[1];
                cec_msg_send.ucOperand[2] = sg_u8VendorID[2];
                cec_msg_send.ucLen = 5;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            break;

        case    CECOP_DEVICE_VENDOR_ID:
                if(from_isr == FALSE)
                {
                    CEC_DBG_PRINT(("VENDOR_ID:\r\n"));
                    for(u8Temp=0;u8Temp<pcec_msg->ucLen-2;u8Temp++)
                    {
                        CEC_DBG_PRINT(("%x ",pcec_msg->ucOperand[u8Temp]));
                    }
                    CEC_DBG_PRINT(("\r\n"));
                }
                break;

        case    CECOP_VENDOR_COMMAND:
                #if 0
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_VENDOR_COMMAND;
                cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
                #endif
                break;
        case    CECOP_VENDOR_REMOTE_BUTTON_DOWN:
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_VENDOR_REMOTE_BUTTON_DOWN;
                cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
                break;
        case    CECOP_VENDOR_REMOTE_BUTTON_UP:
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_VENDOR_REMOTE_BUTTON_UP;
                cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            break;
        case    CECOP_VENDOR_COMMAND_WITH_ID:
            break;
		case    CECOP_ACTIVE_SOURCE:
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}

//****************************************************************************
static void Cec_Cmd_DevOSDTransferFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    //iTE_u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucOpCode){
        case CECOP_GIVE_OSD_NAME:
            {
                //u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4)+0x0F;//((p_rx_cecMsg->addr_loc)<<4) | u8Temp;       0313
                cec_msg_send.ucOpCode = CECOP_SET_OSD_NAME;
                #if 1
                if(pstPrjSaveVar->stPrjNameSet.m_u8Len)
                {
                    if(pstPrjSaveVar->stPrjNameSet.m_u8Len<14)
                    {
                        memcpy(cec_msg_send.ucOperand,pstPrjSaveVar->stPrjNameSet.sPrjName,
                                pstPrjSaveVar->stPrjNameSet.m_u8Len);
                        cec_msg_send.ucLen = pstPrjSaveVar->stPrjNameSet.m_u8Len+2;
                    }
                    else
                    {
                        memcpy(cec_msg_send.ucOperand,pstPrjSaveVar->stPrjNameSet.sPrjName,14);
                        cec_msg_send.ucLen = 14+2;
                    }
                }
                else
                #endif
                {
                    memcpy(cec_msg_send.ucOperand,CEC_OSD_NAME,strlen(CEC_OSD_NAME));
                    cec_msg_send.ucLen = strlen(CEC_OSD_NAME)+2;
                }
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
        case CECOP_SET_OSD_NAME:
            {
                u8 i;

                for(i=0;i<pcec_msg->ucLen-2;i++)
                {
                    if(i>MAX_OSD_NAME_LEN)
                    {
                        g_u8OsdName[cec_index][MAX_OSD_NAME_LEN-1] = '\0';
                        break;
                    }
                    g_u8OsdName[cec_index][i] = pcec_msg->ucOperand[i];
                }
                if ( i <= MAX_OSD_NAME_LEN )
                {
                    g_u8OsdName[cec_index][i] = 0;
                }
                CEC_DBG_PRINT(("%s\r\n",g_u8OsdName[cec_index]));
            }
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}
//****************************************************************************
static void Cec_Cmd_RemoteCtlPassthroughFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
//    u8  u8Temp;
//    static u8 s_firstPress = 0x01;
//    pstCecRxMsg_t p_rx_cecMsg;
//    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    //p_rx_cecMsg = &g_rx_cecMsg[cec_index];
    switch(pcec_msg->ucOpCode){
        case CECOP_USER_CONTROL_PRESSED:
            //u8Temp = pcec_msg->ucOperand[0];
            break;
        case CECOP_USER_CONTROL_RELEASED:
            #if 0
            if(s_firstPress == 0x01)
            {
                s_firstPress = 0;

                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_USER_CONTROL_PRESSED;
                cec_msg_send.ucOperand[0] = CEC_RC_MUTE;
                cec_msg_send.ucLen = 3;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            #endif
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
//        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
//        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}
//****************************************************************************
static void Cec_Cmd_PowerStatusFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucOpCode){
        case CECOP_GIVE_DEVICE_POWER_STATUS:
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_REPORT_POWER_STATUS;
                cec_msg_send.ucOperand[0] = CEC_POWERSTATUS_ON;
                cec_msg_send.ucLen = 3;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            break;
        case CECOP_REPORT_POWER_STATUS:
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }

}
//****************************************************************************
static void Cec_Cmd_GeneralProtocolMessage(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucOpCode){
        case CECOP_FEATURE_ABORT:
            {
                mapi_CEC_SetDestinationAddress(cec_index,(pcec_msg->ucHeader>>4)&0x0F);
	            CEC_DBG_PRINT(("Cec[%d] addr = 0x%x\r\n",cec_index,(pcec_msg->ucHeader>>4)&0x0F));
            }
            break;
        case CECOP_ABORT:
            {
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                cec_msg_send.ucOperand[0] = CECOP_ABORT;
                cec_msg_send.ucOperand[1] = CecAbortReason_Refused;
                cec_msg_send.ucLen = 4;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}

//****************************************************************************
static void Cec_Cmd_SysAudioCtlFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp, u8Cnt1, u8Cnt2;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;
    static u8 sl_u8SystemAudioMode = 0;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucOpCode){
        case CECOP_GIVE_AUDIO_STATUS:
            {
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_REPORT_AUDIO_STATUS;
                cec_msg_send.ucOperand[0] = 0x5A;//0x80 | 0x5A;
                cec_msg_send.ucLen = 3;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
        case CECOP_GIVE_SYSTEM_AUDIO_MODE_STATUS:
            {
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_SYSTEM_AUDIO_MODE_STATUS;
                cec_msg_send.ucOperand[0] = sl_u8SystemAudioMode;
                cec_msg_send.ucLen = 3;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
        case CECOP_REPORT_AUDIO_STATUS:
            break;
        case CECOP_REPORT_SHORT_AUDIO:
            break;
        case CECOP_REQUEST_SHORT_AUDIO:
            {
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_REPORT_SHORT_AUDIO;
                u8Temp = 0;
                if(g_u8DefaultAdb[0]){
                    iTE_u8  u8AdoCnt = g_u8DefaultAdb[0] & 0x1F;
                    for(u8Cnt1 = 0; u8Cnt1 <4 ; u8Cnt1++)
                    {
                        for(u8Cnt2 = 1; u8Cnt2 < u8AdoCnt; )
                        {
                            if(pcec_msg->ucOperand[u8Cnt1])
                            {
                                if(pcec_msg->ucOperand[u8Cnt1] == ((g_u8DefaultAdb[u8Cnt2] & 0x78) >> 3)){
                                    cec_msg_send.ucOperand[u8Temp++] = g_u8DefaultAdb[u8Cnt2++];
                                    cec_msg_send.ucOperand[u8Temp++] = g_u8DefaultAdb[u8Cnt2++];
                                    cec_msg_send.ucOperand[u8Temp++] = g_u8DefaultAdb[u8Cnt2++];
                                    break;
                                }
                                else
                                    u8Cnt2 += 3;
                            }
                            else
                                break;
                        }
                    }
                }
                if(u8Temp != 0){
                    cec_msg_send.ucLen = u8Temp+2;
                    cec_cmd_send(cec_index, &cec_msg_send, TRUE);
                    l_bSendFlag = TRUE;
                }else{
                    u8Temp = pcec_msg->ucHeader >> 4;
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                    cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                    cec_msg_send.ucOperand[0] = CECOP_REQUEST_SHORT_AUDIO;
                    cec_msg_send.ucOperand[1] = CecAbortReason_InvalidOperand;//CecAbortReason_Refused;
                    cec_msg_send.ucLen = 4;
                    cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                    l_bSendFlag = TRUE;
                }
            }
            break;
        case CECOP_SET_SYSTEM_AUDIO_MODE:
            {
                sl_u8SystemAudioMode = cec_msg_send.ucOperand[0];
            }
            break;
        case CECOP_SYSTEM_AUDIO_MODE_REQUEST:
            {
                #if 0//ENABLE_PTN_IT66341
                //u8Temp = pcec_msg->ucHeader >> 4;
                //cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                cec_msg_send.ucOpCode = CECOP_SET_SYSTEM_AUDIO_MODE;
                cec_msg_send.ucOperand[0] = 1;
                cec_msg_send.ucLen = 1;
                cec_cmd_send(cec_output_all, &cec_msg_send, TRUE);
                l_bSendFlag = TRUE;
                #else
                if(pcec_msg->ucOperand[0]||(pcec_msg->ucLen>2)){
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                    cec_msg_send.ucOpCode = CECOP_SET_SYSTEM_AUDIO_MODE;
                    cec_msg_send.ucOperand[0] = 1;
                    cec_msg_send.ucLen = 3;
                    cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                    sl_u8SystemAudioMode = cec_msg_send.ucOperand[0];
                    l_bSendFlag = TRUE;
                }else{
                    #if 1
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | CEC_LOGADDR_BROADCAST;
                    cec_msg_send.ucOpCode = CECOP_SET_SYSTEM_AUDIO_MODE;
                    cec_msg_send.ucOperand[0] = 0;
                    cec_msg_send.ucLen = 3;
                    cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                    sl_u8SystemAudioMode = cec_msg_send.ucOperand[0];
                    l_bSendFlag = TRUE;
                    #endif //add by wf8421
                }
                #endif
            }
            break;
        case CECOP_SYSTEM_AUDIO_MODE_STATUS:
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}

//****************************************************************************
static void Cec_Cmd_ARCFeature(uint8_t cec_index,pstCecMsg_t pcec_msg,bool from_isr)
{
    u8  u8Temp;
    pstCecRxMsg_t p_rx_cecMsg;
    stCecMsg_t cec_msg_send;
    bool l_bSendFlag = FALSE;

    p_rx_cecMsg = &g_rx_cecMsg[cec_index];

    switch(pcec_msg->ucOpCode){
        case CECOP_INITIATE_ARC:
            {
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_REPORT_ARC_INITIATED;
                cec_msg_send.ucLen = 2;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
        case CECOP_REPORT_ARC_INITIATED:
            break;
        case CECOP_REPORT_ARC_TERMINATED:
                // function implement  for ARC Rx De-activate
                u8Temp = 0x00;
            break;
        case CECOP_REQUEST_ARC_INITIATION:
                // function implement for ARC Rx Initiation
                if(1){
                    u8Temp = pcec_msg->ucHeader >> 4;
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                    cec_msg_send.ucOpCode = CECOP_INITIATE_ARC;
                    cec_msg_send.ucLen = 2;
                    cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                    l_bSendFlag = TRUE;
                }else{
                    u8Temp = pcec_msg->ucHeader >> 4;
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                    cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                    cec_msg_send.ucOperand[0] = CECOP_REQUEST_ARC_INITIATION;
                    cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                    cec_msg_send.ucLen = 4;
                    cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                    l_bSendFlag = TRUE;
                }
            break;
        case CECOP_REQUEST_ARC_TERMINATION:
                if(1){
                    u8Temp = pcec_msg->ucHeader >> 4;
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                    cec_msg_send.ucOpCode = CECOP_TERMINATE_ARC;
                    cec_msg_send.ucLen = 3;
                    cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                    l_bSendFlag = TRUE;
                }else{
                    u8Temp = pcec_msg->ucHeader >> 4;
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                    cec_msg_send.ucOpCode = CECOP_FEATURE_ABORT;
                    cec_msg_send.ucOperand[0] = CECOP_REQUEST_ARC_TERMINATION;
                    cec_msg_send.ucOperand[1] = CecAbortReason_UnrecognizedOpcode;
                    cec_msg_send.ucLen = 4;
                    cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                    l_bSendFlag = TRUE;
                }
            break;
        case CECOP_TERMINATE_ARC:
            {
                u8Temp = pcec_msg->ucHeader >> 4;
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4) | u8Temp;
                cec_msg_send.ucOpCode = CECOP_REPORT_ARC_TERMINATED;
                cec_msg_send.ucLen = 2;
                cec_cmd_send(cec_index, &cec_msg_send, from_isr);
                l_bSendFlag = TRUE;
            }
            break;
    }

    if(l_bSendFlag)
    {
        l_bSendFlag = FALSE;
        #if DBG_CEC_RX
        memcpy(&g_stCecTxDbgMsg[cec_index],&cec_msg_send,sizeof(stCecMsg_t));
        g_bCecTxDbgFlag[cec_index] = TRUE;
        #endif //add by wf8421
    }
}
#endif

#if _ENABLE_SWITCH_PORT_INIT_CEC_FUNCTION
void cec_init_to_function(bool from_isr)
{
    uint8_t cec_index = (g_pu8RxSelComm[0]+1);
    if(g_u8SWSrcInitCecCnt>0)
    {
        if(!PlatformTimerExpired(etUser_CecInitTimer))
            return;
        if(cec_index>=NUM_OF_CEC_PINS_MAX)
        {
            g_u8SWSrcInitCecCnt = 0;
            PlatformTimerSet(etUser_CecInitTimer,0);
            return;
        }
        g_u8SWSrcInitCecCnt--;

        #if (_EN_HDMI_INPUT_CEC_PIN_RECEIVE == _MACRO_ENABLE)
        {
            stCecMsg_t cecMsg;
            u8 l_u8Header[4] = {0x04,0x08,0x0B,0x0F};

            cecMsg.ucHeader = l_u8Header[g_u8SWSrcInitCecCnt%4];
            cecMsg.ucOpCode = CECOP_GET_CEC_VERSION;
            cecMsg.ucLen = 2;
            cec_cmd_send(cec_index, &cecMsg, from_isr);
        }
        #endif
        PlatformTimerSet(etUser_CecInitTimer,500);
    }
}

void cec_set_init_function_couter(uint8_t u8Cnt)
{
    g_u8SWSrcInitCecCnt = u8Cnt*4;
    PlatformTimerSet(etUser_CecInitTimer,1000);
}
#endif

void cec_get_display_port_phyaddr(uint8_t u8Port, uint16_t u16Phyaddr)
{
    pstCecDeclare_t pstCecPort = (pstCecDeclare_t)&stProjectCecWorkTable[u8Port];

    if(pstCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Dis)
    {
        //Uart_printf("Port[%d] PHY = %04x\r\n",u8Port,u16Phyaddr);
        g_rx_cecMsg[u8Port].addr_physical = u16Phyaddr;
        mapp_DisplayConnectToInitCecPort(u8Port,1000);
    }
}

void TxPollingMsg(u8 u8Port,u8 u8Head)        // 轮询
{
    stCecMsg_t cecMsg;

    cecMsg.ucHeader = u8Head;//(g_rx_cecMsg[u8Port].addr_loc&0x0F)|(g_rx_cecMsg[u8Port].addr_loc<<4);
    cecMsg.ucOpCode = 0;
    cecMsg.ucLen = 1;
    cec_cmd_send(u8Port, &cecMsg, FALSE);

#if DBG_CEC_RX
    memcpy(&g_stCecTxDbgMsg[u8Port],&cecMsg,sizeof(stCecMsg_t));
    g_bCecTxDbgFlag[u8Port] = TRUE;
#endif //add by wf8421
}

void cec_init_port(void)
{
    u8 i;

    for(i=0;i<NUM_OF_CEC_PINS_MAX;i++)
    {
        pstCecDeclare_t pstCecPort = (pstCecDeclare_t)&stProjectCecWorkTable[i];
        if(pstCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Dis)
            continue;
        TxPollingMsg(i,(g_rx_cecMsg[i].addr_loc<<4)|(g_rx_cecMsg[i].addr_loc&0xF));
    }
}

void mapp_DisplayConnectToInitCecPort(u8 u8Port,u16 u16DelayMs)
{
    emCecRelinkState_t eRelinkAction[] =
    {
        #if ENABLE_USER_ARC_RX
        emCecRelinkSta_AudioInit,
        emCecRelinkSta_ArcInit,
        #else
        emCecRelinkSta_PollingMsg,
//        emCecRelinkSta_SetPhyAddrMsg,
//        emCecRelinkSta_SetOsdName,
        emCecRelinkSta_GetCecVersion,
        #endif //add by wf8421
    };

    mapp_CecRelinkMessage(u8Port,eRelinkAction,sizeof(eRelinkAction),u16DelayMs);
}

void mapp_InputConnectToInitCecPort(u8 u8Port,u16 u16DelayMs)
{
    emCecRelinkState_t eRelinkAction[] =
    {
        #if ENABLE_USER_ARC_TX
        emCecRelinkSta_RequestArcInit,
        emCecRelinkSta_ReportArcInit,//0319
        #else
        emCecRelinkSta_RxPollingMsg,
        //emCecRelinkSta_GetPhyAddrMsg,
        //emCecRelinkSta_GetOsdName,
        emCecRelinkSta_GetCecVersion,
        #endif //add by wf8421
    };
    mapp_CecRelinkMessage(u8Port,eRelinkAction,sizeof(eRelinkAction),u16DelayMs);
}

void mapp_CecUserCtrlReleasedPort(u8 u8Port,u16 u16DelayMs)
{
    emCecRelinkState_t eRelinkAction[] =
    {
        emCecRelinkSta_UserControlReleased,
    };
    mapp_CecRelinkMessage(u8Port,eRelinkAction,sizeof(eRelinkAction),u16DelayMs);
}

#if ENABLE_USER_ARC_RX
void mapp_CecArcInit(uint8_t u8Port)
{
    pstCecDeclare_t pstCecPort = (pstCecDeclare_t)&stProjectCecWorkTable[u8Port];

    if(pstCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Dis)
    {
        emCecRelinkState_t eRelinkAction[] =
        {
            emCecRelinkSta_AudioInit,
            emCecRelinkSta_ArcInit,
        };
        mapp_CecRelinkMessage(u8Port,eRelinkAction,sizeof(eRelinkAction),300);
    }
}
#endif //add by wf8421

void mapp_CecRelinkMessage(u8 u8Port,emCecRelinkState_t *peState,u8 u8StaCnt,u16 u16DelayTimeMs)
{
    u8 i;
    for(i=0;i<u8StaCnt;i++)
    {
        g_stCecRelinkAction[u8Port].u8CecActionSta[i] = peState[i];
    }
    g_stCecRelinkAction[u8Port].u16CecLoopDelayTime = u16DelayTimeMs/10; // unit = 10ms
    g_stCecRelinkAction[u8Port].u8CecActionCnt = u8StaCnt;
    g_stCecRelinkAction[u8Port].u8CecRunningCnt = 0;
    mapp_CecTimerSet(u8Port,u16DelayTimeMs);
}

static void mapp_CecRelinkActionProcess(void)
{
    u8 l_u8Index = 0;

    for(l_u8Index = 0;l_u8Index < NUM_OF_CEC_PINS_MAX; l_u8Index++)
    {
        pstCecDeclare_t pstCecPort = (pstCecDeclare_t)&stProjectCecWorkTable[l_u8Index];
        u8 l_u8temp;
        stCecMsg_t cecMsg;

        if(!mapp_CecTimerExpired(l_u8Index))
        {
            continue;
        }

        if(pstCecPort->pstCecGpioLoc == NULL)
            break;

        if(g_stCecRelinkAction[l_u8Index].u8CecActionCnt > g_stCecRelinkAction[l_u8Index].u8CecRunningCnt)
        {
            g_u8CecDisplayInitSta[l_u8Index] = \
                            g_stCecRelinkAction[l_u8Index].u8CecActionSta[g_stCecRelinkAction[l_u8Index].u8CecRunningCnt];
            mapp_CecTimerSet(l_u8Index,g_stCecRelinkAction[l_u8Index].u16CecLoopDelayTime);
            g_stCecRelinkAction[l_u8Index].u8CecRunningCnt++;
            if(g_stCecRelinkAction[l_u8Index].u8CecActionCnt == g_stCecRelinkAction[l_u8Index].u8CecRunningCnt)
            {
                g_stCecRelinkAction[l_u8Index].u8CecActionCnt = 0;
                g_stCecRelinkAction[l_u8Index].u8CecRunningCnt = 0;
            }
        }
        else
        {
            g_stCecRelinkAction[l_u8Index].u8CecActionCnt = 0;
            g_stCecRelinkAction[l_u8Index].u8CecRunningCnt = 0;
        }

        switch(g_u8CecDisplayInitSta[l_u8Index])
        {
            case emCecRelinkSta_PollingMsg:
                {
                    TxPollingMsg(l_u8Index,(g_rx_cecMsg[l_u8Index].addr_loc<<4)|(g_rx_cecMsg[l_u8Index].addr_loc&0xF));
                    CEC_DBG_PRINT(("[%d]InitPollingMsg\r\n",l_u8Index));
                }
                break;
            case emCecRelinkSta_SetPhyAddrMsg:
                {
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | CEC_LOGADDR_BROADCAST;
                    cecMsg.ucOpCode = CECOP_REPORT_PHYSICAL_ADDRESS;
                    cecMsg.ucOperand[0] = (g_rx_cecMsg[l_u8Index].addr_physical)>>8;
                    cecMsg.ucOperand[1] = (g_rx_cecMsg[l_u8Index].addr_physical)&0xFF;
                    cecMsg.ucOperand[2] = (g_rx_cecMsg[l_u8Index].addr_loc);
                    cecMsg.ucLen = 5;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]SetPhyAddrMsg\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                    g_stCecRelinkAction[l_u8Index].u8RelinkSuccessFlag = 1;
                }
                break;
            case emCecRelinkSta_SetOsdName:
                {
                    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
                    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

                    if(pstCecPort->stCecCnntType.eCecDirType != eCecSrcDisType_Dis)
                    {
                        g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                        break;
                    }

                    #if ENABLE_CEC_USER2
                    l_u8temp = g_stCecRxMsg[l_u8Index][0].ucHeader<<4;
                    #else
                    l_u8temp = 0;
                    #endif
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | 0x0F;//l_u8temp;//0313
                    cecMsg.ucOpCode = CECOP_SET_OSD_NAME;
                    #if 1
                    if(pstPrjSaveVar->stPrjNameSet.m_u8Len)
                    {
                        if(pstPrjSaveVar->stPrjNameSet.m_u8Len<14)
                        {
                            memcpy(cecMsg.ucOperand,pstPrjSaveVar->stPrjNameSet.sPrjName,
                                    pstPrjSaveVar->stPrjNameSet.m_u8Len);
                            cecMsg.ucLen = pstPrjSaveVar->stPrjNameSet.m_u8Len+2;
                        }
                        else
                        {
                            memcpy(cecMsg.ucOperand,pstPrjSaveVar->stPrjNameSet.sPrjName,14);
                            cecMsg.ucLen = 14+2;
                        }
                    }
                    else
                    #endif
                    {
                        memcpy(cecMsg.ucOperand,CEC_OSD_NAME,strlen(CEC_OSD_NAME));
                        //cecMsg.ucOperand[strlen(CEC_OSD_NAME)] = 0x00;
                        cecMsg.ucLen = strlen(CEC_OSD_NAME)+2;
                    }
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]SetOsdName\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                }
                break;

            #if ENABLE_USER_ARC_RX
            case emCecRelinkSta_AudioInit:
                {

                    if(pstCecPort->stCecCnntType.eCecDirType != eCecSrcDisType_Dis)
                    {
                        g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                        break;
                    }

                    #if ENABLE_CEC_USER2
                    l_u8temp = g_stCecRxMsg[l_u8Index][0].ucHeader<<4;
                    #else
                    l_u8temp = 0;
                    #endif
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | l_u8temp;
                    cecMsg.ucOpCode = CECOP_SET_SYSTEM_AUDIO_MODE;
                    cecMsg.ucOperand[0] = 0x01;
                    cecMsg.ucLen = 3;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]AudioInit\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                }
                break;
            case emCecRelinkSta_ArcInit:
                {
                    if(pstCecPort->stCecCnntType.eCecDirType != eCecSrcDisType_Dis)
                    {
                        g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                        break;
                    }

                    #if ENABLE_CEC_USER2
                    l_u8temp = g_stCecRxMsg[l_u8Index][0].ucHeader<<4;
                    #else
                    l_u8temp = 0;
                    #endif
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | l_u8temp;
                    cecMsg.ucOpCode = CECOP_INITIATE_ARC;
                    cecMsg.ucLen = 2;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]ArcInit\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                }
                break;
            #endif //add by wf8421

            case emCecRelinkSta_RxPollingMsg:
                {
                    #if ENABLE_CEC_USER2
                    l_u8temp = mapi_CEC_GetDestinationAddress(l_u8Index);
                    #else
                    l_u8temp = CEC_LOGADDR_PLAYBACK1;
                    #endif
                    TxPollingMsg(l_u8Index,(g_rx_cecMsg[l_u8Index].addr_loc<<4)|l_u8temp);
                    CEC_DBG_PRINT(("[%d]InitRxPollingMsg\r\n",l_u8Index));
                }
                break;
            case emCecRelinkSta_GetPhyAddrMsg:
                {
                    if(pstCecPort->stCecCnntType.eCecDirType != eCecSrcDisType_Src)
                    {
                        g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                        break;
                    }
                    #if ENABLE_CEC_USER2
                    l_u8temp = mapi_CEC_GetDestinationAddress(l_u8Index);
                    #else
                    l_u8temp = CEC_LOGADDR_PLAYBACK1;
                    #endif
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | l_u8temp;
                    cecMsg.ucOpCode = CECOP_GIVE_PHYSICAL_ADDRESS;
                    cecMsg.ucLen = 2;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]SetPhyAddrMsg\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                }
                break;
            case emCecRelinkSta_GetOsdName:
                {
                    if(pstCecPort->stCecCnntType.eCecDirType != eCecSrcDisType_Src)
                    {
                        g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                        break;
                    }
                    #if ENABLE_CEC_USER2
                    l_u8temp = mapi_CEC_GetDestinationAddress(l_u8Index);
                    #else
                    l_u8temp = CEC_LOGADDR_PLAYBACK1;
                    #endif
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | l_u8temp;
                    cecMsg.ucOpCode = CECOP_GIVE_OSD_NAME;
                    cecMsg.ucLen = 2;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]GiveOsdName\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                }
                break;
            case emCecRelinkSta_RequestArcInit:
                {
                    if(pstCecPort->stCecCnntType.eCecDirType != eCecSrcDisType_Src)
                    {
                        g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                        break;
                    }
                    #if ENABLE_CEC_USER2
                    l_u8temp = mapi_CEC_GetDestinationAddress(l_u8Index);
                    #else
                    l_u8temp = CEC_LOGADDR_PLAYBACK1;
                    #endif
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | l_u8temp;
                    cecMsg.ucOpCode = CECOP_REQUEST_ARC_INITIATION;
                    cecMsg.ucLen = 2;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]RequestArcInit\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                }
                break;
			case emCecRelinkSta_ReportArcInit:
                {
                	#if 1//0313
                    if(pstCecPort->stCecCnntType.eCecDirType != eCecSrcDisType_Src)
                    {
                        g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                        break;
                    }
                    #if ENABLE_CEC_USER2
                    l_u8temp = mapi_CEC_GetDestinationAddress(l_u8Index);
                    #else
                    l_u8temp = CEC_LOGADDR_PLAYBACK1;
                    #endif
                    cecMsg.ucHeader = 05;//(g_rx_cecMsg[l_u8Index].addr_loc<<4) | l_u8temp;
                    cecMsg.ucOpCode = CECOP_REPORT_ARC_INITIATED;
                    cecMsg.ucLen = 2;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]RequestArcInit\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
					#endif
                }
                break;
            case emCecRelinkSta_GetCecVersion:
                {
                    #if ENABLE_CEC_USER2
                    l_u8temp = mapi_CEC_GetDestinationAddress(l_u8Index);
                    #else
                    l_u8temp = CEC_LOGADDR_PLAYBACK1;
                    #endif
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | l_u8temp;
                    cecMsg.ucOpCode = CECOP_GET_CEC_VERSION;
                    cecMsg.ucLen = 2;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]RequestArcInit\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                }
                break;
            case emCecRelinkSta_UserControlReleased:
                {
                    l_u8temp = mapi_CEC_GetDestinationAddress(l_u8Index);
                    cecMsg.ucHeader = (g_rx_cecMsg[l_u8Index].addr_loc<<4) | l_u8temp;
                    cecMsg.ucOpCode = CECOP_USER_CONTROL_RELEASED;
                    cecMsg.ucLen = 2;
                    cec_cmd_send(l_u8Index, &cecMsg, FALSE);
                    g_u8CecDisplayInitSta[l_u8Index] = emCecRelinkSta_None;
                    CEC_DBG_PRINT(("[%d]UserControlReleased\r\n",l_u8Index));
                    #if DBG_CEC_RX
                    memcpy(&g_stCecTxDbgMsg[l_u8Index],&cecMsg,sizeof(stCecMsg_t));
                    g_bCecTxDbgFlag[l_u8Index] = TRUE;
                    #endif //add by wf8421
                }
                break;
            default:
                break;
        }
    }
}

void cec_msg_parse(pstCecDeclare_t pCecPort)
{
    pstCecDeclare_t         pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecMsg_t             pcec_msg;
    pstCecRxMsg_t           p_rx_cecMsg;
    stCecMsg_t              cec_msg_send;
    p_rx_cecMsg = &g_rx_cecMsg[pCecPort-pCecPortOrg];
    pcec_msg = cec_msg_pop(pCecPort);

    if(pcec_msg != NULL)
    {
        #if 0//((_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY))&&(!DBG_CEC_RX)
        {
            pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
            pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
            pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;

            //manual mode, and it isn't the active rx
            if(!pstHdmiSw->bAutoSwitch)
            {
                if(((pCecPort-pCecPortOrg) != pstHdmiSw->u8SwChannel)&&(pCecPort->stCecCnntType.eCecDirType==eCecSrcDisType_Src))
                {
                    return;
                }
            }
        }
        #endif

        #if DBG_CEC_RX
        g_bRxDbgRecevieFlag[pCecPort-pCecPortOrg] = TRUE;
        g_stCecRxDbgMsg[pCecPort-pCecPortOrg].ucHeader = pcec_msg->ucHeader;
        g_stCecRxDbgMsg[pCecPort-pCecPortOrg].ucOpCode = pcec_msg->ucOpCode;
        {
            u8 i;
            if(pcec_msg->ucLen>2)
            {
                for(i=0;i<pcec_msg->ucLen-2;i++)
                    g_stCecRxDbgMsg[pCecPort-pCecPortOrg].ucOperand[i] = pcec_msg->ucOperand[i];
            }
        }
        g_stCecRxDbgMsg[pCecPort-pCecPortOrg].ucLen = pcec_msg->ucLen;
        #endif

        switch(pcec_msg->ucOpCode)
        {
            //case CECOP_REPORT_PHYSICAL_ADDRESS:
            case CECOP_CEC_VERSION:
            case CECOP_REPORT_POWER_STATUS:
                p_rx_cecMsg->addr_destination = ((pcec_msg->ucHeader&0x0F)>>4);
                break;

            case CECOP_IMAGE_VIEW_ON://send to tv via cec output pin
                p_rx_cecMsg->addr_destination = ((pcec_msg->ucHeader&0x0F)>>4);
                cec_msg_send.ucHeader = ((CEC_ADDR_LOCAL_OUT)<<4)+0x00;
                cec_msg_send.ucOpCode = CECOP_IMAGE_VIEW_ON;
                cec_msg_send.ucLen = 2;
                cec_cmd_send(CEC_OPERATE_OUTPUT, &cec_msg_send, TRUE);
                break;

            case CECOP_TEXT_VIEW_ON://send to tv via cec output pin
                p_rx_cecMsg->addr_destination = ((pcec_msg->ucHeader&0x0F)>>4);
                cec_msg_send.ucHeader = ((CEC_ADDR_LOCAL_OUT)<<4)+0x00;
                cec_msg_send.ucOpCode = CECOP_TEXT_VIEW_ON;
                cec_msg_send.ucLen = 2;
                cec_cmd_send(CEC_OPERATE_OUTPUT, &cec_msg_send, TRUE);
                break;

            case CECOP_STANDBY://send to tv via cec output pin
                //cec_msg_send.ucHeader = ((CEC_ADDR_LOCAL_OUT)<<4)+0x00;
                //cec_msg_send.ucOpCode = CECOP_STANDBY;
                //cec_msg_send.ucLen = 2;
                //cec_cmd_send(cec_output_all, &cec_msg_send, TRUE);
                break;

            //case CECOP_REPORT_POWER_STATUS:
            //  break;

            case CECOP_GIVE_DEVICE_VENDOR_ID:
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4)+0x0F;
                cec_msg_send.ucOpCode = CECOP_DEVICE_VENDOR_ID;
                cec_msg_send.ucOperand[0] = 0x50; //'P'
                cec_msg_send.ucOperand[1] = 0x54; //'T'
                cec_msg_send.ucOperand[2] = 0x4E; //'N'
                cec_msg_send.ucLen = 5;
                cec_cmd_send((pCecPort-pCecPortOrg), &cec_msg_send, TRUE);
                break;

            case CECOP_GIVE_PHYSICAL_ADDRESS:
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4)+0x0F;
                cec_msg_send.ucOpCode = CECOP_REPORT_PHYSICAL_ADDRESS;
                cec_msg_send.ucOperand[0] = (uint8_t)(p_rx_cecMsg->addr_physical>>8);
                cec_msg_send.ucOperand[1] = (uint8_t)(p_rx_cecMsg->addr_physical&0xFF);
                cec_msg_send.ucOperand[2] = 0x04;
                cec_msg_send.ucLen = 5;
                cec_cmd_send((pCecPort-pCecPortOrg), &cec_msg_send, TRUE);
                break;

            case CECOP_GIVE_OSD_NAME:
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4)+(p_rx_cecMsg->addr_destination);
                cec_msg_send.ucOpCode = CECOP_SET_OSD_NAME;
                memcpy(&cec_msg_send.ucOperand[0], CEC_OSD_NAME, strlen(CEC_OSD_NAME));
                cec_msg_send.ucLen = strlen(CEC_OSD_NAME)+2;
                cec_cmd_send((pCecPort-pCecPortOrg), &cec_msg_send, TRUE);
                break;

            #if 1
            case CECOP_SET_OSD_NAME:
                {
                    u8 i;
                    u8 l_u8DevId = pCecPort-pCecPortOrg;
                    for(i=0;i<pcec_msg->ucLen;i++)
                    {
                        if(i>MAX_OSD_NAME_LEN)
                        {
                            g_u8OsdName[l_u8DevId][MAX_OSD_NAME_LEN-1] = '\0';
                            break;
                        }
                        g_u8OsdName[l_u8DevId][i] = pcec_msg->ucOperand[i];
                    }
                    if ( i <= MAX_OSD_NAME_LEN )
                    {
                        g_u8OsdName[l_u8DevId][i] = 0;
                    }
                }
                break;
            #endif

            case CECOP_GET_CEC_VERSION:
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4)+(p_rx_cecMsg->addr_destination);
                cec_msg_send.ucOpCode = CECOP_CEC_VERSION;
                cec_msg_send.ucOperand[0] = 0x05;
                cec_msg_send.ucLen = 3;
                cec_cmd_send((pCecPort-pCecPortOrg), &cec_msg_send, TRUE);
                break;

            case CECOP_REQUEST_ACTIVE_SOURCE:
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4)+(p_rx_cecMsg->addr_destination);
                cec_msg_send.ucOpCode = CECOP_ACTIVE_SOURCE;
                cec_msg_send.ucOperand[0] = (g_rx_cecMsg[pCecPort-pCecPortOrg].addr_physical>>8)&0xFF;
                cec_msg_send.ucOperand[1] = (g_rx_cecMsg[pCecPort-pCecPortOrg].addr_physical)&0xFF;
                cec_msg_send.ucLen = 4;
                cec_cmd_send((pCecPort-pCecPortOrg), &cec_msg_send, TRUE);
                break;

            case CECOP_GET_MENU_LANGUAGE:
                cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4)+(0x0F);
                cec_msg_send.ucOpCode = CECOP_SET_MENU_LANGUAGE;
                cec_msg_send.ucOperand[0] = 0x63;
                cec_msg_send.ucOperand[1] = 0x68;
                cec_msg_send.ucOperand[2] = 0x69;
                cec_msg_send.ucLen = 5;
                cec_cmd_send((pCecPort-pCecPortOrg), &cec_msg_send, TRUE);
                break;

            case CECOP_ROUTING_CHANGE:
                break;

            case CECOP_USER_CONTROL_PRESSED:
                break;

            #if 0
            case CECOP_GIVE_DEVICE_POWER_STATUS:
                {
                    cec_msg_send.ucHeader = ((p_rx_cecMsg->addr_loc)<<4)+(p_rx_cecMsg->addr_destination);
                    cec_msg_send.ucOpCode = CECOP_REPORT_POWER_STATUS;
                    cec_msg_send.ucOperand[0] = 0x00;
                    cec_msg_send.ucLen = 3;
                    cec_cmd_send((pCecPort-pCecPortOrg), &cec_msg_send, TRUE);
                }
                break;
            #endif

            default:
                break;
        }
    }
}

void cec_msg_process(void)
{
    pstCecDeclare_t             pCecPort, pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    pstCecPinFun_t              pstFun;
    pstCecFrameDetect_t         pstDetect;

    pCecPort = pCecPortOrg;
    while(pCecPort->pstCecGpioLoc != NULL)
    {
        #if ENABLE_CEC_USER2
        pstCecRevType_t l_pstCecRev = &g_stCecRevData[pCecPort-pCecPortOrg];
        #endif
        pstFun = &g_stCecPinFunTable[pCecPort-pCecPortOrg];
        //cec_frame_detect(pCecPort);
        if(pstFun->cecMsg_tx_flag
            #if ENABLE_CEC_USER2
            &&(l_pstCecRev->eCecSta == emCecSta_None)
            #endif
            )
        {
            cec_tx_process(pCecPort);
        }
        else
        {
            if(pCecPort->stCecCnntType.bEnReceive == TRUE)
            {
                #if ENABLE_CEC_USER2
                    cec_rx_process2(pCecPort, pCecPort-pCecPortOrg);
                #else
                    cec_frame_detect(pCecPort);
                    cec_rx_process(pCecPort);
                #endif
            }
        }

        if((!((pstFun->cecMsg_state>=cec_state_start)&&(pstFun->cecMsg_state<=cec_state_operand14)))
            #if ENABLE_CEC_USER2
            &&(l_pstCecRev->eCecSta == emCecSta_None)
            #endif
            )
        {
            pstDetect = &g_cec_frame_dct[pCecPort-pCecPortOrg];

            pstDetect->cec_pin_cur_state = ePinLevel_High;
            pstDetect->cec_pin_pre_state = ePinLevel_High;

        #if 0//(_EN_HDMI_OUTPUT_CEC_PIN_RECEIVE == _MACRO_ENABLE)
            if(pCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Dis) pstDetect->en_detect_mode = TRUE;
        #endif

        #if 0//(_EN_HDMI_INPUT_CEC_PIN_RECEIVE == _MACRO_ENABLE)
            if(pCecPort->stCecCnntType.eCecDirType == eCecSrcDisType_Src) pstDetect->en_detect_mode = TRUE;
        #endif
            if(pstFun->cecMsg_tx_flag)
            {
                pstFun->cecMsg_tx_flag = FALSE;
            }
            else
            {
                if(pCecPort->stCecCnntType.bEnReceive == TRUE)
                {
                    pstDetect->en_detect_mode = TRUE;
                }
            }

            if(pstFun->bPollingFlag)
            {
                pstFun->bPollingFlag = FALSE;
            }

            #if (!ENABLE_CEC_USER2)
            cec_msg_parse(pCecPort);
            #endif
        }

        pCecPort ++;
    }
    mapp_CecTimerProcess();
}

#if (defined(GD32F4XX))
//Time(us) = u16Period*u16Prescaler/HCLK(MHz)
//Freq(Khz) = (HCLK(Mhz)*1000)/(u16Period*u16Prescaler)
void TIMER6_Configuration(u16 arr, u16 psc)
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER6);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER6);

    timer_initpara.prescaler         = (psc - 1);
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (arr - 1);
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER6,&timer_initpara);

    timer_interrupt_enable(TIMER6,TIMER_INT_UP);

    nvic_irq_enable(TIMER6_IRQn,1,0);
    timer_enable(TIMER6);

}

void TIMER6_Configuration2(u16 u16Time) // u16Time uint 1us
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER6);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER6);

    timer_initpara.prescaler         = ((SystemCoreClock/1000000) - 1);
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (u16Time - 1);
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER6,&timer_initpara);

    timer_interrupt_enable(TIMER6,TIMER_INT_UP);

    nvic_irq_enable(TIMER6_IRQn,1,0);
    timer_enable(TIMER6);

}

/* Timer6 interrupt service routine */
void TIMER6_IRQHandler(void)
{
    if(timer_interrupt_flag_get(TIMER6,TIMER_INT_UP))
    {
        #if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
        cec_msg_process();
        #endif

        timer_interrupt_flag_clear(TIMER6,TIMER_INT_UP);
    }
}



#else
void TIMER6_Configuration(u16 arr, u16 psc)
{
#if (defined(N32G45X))
    TIM_TimeBaseInitType  TIM_TimeBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM6, ENABLE);
    TIM_DeInit(TIM6);
    TIM_TimeBaseStructure.Prescaler = (arr-1);
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period    = (psc-1);
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;
    TIM_TimeBaseStructure.RepetCnt  = 0;
    TIM_InitTimeBase(TIM6,&TIM_TimeBaseStructure);
    TIM_Enable(TIM6,ENABLE);
    TIM_ConfigInt(TIM6,TIM_INT_UPDATE,ENABLE);
#else
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //Enable TIM6 RCC.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE );

    TIM_DeInit(TIM6);
    /* TIM6 configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = (arr - 1);//Auto reload couter.
    TIM_TimeBaseStructure.TIM_Prescaler = (psc - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    /* Clear TIM6 update pending flag */
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
    /* Enable TIM6  Update interrupt*/
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    /* TIM6enable counter */
    TIM_Cmd(TIM6, ENABLE);
#endif

    #ifdef STM32F2XX_HD
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    #else
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    #endif //add by wf8421
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* Timer6 interrupt service routine */
#ifdef STM32F2XX_HD
void TIM6_DAC_IRQHandler(void)
#else
void TIM6_IRQHandler(void)
#endif //add by wf8421
{
//    pstCecDeclare_t             pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
//    static volatile uint8_t     svu8TimerIntCnt = 0;

#if (defined(N32G45X))
    if(TIM_GetIntStatus(TIM6,TIM_INT_UPDATE) != RESET)
#else
    if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
#endif
    {
        #if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
        cec_msg_process();
        #endif

#if (defined(N32G45X))
        TIM_ClrIntPendingBit(TIM6,TIM_INT_UPDATE);
#else
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
#endif
    }
}

#endif

#endif


#if (defined(GD32F4XX))
//Time(us) = u16Period*u16Prescaler/HCLK(MHz)
//Freq(Khz) = (HCLK(Mhz)*1000)/(u16Period*u16Prescaler)
void TIMER2_Configuration(u16 arr, u16 psc)
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER2);

    timer_initpara.prescaler         = (psc - 1);
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (arr - 1);
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

//    timer_auto_reload_shadow_enable(TIMER2);

    /* TIMER2 channel control update interrupt enable */
    timer_interrupt_enable(TIMER2,TIMER_INT_UP);

    nvic_irq_enable(TIMER2_IRQn,1,0);
    timer_enable(TIMER2);

}

void TIMER2_Configuration2(u16 u16Time) // u16Time uint 1us
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_deinit(TIMER2);

    timer_initpara.prescaler         = ((SystemCoreClock/1000000) - 1);
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (u16Time - 1);
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

//    timer_auto_reload_shadow_enable(TIMER2);

    /* TIMER2 channel control update interrupt enable */
    timer_interrupt_enable(TIMER2,TIMER_INT_UP);

    nvic_irq_enable(TIMER2_IRQn,1,0);
    timer_enable(TIMER2);

}

/* Timer2 interrupt service routine */
void TIMER2_IRQHandler(void)
{
//    pstCecDeclare_t             pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    static volatile uint8_t     svu8TimerIntCnt = 0;

    if(timer_interrupt_flag_get(TIMER2,TIMER_INT_UP) == SET)
    {

        #if (_EN_WATCH_DOG == _MACRO_ENABLE)
        WatchDogFeed(3);//1.5ms
        #endif

        // 1ms run
        svu8TimerIntCnt ++;
        #if (_EN_WATCH_DOG == _MACRO_ENABLE)// 0.5ms timeout
        {
            if(svu8TimerIntCnt >= 2)
            {
                Sys1MsClockRun();
                svu8TimerIntCnt = 0;
            }
        }
        #else // 1ms timeout
        {
            if(svu8TimerIntCnt >= 1)
            {
                Sys1MsClockRun();
                svu8TimerIntCnt = 0;
            }
        }
        #endif

        timer_interrupt_flag_clear(TIMER2,TIMER_INT_UP);
    }
}
#else

#if (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA1)
void TIMER3_Configuration(u16 arr, u16 psc)
{
#if (defined(N32G45X))
    TIM_TimeBaseInitType  TIM_TimeBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3, ENABLE);
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.Prescaler = (arr-1);
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period    = (psc-1);
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;
    TIM_TimeBaseStructure.RepetCnt  = 0;
    TIM_InitTimeBase(TIM3,&TIM_TimeBaseStructure);
    TIM_Enable(TIM3,ENABLE);
    TIM_ConfigInt(TIM3,TIM_INT_UPDATE,ENABLE);
#else
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //Enable TIM2 RCC.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE );

    TIM_DeInit(TIM3);
    /* TIM2 configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = (arr - 1);//Auto reload couter.
    TIM_TimeBaseStructure.TIM_Prescaler = (psc - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Clear TIM2 update pending flag */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    /* Enable TIM2  Update interrupt*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    /* TIM2enable counter */
    TIM_Cmd(TIM3, ENABLE);
#endif

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* Timer2 interrupt service routine */
void TIM3_IRQHandler(void)
{
//    pstCecDeclare_t             pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    static volatile uint8_t     svu8TimerIntCnt = 0;

#if (defined(N32G45X))
    if(TIM_GetIntStatus(TIM3,TIM_INT_UPDATE) != RESET)
#else
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
#endif
    {
        #if 0//(_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
        cec_msg_process();
        #endif

        #if (_EN_WATCH_DOG == _MACRO_ENABLE)
        #if 0//(_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
        if(pCecPortOrg->pstCecGpioLoc != NULL)  WatchDogFeed(15);//1.5ms
        else WatchDogFeed(3);//1.5ms
        #else
        WatchDogFeed(3);//1.5ms
        #endif //add by wf8421
        #endif

        // 1ms run
        svu8TimerIntCnt ++;
#if 0
        if(pCecPortOrg->pstCecGpioLoc != NULL)// 100us timeout
        {
            if(svu8TimerIntCnt >= 10)
            {
                Sys1MsClockRun();
                svu8TimerIntCnt = 0;
            }
        }
        else
#endif //add by wf8421
        #if (_EN_WATCH_DOG == _MACRO_ENABLE)// 0.5ms timeout
        {
            if(svu8TimerIntCnt >= 2)
            {
                Sys1MsClockRun();
                svu8TimerIntCnt = 0;
            }
        }
        #else // 1ms timeout
        {
            if(svu8TimerIntCnt >= 1)
            {
                Sys1MsClockRun();
                svu8TimerIntCnt = 0;
            }
        }
        #endif

#if (defined(N32G45X))
        TIM_ClrIntPendingBit(TIM3,TIM_INT_UPDATE);
#else
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
#endif
    }
}

#else
void TIMER2_Configuration(u16 arr, u16 psc)
{
#if (defined(N32G45X))
    TIM_TimeBaseInitType  TIM_TimeBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.Prescaler = (arr-1);
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period    = (psc-1);
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;
    TIM_TimeBaseStructure.RepetCnt  = 0;
    TIM_InitTimeBase(TIM2,&TIM_TimeBaseStructure);
    TIM_Enable(TIM2,ENABLE);
    TIM_ConfigInt(TIM2,TIM_INT_UPDATE,ENABLE);
#else
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //Enable TIM2 RCC.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE );

    TIM_DeInit(TIM2);
    /* TIM2 configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = (arr - 1);//Auto reload couter.
    TIM_TimeBaseStructure.TIM_Prescaler = (psc - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM2 update pending flag */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    /* Enable TIM2  Update interrupt*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    /* TIM2enable counter */
    TIM_Cmd(TIM2, ENABLE);
#endif

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* Timer2 interrupt service routine */
void TIM2_IRQHandler(void)
{
//    pstCecDeclare_t             pCecPortOrg = (pstCecDeclare_t)&stProjectCecWorkTable[0];
    static volatile uint8_t     svu8TimerIntCnt = 0;

#if (defined(N32G45X))
    if(TIM_GetIntStatus(TIM2,TIM_INT_UPDATE) != RESET)
#else
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
#endif
    {
        #if 0//(_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
        cec_msg_process();
        #endif

        #if (_EN_WATCH_DOG == _MACRO_ENABLE)
        #if 0//(_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
        if(pCecPortOrg->pstCecGpioLoc != NULL)  WatchDogFeed(15);//1.5ms
        else WatchDogFeed(3);//1.5ms
        #else
        WatchDogFeed(3);//1.5ms
        #endif //add by wf8421
        #endif

        // 1ms run
        svu8TimerIntCnt ++;
#if 0
        if(pCecPortOrg->pstCecGpioLoc != NULL)// 100us timeout
        {
            if(svu8TimerIntCnt >= 10)
            {
                Sys1MsClockRun();
                svu8TimerIntCnt = 0;
            }
        }
        else
#endif //add by wf8421
        #if (_EN_WATCH_DOG == _MACRO_ENABLE)// 0.5ms timeout
        {
            if(svu8TimerIntCnt >= 2)
            {
                Sys1MsClockRun();
                svu8TimerIntCnt = 0;
            }
        }
        #else // 1ms timeout
        {
            if(svu8TimerIntCnt >= 1)
            {
                Sys1MsClockRun();
                svu8TimerIntCnt = 0;
            }
        }
        #endif

#if (defined(N32G45X))
        TIM_ClrIntPendingBit(TIM2,TIM_INT_UPDATE);
#else
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
#endif
    }
}
#endif
#endif

funcPrj1MsLoop pFuncPrj1MsLoopHook = NULL;

void Sys1MsClockRun(void)
{
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    //uart timeout process
    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        if(pstUart->funUartProTimeOut != NULL)
        {
            pstUart->funUartProTimeOut();
        }
        pstUart ++;
    }

    #if (_PRJ_TYPE_SWITCH_SPLITER != _PRJ_SPLITER_ONLY)
    #if Project_SCU61E_66341_AUTO_SWITCH
    AutoSwDetectTime1MsLoop();
    #endif
    #if (IT663XX_VER != IT663XX_NONE)
    AudioProDlyTickLoop();
    #endif
    #endif

#if (_ENABLE_PANASONIC_CHIP_MN86478X == _MACRO_ENABLE)
    HAL_IncTick();
#endif
    PlatformTimerTask();
#if _ENABLE_EXTERNAL_CHIP_TASK
    mapp_ExternalChip1MsTimer();
#endif

#if _ENABLE_CHIP_MS1826
//    app_MS18261MsTimerTask();
#endif

    if(pFuncPrj1MsLoopHook != NULL) pFuncPrj1MsLoopHook();

#if _ENABLE_MAIN_TASK_ETHERNET
    ethnet_time_update();
#endif
}

void drv_TimerFuncInit(void)
{
#if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
    pstCecDeclare_t     pCecPort = (pstCecDeclare_t)&stProjectCecWorkTable[0];
#endif

#if 0
    if(pCecPort->pstCecGpioLoc == NULL)
    {
    #if (_EN_WATCH_DOG == _MACRO_ENABLE)
    #ifdef STM32F2XX_HD
        TIMER2_Configuration(1000, 30); // 0.5ms
    #else
        TIMER2_Configuration(1000, 36); // 0.5ms
    #endif
        IWDG_Configuration();
    #else
        TIMER2_Configuration(2000, 36);// 1ms
    #endif
    }
    else
    {
    #ifdef STM32F2XX_HD
        TIMER2_Configuration(200, 30);// 100us
    #else
        TIMER2_Configuration(200, 36);// 100us
    #endif

    #if (_EN_WATCH_DOG == _MACRO_ENABLE)
        IWDG_Configuration();
    #endif
    }
#endif //add by wf8421

#if (_ENABLE_CEC_FUNCTION_USE == _MACRO_ENABLE)
    if(pCecPort->pstCecGpioLoc != NULL)
    {
        #ifdef STM32F2XX_HD
        TIMER6_Configuration(200, 30);// 100us
        #elif defined(GD32F4XX)
        //TIMER6_Configuration(400, 42);// 100us
        TIMER6_Configuration2(100);
        #else
        TIMER6_Configuration(200, 36);// 100us
        #endif
    }
#endif //add by wf8421
#if (_EN_WATCH_DOG == _MACRO_ENABLE)
    #ifdef STM32F2XX_HD
    TIMER2_Configuration(1000, 30); // 0.5ms
    #else
    TIMER2_Configuration(1000, 36); // 0.5ms
    #endif
    IWDG_Configuration();
#else
    #ifdef STM32F2XX_HD
    TIMER2_Configuration(2000, 30); // 1ms
    #elif defined(GD32F4XX)
//    TIMER2_Configuration(4000, 42); // 1ms
    TIMER2_Configuration2(1000);
    #elif (_IR_SENSOR_IN_PORT_USE ==_IR_SENSOR_IN_GPIOA1)
    TIMER3_Configuration(2000, 36); // 1ms
    #else
    TIMER2_Configuration(2000, 36); // 1ms
    #endif
#endif

}



