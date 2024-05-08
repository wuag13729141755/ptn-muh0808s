
#include "includes.h"

#ifndef IDT_DEBUG
#define IDT_DEBUG           1
#endif

#if IDT_DEBUG
#define IDT_DBG_Print(x)       dbg_printf("[IDT5V]");dbg_printf x
#else
#define IDT_DBG_Print(x)       //dbg_printf
#endif

#ifndef INPUT_CLOCK
#define INPUT_CLOCK   14850
#endif

vvFuncPoint pvvInitIDTFunc = NULL;

stIDTConfig_t g_stIDTConfigVal[DEV_IDT5V_NUM];

u8 g_u8IDT5VDeviceAddrTable[] =
{
    _I2C_TABLE_INDEX_IDT5V49_0,
#if (DEV_IDT5V_NUM>=2)
    _I2C_TABLE_INDEX_IDT5V49_1,
#endif
};

void IDT_delay_nms(u16 u16nms)
{
    delay_ms(u16nms);
}

void i2c_IDT5V_write_one_byte(u8 u8DevId,u8 u8Addr, u8 u8Data)
{
    u8 l_bflag;
    u16 l_u16RealAddr = ((0xFc<<8)|u8Addr);

#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_IIC[g_u8IDT5VDeviceAddrTable[u8DevId]], portMAX_DELAY);
#endif
    l_bflag = i2c_write_bytes(g_u8IDT5VDeviceAddrTable[u8DevId], IDT_SlaveAddr,l_u16RealAddr, &u8Data, 1, FALSE);
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_IIC[g_u8IDT5VDeviceAddrTable[u8DevId]]);
#endif

    if(l_bflag != 0)
    {
        IDT_DBG_Print(("=====IDT[%d] I2C ERROR Write Reg0x%X=%X =====\r\n",u8DevId,(int)u8Addr,(int)u8Data));
    }
}

u8 i2c_IDT5V_read_one_byte(u8 u8DevId,u8 u8Addr)
{
    u8 l_u8Buffer[2] = {0};
    u8 l_bflag;
    u16 l_u16RealAddr = ((0xFc<<8)|u8Addr);

#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_IIC[g_u8IDT5VDeviceAddrTable[u8DevId]], portMAX_DELAY);
#endif
    l_bflag = i2c_read_bytes(g_u8IDT5VDeviceAddrTable[u8DevId], IDT_SlaveAddr,l_u16RealAddr, l_u8Buffer, 2, FALSE);
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_IIC[g_u8IDT5VDeviceAddrTable[u8DevId]]);
#endif

    if(l_bflag != 0)
    {
        IDT_DBG_Print(("=====IDT[%d] I2C ERROR Read Reg0x%X =====\r\n",u8DevId,(int)u8Addr));
    }

    return l_u8Buffer[1];
}

