//upload.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <dirent.h>
#include "../base/base.h"
#include"../lib/cgic.h"

#define LOGFILE  "/app/webapp/httpd/html/file/update.log"



int cgiMain(void){
   char slots[128];
   char cardtypes[128];
   char pkgName[512];
   char cmd[512];
   
  // char *savePath = "/app/update/";
   int ret  = 0;
	
	 /* ==================基本校验 ====================== */
	cgiFormString("cardtypes",cardtypes,128);
	cgiFormString("slots",slots,128);
	cgiFormString("pkgName",pkgName,512);
	webLog("slots=%s,type=%s,",slots,cardtypes);

	//清过程信息
	sprintf(cmd,"echo 加载中.... >%s",LOGFILE);
	system(cmd);                                                                                                                                                                                                                                                                                                                                                                                   
	//清目录 
	system("rm /app/update/* -rf");
	//发命令下载文件  
	//sprintf(cmd,"/usr/bin/wget http://%s/file/%s -P %s >/app/log/result 2>&1",cgiRemoteAddr,pkgName,"/app/update");
	sprintf(cmd,"cp /app/webapp/httpd/html/file/%s /app/update",pkgName);
	webLog("----------cmd=%s",cmd);
	ret = system(cmd);
	
   sprintf(cmd,"/app/bin/pkgproc %s \'%s\' \'%s\' >/app/log/result 2>&1 &",pkgName,slots,cardtypes); 
   webLog("----------cmd=%s",cmd);
	system(cmd);
	//清辅助文件 
	sprintf(cmd,"/app/log/result");
	remove(cmd);
	
	cgiHeaderContentType("text/html; charset=utf-8");
	printf("ok");
	return 0;
}
