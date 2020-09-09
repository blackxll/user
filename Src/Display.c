//==========================================================================
// Function Define:
// void F_Disp_GPIO_Init(void)
// void F_TM3130_SendData(uint8_t R_Disp_Data)
// void F_Refresh_Display(void)
// void F_Set_Disp_All(uint8_t R_DispDat)
// void F_Set_Disp_StairPara(void)
// void F_Set_Disp_FlowPara(void)
// void F_Disp_StartLamp_Time_LV(void)
// void F_Disp_StartLamp_Bright_LV(void)
// void F_Disp_StartLamp_DelayOffTime_LV(void)
// void F_Disp_FlowLED_Time_LV(void)
// void F_Disp_FlowLED_Bright_LV(void)
// void F_Disp_FlowLED_DelayOffTime_LV(void)
//==========================================================================
#include "Public.h"



//==========================================================================
// 显示变量定义
//==========================================================================
// 显示状态标志
uint8_t R_Disp_Update_Flag = 0;

// 进度条当前的显示类型
uint8_t R_Disp_SetType = C_DISP_SET_IDLE;

// 参数设置LED显示倒计时
uint16_t R_Disp_SetPara_DispOff_Timer;

// 楼梯灯和流水灯设置参数
uint8_t R_Disp_StairLamp_Time;
uint8_t R_Disp_StairLamp_Bright;
uint8_t R_Disp_StairLamp_DelayTime;
uint8_t R_Disp_FlowLED_Time;
uint8_t R_Disp_FlowLED_Bright;
uint8_t R_Disp_FlowLED_DelayTime;

// 进度条显示临时存储变量
uint8_t R_Disp_Update_Buf[3];



//==========================================================================
// FUNCTION: 显示IO初始化
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Disp_GPIO_Init(void)
{
	// PORT0
	GPIO_SetMode(P_DISP_DIN_GPIO, C_DISP_DIN_BIT, GPIO_PMD_OUTPUT);
	GPIO_ResetBits(P_DISP_DIN_GPIO, C_DISP_DIN_BIT);
}


//==========================================================================
// FUNCTION: TM3130的DIN输出端发送数据
// INPUT:    
//			 R_Disp_Data -- 显示的LED数据
// OUTPUT:   NONE
//==========================================================================
void F_TM3130_SendData(uint8_t R_DAT)
{
	//=========================================================
	// BIT7
	//=========================================================
	// 码0和码1起始为高电平
	P_DISP_DIN = 1;							//  5个指令周期(80ns)
	//==========================
	// 把码0的电平反转
	DELAY_10NOP();							// 10个指令周期(200ns)
	if(!(R_DAT & 0x80))	P_DISP_DIN = 0;		// 10个指令周期(200ns)
	//==========================
	// 把码1的电平反转
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 0;							//  5个指令周期(100ns)
	
	//=========================================================
	// BIT6
	//=========================================================
	// 码0和码1起始为高电平
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 1;							//  5个指令周期(100ns)
	//==========================
	// 把码0的电平反转
	DELAY_10NOP();							// 10个指令周期(200ns)
	if(!(R_DAT & 0x40))	P_DISP_DIN = 0;		// 10个指令周期(200ns)
	//==========================
	// 把码1的电平反转
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 0;							//  5个指令周期(100ns)
	
	//=========================================================
	// BIT5
	//=========================================================
	// 码0和码1起始为高电平
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 1;							//  5个指令周期(100ns)
	//==========================
	// 把码0的电平反转
	DELAY_10NOP();							// 10个指令周期(200ns)
	if(!(R_DAT & 0x20))	P_DISP_DIN = 0;		// 10个指令周期(200ns)
	//==========================
	// 把码1的电平反转
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 0;							//  5个指令周期(100ns)
	
	//=========================================================
	// BIT4
	//=========================================================
	// 码0和码1起始为高电平
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 1;							//  5个指令周期(100ns)
	//==========================
	// 把码0的电平反转
	DELAY_10NOP();							// 10个指令周期(200ns)
	if(!(R_DAT & 0x10))	P_DISP_DIN = 0;		// 10个指令周期(200ns)
	//==========================
	// 把码1的电平反转
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 0;							//  5个指令周期(100ns)
	
	//=========================================================
	// BIT3
	//=========================================================
	// 码0和码1起始为高电平
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 1;							//  5个指令周期(100ns)
	//==========================
	// 把码0的电平反转
	DELAY_10NOP();							// 10个指令周期(200ns)
	if(!(R_DAT & 0x08))	P_DISP_DIN = 0;		// 10个指令周期(200ns)
	//==========================
	// 把码1的电平反转
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 0;							//  5个指令周期(100ns)
	
	//=========================================================
	// BIT2
	//=========================================================
	// 码0和码1起始为高电平
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 1;							//  5个指令周期(100ns)
	//==========================
	// 把码0的电平反转
	DELAY_10NOP();							// 10个指令周期(200ns)
	if(!(R_DAT & 0x04))	P_DISP_DIN = 0;		// 10个指令周期(200ns)
	//==========================
	// 把码1的电平反转
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 0;							//  5个指令周期(100ns)
	
	//=========================================================
	// BIT1
	//=========================================================
	// 码0和码1起始为高电平
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 1;							//  5个指令周期(100ns)
	//==========================
	// 把码0的电平反转
	DELAY_10NOP();							// 10个指令周期(200ns)
	if(!(R_DAT & 0x02))	P_DISP_DIN = 0;		// 10个指令周期(200ns)
	//==========================
	// 把码1的电平反转
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 0;							//  5个指令周期(100ns)
	
	//=========================================================
	// BIT0
	//=========================================================
	// 码0和码1起始为高电平
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 1;							//  5个指令周期(100ns)
	//==========================
	// 把码0的电平反转
	DELAY_10NOP();							// 10个指令周期(200ns)
	if(!(R_DAT & 0x01))	P_DISP_DIN = 0;		// 10个指令周期(200ns)
	//==========================
	// 把码1的电平反转
	DELAY_5NOP();							//  5个指令周期(100ns)
	DELAY_10NOP();							// 10个指令周期(200ns)
	P_DISP_DIN = 0;							//  5个指令周期(100ns)
	DELAY_3NOP();							//  3个指令周期(60ns)
	DELAY_5NOP();							//  5个指令周期(100ns)
}


