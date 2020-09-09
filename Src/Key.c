//==========================================================================
// Function Define:
// void F_ScanKey_GPIO_Init(void)
// void F_ScanDirect_Handle(uint8_t R_ReadPort, uint8_t R_PortNum)
// void F_ScanKey_Direct(void)
// void F_ScanKey_Service(void)
//==========================================================================
#include "Public.h"


//==========================================================================
//==========================================================================
// KEY State
uint32_t R_NewKey;
uint32_t R_HisKey;
uint32_t R_ReleaseKey;

// Debounce时，备份按键值
uint32_t R_KeyPortReg;

// KEY Debounce
uint8_t R_DebounceCnt;

// KEY Scan Delay
uint8_t R_DirectKey_Scan_Delay;


//========================================================================
// 按键分类
//========================================================================
// 主程序按键
uint32_t R_MainFlow_NewKey;
uint32_t R_MainFlow_HisKey;
uint32_t R_MainFlow_ReleaseKey;

// 传感器1
uint32_t R_Sen_NewKey;
uint32_t R_Sen_HisKey;
uint32_t R_Sen_ReleaseKey;




//==========================================================================
//功能:	扫描按键IO初始化
//输入:	无
//输出:	无
//==========================================================================
void F_ScanKey_GPIO_Init(void)
{
	GPIO_SetMode(P_DirectInGPIO_1, D_IO_Direct_Mask_1, GPIO_PMD_QUASI);
	GPIO_SetBits(P_DirectInGPIO_1, D_IO_Direct_Mask_1);
	
	GPIO_SetMode(P_DirectInGPIO_2, D_IO_Direct_Mask_2, GPIO_PMD_QUASI);
	GPIO_SetBits(P_DirectInGPIO_2, D_IO_Direct_Mask_2);
}


//==========================================================================
//功能：扫描按键
//输入：无
//输出：无
//==========================================================================
void F_ScanKey_Direct(void)
{
	uint32_t R_Tmp320, R_Tmp321, R_Tmp322;
	uint8_t  R_Temp80;
	
	// 正常按键处理
//	if(R_DirectKey_Scan_Delay < D_DirectKey_DelayTime)
//	{
//		//return;	
//	}
//	else
//	{
//		R_DirectKey_Scan_Delay = 0;

	
		//================================================
		// Key Debounce Time Count
		//================================================
		if(R_DebounceCnt < D_DirectKey_DebounceCnt_Max)
		{
			R_DebounceCnt++;
		}
		
		//================================================
		// 把所有端口的按键组合成一个处理
		//================================================
		R_Tmp320 = P_DirectInPort_1 & D_IO_Direct_Mask_1;
		R_Tmp321 = P_DirectInPort_2 & D_IO_Direct_Mask_2;
		R_Tmp322 = R_Tmp320 | (R_Tmp321 << 8);
	#ifdef SEN_PIN_READ_HIGH_TRIGGER
		// 传感器高电平触发
		R_Tmp322 ^= C_KEY_SEN_MASK;
	#endif
		
		//================================================
		//F_ScanDirect_Handle(R_Tmp322, 0);
		//================================================
		// Pull High Key
		R_Tmp320 = R_Tmp322 ^ D_IO_Direct_Mask;
		R_Tmp320 = R_Tmp320 & D_IO_Direct_Mask;
		
		R_Tmp321 = R_KeyPortReg;
		R_KeyPortReg = R_Tmp320;
		if(R_Tmp320 != R_Tmp321)
		{
			// 两次按键值不同，重新计算Debounce
			R_DebounceCnt = 0;
		}
		else
		{
			// 读取PORT的Debounce时间
			R_Temp80 = D_DirectKey_DebounceCnt_Max;
			if(R_DebounceCnt >= R_Temp80)
			{
				R_DebounceCnt = R_Temp80;
				R_NewKey = R_KeyPortReg;
				R_HisKey = R_NewKey & R_HisKey;
				R_NewKey = R_NewKey ^ R_HisKey;
				R_HisKey = R_NewKey | R_HisKey;
			}
		}
//	}
}


//==========================================================================
//功能：扫描按键
//输入：无
//输出：无
//==========================================================================
void F_ScanKey_Service(void)
{
	uint32_t R_Temp = 0;
	uint32_t R_HisKey_BackupBuf;

	// 每次MainFlow都需要清除NewKey
	R_NewKey = 0;
	R_HisKey_BackupBuf = R_HisKey;

	F_ScanKey_Direct();
	
	R_Temp = R_HisKey_BackupBuf ^ R_NewKey;
	R_ReleaseKey = R_Temp ^ R_HisKey;
	
	//================================================
	// 按键分灯
	//================================================
	// 主程序按键
	R_MainFlow_NewKey     = R_NewKey     & C_KEY_MAIN_MASK;
	R_MainFlow_HisKey     = R_HisKey     & C_KEY_MAIN_MASK;
	R_MainFlow_ReleaseKey = R_ReleaseKey & C_KEY_MAIN_MASK;

	// 传感器
	R_Sen_NewKey     = R_NewKey     & C_KEY_SEN_MASK;
	R_Sen_HisKey     = R_HisKey     & C_KEY_SEN_MASK;
	R_Sen_ReleaseKey = R_ReleaseKey & C_KEY_SEN_MASK;
}















