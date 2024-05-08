/**
  *****************************************************************************
  *                            系统延时相关的函数
  * 详细说明请参考《Cortex-M3权威指南(中文)》第133 ~ 134页 第8章 SysTick定时器介绍
  *
  *                       (C) Copyright 2000-2020, ***
  *                            All Rights Reserved
  *****************************************************************************
  *
  * @File    : delay.c
  * @By      : Sam Chan
  * @Version : V1.0
  * @Date    : 2012 / 10 / 20
  *
  *****************************************************************************
  *                                    Update
  * @Version : V1.0.1
  * @By      : Sam Chan
  * @Date    : 2014 / 02 / 26
  * @Brief   : 增加另外一种延时计算方法
  *
  * @Version : V1.1
  * @By      : Sam Chan
  * @Date    : 2014 / 05 / 24
  * @Brief   : A、增加对C++环境支持
  *            B、修改在跑ucos时初始化嘀嗒定时器、延时us和ms函数
  *
  *****************************************************************************
**/



/******************************************************************************

* @ SysTick定时器 相关控制寄存器说明

@ 1、SysTick控制及状态寄存器（地址：0xE000_E010）复位值为0

	bit16 COUNTFLAG(R）  -> 如果在上次读取本寄存器后，SysTick已经数到了0，则该位为1。如果读取该位，该位将自动清零
	bit2  CLKSOURCE(R/W) -> 0=外部时钟源(STCLK)。1=内核时钟(FCLK)
	bit1  TICKINT(R/W)   -> 1=SysTick倒数到0时产生SysTick异常请求,0=数到0时无动作
	bit0  ENABLE(R/W)    -> SysTick定时器的使能位

@ 2、SysTick重装载数值寄存器（地址：0xE000_E014）复位值为0

	[23:0] RELOAD(R/W) -> 当倒数至零时，将被重装载的值

@ 3、SysTick当前数值寄存器（地址：0xE000_E018） 复位值为0

	[23:0] CURRENT(R/Wc) -> 读取时返回当前倒计数的值，写它则使之清零，同时还会清除在SysTick 控制及状态寄存器中的COUNTFLAG标志

@ 4、SysTick校准数值寄存器（地址：0xE000_E01C）复位值: bit31未知。bit30未知。[23:0]为0

	bit32 NOREF(R)    -> 1=没有外部参考时钟（STCLK不可用）。0=外部参考时钟可用
	bit30 SKEW(R)     -> 1=校准值不是准确的10ms。0=校准值是准确的10ms
	[23:0] TENMS(R/W) -> 10ms的时间内倒计数的格数。芯片设计者应该通过Cortex‐M3的输入信号提供该数值。若该值读回零，则表示无法使用校准功能

******************************************************************************/

#include "delay.h"
#include "includes.h"



//=============================================================================


#if _USER_SysTick
void delay_init(void)
{
    ;
}

/**
  *****************************************************************************
  * @Name   : 延时n个us
  *
  * @Brief  :
  *
  * @Input  : nus：要延时的us数
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_us(uint32_t nus)
{
	uint32_t endcnt,CPU_cnt;

	CPU_cnt = nus*(SystemCoreClock/(1000*1000));	//计算达到所需延时值的CPU时钟数。即多少个系统时钟计数
	while(endcnt++<CPU_cnt);
}
#else
/**
  *****************************************************************************
  * @Name   : 初始化延时函数
  *
  * @Brief  : 主要Cortex-M3内核对系统时钟计数单元
  *           详细见《Cortex-M3权威指南(中文)》第216页 a)  时钟周期（CYCCNT） 的内容
  *           周立功《CM3计数参考手册》第28、29页、第110、125页
  *           no support cortex-M0
  * @Input  : none
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_init(void)
{
	DEM_CTRL |= 1<<24;	//该位必须为1，使能跟踪和调试模块的使用。详细见：周立功《CM3计数参考手册》第115页介绍
						//在没有使用跟踪时，该位使能对功率使用的控制。它能够由应用程序或调试器使能，供ITM使用
						//在DWT能够使用之前，调试异常和监控控制寄存器的TRCENA(bit24)位必须置位

	DWT_CTRL |= 1<<0;	//使能DWT_CYCCNT计数器。
						//如果不使能，则计数器不执行计数操作，因此不会产生PC采样或CYCCNTENA事件。
						//在正常使用时，CYCCNT计数器应由调试器初始化为0。
}

/**
  *****************************************************************************
  * @Name   : 延时n个us
  *
  * @Brief  : no support cortex-M0
  *
  * @Input  : nus：要延时的us数
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_us(uint32_t nus)
{
	uint32_t savecount,endcnt,CPU_cnt;

#if 0//def GD32F4XX
	CPU_cnt = nus*(SystemCoreClock/(1000*1000));	//计算达到所需延时值的CPU时钟数。即多少个系统时钟计数
	while(endcnt++<CPU_cnt);
#else
	savecount = DWT_CYCCNT;	//保存计数器当前数值
	CPU_cnt = nus*(SystemCoreClock/(1000*1000));	//计算达到所需延时值的CPU时钟数。即多少个系统时钟计数
											//得到更精确延时时间，减去前面代码运行的时间即可

	endcnt = savecount + CPU_cnt;	//计算所需延时时间DWT_CYCCNT的计数值，在溢出时返回到0

	if(endcnt > savecount)	//所需延时值大于当前计数值
	{
		while(DWT_CYCCNT < endcnt);	//循环等待所需要的延时时间的CPU时钟计数值
	}
	else	//小于当前计数值
	{
		while(DWT_CYCCNT > endcnt);	//等待计数器溢出翻转
		while(DWT_CYCCNT < endcnt);	//等待所需延时时间到达
	}
#endif
}
#endif

/**
  *****************************************************************************
  * @Name   : 延时n个ms
  *
  * @Brief  : 延时ms级定义，延时范围：1 ~ 65535ms。延时最大值可变，不爆机uint32_t/1000范围即可
  *
  * @Input  : nms：要延时的ms数
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_ms(uint16_t nms)
{
#if _SYSTEM_SUPPORT_RTOS  //使用os了
	if(vTaskIsRunning() == 1)  //os已经在跑了
	{
		vTaskDelay(nms);  //采用os延时
	}
    else
    {
        delay_us((uint32_t)(nms*1000));  //采用普通的延时
    }
#else

	delay_us((uint32_t)(nms*1000));  //采用普通的延时

#endif  /* end _SYSTEM_SUPPORT_RTOS */
}

void delay_ms_cpu(uint16_t nms)
{
    delay_us((uint32_t)(nms*1000));  //采用普通的延时
}
//=============================================================================




