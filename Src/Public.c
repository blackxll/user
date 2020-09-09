//==========================================================================
// Function Define:
//==========================================================================
#include "Public.h"


//==========================================================================
// 楼梯灯变量定义
//==========================================================================
// 楼梯灯流水个数
uint16_t R_StairLamp_FlowNum = 0;
// 楼梯灯流水间隔时间
uint16_t R_StairLamp_FlowTimer = 0;
// 楼梯灯显示亮度
uint16_t R_StairLamp_DispBright = 0;
// 楼梯灯延时启动流水灭的时间
uint16_t R_StairLamp_DelayTimer = 0;


//==========================================================================
// 流水灯变量定义
//==========================================================================
// 流水灯流水个数
uint16_t R_FlowLED_FlowNum = 0;
// 流水灯流水间隔时间
uint16_t R_FlowLED_FlowTimer = 0;
// 流水灯显示亮度
uint16_t R_FlowLED_DispBright = 0;
// 流水灯延时启动流水灭的时间
uint16_t R_FlowLED_DelayTimer = 0;



//==========================================================================
// FUNCTION: 系统存储参数为默认参数
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_SetPara_Default(void)
{
	// 楼梯灯参数
	R_SetPara_StairLamp_Num = SET_STAIR_LAMP_FLOW_NUM_MAX;
	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV1;
	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV1;
	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV1;
	
	// 流水灯参数
	R_SetPara_FlowLED_Num = SET_FLOW_LED_FLOW_NUM_MAX;
	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV1;
	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV1;
	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV1;
}


//==========================================================================
// FUNCTION: 系统设置参数值读取处理
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_SetPara_Check(void)
{
	//======================================
	// 楼梯灯数量
	if((R_SetPara_StairLamp_Num == 0) || (R_SetPara_StairLamp_Num > SET_STAIR_LAMP_FLOW_NUM_MAX))
	{
		R_SetPara_StairLamp_Num = SET_STAIR_LAMP_FLOW_NUM_MAX;
	}
	//======================================
	// 楼梯灯流水时间级别
	if(R_SetPara_StairLamp_Time_LV >= C_SET_PARA_MAX)
	{
		R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV1;
	}
	//======================================
	// 楼梯灯显示亮度级别
	if(R_SetPara_StairLamp_Bright_LV >= C_SET_PARA_MAX)
	{
		R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV1;
	}
	//======================================
	// 楼梯灯延时时间级别
	if(R_SetPara_StairLamp_DelayOffTime_LV >= C_SET_PARA_MAX)
	{
		R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV1;
	}
	
	
	//======================================
	// 流水灯数量
	if((R_SetPara_FlowLED_Num == 0) || (R_SetPara_FlowLED_Num > SET_FLOW_LED_FLOW_NUM_MAX))
	{
		R_SetPara_FlowLED_Num = SET_FLOW_LED_FLOW_NUM_MAX;
	}
	//======================================
	// 流水灯流水时间级别
	if(R_SetPara_FlowLED_Time_LV >= C_SET_PARA_MAX)
	{
		R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV1;
	}
	//======================================
	// 流水灯显示亮度级别
	if(R_SetPara_FlowLED_Bright_LV >= C_SET_PARA_MAX)
	{
		R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV1;
	}
	//======================================
	// 流水灯延时时间级别
	if(R_SetPara_FlowLED_DelayOffTime_LV >= C_SET_PARA_MAX)
	{
		R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV1;
	}
}


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
void F_Read_StartLamp_Time(void)
{
#ifdef SET_STAIR_LAMP_FLOW_TIME_FROM_FLOW_LED
	// 根据FlowLED01的长度与流水时间, 计算出StairLamp的流水时间
	F_Set_StairLamp_FlowTime_From_FlowLED();
#else
	// 根据当前级别, 读取设定的时间
	switch(R_SetPara_StairLamp_Time_LV)
	{
		case C_SET_PARA_LV1:	R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV1;	break;
		case C_SET_PARA_LV2:	R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV2;	break;
		case C_SET_PARA_LV3:	R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV3;	break;
		case C_SET_PARA_LV4:	R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV4;	break;
		case C_SET_PARA_LV5:	R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV5;	break;
		case C_SET_PARA_LV6:	R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV6;	break;
		case C_SET_PARA_LV7:	R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV7;	break;
		case C_SET_PARA_LV8:	R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV8;	break;
		default:				R_StairLamp_FlowTimer = SET_STAIR_LAMP_FLOW_TIME_LV1;	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV1;	break;
	}
#endif
}


