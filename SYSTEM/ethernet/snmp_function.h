/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         snmp_function.h
**Writer:           wf8421
**Create Date:      2023-04-20
**Rewriter:         wf8421
**Rewrite Date:     2023-04-20
**Description:      snmp function
**History:
**Version:          V1.0.0
******************************************************/
#ifndef _SNMP_FUNCTION_H_
#define _SNMP_FUNCTION_H_

#define SNMP_Test_debug 1

#if SNMP_Test_debug
#define SNMP_WSEE_ENTERPRISE_OID 17409,85
#define SNMP_Pr_ENTERPRISE  1, 3, 6, 1, 4, 1, SNMP_WSEE_ENTERPRISE_OID
#define SNMP_Pr_ENTERPRISE_OID {SNMP_Pr_ENTERPRISE}
#define SNMP_Pr_ENTERPRISE_OID_LEN 8

#define WSEE_Pr_M1_TECH {SNMP_Pr_ENTERPRISE,1,1,4,1,5}
#define WSEE_Pr_M1_TECH_LEN 13
#define WSEE_Pr_M1_BIAS980H {SNMP_Pr_ENTERPRISE,1,1,4,1,4}
#define WSEE_Pr_M1_BIAS980H_LEN 13
#define WSEE_Pr_M1_BIAS940H {SNMP_Pr_ENTERPRISE,1,1,4,1,7}
#define WSEE_Pr_M1_BIAS940H_LEN 13
#define WSEE_Pr_M1_LASETempH {SNMP_Pr_ENTERPRISE,1,1,4,1,6}
#define WSEE_Pr_M1_LASETempH_LEN 13
#define WSEE_Pr_M1_OPL {SNMP_Pr_ENTERPRISE,1,1,4,1,3}
#define WSEE_Pr_M1_OPL_LEN 13

#define WSEE_Pr_Pwr_OFFLINE	{SNMP_Pr_ENTERPRISE,1,4,1,1,3}
#define WSEE_Pr_Pwr_OFFLINE_LEN 13

#endif


void snmp_trap_init(void);
void snmp_function_init(void);
void snmp_trap_test(void);

#endif  //#ifndef _SNMP_FUNCTION_H_

