///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_IO.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/

#ifndef _IT6615_IO_h_
#define _IT6615_IO_h_
#include "iTE6615_Typedef.h"


u1 i2c_write_byte(u8 address, u8 offset, u8 byteno, u8 *p_data, u8 device);
u1 i2c_read_byte(u8 address, u8 offset, u8 byteno, u8 *p_data, u8 device);

#endif
