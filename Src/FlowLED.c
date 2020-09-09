//==========================================================================
// Function Define:
// void F_FlowLED01_DispLED_On(uint32_t R_LED_Num, uint8_t R_LED_Bright)
// void F_FlowLED01_DispLED_Off(uint32_t R_LED_Num)
// void F_Disp_FlowLED01_All(uint16_t R_DispNum, uint8_t R_DispDat)
// void F_FlowLED01_PWM_TimeUpdate(void)
// void F_Refresh_FlolwLED01(void)
// void F_FlowLED01_Service(void)
// void F_FlowLED01_Init(void)
//==========================================================================
#include "Public.h"


// 流水灯01显示标志
uint8_t R_FlowLED01_Disp_Flag = 0;



//==========================================================================
// FUNCTION: 打开指定位置的LED灯显示
// INPUT:    
//			 R_LED_Num    -- 显示LED的位置
//			 R_LED_Bright -- 显示LED的亮度
// OUTPUT:   NONE
//==========================================================================
void F_FlowLED01_DispLED_On(uint32_t R_LED_Num, uint8_t R_LED_Bright)
{
#ifdef FLOW_LED01_DISP_FOR_PWM
	R_Disp_For_PWM_State[R_LED_Num] |= C_FLOW_LED01_DISP_FOR_PWM_ON;	// 渐变显示
#else
	R_FlowLED01_Disp_Data[R_LED_Num] = R_LED_Bright;	// 直接显示
	Set_FlowLED01_Disp_Updata();	// 显示数据变化, 更新显示
#endif
}


//==========================================================================
// FUNCTION: 关闭指定位置的LED灯显示
// INPUT:    
//			 R_LED_Num -- 关闭LED的位置
// OUTPUT:   NONE
//==========================================================================
void F_FlowLED01_DispLED_Off(uint32_t R_LED_Num)
{
#ifdef FLOW_LED01_DISP_FOR_PWM
	R_Disp_For_PWM_State[R_LED_Num] &= ~C_FLOW_LED01_DISP_FOR_PWM_ON;	// 渐变显示
#else
	R_FlowLED01_Disp_Data[R_LED_Num] = 0x00;	// 直接显示
	Set_FlowLED01_Disp_Updata();	// 显示数据变化, 更新显示
#endif
}


//==========================================================================
// FUNCTION: 流水灯所有显示一致的数据
// INPUT:    
//			 R_DispNum -- 显示的数量
//			 R_DispDat -- 显示的数据
// OUTPUT:   NONE
//==========================================================================
void F_Disp_FlowLED01_All(uint16_t R_DispNum, uint8_t R_DispDat)
{
	uint16_t i;
	
	if(R_DispNum > SET_FLOW_LED_FLOW_NUM_MAX)
	{
		R_DispNum = SET_FLOW_LED_FLOW_NUM_MAX;
	}
	
	for(i=0; i<SET_FLOW_LED_FLOW_NUM_MAX; i++)
	{
		R_FlowLED01_Disp_Data[i] = (i < R_DispNum) ? R_DispDat : 0x00;
	}
	Set_FlowLED01_Disp_Updata();	// 显示数据变化, 更新显示
}