#if _ENABLE_CHIP_IDT5V49EE902
void IDT_5V_init_default(void)
{
	uint8_t tmp, timeout;
    u16 l_u16Mx0 = 0;
    u8 l_u8Dx0 = 0;
    u16 l_u16Mx1 = 0;
    u8 l_u8Dx1 = 0;
    u16 l_u16Mx2 = 0;
    u8 l_u8Dx2 = 0;
    u16 l_u16Mx3 = 0;
    u8 l_u8Dx3 = 0;
    u8 i;

    for(i=0;i<DEV_IDT5V_NUM;i++)
    {
//        g_u8IDT5v49ee902_BusID = _I2C_TABLE_INDEX_IDT5V49_0;
        memset((u8*)&g_stIDTConfigVal[i],0,sizeof(stIDTConfig_t));

    	timeout = 100;
    	do {
    		IDT_delay_nms(5);
    		i2c_IDT5V_write_one_byte(i,0x00,0x01);	//mode: Software Mode control
    		tmp = i2c_IDT5V_read_one_byte(i,0x00);
    	} while((tmp != 0x01) && --timeout);

    	if(tmp != 0x01)
    	{
            IDT_DBG_Print(("IDT_5V[%d]_init fail \r\n",i));
    		return;
    	}

#if 0//IDT5V_PCLK_LVTTL	//PCLK: TTL

    	i2c_IDT5V_write_one_byte(i,0x01,0x00);	//Select CONFIG0
    	i2c_IDT5V_write_one_byte(i,0x02,0x00);	//outputs enabled
    	//i2c_IDT5V_write_one_byte(i,0x03,0xBB);	//output not suspend (OUT2&6 suspend)
        i2c_IDT5V_write_one_byte(i,0x03,0xFF);    //output not suspend

    	/*************************************************************
    	* PCLK: OUT1, OUT3; OUT0: 100MHz; DDR(100MHz): OUT4 & OUT4b, OUT5 & OUT5b;
    	* not used: OUT2, OUT6
    	*
    	* OUT0(TTL): SRC0: DIV3<-SRC3: PLL3
    	* OUT1(TTL): DIV1<-SRC1: PLL2
    	* OUT2(TTL): DIV2<-SRC2: PLL2
    	* OUT3 and OUT6(TTL): DIV6<-SRC6: PLL1
    	* OUT4 and OUT4b(LVDS): DIV4<-SRC4: PLL0
    	* OUT5 and OUT5b(LVDS): DIV5<-SRC5: PLL0
    	* Fout = (Fin * (M/D))/(ODIV)  (D = 1~127, M = 2~4095)
        * PLL0 : M = 2 * N + A + 1 (for A>0)
        *        M = 2 * N  (for A=0)
        * For PLL1, PLL2 and PLL3 : M = N
        * ODIV: PM=1 ,Q=b'1111111 ODIV=1
        *       PM=1 ,Q!=b'1111111 ODIV=(Q+2)*2
    	**************************************************************/

    	i2c_IDT5V_write_one_byte(i,0xC0,0xD6);	//crystal/REFIN, auto switch mode, PLL3 normal, SRC0: DIV3, SRC1: PLL3

    	//PLL0: 100M
    	i2c_IDT5V_write_one_byte(i,0x10,0x01);	//D=1
    	i2c_IDT5V_write_one_byte(i,0x18,0x02);	//N=2
    	i2c_IDT5V_write_one_byte(i,0x1C,0x00);	//A=0, M=2*N=4

    	//PLL1: 148.5M
    	i2c_IDT5V_write_one_byte(i,0x28,0x32);	//D=3
    	i2c_IDT5V_write_one_byte(i,0x30,0x29);	//N=12
    	i2c_IDT5V_write_one_byte(i,0x34,0x01);    //N3[11:8]<<4 | N1[11:8]

    	//PLL2: 148.5M
    	i2c_IDT5V_write_one_byte(i,0x3C,0x10);    //Loop Parameter    // 0x10
    	i2c_IDT5V_write_one_byte(i,0x40,0x32);	//D=50
    	i2c_IDT5V_write_one_byte(i,0x48,0x29);	//N=297
    	i2c_IDT5V_write_one_byte(i,0x4C,0x81);	//Spread Spectrum Enable    // 80

    	//PLL3: 12.288M
    	i2c_IDT5V_write_one_byte(i,0x5C,0x3B);    //D=59
    	i2c_IDT5V_write_one_byte(i,0x64,0x1D);    //N=29

    	i2c_IDT5V_write_one_byte(i,0xC4,0xED);	//SRC3: PLL2, SRC2: PLL2
    	i2c_IDT5V_write_one_byte(i,0xCC,0xB2);	//SRC6: PLL1, SRC5: PLL0, SRC4: PLL1

    	i2c_IDT5V_write_one_byte(i,0x75,0x02);	//S3=0 - Both from DIV6, S1=1 - OUT1/OUT2 are from DIV1/DIV2 respectively
    	i2c_IDT5V_write_one_byte(i,0x7A,0x05);	//OUT5 and OUT5b: invert clock, LVDS
    	i2c_IDT5V_write_one_byte(i,0x79,0x05);	//OUT4 and OUT4b: invert clock, LVDS
    	//i2c_IDT5V_write_one_byte(i,0x79,0x00);	//OUT4 and OUT4b: normal, LVTTL
    	i2c_IDT5V_write_one_byte(i,0x78,0x00);	//OUT3 and OUT6: normal, LVTTL
    	i2c_IDT5V_write_one_byte(i,0x76,0x00);	//OUT1 and OUT2: normal, LVTTL

    	//DIV1~6: /1
    	i2c_IDT5V_write_one_byte(i,0x88,0xFF);
    	i2c_IDT5V_write_one_byte(i,0x90,0xFF);
    	i2c_IDT5V_write_one_byte(i,0x94,0xFF);
    	i2c_IDT5V_write_one_byte(i,0x9C,0xFF);
    	i2c_IDT5V_write_one_byte(i,0xA0,0xFF);
    	i2c_IDT5V_write_one_byte(i,0xA8,0xFF);

#else	//PCLK: LVDS

    	i2c_IDT5V_write_one_byte(i,0x01,0x00);	//Select CONFIG0
    	i2c_IDT5V_write_one_byte(i,0x02,0x00);	//outputs enabled
    	//i2c_IDT5V_write_one_byte(i,0x03,0xFD);	//output not suspend (OUT1 suspend)
        i2c_IDT5V_write_one_byte(i,0x03,0xFF);    //output not suspend

    	/*************************************************************
    	* PCLK: OUT1, OUT3; OUT0: 100MHz; DDR(100MHz): OUT4 & OUT4b, OUT5 & OUT5b;
    	* not used: OUT2, OUT6
    	*
    	* OUT0(TTL): SRC0: DIV3<-SRC3: PLL3
    	* OUT1(TTL): DIV1<-SRC1: PLL2
    	* OUT2(TTL): DIV2<-SRC2: PLL2
    	* OUT3 and OUT6(TTL): DIV6<-SRC6: PLL1
    	* OUT4 and OUT4b(LVDS): DIV4<-SRC4: PLL0
    	* OUT5 and OUT5b(LVDS): DIV5<-SRC5: PLL0
    	* Fout = (Fin * (M/D))/(ODIV)  (D = 1~127, M = 2~4095)
        * PLL0 : M = 2 * N + A + 1 (for A>0)
        *        M = 2 * N  (for A=0)
        * For PLL1, PLL2 and PLL3 : M = N
        * ODIV: PM=1 ,Q=b'1111111 ODIV=1
        *       PM=1 ,Q!=b'1111111 ODIV=(Q+2)*2
    	**************************************************************/

    	i2c_IDT5V_write_one_byte(i,0xC0,0xD6);	//crystal/REFIN, auto switch mode, PLL3 normal, SRC0: DIV3, SRC1: PLL3
        //i2c_IDT5V_write_one_byte(i,0xC0,0x56);    //crystal/REFIN, auto switch mode, PLL3 normal, SRC0: DIV3, SRC1: PLL1

#if 1
        //PLL0: 100M
        IDT_SetClockCoef(INPUT_CLOCK,10000,&l_u16Mx0,&l_u8Dx0);
        if(l_u16Mx0!=0&&(l_u8Dx0!=0&&l_u8Dx0<127))
        {
            i2c_IDT5V_write_one_byte(i,0x10,(l_u8Dx0<<1));    //D=98
            i2c_IDT5V_write_one_byte(i,0x18,((l_u16Mx0)&0xFF));    //N=33
            i2c_IDT5V_write_one_byte(i,0x1C,(0<<4)|(((l_u16Mx0)>>8)&0x0F));    //A=0, M=2*N
        }

        //PLL1: 148.5M
        IDT_SetClockCoef(INPUT_CLOCK,14850,&l_u16Mx1,&l_u8Dx1);
        if(l_u16Mx1!=0&&(l_u8Dx1!=0&&l_u8Dx1<127))
        {
            i2c_IDT5V_write_one_byte(i,0x28,l_u8Dx1);    //D=100
            i2c_IDT5V_write_one_byte(i,0x30,((l_u16Mx1)&0xFF));    //N=297
            i2c_IDT5V_write_one_byte(i,0x34,(((l_u16Mx1)>>8)&0x0F));    //N3[11:8]<<4 | N1[11:8]
        }

        //PLL2: 148.5M
        IDT_SetClockCoef(INPUT_CLOCK,14850,&l_u16Mx2,&l_u8Dx2);
        if(l_u16Mx2!=0&&(l_u8Dx2!=0&&l_u8Dx2<127))
        {
            i2c_IDT5V_write_one_byte(i,0x3C,0x10);    //Loop Parameter    // 0x10
            i2c_IDT5V_write_one_byte(i,0x40,l_u8Dx2);    //D=100
            i2c_IDT5V_write_one_byte(i,0x48,((l_u16Mx2)&0xFF));    //N=297
            i2c_IDT5V_write_one_byte(i,0x4C,(0x80)|(((l_u16Mx2)>>8)&0x0F));    //Spread Spectrum Enable    // 80
        }

        //PLL3: 12.288M *5 = 61.44M
        IDT_SetClockCoef(INPUT_CLOCK,6144,&l_u16Mx3,&l_u8Dx3);
        if(l_u16Mx3!=0&&(l_u8Dx3!=0&&l_u8Dx3<127))
        {
            u8 l_u8Reg34 = i2c_IDT5V_read_one_byte(i,0x34);
            i2c_IDT5V_write_one_byte(i,0x5C,l_u8Dx3);    //D=29
            i2c_IDT5V_write_one_byte(i,0x64,((l_u16Mx3)&0xFF));    //N=12
            i2c_IDT5V_write_one_byte(i,0x34,((((l_u16Mx3)>>8)&0x0F)<<4)|(l_u8Reg34&0x0F));    //N3[11:8]<<4 | N1[11:8]
        }
#else
    	//PLL0: 100M
    	i2c_IDT5V_write_one_byte(i,0x10,0x62);	//D=98
    	i2c_IDT5V_write_one_byte(i,0x18,0x21);	//N=33
    	i2c_IDT5V_write_one_byte(i,0x1C,0x00);	//A=0, M=2*N

    	//PLL1: 148.5M
    	i2c_IDT5V_write_one_byte(i,0x28,0x0C);	//D=100
    	i2c_IDT5V_write_one_byte(i,0x30,0x0C);	//N=297
    	i2c_IDT5V_write_one_byte(i,0x34,0x00);    //N3[11:8]<<4 | N1[11:8]

    	//PLL2: 148.5M
    	i2c_IDT5V_write_one_byte(i,0x3C,0x10);    //Loop Parameter    // 0x10
    	i2c_IDT5V_write_one_byte(i,0x40,0x0C);	//D=100
    	i2c_IDT5V_write_one_byte(i,0x48,0x0C);	//N=297
    	i2c_IDT5V_write_one_byte(i,0x4C,0x80);	//Spread Spectrum Enable    // 80

    	//PLL3: 12.288M *5 = 61.44M
    	i2c_IDT5V_write_one_byte(i,0x5C,0x1D);    //D=29
    	i2c_IDT5V_write_one_byte(i,0x64,0x0C);    //N=12
#endif

    	i2c_IDT5V_write_one_byte(i,0xC4,0xED);	//SRC3: PLL2, SRC2: PLL2
    	i2c_IDT5V_write_one_byte(i,0xCC,0xB2);	//SRC6: PLL1, SRC5: PLL0, SRC4: PLL1

    	i2c_IDT5V_write_one_byte(i,0x75,0x02);	//S3=0 - Both from DIV6, S1=1 - OUT1/OUT2 are from DIV1/DIV2 respectively
    	i2c_IDT5V_write_one_byte(i,0x7A,0x05);	//OUT5 and OUT5b: invert clock, LVDS
    	//i2c_IDT5V_write_one_byte(i,0x79,0x05);	//OUT4 and OUT4b: invert clock, LVDS
    	i2c_IDT5V_write_one_byte(i,0x79,0x00);	//OUT4 and OUT4b: normal, LVTTL
    	i2c_IDT5V_write_one_byte(i,0x78,0x00);	//OUT3 and OUT6: normal, LVTTL
    	i2c_IDT5V_write_one_byte(i,0x76,0x00);	//OUT1 and OUT2: normal, LVTTL

    	//DIV1~6: /1
    	i2c_IDT5V_write_one_byte(i,0x88,0xFF);
    	i2c_IDT5V_write_one_byte(i,0x90,0xFF);
    	i2c_IDT5V_write_one_byte(i,0x94,0xFF);
    	i2c_IDT5V_write_one_byte(i,0x9C,0xFF);
    	i2c_IDT5V_write_one_byte(i,0xA0,0xFF);
    	i2c_IDT5V_write_one_byte(i,0xA8,0xFF);
#endif
    }
}