//==========================================================================
// FUNCTION: TM3130 LED驱动芯片数据更新
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
void F_Refresh_Display(void)
{
#ifdef DISP_DATA_UPDATA_FOR_CHANGE
	if(Be_Disp_Data_Update())
#endif
	{
		//=========================================
		// 清除显示数据更新标志
		//=========================================
		Clr_Disp_Data_Update();
		
		//=========================================
		// 更新显示数据
		//=========================================
		__set_PRIMASK(1);	// 关中断
		F_TM3130_SendData(R_Disp_Update_Buf[0]);
		F_TM3130_SendData(R_Disp_Update_Buf[2]);
		F_TM3130_SendData(0);
		F_TM3130_SendData(R_Disp_Update_Buf[1]);
		__set_PRIMASK(0);	// 开中断
	}
}


//==========================================================================
// FUNCTION: 所有段位显示或清屏
// INPUT:   
//			 R_DispDat -- 显示的数据
// OUTPUT:   NONE
//==========================================================================
void F_Set_Disp_All(uint8_t R_DispDat)
{
	R_Disp_Update_Buf[0] = R_DispDat;
	R_Disp_Update_Buf[1] = R_DispDat;
	R_Disp_Update_Buf[2] = R_DispDat;
	Set_Disp_Data_Update();		// 更新显示数据
}


//==========================================================================
// FUNCTION: 显示楼梯灯设置的参数
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Set_Disp_StairPara(void)
{
	R_Disp_Update_Buf[0] = R_Disp_StairLamp_Time;
	R_Disp_Update_Buf[1] = R_Disp_StairLamp_Bright;
	R_Disp_Update_Buf[2] = R_Disp_StairLamp_DelayTime;
	Set_Disp_Data_Update();		// 更新显示数据
}


//==========================================================================
// FUNCTION: 显示流水灯设置的参数
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Set_Disp_FlowPara(void)
{
	R_Disp_Update_Buf[0] = R_Disp_FlowLED_Time;
	R_Disp_Update_Buf[1] = R_Disp_FlowLED_Bright;
	R_Disp_Update_Buf[2] = R_Disp_FlowLED_DelayTime;
	Set_Disp_Data_Update();		// 更新显示数据
}


