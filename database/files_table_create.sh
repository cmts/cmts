#!/bin/bash

CMTS_FILES_TABLE_CREATE_SQL='create table cmts_files (id serial primary key, src_filename varchar(128), movie_name varchar(128), dst_filename varchar(128), media_info varchar(256), status int(1)) ';

mysql --user='root' --password='root' --database='cmts' --execute="$CMTS_FILES_TABLE_CREATE_SQL"