void IDT_5V_Out4ChangeClock(u8 u8DevId,u8 u8ClkID)
{
    switch(u8ClkID)
    {
        case emClk_148_5m:
            {
            	//PLL1: 148.5M
            	i2c_IDT5V_write_one_byte(u8DevId,0x28,0x32);	//D=50
            	i2c_IDT5V_write_one_byte(u8DevId,0x30,0x29);	//N=297
            	i2c_IDT5V_write_one_byte(u8DevId,0x34,0x01);    //N3[11:8]<<4 | N1[11:8]
            }
            break;
        case emClk_297m:         // 3840x2160_30
            {
            	//PLL1: 297m
            	i2c_IDT5V_write_one_byte(u8DevId,0x28,0x19);	//D=25
            	i2c_IDT5V_write_one_byte(u8DevId,0x30,0x29);	//N=297
            	i2c_IDT5V_write_one_byte(u8DevId,0x34,0x01);    //N3[11:8]<<4 | N1[11:8]
            }
            break;
        case emClk_74_25m:       // 1280x720_60
            {
            	//PLL1: 74.25m
            	i2c_IDT5V_write_one_byte(u8DevId,0x28,0x64);	//D=100
            	i2c_IDT5V_write_one_byte(u8DevId,0x30,0x29);	//N=297
            	i2c_IDT5V_write_one_byte(u8DevId,0x34,0x01);    //N3[11:8]<<4 | N1[11:8]
            }
            break;
        case emClk_154m:         // 1920x1200_60
            {
            	//PLL1: 154m
            	i2c_IDT5V_write_one_byte(u8DevId,0x28,0x19);	//D=25
            	i2c_IDT5V_write_one_byte(u8DevId,0x30,0x9A);	//N=154
            	i2c_IDT5V_write_one_byte(u8DevId,0x34,0x00);    //N3[11:8]<<4 | N1[11:8]
            }
            break;
        case emClk_65m:          // 1024x768_60
            {
            	//PLL1: 65m
            	i2c_IDT5V_write_one_byte(u8DevId,0x28,0x19);	//D=25
            	i2c_IDT5V_write_one_byte(u8DevId,0x30,0x41);	//N=65
            	i2c_IDT5V_write_one_byte(u8DevId,0x34,0x00);    //N3[11:8]<<4 | N1[11:8]
            }
            break;
        case emClk_85_48m:
            {
            	//PLL1: 85.5m
            	i2c_IDT5V_write_one_byte(u8DevId,0x28,0x32);	//D=50
            	i2c_IDT5V_write_one_byte(u8DevId,0x30,0xAB);	//N=171
            	i2c_IDT5V_write_one_byte(u8DevId,0x34,0x00);    //N3[11:8]<<4 | N1[11:8]
            }
            break;
        case emClk_165m:
            {
            	//PLL1: 165m
            	i2c_IDT5V_write_one_byte(u8DevId,0x28,0x19);	//D=25
            	i2c_IDT5V_write_one_byte(u8DevId,0x30,0xA5);	//N=165
            	i2c_IDT5V_write_one_byte(u8DevId,0x34,0x00);    //N3[11:8]<<4 | N1[11:8]
            }
            break;
        default:
            break;
    }
}

