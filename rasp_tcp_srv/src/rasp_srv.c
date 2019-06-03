/*
** server.c -- a stream socket server 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "motor_ctrl.h"
#ifndef __cplusplus
typedef unsigned char  bool; 
#define BOOL bool
#define TRUE  ((bool)1)
#define FALSE ((bool)0)
#define true  ((bool)1)
#define false ((bool)0)
#endif




enum eRaspCommands
{
    RASP_SRV_RESTART_CMD        = 0x00,
    RASP_SRV_SHUTDOWN_CMD       = 0x01,
    RASP_SRV_DISCONNECT_CMD     = 0x02,
    RASP_SYS_REBOOT_CMD         = 0x20,
    RASP_SYS_SHUTDOWN_CMD       = 0x21,  
    RASP_TKCTRL_START_CMD       = 0x40,
    RASP_TKCTRL_STOP_CMD        = 0x41,
    RASP_TKCTRL_LEFT_CMD        = 0x42,
    RASP_TKCTRL_RIGHT_CMD       = 0x43,
};

// unsigned char rasp_cmds[]{
//                             { 0xAA, 0x00, 0x00, 0x5A }, //RASP_SRV_RESTART
//                             { 0xAA, 0x01, 0x00, 0x5A }, //RASP_SRV_SHUTDOWN
//                             { 0xAA, 0x02, 0x00, 0x5A }, //RASP_SRV_DISCONNECT

//                             { 0xAA, 0x20, 0x00, 0x5A }, //RASP_SYS_REBOOT
//                             { 0xAA, 0x21, 0x00, 0x5A }, //RASP_SYS_REBOOT

//                             { 0xAA, 0x40, 0x00, 0x5A }, //RASP_TKCTRL_START
//                             { 0xAA, 0x41, 0x00, 0x5A }, //RASP_TKCTRL_STOP
//                             { 0xAA, 0x42, 0x00, 0x5A }, //RASP_TKCTRL_LEFT
//                             { 0xAA, 0x43, 0x00, 0x5A }, //RASP_TKCTRL_RIGHT
//                             };


#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    init_motor();

    while(1) {  // main accept() loop
        printf("server: waiting for connections...\n");
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);
//        send(new_fd, "Hello, world!", 13, 0);
        unsigned char buf[32];
        recv(new_fd, &buf[0], 4, MSG_WAITALL);
//        printf("\r\n%d, %d, %d, %d\r\n", buf[0],buf[1],buf[2], buf[3]);
//        while ((buf[0] == 0xAA) && (buf[3] == 0x5A) && (buf[1] != 0x02) && (buf[2] != 0x00))
        bool bRunTheLoop = true;
        while(bRunTheLoop)
        {
            printf("\r\n0x%x, 0x%x, 0x%x, 0x%x\r\n", buf[0],buf[1],buf[2], buf[3]);
            if ((buf[0] == 0xAA) && (buf[3] == 0x5A))
            {
                switch(buf[1])
                {
                    case RASP_SRV_DISCONNECT_CMD:
                    {
                        printf("RASP_SRV_DISCONNECT\r\n");
                        bRunTheLoop = false;
                        break;
                    }
                    case RASP_SRV_SHUTDOWN_CMD:
                    {
                        printf("RASP_SRV_SHUTDOWN_CMD\r\n");
                        bRunTheLoop = false;
                        printf("connection is closed. server is off.\r\n");
                        close(new_fd);
                        exit(0);
                        break;
                    }
                    case RASP_TKCTRL_START_CMD:
                    {
                        printf("RASP_TKCTRL_START\r\n");
                        forward();
//                        system("ls");
                        break;
                    }
                    case RASP_TKCTRL_STOP_CMD:
                    {
                        printf("RASP_TKCTRL_STOP\r\n");
                        stop();
                        break;
                    }
                    case RASP_TKCTRL_LEFT_CMD:
                    {
                        printf("RASP_TKCTRL_LEFT\r\n");
                        left();
                        break;
                    }
                    case RASP_TKCTRL_RIGHT_CMD:
                    {
                        printf("RASP_TKCTRL_RIGHT\r\n");
                        right();
                        break;
                    }
                    default:
                        printf("Unknown or unsupported command.");
                        break;
                }
            }
            else
            {
                printf("Incorrect message!\r\n");
            }
            recv(new_fd, &buf[0], 4, MSG_WAITALL);
        }

        // if (!fork()) { // this is the child process
        //     close(sockfd); // child doesn't need the listener
        //     if (send(new_fd, "Hello, world!", 13, 0) == -1)
        //         perror("send");
        //     close(new_fd);
        //     exit(0);
        // }
        // close(new_fd);  // parent doesn't need this
    }

    return 0;
}
