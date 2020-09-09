//==========================================================================
// Function Define:
// void F_StairLamp_DispLED_On(uint32_t R_LED_Num, uint8_t R_LED_Bright)
// void F_StairLamp_DispLED_Off(uint32_t R_LED_Num)
// void F_Disp_StairLamp_All(uint16_t R_DispNum, uint8_t R_DispDat)
// void F_StairLamp_PWM_TimeUpdate(void)
// void F_Refresh_StairLamp(void)
// void F_StairLamp_Service(void)
// void F_StairLamp_Init(void)
//==========================================================================
#include "Public.h"


// 楼梯灯显示标志
uint8_t R_StairLamp_Disp_Flag = 0;


//==========================================================================
// FUNCTION: 打开指定位置的LED灯显示
// INPUT:    
//			 R_LED_Num    -- 显示LED的位置
//			 R_LED_Bright -- 显示LED的亮度
// OUTPUT:   NONE
//==========================================================================
void F_StairLamp_DispLED_On(uint32_t R_LED_Num, uint8_t R_LED_Bright)
{
#ifdef STAIR_LAMP_DISP_FOR_PWM
	R_Disp_For_PWM_State[R_LED_Num] |= C_STAIR_LAMP_DISP_FOR_PWM_ON;	// 渐变显示
#else
	R_StairLamp_Disp_Data[R_LED_Num] = R_LED_Bright;	// 直接显示
	Set_StairLamp_Disp_Updata();	// 显示数据变化, 更新显示
#endif
}


//==========================================================================
// FUNCTION: 关闭指定位置的LED灯显示
// INPUT:    
//			 R_LED_Num -- 关闭LED的位置
// OUTPUT:   NONE
//==========================================================================
void F_StairLamp_DispLED_Off(uint32_t R_LED_Num)
{
#ifdef STAIR_LAMP_DISP_FOR_PWM
	R_Disp_For_PWM_State[R_LED_Num] &= ~C_STAIR_LAMP_DISP_FOR_PWM_ON;	// 渐变显示
#else
	R_StairLamp_Disp_Data[R_LED_Num] = 0x00;	// 直接显示
	Set_StairLamp_Disp_Updata();	// 显示数据变化, 更新显示
#endif
}


//==========================================================================
// FUNCTION: 楼梯灯所有显示一致的数据
// INPUT:    
//			 R_DispNum -- 显示的数量
//			 R_DispDat -- 显示的数据
// OUTPUT:   NONE
//==========================================================================
void F_Disp_StairLamp_All(uint16_t R_DispNum, uint8_t R_DispDat)
{
	uint16_t i;
	
	if(R_DispNum > SET_STAIR_LAMP_FLOW_NUM_MAX)
	{
		R_DispNum = SET_STAIR_LAMP_FLOW_NUM_MAX;
	}
	
	for(i=0; i<SET_STAIR_LAMP_FLOW_NUM_MAX; i++)
	{
		R_StairLamp_Disp_Data[i] = (i < R_DispNum) ? R_DispDat : 0x00;
	}
	Set_StairLamp_Disp_Updata();	// 显示数据变化, 更新显示
}


