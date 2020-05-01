#include <stdio.h>
#include <stdlib.h>
#include "proj.config"


#ifdef HAS_RASP_SRV
#include "rasp_srv.h"
#endif

int main()
{

#ifdef HAS_RASP_SRV
    rasp_srv_start();
#endif
    printf("Privet ot Abrikotyana i Korovki!\n");
    return 0;
}