///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <nop.h>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
#ifndef _IT6602_NOP_H_
#define _IT6602_NOP_H_

#ifdef _MCU_8051_

#include <intrins.h>
#include "it6602_mcu_conf.h"

#define nop1 {\
    _nop_();}

#define nop2 {\
    _nop_();_nop_();}

#define nop3 {\
    _nop_();_nop_();_nop_();}

#define nop4 {\
    _nop_();_nop_();_nop_();_nop_();}

#define nop5 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop6 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop7 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop8 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop9 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop10 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop11 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop12 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop13 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop14 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop15 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop16 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop17 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop18 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop19 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop20 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop21 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop22 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop23 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop24 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop25 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop26 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop27 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop28 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop29 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop30 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop31 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop32 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop33 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop34 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop35 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop36 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop37 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop38 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop39 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop40 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop41 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop42 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop43 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop44 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop45 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop46 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop47 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop48 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop49 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop50 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop51 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop52 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop53 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop54 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop55 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop56 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop57 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop58 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop59 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop60 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop61 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop62 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop63 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop64 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop65 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop66 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop67 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop68 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop69 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop70 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop71 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop72 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop73 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop74 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop75 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop76 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop77 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop78 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop79 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop80 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop81 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop82 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop83 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop84 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop85 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop86 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop87 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop88 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop89 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop90 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop91 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop92 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop93 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop94 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop95 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    }

#define nop96 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();}

#define nop97 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();}

#define nop98 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();}

#define nop99 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();}

#define nop100 {\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();\
    _nop_();_nop_();_nop_();_nop_();_nop_();}

#else // not _MCU_8051

    #define nop1
    #define nop2
    #define nop3
    #define nop4
    #define nop5
    #define nop6
    #define nop7
    #define nop8
    #define nop9
    #define nop10
    #define nop11
    #define nop12
    #define nop13
    #define nop14
    #define nop15
    #define nop16
    #define nop17
    #define nop18
    #define nop19
    #define nop20
    #define nop21
    #define nop22
    #define nop23
    #define nop24
    #define nop25
    #define nop26
    #define nop27
    #define nop28
    #define nop29
    #define nop30
    #define nop31
    #define nop32
    #define nop33
    #define nop34
    #define nop35
    #define nop36
    #define nop37
    #define nop38
    #define nop39
    #define nop40
    #define nop41
    #define nop42
    #define nop43
    #define nop44
    #define nop45
    #define nop46
    #define nop47
    #define nop48
    #define nop49
    #define nop50
    #define nop51
    #define nop52
    #define nop53
    #define nop54
    #define nop55
    #define nop56
    #define nop57
    #define nop58
    #define nop59
    #define nop60
    #define nop61
    #define nop62
    #define nop63
    #define nop64
    #define nop65
    #define nop66
    #define nop67
    #define nop68
    #define nop69
    #define nop70
    #define nop71
    #define nop72
    #define nop73
    #define nop74
    #define nop75
    #define nop76
    #define nop77
    #define nop78
    #define nop79
    #define nop80
    #define nop81
    #define nop82
    #define nop83
    #define nop84
    #define nop85
    #define nop86
    #define nop87
    #define nop88
    #define nop89
    #define nop90
    #define nop91
    #define nop92
    #define nop93
    #define nop94
    #define nop95
    #define nop96
    #define nop97
    #define nop98
    #define nop99
    #define nop100

#endif

#if ((CLOCK/(MachineCycle*500000L))>=60)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>60L), 1us = 30.0 nop, stall1us = 0 nop, stall5us = 150.0 nop, stall10us = 300 nop")
    #define stall1us {nop30;}
    #define stall2us {nop60;}
    #define stall3us {nop90;}
    #define stall4us {nop100;nop20;}
    #define stall5us {nop100;nop50;}
    #define stall6us {nop100;nop80;}
    #define stall7us {nop100;nop100;nop10;}
    #define stall8us {nop100;nop100;nop40;}
    #define stall9us {nop100;nop100;nop70;}
    #define stall10us {nop100;nop100;nop100;}
#elif ((CLOCK/(MachineCycle*500000L))>=59)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>59L), 1us = 29.5 nop, stall1us = 3 nop, stall5us = 147.5 nop, stall10us = 295 nop")
    #define stall1us {nop30;}
    #define stall2us {nop59;}
    #define stall3us {nop89;}
    #define stall4us {nop100;nop18;}
    #define stall5us {nop100;nop48;}
    #define stall6us {nop100;nop77;}
    #define stall7us {nop100;nop100;nop7;}
    #define stall8us {nop100;nop100;nop36;}
    #define stall9us {nop100;nop100;nop66;}
    #define stall10us {nop100;nop100;nop95;}