//==========================================================================
// FUNCTION: PWM占空比的DUTY值更新
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_StairLamp_PWM_TimeUpdate(void)
{
#ifdef STAIR_LAMP_DISP_FOR_PWM
	uint32_t i = 0;
	
	if(Be_StairLamp_PWM_Time_Updata())
	{
		// 流水灯PWM输出DUTY值更新
		Clr_StairLamp_PWM_Time_Updata();
		for(i=0; i<SET_STAIR_LAMP_FLOW_NUM_MAX; i++)
		{
			if(C_STAIR_LAMP_DISP_FOR_PWM_ON & R_Disp_For_PWM_State[i])
			{
				// 渐亮处理
				if(R_StairLamp_Disp_Data[i] < R_StairLamp_DispBright)
				{
					if(R_StairLamp_DispBright < N_STAIR_LAMP_DISP_PWM_STEP)
					{
						R_StairLamp_Disp_Data[i] = R_StairLamp_DispBright;
					}
					else if(R_StairLamp_Disp_Data[i] < (R_StairLamp_DispBright - N_STAIR_LAMP_DISP_PWM_STEP))
					{
						R_StairLamp_Disp_Data[i] += N_STAIR_LAMP_DISP_PWM_STEP;
					}
					else
					{
						R_StairLamp_Disp_Data[i] = R_StairLamp_DispBright;
					}
					//R_StairLamp_Disp_Data[i]++;
					Set_StairLamp_Disp_Updata();
				}
				else if(R_StairLamp_Disp_Data[i] > R_StairLamp_DispBright)
				{
					if(R_StairLamp_Disp_Data[i] < N_STAIR_LAMP_DISP_PWM_STEP)
					{
						R_StairLamp_Disp_Data[i] = R_StairLamp_DispBright;
					}
					else if((R_StairLamp_Disp_Data[i] - N_STAIR_LAMP_DISP_PWM_STEP) > R_StairLamp_DispBright)
					{
						R_StairLamp_Disp_Data[i] -= N_STAIR_LAMP_DISP_PWM_STEP;
					}
					else
					{
						R_StairLamp_Disp_Data[i] = R_StairLamp_DispBright;
					}
					//R_StairLamp_Disp_Data[i]--;
					Set_StairLamp_Disp_Updata();
				}
			}
			else
			{
				// 渐灭处理
				if(R_StairLamp_Disp_Data[i] != 0)
				{
					if(R_StairLamp_Disp_Data[i] >= N_STAIR_LAMP_DISP_PWM_STEP)
					{
						R_StairLamp_Disp_Data[i] -= N_STAIR_LAMP_DISP_PWM_STEP;
					}
					else
					{
						R_StairLamp_Disp_Data[i] = 0;
					}
					//R_StairLamp_Disp_Data[i]--;
					Set_StairLamp_Disp_Updata();
				}
			}
		}
	}
#endif
}


//==========================================================================
// FUNCTION: 更新显示的数据
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Refresh_StairLamp(void)
{
	uint32_t i = 0;
	
#ifdef STAIR_LAMP_DISP_UPDATA_FOR_CHANGE
	if(Be_StairLamp_Disp_Updata())
#endif
	{
		// 清除更新数据标志
		Clr_StairLamp_Disp_Updata();
		
		__set_PRIMASK(1);	// 关中断
		for(i = 0; i < SET_STAIR_LAMP_FLOW_NUM_MAX; i++)
		{
			F_GS1903_1Din_SendData(P_GS1903_DIN_GPIO1, C_STAIR_DIN_BIT, R_StairLamp_Disp_Data[i]);
			F_GS1903_1Din_SendData(P_GS1903_DIN_GPIO1, C_STAIR_DIN_BIT, R_StairLamp_Disp_Data[i]);
			F_GS1903_1Din_SendData(P_GS1903_DIN_GPIO1, C_STAIR_DIN_BIT, R_StairLamp_Disp_Data[i]);
		}
		__set_PRIMASK(0);	// 开中断
	}
		
	//=====================================
	// Updata next time
	if(!Be_StairLamp_PWM_Time_Updata())
	{
		Set_StairLamp_PWM_Time_Updata();
	}
}


//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================

// 流水灯处理变量
STAIR_LAMP_PARA_T	R_StairLamp_Para_Sen1A;
STAIR_LAMP_PARA_T	R_StairLamp_Para_Sen1B;
STAIR_LAMP_PARA_T	R_StairLamp_Para_Sen1C;
STAIR_LAMP_PARA_T	R_StairLamp_Para_Sen2A;
STAIR_LAMP_PARA_T	R_StairLamp_Para_Sen2B;
STAIR_LAMP_PARA_T	R_StairLamp_Para_Sen2C;


