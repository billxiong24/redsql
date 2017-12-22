#!/bin/bash
USER=
if [ "$#" -lt 1 ]; then
    USER="root"
else
    USER=$1
fi
cd "../db_mock" || exit 1

mysql -u "$USER" --password=$(printenv MYSQL_PASS) < "employees.sql" 