#elif ((CLOCK/(MachineCycle*500000L))>=58)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>58L), 1us = 29.0 nop, stall1us = 2 nop, stall5us = 145.0 nop, stall10us = 290 nop")
    #define stall1us {nop29;}
    #define stall2us {nop58;}
    #define stall3us {nop87;}
    #define stall4us {nop100;nop16;}
    #define stall5us {nop100;nop45;}
    #define stall6us {nop100;nop74;}
    #define stall7us {nop100;nop100;nop3;}
    #define stall8us {nop100;nop100;nop32;}
    #define stall9us {nop100;nop100;nop61;}
    #define stall10us {nop100;nop100;nop90;}
#elif ((CLOCK/(MachineCycle*500000L))>=57)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>57L), 1us = 28.5 nop, stall1us = 2 nop, stall5us = 142.5 nop, stall10us = 285 nop")
    #define stall1us {nop29;}
    #define stall2us {nop57;}
    #define stall3us {nop86;}
    #define stall4us {nop100;nop14;}
    #define stall5us {nop100;nop43;}
    #define stall6us {nop100;nop71;}
    #define stall7us {nop100;nop100;}
    #define stall8us {nop100;nop100;nop28;}
    #define stall9us {nop100;nop100;nop57;}
    #define stall10us {nop100;nop100;nop85;}
#elif ((CLOCK/(MachineCycle*500000L))>=56)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>56L), 1us = 28.0 nop, stall1us = 2 nop, stall5us = 140.0 nop, stall10us = 280 nop")
    #define stall1us {nop28;}
    #define stall2us {nop56;}
    #define stall3us {nop84;}
    #define stall4us {nop100;nop12;}
    #define stall5us {nop100;nop40;}
    #define stall6us {nop100;nop68;}
    #define stall7us {nop100;nop96;}
    #define stall8us {nop100;nop100;nop24;}
    #define stall9us {nop100;nop100;nop52;}
    #define stall10us {nop100;nop100;nop80;}
#elif ((CLOCK/(MachineCycle*500000L))>=55)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>55L), 1us = 27.5 nop, stall1us = 2 nop, stall5us = 137.5 nop, stall10us = 275 nop")
    #define stall1us {nop28;}
    #define stall2us {nop55;}
    #define stall3us {nop83;}
    #define stall4us {nop100;nop10;}
    #define stall5us {nop100;nop38;}
    #define stall6us {nop100;nop65;}
    #define stall7us {nop100;nop93;}
    #define stall8us {nop100;nop100;nop20;}
    #define stall9us {nop100;nop100;nop48;}
    #define stall10us {nop100;nop100;nop75;}
#elif ((CLOCK/(MachineCycle*500000L))>=54)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>54L), 1us = 27.0 nop, stall1us = 2 nop, stall5us = 135.0 nop, stall10us = 270 nop")
    #define stall1us {nop27;}
    #define stall2us {nop54;}
    #define stall3us {nop81;}
    #define stall4us {nop100;nop8;}
    #define stall5us {nop100;nop35;}
    #define stall6us {nop100;nop62;}
    #define stall7us {nop100;nop89;}
    #define stall8us {nop100;nop100;nop16;}
    #define stall9us {nop100;nop100;nop43;}
    #define stall10us {nop100;nop100;nop70;}
#elif ((CLOCK/(MachineCycle*500000L))>=53)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>53L), 1us = 26.5 nop, stall1us = 2 nop, stall5us = 132.5 nop, stall10us = 265 nop")
    #define stall1us {nop27;}
    #define stall2us {nop53;}
    #define stall3us {nop80;}
    #define stall4us {nop100;nop6;}
    #define stall5us {nop100;nop33;}
    #define stall6us {nop100;nop59;}
    #define stall7us {nop100;nop86;}
    #define stall8us {nop100;nop100;nop12;}
    #define stall9us {nop100;nop100;nop39;}
    #define stall10us {nop100;nop100;nop65;}
#elif ((CLOCK/(MachineCycle*500000L))>=52)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>52L), 1us = 26.0 nop, stall1us = 2 nop, stall5us = 130.0 nop, stall10us = 260 nop")
    #define stall1us {nop26;}
    #define stall2us {nop52;}
    #define stall3us {nop78;}
    #define stall4us {nop100;nop4;}
    #define stall5us {nop100;nop30;}
    #define stall6us {nop100;nop56;}
    #define stall7us {nop100;nop82;}
    #define stall8us {nop100;nop100;nop8;}
    #define stall9us {nop100;nop100;nop34;}
    #define stall10us {nop100;nop100;nop60;}
#elif ((CLOCK/(MachineCycle*500000L))>=51)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>51L), 1us = 25.5 nop, stall1us = 2 nop, stall5us = 127.5 nop, stall10us = 255 nop")
    #define stall1us {nop26;}
    #define stall2us {nop51;}
    #define stall3us {nop77;}
    #define stall4us {nop100;nop2;}
    #define stall5us {nop100;nop28;}
    #define stall6us {nop100;nop53;}
    #define stall7us {nop100;nop79;}
    #define stall8us {nop100;nop100;nop4;}
    #define stall9us {nop100;nop100;nop30;}
    #define stall10us {nop100;nop100;nop55;}
