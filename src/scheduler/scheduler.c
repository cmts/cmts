#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#define MAX_CMD_LINE_SIZE   4096

void usage(char *program_name)
{
    fprintf(stderr, "%s :\n", program_name);
    fprintf(stderr, "%s :\n", program_name);
    return;
}

int main(int argc, char *argv[])
{
    char ch = 0;
    char *ipaddr = NULL;
    char commandline[MAX_CMD_LINE_SIZE];

    if (argc < 3) {
        usage(argv[0]);
    }
    while ((ch = getopt(argc, argv, "hc:a:")) != -1) {
        switch (ch) {
            case 'c':
                memset(commandline, 0, sizeof(commandline));
                strncpy(commandline, optarg, strlen(optarg));
                fprintf(stderr, "the command line = [%s]\n", commandline);
                break;
            case 'a':
                ipaddr = optarg;
                fprintf(stderr, "ipaddr = [%s]\n", ipaddr);
                break;
            case '?':
            case 'h':
            default:
                usage(argv[0]);
                break;
        }
    }


    return 0;
}



