



// CRC Generate
#define FMC_CRC_SEEK 0XA5




extern uint8_t F_dow_crc_byte(uint8_t old_crc, uint8_t dat);

extern void F_dow_crc_blk2blk(uint8_t * pseek, uint8_t *pdat, uint8_t *pcrc, uint8_t len);

extern void F_dow_crc_byt2blk(uint8_t seek, uint8_t *pdat, uint8_t *pcrc, uint8_t len);

extern uint8_t F_dow_crc(uint8_t old_crc, uint8_t * pdat, uint8_t len);

extern uint8_t F_dow_uncrc_byte(uint8_t crc, uint8_t old_crc);

extern void F_dow_uncrc_blk2blk(uint8_t * pseek, uint8_t *pdat, uint8_t *pcrc, uint8_t len);

extern void F_dow_uncrc_byt2blk(uint8_t seek, uint8_t *pdat, uint8_t *pcrc, uint8_t len);


//==========================================================================
// FUNCTION: 生成CheckSum值
// INPUT:  
//			* padt -- 生成CheckSum的数组
//		   	   len -- 数组的长度
// OUTPUT:  计算的CheckSum值
//==========================================================================
extern uint32_t F_Get_CheckSum(void * pdat, uint16_t len);