//==========================================================================
// FUNCTION: 楼梯灯流水时间级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Disp_StartLamp_Time_LV(void)
{
	switch(R_SetPara_StairLamp_Time_LV)
	{
		case C_SET_PARA_LV1:	R_Disp_StairLamp_Time = C_DISP_SET_LV01;	break;
		case C_SET_PARA_LV2:	R_Disp_StairLamp_Time = C_DISP_SET_LV02;	break;
		case C_SET_PARA_LV3:	R_Disp_StairLamp_Time = C_DISP_SET_LV03;	break;
		case C_SET_PARA_LV4:	R_Disp_StairLamp_Time = C_DISP_SET_LV04;	break;
		case C_SET_PARA_LV5:	R_Disp_StairLamp_Time = C_DISP_SET_LV05;	break;
		case C_SET_PARA_LV6:	R_Disp_StairLamp_Time = C_DISP_SET_LV06;	break;
		case C_SET_PARA_LV7:	R_Disp_StairLamp_Time = C_DISP_SET_LV07;	break;
		case C_SET_PARA_LV8:	R_Disp_StairLamp_Time = C_DISP_SET_LV08;	break;
		default:				R_Disp_StairLamp_Time = C_DISP_SET_LV01;	R_SetPara_StairLamp_Time_LV = C_SET_PARA_LV1;	break;
	}
	Set_Disp_Data_Update();		// 更新显示数据
}


//==========================================================================
// FUNCTION: 楼梯灯显示亮度级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Disp_StartLamp_Bright_LV(void)
{
	switch(R_SetPara_StairLamp_Bright_LV)
	{
		case C_SET_PARA_LV1:	R_Disp_StairLamp_Bright = C_DISP_SET_LV01;	break;
		case C_SET_PARA_LV2:	R_Disp_StairLamp_Bright = C_DISP_SET_LV02;	break;
		case C_SET_PARA_LV3:	R_Disp_StairLamp_Bright = C_DISP_SET_LV03;	break;
		case C_SET_PARA_LV4:	R_Disp_StairLamp_Bright = C_DISP_SET_LV04;	break;
		case C_SET_PARA_LV5:	R_Disp_StairLamp_Bright = C_DISP_SET_LV05;	break;
		case C_SET_PARA_LV6:	R_Disp_StairLamp_Bright = C_DISP_SET_LV06;	break;
		case C_SET_PARA_LV7:	R_Disp_StairLamp_Bright = C_DISP_SET_LV07;	break;
		case C_SET_PARA_LV8:	R_Disp_StairLamp_Bright = C_DISP_SET_LV08;	break;
		default:				R_Disp_StairLamp_Bright = C_DISP_SET_LV01;	R_SetPara_StairLamp_Bright_LV = C_SET_PARA_LV1;	break;
	}
	Set_Disp_Data_Update();		// 更新显示数据
}


//==========================================================================
// FUNCTION: 延时流水关闭级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Disp_StartLamp_DelayOffTime_LV(void)
{
	switch(R_SetPara_StairLamp_DelayOffTime_LV)
	{
		case C_SET_PARA_LV1:	R_Disp_StairLamp_DelayTime = C_DISP_SET_LV01;	break;
		case C_SET_PARA_LV2:	R_Disp_StairLamp_DelayTime = C_DISP_SET_LV02;	break;
		case C_SET_PARA_LV3:	R_Disp_StairLamp_DelayTime = C_DISP_SET_LV03;	break;
		case C_SET_PARA_LV4:	R_Disp_StairLamp_DelayTime = C_DISP_SET_LV04;	break;
		case C_SET_PARA_LV5:	R_Disp_StairLamp_DelayTime = C_DISP_SET_LV05;	break;
		case C_SET_PARA_LV6:	R_Disp_StairLamp_DelayTime = C_DISP_SET_LV06;	break;
		case C_SET_PARA_LV7:	R_Disp_StairLamp_DelayTime = C_DISP_SET_LV07;	break;
		case C_SET_PARA_LV8:	R_Disp_StairLamp_DelayTime = C_DISP_SET_LV08;	break;
		default:				R_Disp_StairLamp_DelayTime = C_DISP_SET_LV01;	R_SetPara_StairLamp_DelayOffTime_LV = C_SET_PARA_LV1;	break;
	}
	Set_Disp_Data_Update();		// 更新显示数据
}


