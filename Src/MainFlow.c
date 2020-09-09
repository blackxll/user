//==========================================================================
// Function Define:
//==========================================================================
#include "Public.h"


//uint8_t R_MainFlow_State = 0xff;
//==========================================================================
// FUNCTION: 主程序
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
int32_t main(void)
{
	// 主程序运行状态
	uint8_t R_MainFlow_State = 0xff;
	uint8_t R_MainFlow_AfterState = 0x00;
	
	// 主程序计数
//	uint16_t R_MainFlow_Timer = 0;
	
	// 长按键定时器
//	uint16_t R_Flash_Disp_Timer = 0;
	
	// 长按键定时器
	uint16_t R_Key_Keep_Timer = 0;
	uint16_t R_Key_Lock_Timer = 0;
	
	// 楼梯灯按键变量
	uint16_t R_StairKey_Long_Timer = 0;	// 长按计时
	// 流水灯按键变量
	uint16_t R_FlowKey_Long_Timer = 0;	// 长按计时


	
	// 通用变量定义
//	uint8_t  tmp80 = 0;		//  8位通用变量
//	uint16_t tmp160 = 0;	// 16位通用变量
//	uint32_t tmp320 = 0;	// 32位通用变量


	// 初始化设备
    F_SystemDevice_Init();
	
	// 清除数码管显示
	F_Set_Disp_All(0x00);
	F_Refresh_Display();
	
	// 清除楼梯灯和流水灯显示
	F_Disp_StairLamp_All(SET_STAIR_LAMP_FLOW_NUM_MAX, 0x00);
	F_Disp_FlowLED01_All(SET_FLOW_LED_FLOW_NUM_MAX,  0x00);
	F_Refresh_GS1903_All_For2Din();
	
	// 等待系统稳定
	//R_MainFlow_Timer = 5;

	while(1)
	{
		//GPIO_ToggleBits(P0, BIT0);
		//==========================================================================
		// 常用特殊处理程序
		//==========================================================================
		F_WDT_RST();
		F_TimerCounting_Service();
		F_ScanKey_Service();
			
		//==========================================================================
		// 处理定时
		//==========================================================================
		if(C_TimerFlag_50ms_Bit & R_TimerFlag)
		{
		//	R_MainFlow_Timer   = (R_MainFlow_Timer != 0)   ? R_MainFlow_Timer-1 : 0;
		//	R_Flash_Disp_Timer = (R_Flash_Disp_Timer != 0) ? R_Flash_Disp_Timer-1 : 0;
			R_Key_Keep_Timer   = (R_Key_Keep_Timer != 0)   ? R_Key_Keep_Timer-1 : 0;
			R_Key_Lock_Timer   = (R_Key_Lock_Timer != 0)   ? R_Key_Lock_Timer-1 : 0;
			R_Disp_SetPara_DispOff_Timer = (R_Disp_SetPara_DispOff_Timer != 0) ? R_Disp_SetPara_DispOff_Timer-1 : 0;
			
			R_StairKey_Long_Timer = (R_StairKey_Long_Timer != 0) ? R_StairKey_Long_Timer-1 : 0;
			R_FlowKey_Long_Timer  = (R_FlowKey_Long_Timer != 0)  ? R_FlowKey_Long_Timer-1 : 0;
		
			
			//GPIO_SetBits(P0, BIT1);
			//F_Disp_StairLamp_All(SET_STAIR_LAMP_FLOW_NUM_MAX, R_Disp_Duty);
			//F_Disp_FlowLED01_All(SET_FLOW_LED_FLOW_NUM_MAX,  R_Disp_Duty);
			// 发送GS1903数据
			F_Refresh_GS1903_All_For2Din();
			// 发送TM3130数码管数据
			F_Refresh_Display();
			//GPIO_ResetBits(P0, BIT1);
		}
		
	
		//=========================================================================================================
		//=========================================================================================================
		// 待机处理
		//=========================================================================================================
		//=========================================================================================================
		if(0x00 == R_MainFlow_State)
		{
			F_StairLamp_PWM_TimeUpdate();
			F_FlowLED01_PWM_TimeUpdate();
			
			F_FlowOFF_Floor1_Service();
			F_StairLamp_Service();	// 楼梯灯处理
			F_FlowLED01_Service();	// 流水灯处理
			
			
			
			//==============================================================
			// 楼梯灯和流水灯数量和演示进入
			//==============================================================
			if((C_KEY_Stair | C_KEY_Flow) & R_MainFlow_NewKey)
			{
				// delay 3s(20ms base)
				R_StairKey_Long_Timer  = SET_PARA_LONG_KEY_TIME;
				R_FlowKey_Long_Timer  = SET_PARA_LONG_KEY_TIME;
			}
			else if((C_KEY_Stair == R_MainFlow_ReleaseKey) && (0 != R_StairKey_Long_Timer))
			{
				// 短按, 进度条显示楼梯灯设置参数
				R_Disp_SetPara_DispOff_Timer = N_DISP_SET_PARA_LED_OFF_TIME;
				R_Disp_SetType = C_DISP_SET_STAIR_PARA;		// 进度条显示
				F_Set_Disp_StairPara();
			}
			else if((C_KEY_Flow == R_MainFlow_ReleaseKey) && (0 != R_FlowKey_Long_Timer))
			{
				// 短按, 进度条显示流水灯设置参数
				R_Disp_SetPara_DispOff_Timer = N_DISP_SET_PARA_LED_OFF_TIME;
				R_Disp_SetType = C_DISP_SET_FLOW_PARA;		// 进度条显示
				F_Set_Disp_FlowPara();
			}
			else if((C_KEY_Stair == R_MainFlow_HisKey) && (0 == R_StairKey_Long_Timer))
			{
				// 长按楼梯键, 进入设置数量
				R_MainFlow_State = 0xA0;
				continue;
			}
			else if((C_KEY_Flow == R_MainFlow_HisKey) && (0 == R_FlowKey_Long_Timer))
			{
				// 长按流水键, 进入设置数量
				R_MainFlow_State = 0xB0;
				continue;
			}
			else if(((C_KEY_Stair | C_KEY_Flow) == R_MainFlow_HisKey) && (0 == R_StairKey_Long_Timer) && (0 == R_FlowKey_Long_Timer))
			{
				// 长按楼梯灯和流水灯键, 进入演示模式
				R_MainFlow_State = 0x30;
				continue;
			}
			
			
			
			//==============================================================
			// 楼梯灯参数设置
			//==============================================================
			if(C_DISP_SET_STAIR_PARA == R_Disp_SetType)
			{
				// 改变流水时间
				if(C_KEY_Time & R_MainFlow_NewKey)
				{
					F_Chagne_StartLamp_TimeLV();
					F_Set_Disp_StairPara();
					R_Disp_SetPara_DispOff_Timer = N_DISP_SET_PARA_LED_OFF_TIME;
					R_MainFlow_AfterState = R_MainFlow_State;
					R_MainFlow_State = 0x01;
				}
				// 改变显示亮度
				if(C_KEY_Bright & R_MainFlow_NewKey)
				{
					F_Chagne_StartLamp_BrightLV();
					F_Set_Disp_StairPara();
					R_Disp_SetPara_DispOff_Timer = N_DISP_SET_PARA_LED_OFF_TIME;
					R_MainFlow_AfterState = R_MainFlow_State;
					R_MainFlow_State = 0x01;
				}
				// 改变延时时间
				if(C_KEY_DelayTime & R_MainFlow_NewKey)
				{
					F_Chagne_StartLamp_DelayOffTimeLV();
					F_Chagne_FlowLED_DelayOffTimeLV();
					F_Set_Disp_StairPara();
					R_Disp_SetPara_DispOff_Timer = N_DISP_SET_PARA_LED_OFF_TIME;
					R_MainFlow_AfterState = R_MainFlow_State;
					R_MainFlow_State = 0x01;
				}
			}
			
			
			//==============================================================
			// 流水灯参数设置
			//==============================================================
			if(C_DISP_SET_FLOW_PARA == R_Disp_SetType)
			{
				// 改变流水时间
				if(C_KEY_Time & R_MainFlow_NewKey)
				{
					F_Chagne_FlowLED_TimeLV();
					F_Set_Disp_FlowPara();
					R_Disp_SetPara_DispOff_Timer = N_DISP_SET_PARA_LED_OFF_TIME;
					R_MainFlow_AfterState = R_MainFlow_State;
					R_MainFlow_State = 0x01;
				}
				// 改变显示亮度
				if(C_KEY_Bright & R_MainFlow_NewKey)
				{
					F_Chagne_FlowLED_BrightLV();
					F_Set_Disp_FlowPara();
					R_Disp_SetPara_DispOff_Timer = N_DISP_SET_PARA_LED_OFF_TIME;
					R_MainFlow_AfterState = R_MainFlow_State;
					R_MainFlow_State = 0x01;
				}
				// 改变延时时间
				if(C_KEY_DelayTime & R_MainFlow_NewKey)
				{
					F_Chagne_StartLamp_DelayOffTimeLV();
					F_Chagne_FlowLED_DelayOffTimeLV();
					F_Set_Disp_FlowPara();
					R_Disp_SetPara_DispOff_Timer = N_DISP_SET_PARA_LED_OFF_TIME;
					R_MainFlow_AfterState = R_MainFlow_State;
					R_MainFlow_State = 0x01;
				}
			}
			
			
			//==============================================================
			// 进度条显示处理
			//==============================================================
			if(0 == R_Disp_SetPara_DispOff_Timer)
			{
				#ifdef DISP_DATA_UPDATA_FOR_CHANGE
				if((R_Disp_SetType == C_DISP_SET_STAIR_PARA) || (R_Disp_SetType == C_DISP_SET_FLOW_PARA))
				#endif
				{
					// 关闭进度条显示
					R_Disp_SetType = C_DISP_SET_IDLE;
					F_Set_Disp_All(0x00);
				}
			}
		}
		
		//=========================================================================================================
		//=========================================================================================================
		// 存储设定参数
		//=========================================================================================================
		//=========================================================================================================
		else if(0x01 == R_MainFlow_State)
		{
			// 把参数存储到APROM
			*(uint32_t *)&RT_Safety_Flash.CRCPRT = 0x06050309;
			F_GenerateCRC_Seek(&RT_Safety_Flash.CRCPRT);
			R_SetPara_RAM.CheckSum = F_Get_CheckSum(&R_SetPara_RAM, sizeof(SYS_SET_PARA_T));
			if(0 != F_Save_Parameters(&RT_Safety_Flash, &R_SetPara_RAM))
			{
				// 存储参数失败
				__NOP();
				__NOP();
			}
			R_MainFlow_State = R_MainFlow_AfterState;
			continue;
		}
		
		//=========================================================================================================
		//=========================================================================================================
		// 楼梯灯演示处理
		//=========================================================================================================
		//=========================================================================================================
		else if(0x10 == R_MainFlow_State)
		{
			// 存储系统的工作模式
			F_Save_System_RunMode(C_SYS_RUN_STAIR_TEST);
			
			// 清除进度条显示
			R_Disp_SetType = C_DISP_SET_IDLE;
			F_Set_Disp_All(0x00);
			
			F_StairLamp_Init();
			F_Disp_StairLamp_All(SET_STAIR_LAMP_FLOW_NUM_MAX, 0x00);
			F_StairTest_Start();
			R_MainFlow_State++;
			continue;
		}
		else if(0x11 == R_MainFlow_State)
		{
			// 演示模式处理
			F_StairTest_Service();
		
			// 等待释放按键
			if(C_KEY_Stair & R_MainFlow_HisKey)	continue;
			R_MainFlow_State++;
			continue;
		}
		else if(0x12 == R_MainFlow_State)
		{
			// 演示模式处理
			F_StairTest_Service();
			
			// 按键返回
			if(C_KEY_Stair & R_MainFlow_NewKey)
			{
				R_Key_Lock_Timer = SET_PARA_LONG_KEY_TIME;		// 长按时间为3秒钟
			}
			else if((0 == R_Key_Lock_Timer) && (C_KEY_Stair == R_MainFlow_HisKey))
			{
				// 长按, 返回
				F_StairTest_Stop();
				F_Disp_StairLamp_All(SET_STAIR_LAMP_FLOW_NUM_MAX, 0x00);	// 清显示
				R_MainFlow_State++;
				continue;
			}
		}
		else if(0x13 == R_MainFlow_State)
		{
			// 等待释放按键
			if(C_KEY_Stair & R_MainFlow_HisKey)	continue;
			
			// 存储系统的工作模式
			F_Save_System_RunMode(C_SYS_RUN_STD);
			R_MainFlow_State = 0x00;
			continue;
		}
		
		//=========================================================================================================
		//=========================================================================================================
		// 流水灯演示处理
		//=========================================================================================================
		//=========================================================================================================
		else if(0x20 == R_MainFlow_State)
		{
			// 存储系统的工作模式
			F_Save_System_RunMode(C_SYS_RUN_FLOW_TEST);
			
			// 清除进度条显示
			R_Disp_SetType = C_DISP_SET_IDLE;
			F_Set_Disp_All(0x00);
			
			F_FlowLED01_Init();
			F_Disp_FlowLED01_All(SET_FLOW_LED_FLOW_NUM_MAX, 0x00);
			F_FlowTest_Start();
			R_MainFlow_State++;
			continue;
		}
		else if(0x21 == R_MainFlow_State)
		{
			// 演示模式处理
			F_FlowTest_Service();
			
			// 等待释放按键
			if(C_KEY_Flow & R_MainFlow_HisKey)	continue;
			R_MainFlow_State++;
			continue;
		}
		else if(0x22 == R_MainFlow_State)
		{
			// 演示模式处理
			F_FlowTest_Service();
			
			// 按键返回
			if(C_KEY_Flow & R_MainFlow_NewKey)
			{
				R_Key_Lock_Timer = SET_PARA_LONG_KEY_TIME;		// 长按时间为3秒钟
			}
			else if((0 == R_Key_Lock_Timer) && (C_KEY_Flow == R_MainFlow_HisKey))
			{
				// 长按, 返回
				F_FlowTest_Stop();
				F_Disp_FlowLED01_All(SET_FLOW_LED_FLOW_NUM_MAX, 0x00);	// 清显示
				R_MainFlow_State++;
				continue;
			}
		}
		else if(0x23 == R_MainFlow_State)
		{
			// 等待释放按键
			if(C_KEY_Flow & R_MainFlow_HisKey)	continue;
			
			// 存储系统的工作模式
			F_Save_System_RunMode(C_SYS_RUN_STD);
			R_MainFlow_State = 0x00;
			continue;
		}
		
		//=========================================================================================================
		//=========================================================================================================
		// 楼梯灯和流水灯演示处理
		//=========================================================================================================
		//=========================================================================================================
		else if(0x30 == R_MainFlow_State)
		{
			// 存储系统的工作模式
			F_Save_System_RunMode(C_SYS_RUN_ALL_TEST);
			
			// 清除进度条显示
			R_Disp_SetType = C_DISP_SET_IDLE;
			F_Set_Disp_All(0x00);
			
			// 楼梯灯启动
			F_StairLamp_Init();
			F_Disp_StairLamp_All(SET_STAIR_LAMP_FLOW_NUM_MAX, 0x00);
			F_StairTest_Start();
			
			// 流水灯启动
			F_FlowLED01_Init();
			F_Disp_FlowLED01_All(SET_FLOW_LED_FLOW_NUM_MAX, 0x00);
			F_FlowTest_Start();
			
			R_MainFlow_State++;
			continue;
		}
		else if(0x31 == R_MainFlow_State)
		{
			// 演示模式处理
			F_FlowTest_Service();
			F_StairTest_Service();
			
			// 等待释放按键
			if((C_KEY_Stair | C_KEY_Flow) & R_MainFlow_HisKey)	continue;
			R_MainFlow_State++;
			continue;
		}
		else if(0x32 == R_MainFlow_State)
		{
			// 演示模式处理
			F_FlowTest_Service();
			F_StairTest_Service();
			
			// 按键返回
			if((C_KEY_Stair | C_KEY_Flow) & R_MainFlow_NewKey)
			{
				R_Key_Lock_Timer = SET_PARA_LONG_KEY_TIME;		// 长按时间为3秒钟
			}
			else if((0 == R_Key_Lock_Timer) && ((C_KEY_Stair | C_KEY_Flow) & R_MainFlow_HisKey))
			{
				// 长按, 返回
				F_FlowTest_Stop();
				F_StairTest_Stop();
				F_Disp_FlowLED01_All(SET_FLOW_LED_FLOW_NUM_MAX, 0x00);		// 清显示
				F_Disp_StairLamp_All(SET_STAIR_LAMP_FLOW_NUM_MAX, 0x00);	// 清显示
				R_MainFlow_State++;
				continue;
			}
		}
		else if(0x33 == R_MainFlow_State)
		{
			// 等待释放按键
			if((C_KEY_Stair | C_KEY_Flow) & R_MainFlow_HisKey)	continue;
			
			// 存储系统的工作模式
			F_Save_System_RunMode(C_SYS_RUN_STD);
			R_MainFlow_State = 0x00;
			continue;
		}
		
		//=========================================================================================================
		//=========================================================================================================
		// 楼梯灯数量设置
		//=========================================================================================================
		//=========================================================================================================
		else if(0xA0 == R_MainFlow_State)
		{
			// 进入初始化显示
			R_StairLamp_FlowNum = SET_STAIR_LAMP_FLOW_NUM_MIN;
			F_StairLamp_Init();
			F_Disp_StairLamp_All(R_StairLamp_FlowNum, R_StairLamp_DispBright);
			R_MainFlow_State++;
			continue;
		}
		else if(0xA1 == R_MainFlow_State)
		{
			// 等待释放按键
			if(C_KEY_Stair & R_MainFlow_HisKey)	continue;
			R_MainFlow_State++;
			continue;
		}
		else if(0xA2 == R_MainFlow_State)
		{
			if(C_KEY_Stair & R_MainFlow_NewKey)
			{
				R_Key_Lock_Timer = SET_PARA_LONG_KEY_TIME;		// 长按时间为3秒钟
			}
			else if((0 == R_Key_Lock_Timer) && (C_KEY_Stair == R_MainFlow_HisKey))
			{
				// 长按, 保存设置参数
				F_Disp_StairLamp_All(SET_STAIR_LAMP_FLOW_NUM_MAX, 0x00);	// 清除所有显示
				
				// 把参数存储到APROM
				R_SetPara_StairLamp_Num = R_StairLamp_FlowNum;
				*(uint32_t *)&RT_Safety_Flash.CRCPRT = 0x06050309;
				F_GenerateCRC_Seek(&RT_Safety_Flash.CRCPRT);
				R_SetPara_RAM.CheckSum = F_Get_CheckSum(&R_SetPara_RAM, sizeof(SYS_SET_PARA_T));
				if(0 != F_Save_Parameters(&RT_Safety_Flash, &R_SetPara_RAM))
				{
					// 存储参数失败
					__NOP();
					__NOP();
				}
				R_MainFlow_State++;
				continue;
			}
			else if( (C_KEY_INC == R_MainFlow_NewKey) || 
					((C_KEY_INC == R_MainFlow_HisKey) && (0 == R_Key_Keep_Timer)))
			{
				// 增加楼梯数量
				R_StairLamp_FlowNum = (R_StairLamp_FlowNum < SET_STAIR_LAMP_FLOW_NUM_MAX) ? (R_StairLamp_FlowNum + 1) : SET_STAIR_LAMP_FLOW_NUM_MAX;
				F_Disp_StairLamp_All(R_StairLamp_FlowNum, R_StairLamp_DispBright);
				R_Key_Keep_Timer = (C_KEY_INC == R_MainFlow_NewKey) ? 12 : 4;
			}
			else if( (C_KEY_DEC == R_MainFlow_NewKey) || 
					((C_KEY_DEC == R_MainFlow_HisKey) && (0 == R_Key_Keep_Timer)))
			{
				// 减少楼梯数量
				R_StairLamp_FlowNum = (R_StairLamp_FlowNum > SET_STAIR_LAMP_FLOW_NUM_MIN) ? (R_StairLamp_FlowNum - 1) : SET_STAIR_LAMP_FLOW_NUM_MIN;
				F_Disp_StairLamp_All(R_StairLamp_FlowNum, R_StairLamp_DispBright);
				R_Key_Keep_Timer = (C_KEY_INC == R_MainFlow_NewKey) ? 12 : 4;
			}
		}
		else if(0xA3 == R_MainFlow_State)
		{
			// 等待释放按键
			if(C_KEY_Stair & R_MainFlow_HisKey)	continue;
			
		#ifdef SET_STAIR_LAMP_FLOW_TIME_FROM_FLOW_LED
			// 楼梯灯数量改变, 楼梯灯流水时间需要重新计算楼梯的流水时间(根据流水灯数量和时间计算)
			F_Read_StartLamp_Time();
		#endif
			
			R_MainFlow_State = 0x00;
			continue;
		}
		
		//=========================================================================================================
		//=========================================================================================================
		// 流水灯数量设置
		//=========================================================================================================
		//=========================================================================================================
		else if(0xB0 == R_MainFlow_State)
		{
			// 进入初始化显示
			R_FlowLED_FlowNum = SET_FLOW_LED_FLOW_NUM_MIN;
			F_FlowLED01_Init();
			F_Disp_FlowLED01_All(R_FlowLED_FlowNum, R_FlowLED_DispBright);
			R_MainFlow_State++;
			continue;
		}
		else if(0xB1 == R_MainFlow_State)
		{
			// 等待释放按键
			if(C_KEY_Flow & R_MainFlow_HisKey)	continue;
			R_MainFlow_State++;
			continue;
		}
		else if(0xB2 == R_MainFlow_State)
		{
			if(C_KEY_Flow & R_MainFlow_NewKey)
			{
				R_Key_Lock_Timer = SET_PARA_LONG_KEY_TIME;		// 长按时间为3秒钟
			}
			else if((0 == R_Key_Lock_Timer) && (C_KEY_Flow == R_MainFlow_HisKey))
			{
				// 长按, 保存设置参数
				F_Disp_FlowLED01_All(SET_FLOW_LED_FLOW_NUM_MAX, 0x00);	// 清除所有显示
				
				// 把参数存储到APROM
				R_SetPara_FlowLED_Num = R_FlowLED_FlowNum;
				*(uint32_t *)&RT_Safety_Flash.CRCPRT = 0x06050309;
				F_GenerateCRC_Seek(&RT_Safety_Flash.CRCPRT);
				R_SetPara_RAM.CheckSum = F_Get_CheckSum(&R_SetPara_RAM, sizeof(SYS_SET_PARA_T));
				if(0 != F_Save_Parameters(&RT_Safety_Flash, &R_SetPara_RAM))
				{
					// 存储参数失败
					__NOP();
					__NOP();
				}
				R_MainFlow_State++;
				continue;
			}
			else if( (C_KEY_INC == R_MainFlow_NewKey) || 
					((C_KEY_INC == R_MainFlow_HisKey) && (0 == R_Key_Keep_Timer)))
			{
				// 增加楼梯数量
			#if (SET_FLOW_LED_FLOW_NUM_MAX == N_FLOW_LED_GS1903_NUM_MAX)
				R_FlowLED_FlowNum = (R_FlowLED_FlowNum < SET_FLOW_LED_FLOW_NUM_MAX) ? (R_FlowLED_FlowNum + 1) : SET_FLOW_LED_FLOW_NUM_MAX;
			#elif (SET_FLOW_LED_FLOW_NUM_MAX == N_FLOW_LED_GS1903_NUM_MAX * 3)
				R_FlowLED_FlowNum = (R_FlowLED_FlowNum < SET_FLOW_LED_FLOW_NUM_MAX) ? (R_FlowLED_FlowNum + 3) : SET_FLOW_LED_FLOW_NUM_MAX;
			#endif
				F_Disp_FlowLED01_All(R_FlowLED_FlowNum, R_FlowLED_DispBright);
				R_Key_Keep_Timer = (C_KEY_INC == R_MainFlow_NewKey) ? 12 : 2;
			}
			else if( (C_KEY_DEC == R_MainFlow_NewKey) || 
					((C_KEY_DEC == R_MainFlow_HisKey) && (0 == R_Key_Keep_Timer)))
			{
				// 减少楼梯数量
			#if (SET_FLOW_LED_FLOW_NUM_MAX == N_FLOW_LED_GS1903_NUM_MAX)
				R_FlowLED_FlowNum = (R_FlowLED_FlowNum > SET_FLOW_LED_FLOW_NUM_MIN) ? (R_FlowLED_FlowNum - 1) : SET_FLOW_LED_FLOW_NUM_MIN;
			#elif (SET_FLOW_LED_FLOW_NUM_MAX == N_FLOW_LED_GS1903_NUM_MAX * 3)
				R_FlowLED_FlowNum = (R_FlowLED_FlowNum > SET_FLOW_LED_FLOW_NUM_MIN) ? (R_FlowLED_FlowNum - 3) : SET_FLOW_LED_FLOW_NUM_MIN;
			#endif
				F_Disp_FlowLED01_All(R_FlowLED_FlowNum, R_FlowLED_DispBright);
				R_Key_Keep_Timer = (C_KEY_INC == R_MainFlow_NewKey) ? 12 : 2;
			}
		}
		else if(0xB3 == R_MainFlow_State)
		{
			// 等待释放按键
			if(C_KEY_Flow & R_MainFlow_HisKey)	continue;
			
		#ifdef SET_STAIR_LAMP_FLOW_TIME_FROM_FLOW_LED
			// 流水灯数量改变, 楼梯流水时间需要重新计算楼梯的流水时间(根据流水灯数量和时间计算)
			F_Read_StartLamp_Time();
		#endif
			
			R_MainFlow_State = 0x00;
			continue;
		}
		
		//=========================================================================================================
		//=========================================================================================================
		// 开机处理
		//=========================================================================================================
		//=========================================================================================================
		else if(0xf0 == R_MainFlow_State)
		{
			//=============================================
			// 根据系统设定的参数, 读取系统工作参数
			R_StairLamp_FlowNum = R_SetPara_StairLamp_Num;	// 楼梯灯数量
			R_FlowLED_FlowNum = R_SetPara_FlowLED_Num;		// 流水灯数量
			F_Read_StartLamp_Time();			// 读取楼梯灯流水时间
			F_Read_StartLamp_Bright();			// 读取楼梯灯显示亮度
			F_Read_StartLamp_DelayOffTime();	// 读取流水灯显示亮度
			F_Read_FlowLED_Time();				// 读取流水灯流水时间
			F_Read_FlowLED_Bright();			// 读取流水灯显示亮度
			F_Read_FlowLED_DelayOffTime();		// 读取流水灯显示亮度
			
			//=============================================
			// 显示进度条级别
			F_Disp_StartLamp_Time_LV();			// 显示楼梯灯流水时间当前的级别
			F_Disp_StartLamp_Bright_LV();		// 显示当前楼梯灯显示亮度的级别
			F_Disp_StartLamp_DelayOffTime_LV();	// 显示当前楼梯灯延时时间的级别
			F_Disp_FlowLED_Time_LV();			// 显示当前流水灯流水时间的级别
			F_Disp_FlowLED_Bright_LV();			// 显示当前流水灯显示亮度的级别
			F_Disp_FlowLED_DelayOffTime_LV();	// 显示当前流水灯延时时间的级别
			
			//=============================================
			// 根据关机前的模式, 进入相应模式
			switch(R_SetPara_System_RunMode)
			{
				case C_SYS_RUN_STD:				R_MainFlow_State = 0x00;	break;
				case C_SYS_RUN_STAIR_TEST:		R_MainFlow_State = 0x10;	break;
				case C_SYS_RUN_FLOW_TEST:		R_MainFlow_State = 0x20;	break;
				case C_SYS_RUN_ALL_TEST:		R_MainFlow_State = 0x30;	break;
				default:
				{
					F_Save_System_RunMode(C_SYS_RUN_STD);
					R_MainFlow_State = 0x00;
					break;
				}
			}
			//R_MainFlow_State = 0x00;
			continue;
		}
		else if(0xfe == R_MainFlow_State)
		{
			// 读取设定参数
			if(0x00 != F_Get_Parameters())
			{
				F_SetPara_Default();	// 读取失败,使用默认值
			}
			else
			{
				R_SetPara_RAM = R_SetPara_APROM;	// 读取参数成功
			}
			// 检测读取参数的范围
			F_SetPara_Check();
			
			R_MainFlow_State = 0xf0;
			continue;
		}
		else if(0xff == R_MainFlow_State)
		{
			// wait stable for system
			//if(0 != R_MainFlow_Timer)	continue;
			
			R_MainFlow_State = 0xfe;
			continue;
		}
	}	
}



