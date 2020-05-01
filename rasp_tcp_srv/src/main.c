#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "proj.config"


#ifdef HAS_RASP_SRV
#include "rasp_srv.h"
pthread_t tid[2];
#endif

void raspDispatch(int msg)
{
    printf("incoming...[%d]\n", msg);
}

int main()
{

#ifdef HAS_RASP_SRV
    int err = pthread_create(&(tid[0]), NULL, &rasp_srv_start, (fun_ptr)raspDispatch);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));
    else
        printf("\n Rasp server thread created successfully\n");

#endif
    while(1)
    {

    }
    printf("Privet ot Abrikotyana i Korovki!\n");
    return 0;
}