//==========================================================================
// FUNCTION: 流水灯处理服务程序
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_StairLamp_Service(void)
{
	uint16_t i = 0;
	
	
	//=========================================================
	// 判断传感器,启动相应工作
	//=========================================================
	if(R_Sen_HisKey & C_KEY_SEN01)
	{
		// 传感器1有信号, 流水从1开始
		if(R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_ON)
		{
			// Sen1B启动流水显示开处理
			R_StairLamp_Para_Sen1B.State = C_STAIR_LAMP_ON;
			R_StairLamp_Para_Sen1B.Cnt = 0;
			R_StairLamp_Para_Sen1B.DelayTime = 0;
		}
		else if(R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_OFF)
		{
			// Sen1A正在流水关闭处理
			if(R_StairLamp_Para_Sen1B.State == 0)
			{
				R_StairLamp_Para_Sen1B.State = C_STAIR_LAMP_ON;
				R_StairLamp_Para_Sen1B.Cnt = 0;
				R_StairLamp_Para_Sen1B.DelayTime = 0;
			}
			else if(R_StairLamp_Para_Sen1B.State == C_STAIR_LAMP_ON)
			{
				R_StairLamp_Para_Sen1C.State = C_STAIR_LAMP_ON;
				R_StairLamp_Para_Sen1C.Cnt = 0;
				R_StairLamp_Para_Sen1C.DelayTime = 0;
			}
			else
			{
			}
		}
		else
		{
			// Sen1A启动流水显示开处理
			R_StairLamp_Para_Sen1A.State = C_STAIR_LAMP_ON;
			R_StairLamp_Para_Sen1A.Cnt = 0;
			R_StairLamp_Para_Sen1A.DelayTime = 0;
		}
	}
	if(R_Sen_HisKey & C_KEY_SEN02)
	{
		// 传感器2有信号, 流水从32开始
		if(R_StairLamp_Para_Sen2A.State == C_STAIR_LAMP_ON)
		{
			// Sen2B启动流水显示开处理
			R_StairLamp_Para_Sen2B.State = C_STAIR_LAMP_ON;
			R_StairLamp_Para_Sen2B.Cnt = 0;
			R_StairLamp_Para_Sen2B.DelayTime = 0;
		}
		else if(R_StairLamp_Para_Sen2A.State == C_STAIR_LAMP_OFF)
		{
			// Sen2A正在流水关闭处理
			if(R_StairLamp_Para_Sen2B.State == 0)
			{
				R_StairLamp_Para_Sen2B.State = C_STAIR_LAMP_ON;
				R_StairLamp_Para_Sen2B.Cnt = 0;
				R_StairLamp_Para_Sen2B.DelayTime = 0;
			}
			else if(R_StairLamp_Para_Sen2B.State == C_STAIR_LAMP_ON)
			{
				R_StairLamp_Para_Sen2C.State = C_STAIR_LAMP_ON;
				R_StairLamp_Para_Sen2C.Cnt = 0;
				R_StairLamp_Para_Sen2C.DelayTime = 0;
			}
			else
			{
			}
		}
		else
		{
			// Sen2A启动流水显示开处理
			R_StairLamp_Para_Sen2A.State = C_STAIR_LAMP_ON;
			R_StairLamp_Para_Sen2A.Cnt = 0;
			R_StairLamp_Para_Sen2A.DelayTime = 0;
		}
	}
	
	
	//=========================================================
	// Sen1A显示处理
	if((0 != R_StairLamp_Para_Sen1A.State) && (0 == R_StairLamp_Para_Sen1A.DelayTime))
	{		
		if(R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_ON)
		{
			for(i = 0; i < N_STAIR_LAMP_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_StairLamp_Para_Sen1A.Cnt < R_StairLamp_FlowNum)
				{
					// 继续显示下一个
					F_StairLamp_DispLED_On(R_StairLamp_Para_Sen1A.Cnt, R_StairLamp_DispBright);
					R_StairLamp_Para_Sen1A.Cnt++;
					R_StairLamp_Para_Sen1A.DelayTime = R_StairLamp_FlowTimer;
					
					// 流水亮完成
					if(R_StairLamp_Para_Sen1A.Cnt >= R_StairLamp_FlowNum)
					{
						R_StairLamp_Para_Sen1A.DelayTime = R_StairLamp_DelayTimer;	// 流水完成, 延时灭处理
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if(R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_OFF)
		{
			for(i = 0; i < N_STAIR_LAMP_DISP_STEP_NUM; i++)
			{
				if(R_StairLamp_Para_Sen1A.Cnt < R_StairLamp_FlowNum)
				{
					// 流水渐灭处理
					if( (R_StairLamp_Para_Sen2A.State == C_STAIR_LAMP_ON) && (R_StairLamp_Para_Sen1A.Cnt >= (R_StairLamp_FlowNum - 1 - R_StairLamp_Para_Sen2A.Cnt)) )
					{
						// SNE2A正在流水开, 相遇后不关灯
					}
					else if( (R_StairLamp_Para_Sen2B.State == C_STAIR_LAMP_ON) && (R_StairLamp_Para_Sen1A.Cnt >= (R_StairLamp_FlowNum - 1 - R_StairLamp_Para_Sen2B.Cnt)) )
					{
						// SNE2B正在流水开, 相遇后不关灯
					}
					else
					{
						// 继续关闭下一个
						F_StairLamp_DispLED_Off(R_StairLamp_Para_Sen1A.Cnt);
					}
					R_StairLamp_Para_Sen1A.Cnt++;
					R_StairLamp_Para_Sen1A.DelayTime = R_StairLamp_FlowTimer;
				}
				else
				{
					break;
				}
			}
		}	
		
		//==================================
		// 流水数据转移
		//==================================
		if((R_StairLamp_Para_Sen1A.Cnt >= R_StairLamp_FlowNum) && (R_StairLamp_Para_Sen1A.DelayTime == 0))
		{
			if((R_StairLamp_Para_Sen1A.State != C_STAIR_LAMP_IDLE) && (R_StairLamp_Para_Sen1B.State != C_STAIR_LAMP_IDLE))
			{
				// 流水亮完成, 数据转换
				R_StairLamp_Para_Sen1A = R_StairLamp_Para_Sen1B;
				R_StairLamp_Para_Sen1B = R_StairLamp_Para_Sen1C;
				//=============================
				R_StairLamp_Para_Sen1C.State     = 0;
				R_StairLamp_Para_Sen1C.Cnt       = 0;
				R_StairLamp_Para_Sen1C.DelayTime = 0;
			}
			else if((R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_OFF) && (R_StairLamp_Para_Sen1B.State == C_STAIR_LAMP_IDLE))
			{
				R_StairLamp_Para_Sen1A.State     = 0;
				R_StairLamp_Para_Sen1A.Cnt       = 0;
				R_StairLamp_Para_Sen1A.DelayTime = 0;
			}
		}
	}
	//=========================================================
	// Sen1B显示处理
	if((0 != R_StairLamp_Para_Sen1B.State) && (0 == R_StairLamp_Para_Sen1B.DelayTime))
	{
		if(R_StairLamp_Para_Sen1B.State == C_STAIR_LAMP_ON)
		{
			for(i = 0; i < N_STAIR_LAMP_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_StairLamp_Para_Sen1B.Cnt < R_StairLamp_FlowNum)
				{
					// 继续显示下一个
					F_StairLamp_DispLED_On(R_StairLamp_Para_Sen1B.Cnt, R_StairLamp_DispBright);
					R_StairLamp_Para_Sen1B.Cnt++;
					R_StairLamp_Para_Sen1B.DelayTime = R_StairLamp_FlowTimer;
					
					// 流水亮完成
					if(R_StairLamp_Para_Sen1B.Cnt >= R_StairLamp_FlowNum)
					{
						R_StairLamp_Para_Sen1B.DelayTime = R_StairLamp_DelayTimer;	// 流水完成, 延时灭处理
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	//=========================================================
	// Sen1C显示处理
	if((0 != R_StairLamp_Para_Sen1C.State) && (0 == R_StairLamp_Para_Sen1C.DelayTime))
	{
		if(R_StairLamp_Para_Sen1C.State == C_STAIR_LAMP_ON)
		{
			for(i = 0; i < N_STAIR_LAMP_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_StairLamp_Para_Sen1C.Cnt < R_StairLamp_FlowNum)
				{
					// 继续显示下一个
					//F_StairLamp_DispLED_On(R_StairLamp_Para_Sen1C.Cnt, R_StairLamp_DispBright);	// Sen1B在流水显示, Sen1C无需显示处理
					R_StairLamp_Para_Sen1C.Cnt++;
					R_StairLamp_Para_Sen1C.DelayTime = R_StairLamp_FlowTimer;
					
					// 流水亮完成
					if(R_StairLamp_Para_Sen1C.Cnt >= R_StairLamp_FlowNum)
					{
						R_StairLamp_Para_Sen1C.DelayTime = R_StairLamp_DelayTimer;	// 流水完成, 延时灭处理
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	
	
	//=========================================================
	// Sen2A显示处理
	if((0 != R_StairLamp_Para_Sen2A.State) && (0 == R_StairLamp_Para_Sen2A.DelayTime))
	{
		if(R_StairLamp_Para_Sen2A.State == C_STAIR_LAMP_ON)
		{
			for(i = 0; i < N_STAIR_LAMP_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_StairLamp_Para_Sen2A.Cnt < R_StairLamp_FlowNum)
				{
					// 继续显示下一个
					F_StairLamp_DispLED_On(R_StairLamp_FlowNum - 1 - R_StairLamp_Para_Sen2A.Cnt, R_StairLamp_DispBright);
					R_StairLamp_Para_Sen2A.Cnt++;
					R_StairLamp_Para_Sen2A.DelayTime = R_StairLamp_FlowTimer;
					
					// 流水亮完成
					if(R_StairLamp_Para_Sen2A.Cnt >= R_StairLamp_FlowNum)
					{
						R_StairLamp_Para_Sen2A.DelayTime = R_StairLamp_DelayTimer;	// 流水完成, 延时灭处理
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if(R_StairLamp_Para_Sen2A.State == C_STAIR_LAMP_OFF)
		{
			for(i = 0; i < N_STAIR_LAMP_DISP_STEP_NUM; i++)
			{
				// 流水渐灭处理
				if(R_StairLamp_Para_Sen2A.Cnt < R_StairLamp_FlowNum)
				{
					// 流水渐灭处理
					if( (R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_ON) && ((R_StairLamp_FlowNum - 1 - R_StairLamp_Para_Sen2A.Cnt) <= R_StairLamp_Para_Sen1A.Cnt) )
					{
						// SNE1A正在流水开, 相遇后不关灯
					}
					else if( (R_StairLamp_Para_Sen1B.State == C_STAIR_LAMP_ON) && ((R_StairLamp_FlowNum - 1 - R_StairLamp_Para_Sen2A.Cnt) <= R_StairLamp_Para_Sen1B.Cnt) )
					{
						// SNE1B正在流水开, 相遇后不关灯
					}
					else
					{
						// 继续关闭下一个
						F_StairLamp_DispLED_Off(R_StairLamp_FlowNum - 1 - R_StairLamp_Para_Sen2A.Cnt);
					}
					R_StairLamp_Para_Sen2A.Cnt++;
					R_StairLamp_Para_Sen2A.DelayTime = R_StairLamp_FlowTimer;
				}
				else
				{
					break;
				}
			}
		}
		
		//==================================
		// 流水数据转移
		//==================================
		if((R_StairLamp_Para_Sen2A.Cnt >= R_StairLamp_FlowNum) && (R_StairLamp_Para_Sen2A.DelayTime == 0))
		{
			if((R_StairLamp_Para_Sen2A.State != C_STAIR_LAMP_IDLE) && (R_StairLamp_Para_Sen2B.State != C_STAIR_LAMP_IDLE))
			{
				// 流水亮完成, 数据转换
				R_StairLamp_Para_Sen2A = R_StairLamp_Para_Sen2B;
				R_StairLamp_Para_Sen2B = R_StairLamp_Para_Sen2C;
				//=============================
				R_StairLamp_Para_Sen2C.State     = 0;
				R_StairLamp_Para_Sen2C.Cnt       = 0;
				R_StairLamp_Para_Sen2C.DelayTime = 0;
			}
			else if((R_StairLamp_Para_Sen2A.State == C_STAIR_LAMP_OFF) && (R_StairLamp_Para_Sen2B.State == C_STAIR_LAMP_IDLE))
			{
				R_StairLamp_Para_Sen2A.State     = 0;
				R_StairLamp_Para_Sen2A.Cnt       = 0;
				R_StairLamp_Para_Sen2A.DelayTime = 0;
			}
		}
	}
	//=========================================================
	// Sen2B显示处理
	if((0 != R_StairLamp_Para_Sen2B.State) && (0 == R_StairLamp_Para_Sen2B.DelayTime))
	{
		if(R_StairLamp_Para_Sen2B.State == C_STAIR_LAMP_ON)
		{
			for(i = 0; i < N_STAIR_LAMP_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_StairLamp_Para_Sen2B.Cnt < R_StairLamp_FlowNum)
				{
					// 继续显示下一个
					F_StairLamp_DispLED_On(R_StairLamp_FlowNum - 1 - R_StairLamp_Para_Sen2B.Cnt, R_StairLamp_DispBright);
					R_StairLamp_Para_Sen2B.Cnt++;
					R_StairLamp_Para_Sen2B.DelayTime = R_StairLamp_FlowTimer;
					
					// 流水亮完成
					if(R_StairLamp_Para_Sen2B.Cnt >= R_StairLamp_FlowNum)
					{
						R_StairLamp_Para_Sen2B.DelayTime = R_StairLamp_DelayTimer;	// 流水完成, 延时灭处理
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	//=========================================================
	// Sen2C显示处理
	if((0 != R_StairLamp_Para_Sen2C.State) && (0 == R_StairLamp_Para_Sen2C.DelayTime))
	{
		if(R_StairLamp_Para_Sen2C.State == C_STAIR_LAMP_ON)
		{
			for(i = 0; i < N_STAIR_LAMP_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_StairLamp_Para_Sen2C.Cnt < R_StairLamp_FlowNum)
				{
					// 继续显示下一个
					//F_StairLamp_DispLED_On(R_StairLamp_FlowNum - 1 - R_StairLamp_Para_Sen2C.Cnt, R_StairLamp_DispBright);	// Sen1B在流水显示, Sen1C无需显示处理
					R_StairLamp_Para_Sen2C.Cnt++;
					R_StairLamp_Para_Sen2C.DelayTime = R_StairLamp_FlowTimer;
					
					// 流水亮完成
					if(R_StairLamp_Para_Sen2C.Cnt >= R_StairLamp_FlowNum)
					{
						R_StairLamp_Para_Sen2C.DelayTime = R_StairLamp_DelayTimer;	// 流水完成, 延时灭处理
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
}


//==========================================================================
// FUNCTION: 重置楼梯灯处理变量
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_StairLamp_Init(void)
{
	uint16_t i = 0;
	STAIR_LAMP_PARA_T R_RST_Para_SEN;
	
	for(i=0; i<SET_STAIR_LAMP_FLOW_NUM_MAX; i++)
	{
		R_StairLamp_Disp_Data[i] = 0;
	#ifdef STAIR_LAMP_DISP_FOR_PWM
		R_Disp_For_PWM_State[i] &= ~C_STAIR_LAMP_DISP_FOR_PWM_ON;
	#endif
	}
	R_StairLamp_Disp_Flag = 0;
	
	// 清除当前的工作状态
	R_RST_Para_SEN.State = 0;
	R_RST_Para_SEN.DelayTime = 0;
	R_RST_Para_SEN.Cnt = 0;
	R_StairLamp_Para_Sen1A = R_RST_Para_SEN;
	R_StairLamp_Para_Sen1B = R_RST_Para_SEN;
	R_StairLamp_Para_Sen1C = R_RST_Para_SEN;
	R_StairLamp_Para_Sen2A = R_RST_Para_SEN;
	R_StairLamp_Para_Sen2B = R_RST_Para_SEN;
	R_StairLamp_Para_Sen2C = R_RST_Para_SEN;
	
	// 显示数据变化, 更新显示
	Set_StairLamp_Disp_Updata();
}



//==============================================================================================================
//==============================================================================================================
// 演示功能
//==============================================================================================================
//==============================================================================================================

// 流水显示的数据
const uint8_t T_StairTest_BinghtTab[] = {250, 100,  50, 30, 10, 0, 0, 0};
// 流水显示的数据长度
const uint8_t R_StairTest_Len = sizeof(T_StairTest_BinghtTab) / sizeof(T_StairTest_BinghtTab[0]);
// 流水显示计数
uint8_t R_StairTest_Cnt = 0;


//==========================================================================
// FUNCTION: 楼梯灯测试开始
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_StairTest_Start(void)
{
	R_StairLamp_Para_Sen1A.State = C_STAIR_LAMP_ON;
	R_StairLamp_Para_Sen1A.Cnt = 0;
	R_StairLamp_Para_Sen1A.DelayTime = 0;
	R_StairTest_Cnt = 0;
}


//==========================================================================
// FUNCTION: 楼梯灯测试停止
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_StairTest_Stop(void)
{
	R_StairLamp_Para_Sen1A.State = C_STAIR_LAMP_IDLE;
	R_StairLamp_Para_Sen1A.Cnt = 0;
	R_StairLamp_Para_Sen1A.DelayTime = 0;
	R_StairTest_Cnt = 0;
}
	

//==========================================================================
// FUNCTION: 楼梯灯测试处理服务程序
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_StairTest_Service(void)
{
	uint16_t i = 0;
	uint16_t j = 0;
	
	//=========================================================
	// 流水显示处理
	//=========================================================
	// 定时处理
	if(C_TimerFlag_50ms_Bit & R_TimerFlag)
	{
		R_StairLamp_Para_Sen1A.DelayTime = (R_StairLamp_Para_Sen1A.DelayTime != 0) ? R_StairLamp_Para_Sen1A.DelayTime-1 : 0;
	}
	
	
	//=========================================================
	// Sen1A显示处理
	//=========================================================
	if((R_StairLamp_Para_Sen1A.State == C_STAIR_LAMP_ON) && (0 == R_StairLamp_Para_Sen1A.DelayTime))
	{
		// 流水渐亮处理
		if(R_StairLamp_Para_Sen1A.Cnt < R_StairLamp_FlowNum)
		{
			// 把上次的数据进行移位
			j = R_StairLamp_FlowNum - 1;
			for(i = 0; i < j; i++)
			{
				R_StairLamp_Disp_Data[j-i] = R_StairLamp_Disp_Data[j-1-i];
			}
			// 增加显示数据
			R_StairLamp_Disp_Data[0] = T_StairTest_BinghtTab[R_StairTest_Cnt];
			R_StairTest_Cnt = (R_StairTest_Cnt < (R_StairTest_Len - 1)) ? (R_StairTest_Cnt + 1) : 0;
			Set_StairLamp_Disp_Updata();	// 显示数据变化, 更新显示
			//R_StairLamp_Para_Sen1A.DelayTime = R_StairLamp_FlowTimer;
			switch(R_SetPara_StairLamp_Time_LV)
			{
				case C_SET_PARA_LV1:	R_StairLamp_Para_Sen1A.DelayTime = 1;	break;
				case C_SET_PARA_LV2:	R_StairLamp_Para_Sen1A.DelayTime = 2;	break;
				case C_SET_PARA_LV3:	R_StairLamp_Para_Sen1A.DelayTime = 3;	break;
				case C_SET_PARA_LV4:	R_StairLamp_Para_Sen1A.DelayTime = 4;	break;
				case C_SET_PARA_LV5:	R_StairLamp_Para_Sen1A.DelayTime = 5;	break;
				case C_SET_PARA_LV6:	R_StairLamp_Para_Sen1A.DelayTime = 6;	break;
				case C_SET_PARA_LV7:	R_StairLamp_Para_Sen1A.DelayTime = 7;	break;
				case C_SET_PARA_LV8:	R_StairLamp_Para_Sen1A.DelayTime = 8;	break;
				default:				R_StairLamp_Para_Sen1A.DelayTime = 1;	break;
			}
		}
	}
}













