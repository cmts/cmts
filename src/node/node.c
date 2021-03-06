#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

#include "config.h"
#include "kill_process.h"
#include "cpu_get_freq.h"

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

static int process_cancel(int sockfd, char *buf)
{
    int ret = 0;
    char pid_process_msg[TASK_CMD_MAX_LEN];

    memset(pid_process_msg, 0, sizeof(pid_process_msg));
    ret = kill_pid_handle(pid_process_msg, buf);
    if (ret > 0) {
        if (send(sockfd, pid_process_msg, strlen(pid_process_msg), 0) == -1) {
            printf("send:%s", strerror(errno));
            return -1;
        }
    }

    if (sockfd) {
        close(sockfd);
    }

    return ret;
}

static int process_get_resource(int sockfd, char *msg)
{
    char buf[TASK_CMD_MAX_LEN];

    memset(buf, 0, sizeof(buf));
    cpu_get_freq(buf);

    if (send(sockfd, buf, strlen(buf), 0) == -1) {
        printf("send:%s", strerror(errno));
        return -1;
    }

    if (sockfd) {
        close(sockfd);
    }
    return 0;
}

static void zombie_handler (int sig)
{
      waitpid (-1, NULL, WNOHANG);
}

static void zombie_listen()
{
    struct sigaction act;

    act.sa_handler = zombie_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, NULL);
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
    char response_string[128];
    pid_t child_pid = 0;
    pid_t transcoder_pid = 0;
    int status = 0;

    zombie_listen();

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
                    ret = process_cancel(client_sockfd, recvbuf);
                } else if (!strncasecmp ((char *)&recvbuf, "getcpu", strlen ("getcpu"))) {
                    process_get_resource(client_sockfd, recvbuf);
                } else {
                    if (client_sockfd) {
                        close(client_sockfd);
                        client_sockfd = 0;
                    }
                }
            } else {
                if (!(transcoder_pid = fork())) {
                    close(client_sockfd);
                    client_sockfd = 0;
                    execlp("sh", "sh", "-c", recvbuf, NULL);
                    exit(1);
                }

                memset(response_string, 0, sizeof(response_string));
                sprintf (response_string, "waiting pid=%d", transcoder_pid);
                if (send(client_sockfd, response_string, strlen(response_string), 0) == -1) {
                    printf("child-send:%s", strerror(errno));
                }

                do {
                    ret = waitpid(transcoder_pid, &status, WUNTRACED | WCONTINUED);
                    if (ret == -1) {
                        printf("child:waitpid:%s", strerror(errno));
                        exit(-1);
                    }
                    if (WIFEXITED(status)) {
                    } else if (WIFSIGNALED(status)) {
                    } else if (WIFSTOPPED(status)) {
                    } else if (WIFCONTINUED(status)) {
                    }
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));

                if (client_sockfd) {
                    close(client_sockfd);
                    client_sockfd = 0;
                }
            }
        } else {
            if (client_sockfd) {
                close(client_sockfd);
                client_sockfd = 0;
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