//==========================================================================
// FUNCTION: PWM占空比的DUTY值更新
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_FlowLED01_PWM_TimeUpdate(void)
{
#ifdef FLOW_LED01_DISP_FOR_PWM
	uint32_t i = 0;
	
	if(Be_FlowLED01_PWM_Time_Updata())
	{
		// 流水灯PWM输出DUTY值更新
		Clr_FlowLED01_PWM_Time_Updata();
		for(i=0; i<SET_FLOW_LED_FLOW_NUM_MAX; i++)
		{
			if(C_FLOW_LED01_DISP_FOR_PWM_ON & R_Disp_For_PWM_State[i])
			{
				// 渐亮处理
				if(R_FlowLED01_Disp_Data[i] < R_FlowLED_DispBright)
				{
					if(R_FlowLED_DispBright < N_FLOW_LED01_DISP_PWM_STEP)
					{
						R_FlowLED01_Disp_Data[i] = R_FlowLED_DispBright;
					}
					else if(R_FlowLED01_Disp_Data[i] < (R_FlowLED_DispBright - N_FLOW_LED01_DISP_PWM_STEP))
					{
						R_FlowLED01_Disp_Data[i] += N_FLOW_LED01_DISP_PWM_STEP;
					}
					else
					{
						R_FlowLED01_Disp_Data[i] = R_FlowLED_DispBright;
					}
					//R_FlowLED01_Disp_Data[i]++;
					Set_FlowLED01_Disp_Updata();
				}
				else if(R_FlowLED01_Disp_Data[i] > R_FlowLED_DispBright)
				{
					if(R_FlowLED01_Disp_Data[i] < N_FLOW_LED01_DISP_PWM_STEP)
					{
						R_FlowLED01_Disp_Data[i] = R_FlowLED_DispBright;
					}
					else if((R_FlowLED01_Disp_Data[i] - N_FLOW_LED01_DISP_PWM_STEP) > R_FlowLED_DispBright)
					{
						R_FlowLED01_Disp_Data[i] -= N_FLOW_LED01_DISP_PWM_STEP;
					}
					else
					{
						R_FlowLED01_Disp_Data[i] = R_FlowLED_DispBright;
					}
					//R_FlowLED01_Disp_Data[i]--;
					Set_FlowLED01_Disp_Updata();
				}
			}
			else
			{
				// 渐灭处理
				if(R_FlowLED01_Disp_Data[i] != 0)
				{
					if(R_FlowLED01_Disp_Data[i] >= N_FLOW_LED01_DISP_PWM_STEP)
					{
						R_FlowLED01_Disp_Data[i] -= N_FLOW_LED01_DISP_PWM_STEP;
					}
					else
					{
						R_FlowLED01_Disp_Data[i] = 0;
					}
					//R_FlowLED01_Disp_Data[i]--;
					Set_FlowLED01_Disp_Updata();
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
void F_Refresh_FlolwLED01(void)
{
	uint32_t i = 0;
	
#ifdef FLOW_LED01_DISP_UPDATA_FOR_CHANGE
	if(Be_FlowLED01_Disp_Updata())
#endif
	{
		// 清除更新数据标志
		Clr_FlowLED01_Disp_Updata();
		
		__set_PRIMASK(1);	// 关中断
		for(i = 0; i < SET_FLOW_LED_FLOW_NUM_MAX; i++)
		{
			#if (SET_FLOW_LED_FLOW_NUM_MAX == N_FLOW_LED_GS1903_NUM_MAX)
				// 程序FlowLED输出按3个灯为单位输出显示(输出时,1个GS1903需要发送3次数据)
				F_GS1903_1Din_SendData(P_GS1903_DIN_GPIO1, C_FLOW01_DIN_BIT, R_FlowLED01_Disp_Data[i]);
				F_GS1903_1Din_SendData(P_GS1903_DIN_GPIO1, C_FLOW01_DIN_BIT, R_FlowLED01_Disp_Data[i]);
				F_GS1903_1Din_SendData(P_GS1903_DIN_GPIO1, C_FLOW01_DIN_BIT, R_FlowLED01_Disp_Data[i]);
			#elif (SET_FLOW_LED_FLOW_NUM_MAX == (N_FLOW_LED_GS1903_NUM_MAX * 3))
				// 程序FlowLED输出按1个灯为单位输出显示(输出时,1个GS1903只需发送1次数据)
				F_GS1903_1Din_SendData(P_GS1903_DIN_GPIO1, C_FLOW01_DIN_BIT, R_FlowLED01_Disp_Data[i]);
			#endif
		}
		__set_PRIMASK(0);	// 开中断
	}
		
	//=====================================
	// Updata next time
	if(!Be_FlowLED01_PWM_Time_Updata())
	{
		Set_FlowLED01_PWM_Time_Updata();
	}
}


//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================

// 流水灯处理变量
FLOW_LED01_PARA_T	R_FlowLED01_Para_Sen1A;
FLOW_LED01_PARA_T	R_FlowLED01_Para_Sen1B;
FLOW_LED01_PARA_T	R_FlowLED01_Para_Sen1C;
FLOW_LED01_PARA_T	R_FlowLED01_Para_Sen2A;
FLOW_LED01_PARA_T	R_FlowLED01_Para_Sen2B;
FLOW_LED01_PARA_T	R_FlowLED01_Para_Sen2C;


//==========================================================================
// FUNCTION: 流水灯处理服务程序
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_FlowLED01_Service(void)
{
	uint16_t i = 0;
	
	
	//=========================================================
	// 判断传感器,启动相应工作
	//=========================================================
	if(R_Sen_HisKey & C_KEY_SEN01)
	{
		// 传感器1有信号, 流水从1开始
		if(R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_ON)
		{
			// Sen1B启动流水显示开处理
			R_FlowLED01_Para_Sen1B.State = C_FLOW_LED01_ON;
			R_FlowLED01_Para_Sen1B.Cnt = 0;
			R_FlowLED01_Para_Sen1B.DelayTime = 0;
		}
		else if(R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_OFF)
		{
			// Sen1A正在流水关闭处理
			if(R_FlowLED01_Para_Sen1B.State == 0)
			{
				R_FlowLED01_Para_Sen1B.State = C_FLOW_LED01_ON;
				R_FlowLED01_Para_Sen1B.Cnt = 0;
				R_FlowLED01_Para_Sen1B.DelayTime = 0;
			}
			else if(R_FlowLED01_Para_Sen1B.State == C_FLOW_LED01_ON)
			{
				R_FlowLED01_Para_Sen1C.State = C_FLOW_LED01_ON;
				R_FlowLED01_Para_Sen1C.Cnt = 0;
				R_FlowLED01_Para_Sen1C.DelayTime = 0;
			}
			else
			{
			}
		}
		else
		{
			// Sen1A启动流水显示开处理
			R_FlowLED01_Para_Sen1A.State = C_FLOW_LED01_ON;
			R_FlowLED01_Para_Sen1A.Cnt = 0;
			R_FlowLED01_Para_Sen1A.DelayTime = 0;
		}
	}
	if(R_Sen_HisKey & C_KEY_SEN02)
	{
		// 传感器2有信号, 流水从32开始
		if(R_FlowLED01_Para_Sen2A.State == C_FLOW_LED01_ON)
		{
			// Sen2B启动流水显示开处理
			R_FlowLED01_Para_Sen2B.State = C_FLOW_LED01_ON;
			R_FlowLED01_Para_Sen2B.Cnt = 0;
			R_FlowLED01_Para_Sen2B.DelayTime = 0;
		}
		else if(R_FlowLED01_Para_Sen2A.State == C_FLOW_LED01_OFF)
		{
			// Sen2A正在流水关闭处理
			if(R_FlowLED01_Para_Sen2B.State == 0)
			{
				R_FlowLED01_Para_Sen2B.State = C_FLOW_LED01_ON;
				R_FlowLED01_Para_Sen2B.Cnt = 0;
				R_FlowLED01_Para_Sen2B.DelayTime = 0;
			}
			else if(R_FlowLED01_Para_Sen2B.State == C_FLOW_LED01_ON)
			{
				R_FlowLED01_Para_Sen2C.State = C_FLOW_LED01_ON;
				R_FlowLED01_Para_Sen2C.Cnt = 0;
				R_FlowLED01_Para_Sen2C.DelayTime = 0;
			}
			else
			{
			}
		}
		else
		{
			// Sen2A启动流水显示开处理
			R_FlowLED01_Para_Sen2A.State = C_FLOW_LED01_ON;
			R_FlowLED01_Para_Sen2A.Cnt = 0;
			R_FlowLED01_Para_Sen2A.DelayTime = 0;
		}
	}
	
	
	//=========================================================
	// Sen1A显示处理
	if((0 != R_FlowLED01_Para_Sen1A.State) && (0 == R_FlowLED01_Para_Sen1A.DelayTime))
	{		
		if(R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_ON)
		{
			for(i = 0; i < N_FLOW_LED01_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_FlowLED01_Para_Sen1A.Cnt < R_FlowLED_FlowNum)
				{
					// 继续显示下一个
					F_FlowLED01_DispLED_On(R_FlowLED01_Para_Sen1A.Cnt, R_FlowLED_DispBright);
					R_FlowLED01_Para_Sen1A.Cnt++;
					R_FlowLED01_Para_Sen1A.DelayTime = R_FlowLED_FlowTimer;
					
					// 流水亮完成
					if(R_FlowLED01_Para_Sen1A.Cnt >= R_FlowLED_FlowNum)
					{
						R_FlowLED01_Para_Sen1A.DelayTime = R_FlowLED_DelayTimer;	// 流水完成, 延时灭处理
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if(R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_OFF)
		{
			for(i = 0; i < N_FLOW_LED01_DISP_STEP_NUM; i++)
			{
				if(R_FlowLED01_Para_Sen1A.Cnt < R_FlowLED_FlowNum)
				{
					// 流水渐灭处理
					if( (R_FlowLED01_Para_Sen2A.State == C_FLOW_LED01_ON) && (R_FlowLED01_Para_Sen1A.Cnt >= (R_FlowLED_FlowNum - 1 - R_FlowLED01_Para_Sen2A.Cnt)) )
					{
						// SNE2A正在流水开, 相遇后不关灯
					}
					else if( (R_FlowLED01_Para_Sen2B.State == C_FLOW_LED01_ON) && (R_FlowLED01_Para_Sen1A.Cnt >= (R_FlowLED_FlowNum - 1 - R_FlowLED01_Para_Sen2B.Cnt)) )
					{
						// SNE2B正在流水开, 相遇后不关灯
					}
					else
					{
						// 继续关闭下一个
						F_FlowLED01_DispLED_Off(R_FlowLED01_Para_Sen1A.Cnt);
					}
					R_FlowLED01_Para_Sen1A.Cnt++;
					R_FlowLED01_Para_Sen1A.DelayTime = R_FlowLED_FlowTimer;
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
		if((R_FlowLED01_Para_Sen1A.Cnt >= R_FlowLED_FlowNum) && (R_FlowLED01_Para_Sen1A.DelayTime == 0))
		{
			if((R_FlowLED01_Para_Sen1A.State != C_FLOW_LED01_IDLE) && (R_FlowLED01_Para_Sen1B.State != C_FLOW_LED01_IDLE))
			{
				// 流水亮完成, 数据转换
				R_FlowLED01_Para_Sen1A = R_FlowLED01_Para_Sen1B;
				R_FlowLED01_Para_Sen1B = R_FlowLED01_Para_Sen1C;
				//=============================
				R_FlowLED01_Para_Sen1C.State     = 0;
				R_FlowLED01_Para_Sen1C.Cnt       = 0;
				R_FlowLED01_Para_Sen1C.DelayTime = 0;
			}
			else if((R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_OFF) && (R_FlowLED01_Para_Sen1B.State == C_FLOW_LED01_IDLE))
			{
				R_FlowLED01_Para_Sen1A.State     = 0;
				R_FlowLED01_Para_Sen1A.Cnt       = 0;
				R_FlowLED01_Para_Sen1A.DelayTime = 0;
			}
		}
	}
	//=========================================================
	// Sen1B显示处理
	if((0 != R_FlowLED01_Para_Sen1B.State) && (0 == R_FlowLED01_Para_Sen1B.DelayTime))
	{
		if(R_FlowLED01_Para_Sen1B.State == C_FLOW_LED01_ON)
		{
			for(i = 0; i < N_FLOW_LED01_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_FlowLED01_Para_Sen1B.Cnt < R_FlowLED_FlowNum)
				{
					// 继续显示下一个
					F_FlowLED01_DispLED_On(R_FlowLED01_Para_Sen1B.Cnt, R_FlowLED_DispBright);
					R_FlowLED01_Para_Sen1B.Cnt++;
					R_FlowLED01_Para_Sen1B.DelayTime = R_FlowLED_FlowTimer;
					
					// 流水亮完成
					if(R_FlowLED01_Para_Sen1B.Cnt >= R_FlowLED_FlowNum)
					{
						R_FlowLED01_Para_Sen1B.DelayTime = R_FlowLED_DelayTimer;	// 流水完成, 延时灭处理
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
	if((0 != R_FlowLED01_Para_Sen1C.State) && (0 == R_FlowLED01_Para_Sen1C.DelayTime))
	{
		if(R_FlowLED01_Para_Sen1C.State == C_FLOW_LED01_ON)
		{
			for(i = 0; i < N_FLOW_LED01_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_FlowLED01_Para_Sen1C.Cnt < R_FlowLED_FlowNum)
				{
					// 继续显示下一个
					//F_FlowLED01_DispLED_On(R_FlowLED01_Para_Sen1C.Cnt, R_FlowLED_DispBright);	// Sen1B在流水显示, Sen1C无需显示处理
					R_FlowLED01_Para_Sen1C.Cnt++;
					R_FlowLED01_Para_Sen1C.DelayTime = R_FlowLED_FlowTimer;
					
					// 流水亮完成
					if(R_FlowLED01_Para_Sen1C.Cnt >= R_FlowLED_FlowNum)
					{
						R_FlowLED01_Para_Sen1C.DelayTime = R_FlowLED_DelayTimer;	// 流水完成, 延时灭处理
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
	if((0 != R_FlowLED01_Para_Sen2A.State) && (0 == R_FlowLED01_Para_Sen2A.DelayTime))
	{
		if(R_FlowLED01_Para_Sen2A.State == C_FLOW_LED01_ON)
		{
			for(i = 0; i < N_FLOW_LED01_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_FlowLED01_Para_Sen2A.Cnt < R_FlowLED_FlowNum)
				{
					// 继续显示下一个
					F_FlowLED01_DispLED_On(R_FlowLED_FlowNum - 1 - R_FlowLED01_Para_Sen2A.Cnt, R_FlowLED_DispBright);
					R_FlowLED01_Para_Sen2A.Cnt++;
					R_FlowLED01_Para_Sen2A.DelayTime = R_FlowLED_FlowTimer;
					
					// 流水亮完成
					if(R_FlowLED01_Para_Sen2A.Cnt >= R_FlowLED_FlowNum)
					{
						R_FlowLED01_Para_Sen2A.DelayTime = R_FlowLED_DelayTimer;	// 流水完成, 延时灭处理
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if(R_FlowLED01_Para_Sen2A.State == C_FLOW_LED01_OFF)
		{
			for(i = 0; i < N_FLOW_LED01_DISP_STEP_NUM; i++)
			{
				// 流水渐灭处理
				if(R_FlowLED01_Para_Sen2A.Cnt < R_FlowLED_FlowNum)
				{
					// 流水渐灭处理
					if( (R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_ON) && ((R_FlowLED_FlowNum - 1 - R_FlowLED01_Para_Sen2A.Cnt) <= R_FlowLED01_Para_Sen1A.Cnt) )
					{
						// SNE1A正在流水开, 相遇后不关灯
					}
					else if( (R_FlowLED01_Para_Sen1B.State == C_FLOW_LED01_ON) && ((R_FlowLED_FlowNum - 1 - R_FlowLED01_Para_Sen2A.Cnt) <= R_FlowLED01_Para_Sen1B.Cnt) )
					{
						// SNE1B正在流水开, 相遇后不关灯
					}
					else
					{
						// 继续关闭下一个
						F_FlowLED01_DispLED_Off(R_FlowLED_FlowNum - 1 - R_FlowLED01_Para_Sen2A.Cnt);
					}
					R_FlowLED01_Para_Sen2A.Cnt++;
					R_FlowLED01_Para_Sen2A.DelayTime = R_FlowLED_FlowTimer;
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
		if((R_FlowLED01_Para_Sen2A.Cnt >= R_FlowLED_FlowNum) && (R_FlowLED01_Para_Sen2A.DelayTime == 0))
		{
			if((R_FlowLED01_Para_Sen2A.State != C_FLOW_LED01_IDLE) && (R_FlowLED01_Para_Sen2B.State != C_FLOW_LED01_IDLE))
			{
				// 流水亮完成, 数据转换
				R_FlowLED01_Para_Sen2A = R_FlowLED01_Para_Sen2B;
				R_FlowLED01_Para_Sen2B = R_FlowLED01_Para_Sen2C;
				//=============================
				R_FlowLED01_Para_Sen2C.State     = 0;
				R_FlowLED01_Para_Sen2C.Cnt       = 0;
				R_FlowLED01_Para_Sen2C.DelayTime = 0;
			}
			else if((R_FlowLED01_Para_Sen2A.State == C_FLOW_LED01_OFF) && (R_FlowLED01_Para_Sen2B.State == C_FLOW_LED01_IDLE))
			{
				R_FlowLED01_Para_Sen2A.State     = 0;
				R_FlowLED01_Para_Sen2A.Cnt       = 0;
				R_FlowLED01_Para_Sen2A.DelayTime = 0;
			}
		}
	}
	//=========================================================
	// Sen2B显示处理
	if((0 != R_FlowLED01_Para_Sen2B.State) && (0 == R_FlowLED01_Para_Sen2B.DelayTime))
	{
		if(R_FlowLED01_Para_Sen2B.State == C_FLOW_LED01_ON)
		{
			for(i = 0; i < N_FLOW_LED01_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_FlowLED01_Para_Sen2B.Cnt < R_FlowLED_FlowNum)
				{
					// 继续显示下一个
					F_FlowLED01_DispLED_On(R_FlowLED_FlowNum - 1 - R_FlowLED01_Para_Sen2B.Cnt, R_FlowLED_DispBright);
					R_FlowLED01_Para_Sen2B.Cnt++;
					R_FlowLED01_Para_Sen2B.DelayTime = R_FlowLED_FlowTimer;
					
					// 流水亮完成
					if(R_FlowLED01_Para_Sen2B.Cnt >= R_FlowLED_FlowNum)
					{
						R_FlowLED01_Para_Sen2B.DelayTime = R_FlowLED_DelayTimer;	// 流水完成, 延时灭处理
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
	if((0 != R_FlowLED01_Para_Sen2C.State) && (0 == R_FlowLED01_Para_Sen2C.DelayTime))
	{
		if(R_FlowLED01_Para_Sen2C.State == C_FLOW_LED01_ON)
		{
			for(i = 0; i < N_FLOW_LED01_DISP_STEP_NUM; i++)
			{
				// 流水渐亮处理
				if(R_FlowLED01_Para_Sen2C.Cnt < R_FlowLED_FlowNum)
				{
					// 继续显示下一个
					//F_FlowLED01_DispLED_On(R_FlowLED_FlowNum - 1 - R_FlowLED01_Para_Sen2C.Cnt, R_FlowLED_DispBright);	// Sen1B在流水显示, Sen1C无需显示处理
					R_FlowLED01_Para_Sen2C.Cnt++;
					R_FlowLED01_Para_Sen2C.DelayTime = R_FlowLED_FlowTimer;
					
					// 流水亮完成
					if(R_FlowLED01_Para_Sen2C.Cnt >= R_FlowLED_FlowNum)
					{
						R_FlowLED01_Para_Sen2C.DelayTime = R_FlowLED_DelayTimer;	// 流水完成, 延时灭处理
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
// FUNCTION: 重置流水灯处理变量
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_FlowLED01_Init(void)
{
	uint16_t i = 0;
	FLOW_LED01_PARA_T R_RST_Para_SEN;
	
	for(i=0; i<SET_FLOW_LED_FLOW_NUM_MAX; i++)
	{
		R_FlowLED01_Disp_Data[i] = 0;
	#ifdef FLOW_LED01_DISP_FOR_PWM
		R_Disp_For_PWM_State[i] &= ~C_FLOW_LED01_DISP_FOR_PWM_ON;
	#endif
	}
	R_FlowLED01_Disp_Flag = 0;
	
	// 清除当前的工作状态
	R_RST_Para_SEN.State = 0;
	R_RST_Para_SEN.DelayTime = 0;
	R_RST_Para_SEN.Cnt = 0;
	R_FlowLED01_Para_Sen1A = R_RST_Para_SEN;
	R_FlowLED01_Para_Sen1B = R_RST_Para_SEN;
	R_FlowLED01_Para_Sen1C = R_RST_Para_SEN;
	R_FlowLED01_Para_Sen2A = R_RST_Para_SEN;
	R_FlowLED01_Para_Sen2B = R_RST_Para_SEN;
	R_FlowLED01_Para_Sen2C = R_RST_Para_SEN;
	
	// 显示数据变化, 更新显示
	Set_FlowLED01_Disp_Updata();
}



//==============================================================================================================
//==============================================================================================================
// 演示功能
//==============================================================================================================
//==============================================================================================================

// 流水显示的数据
const uint8_t T_FlowTest_BinghtTab[] = {250, 100, 75,  50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0};
// 流水显示的数据长度
const uint8_t R_FlowTest_Len = sizeof(T_FlowTest_BinghtTab) / sizeof(T_FlowTest_BinghtTab[0]);
// 流水显示计数
uint8_t R_FlowTest_Cnt = 0;


//==========================================================================
// FUNCTION: 流水灯测试开始
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_FlowTest_Start(void)
{
	R_FlowLED01_Para_Sen1A.State = C_FLOW_LED01_ON;
	R_FlowLED01_Para_Sen1A.Cnt = 0;
	R_FlowLED01_Para_Sen1A.DelayTime = 0;
	R_FlowTest_Cnt = 0;
}


//==========================================================================
// FUNCTION: 流水灯测试停止
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_FlowTest_Stop(void)
{
	R_FlowLED01_Para_Sen1A.State = C_FLOW_LED01_IDLE;
	R_FlowLED01_Para_Sen1A.Cnt = 0;
	R_FlowLED01_Para_Sen1A.DelayTime = 0;
	R_FlowTest_Cnt = 0;
}
	

//==========================================================================
// FUNCTION: 流水灯测试处理服务程序
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_FlowTest_Service(void)
{
	uint16_t i = 0;
	uint16_t j = 0;
	
	//=========================================================
	// 流水显示处理
	//=========================================================
	// 定时处理
	if(C_TimerFlag_50ms_Bit & R_TimerFlag)
	{
		R_FlowLED01_Para_Sen1A.DelayTime = (R_FlowLED01_Para_Sen1A.DelayTime != 0) ? R_FlowLED01_Para_Sen1A.DelayTime-1 : 0;
	}
	
	
	//=========================================================
	// Sen1A显示处理
	//=========================================================
	if((R_FlowLED01_Para_Sen1A.State == C_FLOW_LED01_ON) && (0 == R_FlowLED01_Para_Sen1A.DelayTime))
	{
		// 流水渐亮处理
		if(R_FlowLED01_Para_Sen1A.Cnt < R_FlowLED_FlowNum)
		{
			// 把上次的数据进行移位
			j = R_FlowLED_FlowNum - 1;
			for(i = 0; i < j; i++)
			{
				R_FlowLED01_Disp_Data[j-i] = R_FlowLED01_Disp_Data[j-1-i];
			}
			// 增加显示数据
			R_FlowLED01_Disp_Data[0] = T_FlowTest_BinghtTab[R_FlowTest_Cnt];
			R_FlowTest_Cnt = (R_FlowTest_Cnt < (R_FlowTest_Len - 1)) ? (R_FlowTest_Cnt + 1) : 0;
			Set_FlowLED01_Disp_Updata();	// 显示数据变化, 更新显示
			R_FlowLED01_Para_Sen1A.DelayTime = R_FlowLED_FlowTimer;
		}
	}
}





