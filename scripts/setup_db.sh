#!/bin/bash
if [ "$#" -lt 1 ]; then
    echo "Input user"
    exit 1
fi
USER=$1
cd "../db_mock" || exit 1

mysql -u "$USER" -p < "employees.sql" 
