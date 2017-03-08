#/bin/bash

INPUT_FILE=$1
TEMPLATE_NAME=$2

GET_TRANSCODE_PARAM=`/usr/local/openresty/nginx/html/template_table_select.sh "${TEMPLATE_NAME}" 2> /dev/null |grep vcodec`


DST_FILE_NAME="${INPUT_FILE##*.}"



CMD_STRING="\"ffmpeg -i ${INPUT_FILE} ${GET_TRANSCODE_PARAM} ${DST_FILE_NAME}.m3u8\""


echo "${CMD_STRING}"

/usr/local/openresty/nginx/html/scheduler -c "${CMD_STRING}" -a 127.0.0.1
