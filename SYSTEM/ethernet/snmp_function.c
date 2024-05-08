/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         snmp_function.c
**Writer:           wf8421
**Create Date:      2023-04-20
**Rewriter:         wf8421
**Rewrite Date:     2023-04-20
**Description:      snmp function
**History:
**Version:          V1.0.0
******************************************************/
#include "includes.h"
#include "lwip/opt.h"
#if LWIP_SNMP
#include "lwip/apps/snmp.h"
#include "lwip/snmp.h"
#include "lwip/apps/snmp.h"
#include "lwip/apps/snmp_core.h"
#include "lwip/apps/snmp_mib2.h"
#include "lwip/apps/snmp_scalar.h"
#include "snmp_function.h"

#if (LWIP_SNMP_V2&&LWIP_SNMP_V3)
#include "lwip/apps/snmp_snmpv2_framework.h"
#include "lwip/apps/snmp_snmpv2_usm.h"
#endif

#if 1
#define DBG_SNMP          dbg_printf
#else
#define DBG_SNMP(x)
#endif

static const struct snmp_mib *mibs[] = {
  &mib2,
//	#if SNMP_Test_debug
//  &mib_private1,
//	&mib_private2,
//	&mib_private3,
//	&mib_private4,
//	#else
//	&mib_private,
//	#endif
#if LWIP_SNMP_V3
  &snmpframeworkmib,
  &snmpusmmib,
#endif
};

#if SNMP_Test_debug
static const struct snmp_obj_id WSEE_mib_enterprise_oid_default={SNMP_Pr_ENTERPRISE_OID_LEN,SNMP_Pr_ENTERPRISE_OID};
static const struct snmp_obj_id *WSEE_mib_enterprise_oid=&WSEE_mib_enterprise_oid_default;
#endif

void snmp_function_init(void)
{
//    snmp_set_community(GetStrSysInfoUart(index_sys_net_ReadCommunity));
//    snmp_set_community_write(GetStrSysInfoUart(index_sys_net_WriteCommunity));

#if SNMP_LWIP_MIB2
#if SNMP_USE_NETCONN
    snmp_threadsync_init(&snmp_mib2_lwip_locks, snmp_mib2_lwip_synchronizer);
#endif /* SNMP_USE_NETCONN */
    snmp_mib2_set_syscontact_readonly((const u8_t*)"root", NULL);
    snmp_mib2_set_syslocation_readonly((const u8_t*)"lwIP development PC", NULL);
    snmp_mib2_set_sysdescr((const u8_t*)"simhost", NULL);
    snmp_mib2_set_sysname_readonly((const u8_t *)"dest", NULL);
#endif /* SNMP_LWIP_MIB2 */
#if LWIP_SNMP_V3
//    snmpv3_dummy_init();
#endif
#if SNMP_Test_debug
    //  snmp_set_device_enterprise_oid(WSEE_mib_enterprise_oid);
    snmp_set_device_enterprise_oid(WSEE_mib_enterprise_oid);
#endif
    snmp_set_mibs(mibs, LWIP_ARRAYSIZE(mibs));
    snmp_trap_init();
    snmp_set_auth_traps_enabled(SNMP_AUTH_TRAPS_ENABLED);
    snmp_init();

    snmp_trap_test();

}

void snmp_trap_init(void)
{
	static ip4_addr_t trapIP;
    IP4_ADDR(&trapIP,192,168,0,100);
    snmp_trap_dst_ip_set(0,&trapIP);
	snmp_trap_dst_enable(0,1);
}

void snmp_trap_test(void)
{
    DBG_SNMP("===================================\r\n");
    DBG_SNMP("snmp_authfail_trap\r\n");
    snmp_authfail_trap();
    DBG_SNMP("snmp_coldstart_trap\r\n");
    snmp_coldstart_trap();
    DBG_SNMP("===================================\r\n");
}

#endif

