

//==============================================================================================================
//==============================================================================================================
// 发送数据时间说明(时间只能作为参考, 最后请用示波器测量实际时间)
//==============================================================================================================
//==============================================================================================================

//==========================================================================
// GS1903同时控制3个LED(1个流水单位为3个LED)
// SET_FLOW_LED_FLOW_NUM_MAX == N_FLOW_LED_GS1903_NUM_MAX
//==========================================================================
// 1、2线发送数据:
//   SendData:
//		GS1903 = 300个, MainFlow = 14.0ms, SendTime = 13.5ms
//		GS1903 = 400个, MainFlow = 18.0ms, SendTime = 17.5ms
//   SendPort:
//		GS1903 = 300个, MainFlow = 12.5ms, SendTime = 12.0ms
//		GS1903 = 400个, MainFlow = 17.0ms, SendTime = 16.0ms

// 2、5线发送数据:
//   SendData:
//		GS1903 = 300个, MainFlow = 14.5ms, SendTime = 14.0ms
//		GS1903 = 400个, MainFlow = 18.0ms, SendTime = 17.0ms
//   SendPort:
//		GS1903 = 300个, MainFlow = 12.5ms, SendTime = 12.0ms
//		GS1903 = 400个, MainFlow = 16.5ms, SendTime = 16.0ms

// 3、单线发送数据:
//		GS1903 = 300个, MainFlow = 11.0ms, SendTime = 11.0ms
//		GS1903 = 400个, MainFlow = 15.0ms, SendTime = 14.0ms

//==========================================================================
// GS1903分别控制3个LED(1个流水单位为1个LED)
// SET_FLOW_LED_FLOW_NUM_MAX == (N_FLOW_LED_GS1903_NUM_MAX * 3)
//==========================================================================
// 1、2线发送数据:
//   SendData:
//		GS1903 = 300个, MainFlow = 15.0ms, SendTime = 14.0ms
//		GS1903 = 400个, MainFlow = 20.0ms, SendTime = 18.0ms
//   SendPort:
//		GS1903 = 300个, MainFlow = 14.0ms, SendTime = 12.5ms
//		GS1903 = 400个, MainFlow = 18.0ms, SendTime = 16.5ms

// 2、5线发送数据:
//   SendData:
//		GS1903 = 300个, MainFlow = 15.0ms, SendTime = 14.0ms
//		GS1903 = 400个, MainFlow = 20.0ms, SendTime = 19.0ms
//   SendPort:
//		GS1903 = 300个, MainFlow = 13.5ms, SendTime = 12.0ms
//		GS1903 = 400个, MainFlow = 18.0ms, SendTime = 16.5ms

// 3、单线发送数据:
//		GS1903 = 300个, MainFlow = 12.5ms, SendTime = 11.5ms
//		GS1903 = 400个, MainFlow = 16.5ms, SendTime = 15.0ms

//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================



//==========================================================================
// GS1903控制IO定义
//==========================================================================
#define P_GS1903_DIN_GPIO1		P2
#define P_GS1903_DIN_PORT1		P2->DOUT
#define P_GS1903_DIN_PIN1		P2->PIN
//======================================
#define C_STAIR_DIN_BIT			BIT3
#define P_STAIR_DIN				P23
//======================================
#define C_FLOW01_DIN_BIT		BIT4
#define P_FLOW01_DIN			P24
//======================================
#define C_GS1903_DIN_MASK1		(C_STAIR_DIN_BIT | C_FLOW01_DIN_BIT)


//==========================================================================
// 输出控制重定义
//==========================================================================
#define C_GS1903_DIN1_BIT		C_STAIR_DIN_BIT
#define C_GS1903_DIN2_BIT		C_FLOW01_DIN_BIT
//======================================
#define C_GS1903_1DIN_MASK		(C_GS1903_DIN1_BIT)
#define C_GS1903_2DIN_MASK		(C_GS1903_DIN1_BIT | C_GS1903_DIN2_BIT)



