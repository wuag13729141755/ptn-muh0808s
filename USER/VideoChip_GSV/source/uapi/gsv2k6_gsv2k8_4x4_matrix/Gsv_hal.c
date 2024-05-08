/**
 * @file hal.c
 *
 * @brief hardare abstract layer implmentation
 */
#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "Gsv_hal.h"
#include "Gsv_uapi.h"

/**
 * @brief  abstract i2c read function
 * @param  devAddress = device address
 * @param  regAddress = register address
 * @param  avdata = reading back data
 * @param  count = how many registers to read
 * @return AvOk if success, else AvError
 * @note   none
 */
AvRet AvHalI2cRead(pin uint32 devAddress, pin uint32 regAddress, pout uint8 *avdata, pin uint16 count)
{
    AvRet ret = AvOk;
    ret = AvI2cRead(devAddress, regAddress, avdata, count);
    return ret;
}

/**
 * @brief  abstract i2c write funciton
 * @param  devAddress = device address
 * @param  regAddress = register address
 * @param  avdata = data to write
 * @param  count = how many registers to write
 * @return AvOk if successs
 * @note   none
 */
AvRet AvHalI2cWrite(pin uint32 devAddress, pin uint32 regAddress, pin uint8 *avdata, pin uint16 count)
{
    AvRet ret = AvOk;
    ret = AvI2cWrite(devAddress, regAddress, avdata, count);
    return ret;
}

/**
 * @brief  abstract uart function to transmit a byte
 * @return AvOk if success
 * @note   none
 */
AvRet AvHalUartSendByte(pin uint8 *avdata, uint16 avsize)
{
    AvRet ret = AvOk;
    ret = AvUartTxByte(avdata, avsize);
    return ret;
}

/**
 * @brief  abstract uart function to receive a byte
 * @return AvOk if success
 * @note   none
 */
AvRet AvHalUartGetByte(pout uint8 *avdata)
{
    AvRet ret = AvOk;
    ret = AvUartRxByte(avdata);
    return ret;
}

/**
 * @brief  abstract timer function to read current time in millisencond
 * @return AvOk if success
 * @note   none
 */
AvRet AvHalGetMilliSecond(pout uint32 *ms)
{
    AvRet ret = AvOk;
    ret = AvGetMilliSecond(ms);
    return ret;
}

/**
 * @brief  abstract key function to read current key value
 * @return AvOk if success
 * @note   none
 */
AvRet AvHalGetKey(uint8 *avdata)
{
    AvRet ret = AvOk;
    ret = AvGetKey(avdata);
    return ret;
}

/**
 * @brief  abstract timer function to how much time has elapsed
 * @return AvOk if success
 */
AvRet AvHalGetElapsedMilliSecond(pin uint32 *oldTime, pout uint32 *elapsedTime)
{
    AvRet ret = AvOk;
    uint32 currTime = 0;
    AvGetMilliSecond(&currTime);
    if (currTime > *oldTime) /* not overflow */
        *elapsedTime = (currTime - *oldTime);
    else
        *elapsedTime = currTime + (0xffffffff - *oldTime);
    return ret;
}

/**
 * @brief  abstract timer function to how much time has elapsed
 * @return AvOk if success
 */
AvRet AvHalGetTime(pout uint32 *day, pout uint32 *hour, pout uint32 *min, pout uint32 *sec)
{
    AvRet ret = AvOk;
    uint32 currTime = 0;
    AvGetMilliSecond(&currTime);
    *sec = currTime / 1000;
    *min = (*sec)   / 60;
    *hour = (*min)  / 60;
    *day = (*hour)  / 24;
    return ret;
}

/**
 * @brief  read multi fields from multi registers
 * @param  devAddress = device address
 * @param  regAddress = register address
 * @param  number = how many registers to read
 * @param  avdata = reading back data
 * @return AvOk if success, else AvError
 */
AvRet AvHalI2cRdMultiField(pin uint32 devAddress, pin uint32 regAddress, pin uint16 number, pout uint8 *avdata)
{
    AvRet ret = AvOk;

    ret = AvI2cRead(devAddress, regAddress, avdata, number);

    return ret;
}

/**
 * @brief  write multi fields to multi registers
 * @param  devAddress = device address
 * @param  regAddress = register address
 * @param  number = how many registers to write
 * @param  data = data to write
 * @return AvOk if success, else AvError
 */
AvRet AvHalI2cWrMultiField(pin uint32 devAddress, pin uint32 regAddress, pin uint16 number, pin uint8 *avdata)
{
    AvRet ret = AvOk;

    ret = AvI2cWrite(devAddress, regAddress, avdata, number);

    return ret;
}


/**
 * @brief  read up to 8-bit field from a single 8-bit register
 * @param  devAddress = device address
 * @param  regAddress = register address
 * @param  mask = field mask
 * @param  bitPos = fild lsb position in the register (0-7)
 * @param  avdata = reading back data
 * @return AvOk if success, else AvError
 * @note   example
 * @note           .
 * @note      |___***__|  mask = 0x1C     bitPos = 2
 */