// u16InClk * (pu16Mx/pu8Dx) = u16OutClk
void IDT_SetClockCoef(u16 u16InClk, u16 u16OutClk, u16 *pu16Mx, u8 *pu8Dx)
{
    u8 i;
    u16 l_u16TempM = 12;
    u16 l_u16Coef = 0;
    u16 l_u16TempClk = 0;
    u8 l_u8CoefNum = 1;
    u8 l_u8fsr = 2;
    u16 l_u16RealOutClk = u16OutClk*l_u8fsr;

    for(i=3;i<=127;i++)
    {
        l_u16TempM = ((u32)(((l_u16RealOutClk*10000)/u16InClk)*i+5000)/10000);
        if(l_u16TempM<12)
            continue;
        l_u16TempClk = ((u32)(l_u16TempM*u16InClk/i));
        l_u16Coef = ((u32)(abs(l_u16TempClk-l_u16RealOutClk)*1000)/l_u16RealOutClk);
        if(l_u16Coef<l_u8CoefNum)
        {
            *pu16Mx = l_u16TempM;
            *pu8Dx = i;
            IDT_DBG_Print(("l_u16Coef == %d\r\n",l_u16Coef));
            break;
        }
        else if(i>=127)
        {
            i=3;
            l_u8CoefNum += 1;
            IDT_DBG_Print(("l_u8CoefNum == %d\r\n",l_u8CoefNum));
        }
        else if(l_u8CoefNum>15)
        {
            break;
        }
    }
    IDT_DBG_Print(("Inclk = %d  OutClk = %d l_u8Dx = 0x%x ; l_u16Mx = 0x%x \r\n",u16InClk,u16OutClk,*pu8Dx,*pu16Mx));
}

void IDT_5V_Out4ProgramClock(u8 u8DevId,u16 u16Clk)    // 10k clock
{
    u16 l_u16Mx = 0;
    u8 l_u8Dx = 0;
    u8 l_u8Reg34 = 0;
    l_u8Reg34=i2c_IDT5V_read_one_byte(u8DevId,0x34);
    IDT_SetClockCoef(INPUT_CLOCK,u16Clk,&l_u16Mx,&l_u8Dx);
    IDT_DBG_Print(("Device[%d] Inclk = %d  OutClk = %d l_u8Dx = 0x%x ; l_u16Mx = 0x%x \r\n",u8DevId,INPUT_CLOCK,u16Clk,l_u8Dx,l_u16Mx));
    if(l_u16Mx==0||l_u8Dx==0||l_u8Dx>127)
        return;
    i2c_IDT5V_write_one_byte(u8DevId,0x28,((l_u8Dx)&0x7F));            //D=50
    i2c_IDT5V_write_one_byte(u8DevId,0x30,((l_u16Mx)&0xFF));    //N=16Clk%100
    i2c_IDT5V_write_one_byte(u8DevId,0x34,(l_u8Reg34&0xF0)|((l_u16Mx>>8)&0x0F));    //N3[11:8]<<4 | N1[11:8]
}

// Change to PLL number clock
void IDT_5V_OutProgramClock(u8 u8DevId,u8 u8Ch,u16 u16Clk)    // unit: 10k clock
{
    u16 l_u16Mx = 0;
    u8 l_u8Dx = 0;

    if(u8Ch == 0)
    {
        u8 l_u8Reg1C = 0;
        l_u8Reg1C=i2c_IDT5V_read_one_byte(u8DevId,0x34);
        IDT_SetClockCoef(INPUT_CLOCK,u16Clk/2,&l_u16Mx,&l_u8Dx);    // M = 2*N
        IDT_DBG_Print(("0 Device[%d] Inclk = %d  OutClk = %d l_u8Dx = 0x%x ; l_u16Mx = 0x%x \r\n",u8DevId,INPUT_CLOCK,u16Clk,l_u8Dx,l_u16Mx));
        if(l_u16Mx==0||l_u8Dx==0||l_u8Dx>127)
            return;
        i2c_IDT5V_write_one_byte(u8DevId,0x10,((l_u8Dx)&0x7F));    //D=3
        i2c_IDT5V_write_one_byte(u8DevId,0x18,((l_u16Mx)&0xFF));    //N=11
        i2c_IDT5V_write_one_byte(u8DevId,0x1C,(l_u8Reg1C&0xF0)|((l_u16Mx>>8)&0x0F));    //A=0, M=2*N=22 >10~8026
    }
    else if(u8Ch == 1)
    {
        u8 l_u8Reg34 = 0;
        l_u8Reg34=i2c_IDT5V_read_one_byte(u8DevId,0x34);
        IDT_SetClockCoef(INPUT_CLOCK,u16Clk,&l_u16Mx,&l_u8Dx);
        IDT_DBG_Print(("1 Device[%d] Inclk = %d  OutClk = %d l_u8Dx = 0x%x ; l_u16Mx = 0x%x \r\n",u8DevId,INPUT_CLOCK,u16Clk,l_u8Dx,l_u16Mx));
        if(l_u16Mx==0||l_u8Dx==0||l_u8Dx>127)
            return;
        i2c_IDT5V_write_one_byte(u8DevId,0x28,((l_u8Dx)&0x7F));            //D=50
        i2c_IDT5V_write_one_byte(u8DevId,0x30,((l_u16Mx)&0xFF));    //N=16Clk%100
        i2c_IDT5V_write_one_byte(u8DevId,0x34,(l_u8Reg34&0xF0)|((l_u16Mx>>8)&0x0F));    //N3[11:8]<<4 | N1[11:8]
    }
    else if(u8Ch == 2)
    {
        u8 l_u8Reg4C = 0;
        l_u8Reg4C=i2c_IDT5V_read_one_byte(u8DevId,0x4C);
        IDT_SetClockCoef(INPUT_CLOCK,u16Clk,&l_u16Mx,&l_u8Dx);
        IDT_DBG_Print(("2 Device[%d] Inclk = %d  OutClk = %d l_u8Dx = 0x%x ; l_u16Mx = 0x%x \r\n",u8DevId,INPUT_CLOCK,u16Clk,l_u8Dx,l_u16Mx));
        if(l_u16Mx==0||l_u8Dx==0||l_u8Dx>127)
            return;
        i2c_IDT5V_write_one_byte(u8DevId,0x3C,0x10); //Loop Parameter
        i2c_IDT5V_write_one_byte(u8DevId,0x40,((l_u8Dx)&0x7F));
        i2c_IDT5V_write_one_byte(u8DevId,0x48,((l_u16Mx)&0xFF));
        i2c_IDT5V_write_one_byte(u8DevId,0x4C,(l_u8Reg4C&0xF0)|((l_u16Mx>>8)&0x0F));    //Spread Spectrum Enable
    }
    else if(u8Ch == 3)
    {
        u8 l_u8Reg34 = 0;
        l_u8Reg34=i2c_IDT5V_read_one_byte(u8DevId,0x34);
        IDT_SetClockCoef(INPUT_CLOCK,u16Clk,&l_u16Mx,&l_u8Dx);
        IDT_DBG_Print(("3 Device[%d] Inclk = %d  OutClk = %d l_u8Dx = 0x%x ; l_u16Mx = 0x%x \r\n",u8DevId,INPUT_CLOCK,u16Clk,l_u8Dx,l_u16Mx));
        if(l_u16Mx==0||l_u8Dx==0||l_u8Dx>127)
            return;
        i2c_IDT5V_write_one_byte(u8DevId,0x5C,((l_u8Dx)&0x7F));
        i2c_IDT5V_write_one_byte(u8DevId,0x64,((l_u16Mx)&0xFF));
        i2c_IDT5V_write_one_byte(u8DevId,0x34,((((l_u16Mx)>>8)&0x0F)<<4)|(l_u8Reg34&0x0F));    //N3[11:8]<<4 | N1[11:8]
    }
}
#endif

