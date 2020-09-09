
//==========================================================================
// PRJ:	FlowLight
// MCU:	NUC029LAN
// OSC:	External 12MHz
//==========================================================================
#include <stdio.h>
#include "NUC029xAN.h"
#include "absacc.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "math.h"


//==========================================================================
// IO Control Define
//==========================================================================
#define GPIO_SetBits(port, u32PinMask)		((port)->DOUT |= (u32PinMask))
#define GPIO_ResetBits(port, u32PinMask)	((port)->DOUT &= ~(u32PinMask))
#define GPIO_ToggleBits(port, u32PinMask)	((port)->DOUT ^= (u32PinMask))

#define UNLOCKREG(x)        SYS->REGWRPROT = 0x59;	SYS->REGWRPROT = 0x16;	SYS->REGWRPROT = 0x88;
#define LOCKREG(x)          SYS->REGWRPROT = 0;  



//==========================================================================
// 特殊功能使能
//==========================================================================
//#define 	WDT_FUC_ENABLE				// 使能看门狗
#define 	SYS_CLK_FOR_OSC22M			// 使用内部RC: 22.1184MHz


//==========================================================================
// 自定义头文件包含
//==========================================================================
#include "Init.h"
#include "Time.h"
#include "Key.h"
#include "Display.h"
#include "CRC.h"
#include "PARA_APROM.h"

//==========================================================================
// 模式头文件包含
//==========================================================================
#include "GS1903.h"
#include "FlowPublic.h"
#include "StairLamp.h"	// 楼梯灯头文件
#include "FlowLED.h"	// 流水灯头文件




//==============================================================================================================
//==============================================================================================================
// 变量参数定义
//==============================================================================================================
//==============================================================================================================


//==========================================================================
// 楼梯灯变量
//==========================================================================
// 楼梯灯流水个数
extern uint16_t R_StairLamp_FlowNum;
#define SET_STAIR_LAMP_FLOW_NUM_MIN			2
#define SET_STAIR_LAMP_FLOW_NUM_MAX			N_STAIR_LAMP_GS1903_NUM_MAX

// 楼梯灯流水间隔时间(50ms base)
extern uint16_t R_StairLamp_FlowTimer;
#define SET_STAIR_LAMP_FLOW_TIME_LV1		2
#define SET_STAIR_LAMP_FLOW_TIME_LV2		4
#define SET_STAIR_LAMP_FLOW_TIME_LV3		6
#define SET_STAIR_LAMP_FLOW_TIME_LV4		8
#define SET_STAIR_LAMP_FLOW_TIME_LV5		10
#define SET_STAIR_LAMP_FLOW_TIME_LV6		12
#define SET_STAIR_LAMP_FLOW_TIME_LV7		14
#define SET_STAIR_LAMP_FLOW_TIME_LV8		16

// 楼梯灯显示亮度
extern uint16_t R_StairLamp_DispBright;
#define SET_STAIR_LAMP_DISP_BRIGHT_LV1		31
#define SET_STAIR_LAMP_DISP_BRIGHT_LV2		63
#define SET_STAIR_LAMP_DISP_BRIGHT_LV3		95
#define SET_STAIR_LAMP_DISP_BRIGHT_LV4		127
#define SET_STAIR_LAMP_DISP_BRIGHT_LV5		159
#define SET_STAIR_LAMP_DISP_BRIGHT_LV6		191
#define SET_STAIR_LAMP_DISP_BRIGHT_LV7		223
#define SET_STAIR_LAMP_DISP_BRIGHT_LV8		255

// 楼梯灯延时启动流水灭的时间(50ms base)
extern uint16_t R_StairLamp_DelayTimer;
#define SET_STAIR_LAMP_OFF_DELAY_TIME_LV1	60
#define SET_STAIR_LAMP_OFF_DELAY_TIME_LV2	120
#define SET_STAIR_LAMP_OFF_DELAY_TIME_LV3	180
#define SET_STAIR_LAMP_OFF_DELAY_TIME_LV4	240
#define SET_STAIR_LAMP_OFF_DELAY_TIME_LV5	300
#define SET_STAIR_LAMP_OFF_DELAY_TIME_LV6	360
#define SET_STAIR_LAMP_OFF_DELAY_TIME_LV7	420
#define SET_STAIR_LAMP_OFF_DELAY_TIME_LV8	480


//==========================================================================
// 流水灯变量
//==========================================================================
// 流水灯流水个数
extern uint16_t R_FlowLED_FlowNum;
#define SET_FLOW_LED_FLOW_NUM_MIN			(2)							//(2 * 3)
#define SET_FLOW_LED_FLOW_NUM_MAX			(N_FLOW_LED_GS1903_NUM_MAX)	//(N_FLOW_LED_GS1903_NUM_MAX * 3)