#elif ((CLOCK/(MachineCycle*500000L))>=50)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>50L), 1us = 25.0 nop, stall1us = 2 nop, stall5us = 125.0 nop, stall10us = 250 nop")
    #define stall1us {nop25;}
    #define stall2us {nop50;}
    #define stall3us {nop75;}
    #define stall4us {nop100;}
    #define stall5us {nop100;nop25;}
    #define stall6us {nop100;nop50;}
    #define stall7us {nop100;nop75;}
    #define stall8us {nop100;nop100;}
    #define stall9us {nop100;nop100;nop25;}
    #define stall10us {nop100;nop100;nop50;}
#elif ((CLOCK/(MachineCycle*500000L))>=49)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>49L), 1us = 24.5 nop, stall1us = 2 nop, stall5us = 122.5 nop, stall10us = 245 nop")
    #define stall1us {nop25;}
    #define stall2us {nop49;}
    #define stall3us {nop74;}
    #define stall4us {nop98;}
    #define stall5us {nop100;nop23;}
    #define stall6us {nop100;nop47;}
    #define stall7us {nop100;nop72;}
    #define stall8us {nop100;nop96;}
    #define stall9us {nop100;nop100;nop21;}
    #define stall10us {nop100;nop100;nop45;}
#elif ((CLOCK/(MachineCycle*500000L))>=48)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>48L), 1us = 24.0 nop, stall1us = 2 nop, stall5us = 120.0 nop, stall10us = 240 nop")
    #define stall1us {nop24;}
    #define stall2us {nop48;}
    #define stall3us {nop72;}
    #define stall4us {nop96;}
    #define stall5us {nop100;nop20;}
    #define stall6us {nop100;nop44;}
    #define stall7us {nop100;nop68;}
    #define stall8us {nop100;nop92;}
    #define stall9us {nop100;nop100;nop16;}
    #define stall10us {nop100;nop100;nop40;}
#elif ((CLOCK/(MachineCycle*500000L))>=47)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>47L), 1us = 23.5 nop, stall1us = 2 nop, stall5us = 117.5 nop, stall10us = 235 nop")
    #define stall1us {nop24;}
    #define stall2us {nop47;}
    #define stall3us {nop71;}
    #define stall4us {nop94;}
    #define stall5us {nop100;nop18;}
    #define stall6us {nop100;nop41;}
    #define stall7us {nop100;nop65;}
    #define stall8us {nop100;nop88;}
    #define stall9us {nop100;nop100;nop12;}
    #define stall10us {nop100;nop100;nop35;}
#elif ((CLOCK/(MachineCycle*500000L))>=46)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>46L), 1us = 23.0 nop, stall1us = 2 nop, stall5us = 115.0 nop, stall10us = 230 nop")
    #define stall1us {nop23;}
    #define stall2us {nop46;}
    #define stall3us {nop69;}
    #define stall4us {nop92;}
    #define stall5us {nop100;nop15;}
    #define stall6us {nop100;nop38;}
    #define stall7us {nop100;nop61;}
    #define stall8us {nop100;nop84;}
    #define stall9us {nop100;nop100;nop7;}
    #define stall10us {nop100;nop100;nop30;}
#elif ((CLOCK/(MachineCycle*500000L))>=45)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>45L), 1us = 22.5 nop, stall1us = 2 nop, stall5us = 112.5 nop, stall10us = 225 nop")
    #define stall1us {nop23;}
    #define stall2us {nop45;}
    #define stall3us {nop68;}
    #define stall4us {nop90;}
    #define stall5us {nop100;nop13;}
    #define stall6us {nop100;nop35;}
    #define stall7us {nop100;nop58;}
    #define stall8us {nop100;nop80;}
    #define stall9us {nop100;nop100;nop3;}
    #define stall10us {nop100;nop100;nop25;}
#elif ((CLOCK/(MachineCycle*500000L))>=44)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>44L), 1us = 22.0 nop, stall1us = 2 nop, stall5us = 110.0 nop, stall10us = 220 nop")
    #define stall1us {nop22;}
    #define stall2us {nop44;}
    #define stall3us {nop66;}
    #define stall4us {nop88;}
    #define stall5us {nop100;nop10;}
    #define stall6us {nop100;nop32;}
    #define stall7us {nop100;nop54;}
    #define stall8us {nop100;nop76;}
    #define stall9us {nop100;nop98;}
    #define stall10us {nop100;nop100;nop20;}
#elif ((CLOCK/(MachineCycle*500000L))>=43)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>43L), 1us = 21.5 nop, stall1us = 2 nop, stall5us = 107.5 nop, stall10us = 215 nop")
    #define stall1us {nop22;}
    #define stall2us {nop43;}
    #define stall3us {nop65;}
    #define stall4us {nop86;}
    #define stall5us {nop100;nop8;}
    #define stall6us {nop100;nop29;}
    #define stall7us {nop100;nop51;}
    #define stall8us {nop100;nop72;}
    #define stall9us {nop100;nop94;}
    #define stall10us {nop100;nop100;nop15;}
