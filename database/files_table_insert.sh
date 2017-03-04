#!/bin/bash

SRC_FILE_NAME="$1"

echo "SRC FILE NAME = ${SRC_FILE_NAME}"
MOVIE_NAME="$2"

DST_FILE_NAME_MD5=`cat "${SRC_FILE_NAME}" | md5 `
DST_FILE_DIR="/usr/local/nginx/html/input/"


#echo ${DST_FILE_NAME_MD5}
if [ -d ${DST_FILE_DIR} ]
then
    echo "has dir"
else
    mkdir ${DST_FILE_DIR}
fi

DST_FILE_NAME="${DST_FILE_DIR}/${DST_FILE_NAME_MD5}.${SRC_FILE_NAME##*.}"

echo "SRC FILE NAME = ${SRC_FILE_NAME}"
echo "DST FILE NAME = ${DST_FILE_NAME}"

cp "${SRC_FILE_NAME}" ${DST_FILE_NAME}

FILE_INFO_INSERT_SQL="insert into cmts_files (src_filename, movie_name, dst_filename, status) value ( \"${SRC_FILE_NAME}\", \"${MOVIE_NAME}\", \"${DST_FILE_NAME}\", 0 )";

echo ${FILE_INFO_INSERT_SQL}

mysql --user='root' --password='root' --database='cmts' --execute="$FILE_INFO_INSERT_SQL"
