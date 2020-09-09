//==========================================================================
// Function:
// void F_GenerateCRC_Seek(uint8_t * ps)
// void DrvSYS_Delay(uint32_t us)
// uint8_t F_Save_Parameters(FMC_SAFETY_T * PT_Safety, SYS_SET_PARA_T * PT_SET_PARA)
// uint8_t F_Get_Parameters(void)
//==========================================================================
#include "Public.h"




const FMC_SAFETY_T	RT_Safety_Init = 
{
	(void *)0x06998099, 
	(void *)0xa33c063c, 
	(void *)0xc55a60f6, 
	(void *)0xf9665cef, 
	0, 0, 0, 0
};	

const FMC_SAFETY_T	RT_Safety_Blank = 
{
	(void *)((uint32_t *)&SYS->REGWRPROT + 0x10),
	(void *)((uint32_t *)&FMC->ISPCON + 0x20),
	(void *)((uint32_t *)&FMC->ISPCMD + 0x20),
	(void *)((uint32_t *)&FMC->ISPTRG + 0x20),
	0, 0, 0, 0
};

FMC_SAFETY_T	RT_Safety_Flash = 
{
	(void *)((uint32_t *)&SYS->REGWRPROT + 0x10),
	(void *)((uint32_t *)&FMC->ISPCON + 0x20),
	(void *)((uint32_t *)&FMC->ISPCMD + 0x20),
	(void *)((uint32_t *)&FMC->ISPCMD + 0x20),
	0, 0, 0, 0
};

FMC_SAFETY_T 	RT_Safety_Eeprom =
{
	(void *)((uint32_t *)&SYS->REGWRPROT + 0x10), 
	(void *)((uint32_t *)&FMC->ISPCON + 0x20), 
	(void *)((uint32_t *)&FMC->ISPCMD + 0x20), 
	(void *)((uint32_t *)&FMC->ISPCMD + 0x20),
	0, 0, 0, 0
};



//==========================================================================
// FUNCTION: 把数左移并反转，eg:0x03--> 0x3c(0b0011-->0b1100)
// INPUT:   
//			* ps -- 连续的4个字节的地址
// OUTPUT:  NONE
//==========================================================================
void F_GenerateCRC_Seek(uint8_t * ps)
{
	uint8_t i;
	uint8_t j;
	for(i = 0; i < 4; i++, ps++)
	{
		*ps <<= 4;
		for(j = 0; j < 4; j++)
		{
			*ps |= (((*ps >> (7 - j)) & 1) << j);
		}
	}
}


//==========================================================================
// FUNCTION: 延时子程序
// INPUT:   
//			us     - [in]      Delay time. The Max value is 335000  
// OUTPUT:  NONE
//==========================================================================
void DrvSYS_Delay(uint32_t us)
{
    // Waiting for down-count to zero
	uint32_t i;
	for(;us != 0; us--)
	{
		for (i = 20; i != 0; i--)
		{
			__nop();
		}
	}
}




//==============================================================================================================
//==============================================================================================================
// 设定参数存储
//==============================================================================================================
//==============================================================================================================


// 设定参数存储(存储到APROM),固定地址存储的参数，只用于存储时使用
SYS_SET_PARA_T	__at(APROM_SET_PARA_DATA_BASE_ADDR) R_SetPara_APROM;

// 设定参数(RAM)
SYS_SET_PARA_T	R_SetPara_RAM;