#elif ((CLOCK/(MachineCycle*500000L))>=42)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>42L), 1us = 21.0 nop, stall1us = 2 nop, stall5us = 105.0 nop, stall10us = 210 nop")
    #define stall1us {nop21;}
    #define stall2us {nop42;}
    #define stall3us {nop63;}
    #define stall4us {nop84;}
    #define stall5us {nop100;nop5;}
    #define stall6us {nop100;nop26;}
    #define stall7us {nop100;nop47;}
    #define stall8us {nop100;nop68;}
    #define stall9us {nop100;nop89;}
    #define stall10us {nop100;nop100;nop10;}
#elif ((CLOCK/(MachineCycle*500000L))>=41)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>41L), 1us = 20.5 nop, stall1us = 2 nop, stall5us = 102.5 nop, stall10us = 205 nop")
    #define stall1us {nop21;}
    #define stall2us {nop41;}
    #define stall3us {nop62;}
    #define stall4us {nop82;}
    #define stall5us {nop100;nop3;}
    #define stall6us {nop100;nop23;}
    #define stall7us {nop100;nop44;}
    #define stall8us {nop100;nop64;}
    #define stall9us {nop100;nop85;}
    #define stall10us {nop100;nop100;nop5;}
#elif ((CLOCK/(MachineCycle*500000L))>=40)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>40L), 1us = 20.0 nop, stall1us = 2 nop, stall5us = 100.0 nop, stall10us = 200 nop")
    #define stall1us {nop20;}
    #define stall2us {nop40;}
    #define stall3us {nop60;}
    #define stall4us {nop80;}
    #define stall5us {nop100;}
    #define stall6us {nop100;nop20;}
    #define stall7us {nop100;nop40;}
    #define stall8us {nop100;nop60;}
    #define stall9us {nop100;nop80;}
    #define stall10us {nop100;nop100;}
#elif ((CLOCK/(MachineCycle*500000L))>=39)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>39L), 1us = 19.5 nop, stall1us = 2 nop, stall5us = 97.5 nop, stall10us = 195 nop")
    #define stall1us {nop20;}
    #define stall2us {nop39;}
    #define stall3us {nop59;}
    #define stall4us {nop78;}
    #define stall5us {nop98;}
    #define stall6us {nop100;nop17;}
    #define stall7us {nop100;nop37;}
    #define stall8us {nop100;nop56;}
    #define stall9us {nop100;nop76;}
    #define stall10us {nop100;nop95;}
#elif ((CLOCK/(MachineCycle*500000L))>=38)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>38L), 1us = 19.0 nop, stall1us = 1 nop, stall5us = 95.0 nop, stall10us = 190 nop")
    #define stall1us {nop19;}
    #define stall2us {nop38;}
    #define stall3us {nop57;}
    #define stall4us {nop76;}
    #define stall5us {nop95;}
    #define stall6us {nop100;nop14;}
    #define stall7us {nop100;nop33;}
    #define stall8us {nop100;nop52;}
    #define stall9us {nop100;nop71;}
    #define stall10us {nop100;nop90;}
#elif ((CLOCK/(MachineCycle*500000L))>=37)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>37L), 1us = 18.5 nop, stall1us = 1 nop, stall5us = 92.5 nop, stall10us = 185 nop")
    #define stall1us {nop19;}
    #define stall2us {nop37;}
    #define stall3us {nop56;}
    #define stall4us {nop74;}
    #define stall5us {nop93;}
    #define stall6us {nop100;nop11;}
    #define stall7us {nop100;nop30;}
    #define stall8us {nop100;nop48;}
    #define stall9us {nop100;nop67;}
    #define stall10us {nop100;nop85;}
#elif ((CLOCK/(MachineCycle*500000L))>=36)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>36L), 1us = 18.0 nop, stall1us = 1 nop, stall5us = 90.0 nop, stall10us = 180 nop")
    #define stall1us {nop18;}
    #define stall2us {nop36;}
    #define stall3us {nop54;}
    #define stall4us {nop72;}
    #define stall5us {nop90;}
    #define stall6us {nop100;nop8;}
    #define stall7us {nop100;nop26;}
    #define stall8us {nop100;nop44;}
    #define stall9us {nop100;nop62;}
    #define stall10us {nop100;nop80;}
#elif ((CLOCK/(MachineCycle*500000L))>=35)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>35L), 1us = 17.5 nop, stall1us = 1 nop, stall5us = 87.5 nop, stall10us = 175 nop")
    #define stall1us {nop18;}
    #define stall2us {nop35;}
    #define stall3us {nop53;}
    #define stall4us {nop70;}
    #define stall5us {nop88;}
    #define stall6us {nop100;nop5;}
    #define stall7us {nop100;nop23;}
    #define stall8us {nop100;nop40;}
    #define stall9us {nop100;nop58;}
    #define stall10us {nop100;nop75;}