//====================================================================================
#if _ENABLE_CHIP_IDT5P49V6965
#define FRAC_Coeff   		    0x1000000
#define DEFAULT_VCO_Clk   		(INPUT_CLOCK*1000000)

unsigned char  IDT_REG_Start00_25M_In[0x10]=   //CVO_CLK=2673M
{
    0xA8,
    0xCC,
    0x81,
    0x80,
    0x00,
    0x03,
    0x8C,
    0x03,
    0x20,
    0x00,
    0x00,
    0x00,
    0x9F,
    0xFF,
    0xF0,
    0x80,
};

unsigned char  IDT_REG_Start20_Fout1[0x10]=
{
    0x00,
    0x81,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x04,
    0x00,
    0x00,
    0x00,
    0xC0,
    0x20,
};

unsigned char  IDT_REG_Start30_Fout2[0x10]=
{
    0x00,
    0x81,
    0x01,
    0xAB,
    0x87,
    0x50,
    0x00,
    0x00,
    0x00,
    0x00,
    0x04,
    0x00,
    0x00,
    0x00,
    0x80,
    0x1B,
};
unsigned char  IDT_REG_Start40_Fout3[0x10]=
{
    0x00,
    0x81,
    0x01,
    0xAB,
    0x87,
    0x50,
    0x00,
    0x00,
    0x00,
    0x00,
    0x04,
    0x00,
    0x00,
    0x00,
    0x80,
    0x1B,
};

unsigned char  IDT_REG_Start50_Fout4[0x10]=
{
    0x00,
    0x81,
    0x01,
    0xAB,
    0x87,
    0x50,
    0x00,
    0x00,
    0x00,
    0x00,
    0x04,
    0x00,
    0x00,
    0x00,
    0x80,
    0x1B,
};

/*
Reg:0x60
Bits     Default  Value         Name        Function
D7~D5       101             CLK1_cfg[2:0]   These bits give us the output type configuration mode.
                                            For D7, D6, D5 respectively:
                                            (D7, D6, D5) = 000: low-voltage positive/pseudo emitter-coupled logic (LVPECL);
                                            (D7, D6, D5) = 001: CMOS;
                                            (D7, D6, D5) = 010: HCSL33;
                                            (D7, D6, D5) = 011: Low Voltage Differential Signal (LVDS);
                                            (D7, D6, D5) = 100: CMOS2;
                                            (D7, D6, D5) = 101: CMOSD;
                                            (D7, D6, D5) = 110: HCSL25.
D4~D3       11          clk1_pwr_sel[1:0]   Output Drive Voltage is set by those bits.
                                            D4 D3 = 00 sets 1.8V.
                                            D4 D3 = 10 sets 2.5V.
                                            D4 D3 = 11 sets 3.3V.
D2          0               unused          bits Factory reserved unused bits.
D1~D0       11            CLK1_slew[1:0]    Slew rate control for CMOS single-ended.
                                            D1 D0 = 00 then output slew rate is 0.8*Normal.
                                            D1 D0 = 01 then output slew rate indicates 0.85*Normal.
                                            D1 D0 = 10 then output slew rate indicates 0.9*Normal.
                                            D1 D0 = 11 then output slew rate indicates 1*Normal.

Reg:0x61
D7~D3       00000    CLK1_slew_diff[5:0]    Unused register bits.
D2          0           SDOE_CLK1           SDOE_CLK1 = 1 causes OUT1 to disable to Hi-Z when disabled with SD/OE pin.
                                            SDOE_CLK1 = 0 causes OUT1 to disable to High/Low when disabled with SD/OE pin.
D1          0           clk1_amuxen2        This bit is used to disable the output value.
                                            Active High (1) to disable output.
D0          1           en_clkbuf1          This bit is used to enable the clock output.
                                            Active High (1) to enable the clock output
*/
//0x7B ,0x04 ,0x7B ,0x04 ,0x7B ,0x04 ,0x7B ,0x04 ,0xFF ,0x04
unsigned char  IDT_REG_Start60[0xA]=
{

    0x3B,//0xBB,
	0x05,
	0x3B,//0xBB,
	0x05,
	0x3B,//TTL	0xBB,//LVDS
	0x05,
    0x7B,//TTL	0xBB,//LVDS
	0x05,
	0xFF,
	0x7C,
};

unsigned char  IDT_REG_end[2*2]=
{
		0x1c,0x1f,
		0x1c,0x9f,
};

