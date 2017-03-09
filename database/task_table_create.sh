#!/bin/bash

CMTS_TASK_TABLE_CREATE_SQL='create table cmts_task (id serial primary key, input varchar(128), template_name varchar(128), output varchar(128), task_pid varchar(8)) ';

mysql --user='root' --password='root' --database='cmts' --execute="$CMTS_TASK_TABLE_CREATE_SQL"