//==========================================================================
// FUNCTION: 流水灯流水时间级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Disp_FlowLED_Time_LV(void)
{
	switch(R_SetPara_FlowLED_Time_LV)
	{
		case C_SET_PARA_LV1:	R_Disp_FlowLED_Time = C_DISP_SET_LV01;	break;
		case C_SET_PARA_LV2:	R_Disp_FlowLED_Time = C_DISP_SET_LV02;	break;
		case C_SET_PARA_LV3:	R_Disp_FlowLED_Time = C_DISP_SET_LV03;	break;
		case C_SET_PARA_LV4:	R_Disp_FlowLED_Time = C_DISP_SET_LV04;	break;
		case C_SET_PARA_LV5:	R_Disp_FlowLED_Time = C_DISP_SET_LV05;	break;
		case C_SET_PARA_LV6:	R_Disp_FlowLED_Time = C_DISP_SET_LV06;	break;
		case C_SET_PARA_LV7:	R_Disp_FlowLED_Time = C_DISP_SET_LV07;	break;
		case C_SET_PARA_LV8:	R_Disp_FlowLED_Time = C_DISP_SET_LV08;	break;
		default:				R_Disp_FlowLED_Time = C_DISP_SET_LV01;	R_SetPara_FlowLED_Time_LV = C_SET_PARA_LV1;	break;
	}
	Set_Disp_Data_Update();		// 更新显示数据
}


//==========================================================================
// FUNCTION: 流水灯显示亮度级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Disp_FlowLED_Bright_LV(void)
{
	switch(R_SetPara_FlowLED_Bright_LV)
	{
		case C_SET_PARA_LV1:	R_Disp_FlowLED_Bright = C_DISP_SET_LV01;	break;
		case C_SET_PARA_LV2:	R_Disp_FlowLED_Bright = C_DISP_SET_LV02;	break;
		case C_SET_PARA_LV3:	R_Disp_FlowLED_Bright = C_DISP_SET_LV03;	break;
		case C_SET_PARA_LV4:	R_Disp_FlowLED_Bright = C_DISP_SET_LV04;	break;
		case C_SET_PARA_LV5:	R_Disp_FlowLED_Bright = C_DISP_SET_LV05;	break;
		case C_SET_PARA_LV6:	R_Disp_FlowLED_Bright = C_DISP_SET_LV06;	break;
		case C_SET_PARA_LV7:	R_Disp_FlowLED_Bright = C_DISP_SET_LV07;	break;
		case C_SET_PARA_LV8:	R_Disp_FlowLED_Bright = C_DISP_SET_LV08;	break;
		default:				R_Disp_FlowLED_Bright = C_DISP_SET_LV01;	R_SetPara_FlowLED_Bright_LV = C_SET_PARA_LV1;	break;
	}
	Set_Disp_Data_Update();		// 更新显示数据
}


//==========================================================================
// FUNCTION: 延时流水关闭级别显示
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
void F_Disp_FlowLED_DelayOffTime_LV(void)
{
	switch(R_SetPara_FlowLED_DelayOffTime_LV)
	{
		case C_SET_PARA_LV1:	R_Disp_FlowLED_DelayTime = C_DISP_SET_LV01;	break;
		case C_SET_PARA_LV2:	R_Disp_FlowLED_DelayTime = C_DISP_SET_LV02;	break;
		case C_SET_PARA_LV3:	R_Disp_FlowLED_DelayTime = C_DISP_SET_LV03;	break;
		case C_SET_PARA_LV4:	R_Disp_FlowLED_DelayTime = C_DISP_SET_LV04;	break;
		case C_SET_PARA_LV5:	R_Disp_FlowLED_DelayTime = C_DISP_SET_LV05;	break;
		case C_SET_PARA_LV6:	R_Disp_FlowLED_DelayTime = C_DISP_SET_LV06;	break;
		case C_SET_PARA_LV7:	R_Disp_FlowLED_DelayTime = C_DISP_SET_LV07;	break;
		case C_SET_PARA_LV8:	R_Disp_FlowLED_DelayTime = C_DISP_SET_LV08;	break;
		default:				R_Disp_FlowLED_DelayTime = C_DISP_SET_LV01;	R_SetPara_FlowLED_DelayOffTime_LV = C_SET_PARA_LV1;	break;
	}
	Set_Disp_Data_Update();		// 更新显示数据
}







