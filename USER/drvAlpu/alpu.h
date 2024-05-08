
#ifndef __ALPU_H_
#define __ALPU_H_

/***************************************************************************************
* Define
***************************************************************************************/
#define ERROR_CODE_TRUE			0
#define ERROR_CODE_FALSE		1
#define ERROR_CODE_WRITE_ADDR	10
#define ERROR_CODE_WRITE_DATA	20
#define ERROR_CODE_READ_ADDR	30
#define ERROR_CODE_READ_DATA	40
#define ERROR_CODE_START_BIT	50
#define ERROR_CODE_APROCESS		60
#define ERROR_CODE_DENY			70

/***************************************************************************************
* Global Variable
***************************************************************************************/

/***************************************************************************************
* Function Proto Type
***************************************************************************************/

/***************************************************************************************
* Extern Function
***************************************************************************************/
 // Write Addr : 0x7a, Read Addr, 0x7a | 0x01
extern unsigned char _alpu_rand(void);
extern unsigned char _i2c_write(unsigned char , unsigned char, unsigned char *, int);
extern unsigned char _i2c_read(unsigned char , unsigned char, unsigned char *, int);

extern int alpu_func(void);
/* EOF */
#endif