#elif ((CLOCK/(MachineCycle*500000L))>=34)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>34L), 1us = 17.0 nop, stall1us = 1 nop, stall5us = 85.0 nop, stall10us = 170 nop")
    #define stall1us {nop17;}
    #define stall2us {nop34;}
    #define stall3us {nop51;}
    #define stall4us {nop68;}
    #define stall5us {nop85;}
    #define stall6us {nop100;nop2;}
    #define stall7us {nop100;nop19;}
    #define stall8us {nop100;nop36;}
    #define stall9us {nop100;nop53;}
    #define stall10us {nop100;nop70;}
#elif ((CLOCK/(MachineCycle*500000L))>=33)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>33L), 1us = 16.5 nop, stall1us = 1 nop, stall5us = 82.5 nop, stall10us = 165 nop")
    #define stall1us {nop17;}
    #define stall2us {nop33;}
    #define stall3us {nop50;}
    #define stall4us {nop66;}
    #define stall5us {nop83;}
    #define stall6us {nop99;}
    #define stall7us {nop100;nop16;}
    #define stall8us {nop100;nop32;}
    #define stall9us {nop100;nop49;}
    #define stall10us {nop100;nop65;}
#elif ((CLOCK/(MachineCycle*500000L))>=32)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>32L), 1us = 16.0 nop, stall1us = 1 nop, stall5us = 80.0 nop, stall10us = 160 nop")
    #define stall1us {nop16;}
    #define stall2us {nop32;}
    #define stall3us {nop48;}
    #define stall4us {nop64;}
    #define stall5us {nop80;}
    #define stall6us {nop96;}
    #define stall7us {nop100;nop12;}
    #define stall8us {nop100;nop28;}
    #define stall9us {nop100;nop44;}
    #define stall10us {nop100;nop60;}
#elif ((CLOCK/(MachineCycle*500000L))>=31)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>31L), 1us = 15.5 nop, stall1us = 1 nop, stall5us = 77.5 nop, stall10us = 155 nop")
    #define stall1us {nop16;}
    #define stall2us {nop31;}
    #define stall3us {nop47;}
    #define stall4us {nop62;}
    #define stall5us {nop78;}
    #define stall6us {nop93;}
    #define stall7us {nop100;nop9;}
    #define stall8us {nop100;nop24;}
    #define stall9us {nop100;nop40;}
    #define stall10us {nop100;nop55;}
#elif ((CLOCK/(MachineCycle*500000L))>=30)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>30L), 1us = 15.0 nop, stall1us = 1 nop, stall5us = 75.0 nop, stall10us = 150 nop")
    #define stall1us {nop15;}
    #define stall2us {nop30;}
    #define stall3us {nop45;}
    #define stall4us {nop60;}
    #define stall5us {nop75;}
    #define stall6us {nop90;}
    #define stall7us {nop100;nop5;}
    #define stall8us {nop100;nop20;}
    #define stall9us {nop100;nop35;}
    #define stall10us {nop100;nop50;}
#elif ((CLOCK/(MachineCycle*500000L))>=29)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>29L), 1us = 14.5 nop, stall1us = 1 nop, stall5us = 72.5 nop, stall10us = 145 nop")
    #define stall1us {nop15;}
    #define stall2us {nop29;}
    #define stall3us {nop44;}
    #define stall4us {nop58;}
    #define stall5us {nop73;}
    #define stall6us {nop87;}
    #define stall7us {nop100;nop2;}
    #define stall8us {nop100;nop16;}
    #define stall9us {nop100;nop31;}
    #define stall10us {nop100;nop45;}
#elif ((CLOCK/(MachineCycle*500000L))>=28)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>28L), 1us = 14.0 nop, stall1us = 1 nop, stall5us = 70.0 nop, stall10us = 140 nop")
    #define stall1us {nop14;}
    #define stall2us {nop28;}
    #define stall3us {nop42;}
    #define stall4us {nop56;}
    #define stall5us {nop70;}
    #define stall6us {nop84;}
    #define stall7us {nop98;}
    #define stall8us {nop100;nop12;}
    #define stall9us {nop100;nop26;}
    #define stall10us {nop100;nop40;}
#elif ((CLOCK/(MachineCycle*500000L))>=27)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>27L), 1us = 13.5 nop, stall1us = 1 nop, stall5us = 67.5 nop, stall10us = 135 nop")
    #define stall1us {nop14;}
    #define stall2us {nop27;}
    #define stall3us {nop41;}
    #define stall4us {nop54;}
    #define stall5us {nop68;}
    #define stall6us {nop81;}
    #define stall7us {nop95;}
    #define stall8us {nop100;nop8;}
    #define stall9us {nop100;nop22;}
    #define stall10us {nop100;nop35;}