//==========================================================================
// GS1903输出时间定义(CLK = 50MHz, 1个NOP = 20ns)
// 发送0码和1码的周期时间为1.25us(高速模式, 频率800KHz)
// DAT = 0, 高电平300ns, 低电平1000ns
// DAT = 1, 高电平1000ns, 低电平300ns
//==========================================================================
#define DELAY_1NOP()	__NOP();
#define DELAY_2NOP()	__NOP();__NOP();
#define DELAY_3NOP()	__NOP();__NOP();__NOP();
#define DELAY_4NOP()	__NOP();__NOP();__NOP();__NOP();
#define DELAY_5NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();
#define DELAY_7NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();
#define DELAY_10NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	
#define DELAY_11NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();
#define DELAY_18NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();
#define DELAY_23NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();
#define DELAY_25NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();
#define DELAY_26NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();
#define DELAY_35NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();
#define DELAY_40NOP()	__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();	\
						__NOP();__NOP();__NOP();__NOP();__NOP();


//==========================================================================
// FUNCTION: GS1903初始化IO
// INPUT:    NONE
// OUTPUT:   NONE
//==========================================================================
extern void F_GS1903_GPIO_Init(void);


//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================


//==========================================================================
// FUNCTION: 单个DIN发送输出数据
// INPUT:    
//			 *port      -- DIN引脚GPIO口
//			 u32PinMask -- DIN引脚位
//			 R_DAT      -- DIN输出数据
// OUTPUT:   NONE
// NOTE:	 发送400个数据时, MAINFLOW用时20ms, GS用时18.5ms
//==========================================================================
extern void F_GS1903_1Din_SendData(GPIO_T *port, uint32_t u32PinMask, uint8_t R_DAT);


//==========================================================================
// FUNCTION: 单个DIN发送引脚必须在一个PORT, 其它引脚可用作其它用途
// INPUT:    
//			 u32PinMask -- DIN引脚位
//			 R_DAT      -- DIN输出数据
// OUTPUT:   NONE
//==========================================================================
extern void F_GS1903_1Din_SendPort(uint32_t u32PinMask, uint8_t R_DAT);


//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================


//==========================================================================
// FUNCTION:  2个DIN发送引脚必须在一个PORT, 其它引脚可用作其它用途
// INPUT:    
//			 R_DAT1 -- DIN1引脚发送的数据
//			 R_DAT2 -- DIN2引脚发送的数据
// OUTPUT:   NONE
//==========================================================================
extern void F_GS1903_2Din_SendData(uint8_t R_DAT1, uint8_t R_DAT2);


//==========================================================================
// FUNCTION: 2个DIN发送引脚必须在一个PORT, 其它引脚只能用作DIN或悬空
// INPUT:    
//			 R_DAT1 -- DIN1引脚发送的数据
//			 R_DAT2 -- DIN2引脚发送的数据
// OUTPUT:   NONE
//==========================================================================
extern void F_GS1903_2Din_SendPort(uint8_t R_DAT1, uint8_t R_DAT2);


//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================

#if (SET_FLOW_LED_FLOW_NUM_MAX == N_FLOW_LED_GS1903_NUM_MAX * 3)
//==========================================================================
// FUNCTION: 1条GS1903的DIN控制线同时发送数据
// INPUT:    NONE
// OUTPUT:   NONE
// 备注:     GS1903分别控制3个LED(1个流水单位为1个LED)
//==========================================================================
extern void F_Refresh_GS1903_All_For1Din(void);


//==========================================================================
// FUNCTION: 2条GS1903的DIN控制线同时发送数据
// INPUT:    NONE
// OUTPUT:   NONE
// 备注:     GS1903分别控制3个LED(1个流水单位为1个LED)
//==========================================================================
extern void F_Refresh_GS1903_All_For2Din(void);


//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================


#elif (SET_FLOW_LED_FLOW_NUM_MAX == N_FLOW_LED_GS1903_NUM_MAX)
//==========================================================================
// FUNCTION: 1条GS1903的DIN控制线同时发送数据
// INPUT:    NONE
// OUTPUT:   NONE
// 备注:     GS1903同时控制3个LED(1个流水单位为3个LED)
//==========================================================================
extern void F_Refresh_GS1903_All_For1Din(void);


//==========================================================================
// FUNCTION: 2条GS1903的DIN控制线同时发送数据
// INPUT:    NONE
// OUTPUT:   NONE
// 备注:     GS1903同时控制3个LED(1个流水单位为3个LED)
//==========================================================================
extern void F_Refresh_GS1903_All_For2Din(void);


#endif
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//==============================================================================================================



