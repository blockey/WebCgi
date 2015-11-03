#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdarg.h> 
#include "../lib/cgic.h"
#include "../base/base.h"
#include "../util/commonUtil.h"
#define ERRORIP "192.168.1.1"
#define INNERIPADDR "192.168.0"
#define MAIN_BACKFILES "/app/webapp/httpd/html/file/to_pc"


int   white_sock = 0; 
int   htconnect(char   *host,   int   port){ 
        struct   hostent   *   site; 
        struct   sockaddr_in   me; 
        site   =   gethostbyname(host); 
        if   (site==NULL)   return   -2; 
        white_sock   =   socket(AF_INET,SOCK_STREAM,0); 
        if   (white_sock   <0)   return   -1; 
        memset(&me,   0,   sizeof(struct   sockaddr_in)); 
        memcpy(&me.sin_addr,   site->   h_addr_list[0],   site->   h_length); 
        me.sin_family   =   AF_INET; 
        me.sin_port   =   htons(port); 
        return   (connect(white_sock,   (struct   sockaddr   *)&me,sizeof(struct   sockaddr))   <0)   ?   -1   :   white_sock; 
} 

int   htsend(int   sock,   char   *fmt,   ...){ 
        char   BUF[1024]; 
        va_list   argptr; 
        va_start(argptr,fmt); 
        vsprintf(BUF,fmt,argptr); 
        va_end(argptr); 
        printf("%s", BUF); 
        return   send(sock,BUF,strlen(BUF),0); 
} 
        
int SendCmdToCgi(char *reqtype, char *tableName, int itemId,int slot,char *extArgs){ 
   	int  black_sock;
   	char  msg[1024]={0};
   	char  request[1024] = {0};
   	char URL[1024] = {0};
   	char ipaddr[32];
   	sprintf(URL,"/cgi-bin/webadmin/subGetDevFiles.cgi");
   	char *Content = "application/x-www-form-urlencoded";
   	char conf[1024] = {0};
	sprintf(conf,"reqtype=%s&devId=%d&tableName=%s&slot=%d&extArgs=%s",reqtype,itemId, tableName,slot,extArgs); 
#ifdef RUN_ON_PC	
	sprintf(ipaddr,"192.168.1.169");
#else 
	char *lIpaddr = getDistInnerIp(itemId);
	webLog("-------lIpaddr=%s",lIpaddr);
	if(lIpaddr != NULL){
		sprintf(ipaddr,"%s",lIpaddr);
	}else{
		webLog("getLocalIpError\n");
		return 0;
	}
#endif 

   black_sock   =   htconnect(ipaddr,80);
   if(black_sock   <1){
  		 return 0;  
  	}
  sprintf(request, "POST %s HTTP/1.0\r\nAccept: */*\r\nContent-Length:%d\r\nContent-Type: %s\r\nAuthorization:Basic YWRtaW46YWRtaW4=\r\nCredentials:admin:admin\r\n\r\n%s",URL,strlen(conf),Content,conf);
  webLog("request = %s", request);
  send(black_sock, request,sizeof(request),0);
  recv(black_sock,msg,sizeof(msg),0);   
  webLog("return:%s",msg);
  close(black_sock);
  close(white_sock);
  return 1;
}

