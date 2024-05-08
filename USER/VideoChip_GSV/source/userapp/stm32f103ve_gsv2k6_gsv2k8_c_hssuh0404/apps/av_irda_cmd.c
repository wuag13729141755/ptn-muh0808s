#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "av_config.h"
#include "Gsv_hal.h"
#include "Gsv_kapi.h"
#include "Gsv_bsp.h"
#include "av_irda_cmd.h"

void ProcessIrda(AvPort *port)
{
    return;
}
void ListenToIrdaCommand(AvPort *port)
{
#if AvIrdaFunctionInput
    ProcessIrda(port);
#endif
}

#endif
