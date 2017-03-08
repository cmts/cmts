#!/bin/bash

declare -a SRC_FILENAMES

################## Read file name for task inputs ################
FILE_INFO_INSERT_SQL="select dst_filename from cmts_files where status = 0";
echo ${FILE_INFO_INSERT_SQL}
PROVERB="`mysql --user='root' --password='root' --database='cmts' --execute=\"$FILE_INFO_INSERT_SQL\" | awk -F"dst_filename" '{ print $1 }'` "
read -a SRC_FILENAMES <<< $PROVERB
STREAM_NUM=${#SRC_FILENAMES[@]}
for((i=0;i<$STREAM_NUM;i++))
{
    echo "=== ${SRC_FILENAMES[$i]}"
}


################## Read template for transdor #################
TEMPLATE_NAME_SELECT_SQL="select template_name from cmts_template";
#echo ${TEMPLATE_NAME_SELECT_SQL}
TEMPLATE_NAME="`mysql --user='root' --password='root' --database='cmts' --execute=\"$TEMPLATE_NAME_SELECT_SQL\" | awk -F"template_name" '{ print $1 }' | grep -o \"[^ ]\+\( \+[^ ]\+\)*\"` "
if [[ $TEMPLATE_NAME == "NULL" ]]
then
    TEMPLATE_NAME=""
fi

TEMPLATE_FILTER_PARAM_SELECT_SQL="select filter_param from cmts_template";
#echo ${TEMPLATE_FILTER_PARAM_SELECT_SQL}
TEMPLATE_FILTER_PARAM_NAME="`mysql --user='root' --password='root' --database='cmts' --execute=\"$TEMPLATE_FILTER_PARAM_SELECT_SQL\" | awk -F"filter_param" '{ print $1 }' | grep -o \"[^ ]\+\( \+[^ ]\+\)*\"`"
if [[ $TEMPLATE_FILTER_PARAM_NAME == "NULL" ]]
then
    TEMPLATE_FILTER_PARAM_NAME=""
fi

TEMPLATE_VIDEO_PARAM_SELECT_SQL="select video_param from cmts_template";
#echo "${TEMPLATE_VIDEO_PARAM_SELECT_SQL}"
TEMPLATE_VIDEO_PARAM_NAME="`mysql --user='root' --password='root' --database='cmts' --execute=\"$TEMPLATE_VIDEO_PARAM_SELECT_SQL\" | awk -F"video_param" '{ print $1 }' | grep -o \"[^ ]\+\( \+[^ ]\+\)*\"` "
if [[ $TEMPLATE_VIDEO_PARAM_NAME == "NULL" ]]
then
    TEMPLATE_VIDEO_PARAM_NAME=""
fi

TEMPLATE_AUDIO_PARAM_SELECT_SQL="select audio_param from cmts_template";
#echo ${TEMPLATE_AUDIO_PARAM_SELECT_SQL}
TEMPLATE_AUDIO_PARAM_NAME="`mysql --user='root' --password='root' --database='cmts' --execute=\"$TEMPLATE_AUDIO_PARAM_SELECT_SQL\" | awk -F"audio_param" '{ print $1 }' | grep -o \"[^ ]\+\( \+[^ ]\+\)*\"` "
if [[ $TEMPLATE_AUDIO_PARAM_NAME == "NULL" ]]
then
    TEMPLATE_AUDIO_PARAM_NAME=""
fi

########## strcat ffmpeg command line ######
#echo "[ $TEMPLATE_NAME ]"

#echo "[ ${TEMPLATE_FILTER_PARAM_NAME} ]"

#echo "[ $TEMPLATE_VIDEO_PARAM_NAME ]"

#echo "[ $TEMPLATE_AUDIO_PARAM_NAME ]"

for((i=0;i<$STREAM_NUM;i++))
{
    TEMP_OUTPUT_FILE_NAME=${SRC_FILENAMES[$i]};
    OUTPUT_FILE_NAME="${TEMP_OUTPUT_FILE_NAME%%.*}_$TEMPLATE_NAME.m3u8"
    echo "ffmpeg -i ${SRC_FILENAMES[$i]}  ${TEMPLATE_FILTER_PARAM_NAME} $TEMPLATE_VIDEO_PARAM_NAME  $TEMPLATE_AUDIO_PARAM_NAME  -hls_list_size 0 $OUTPUT_FILE_NAME"
}






