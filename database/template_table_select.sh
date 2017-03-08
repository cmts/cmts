#!/bin/bash

declare -a TEMPLATE_VIDEO_PARAM
declare -a TEMPLATE_AUDIO_PARAM
declare -a TEMPLATE_FILTER_PARAM
declare -a TEMPLATE_SUBTITLE_PARAM

TEMPLATE_NAME=$1

#TEMPLATE_VIDEO_PARAM="[{\"test\":{\"video\":{\"vcodec\":\"h264_nvenc\",\"b:v\":\"2000k\",\"r:v\":\"30\"},\"audio\":{\"acodec\":\"aac\"}}, \"think_test\": {\"video\": {\"vcodec\":\"h264_nvenc\"}, \"audio\":{\"acodec\":\"aac\"}}}]"

TEMPLATE_VIDEO_SELECT_SQL="select video_param from cmts_template where template_name=\"$TEMPLATE_NAME\"";
VIDEO_RESULT="`mysql --user='root' --password='root' --database='cmts' --execute=\"$TEMPLATE_VIDEO_SELECT_SQL\" | awk -F"video_param" '{ print $1 }' | grep -o \"[^ ]\+\( \+[^ ]\+\)*\"`"


TEMPLATE_AUDIO_SELECT_SQL="select audio_param from cmts_template where template_name=\"$TEMPLATE_NAME\"";
AUDIO_RESULT="`mysql --user='root' --password='root' --database='cmts' --execute=\"$TEMPLATE_AUDIO_SELECT_SQL\" | awk -F"audio_param" '{ print $1 }' | grep -o \"[^ ]\+\( \+[^ ]\+\)*\" `"




echo "$VIDEO_RESULT $AUDIO_RESULT"