void InitIDT_Reg(u8 u8DevId)
{
	unsigned short i=0;
	for(i=0;i<0x10;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x10+i, IDT_REG_Start00_25M_In[i]);
		//IDT_DBG_Print(("ret=%d\r\n",ret));
	}

	for(i=0;i<0x10;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x20+i, IDT_REG_Start20_Fout1[i]);
	}

	for(i=0;i<0x10;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x30+i, IDT_REG_Start30_Fout2[i]);
	}

	for(i=0;i<0x10;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x40+i, IDT_REG_Start40_Fout3[i]);
	}

	for(i=0;i<0x10;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x50+i, IDT_REG_Start50_Fout4[i]);
	}


	for(i=0;i<0xa;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x60+i, IDT_REG_Start60[i]);
	}

	for(i=0;i<2;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,IDT_REG_end[i*2+0], IDT_REG_end[i*2+1]);
	}
}

#define VC6E_STEP_NORMAL   1
#if 0
void IDT_UserCustomConfigOutClk_Normal(u8 u8DevId,unsigned int OutClk1,unsigned int OutClk2,unsigned int OutClk3,unsigned int OutClk4)
{
	//四个输出无关联配置FOUT1-4.
	u32 OutClk1_INT=0,OutClk2_INT=0,OutClk3_INT=0,OutClk4_INT=0;
	u32 OutClk1_FRAC=0,OutClk2_FRAC=0,OutClk3_FRAC=0,OutClk4_FRAC=0;

	OutClk1_INT=DEFAULT_VCO_Clk/((OutClk1*10000)/VC6E_STEP_NORMAL*2);
	OutClk2_INT=DEFAULT_VCO_Clk/((OutClk2*10000)/VC6E_STEP_NORMAL*2);
	OutClk3_INT=DEFAULT_VCO_Clk/((OutClk3*10000)/VC6E_STEP_NORMAL*2);
	OutClk4_INT=DEFAULT_VCO_Clk/((OutClk4*10000)/VC6E_STEP_NORMAL*2);
	OutClk1_FRAC=(unsigned int)((double)((double)DEFAULT_VCO_Clk/(double)((OutClk1*10000)/VC6E_STEP_NORMAL*2)-(double)OutClk1_INT)*FRAC_Coeff);
	OutClk2_FRAC=(unsigned int)((double)((double)DEFAULT_VCO_Clk/(double)((OutClk2*10000)/VC6E_STEP_NORMAL*2)-(double)OutClk2_INT)*FRAC_Coeff);
	OutClk3_FRAC=(unsigned int)((double)((double)DEFAULT_VCO_Clk/(double)((OutClk3*10000)/VC6E_STEP_NORMAL*2)-(double)OutClk3_INT)*FRAC_Coeff);
	OutClk4_FRAC=(unsigned int)((double)((double)DEFAULT_VCO_Clk/(double)((OutClk4*10000)/VC6E_STEP_NORMAL*2)-(double)OutClk4_INT)*FRAC_Coeff);
#if 1
	//out1 clk cfg
	IDT_REG_Start20_Fout1[0x5] &= 0x03;
	IDT_REG_Start20_Fout1[0x5] |= (OutClk1_FRAC&0x3F)<<2;
	IDT_REG_Start20_Fout1[0x4] = (OutClk1_FRAC>>6)&0xFF;
	IDT_REG_Start20_Fout1[0x3] = (OutClk1_FRAC>>14)&0xFF;
	IDT_REG_Start20_Fout1[0x2] = (OutClk1_FRAC>>22)&0xFF;

	IDT_REG_Start20_Fout1[0xE] &= 0x0F;
	IDT_REG_Start20_Fout1[0xE] |= (OutClk1_INT&0x0F)<<4;
	IDT_REG_Start20_Fout1[0xD] = (OutClk1_INT>>4)&0xFF;

	//out2 clk cfg
	IDT_REG_Start30_Fout2[0x5] &= 0x03;
	IDT_REG_Start30_Fout2[0x5] |= (OutClk2_FRAC&0x3F)<<2;
	IDT_REG_Start30_Fout2[0x4] = (OutClk2_FRAC>>6)&0xFF;
	IDT_REG_Start30_Fout2[0x3] = (OutClk2_FRAC>>14)&0xFF;
	IDT_REG_Start30_Fout2[0x2] = (OutClk2_FRAC>>22)&0xFF;

	IDT_REG_Start30_Fout2[0xE] &= 0x0F;
	IDT_REG_Start30_Fout2[0xE] |= (OutClk2_INT&0x0F)<<4;
	IDT_REG_Start30_Fout2[0xD] = (OutClk2_INT>>4)&0xFF;

	//out3 clk cfg
	IDT_REG_Start40_Fout3[0x5] &= 0x03;
	IDT_REG_Start40_Fout3[0x5] |= (OutClk3_FRAC&0x3F)<<2;
	IDT_REG_Start40_Fout3[0x4] = (OutClk3_FRAC>>6)&0xFF;
	IDT_REG_Start40_Fout3[0x3] = (OutClk3_FRAC>>14)&0xFF;
	IDT_REG_Start40_Fout3[0x2] = (OutClk3_FRAC>>22)&0xFF;

	IDT_REG_Start40_Fout3[0xE] &= 0x0F;
	IDT_REG_Start40_Fout3[0xE] |= (OutClk3_INT&0x0F)<<4;
	IDT_REG_Start40_Fout3[0xD] = (OutClk3_INT>>4)&0xFF;

	//out4 clk cfg
	IDT_REG_Start50_Fout4[0x5] &= 0x03;
	IDT_REG_Start50_Fout4[0x5] |= (OutClk4_FRAC&0x3F)<<2;
	IDT_REG_Start50_Fout4[0x4] = (OutClk4_FRAC>>6)&0xFF;
	IDT_REG_Start50_Fout4[0x3] = (OutClk4_FRAC>>14)&0xFF;
	IDT_REG_Start50_Fout4[0x2] = (OutClk4_FRAC>>22)&0xFF;

	IDT_REG_Start50_Fout4[0xE] &= 0x0F;
	IDT_REG_Start50_Fout4[0xE] |= (OutClk4_INT&0x0F)<<4;
	IDT_REG_Start50_Fout4[0xD] = (OutClk4_INT>>4)&0xFF;
#endif
	InitIDT_Reg(u8DevId);
}
#endif

