#ifndef RASP_SRV_H
#define RASP_SRV_H

typedef void (*fun_ptr)(int);

void* rasp_srv_start(fun_ptr func);



#endif // RASP_SRV_H
