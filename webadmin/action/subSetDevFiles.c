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
#include "../services/snmpServices.h"
//#include "../card/snmpSetMeta.h"

void sendSnmpToRecovery(){
	
	char itemNames[500];       //硬件参数名称
	char itemValues[2000];     //要设置的硬件参数值
	char mValue[4000]="";      //设置后,硬件参数值
	strcpy(itemNames,"saveRecovery");
	strcpy(itemValues,"");
	
	//setDeviceParameters(itemNames,itemValues,mValue);
}


int cgiMain(void){
	char value[20];
	int devId = 0;
   char pkgName[512];
   char reqtype[128];
//   char extArgs[128];
   int slot = 0;
   char cmd[512];
	char devtype[128];
	
	 /* ==================基本校验 ====================== */
	cgiFormString("devId",value,20);
	devId = atol(value);
	cgiFormString("slot",value,20);
	slot = atol(value);
	cgiFormString("pkgName",pkgName,512);
	cgiFormString("reqtype",reqtype,128);
	//getCardTypeByPosition(0,devtype);
	if(!strcmp(reqtype,"setCrossLinkRemarks")){			//1.交叉备注信息
		//删除临时目录文件 
		sprintf(cmd,"/app/sysfile/%s",pkgName);
		remove(cmd);
		//发命令下载文件  
		sprintf(cmd,"/usr/bin/wget http://%s/file/%s -P %s >/app/log/result 2>&1",cgiRemoteAddr,pkgName,"/app/sysfile/");
		system(cmd);
		//删除原备份 
//#ifdef MSAP_DT
		if(!strcmp(devtype,"SDH")){		//台式155B
			sprintf(cmd,"/app/s_agent/CardInfo/0/CrosslinkRemarks.txt");
			remove(cmd);
			//拷贝
	   		sprintf(cmd,"mv /app/sysfile/%s /app/s_agent/CardInfo/0/CrosslinkRemarks.txt",pkgName); 
			system(cmd);
		}else{							//机架
//#else 
			sprintf(cmd,"/app/s_agent/CardInfo/7/CrosslinkRemarks.txt");
			remove(cmd);
			//拷贝
	   		sprintf(cmd,"mv /app/sysfile/%s /app/s_agent/CardInfo/7/CrosslinkRemarks.txt",pkgName); 
			system(cmd);
		}
//#endif 
	}else if(!strcmp(reqtype,"setConfigFiles")){	//2.配置信息
		//删除临时目录文件 
		sprintf(cmd,"/app/sysfile/%s",pkgName);
		remove(cmd);
		//发命令下载文件  
		sprintf(cmd,"/usr/bin/wget http://%s/file/%s -P %s >/app/log/result 2>&1",cgiRemoteAddr,pkgName,"/app/sysfile/");
		system(cmd);
		//暂停网管代理 
//		sprintf(cmd,"pkill agent >/app/log/result 2>&1 "); 
//		system(cmd);
		//删除原备份 
		
		//if(slot == 0xFF){		//整个机架 
		//	sprintf(cmd,"rm -rf /app/s_agent/CardInfo/*  >/app/log/result 2>&1 "); 
		//}else{					//单个板卡 
		//	sprintf(cmd,"rm -f /app/s_agent/CardInfo/%d/*  >/app/log/result 2>&1 ",slot); 
		//}
		//system(cmd);
		
		//解压 
   		sprintf(cmd,"tar -zxvf /app/sysfile/%s -C %s",pkgName,"/"); 
   		webLog("cmd:%s",cmd);
		system(cmd);
		//启动网管代理
//		chdir("/app/bin");
//		sprintf(cmd,"./agent >/dev/null 2>&1 &"); 
//		system(cmd);
		//发SNMP命令设置标志位
		sendSnmpToRecovery();
		
	}else if(!strcmp(reqtype,"setAlarmFiles")){	//3.告警文件 
		//删除临时目录文件 
		sprintf(cmd,"/app/sysfile/%s",pkgName);
		remove(cmd);
		//发命令下载文件  
		sprintf(cmd,"/usr/bin/wget http://%s/file/%s -P %s >/app/log/result 2>&1",cgiRemoteAddr,pkgName,"/app/sysfile/");
		system(cmd);
		//删除原备份 
		sprintf(cmd,"/app/s_agent/MSAP_AlarmLevel.cfg");
		remove(cmd);
		//拷贝
		sprintf(cmd,"mv /app/sysfile/%s /app/s_agent/MSAP_AlarmLevel.cfg",pkgName); 
		system(cmd);
	}else{															//未定义 
		//do nothing 
	
	}
	
	//清辅助文件 
	sprintf(cmd,"/app/log/result");
	remove(cmd);
	
	cgiHeaderContentType("text/html; charset=utf-8");
	printf("ok");
	return 0;
}