//==========================================================================
// FUNCTION: 把ADC参数存储到APROM里
// INPUT:   
//			* PT_Safety -- 写入的位置
//			* PT_ADC -- 写入的数据
// OUTPUT:  NONE
//==========================================================================
uint8_t F_Save_Parameters(FMC_SAFETY_T * PT_Safety, SYS_SET_PARA_T * PT_SET_PARA)
{
	uint8_t result = 0;
	uint32_t R_Register;
	uint32_t addr;
	uint32_t addr0 = APROM_SET_PARA_DATA_BASE_ADDR;
	
	// Check Process Data
	if(PT_Safety != &(RT_Safety_Flash))
	{
		return 1;
	}
	if(PT_SET_PARA != &(R_SetPara_RAM))
	{
		return 1;
	}
	
	// 读取存储ADC参数的APROM地址
	addr0 = (uint32_t)&R_SetPara_APROM;
	
	memcpy(PT_Safety, &RT_Safety_Blank, 16);
	if(PT_Safety->CRCPRT != 0x99)	result = 2;
	if(PT_Safety->CRCCON != 0x3c)	result = 2;
	if(PT_Safety->CRCCMD != 0x5a)	result = 2;
	if(PT_Safety->CRCTRG != 0x66)	result = 2;
	
	if(0 != result)
	{
		PT_Safety->CRCPRT = 0;
		PT_Safety->CRCCON = 0;
		PT_Safety->CRCCMD = 0;
		PT_Safety->CRCTRG = 0;
		return result;
	}
	
	// 
	memcpy(PT_Safety, &RT_Safety_Init, 16);
	
	// Get REGWRPROT Address
	F_dow_crc_byt2blk(PT_Safety->CRCPRT, (uint8_t *)&PT_Safety->PREGWRPROT, (uint8_t *)&R_Register, 4);
	if(R_Register != (uint32_t)&SYS->REGWRPROT)
	{
		// CRC Fail
		memcpy(PT_Safety, &RT_Safety_Init, 20);
		return 3;
	}
	
	// Unlock Register
	outpw(R_Register, 0x59);	//REGWRPROT = 0x59
	outpw(R_Register, 0x16);	//REGWRPROT = 0x16
	outpw(R_Register, 0x88);	//REGWRPROT = 0x88
	
	// 使能内部22M时钟，以免在写FLASH过程时，导致死机
	if(!(CLK->PWRCON & CLK_PWRCON_OSC22M_EN_Msk))
	{
		CLK->PWRCON |= CLK_PWRCON_OSC22M_EN_Msk;
		// Wait 22M stable
		DrvSYS_Delay(12);
	}
	// 使能FLASH ISP时钟
	CLK->AHBCLK |= CLK_AHBCLK_ISP_EN_Msk;
	
	
	// Get ISPCON Address
	F_dow_crc_byt2blk(PT_Safety->CRCCON, (uint8_t *)&PT_Safety->PISPCON, (uint8_t *)&R_Register, 4);
	if(R_Register != (uint32_t)&FMC->ISPCON)
	{
		result = 4;
		goto exit;
	}
	
	outpw(R_Register, (inpw(R_Register)|0x01));	// 使能ISP功能, ISPEN = 1;
	outpw(R_Register, (inpw(R_Register)|0x08));	// APROM更新使能, APUEN = 1;
	
	// Get ISPCMD Address
	F_dow_crc_byt2blk(PT_Safety->CRCCMD, (uint8_t *)&PT_Safety->PISPCMD, (uint8_t *)&R_Register, 4);
	if(R_Register != (uint32_t)&FMC->ISPCMD)
	{
		result = 5;
		goto exit;
	}
	
	// Earse Page Command(0x22)
	outpw(R_Register, (inpw(R_Register)&~0x01|0x02));
	outpw(R_Register, (inpw(R_Register)&~0x10));
	outpw(R_Register, (inpw(R_Register)|0x20));
	
	// 擦除页地址
	FMC->ISPADR = addr0;	//0x0001 0000

	// Get ISPTRG Address
	F_dow_crc_byt2blk(PT_Safety->CRCTRG, (uint8_t *)&PT_Safety->PISPTRG, (uint8_t *)&R_Register, 4);
	if(R_Register != (uint32_t)&FMC->ISPTRG)
	{
		result = 6;
		goto exit;
	}
	
	__set_PRIMASK(1);
	// 写1开始ISP操作，当ISP操作结束后，该位由硬件自动清零
	outpw(R_Register, 1);
	__ISB();
	// wait ISP finish 
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);
	__set_PRIMASK(0);
	
	// ISP失败标志
	if(FMC->ISPCON & FMC_ISPCON_ISPFF_Msk)
	{
		FMC->ISPCON |= FMC_ISPCON_ISPFF_Msk;
		result = 0x80;
		goto exit;
	}
	
	
	for(addr = addr0; addr < addr0 + sizeof(SYS_SET_PARA_T); addr += 4)
	{
		if(*((uint32_t *)addr) != 0xFFFFFFFF)
		{
			result = 0x40;
			goto exit;
		}
	
		F_dow_crc_byt2blk(PT_Safety->CRCCMD, (uint8_t *)&PT_Safety->PISPCMD, (uint8_t *)&R_Register, 4);
		if(R_Register != (uint32_t)&FMC->ISPCMD)
		{
			result = 7;
			goto exit;
		}
		// 进行写操作(0x21)
		outpw(R_Register, (inpw(R_Register)&~0x02|0x01));
		outpw(R_Register, (inpw(R_Register)&~0x10));
		outpw(R_Register, (inpw(R_Register)|0x20));
		
		FMC->ISPADR = addr;
		FMC->ISPDAT = *((uint32_t *)(addr + (uint32_t)PT_SET_PARA - addr0));
		
		F_dow_crc_byt2blk(PT_Safety->CRCTRG, (uint8_t *)&PT_Safety->PISPTRG, (uint8_t *)&R_Register, 4);
		if(R_Register != (uint32_t)&FMC->ISPTRG)
		{
			result = 8;
			goto exit;
		}
		__set_PRIMASK(1); 
		outpw(R_Register, 1);	// 写1开始ISP操作，当ISP操作结束后，该位由硬件自动清零
		__ISB();
		while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk);
		__set_PRIMASK(0);
		
		if(FMC->ISPCON & FMC_ISPCON_ISPFF_Msk)
		{
			// ISP Fail
			FMC->ISPCON |= FMC_ISPCON_ISPFF_Msk;
			result = 0x81;
			goto exit;
		}
	}
	
	// Write Data to Flash
	FMC->ISPCON &= (~FMC_ISPCON_ISPEN_Msk);
	CLK->AHBCLK &= (~CLK_AHBCLK_ISP_EN_Msk);
	//LOCKREG();
	outpw((uint32_t)&SYS->REGWRPROT, 0x00);
	memcpy(PT_Safety, &RT_Safety_Blank, sizeof(FMC_SAFETY_T));
	if(0 != memcmp((const void *)addr0, PT_SET_PARA, sizeof(SYS_SET_PARA_T)))
	{
		result = 0x41;
	}
	return result;
exit:
	FMC->ISPCON &= (~FMC_ISPCON_ISPEN_Msk);
	CLK->AHBCLK &= (~CLK_AHBCLK_ISP_EN_Msk);
	//LOCKREG();
	outpw((uint32_t)&SYS->REGWRPROT, 0x00);
	memcpy(PT_Safety, &RT_Safety_Blank, sizeof(FMC_SAFETY_T));
	return result;
}


//==========================================================================
// FUNCTION: 读取APROM里的参数
// INPUT:   NONE
// OUTPUT:  NONE
//==========================================================================
uint8_t F_Get_Parameters(void)
{
	if(R_SetPara_APROM.CheckSum != F_Get_CheckSum(&R_SetPara_APROM, sizeof(SYS_SET_PARA_T)))
	{
		return 0x01;
	}
	return 0x00;
}





