//==========================================================================
// Function Define:
// void F_Timer1_Init(void)
// void TMR1_IRQHandler(void)
// void F_TimerCount_Init(void)
// void F_TimerCounting_Service(void)
//==========================================================================
#include "Public.h"


uint8_t R_TimerFlag = 0;				// 定时器标志
uint8_t R_TimerCount_Sampling = 0;		// 定时器采样变量
uint8_t R_TimerCount_50msBase_Cnt = 0;	// 20ms 定时计数器


//==========================================================================
// FUNCTION: Main Timer Init
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Timer_Init(void)
{
	// Open Timer0 in periodic mode, enable interrupt and 1 interrupt tick per second
	// 1ms intrerrupt
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 20);
	
	// 使能定时器0中断
    TIMER_EnableInt(TIMER0);
	
    // Enable Timer0 NVIC(设置使能控制寄存器)
    NVIC_EnableIRQ(TMR0_IRQn);
	
    // 定时器0开始计数
	TIMER_Start(TIMER0);
}


//==========================================================================
// FUNCTION: 初始化时间标志的变量
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_TimerCount_Init(void)
{
	R_TimerFlag = 0;
	R_TimerCount_Sampling = 0;
	R_TimerCount_50msBase_Cnt = 0;
}


//==========================================================================
// FUNCTION: 置位相应时间标志位
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_TimerCounting_Service(void)
{
	// Clear Flag
	R_TimerFlag = 0;

	if(R_TimerCount_Sampling < D_TimerCount_50msValue)
	{
		// Time Wait
	}
	else
	{
		// Timer Out,Set Flag
		if(R_TimerCount_Sampling > (D_TimerCount_50msValue * 3))
		{
			// Sampling Out
			R_TimerCount_Sampling = D_TimerCount_50msValue + 1;
		}
		R_TimerCount_Sampling -= D_TimerCount_50msValue;

		// Set 10ms flag
		R_TimerFlag |= C_TimerFlag_50ms_Bit;
		R_TimerCount_50msBase_Cnt++;

		// Check 100ms flag
		if(R_TimerCount_50msBase_Cnt >= 2)
		{
			R_TimerFlag |= C_TimerFlag_100ms_Bit;
			R_TimerCount_50msBase_Cnt = 0;
		}
	}
}


//==========================================================================
// FUNCTION: Timer IRQ
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void TMR0_IRQHandler(void)
{	
    // clear timer interrupt flag
    TIMER_ClearIntFlag(TIMER0);
	
	//=========================================
	// Timer Time Count
	R_TimerCount_Sampling++;
	
	//=========================================
	// Key Scan Delay
//	if(R_DirectKey_Scan_Delay < 0xFF)					R_DirectKey_Scan_Delay++;
	// Key Debounce Time Count
//	if(R_DebounceCnt < D_DirectKey_DebounceCnt_Max)		R_DebounceCnt++;
}



