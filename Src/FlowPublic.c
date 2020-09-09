//==========================================================================
// Function Define:
// void F_FlowOFF_Floor1_Service(void)
//==========================================================================
#include "Public.h"



//==========================================================================
// 流水渐变显示处理的状态
//==========================================================================
#if defined(STAIR_LAMP_DISP_FOR_PWM) || defined(FLOW_LED01_DISP_FOR_PWM)
uint8_t R_Disp_For_PWM_State[SET_FLOW_LED_FLOW_NUM_MAX];
#endif


//==========================================================================
// GS1903显示数据临时存储变量
//==========================================================================
uint8_t R_StairLamp_Disp_Data[SET_STAIR_LAMP_FLOW_NUM_MAX];		// 楼梯灯显示数据影像变量
uint8_t R_FlowLED01_Disp_Data[SET_FLOW_LED_FLOW_NUM_MAX];		// 流水灯01显示数据影像变量






//==========================================================================
// FUNCTION: 第一层楼梯灭灯处理服务程序
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_FlowOFF_Floor1_Service(void)
{
	//=========================================================
	// 定时处理
	//=========================================================
	if(C_TimerFlag_50ms_Bit & R_TimerFlag)
	{
		R_StairLamp_Para_Sen1A.DelayTime = (R_StairLamp_Para_Sen1A.DelayTime != 0) ? R_StairLamp_Para_Sen1A.DelayTime-1 : 0;
		R_StairLamp_Para_Sen1B.DelayTime = (R_StairLamp_Para_Sen1B.DelayTime != 0) ? R_StairLamp_Para_Sen1B.DelayTime-1 : 0;
		R_StairLamp_Para_Sen1C.DelayTime = (R_StairLamp_Para_Sen1C.DelayTime != 0) ? R_StairLamp_Para_Sen1C.DelayTime-1 : 0;
		R_StairLamp_Para_Sen2A.DelayTime = (R_StairLamp_Para_Sen2A.DelayTime != 0) ? R_StairLamp_Para_Sen2A.DelayTime-1 : 0;
		R_StairLamp_Para_Sen2B.DelayTime = (R_StairLamp_Para_Sen2B.DelayTime != 0) ? R_StairLamp_Para_Sen2B.DelayTime-1 : 0;
		R_StairLamp_Para_Sen2C.DelayTime = (R_StairLamp_Para_Sen2C.DelayTime != 0) ? R_StairLamp_Para_Sen2C.DelayTime-1 : 0;
		//=====================================================
		R_FlowLED01_Para_Sen1A.DelayTime = (R_FlowLED01_Para_Sen1A.DelayTime != 0) ? R_FlowLED01_Para_Sen1A.DelayTime-1 : 0;
		R_FlowLED01_Para_Sen1B.DelayTime = (R_FlowLED01_Para_Sen1B.DelayTime != 0) ? R_FlowLED01_Para_Sen1B.DelayTime-1 : 0;
		R_FlowLED01_Para_Sen1C.DelayTime = (R_FlowLED01_Para_Sen1C.DelayTime != 0) ? R_FlowLED01_Para_Sen1C.DelayTime-1 : 0;
		R_FlowLED01_Para_Sen2A.DelayTime = (R_FlowLED01_Para_Sen2A.DelayTime != 0) ? R_FlowLED01_Para_Sen2A.DelayTime-1 : 0;
		R_FlowLED01_Para_Sen2B.DelayTime = (R_FlowLED01_Para_Sen2B.DelayTime != 0) ? R_FlowLED01_Para_Sen2B.DelayTime-1 : 0;
		R_FlowLED01_Para_Sen2C.DelayTime = (R_FlowLED01_Para_Sen2C.DelayTime != 0) ? R_FlowLED01_Para_Sen2C.DelayTime-1 : 0;
	}
	
	
	//=========================================================
	// 灭灯条件判断处理
	//=========================================================
	if( ((R_StairLamp_Para_Sen1B.State == 0) && (R_StairLamp_Para_Sen2B.State == 0))
	 && ((R_FlowLED01_Para_Sen1B.State == 0) && (R_FlowLED01_Para_Sen2B.State == 0)) )
	{
		if( ((R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_ON) && (R_StairLamp_Para_Sen2A.State == C_STAIR_LAMP_ON))
		 && ((R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_ON) && (R_FlowLED01_Para_Sen2A.State == C_FLOW_LED01_ON)) )
		{
			if( ((R_StairLamp_Para_Sen1A.DelayTime == 0) && (R_StairLamp_Para_Sen1A.Cnt >= R_StairLamp_FlowNum))
			 && ((R_StairLamp_Para_Sen2A.DelayTime == 0) && (R_StairLamp_Para_Sen2A.Cnt >= R_StairLamp_FlowNum))
			 && ((R_FlowLED01_Para_Sen1A.DelayTime == 0) && (R_FlowLED01_Para_Sen1A.Cnt >= R_FlowLED_FlowNum))
			 && ((R_FlowLED01_Para_Sen2A.DelayTime == 0) && (R_FlowLED01_Para_Sen2A.Cnt >= R_FlowLED_FlowNum)) )
			{
				// 从楼梯底和楼梯顶同时渐灭处理
				R_StairLamp_Para_Sen1A.Cnt = 0;
				R_StairLamp_Para_Sen1A.DelayTime = 0;
				R_StairLamp_Para_Sen1A.State = C_STAIR_LAMP_OFF;
				R_StairLamp_Para_Sen2A.Cnt = 0;
				R_StairLamp_Para_Sen2A.DelayTime = 0;
				R_StairLamp_Para_Sen2A.State = C_STAIR_LAMP_OFF;
				// 从楼梯底和楼梯顶同时渐灭处理
				R_FlowLED01_Para_Sen1A.Cnt = 0;
				R_FlowLED01_Para_Sen1A.DelayTime = 0;
				R_FlowLED01_Para_Sen1A.State = C_FLOW_LED01_OFF;
				R_FlowLED01_Para_Sen2A.Cnt = 0;
				R_FlowLED01_Para_Sen2A.DelayTime = 0;
				R_FlowLED01_Para_Sen2A.State = C_FLOW_LED01_OFF;
			}
		}
		else if((R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_ON)
			 && (R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_ON))
		{
			if( ((R_StairLamp_Para_Sen1A.DelayTime == 0) && (R_StairLamp_Para_Sen1A.Cnt >= R_StairLamp_FlowNum))
			 && ((R_FlowLED01_Para_Sen1A.DelayTime == 0) && (R_FlowLED01_Para_Sen1A.Cnt >= R_FlowLED_FlowNum)) )
			{
				// 从楼梯底渐灭处理
				R_StairLamp_Para_Sen1A.Cnt = 0;
				R_StairLamp_Para_Sen1A.DelayTime = 0;
				R_StairLamp_Para_Sen1A.State = C_STAIR_LAMP_OFF;
				// 从楼梯底渐灭处理
				R_FlowLED01_Para_Sen1A.Cnt = 0;
				R_FlowLED01_Para_Sen1A.DelayTime = 0;
				R_FlowLED01_Para_Sen1A.State = C_FLOW_LED01_OFF;
			}
		}
		else if((R_StairLamp_Para_Sen2A.State == C_STAIR_LAMP_ON)
			 && (R_FlowLED01_Para_Sen2A.State == C_FLOW_LED01_ON))
		{
			if( ((R_StairLamp_Para_Sen2A.DelayTime == 0) && (R_StairLamp_Para_Sen2A.Cnt >= R_StairLamp_FlowNum))
			 && ((R_FlowLED01_Para_Sen2A.DelayTime == 0) && (R_FlowLED01_Para_Sen2A.Cnt >= R_FlowLED_FlowNum)) )
			{
				// 从楼梯顶渐灭处理
				R_StairLamp_Para_Sen2A.Cnt = 0;
				R_StairLamp_Para_Sen2A.DelayTime = 0;
				R_StairLamp_Para_Sen2A.State = C_STAIR_LAMP_OFF;
				// 从楼梯顶渐灭处理
				R_FlowLED01_Para_Sen2A.Cnt = 0;
				R_FlowLED01_Para_Sen2A.DelayTime = 0;
				R_FlowLED01_Para_Sen2A.State = C_FLOW_LED01_OFF;
			}
		}
		else
		{
		}
	}
}