AvRet AvHalI2cReadField8(pin uint32 devAddress, pin uint32 regAddress, pin uint8 mask, pin uint8 bitPos, pout uint8 *avdata)
{
    AvRet ret = AvOk;
    ret = AvI2cRead(devAddress, regAddress, avdata, 1);
    *avdata = (*avdata & mask) >> bitPos;
    return ret;
}

/**
 * @brief  write up to 8-bit field to a single 8-bit register
 * @param  devAddress = device address
 * @param  regAddress = register address
 * @param  mask = field mask
 * @param  bitPos = field lsb position in the register (0-7)
 *         Set to 0 if fieldVal is in correct position of the reg
 * @param  fieldVal = value (in the LSBits) of the field to be written
 *         if fieldVal is already in the correct position (i.e. does not need to be shifted,) set bitPos to 0
 * @return AvOk if success, else AvError
 * @note   example
 * @note          .
 * @note     |___****_|  mask = 0x1E     bitPos = 1
 */
AvRet AvHalI2cWriteField8(pin uint32 devAddress, pin uint32 regAddress, pin uint8 mask, pin uint8 bitPos, pin uint8 fieldVal)
{
    AvRet ret = AvOk;
    uint8 val = fieldVal;
    if(mask != 0xff)
    {
        AvI2cRead(devAddress, regAddress, &val, 1);
        val = (val & ~mask) | ((fieldVal << bitPos) & mask);
    }
    ret = AvI2cWrite(devAddress, regAddress, &val, 1);
    return ret;
}


/**
 * @brief  read up to 32-bit field from two or more consecutive 8-bit registers
 * @param  devAddress = device address
 * @param  regAddress = register address
 * @param  msbMask = mask for MSBits
 * @param  lsbMask = mask for LSBits
 * @param  lsbPos = LSbit position (0-7)
 * @param  endian = big endian or little endian
 * @param  fldSpan = number of consecutive registers containing field bits (Maximum 5 registers)
 * @param  avdata = reading back data
 * @return AvOk if success, else AvError
 * @note   example
 * @note     Start Reg Addr-> |     ***|  MSbits          MsbMask = 0x07
 * @note                      |********|  Middle bits
 * @note                      |********|  Middle bits
 * @note                      |********|  Middle bits
 * @note     End Reg Addr---> |**______|  LSbits          LsbMask = 0xC0, LsbPos = 6
 * @note                                                  FldSpan = 5
 */
AvRet AvHalI2cReadField32(pin uint32 devAddress, pin uint32 regAddress, pin uint8 msbMask, pin uint8 lsbMask, pin uint8 lsbPos, pin uint8 endian, pin uint8 fldSpan, pout uint32 *avdata)
{
    AvRet ret = AvOk;
    uint8 i, j, bytes[5];
    *avdata = 0;

    ret = AvI2cRead(devAddress, regAddress, bytes, fldSpan);

    if (endian == AvBigEndian)
    {
        bytes[0] &= msbMask;
        fldSpan--;
        bytes[fldSpan] &= lsbMask;

        j = (fldSpan << 3) - lsbPos;  /* j = how many total bits */
        for (i=0; i<fldSpan; i++)
        {
            *avdata |= (((uint32)(bytes[i])) << j);
            j -= 8;
        }
        *avdata |= (uint32)((bytes[fldSpan]) >> lsbPos);
    }
    else
    {
        bytes[0] &= lsbMask;
        fldSpan--;
        bytes[fldSpan] &= msbMask;

        j = (fldSpan << 3) - lsbPos;
        for (i=fldSpan; i>0; i--)
        {
            *avdata |= (((uint32)(bytes[i])) << j);
            j -= 8;
        }
        *avdata |= (uint32)((bytes[0]) >> lsbPos);
    }
    return ret;
}

/**
 * @brief  Write up to 32-bit field to two or more consecutive 8-bit registers
 * @param  devAddress = device address
 * @param  regAddress = register address
 * @param  msbMask = mask for MSBits
 * @param  lsbMask = mask for LSBits
 * @param  lsbPos = LSBit position (0-7)
 * @param  endian = big endian or little endian
 * @param  fldSpan = number of consecutive registers containing field bits (Maximum 5 registers)
 * @param  avdata = value to be written
 * @return AvOk if success, else AvError
 * @note   example
 * @note     Start Reg Addr-> |   *****|  MSBits          MsbMask = 0x1F
 * @note                           .
 * @note                      |********|  Middle bits
 * @note                           .
 * @note     End Reg Addr---> |******__|  LSBits          LsbMask = 0xFC, LsbPos = 2
 * @note                                  FldSpan = 3
 */