#elif ((CLOCK/(MachineCycle*500000L))>=26)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>26L), 1us = 13.0 nop, stall1us = 1 nop, stall5us = 65.0 nop, stall10us = 130 nop")
    #define stall1us {nop13;}
    #define stall2us {nop26;}
    #define stall3us {nop39;}
    #define stall4us {nop52;}
    #define stall5us {nop65;}
    #define stall6us {nop78;}
    #define stall7us {nop91;}
    #define stall8us {nop100;nop4;}
    #define stall9us {nop100;nop17;}
    #define stall10us {nop100;nop30;}
#elif ((CLOCK/(MachineCycle*500000L))>=25)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>25L), 1us = 12.5 nop, stall1us = 1 nop, stall5us = 62.5 nop, stall10us = 125 nop")
    #define stall1us {nop13;}
    #define stall2us {nop25;}
    #define stall3us {nop38;}
    #define stall4us {nop50;}
    #define stall5us {nop63;}
    #define stall6us {nop75;}
    #define stall7us {nop88;}
    #define stall8us {nop100;}
    #define stall9us {nop100;nop13;}
    #define stall10us {nop100;nop25;}
#elif ((CLOCK/(MachineCycle*500000L))>=24)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>24L), 1us = 12.0 nop, stall1us = 1 nop, stall5us = 60.0 nop, stall10us = 120 nop")
    #define stall1us {nop12;}
    #define stall2us {nop24;}
    #define stall3us {nop36;}
    #define stall4us {nop48;}
    #define stall5us {nop60;}
    #define stall6us {nop72;}
    #define stall7us {nop84;}
    #define stall8us {nop96;}
    #define stall9us {nop100;nop8;}
    #define stall10us {nop100;nop20;}
#elif ((CLOCK/(MachineCycle*500000L))>=23)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>23L), 1us = 11.5 nop, stall1us = 1 nop, stall5us = 57.5 nop, stall10us = 115 nop")
    #define stall1us {nop12;}
    #define stall2us {nop23;}
    #define stall3us {nop35;}
    #define stall4us {nop46;}
    #define stall5us {nop58;}
    #define stall6us {nop69;}
    #define stall7us {nop81;}
    #define stall8us {nop92;}
    #define stall9us {nop100;nop4;}
    #define stall10us {nop100;nop15;}
#elif ((CLOCK/(MachineCycle*500000L))>=22)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>22L), 1us = 11.0 nop, stall1us = 1 nop, stall5us = 55.0 nop, stall10us = 110 nop")
    #define stall1us {nop11;}
    #define stall2us {nop22;}
    #define stall3us {nop33;}
    #define stall4us {nop44;}
    #define stall5us {nop55;}
    #define stall6us {nop66;}
    #define stall7us {nop77;}
    #define stall8us {nop88;}
    #define stall9us {nop99;}
    #define stall10us {nop100;nop10;}
#elif ((CLOCK/(MachineCycle*500000L))>=21)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>21L), 1us = 10.5 nop, stall1us = 1 nop, stall5us = 52.5 nop, stall10us = 105 nop")
    #define stall1us {nop11;}
    #define stall2us {nop21;}
    #define stall3us {nop32;}
    #define stall4us {nop42;}
    #define stall5us {nop53;}
    #define stall6us {nop63;}
    #define stall7us {nop74;}
    #define stall8us {nop84;}
    #define stall9us {nop95;}
    #define stall10us {nop100;nop5;}
#elif ((CLOCK/(MachineCycle*500000L))>=20)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>20L), 1us = 10.0 nop, stall1us = 1 nop, stall5us = 50.0 nop, stall10us = 100 nop")
    #define stall1us {nop10;}
    #define stall2us {nop20;}
    #define stall3us {nop30;}
    #define stall4us {nop40;}
    #define stall5us {nop50;}
    #define stall6us {nop60;}
    #define stall7us {nop70;}
    #define stall8us {nop80;}
    #define stall9us {nop90;}
    #define stall10us {nop100;}
#elif ((CLOCK/(MachineCycle*500000L))>=19)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>19L), 1us =  9.5 nop, stall1us = 1 nop, stall5us = 47.5 nop, stall10us = 95 nop")
    #define stall1us {nop10;}
    #define stall2us {nop19;}
    #define stall3us {nop29;}
    #define stall4us {nop38;}
    #define stall5us {nop48;}
    #define stall6us {nop57;}
    #define stall7us {nop67;}
    #define stall8us {nop76;}
    #define stall9us {nop86;}
    #define stall10us {nop95;}
#elif ((CLOCK/(MachineCycle*500000L))>=18)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>18L), 1us =  9.0 nop, stall1us = 0 nop, stall5us = 45.0 nop, stall10us = 90 nop")
    #define stall1us {nop9;}
    #define stall2us {nop18;}
    #define stall3us {nop27;}
    #define stall4us {nop36;}
    #define stall5us {nop45;}
    #define stall6us {nop54;}
    #define stall7us {nop63;}
    #define stall8us {nop72;}
    #define stall9us {nop81;}
    #define stall10us {nop90;}
