#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

#define MAX_CMD_LINE_SIZE   4096

void usage(char *program_name)
{
    fprintf(stderr, "%s :\n", program_name);
    fprintf(stderr, "%s :\n", program_name);
    return;
}

int create_connect(int fd, char *ipaddr)
{
    struct sockaddr_in addr;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("create socket error\n");
        return -1;
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8899);
    addr.sin_addr.s_addr = inet_addr(ipaddr);
    if (connect(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("connect error\n");
        return -1;
    }

    return fd;
}

void task_send(int fd, char *command)
{
    write(fd, command, strlen(command));
}

void task_pid_recv(int fd)
{
    char buffer[MAX_CMD_LINE_SIZE];

    memset(buffer, 0, sizeof(buffer));
    read(fd, buffer, sizeof(buffer));
}

int main(int argc, char *argv[])
{
    char ch = 0;
    char *ipaddr = NULL;
    char commandline[MAX_CMD_LINE_SIZE];
    int sockfd = 0;

    if (argc < 3) {
        usage(argv[0]);
    }
    while ((ch = getopt(argc, argv, "hc:a:")) != -1) {
        switch (ch) {
            case 'c':
                memset(commandline, 0, sizeof(commandline));
                strncpy(commandline, optarg, strlen(optarg));
                fprintf(stdout, "the command line = [%s]\n", commandline);
                break;
            case 'a':
                ipaddr = optarg;
                fprintf(stdout, "ipaddr = [%s]\n", ipaddr);
                break;
            case '?':
            case 'h':
            default:
                usage(argv[0]);
                break;
        }
    }

    create_connect(sockfd, ipaddr);
    task_send(sockfd, commandline);
    task_pid_recv(sockfd);

    return 0;
}



