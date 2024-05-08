
#include "includes.h"

#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)
#include "Sii9630_si_datatypes.h"
#include "Sii9630_si_drv_cp8630.h"
#if (CONFIG_SOURCE_ONLY == 0)
#include "Sii9630_spiderman.h"
#endif

void mapp_Sil9630Init(void)
{
    #if (CONFIG_SOURCE_ONLY == 0)
    spiderman_init();
    #endif
    sii_drv_init();

}
void mapp_Sil9630_Task(void)
{
    mapp_Sil9630_main();
}

#endif
