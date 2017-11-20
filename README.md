# Dependencies  
  
## MYSQL C client  
  
#### Debian/Ubuntu  
```  
sudo apt-get install libmysqlclient libmysqlclient-dev  
```  
  
#### CentOS/Fedora/RHEL  
```  
yum install mysql-devel  
```  
  
## HIREDIS C client  
git clone https://github.com/redis/hiredis.git  
Find the makefile.    
  
```    
make    
```  
```  
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

## TPL binary serialization library

```
https://github.com/troydhanson/tpl.git  
```

Follow README instructions  
