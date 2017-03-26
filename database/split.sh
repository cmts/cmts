#/bin/bash

SPLIT_CMD=$1

echo "${SPLIT_CMD}"

SCHEDULER_RESULT=`/usr/local/openresty/nginx/html/scheduler -c "${SPLIT_CMD}" -a 127.0.0.1 | grep "waiting pid=" | awk -F"pid=" '{ print $2 }' | awk -F"]" '{ print $1 }'`

echo "($SCHEDULER_RESULT)"


