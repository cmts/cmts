#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define TASK_MAX_NUM            48
#define TASK_CMD_MAX_LEN        2048
typedef struct transcode_taskinfo_s {
    pid_t task_pid;
    char cmd[TASK_CMD_MAX_LEN];
}transcode_taskinfo_t;

transcode_taskinfo_t g_task[TASK_MAX_NUM];

int node_transcode_task_init()
{
    memset(g_task, 0, sizeof(g_task));

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

    return ret;
}



