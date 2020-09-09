



//==========================================================================
// 显示更新处理
//==========================================================================
// 显示数据有变化时,才发送更新的数据, 否则周期性发送数据
#define DISP_DATA_UPDATA_FOR_CHANGE



//==========================================================================
// TM3130控制IO定义
//==========================================================================
#define P_DISP_DIN_GPIO			P4
#define C_DISP_DIN_BIT			BIT2
#define P_DISP_DIN				P42


//==========================================================================
// TM3130输出时间定义
// 发送0码和1码的周期时间为1.25us(频率800KHz)
// DAT = 0, 高电平400ns, 低电平800ns
// DAT = 1, 高电平800ns, 低电平400ns
//==========================================================================
#define TM3130_TIME_400NS()		__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();						
#define TM3130_TIME_800NS()		__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();	\
								__NOP();__NOP();__NOP();__NOP();__NOP();

// 发送0码 
#define TM3130_SEND_CODE0()		P_DISP_DIN = 1;			\
								TM3130_TIME_400NS();	\
								P_DISP_DIN = 0;			\
								TM3130_TIME_800NS();
// 发送1码 
#define TM3130_SEND_CODE1()		P_DISP_DIN = 1;			\
								TM3130_TIME_800NS();	\
								P_DISP_DIN = 0;			\
								TM3130_TIME_400NS();



//==========================================================================
// 进度条位定义
//==========================================================================
#define C_DISP_LV01_BIT			0x04
#define C_DISP_LV02_BIT			0x01
#define C_DISP_LV03_BIT			0x10
#define C_DISP_LV04_BIT			0x40
#define C_DISP_LV05_BIT			0x80
#define C_DISP_LV06_BIT			0x20
#define C_DISP_LV07_BIT			0x08
#define C_DISP_LV08_BIT			0x02


//==========================================================================
// 显示LV定义
//==========================================================================
#define C_DISP_SET_LV01			(C_DISP_LV01_BIT)
#define C_DISP_SET_LV02			(C_DISP_LV01_BIT + C_DISP_LV02_BIT)
#define C_DISP_SET_LV03			(C_DISP_LV01_BIT + C_DISP_LV02_BIT + C_DISP_LV03_BIT)
#define C_DISP_SET_LV04			(C_DISP_LV01_BIT + C_DISP_LV02_BIT + C_DISP_LV03_BIT + C_DISP_LV04_BIT)
#define C_DISP_SET_LV05			(C_DISP_LV01_BIT + C_DISP_LV02_BIT + C_DISP_LV03_BIT + C_DISP_LV04_BIT + C_DISP_LV05_BIT)
#define C_DISP_SET_LV06			(C_DISP_LV01_BIT + C_DISP_LV02_BIT + C_DISP_LV03_BIT + C_DISP_LV04_BIT + C_DISP_LV05_BIT + C_DISP_LV06_BIT)
#define C_DISP_SET_LV07			(C_DISP_LV01_BIT + C_DISP_LV02_BIT + C_DISP_LV03_BIT + C_DISP_LV04_BIT + C_DISP_LV05_BIT + C_DISP_LV06_BIT + C_DISP_LV07_BIT)
#define C_DISP_SET_LV08			(C_DISP_LV01_BIT + C_DISP_LV02_BIT + C_DISP_LV03_BIT + C_DISP_LV04_BIT + C_DISP_LV05_BIT + C_DISP_LV06_BIT + C_DISP_LV07_BIT + C_DISP_LV08_BIT)



//==========================================================================
// 显示变量定义
//==========================================================================

// 显示状态标志
extern uint8_t R_Disp_Update_Flag;
#define C_DISP_DATA_UPDATE				0x01	// 显示数据更新时刷新显示
//=============================================
#define Set_Disp_Data_Update()			R_Disp_Update_Flag |=  C_DISP_DATA_UPDATE;
#define Clr_Disp_Data_Update()			R_Disp_Update_Flag &= ~C_DISP_DATA_UPDATE;
#define  Be_Disp_Data_Update()			(R_Disp_Update_Flag & C_DISP_DATA_UPDATE)


// 进度条当前的显示类型
extern uint8_t R_Disp_SetType;
#define C_DISP_SET_IDLE			0
#define C_DISP_SET_STAIR_PARA	1
#define C_DISP_SET_FLOW_PARA	2



// 参数设置LED显示关闭倒计时
extern uint16_t R_Disp_SetPara_DispOff_Timer;
#define N_DISP_SET_PARA_LED_OFF_TIME	100		// 5秒(50ms base)


// 楼梯灯和流水灯设置参数
extern uint8_t R_Disp_StairLamp_Time;
extern uint8_t R_Disp_StairLamp_Bright;
extern uint8_t R_Disp_StairLamp_DelayTime;
extern uint8_t R_Disp_FlowLED_Time;
extern uint8_t R_Disp_FlowLED_Bright;
extern uint8_t R_Disp_FlowLED_DelayTime;

// 进度条显示临时存储变量
extern uint8_t R_Disp_Update_Buf[];



//==========================================================================
// FUNCTION: 显示IO初始化
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Disp_GPIO_Init(void);


//==========================================================================
// FUNCTION: TM3130 LED驱动芯片数据更新
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
extern void F_Refresh_Display(void);


//==========================================================================
// FUNCTION: 所有段位显示或清屏
// INPUT:   
//			 R_DispDat -- 显示的数据
// OUTPUT:   NONE
//==========================================================================
extern void F_Set_Disp_All(uint8_t R_DispDat);


//==========================================================================
// FUNCTION: 显示楼梯灯设置的参数
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Set_Disp_StairPara(void);


//==========================================================================
// FUNCTION: 显示流水灯设置的参数
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Set_Disp_FlowPara(void);


//==========================================================================
// FUNCTION: 楼梯灯流水时间级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Disp_StartLamp_Time_LV(void);


//==========================================================================
// FUNCTION: 楼梯灯显示亮度级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Disp_StartLamp_Bright_LV(void);


//==========================================================================
// FUNCTION: 延时流水关闭级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Disp_StartLamp_DelayOffTime_LV(void);


//==========================================================================
// FUNCTION: 流水灯流水时间级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Disp_FlowLED_Time_LV(void);


//==========================================================================
// FUNCTION: 流水灯显示亮度级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Disp_FlowLED_Bright_LV(void);


//==========================================================================
// FUNCTION: 延时流水关闭级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Disp_FlowLED_DelayOffTime_LV(void);




