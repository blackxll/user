//==========================================================================
// Function Define:
// uint8_t F_dow_crc_byte(uint8_t old_crc, uint8_t dat)
// void F_dow_crc_blk2blk(uint8_t * pseek, uint8_t *pdat, uint8_t *pcrc, uint8_t len)
// void F_dow_crc_byt2blk(uint8_t seek, uint8_t *pdat, uint8_t *pcrc, uint8_t len)
// uint8_t F_dow_crc(uint8_t old_crc, uint8_t * pdat, uint8_t len)
// uint8_t F_dow_uncrc_byte(uint8_t crc, uint8_t old_crc)
// void F_dow_uncrc_blk2blk(uint8_t * pseek, uint8_t *pdat, uint8_t *pcrc, uint8_t len)
// void F_dow_uncrc_byt2blk(uint8_t seek, uint8_t *pdat, uint8_t *pcrc, uint8_t len)
// uint32_t F_Get_CheckSum(void * pdat, uint16_t len)
//==========================================================================
#include "Public.h"


const uint8_t crc_code[] = 
{
	0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
	157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
	35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
	190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
	70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
	219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
	101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
	248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
	140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
	17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
	175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
	50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
	202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
	87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
	233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
	116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};


uint8_t F_dow_crc_byte(uint8_t old_crc, uint8_t dat)
{
	return crc_code[old_crc ^ dat];
}


void F_dow_crc_blk2blk(uint8_t * pseek, uint8_t *pdat, uint8_t *pcrc, uint8_t len)
{
	for(;len != 0; len--)
	{
		*pcrc++ = crc_code[(*pseek++) ^ (*pdat++)]; 
	}
}


void F_dow_crc_byt2blk(uint8_t seek, uint8_t *pdat, uint8_t *pcrc, uint8_t len)
{
	for(;len != 0; len--)
	{
		*pcrc++ = crc_code[seek ^ (*pdat++)]; 
	}
}


uint8_t F_dow_crc(uint8_t old_crc, uint8_t * pdat, uint8_t len)
{
	for(; len != 0; len--)
	{
		old_crc = crc_code[old_crc ^ *pdat++];
	}
	return old_crc;
}


uint8_t F_dow_uncrc_byte(uint8_t crc, uint8_t old_crc)
{
	uint8_t j;
	j = 0;
	while(1)
	{
		if(crc_code[j] == crc)
		{
			break;
		}
		j++;
	}
	return (j ^ old_crc);
}


void F_dow_uncrc_blk2blk(uint8_t * pseek, uint8_t *pdat, uint8_t *pcrc, uint8_t len)
{
	for(; len > 0; len--)
	{
		*pdat++ = F_dow_uncrc_byte(*pcrc++, *pseek++);
	}
}


void F_dow_uncrc_byt2blk(uint8_t seek, uint8_t *pdat, uint8_t *pcrc, uint8_t len)
{
	for(;len != 0; len--)
	{
		*pdat++ = F_dow_uncrc_byte(*pcrc++, seek); 
	}
}



//==========================================================================
// FUNCTION: 生成CheckSum值
// INPUT:  
//			* padt -- 生成CheckSum的数组
//		   	   len -- 数组的长度
// OUTPUT:  计算的CheckSum值
//==========================================================================
uint32_t F_Get_CheckSum(void * pdat, uint16_t len)
{
	uint8_t crc;
	uint32_t checksum;

	crc = F_dow_crc(FMC_CRC_SEEK, (uint8_t *)pdat + 4, len - 4);
	checksum = 
	(
		((((uint32_t)(~crc ^ FMC_CRC_SEEK)) << 24) & 0xff000000) | 
		((((uint32_t)(crc ^ FMC_CRC_SEEK)) << 16) & 0x00ff0000) |
		((((uint32_t)~crc) << 8) & 0x0000ff00) |
		(((uint32_t)crc) & 0x000000ff)
	);
	return checksum;
}