int cgiMain() {	
	char cmd[256];
	int  devId = 0;         
	int  slot = 0; 
	char tableName[256]; 
	char value[256];  
//	char extArgs[128];
	char reqtype[64];     
//	char pw[50];                
//	char calpw[50];   
//	char res[256];					//处理结果 
//	char srcfile[100];
//	char servicefile[100];
//	char *srcpath = "/app/webapp/datas/perm";
	char url[256] ;
	/* ==================基本校验 ====================== */
	//表名
	cgiFormString("tableName",tableName,256);
	webLog("tableName:%s",tableName);
	/*  获取请求参数  */
	//请求类型 
	cgiFormString("reqtype",reqtype,64);
	//----------------------------分3种类处理---------------------------------------
	
	if(!strcmp(reqtype,"getCrossLinkRemarks")){			//1.交叉备注信息
		//设备号
		cgiFormString("devId",value,20);
		devId=atol(value);
		//删除原来的文件
		sprintf(cmd,"%s/CrosslinkRemarks_%02d.txt",MAIN_BACKFILES,devId);
		remove(cmd); 
		//发下载请求 
		SendCmdToCgi(reqtype, tableName,devId,0,"resv");
		//下载文件 
#ifdef RUN_ON_PC 
		sprintf(cmd,"/usr/bin/wget http://192.168.1.169/subbackup/CrosslinkRemarks_%02d.txt -P %s >/app/log/result 2>&1",devId,MAIN_BACKFILES);
		system(cmd); 
#else 
		sprintf(cmd,"/usr/bin/wget http://%s/subbackup/CrosslinkRemarks_%02d.txt -P %s >/app/log/result 2>&1",getDistInnerIp(devId),devId,MAIN_BACKFILES);
		system(cmd);
		
#endif
		/*   ==================   输出hmtl内容  ===================  */
		sprintf(url,"http://%s/backup/CrosslinkRemarks_%02d.txt",getLocalIp(),devId); 
		
	}else if(!strcmp(reqtype,"getConfigFiles")){	//2.配置信息
		webLog("---reqtype=%s",reqtype);
		//设备号
		cgiFormString("devId",value,20);
		devId=atol(value);
		//槽位号
		cgiFormString("slot",value,20);
		slot=atol(value);
		//表名
		cgiFormString("tableName",tableName,256);
		sprintf(cmd,"chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		sprintf(cmd, "rm /app/webapp/httpd/html/file/to_gateway/%s.csv", tableName);
		system(cmd);
		//删除原来的文件
		//sprintf(cmd,"%s/CardCfgBackup_%02d_%02d.zip",MAIN_BACKFILES,devId,slot);
		//sprintf(cmd,"%s/12.zip",MAIN_BACKFILES);
		//remove(cmd); 
		//发下载请求 
		SendCmdToCgi(reqtype,tableName,devId,slot,"resv");
		//下载文件 
		#ifdef RUN_ON_PC 
		sprintf(cmd,"/usr/bin/wget http://192.168.1.169/file/to_gateway/%s.csv -P %s >/app/log/result 2>&1", tableName, "/app/webapp/httpd/html/file/to_gateway/");
		system(cmd);
		#else 
		sprintf(cmd,"/usr/bin/wget http://%s/file/to_gateway/%s.csv -P %s >/app/log/result 2>&1",getDistInnerIp(devId), tableName,"/app/webapp/httpd/html/file/to_gateway/");
		webLog("----------------cmd=%s",cmd);
		system(cmd);
		#endif	
		sprintf(cmd,"chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		
		/*   ==================   输出hmtl内容  ===================  */
		//sprintf(url,"http://%s/backup/CardCfgBackup_%02d_%02d.zip",getLocalIp(),devId,slot);
		//sprintf(url,"http://%s/file/to_pc/%s_%d_%d.csv",getLocalIp(), tableName, devId, slot);

		//sprintf(cmd,"cd /app/webapp/httpd/html/file/to_pc && rm cardCfg_%02d_%02d.zip && tar -cf cardCfg_%02d_%02d.zip *_%02d.csv", devId, slot, devId, slot, slot);
		//System(cmd);
		//ptn_export_to_csv_by_tablename(slot, tableName);
		sprintf(url,"http://%s/file/to_gateway/%s.csv",getLocalIp(),tableName);
		webLog("url = %s", url);
	}else if(!strcmp(reqtype,"getConfigRar")){	//3.配置信息压缩包
		
		//设备号
		cgiFormString("devId",value,20);
		devId=atol(value);
		//槽位号
		cgiFormString("slot",value,20);
		slot=atol(value);
		//表名
		cgiFormString("tableName",tableName,256);
		sprintf(cmd, "chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		//删除原来的文件
		sprintf(cmd, "rm /app/webapp/httpd/html/file/to_gateway/cardCfg_%02d.tar.gz", devId);
		system(cmd);
		//webLog("cmd = %s", cmd);

		//发下载请求 
		SendCmdToCgi(reqtype, tableName,devId,slot,"resv");
		//下载文件 
		#ifdef RUN_ON_PC 
		sprintf(cmd,"/usr/bin/wget http://192.168.1.169/file/to_gateway/cardCfg_%02d.tar.gz -P %s >/app/log/result 2>&1",devId, "/app/webapp/httpd/html/file/to_gateway/");
		system(cmd);
		#else 
		sprintf(cmd,"/usr/bin/wget http://%s/file/to_gateway/cardCfg_%02d.tar.gz -P %s >/app/log/result 2>&1",getDistInnerIp(devId),devId,"/app/webapp/httpd/html/file/to_gateway/");
		system(cmd);
		#endif	
		webLog("dist inner ip %s, local ip %s\n", getDistInnerIp(devId), getLocalIp());
		sprintf(cmd, "chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		/*   ==================   输出hmtl内容  ===================  */
		//sprintf(url,"http://%s/backup/CardCfgBackup_%02d_%02d.zip",getLocalIp(),devId,slot);
		//sprintf(url,"http://%s/file/to_pc/%s_%d_%d.csv",getLocalIp(), tableName, devId, slot);
		//删除原来的文件
		//sprintf(cmd,"/app/webapp/httpd/html/file/to_pc/*.csv");
		//remove(cmd); 
		//导出所有CSV
		//export_all_table_to_csv();
		
		
		//webLog("cmd = %s", cmd);
		sprintf(url,"http://%s/file/to_gateway/cardCfg_%02d.tar.gz",getLocalIp(), devId);
		webLog("url = %s", url);
	}else if(!strcmp(reqtype,"getConfigBak")){	//3.配置信息压缩包
		
		//设备号
		cgiFormString("devId",value,20);
		devId=atol(value);
		//槽位号
		cgiFormString("slot",value,20);
		slot=atol(value);
		//表名
		cgiFormString("tableName",tableName,256);
		sprintf(cmd, "chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		//删除原来的文件
		sprintf(cmd, "rm /app/webapp/httpd/html/file/to_gateway/cardCfgBak_%02d.tar.gz", devId);
		system(cmd);
		//webLog("cmd = %s", cmd);

		//发下载请求 
		SendCmdToCgi(reqtype, tableName,devId,slot,"resv");
		//下载文件 
		#ifdef RUN_ON_PC 
		sprintf(cmd,"/usr/bin/wget http://192.168.1.169/file/to_gateway/cardCfgBak_%02d.tar.gz -P %s >/app/log/result 2>&1",devId, "/app/webapp/httpd/html/file/to_gateway/");
		system(cmd);
		#else 
		sprintf(cmd,"/usr/bin/wget http://%s/file/to_gateway/cardCfgBak_%02d.tar.gz -P %s >/app/log/result 2>&1",getDistInnerIp(devId),devId,"/app/webapp/httpd/html/file/to_gateway/");
		system(cmd);
		#endif	
		sprintf(cmd, "chmod 777 /app/webapp/httpd/html/file/to_gateway/*");
		system(cmd);
		
		sprintf(url,"http://%s/file/to_gateway/cardCfgBak_%02d.tar.gz",getLocalIp(), devId);
	}else if(!strcmp(reqtype,"getAlarmFiles")){	//3.告警文件 
		//设备号
		cgiFormString("devId",value,20);
		devId=atol(value);
		//删除原来的文件
		sprintf(cmd,"%s/MSAP_AlarmLevel_%02d.cfg",MAIN_BACKFILES,devId);
		remove(cmd); 
		//发下载请求 
		SendCmdToCgi(reqtype, tableName,devId,0,"resv");
		//下载性能文件 
#ifdef RUN_ON_PC 
		sprintf(cmd,"/usr/bin/wget http://192.168.1.169/subbackup/MSAP_AlarmLevel_%02d.cfg -P %s >/app/log/result 2>&1",devId,MAIN_BACKFILES);
		system(cmd); 
#else 
		sprintf(cmd,"/usr/bin/wget http://%s/subbackup/MSAP_AlarmLevel_%02d.cfg -P %s >/app/log/result 2>&1",getDistInnerIp(devId),devId,MAIN_BACKFILES);
		system(cmd);
#endif 
		/*   ==================   输出hmtl内容  ===================  */
		sprintf(url,"http://%s/backup/MSAP_AlarmLevel_%02d.cfg",getLocalIp(),devId);
	}
	/*else if(!strcmp(reqtype,"getUpdateCsv")){
		sprintf(url,"http://%s/file/to_pc/update_csv.txt",getLocalIp());
	}*/
	else{//未定义 
		sprintf(url,"about:blank");
	}
	//清辅助文件 
	sprintf(cmd,"/app/log/result");
	remove(cmd);
	cgiHeaderLocation(url);
	webLog("getDevFile end");
	return 0;
}



