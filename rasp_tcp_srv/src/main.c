#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "proj.config"

#ifdef HAS_MOTOR_CTRL
#include "motor_ctrl.h"
#endif


#ifdef HAS_RASP_SRV
#include "rasp_srv.h"
pthread_t tid[2];

enum eMessageIDs
{
    MOTOR_CTRL = 0x39,
    TANK_STATE = 0x61,
};

#endif




void raspDispatch(unsigned  char* msg)
{
    int i;
    int msg_size = *msg;
    msg++;
    int msg_id = *msg;
    msg++;
    switch(msg_id)
    {
#ifdef HAS_MOTOR_CTRL
        case MOTOR_CTRL:
            unsigned char m1 = *msg;
            msg++;
            unsigned char m2 = *msg;
            printf("MOTOR_CTRL:[%d:%d]\n", m1, m2);
            motor_control(m1, m2);
            break;
#endif
        default:
        {
            printf("incoming...size:[%d];[%d, ", msg_size, msg_id);
            for (i = 0; i < msg_size - 2; i++)
            {
                printf("%d, ", *msg);
                msg++;
            }
            printf("]\n");
        }
        break;
    }
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