

//==========================================================================
//==========================================================================
//==========================================================================
// 注意：在写APROM或数据FLASH时，存储的地址必须以32字节存储
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================


//==========================================================================
// APROM
//==========================================================================
#define FMC_APROM_PAGE_SIZE			512		// APROM 1个扇区大小
#define FMC_APROM_PAGE_MAX			2		// APROM 2个扇区
#define FMC_APROM_BASE_ADDR			0xFC00	// APROM 的起始地址



//==========================================================================
// 寄存器处理
//==========================================================================
typedef struct 
{
	void * PREGWRPROT;
	void * PISPCON;
	void * PISPCMD;
	void * PISPTRG;
	uint8_t	CRCPRT;
	uint8_t CRCCON;
	uint8_t CRCCMD;
	uint8_t CRCTRG;
}FMC_SAFETY_T;

extern const FMC_SAFETY_T	RT_Safety_Init;
extern const FMC_SAFETY_T	RT_Safety_Blank;
extern 		 FMC_SAFETY_T	RT_Safety_Flash;
extern 		 FMC_SAFETY_T 	RT_Safety_Eeprom;



//==========================================================================
// FUNCTION: 把数左移并反转，eg:0x03--> 0x3c(0b0011-->0b1100)
// INPUT:   
//			* ps -- 连续的4个字节的地址
// OUTPUT:  NONE
//==========================================================================
extern void F_GenerateCRC_Seek(uint8_t * ps);


//==========================================================================
// FUNCTION: 延时子程序
// INPUT:   
//			us     - [in]      Delay time. The Max value is 335000  
// OUTPUT:  NONE
//==========================================================================
extern void DrvSYS_Delay(uint32_t us);




//==============================================================================================================
//==============================================================================================================
// 传感器参数存储
//==============================================================================================================
//==============================================================================================================

// 使用页数设定
#define APROM_SET_PARA_PAGE_START_NUM 	1
#define APROM_SET_PARA_PAGE_END_NUM 	1

// 存储参数的起始地址
#define APROM_SET_PARA_DATA_BASE_ADDR 	(FMC_APROM_BASE_ADDR + (FMC_APROM_PAGE_SIZE * APROM_SET_PARA_PAGE_START_NUM))


//==========================================================================
// 设定参数存储数据内容
//==========================================================================
typedef struct
{
	uint32_t CheckSum;
	uint32_t StairNum;		// 楼梯灯数量
	uint32_t FlowNum;		// 流水灯数量
	uint8_t  Para01;		// 参数01
	uint8_t  Para02;		// 参数02
	uint8_t  Para03;		// 参数03
	uint8_t  Para04;		// 参数04
	uint8_t  Para05;		// 参数05
	uint8_t  Para06;		// 参数06
	uint8_t  RunMode;		// 上次的运行模式
}SYS_SET_PARA_T;

// 设定参数
extern SYS_SET_PARA_T	R_SetPara_APROM;
extern SYS_SET_PARA_T	R_SetPara_RAM;


// 定义参数对应的变量
#define R_SetPara_StairLamp_Num				R_SetPara_RAM.StairNum
#define R_SetPara_FlowLED_Num				R_SetPara_RAM.FlowNum
#define R_SetPara_StairLamp_Time_LV			R_SetPara_RAM.Para01
#define R_SetPara_StairLamp_Bright_LV		R_SetPara_RAM.Para02
#define R_SetPara_StairLamp_DelayOffTime_LV	R_SetPara_RAM.Para03
#define R_SetPara_FlowLED_Time_LV			R_SetPara_RAM.Para04
#define R_SetPara_FlowLED_Bright_LV			R_SetPara_RAM.Para05
#define R_SetPara_FlowLED_DelayOffTime_LV	R_SetPara_RAM.Para06
#define R_SetPara_System_RunMode			R_SetPara_RAM.RunMode


// 设置参数级别选择
#define C_SET_PARA_LV1			0
#define C_SET_PARA_LV2			1
#define C_SET_PARA_LV3			2
#define C_SET_PARA_LV4			3
#define C_SET_PARA_LV5			4
#define C_SET_PARA_LV6			5
#define C_SET_PARA_LV7			6
#define C_SET_PARA_LV8			7
#define C_SET_PARA_MAX			8
#define C_SET_PARA_DEF			C_SET_PARA_LV1


// 系统运行模式
#define C_SYS_RUN_STD			0
#define C_SYS_RUN_STAIR_TEST	1
#define C_SYS_RUN_FLOW_TEST		2
#define C_SYS_RUN_ALL_TEST		3



//==========================================================================
// FUNCTION: 把设定参数存储到APROM里
// INPUT:   
//			* PT_Safety -- 写入的位置
//			* PT_SET_PARA -- 写入的数据
// OUTPUT:  NONE
//==========================================================================
extern uint8_t F_Save_Parameters(FMC_SAFETY_T * PT_Safety, SYS_SET_PARA_T * PT_SET_PARA);


//==========================================================================
// FUNCTION: 读取APROM里的设定参数
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
extern uint8_t F_Get_Parameters(void);












