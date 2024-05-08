/**
 * @file hal.h
 *
 * @brief hardware abstract layer header file
 */

#ifndef __hal_h
#define __hal_h

#include "av_config.h"

typedef struct
{
  uint8 regAddr;   /* Register address */
  uint8 mask;      /* Mask for the bits in RegAddr */
  uint8 lShift;    /* Absolute left shift (in 2's complement) of */
  uint8 Reserved;
}AvI2CFieldInfo;

/* hardware access functions */
typedef AvRet (*AvFpI2cRead)(uint32, uint32, uint8 *, uint16);
typedef AvRet (*AvFpI2cWrite)(uint32, uint32, uint8 *, uint16);
typedef AvRet (*AvFpUartSendByte)(uint8 *, uint16);
typedef AvRet (*AvFpUartGetByte)(uint8 *);
typedef AvRet (*AvFpGetMilliSecond)(uint32 *);
typedef AvRet (*AvFpGetKey)(uint8 *);
typedef AvRet (*AvFpGetIrda)(uint8 *);
#ifdef COMPILER_C51_MODE
#include "Gsv_bsp.h"
#define AvI2cRead         BspI2cRead
#define AvI2cWrite        BspI2cWrite
#define AvUartTxByte      BspUartSendByte
#define AvUartRxByte      BspUartGetByte
#define AvGetMilliSecond  BspGetMilliSecond
#define AvGetKey          BspGetKey
#define AvIrdaRxByte      BspIrdaGetByte
#else
#define AvI2cRead         AvHookI2cRead
#define AvI2cWrite        AvHookI2cWrite
#define AvUartTxByte      AvHookUartTxByte
#define AvUartRxByte      AvHookUartRxByte
#define AvGetMilliSecond  AvHookGetMilliSecond
#define AvGetKey          AvHookGetKey
#define AvIrdaRxByte      AvHookGetIrda
#endif

