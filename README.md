# REDSQL Library [![Build Status](https://travis-ci.org/billxiong24/redsql.svg?branch=master)](https://travis-ci.org/billxiong24/redsql)

## Dependencies  
  
### MYSQL C client  
To install MYSQL C client, run the following command, depending on your distribution.  
#### Debian/Ubuntu  
`sudo apt-get install libmysqlclient-dev`  
  
#### CentOS/Fedora/RHEL  
`yum install mysql-devel`  
  
### HIREDIS C client  
First, we need libevent library. Run the following command depending on your distribution.  
#### Debian/Ubuntu
`sudo apt-get install libevent-dev`  

#### CentOS/Fedora/RHEL
`sudo yum install libevent-devel`  

#### Setting up hiredis

NOTE: You can also run `scripts/setup_redis.sh`, which sets up hiredis for use.
```
git clone https://github.com/redis/hiredis.git  
make  
sudo make install  
```
  
Make header files and library globally available  
```  
sudo mkdir /usr/include/hiredis    
sudo cp libhiredis.so /usr/lib/    
sudo cp hiredis.h /usr/include/hiredis/    
sudo cp read.h /usr/include/hiredis/    
sudo cp sds.h /usr/include/hiredis/    
ldconfig    
```    

Hiredis header files will be found at ```/usr/include/hiredis```  

## Making the library
Run `. ./setenv` in the scripts directory, replacing values where appropriate. This sets up the appropriate environment variables.  
Run `make`.  
Run `make test` to run unit tests and ensure the library is working. exit code 0 means success.  
