//==========================================================================
// Function Define:
// void F_SysClock_Init(void)
// void F_WDT_RST(void)
// void F_GPIO_Init(void)
// void F_SystemDevice_Init(void)
// void F_Restart_SYS(void)
//==========================================================================
#include "Public.h"


//==========================================================================
// FUNCTION: 系统时钟初始化
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
void F_SysClock_Init(void)
{
	#ifdef SYS_CLK_FOR_OSC22M
	{
		//==================================================================
		//==================================================================
		// 内部RC 22.1184MHz
		//==================================================================
		//==================================================================
		// Unlock protected registers
		SYS_UnlockReg();

		//========================================================
		// Enable Internal RC 22.1184MHz clock
		CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);
		// Waiting for Internal RC clock ready
		CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);
		// Switch HCLK clock source to Internal RC and HCLK source divide 1
		CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));
		//  Set HCLK frequency 32MHz
		CLK_SetCoreClock(50000000);
		
		//========================================================
		// TIMER0 CLK for MainFlow Time
		//========================================================
		// Enable Timer0 module clock
		CLK_EnableModuleClock(TMR0_MODULE);    
		// Select Timer0 module clock source
		CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HIRC, 0);


		//========================================================
		// WDT CLK
		//========================================================
		#ifdef WDT_FUC_ENABLE
			// Enable WDT module clock
			CLK_EnableModuleClock(WDT_MODULE);    
			// Select WDT module clock source
			CLK_SetModuleClock(WDT_MODULE, CLK_CLKSEL1_WDT_S_HCLK_DIV2048, NULL);
			// WDT timeout every 2^14 WDT clock, Enable system reset, disable wake up system(1.7S)
			WDT_Open(WDT_TIMEOUT_2POW14, 0, TRUE, FALSE);
		#endif
		
		// Lock protected registers
		SYS_LockReg();
	}
	#else
	{
		//==================================================================
		//==================================================================
		// 外部晶振4~24 MHz
		//==================================================================
		//==================================================================
		// Unlock protected registers
		SYS_UnlockReg();
		
		//========================================================
		// SYSTEM CLK
		//========================================================
		// Enable External XTAL (4~24 MHz) HXT, 32KHz LXT
		CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
		// Waiting for 12MHz clock ready
		CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);
		// Switch HCLK clock source to HXT
		CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT,CLK_CLKDIV_HCLK(1));
		//  Set HCLK frequency 32MHz
		CLK_SetCoreClock(50000000);
		
		//========================================================
		// TIMER0 CLK for MainFlow Time
		//========================================================
		// Enable Timer0 module clock
		CLK_EnableModuleClock(TMR0_MODULE);    
		// Select Timer0 module clock source
		CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);

		//========================================================
		// WDT CLK
		//========================================================
		#ifdef WDT_FUC_ENABLE
			// Enable WDT module clock
			CLK_EnableModuleClock(WDT_MODULE);    
			// Select WDT module clock source
			CLK_SetModuleClock(WDT_MODULE, CLK_CLKSEL1_WDT_S_HCLK_DIV2048, NULL);
			// WDT timeout every 2^14 WDT clock, Enable system reset, disable wake up system(1.7S)
			WDT_Open(WDT_TIMEOUT_2POW14, 0, TRUE, FALSE);
		#endif
		
		// Lock protected registers
		SYS_LockReg();
	}
	#endif
}


//==========================================================================
// FUNCTION: 清看门狗定时器计数器(1.7S)
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
void F_WDT_RST(void)
{
#ifdef WDT_FUC_ENABLE
	__set_PRIMASK(1);		// 关中断
	SYS_UnlockReg();		// 解锁寄存器
	WDT_RESET_COUNTER();	// 清看门狗
	SYS_LockReg();			// 锁上寄存器
	__set_PRIMASK(0);		// 开中断
#endif
}


//==========================================================================
// FUNCTION: IO端口初始化
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
void F_GPIO_Init(void)
{
	// 初始化按键IO
	F_ScanKey_GPIO_Init();
	
	// LED显示初始化IO
	F_Disp_GPIO_Init();
	
	// GS1903初始化IO
	F_GS1903_GPIO_Init();
	
	// 测试
//	GPIO_SetMode(P0, (BIT0|BIT1), GPIO_PMD_OUTPUT);
//	GPIO_ResetBits(P0, (BIT0|BIT1));
}


//==========================================================================
// FUNCTION: 驱动程序初始化
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
void F_SystemDevice_Init(void)
{
	__set_PRIMASK(1);		// 关闭中断
	F_SysClock_Init();		// 初始化系统时钟
	F_GPIO_Init();			// 初始化IO口设置
	F_Timer_Init();			// 初始化时钟计数
	F_TimerCount_Init();	// 时间标志处理初始化
	__set_PRIMASK(0);		// 打开中断
}


//==========================================================================
// FUNCTION: 系统复位
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
void F_Restart_SYS(void)
{
    UNLOCKREG();
    //SYSCLK->PWRCON.XTL12M_EN = 1;
	CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk;
	
    // Waiting for 12M Xtal stable
    DrvSYS_Delay(5000);
	
    // Enable ISP function
    //DrvFMC_EnableISP();
	if(CLK->PWRCON & CLK_PWRCON_OSC22M_EN_Msk)
	{
		CLK->PWRCON |= CLK_PWRCON_OSC22M_EN_Msk;
		DrvSYS_Delay(12);
	}
	CLK->AHBCLK |= CLK_AHBCLK_ISP_EN_Msk;
	FMC->ISPCON |= FMC_ISPCON_ISPEN_Msk;

	// Reset CPU to boot to AP mode
	//DrvFMC_BootSelect(E_FMC_APROM);
	FMC->ISPCON &= (~FMC_ISPCON_BS_Msk);
	//DrvSYS_ResetCPU();
	SYS->IPRSTC1 |= SYS_IPRSTC1_CPU_RST_Msk;
}









