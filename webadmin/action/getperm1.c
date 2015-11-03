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
#define PERMFILES "/app/webapp/httpd/html/performances"

int cgiMain() {	
	char cmd[256];
	char emId[20];              
	char name[40];
	char url[100] ;
	/* ==================基本校验 ====================== */
	
	/*  获取请求参数  */
	cgiFormString("emId",emId,20);
	cgiFormString("name",name,40);

	//下载性能文件 
	//sprintf(cmd,"wget http://%s/performance/%s_%s.bin -P %s >/app/log/result //2>&1",getDistInnerIp(atol(emId)),emId,name,PERMFILES);
    //system(cmd); 
	//webLog("cmd: %s",cmd);

    if(strstr(name,"cpe_status"))
    {
    	sprintf(url,"http://%s/performance/%s_%s.tar.gz",getLocalIp(),emId,name);
    }
    else
    {
    	sprintf(url,"http://%s/performance/%s_%s.bin",getLocalIp(),emId,name);
    }
	webLog("url: %s",url);
	cgiHeaderLocation(url);
	return 0;
}



