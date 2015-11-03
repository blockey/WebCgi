//upload.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "../base/base.h"
#include"../lib/cgic.h"

// #include "../card/snmpSetMeta.h"

int cgiMain(void){
	char value[20];
	int slot = 0;
	int devId = 0;
//	char extArgs[128];
	char reqtype[128];
   	char cmd[512];
	char tableName[256]; 
	char configToPcRoute[128] = "/app/webapp/httpd/html/file/to_pc/";
	//long t1, t2;
//   	int ret  = 0;
	// char devtype[128];
	 /* ==================基本校验 ====================== */
	//webLog("-------------subGetDevFilie--------------start");
	cgiFormString("reqtype",reqtype,128);
	
	if(!strcmp(reqtype,"getConfigRar")){	//2.配置信息	
		cgiFormString("slot",value,20);
		slot = atol(value);
		cgiFormString("devId",value,20);
		devId = atol(value);
		sprintf(cmd, "chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		//删除文件 
		sprintf(cmd,"/app/webapp/httpd/html/file/to_pc/cardCfg_%02d.tar.gz",devId);
		remove(cmd);
		chdir("/app/webapp/httpd/html/file/to_pc");
		sprintf(cmd,"tar -zcf cardCfg_%02d.tar.gz *.csv", devId);
		system(cmd);
		sprintf(cmd,"cp /app/webapp/httpd/html/file/to_pc/cardCfg_%02d.tar.gz /app/webapp/httpd/html/file/to_gateway/", devId);
		system(cmd);
		sprintf(cmd,"chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
	}else if(!strcmp(reqtype,"getConfigBak")){	//2.配置信息	
		cgiFormString("slot",value,20);
		slot = atol(value);
		cgiFormString("devId",value,20);
		devId = atol(value);
		sprintf(cmd, "chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		sprintf(cmd, "chmod 777 /app/webapp/datas/*");
		system(cmd);
		//删除文件 
		sprintf(cmd,"/app/webapp/datas/cardCfgBak_%02d.tar.gz",devId);
		remove(cmd);
		chdir("/app/webapp/datas");
		sprintf(cmd,"tar -zcf cardCfgBak_%02d.tar.gz ptn.db", devId);
		system(cmd);
		sprintf(cmd, "chmod 777 /app/webapp/datas/*");
		system(cmd);
		sprintf(cmd,"cp /app/webapp/datas/cardCfgBak_%02d.tar.gz /app/webapp/httpd/html/file/to_gateway/", devId);
		system(cmd);
		sprintf(cmd,"chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
	}else if(!strcmp(reqtype,"getConfigFiles")){	//2.配置信息	
		cgiFormString("slot",value,20);
		slot = atol(value);
		cgiFormString("devId",value,20);
		devId = atol(value);
		//表名
		cgiFormString("tableName",tableName,256);
		
		sprintf(cmd,"chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		sprintf(cmd,"cp /app/webapp/httpd/html/file/to_pc/%s.csv /app/webapp/httpd/html/file/to_gateway/", tableName);
		system(cmd);
		sprintf(cmd,"chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
	}else if(!strcmp(reqtype,"getAlarmFiles")){	//3.告警文件 
		cgiFormString("devId",value,20);
		devId = atol(value);
		//删除文件 
		sprintf(cmd,"%s/MSAP_AlarmLevel_%02d.cfg",configToPcRoute,devId);
		remove(cmd);
		
		//拷贝文件 
		sprintf(cmd,"cp -f /app/s_agent/MSAP_AlarmLevel.cfg  %s/MSAP_AlarmLevel_%02d.cfg >/app/log/result 2>&1",configToPcRoute,devId);
		system(cmd);
	}else{															//未定义 
		//do nothing 
	}
	
	//清辅助文件 
	sprintf(cmd,"/app/log/result");
	remove(cmd);
	
	cgiHeaderContentType("text/html; charset=utf-8");
	printf("MSAP_AlarmLevel_d.cfg");
   
	return 0;
}