AvRet AvHalI2cWriteField32 (pin uint32 devAddress, pin uint32 regAddress, pin uint8 msbMask, pin uint8 lsbMask, pin uint8 lsbPos, pin uint8 endian, pin uint8 fldSpan, pin uint32 avdata)
{
    AvRet ret = AvOk;
    uint8 i, bytes[5];

    ret = AvI2cRead(devAddress, regAddress, bytes, fldSpan);

    if (endian == AvBigEndian)
    {
        bytes[0] = (bytes[0] & (~msbMask)) |
                       ((avdata >> (8 * (fldSpan - 1) - lsbPos)) & msbMask);
        for (i=1; i<fldSpan-1; i++)
        {
            bytes[i] = avdata >> (8 * (fldSpan - i - 1) - lsbPos);
        }
        bytes[fldSpan-1] = (bytes[fldSpan-1] & ~lsbMask) |
                       (uint8) ((avdata << lsbPos) & lsbMask);
        ret = AvI2cWrite(devAddress, regAddress, bytes, fldSpan);
    }
    else
    {
        bytes[0] = (bytes[0] & (~lsbMask)) |
                       (uint8) ((avdata << lsbPos) & lsbMask);
        for (i=1; i<fldSpan-1; i++)
        {
            bytes[i] = avdata >> (8 * (fldSpan - i - 1) - lsbPos);
        }
        bytes[fldSpan-1] = (bytes[fldSpan-1] & ~msbMask) |
                       ((avdata >> (8 * (fldSpan - 1) - lsbPos)) & msbMask);
        ret = AvI2cWrite(devAddress, regAddress, bytes, fldSpan);
    }
    return ret;
}

/*===========================================================================
 * Look for UCHAR Value in Table with Step increments
 *
 * Return:  Offset of matching value or end value
 *
 *==========================================================================*/
uint16 LookupValue8 (uchar *Table, uchar Value, uchar EndVal, uchar Step)
{
    uint16 i=0;

    while (Table[i] != EndVal)
    {
        if (Table[i] == Value)
        {
            break;
        }
        i+= Step;
    }
    return(i);
}

uint16 AvMemcpy(void *dst, void *src, uint32 count)
{
    if (count)
    {
        if (!dst || !src)
        {
            return false;
        }

        while (count)
        {
            *(char *)dst = *(char *)src;
            dst = ((char *)dst)+1;
            src = ((char *)src)+1;
            count--;
        }
    }

    return true;
}

uint16 AvMemset(void *ptr, uint8 avdata, uint32 count)
{
    if(count)
    {
        if(!ptr)
        {
            return false;
        }
        while (count)
        {
            *(char *)ptr = avdata;
            ptr = (char *)ptr + 1;
            count --;
        }
    }
    return true;
}

uint16 AvMemcmp(const void * cs,const void * ct, unsigned int count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for(su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
    if ((res = *su1 - *su2) != 0)
    {
        break;
    }
    return (res);
}

uint16 AvStrcmp(const void * cs,const void * ct)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for(su1 = cs, su2 = ct; ; ++su1, ++su2)
    {
        if((IsLetter(*su1) && IsLetter(*su2)) == 0)
            break;
        if ((res = *su1 - *su2) != 0)
            break;
    }
    return (res);
}

uint8 AvCheckSum(uint8 *avdata, uint16 TotalNumber, uint16 CheckSumPosition)
{
    uint16 i = 0;
    uint8  Checksum = 0;
    avdata[CheckSumPosition] = 0x00;
    for (i=0; i<TotalNumber; i++)
    {
        Checksum += avdata[i];
    }
    Checksum = 0x100 - Checksum;
    avdata[CheckSumPosition] = Checksum;
    return Checksum;
}

uint8 IsLetter(uint8 uChar)
{
    if (uChar>='A' && uChar <= 'Z') return uChar;
    if (uChar>='a' && uChar <= 'z') return (uChar - 0x20);
    if (uChar>='0' && uChar <= '9') return uChar;
    if (uChar=='+' || uChar=='-') return uChar;
    if (uChar=='?') return uChar;
    return 0;
}

bool IsBackSpace(uint8 uChar)
{
    if (uChar==0x08) return true;
    else return false;
}

bool IsCR(uint8 uChar)
{
    if (uChar==0xD) return true;
    else return false;
}

bool IsSpace(uint8 uChar)
{
    if (uChar==0x20) return true;
    else return false;
}

uint32 AsciiToNumber(uint8 *pString, uint8 Length)
{
    uint8 i = 0;
    uint32 value = 0;
    uint8 temp;

    for (i=0; i<Length; i++)
    {
        value = value * 16;
        temp = IsLetter(pString[i]);
        if (temp >='A' && temp <='F')
        {
            value += (temp - 'A') + 10;
        }
        else if (temp >= '0' && temp <= '9')
        {
            value += (temp - '0');
        }
        else return 0;
    }

    return value;
}
/**
 * @brief  abstract uart irda function to receive a byte
 * @return AvOk if success
 * @note   none
 */
AvRet AvHalGetIrda(uint8 *avdata)
{
    AvRet ret = AvOk;
    ret = AvIrdaRxByte(avdata);
    return ret;
}

#ifdef COMPILER_C51_MODE
void AvUselessFunction(AvPort *port, ...)
{
    return;
}
#endif

#endif

