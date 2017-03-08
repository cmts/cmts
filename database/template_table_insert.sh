#!/bin/bash

declare -a TEMPLATE_VIDEO_PARAM
declare -a TEMPLATE_AUDIO_PARAM
declare -a TEMPLATE_FILTER_PARAM
declare -a TEMPLATE_SUBTITLE_PARAM

TEMPLATE_NAME="test_StevenLiu"

#TEMPLATE_VIDEO_PARAM="[{\"test\":{\"video\":{\"vcodec\":\"h264_nvenc\",\"b:v\":\"2000k\",\"r:v\":\"30\"},\"audio\":{\"acodec\":\"aac\"}}, \"think_test\": {\"video\": {\"vcodec\":\"h264_nvenc\"}, \"audio\":{\"acodec\":\"aac\"}}}]"

TEMPLATE_INSERT_SQL="insert into cmts_template (template_name, video_param, audio_param) value (\"test\", \"-vcodec h264_nvenc -b:v 2000k\", \"-acodec aac\")"

echo "${TEMPLATE_INSERT_SQL}"
mysql --user='root' --password='root' --database='cmts' --execute="$TEMPLATE_INSERT_SQL"


