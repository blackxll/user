



//=====================================================================//
// Check Timer Flag
//=====================================================================//
extern uint8_t R_TimerFlag;
#define C_TimerFlag_5ms_Bit			BIT7
#define C_TimerFlag_10ms_Bit		BIT6
#define C_TimerFlag_20ms_Bit		BIT5
#define C_TimerFlag_50ms_Bit		BIT4
#define C_TimerFlag_100ms_Bit		BIT3
#define C_TimerFlag_1s_Bit			BIT2


//=====================================================================//
// 定时器采样变量
//=====================================================================//
extern uint8_t R_TimerCount_Sampling;
#define D_TimerCount_50msValue		1		// 50ms base


//==========================================================================
// FUNCTION: Main Timer Init
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Timer_Init(void);


//==========================================================================
// FUNCTION: 初始化时间标志的变量
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_TimerCount_Init(void);


//==========================================================================
// FUNCTION: 置位相应时间标志位
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_TimerCounting_Service(void);





