#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#include "kill_process.h"

#define TASK_MAX_NUM            48
#define TASK_CMD_MAX_LEN        2048
typedef struct transcode_taskinfo_s {
    pid_t task_pid;
    char cmd[TASK_CMD_MAX_LEN];
} transcode_taskinfo_t;

struct cpu_freq_info
{
    char ipaddr[16];
    char cpu_freq[8];
};

struct report_data {
    int fd;
    char src[1024];
};
transcode_taskinfo_t g_task[TASK_MAX_NUM];

int node_transcode_task_init()
{
    memset(g_task, 0, sizeof(g_task));

    return 0;
}

/* server socket listen operation api*/
static int node_transcode_listen(int sockfd)
{
    struct sockaddr_in server_addr;
    int local_port = 8899;
    int lisen_num = 2;
    int one = 1;

    if ((sockfd = socket (PF_INET, SOCK_STREAM, 0)) == -1) {
        printf("socket:%s", strerror (errno));
        exit (1);
    }

    setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, (void *) &one, sizeof (one));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons (local_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero (&(server_addr.sin_zero), 0);
    if (bind (sockfd, (struct sockaddr *) &server_addr, sizeof (struct sockaddr)) == -1) {
        printf("bind:%s", strerror (errno));
        exit (1);
    }

    if (listen (sockfd, lisen_num) == -1) {
        printf("listen:%s", strerror (errno));
        exit (1);
    }

    return sockfd;
}
/* accept new connect and use the client_sockfd to recv message from client */
static int node_transcode_worker(int sockfd)
{
    int ret = 0;
    int recvbytes = 0;
    int client_sockfd = 0;
    unsigned int sin_size = 0;
    struct sockaddr_in client_addr;
    char recvbuf[TASK_CMD_MAX_LEN];
    char pid_process_msg[TASK_CMD_MAX_LEN];
    pid_t child_pid = 0;


    while (1) {
        if ((client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &sin_size)) == -1) {
            printf("accept:%s", strerror (errno));
            usleep(10000);
            continue;
        }

        if (!(child_pid = fork())) {
            memset(recvbuf, 0, sizeof(recvbuf));
            if ((recvbytes = recv(client_sockfd, &recvbuf, sizeof(recvbuf), 0)) == -1) {
                printf("recv:[%s]\n", strerror(errno));
                exit(1);
            }
            if (recvbytes < 16) {
                if (!strncasecmp ((char *)&recvbuf, "pid=", strlen ("pid="))) {
                    memset(pid_process_msg, 0, sizeof(pid_process_msg));
                    ret = kill_pid_handle(pid_process_msg, recvbuf);

                    fprintf(stderr, "oooooooo ret = [%d]\n", ret);
                    if (ret > 0) {
                        if (send(client_sockfd, pid_process_msg, strlen(pid_process_msg), 0) == -1) {
                            printf("send:%s", strerror(errno));
                            return -1;
                        }
                    }

                    if (client_sockfd) {
                        close (client_sockfd);
                    }
                } else if (!strncasecmp ((char *)&recvbuf, "getcpu", strlen ("getcpu"))) {
                    if(client_sockfd) {
                        close(client_sockfd);
                    }
                } else {
                    if (client_sockfd) {
                        close(client_sockfd);
                    }
                }
            } else {

            }
        } else {
            if (client_sockfd) {
//                close(client_sockfd);
            }
        }

        printf("recvied buffer size = %d\n", recvbytes);
        printf("recvied byffer = [%s]\n", recvbuf);
    }
}

/* server idle */
static int node_transcode_server()
{
    int sockfd = 0;

    sockfd = node_transcode_listen(sockfd);
    node_transcode_worker(sockfd);

    return 0;
}

void usage()
{
    /* TODO: Add help usage message here. */
    fprintf(stderr, "Print Help Messages\n");
}

int main(int argc, char *argv[])
{
    int ret = 0;
    char ch = 0;

    while ((ch = getopt(argc, argv, "hc:")) != -1) {
        switch (ch) {
            case 'c':
                fprintf(stderr, "the command line = [%s]\n", optarg);
                break;

            case '?':
            case 'h':
            default:
                usage();
                break;
        }
    }

    node_transcode_task_init();
    node_transcode_server();

    return ret;
}



