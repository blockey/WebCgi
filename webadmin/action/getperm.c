#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "../lib/cgic.h"
#include "../base/base.h"
#define ERRORIP "192.168.1.1"
#define MAIN_PERMFILES "/app/webapp/httpd/html/mainperformances"





int cgiMain() {	
	char cmd[256];
	char emId[20];         
	char slot[20];         
	char port[20];         
	char subport[20];         
	char item[20];         
	char proid[20];         
	char pw[50];                
	char calpw[50];   
	char res[256];					//处理结果 
	char srcfile[100];
	char servicefile[100];
	char *servicepath = "/app/webapp/httpd/html/performance";
	char *srcpath = "/app/webapp/datas/perm";
	char url[100] ;
	/* ==================基本校验 ====================== */
	
	/*  获取请求参数  */
	cgiFormString("emId",emId,20);
	cgiFormString("slot",slot,20);
	cgiFormString("port",port,20);
	cgiFormString("subport",subport,20);
	cgiFormString("item",item,20);
	cgiFormString("proid",proid,20);
	cgiFormString("pw",pw,50);	
	
	//webLog("getperm1");
	//删除原来的文件
	//sprintf(cmd,"%s/%s_%s_%s_%s_%s_%s.txt",MAIN_PERMFILES,emId,slot,port,subport,item,proid);
	sprintf(cmd,"%s/%s_%s_%s_%s_%s_%s.bin",MAIN_PERMFILES,emId,slot,port,subport,item,proid);
	remove(cmd); 
	//webLog("getperm2");
	//下载性能文件 
	//sprintf(cmd,"/usr/bin/wget http://%s.%s/performance/%s_%s_%s_%s_%s_%s.txt -P %s >/app/log/result 2>&1",INNERIPADDR,emId,emId,slot,port,subport,item,proid,MAIN_PERMFILES);
	//sprintf(cmd,"/usr/bin/wget http://%s/performance/%s_%s_%s_%s_%s_%s.txt -P %s >/app/log/result 2>&1",getDistInnerIp(atol(emId)),emId,slot,port,subport,item,proid,MAIN_PERMFILES);
	sprintf(cmd,"/usr/bin/wget http://%s/performance/%s_%s_%s_%s_%s_%s.bin -P %s >/app/log/result 2>&1",getDistInnerIp(atol(emId)),emId,slot,port,subport,item,proid,MAIN_PERMFILES);
	system(cmd); 

	//webLog("getperm:%s",cmd);
	/*   ==================   输出hmtl内容  ===================  */
	
	//清辅助文件 
	sprintf(cmd,"/app/log/result");
	remove(cmd);
	
	//sprintf(url,"http://%s/mainperformances/%s_%s_%s_%s_%s_%s.txt",getLocalIp(),emId,slot,port,subport,item,proid);
	sprintf(url,"http://%s/mainperformances/%s_%s_%s_%s_%s_%s.bin",getLocalIp(),emId,slot,port,subport,item,proid);
//	cgiHeaderContentType("text/html; charset=utf-8");
//	cgiHeaderStatus(301,"redict");
	cgiHeaderLocation(url);
	return 0;
}