/* exported functions */
#ifdef __cplusplus
extern "C" {
#endif
AvRet AvHalI2cRead(pin uint32 devAddress, pin uint32 regAddress, pout uint8 *avdata, pin uint16 count);
AvRet AvHalI2cWrite(pin uint32 devAddress, pin uint32 regAddress, pin uint8 *avdata, pin uint16 count);
AvRet AvHalUartSendByte(pin uint8 *avdata, uint16 avsize);
AvRet AvHalUartGetByte(pout uint8 *avdata);
AvRet AvHalGetMilliSecond(pout uint32 *ms);
AvRet AvHalGetKey(uint8 *avdata);
AvRet AvHalGetIrda(pout uint8 *avdata);
AvRet AvHalGetElapsedMilliSecond(pin uint32 *oldTime, pout uint32 *elapsedTime);
AvRet AvHalGetTime(pout uint32 *day, pout uint32 *hour, pout uint32 *min, pout uint32 *sec);

AvRet AvHalI2cRdMultiField(pin uint32 devAddress, pin uint32 regAddress, pin uint16 number, pout uint8 *avdata);
AvRet AvHalI2cWrMultiField(pin uint32 devAddress, pin uint32 regAddress, pin uint16 number, pin uint8 *avdata);
AvRet AvHalI2cReadField8(pin uint32 devAddress, pin uint32 regAddress, pin uint8 mask, pin uint8 bitPos, pout uint8 *avdata);
AvRet AvHalI2cWriteField8(pin uint32 devAddress, pin uint32 regAddress, pin uint8 mask, pin uint8 bitPos, pin uint8 fieldVal);
AvRet AvHalI2cWriteOField8(pin uint32 devAddress, pin uint32 regAddress, pin uint8 mask, pin uint8 bitPos, pin uint8 *currVal, pin uint8 fieldVal);
AvRet AvHalI2cReadField32(pin uint32 devAddress, pin uint32 regAddress, pin uint8 msbMask, pin uint8 lsbMask, pin uint8 lsbPos, pin uint8 endian, pin uint8 fldSpan, pout uint32 *avdata);
AvRet AvHalI2cReadFlagField32(pin uint32 devAddress, pin uint32 *addrTable, pin uint8 msbMask, pin uint8 lsbMask, pin uint8 lsbPos, pin uint8 endian, pin uint8 fldSpan, pout uint32 *avdata);
AvRet AvHalI2cWriteField32(pin uint32 devAddress, pin uint32 regAddress, pin uint8 msbMask, pin uint8 lsbMask, pin uint8 lsbPos, pin uint8 endian, pin uint8 fldSpan, pin uint32 avdata);
AvRet AvHalI2cWriteFragField32(pin uint32 devAddress, pin uint32 *addrTable, pin uint8 msbMask, pin uint8 lsbMask, pin uint8 lsbPos, pin uint8 endian, pin uint8 fldSpan, pin uint32 avdata);
AvRet AvHalI2cReadRandField32(pin uint32 devAddress, pin AvI2CFieldInfo *fldInfo, pin uint8 fldSpan, pout uint32 *avdata);
AvRet AvHalI2cWriteRandField32(pin uint32 devAddress, pin AvI2CFieldInfo *fldInfo, pin uint8 fldSpan, pin uint32 avdata);

uint16 LookupValue8 (uchar *Table, uchar Value, uchar EndVal, uchar Step);
uint16 AvMemcpy(void *dst, void *src, uint32 count);
uint16 AvMemset(void *ptr, uint8 avdata, uint32 count);
uint16 AvMemcmp(const void * cs,const void * ct, unsigned int count);
uint16 AvStrcmp(const void * cs,const void * ct);
uint8 AvCheckSum(uint8 *avdata, uint16 TotalNumber, uint16 CheckSumPosition);
uint8 IsLetter(uint8 uChar);
// returns capital letter if it's a letter,
// or the number if it's a number and 0 if it's neither
bool IsBackSpace(uint8 uChar);
// returns true if it's a backspace
bool IsCR(uint8 uChar);
// returns true if it's a carriage return
bool IsSpace(uint8 uChar);
// returns true if it's a space
uint32 AsciiToNumber(uint8 *pString, uint8 Length);
void AvUselessFunction(AvPort *port, ...);

#define AvHalI2cReadMultiField(devAddress, regAddress, number, avdata)  \
            AvHalI2cRdMultiField(devAddress, (0x080000 | (regAddress)), number, avdata)
#define AvHalI2cWriteMultiField(devAddress, regAddress, number, avdata) \
            AvHalI2cWrMultiField(devAddress, (0x080000 | (regAddress)), number, avdata)
#define AvHalI2cReadField32BE(devAddress, regAddress, msbMask, lsbMask, lsbPos, fldSpan, avdata) \
            AvHalI2cReadField32(devAddress,  (0x080000 | (regAddress)), msbMask, lsbMask, lsbPos, AvBigEndian, fldSpan, avdata)
#define AvHalI2cReadField32LE(devAddress, regAddress, msbMask, lsbMask, lsbPos, fldSpan, avdata) \
            AvHalI2cReadField32(devAddress,  (0x080000 | (regAddress)), msbMask, lsbMask, lsbPos, AvLittleEndian, fldSpan, avdata)
#define AvHalI2cWriteField32BE(devAddress, regAddress, msbMask, lsbMask, lsbPos, fldSpan, avdata) \
            AvHalI2cWriteField32(devAddress, (0x080000 | (regAddress)), msbMask, lsbMask, lsbPos, AvBigEndian, fldSpan, avdata)
#define AvHalI2cWriteField32LE(devAddress, regAddress, msbMask, lsbMask, lsbPos, fldSpan, avdata) \
            AvHalI2cWriteField32(devAddress, (0x080000 | (regAddress)), msbMask, lsbMask, lsbPos, AvLittleEndian, fldSpan, avdata)

#define AvHalI2c16ReadMultiField(devAddress, regAddress, number, avdata) \
            AvHalI2cRdMultiField(devAddress, (0x100000 | (regAddress)), number, avdata)
#define AvHalI2c16WriteMultiField(devAddress, regAddress, number, avdata) \
            AvHalI2cWrMultiField(devAddress, (0x100000 | (regAddress)), number, avdata)
#define AvHalI2c16ReadField32BE(devAddress, regAddress, msbMask, lsbMask, lsbPos, fldSpan, avdata) \
            AvHalI2cReadField32(devAddress,  (0x100000 | (regAddress)), msbMask, lsbMask, lsbPos, AvBigEndian, fldSpan, avdata)
#define AvHalI2c16ReadField32LE(devAddress, regAddress, msbMask, lsbMask, lsbPos, fldSpan, avdata) \
            AvHalI2cReadField32(devAddress,  (0x100000 | (regAddress)), msbMask, lsbMask, lsbPos, AvLittleEndian, fldSpan, avdata)
#define AvHalI2c16WriteField32BE(devAddress, regAddress, msbMask, lsbMask, lsbPos, fldSpan, avdata) \
            AvHalI2cWriteField32(devAddress, (0x100000 | (regAddress)), msbMask, lsbMask, lsbPos, AvBigEndian, fldSpan, avdata)
#define AvHalI2c16WriteField32LE(devAddress, regAddress, msbMask, lsbMask, lsbPos, fldSpan, avdata) \
            AvHalI2cWriteField32(devAddress, (0x100000 | (regAddress)), msbMask, lsbMask, lsbPos, AvLittleEndian, fldSpan, avdata)

#ifdef __cplusplus
}
#endif
#endif
