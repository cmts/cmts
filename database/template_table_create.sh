#!/bin/bash

CREATE_TRANSCODE_TEMPLATE_SQL='create table cmts_template (id serial primary key, template_name varchar(128), filter_param varchar(128), video_param varchar(128), audio_param varchar(128), subtitle_param varchar(128)) ';

mysql --user='root' --password='root' --database='cmts' --execute="$CREATE_TRANSCODE_TEMPLATE_SQL"

