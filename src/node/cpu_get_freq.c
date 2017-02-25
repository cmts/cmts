#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#include "config.h"
#include "cpu_get_freq.h"

#define GET_CPU_FREQ "top -o CPU -s 5 -n1 -l 1|grep \"CPU usage\" | awk -F\"CPU usage:\" '{ print $2 }' | awk -F\",\" '{ print $1 + $2}'"

int cpu_get_freq(char *dst)
{
    FILE *fp = NULL;
    char buf[TASK_CMD_MAX_LEN];
    char cmdline[TASK_CMD_MAX_LEN];

    memset(cmdline, 0, sizeof(cmdline));
    strncpy(cmdline, GET_CPU_FREQ, strlen(GET_CPU_FREQ));

    fp = popen(cmdline, "r");
    if (!fp) {
        printf("popen %s error:%s", GET_CPU_FREQ, strerror(errno));
        return -ENOENT;
    }

    while (fgets(buf, sizeof(buf), fp)) {
        strncpy(dst, buf, strlen(buf) - 1);
        printf("\tSCANCPU:cpu_freq = [%s]", dst);
    }

    pclose(fp);


    return 0;
}



