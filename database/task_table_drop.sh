#!/bin/bash

CMTS_TASK_TABLE_DROP_SQL='drop table if exists cmts_task';

mysql --user='root' --password='root' --database='cmts' --execute="$CMTS_TASK_TABLE_DROP_SQL"

