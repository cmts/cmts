#/bin/bash

INPUT_FILE=$1
TEMPLATE_NAME=$2

GET_TRANSCODE_PARAM=`/usr/local/openresty/nginx/html/template_table_select.sh "${TEMPLATE_NAME}" 2> /dev/null |grep vcodec`

DST_FILE_DIR="/usr/local/openresty/nginx/html/input/"


DST_FILE_NAME="${INPUT_FILE}.m3u8"


CMD_STRING="ffmpeg -i ${INPUT_FILE} ${GET_TRANSCODE_PARAM} ${DST_FILE_NAME}"


echo "${CMD_STRING}"

SCHEDULER_RESULT=`/usr/local/openresty/nginx/html/scheduler -c "${CMD_STRING}" -a 127.0.0.1 | grep "waiting pid=" | awk -F"pid=" '{ print $2 }' | awk -F"]" '{ print $1 }'`

echo "($SCHEDULER_RESULT)"


SET_TASK_INSERT=`/usr/local/openresty/nginx/html/task_table_insert.sh "{$INPUT_FILE}" "${DST_FILE_NAME}" "${TEMPLATE_NAME}" "${SCHEDULER_RESULT}"`