// 流水灯流水间隔时间(50ms base)
extern uint16_t R_FlowLED_FlowTimer;
#define SET_FLOW_LED_FLOW_TIME_LV1			1
#define SET_FLOW_LED_FLOW_TIME_LV2			2
#define SET_FLOW_LED_FLOW_TIME_LV3			3
#define SET_FLOW_LED_FLOW_TIME_LV4			4
#define SET_FLOW_LED_FLOW_TIME_LV5			5
#define SET_FLOW_LED_FLOW_TIME_LV6			6
#define SET_FLOW_LED_FLOW_TIME_LV7			7
#define SET_FLOW_LED_FLOW_TIME_LV8			8

// 流水灯显示亮度
extern uint16_t R_FlowLED_DispBright;
#define SET_FLOW_LED_DISP_BRIGHT_LV1		31
#define SET_FLOW_LED_DISP_BRIGHT_LV2		63
#define SET_FLOW_LED_DISP_BRIGHT_LV3		95
#define SET_FLOW_LED_DISP_BRIGHT_LV4		127
#define SET_FLOW_LED_DISP_BRIGHT_LV5		159
#define SET_FLOW_LED_DISP_BRIGHT_LV6		191
#define SET_FLOW_LED_DISP_BRIGHT_LV7		223
#define SET_FLOW_LED_DISP_BRIGHT_LV8		255

// 流水灯延时启动流水灭的时间(50ms base)
extern uint16_t R_FlowLED_DelayTimer;
#define SET_FLOW_LED_OFF_DELAY_TIME_LV1		60
#define SET_FLOW_LED_OFF_DELAY_TIME_LV2		120
#define SET_FLOW_LED_OFF_DELAY_TIME_LV3		180
#define SET_FLOW_LED_OFF_DELAY_TIME_LV4		240
#define SET_FLOW_LED_OFF_DELAY_TIME_LV5		300
#define SET_FLOW_LED_OFF_DELAY_TIME_LV6		360
#define SET_FLOW_LED_OFF_DELAY_TIME_LV7		420
#define SET_FLOW_LED_OFF_DELAY_TIME_LV8		480



//==========================================================================
// 长按键的长按时间定义(20ms base)
//==========================================================================
#define SET_PARA_SHORT_KEY_TIME				10		// 500ms
#define SET_PARA_LONG_KEY_TIME				60		// 3秒





//==========================================================================
// FUNCTION: 系统存储参数为默认参数
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_SetPara_Default(void);


//==========================================================================
// FUNCTION: 系统设置参数值读取处理
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_SetPara_Check(void);
	

//==============================================================================================================
//==============================================================================================================
// 设置参数数据读取
//==============================================================================================================
//==============================================================================================================

//==========================================================================
// FUNCTION: 读取楼梯灯流水时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Read_StartLamp_Time(void);


//==========================================================================
// FUNCTION: 读取楼梯灯显示亮度
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Read_StartLamp_Bright(void);


//==========================================================================
// FUNCTION: 读取流水灭延时时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Read_StartLamp_DelayOffTime(void);


//==========================================================================
// FUNCTION: 读取流水灯流水时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Read_FlowLED_Time(void);


//==========================================================================
// FUNCTION: 读取流水灯显示亮度
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Read_FlowLED_Bright(void);


//==========================================================================
// FUNCTION: 读取流水灭延时时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Read_FlowLED_DelayOffTime(void);




//==============================================================================================================
//==============================================================================================================
// 改变进度条级别
//==============================================================================================================
//==============================================================================================================

//==========================================================================
// FUNCTION: 改变楼梯灯流水时间级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Chagne_StartLamp_TimeLV(void);


//==========================================================================
// FUNCTION: 改变楼梯灯显示亮度级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Chagne_StartLamp_BrightLV(void);


//==========================================================================
// FUNCTION: 改变流水灯显示亮度级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Chagne_StartLamp_DelayOffTimeLV(void);


//==========================================================================
// FUNCTION: 改变流水灯流水时间级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Chagne_FlowLED_TimeLV(void);


//==========================================================================
// FUNCTION: 改变流水灯显示亮度级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Chagne_FlowLED_BrightLV(void);


//==========================================================================
// FUNCTION: 改变流水灯显示亮度级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Chagne_FlowLED_DelayOffTimeLV(void);


//==========================================================================
// FUNCTION: 存储设定的工作模式
// INPUT:    
//			 R_Run_Mode -- 存储系统工作模式
// OUTPUT:   NONE
//==========================================================================
extern void F_Save_System_RunMode(uint8_t R_Run_Mode);





//==============================================================================================================
//==============================================================================================================
// 根据指定的流水参照对象计算出相对应的流水目标的流水时间
//==============================================================================================================
//==============================================================================================================


//==========================================================================
// FUNCTION: 根据FlowLED01的长度与流水时间, 计算出StairLamp的流水时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_Set_StairLamp_FlowTime_From_FlowLED(void);