//==========================================================================
// FUNCTION: 读取楼梯灯显示亮度
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Read_StartLamp_Bright(void)
{
	// 根据当前级别, 读取显示亮度
	switch(R_SetPara_StairLamp_Bright_LV)
	{
		case C_SET_PARA_LV1:	R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV1;	break;
		case C_SET_PARA_LV2:	R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV2;	break;
		case C_SET_PARA_LV3:	R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV3;	break;
		case C_SET_PARA_LV4:	R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV4;	break;
		case C_SET_PARA_LV5:	R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV5;	break;
		case C_SET_PARA_LV6:	R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV6;	break;
		case C_SET_PARA_LV7:	R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV7;	break;
		case C_SET_PARA_LV8:	R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV8;	break;
		default:				R_StairLamp_DispBright = SET_STAIR_LAMP_DISP_BRIGHT_LV1;	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV1;	break;
	}
}


//==========================================================================
// FUNCTION: 读取流水灭延时时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Read_StartLamp_DelayOffTime(void)
{
	// 根据当前级别, 读取显示亮度
	switch(R_SetPara_StairLamp_DelayOffTime_LV)
	{
		case C_SET_PARA_LV1:	R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV1;	break;
		case C_SET_PARA_LV2:	R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV2;	break;
		case C_SET_PARA_LV3:	R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV3;	break;
		case C_SET_PARA_LV4:	R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV4;	break;
		case C_SET_PARA_LV5:	R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV5;	break;
		case C_SET_PARA_LV6:	R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV6;	break;
		case C_SET_PARA_LV7:	R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV7;	break;
		case C_SET_PARA_LV8:	R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV8;	break;
		default:				R_StairLamp_DelayTimer = SET_STAIR_LAMP_OFF_DELAY_TIME_LV1;	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV1;	break;
	}
}


//==========================================================================
// FUNCTION: 读取流水灯流水时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Read_FlowLED_Time(void)
{
	// 根据当前级别, 读取设定的时间
	switch(R_SetPara_FlowLED_Time_LV)
	{
		case C_SET_PARA_LV1:	R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV1;	break;
		case C_SET_PARA_LV2:	R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV2;	break;
		case C_SET_PARA_LV3:	R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV3;	break;
		case C_SET_PARA_LV4:	R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV4;	break;
		case C_SET_PARA_LV5:	R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV5;	break;
		case C_SET_PARA_LV6:	R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV6;	break;
		case C_SET_PARA_LV7:	R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV7;	break;
		case C_SET_PARA_LV8:	R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV8;	break;
		default:				R_FlowLED_FlowTimer = SET_FLOW_LED_FLOW_TIME_LV1;	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV1;	break;
	}
}


//==========================================================================
// FUNCTION: 读取流水灯显示亮度
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Read_FlowLED_Bright(void)
{
	// 根据当前级别, 读取显示亮度
	switch(R_SetPara_FlowLED_Bright_LV)
	{
		case C_SET_PARA_LV1:	R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV1;	break;
		case C_SET_PARA_LV2:	R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV2;	break;
		case C_SET_PARA_LV3:	R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV3;	break;
		case C_SET_PARA_LV4:	R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV4;	break;
		case C_SET_PARA_LV5:	R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV5;	break;
		case C_SET_PARA_LV6:	R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV6;	break;
		case C_SET_PARA_LV7:	R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV7;	break;
		case C_SET_PARA_LV8:	R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV8;	break;
		default:				R_FlowLED_DispBright = SET_FLOW_LED_DISP_BRIGHT_LV1;	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV1;	break;
	}
}


//==========================================================================
// FUNCTION: 读取流水灭延时时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Read_FlowLED_DelayOffTime(void)
{
	// 根据当前级别, 读取显示亮度
	switch(R_SetPara_FlowLED_DelayOffTime_LV)
	{
		case C_SET_PARA_LV1:	R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV1;	break;
		case C_SET_PARA_LV2:	R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV2;	break;
		case C_SET_PARA_LV3:	R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV3;	break;
		case C_SET_PARA_LV4:	R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV4;	break;
		case C_SET_PARA_LV5:	R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV5;	break;
		case C_SET_PARA_LV6:	R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV6;	break;
		case C_SET_PARA_LV7:	R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV7;	break;
		case C_SET_PARA_LV8:	R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV8;	break;
		default:				R_FlowLED_DelayTimer = SET_FLOW_LED_OFF_DELAY_TIME_LV1;	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV1;	break;
	}
}



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
void F_Chagne_StartLamp_TimeLV(void)
{
	// 改变当前级别
	switch(R_SetPara_StairLamp_Time_LV)
	{
		case C_SET_PARA_LV1:	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV2;	break;
		case C_SET_PARA_LV2:	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV3;	break;
		case C_SET_PARA_LV3:	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV4;	break;
		case C_SET_PARA_LV4:	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV5;	break;
		case C_SET_PARA_LV5:	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV6;	break;
		case C_SET_PARA_LV6:	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV7;	break;
		case C_SET_PARA_LV7:	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV8;	break;
		case C_SET_PARA_LV8:	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV1;	break;
		default:				R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV1;	break;
	}
	F_Read_StartLamp_Time();	// 读取楼梯灯流水时间
	F_Disp_StartLamp_Time_LV();	// 显示当前的级别
}