#if 0
void IDT_ConfigOutClk_3(u8 u8DevId,unsigned int OutClk3)
{

	u32  OutClk3_INT=0,i;
	u32  OutClk3_FRAC=0;


	OutClk3_INT=DEFAULT_VCO_Clk/(OutClk3/VC6E_STEP_NORMAL*2);

	OutClk3_FRAC=(unsigned int)((double)((double)DEFAULT_VCO_Clk/(double)(OutClk3/VC6E_STEP_NORMAL*2)-(double)OutClk3_INT)*FRAC_Coeff);


	//out3 clk cfg
	IDT_REG_Start40_Fout3[0x5] &= 0x03;
	IDT_REG_Start40_Fout3[0x5] |= (OutClk3_FRAC&0x3F)<<2;
	IDT_REG_Start40_Fout3[0x4] = (OutClk3_FRAC>>6)&0xFF;
	IDT_REG_Start40_Fout3[0x3] = (OutClk3_FRAC>>14)&0xFF;
	IDT_REG_Start40_Fout3[0x2] = (OutClk3_FRAC>>22)&0xFF;

	IDT_REG_Start40_Fout3[0xE] &= 0x0F;
	IDT_REG_Start40_Fout3[0xE] |= (OutClk3_INT&0x0F)<<4;
	IDT_REG_Start40_Fout3[0xD] = (OutClk3_INT>>4)&0xFF;


	for(i=0;i<0x10;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x40+i, IDT_REG_Start40_Fout3[i]);
	}


	for(i=0;i<2;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,IDT_REG_end[i*2+0], IDT_REG_end[i*2+1]);
	}


}

void IDT_ConfigOutClk_4(u8 u8DevId,unsigned int OutClk4)
{

	u32  OutClk4_INT=0,i;
	u32  OutClk4_FRAC=0;


	OutClk4_INT=DEFAULT_VCO_Clk/(OutClk4/VC6E_STEP_NORMAL*2);

	OutClk4_FRAC=(unsigned int)((double)((double)DEFAULT_VCO_Clk/(double)(OutClk4/VC6E_STEP_NORMAL*2)-(double)OutClk4_INT)*FRAC_Coeff);



	//out4 clk cfg
	IDT_REG_Start50_Fout4[0x5] &= 0x03;
	IDT_REG_Start50_Fout4[0x5] |= (OutClk4_FRAC&0x3F)<<2;
	IDT_REG_Start50_Fout4[0x4] = (OutClk4_FRAC>>6)&0xFF;
	IDT_REG_Start50_Fout4[0x3] = (OutClk4_FRAC>>14)&0xFF;
	IDT_REG_Start50_Fout4[0x2] = (OutClk4_FRAC>>22)&0xFF;

	IDT_REG_Start50_Fout4[0xE] &= 0x0F;
	IDT_REG_Start50_Fout4[0xE] |= (OutClk4_INT&0x0F)<<4;
	IDT_REG_Start50_Fout4[0xD] = (OutClk4_INT>>4)&0xFF;



	for(i=0;i<0x10;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x50+i, IDT_REG_Start50_Fout4[i]);
	}



	for(i=0;i<2;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,IDT_REG_end[i*2+0], IDT_REG_end[i*2+1]);
	}


}
#endif

////u16Clk: unit 10K; u8Ch 0~3 for out1~out4; u8DevId for chip iic device
void IDT_5P_OutProgramClock(u8 u8DevId,u8 u8Ch,u16 u16Clk)
{
	u32 l_u32OutClk_INT=0,i;
	u32 l_u32OutClk_FRAC=0;

//	l_u32OutClk_INT=(u32)DEFAULT_VCO_Clk/((u16Clk*10000)/VC6E_STEP_NORMAL*2);
//	l_u32OutClk_FRAC=(unsigned int)((double)((double)DEFAULT_VCO_Clk/(double)((u16Clk*10000)/VC6E_STEP_NORMAL*2)-(double)l_u32OutClk_INT)*FRAC_Coeff);
	l_u32OutClk_INT=(u32)(INPUT_CLOCK*100)/((u16Clk)/VC6E_STEP_NORMAL*2);
	l_u32OutClk_FRAC=(((INPUT_CLOCK*100)/((u16Clk)/VC6E_STEP_NORMAL*2)-(double)l_u32OutClk_INT)*FRAC_Coeff);

    switch(u8Ch)
    {
        case 0:
            {
                //out1 clk cfg
                IDT_REG_Start20_Fout1[0x5] &= 0x03;
                IDT_REG_Start20_Fout1[0x5] |= (l_u32OutClk_FRAC&0x3F)<<2;
                IDT_REG_Start20_Fout1[0x4] = (l_u32OutClk_FRAC>>6)&0xFF;
                IDT_REG_Start20_Fout1[0x3] = (l_u32OutClk_FRAC>>14)&0xFF;
                IDT_REG_Start20_Fout1[0x2] = (l_u32OutClk_FRAC>>22)&0xFF;

                IDT_REG_Start20_Fout1[0xE] &= 0x0F;
                IDT_REG_Start20_Fout1[0xE] |= (l_u32OutClk_INT&0x0F)<<4;
                IDT_REG_Start20_Fout1[0xD] = (l_u32OutClk_INT>>4)&0xFF;

                for(i=0;i<0x10;i++)
                {
                    i2c_IDT5V_write_one_byte(u8DevId,0x20+i, IDT_REG_Start20_Fout1[i]);
                }
            }
            break;
        case 1:
            {
                //out1 clk cfg
                IDT_REG_Start30_Fout2[0x5] &= 0x03;
                IDT_REG_Start30_Fout2[0x5] |= (l_u32OutClk_FRAC&0x3F)<<2;
                IDT_REG_Start30_Fout2[0x4] = (l_u32OutClk_FRAC>>6)&0xFF;
                IDT_REG_Start30_Fout2[0x3] = (l_u32OutClk_FRAC>>14)&0xFF;
                IDT_REG_Start30_Fout2[0x2] = (l_u32OutClk_FRAC>>22)&0xFF;

                IDT_REG_Start30_Fout2[0xE] &= 0x0F;
                IDT_REG_Start30_Fout2[0xE] |= (l_u32OutClk_INT&0x0F)<<4;
                IDT_REG_Start30_Fout2[0xD] = (l_u32OutClk_INT>>4)&0xFF;

                for(i=0;i<0x10;i++)
                {
                    i2c_IDT5V_write_one_byte(u8DevId,0x30+i, IDT_REG_Start30_Fout2[i]);
                }
            }
            break;

        case 2:
            {
                //out1 clk cfg
                IDT_REG_Start40_Fout3[0x5] &= 0x03;
                IDT_REG_Start40_Fout3[0x5] |= (l_u32OutClk_FRAC&0x3F)<<2;
                IDT_REG_Start40_Fout3[0x4] = (l_u32OutClk_FRAC>>6)&0xFF;
                IDT_REG_Start40_Fout3[0x3] = (l_u32OutClk_FRAC>>14)&0xFF;
                IDT_REG_Start40_Fout3[0x2] = (l_u32OutClk_FRAC>>22)&0xFF;

                IDT_REG_Start40_Fout3[0xE] &= 0x0F;
                IDT_REG_Start40_Fout3[0xE] |= (l_u32OutClk_INT&0x0F)<<4;
                IDT_REG_Start40_Fout3[0xD] = (l_u32OutClk_INT>>4)&0xFF;

                for(i=0;i<0x10;i++)
                {
                    i2c_IDT5V_write_one_byte(u8DevId,0x40+i, IDT_REG_Start40_Fout3[i]);
                }
            }
            break;

        case 3:
            {
                //out1 clk cfg
                IDT_REG_Start50_Fout4[0x5] &= 0x03;
                IDT_REG_Start50_Fout4[0x5] |= (l_u32OutClk_FRAC&0x3F)<<2;
                IDT_REG_Start50_Fout4[0x4] = (l_u32OutClk_FRAC>>6)&0xFF;
                IDT_REG_Start50_Fout4[0x3] = (l_u32OutClk_FRAC>>14)&0xFF;
                IDT_REG_Start50_Fout4[0x2] = (l_u32OutClk_FRAC>>22)&0xFF;

                IDT_REG_Start50_Fout4[0xE] &= 0x0F;
                IDT_REG_Start50_Fout4[0xE] |= (l_u32OutClk_INT&0x0F)<<4;
                IDT_REG_Start50_Fout4[0xD] = (l_u32OutClk_INT>>4)&0xFF;

                for(i=0;i<0x10;i++)
                {
                    i2c_IDT5V_write_one_byte(u8DevId,0x50+i, IDT_REG_Start50_Fout4[i]);
                }
            }
            break;
        default:
            break;
    }

	InitIDT_Reg(u8DevId);
}


