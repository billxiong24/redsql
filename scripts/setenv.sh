#!/bin/bash
export LD_LIBRARY_PATH="/usr/local/lib:."

# angled includes are treated differently by different compilers
# various header files in mysql files use angled includes for header files in 
# /usr/include/mysql, so we add it to the include path. This is compiler dependent,
# so comment as necessary
export C_INCLUDE_PATH=.:/usr/include/mysql/:/usr/include

#### MYSQL CONFIG VALUES (replace these with own values)
export MYSQL_HOST=localhost
export MYSQL_USER=root
export MYSQL_PASS=

#### REDIS CONFIG VALUES (replace these with own values)
export REDIS_HOST=localhost
export REDIS_PORT=6379