//==========================================================================
// FUNCTION: 改变楼梯灯显示亮度级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Chagne_StartLamp_BrightLV(void)
{
	// 改变当前级别
	switch(R_SetPara_StairLamp_Bright_LV)
	{
		case C_SET_PARA_LV1:	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV2;	break;
		case C_SET_PARA_LV2:	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV3;	break;
		case C_SET_PARA_LV3:	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV4;	break;
		case C_SET_PARA_LV4:	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV5;	break;
		case C_SET_PARA_LV5:	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV6;	break;
		case C_SET_PARA_LV6:	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV7;	break;
		case C_SET_PARA_LV7:	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV8;	break;
		case C_SET_PARA_LV8:	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV1;	break;
		default:				R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV1;	break;
	}
	F_Read_StartLamp_Bright();		// 读取楼梯灯显示亮度
	F_Disp_StartLamp_Bright_LV();	// 显示当前的级别
}


//==========================================================================
// FUNCTION: 改变流水灯显示亮度级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Chagne_StartLamp_DelayOffTimeLV(void)
{
	// 改变当前级别
	switch(R_SetPara_StairLamp_DelayOffTime_LV)
	{
		case C_SET_PARA_LV1:	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV2;	break;
		case C_SET_PARA_LV2:	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV3;	break;
		case C_SET_PARA_LV3:	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV4;	break;
		case C_SET_PARA_LV4:	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV5;	break;
		case C_SET_PARA_LV5:	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV6;	break;
		case C_SET_PARA_LV6:	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV7;	break;
		case C_SET_PARA_LV7:	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV8;	break;
		case C_SET_PARA_LV8:	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV1;	break;
		default:				R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV1;	break;
	}
	F_Read_StartLamp_DelayOffTime();	// 读取流水灯显示亮度
	F_Disp_StartLamp_DelayOffTime_LV();	// 显示当前的级别
}


//==========================================================================
// FUNCTION: 改变流水灯流水时间级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Chagne_FlowLED_TimeLV(void)
{
	// 改变当前级别
	switch(R_SetPara_FlowLED_Time_LV)
	{
		case C_SET_PARA_LV1:	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV2;		break;
		case C_SET_PARA_LV2:	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV3;		break;
		case C_SET_PARA_LV3:	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV4;		break;
		case C_SET_PARA_LV4:	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV5;		break;
		case C_SET_PARA_LV5:	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV6;		break;
		case C_SET_PARA_LV6:	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV7;		break;
		case C_SET_PARA_LV7:	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV8;		break;
		case C_SET_PARA_LV8:	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV1;		break;
		default:				R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV1;		break;
	}
	F_Read_FlowLED_Time();		// 读取流水灯流水时间
	F_Disp_FlowLED_Time_LV();	// 显示当前的级别
	
#ifdef SET_STAIR_LAMP_FLOW_TIME_FROM_FLOW_LED
	// 流水灯的流水时间改变, 重新计算楼梯灯的流水同步时间
	F_Read_StartLamp_Time();
#endif
}


//==========================================================================
// FUNCTION: 改变流水灯显示亮度级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Chagne_FlowLED_BrightLV(void)
{
	// 改变当前级别
	switch(R_SetPara_FlowLED_Bright_LV)
	{
		case C_SET_PARA_LV1:	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV2;	break;
		case C_SET_PARA_LV2:	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV3;	break;
		case C_SET_PARA_LV3:	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV4;	break;
		case C_SET_PARA_LV4:	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV5;	break;
		case C_SET_PARA_LV5:	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV6;	break;
		case C_SET_PARA_LV6:	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV7;	break;
		case C_SET_PARA_LV7:	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV8;	break;
		case C_SET_PARA_LV8:	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV1;	break;
		default:				R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV1;	break;
	}
	F_Read_FlowLED_Bright();	// 读取流水灯显示亮度
	F_Disp_FlowLED_Bright_LV();	// 显示当前的级别
}