#elif ((CLOCK/(MachineCycle*500000L))>=17)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>17L), 1us =  8.5 nop, stall1us = 0 nop, stall5us = 42.5 nop, stall10us = 85 nop")
    #define stall1us {nop9;}
    #define stall2us {nop17;}
    #define stall3us {nop26;}
    #define stall4us {nop34;}
    #define stall5us {nop43;}
    #define stall6us {nop51;}
    #define stall7us {nop60;}
    #define stall8us {nop68;}
    #define stall9us {nop77;}
    #define stall10us {nop85;}
#elif ((CLOCK/(MachineCycle*500000L))>=16)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>16L), 1us =  8.0 nop, stall1us = 0 nop, stall5us = 40.0 nop, stall10us = 80 nop")
    #define stall1us {nop8;}
    #define stall2us {nop16;}
    #define stall3us {nop24;}
    #define stall4us {nop32;}
    #define stall5us {nop40;}
    #define stall6us {nop48;}
    #define stall7us {nop56;}
    #define stall8us {nop64;}
    #define stall9us {nop72;}
    #define stall10us {nop80;}
#elif ((CLOCK/(MachineCycle*500000L))>=15)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>15L), 1us =  7.5 nop, stall1us = 0 nop, stall5us = 37.5 nop, stall10us = 75 nop")
    #define stall1us {nop8;}
    #define stall2us {nop15;}
    #define stall3us {nop23;}
    #define stall4us {nop30;}
    #define stall5us {nop38;}
    #define stall6us {nop45;}
    #define stall7us {nop53;}
    #define stall8us {nop60;}
    #define stall9us {nop68;}
    #define stall10us {nop75;}
#elif ((CLOCK/(MachineCycle*500000L))>=14)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>14L), 1us =  7.0 nop, stall1us = 0 nop, stall5us = 35.0 nop, stall10us = 70 nop")
    #define stall1us {nop7;}
    #define stall2us {nop14;}
    #define stall3us {nop21;}
    #define stall4us {nop28;}
    #define stall5us {nop35;}
    #define stall6us {nop42;}
    #define stall7us {nop49;}
    #define stall8us {nop56;}
    #define stall9us {nop63;}
    #define stall10us {nop70;}
#elif ((CLOCK/(MachineCycle*500000L))>=13)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>13L), 1us =  6.5 nop, stall1us = 0 nop, stall5us = 32.5 nop, stall10us = 65 nop")
    #define stall1us {nop7;}
    #define stall2us {nop13;}
    #define stall3us {nop20;}
    #define stall4us {nop26;}
    #define stall5us {nop33;}
    #define stall6us {nop39;}
    #define stall7us {nop46;}
    #define stall8us {nop52;}
    #define stall9us {nop59;}
    #define stall10us {nop65;}
#elif ((CLOCK/(MachineCycle*500000L))>=12)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>12L), 1us =  6.0 nop, stall1us = 0 nop, stall5us = 30.0 nop, stall10us = 60 nop")
    #define stall1us {nop6;}
    #define stall2us {nop12;}
    #define stall3us {nop18;}
    #define stall4us {nop24;}
    #define stall5us {nop30;}
    #define stall6us {nop36;}
    #define stall7us {nop42;}
    #define stall8us {nop48;}
    #define stall9us {nop54;}
    #define stall10us {nop60;}
#elif ((CLOCK/(MachineCycle*500000L))>=11)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>11L), 1us =  5.5 nop, stall1us = 0 nop, stall5us = 27.5 nop, stall10us = 55 nop")
    #define stall1us {nop6;}
    #define stall2us {nop11;}
    #define stall3us {nop17;}
    #define stall4us {nop22;}
    #define stall5us {nop28;}
    #define stall6us {nop33;}
    #define stall7us {nop39;}
    #define stall8us {nop44;}
    #define stall9us {nop50;}
    #define stall10us {nop55;}
#elif ((CLOCK/(MachineCycle*500000L))>=10)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>10L), 1us =  5.0 nop, stall1us = 0 nop, stall5us = 25.0 nop, stall10us = 50 nop")
    #define stall1us {nop5;}
    #define stall2us {nop10;}
    #define stall3us {nop15;}
    #define stall4us {nop20;}
    #define stall5us {nop25;}
    #define stall6us {nop30;}
    #define stall7us {nop35;}
    #define stall8us {nop40;}
    #define stall9us {nop45;}
    #define stall10us {nop50;}
#elif ((CLOCK/(MachineCycle*500000L))>=9)
//    #pragma message ("((CLOCK/(MachineCycle*500000L))>9L), 1us =  4.5 nop, stall1us = 0 nop, stall5us = 22.5 nop, stall10us = 45 nop")
    #define stall1us {nop5;}
    #define stall2us {nop9;}
    #define stall3us {nop14;}
    #define stall4us {nop18;}
    #define stall5us {nop23;}
    #define stall6us {nop27;}
    #define stall7us {nop32;}
    #define stall8us {nop36;}
    #define stall9us {nop41;}
    #define stall10us {nop45;}
