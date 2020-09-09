


//==========================================================================
// 按键读取方式
//==========================================================================
//#define SEN_PIN_READ_HIGH_TRIGGER		// 传感器引脚读取高电平触发




//==========================================================================
// 传感器状态定义
//==========================================================================
#define C_KEY_SEN01				BIT3
#define C_KEY_SEN02				BIT2
#define C_KEY_SEN_MASK			(C_KEY_SEN01 | C_KEY_SEN02)


//==========================================================================
// 按键状态定义:
// 楼梯灯设置－－流水灯设置－－延时关灯－－亮度－－速度
//==========================================================================
//#define C_KEY_Stair				BIT13
//#define C_KEY_Flow				BIT12
//#define C_KEY_DelayTime			BIT11
//#define C_KEY_Bright			BIT9
//#define C_KEY_Time				BIT10

//==========================================================================
// 按键状态定义:
// 楼梯灯设置－－流水灯设置－－延时关灯－－亮度－－速度
//==========================================================================
#define C_KEY_Stair				BIT13
#define C_KEY_DelayTime			BIT12
#define C_KEY_Bright			BIT11
#define C_KEY_Time				BIT9
#define C_KEY_Flow				BIT10



//==========================================================================
// 按键复用定义
//==========================================================================
#define C_KEY_MAIN_MASK			(C_KEY_Stair | C_KEY_Flow | C_KEY_DelayTime | C_KEY_Bright | C_KEY_Time)
#define C_KEY_INC				C_KEY_Bright
#define C_KEY_DEC				C_KEY_Time




//==========================================================================
// 按键定义
//==========================================================================
#define D_IO_Direct_Mask_1		(BIT2|BIT3)
#define D_IO_Direct_Mask_2		(BIT1|BIT2|BIT3|BIT4|BIT5)
#define D_IO_Direct_Mask		(D_IO_Direct_Mask_1 | (D_IO_Direct_Mask_2 << 8))

// 定义读取端口的顺序
#define P_DirectInPort_1		P1->PIN
#define P_DirectInPort_2		P3->PIN

// 定义按键的GPIO口
#define P_DirectInGPIO_1		P1
#define P_DirectInGPIO_2		P3



//========================================================================
// Define KEY RAM
//========================================================================
// KEY State
extern uint32_t R_NewKey;
extern uint32_t R_HisKey;
extern uint32_t R_ReleaseKey;


// KEY Debounce
extern uint8_t R_DebounceCnt;
#define D_DirectKey_DebounceCnt_Max		3	// 20ms base


// KEY Scan Delay
extern uint8_t R_DirectKey_Scan_Delay;
#define D_DirectKey_DelayTime			1	// 20ms base




//========================================================================
// 按键分类
//========================================================================
// 主程序按键
extern uint32_t R_MainFlow_NewKey;
extern uint32_t R_MainFlow_HisKey;
extern uint32_t R_MainFlow_ReleaseKey;

// 传感器1
extern uint32_t R_Sen_NewKey;
extern uint32_t R_Sen_HisKey;
extern uint32_t R_Sen_ReleaseKey;



//==========================================================================
//功能:	扫描按键IO初始化
//输入:	无
//输出:	无
//==========================================================================
extern void F_ScanKey_GPIO_Init(void);


//========================================================================
//功能:	扫描按键状态
//输入:	无
//输出:	无
//========================================================================
extern void F_ScanKey_Service(void);