//==========================================================================
// FUNCTION: 改变流水灯显示亮度级别
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Chagne_FlowLED_DelayOffTimeLV(void)
{
	// 改变当前级别
	switch(R_SetPara_FlowLED_DelayOffTime_LV)
	{
		case C_SET_PARA_LV1:	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV2;	break;
		case C_SET_PARA_LV2:	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV3;	break;
		case C_SET_PARA_LV3:	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV4;	break;
		case C_SET_PARA_LV4:	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV5;	break;
		case C_SET_PARA_LV5:	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV6;	break;
		case C_SET_PARA_LV6:	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV7;	break;
		case C_SET_PARA_LV7:	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV8;	break;
		case C_SET_PARA_LV8:	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV1;	break;
		default:				R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV1;	break;
	}
	F_Read_FlowLED_DelayOffTime();		// 读取流水灯显示亮度
	F_Disp_FlowLED_DelayOffTime_LV();	// 显示当前的级别
}


//==========================================================================
// FUNCTION: 存储设定的工作模式
// INPUT:    
//			 R_Run_Mode -- 存储系统工作模式
// OUTPUT:   NONE
//==========================================================================
void F_Save_System_RunMode(uint8_t R_Run_Mode)
{
	if(R_Run_Mode != R_SetPara_System_RunMode)
	{
		switch(R_Run_Mode)
		{
			case C_SYS_RUN_STD:
			case C_SYS_RUN_STAIR_TEST:
			case C_SYS_RUN_FLOW_TEST:
			case C_SYS_RUN_ALL_TEST:	R_SetPara_System_RunMode = R_Run_Mode;		break;
			default: 					R_SetPara_System_RunMode = C_SYS_RUN_STD;	break;
		}
		
		// 把参数存储到APROM
		*(uint32_t *)&RT_Safety_Flash.CRCPRT = 0x06050309;
		F_GenerateCRC_Seek(&RT_Safety_Flash.CRCPRT);
		R_SetPara_RAM.CheckSum = F_Get_CheckSum(&R_SetPara_RAM, sizeof(SYS_SET_PARA_T));
		if(0 != F_Save_Parameters(&RT_Safety_Flash, &R_SetPara_RAM))
		{
			// 存储参数失败
			__NOP();
		}
	}
}




//==============================================================================================================
//==============================================================================================================
// 根据指定的流水参照对象计算出相对应的流水目标的流水时间
//==============================================================================================================
//==============================================================================================================
#ifdef SET_STAIR_LAMP_FLOW_TIME_FROM_FLOW_LED


//==========================================================================
// FUNCTION: 根据FlowLED01的长度与流水时间, 计算出StairLamp的流水时间
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Set_StairLamp_FlowTime_From_FlowLED(void)
{
	uint32_t R_Total_FlowLED_Time = 0;
	
	// 读取流水灯的流水时间
	F_Read_FlowLED_Time();
	
	
	// 计算总的流水时间
	if(R_FlowLED_FlowNum % N_FLOW_LED01_DISP_STEP_NUM)
	{
		R_Total_FlowLED_Time  = (uint32_t)R_FlowLED_FlowTimer * (R_FlowLED_FlowNum / N_FLOW_LED01_DISP_STEP_NUM);
	}
	else
	{
		R_Total_FlowLED_Time  = (uint32_t)R_FlowLED_FlowTimer * (R_FlowLED_FlowNum / N_FLOW_LED01_DISP_STEP_NUM - 1);
	}
	
	// 计算楼梯灯的流水时间
	if(R_StairLamp_FlowNum % N_STAIR_LAMP_DISP_STEP_NUM)
	{
		R_StairLamp_FlowTimer = (uint16_t)(R_Total_FlowLED_Time * 10 / (R_StairLamp_FlowNum / N_STAIR_LAMP_DISP_STEP_NUM));	
	}
	else
	{
		R_StairLamp_FlowTimer = (uint16_t)(R_Total_FlowLED_Time * 10 / (R_StairLamp_FlowNum / N_STAIR_LAMP_DISP_STEP_NUM - 1));	
	}
	
	// 根据时间的小数点后一位进行四舍五入处理
	R_StairLamp_FlowTimer += 5;
	R_StairLamp_FlowTimer /= 10;
}

#endif






