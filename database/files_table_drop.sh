#!/bin/bash

CMTS_FILES_TABLE_DROP_SQL='drop table if exists cmts_files';

mysql --user='root' --password='root' --database='cmts' --execute="$CMTS_FILES_TABLE_DROP_SQL"