#elif ((CLOCK/(MachineCycle*500000L))>=8)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>8L), 1us =  4.0 nop, stall1us = 0 nop, stall5us = 20.0 nop, stall10us = 40 nop")
    #define stall1us {nop4;}
    #define stall2us {nop8;}
    #define stall3us {nop12;}
    #define stall4us {nop16;}
    #define stall5us {nop20;}
    #define stall6us {nop24;}
    #define stall7us {nop28;}
    #define stall8us {nop32;}
    #define stall9us {nop36;}
    #define stall10us {nop40;}
#elif ((CLOCK/(MachineCycle*500000L))>=7)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>7L), 1us =  3.5 nop, stall1us = 0 nop, stall5us = 17.5 nop, stall10us = 35 nop")
    #define stall1us {nop4;}
    #define stall2us {nop7;}
    #define stall3us {nop11;}
    #define stall4us {nop14;}
    #define stall5us {nop18;}
    #define stall6us {nop21;}
    #define stall7us {nop25;}
    #define stall8us {nop28;}
    #define stall9us {nop32;}
    #define stall10us {nop35;}
#elif ((CLOCK/(MachineCycle*500000L))>=6)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>6L), 1us =  3.0 nop, stall1us = 0 nop, stall5us = 15.0 nop, stall10us = 30 nop")
    #define stall1us {nop3;}
    #define stall2us {nop6;}
    #define stall3us {nop9;}
    #define stall4us {nop12;}
    #define stall5us {nop15;}
    #define stall6us {nop18;}
    #define stall7us {nop21;}
    #define stall8us {nop24;}
    #define stall9us {nop27;}
    #define stall10us {nop30;}
#elif ((CLOCK/(MachineCycle*500000L))>=5)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>5L), 1us =  2.5 nop, stall1us = 0 nop, stall5us = 12.5 nop, stall10us = 25 nop")
    #define stall1us {nop3;}
    #define stall2us {nop5;}
    #define stall3us {nop8;}
    #define stall4us {nop10;}
    #define stall5us {nop13;}
    #define stall6us {nop15;}
    #define stall7us {nop18;}
    #define stall8us {nop20;}
    #define stall9us {nop23;}
    #define stall10us {nop25;}
#elif ((CLOCK/(MachineCycle*500000L))>=4)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>4L), 1us =  2.0 nop, stall1us = 0 nop, stall5us = 10.0 nop, stall10us = 20 nop")
    #define stall1us {nop2;}
    #define stall2us {nop4;}
    #define stall3us {nop6;}
    #define stall4us {nop8;}
    #define stall5us {nop10;}
    #define stall6us {nop12;}
    #define stall7us {nop14;}
    #define stall8us {nop16;}
    #define stall9us {nop18;}
    #define stall10us {nop20;}
#elif ((CLOCK/(MachineCycle*500000L))>=3)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>3L), 1us =  1.5 nop, stall1us = 0 nop, stall5us =  7.5 nop, stall10us = 15 nop")
    #define stall1us {nop2;}
    #define stall2us {nop3;}
    #define stall3us {nop5;}
    #define stall4us {nop6;}
    #define stall5us {nop8;}
    #define stall6us {nop9;}
    #define stall7us {nop11;}
    #define stall8us {nop12;}
    #define stall9us {nop14;}
    #define stall10us {nop15;}
#elif ((CLOCK/(MachineCycle*500000L))>=2)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>2L), 1us =  1.0 nop, stall1us = 0 nop, stall5us =  5.0 nop, stall10us = 10 nop")
    #define stall1us {nop1;}
    #define stall2us {nop2;}
    #define stall3us {nop3;}
    #define stall4us {nop4;}
    #define stall5us {nop5;}
    #define stall6us {nop6;}
    #define stall7us {nop7;}
    #define stall8us {nop8;}
    #define stall9us {nop9;}
    #define stall10us {nop10;}
#elif ((CLOCK/(MachineCycle*500000L))>=1)
    #pragma message ("((CLOCK/(MachineCycle*500000L))>1L), 1us =  0.5 nop, stall1us = 0 nop, stall5us =  2.5 nop, stall10us = 5 nop")
    #define stall1us {nop1;}
    #define stall2us {nop1;}
    #define stall3us {nop2;}
    #define stall4us {nop2;}
    #define stall5us {nop3;}
    #define stall6us {nop3;}
    #define stall7us {nop4;}
    #define stall8us {nop4;}
    #define stall9us {nop5;}
    #define stall10us {nop5;}
#else
    #pragma message ("Slow mcu, no stall function.")
    #define stall1us
    #define stall2us
    #define stall3us
    #define stall4us
    #define stall5us
    #define stall6us
    #define stall7us
    #define stall8us
    #define stall9us
    #define stall10us
#endif

#endif // _NOP_H_