#if 0
void InitIDT_Reg_Setting(u8 u8DevId)
{
	unsigned short i=0;

	for(i=0;i<0x10;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,0x30+i, IDT_REG_Start30_Fout2[i]);
	}

	for(i=0;i<2;i++)
	{
		i2c_IDT5V_write_one_byte(u8DevId,IDT_REG_end[i*2+0], IDT_REG_end[i*2+1]);
	}
}

void IDT_UserCustomConfigOutClk_Setting(u8 u8DevId,unsigned int OutClk2)
{
	unsigned int OutClk2_INT =0;
	unsigned int OutClk2_FRAC=0;
	OutClk2_INT=DEFAULT_VCO_Clk/(OutClk2/VC6E_STEP_NORMAL*2);
	OutClk2_FRAC=(unsigned int)((double)((double)DEFAULT_VCO_Clk/(double)(OutClk2/VC6E_STEP_NORMAL*2)-(double)OutClk2_INT)*FRAC_Coeff);
	//out2 clk cfg
	IDT_REG_Start30_Fout2[0x5] &= 0x03;
	IDT_REG_Start30_Fout2[0x5] |= (OutClk2_FRAC&0x3F)<<2;
	IDT_REG_Start30_Fout2[0x4] = (OutClk2_FRAC>>6)&0xFF;
	IDT_REG_Start30_Fout2[0x3] = (OutClk2_FRAC>>14)&0xFF;
	IDT_REG_Start30_Fout2[0x2] = (OutClk2_FRAC>>22)&0xFF;

	IDT_REG_Start30_Fout2[0xE] &= 0x0F;
	IDT_REG_Start30_Fout2[0xE] |= (OutClk2_INT&0x0F)<<4;
	IDT_REG_Start30_Fout2[0xD] = (OutClk2_INT>>4)&0xFF;

    InitIDT_Reg_Setting(u8DevId);

}
#endif

void IDT_5P6965_init(void)
{
    u8 i;

    for(i=0;i<DEV_IDT5V_NUM;i++)
    {
        IDT_5P_OutProgramClock(i,0,400);
        IDT_5P_OutProgramClock(i,1,14850);
        IDT_5P_OutProgramClock(i,2,14850);
        IDT_5P_OutProgramClock(i,3,14850);
        //InitIDT_Reg(i);

//        IDT_UserCustomConfigOutClk_Normal(10000,10000,14850,14850);
//        delay_ms(500);
//        IDT_UserCustomConfigOutClk_Normal(10000,10000,14850,14850);
    }
}
#endif

void IDT_5V_init(void)
{
    memset(g_stIDTConfigVal,0,(sizeof(stIDTConfig_t)*DEV_IDT5V_NUM));
    if(pvvInitIDTFunc)
    {
        pvvInitIDTFunc();
    }
    else
    {
        #if _ENABLE_CHIP_IDT5V49EE902
        IDT_5V_init_default();
        #endif

        #if _ENABLE_CHIP_IDT5P49V6965
        IDT_5P6965_init();
        #endif
    }
}

void mapp_IDTProcess(void)
{
    u8 i,l_u8DevId;

    for(l_u8DevId=0;l_u8DevId<DEV_IDT5V_NUM;l_u8DevId++)
    {
        for(i=0;i<4;i++)
        {
            if(g_stIDTConfigVal[l_u8DevId].bDataChange[i] == true)
            {
                g_stIDTConfigVal[l_u8DevId].bDataChange[i] = false;
                #if _ENABLE_CHIP_IDT5V49EE902
                IDT_5V_OutProgramClock(l_u8DevId,i,g_stIDTConfigVal[l_u8DevId].u16PClk[i]);
                #endif

                #if _ENABLE_CHIP_IDT5P49V6965
                IDT_5P_OutProgramClock(l_u8DevId,i,g_stIDTConfigVal[l_u8DevId].u16PClk[i]);
                #endif
            }
        }
    }
}

void mapp_IDTSetPclk(u8 u8DevID,u16 u16PClk, u8 u8Channel)
{
    g_stIDTConfigVal[u8DevID].bDataChange[u8Channel] = true;
    g_stIDTConfigVal[u8DevID].u16PClk[u8Channel] = u16PClk;
    //g_stIDTConfigVal[u8DevID].u8Channel = u8Channel;
}





