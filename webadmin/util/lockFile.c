#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/file.h>
#include "lockFile.h"
#include "../base/base.h"
/*
*对指定文件描述符加锁
*/
int lock_set(int fd,int types){
	struct flock lock;			//锁 
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	int times = 0;
	while(1){
		//如果在指定的加锁次数内未能对文件加锁，则返回文件繁忙信息
		if(times > MAX_WAIT_TIME){
			webLog("file=%s;func=%s;msg=the file has been locked;",__FILE__,__FUNCTION__);
			return FILE_BUSY;	
		}
      lock.l_type=types;
		/* 设置锁 */
      if (fcntl(fd,F_SETLK,&lock)==0){
          return 0;
   	}
//   	webLog("try %d ....",times);
		//睡眠一秒钟后再查询是否可对文件加锁
       sleep(1);
       ++times;
	}
}
/* open file */
lock_t lck_open(const char *path,const char *mode,FILE **fp){
	int types = 0;
	int lmode = 0;
	//mode 
	// read lock 
	if(!strcmp(mode,"r") || !strcmp(mode,"r+")){
		types = F_RDLCK;
		lmode = O_RDONLY;
	}else{
		//write lock 
		types = F_WRLCK;
		lmode = O_WRONLY;
	}
	lock_t fd = open(path,lmode);
	if(fd < 0){
		webLog("file=%s;func=%s;msg=open file %s error,no such path;",__FILE__,__FUNCTION__,path);
		return ERROR;
	}
	//lock file 
	if(lock_set(fd,types) < 0){
		webLog("file=%s;func=%s;msg=the file has been locked;",__FILE__,__FUNCTION__);
		close(fd);
		return FILE_BUSY;
	}
	
	*fp = fopen(path,mode);
	if(*fp == NULL){
		webLog("file=%s;func=%s;msg=open file :%s error,system error;",__FILE__,__FUNCTION__,path);
		close(fd);
		return ERROR;
	}
	
	//*fp = fdopen(fd,mode);
	return fd ;
}

/* release file  */
void  lck_close(lock_t lock,FILE *fp){
	lock_set(lock,F_UNLCK);	
	close(lock);
	fclose(fp);
}
