#!/bin/bash

CREATE_TRANSCODE_TEMPLATE_SQL='drop table if exists cmts_template';

mysql --user='root' --password='root' --database='cmts' --execute="$CREATE_TRANSCODE_TEMPLATE_SQL"

