#!/bin/bash

declare -a SRC_FILENAMES
SRC_FILE_NAME=$1
DST_FILE_NAME=$2
TEMPLATE_NAME=$3
TASK_PID=$4

################## Insert transcode task into cmts_task #################

TASK_INSERT_SQL="insert into cmts_task (input, template_name, output, task_pid) value (\"$SRC_FILE_NAME\", \"${DST_FILE_NAME}\", \"${TEMPLATE_NAME}\", \"${TASK_PID}\" )"
#echo ${TEMPLATE_AUDIO_PARAM_SELECT_SQL}
mysql --user='root' --password='root' --database='cmts' --execute="$TASK_INSERT_SQL"






