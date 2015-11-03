/*
*syslogDao.c
*日志DAO
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../base/base.h"
#include "syslogDao.h"
#include "../model/syslogModel.h"
#include "../util/syslogUtil.h"
#include "../util/adstring.h"
#include "../util/lockFile.h"
//全局变量 不同日志类型读取不同的日志文件
char *syslogFilePath;			//日志文件路径
/*
*根据系统参数名称获取参数值 
*@argsName 	:in 	参数名
*@argsValue	:out	参数值 
*find it ,return true ,else return false 
*/
boolean getSystemInfoByNameDao(const char *argsName,char *argsValue){
	char record[200];
	boolean result = false ;
	FILE *sysfile = fopen(systemInfoPath,"r");
	if(sysfile == NULL){
		return false; 
	}
	if(fgets(record,200,sysfile)){
		if(getValueByName(record,argsName,argsValue)){
			result = true;
		}
	}
	fclose(sysfile);
	return result ;
}
/*
*功能:根据类型选择日志文件
*@category :in 类别
*return
*说明:调用Dao之前必需先选择日志文件
*/
boolean setLogFile(char *category){
	if(!strcmp(category,"loginLog")){
		syslogFilePath = loginLogFilePath;

	}else if(!strcmp(category,"setupLog")){
		syslogFilePath = setupLogFilePath;

	}else{
		webLog("file =%s;func=%s;msg=there is no such category %s;",__FILE__,__FUNCTION__,category);
		return false;
	}
	return true;
}


/*
 * 功能，保存日志
 * @ syslog :in
 * 保存成功,返回值,反之返回假
 */
boolean saveSyslogDao(struct Syslog *syslog){
	FILE *syslogfile = NULL;
	lock_t lock = lck_open(syslogFilePath,"a",&syslogfile);
	//lock file error
	if(lock < 0 ){	//this file is already lock by other process
		webLog("file=%s;func=%s;msg=open file %s error;",__FILE__,__FUNCTION__,syslogFilePath);
		return false  ;
	}
	//设置记录内容
	fprintf(syslogfile,
			"%ld %s %ld %s %s %s\n",
			syslog->logId,
			syslog->username,
			syslog->createTime,
			syslog->isSuccess,
			syslog->action,
			syslog->loginIp);
	//关闭文件
	lck_close(lock,syslogfile);
	return true;
}
/*
*功能:删除日志
*@ logIds :in 日志的ID集
*操作成功,返回真,反之返回假
*注:根据需求,此函数已不再使用
*/
boolean deleteSyslogsDao(char *logIds){

	return true;
}


/*
*功能：根据数据结构中设置的数据作为条件查询符合条件的日志
*@condition :in　   查询条件
*@page      :in out 分页信息
*@tr        :in		时间范围
*@list      :out	输出结果
*查询成功返回真,否则返回假
*/
boolean querySyslogsDao(struct Syslog *condition,struct Page *page,struct TimeRange *tr,struct Syslogs **list){
	int ret = 0;
	char username[40];				//用户名
	struct Syslogs *syslogs;		//日志链表
	struct Syslog *syslog ;			//日志
	struct Syslog tempLog ;
	char record[300];
	
	/*
	*用户名条件查询
	*用户名后三位以"##0"结束,表示非模糊查询
	*用户名后三位以"##1"结束,表示模糊查询
	*/
	int length = strlen(condition->username);//username length
	boolean isUsernameMix;//用户名是否为模糊搜索条件标志
	if(condition->username[0] != 0){
		//非模糊搜索
		if(condition->username[length - 1] == '0'){
			isUsernameMix = false;
		}
		//模糊搜索
		else{
			isUsernameMix = true;
		}
		//去掉是模块搜索标志
		memcpy(username,condition->username,length - 3);
		username[length-3]='\0';
	}
	/*
	*为提高效率,修改为使用管程通讯 
	*
	*/
	char command[100];				//命令
	FILE *syslogfile = NULL; 
	
	//设置命令内容
	sprintf(command,"sed -e '1!G;h;$!d' %s",syslogFilePath);
	if((syslogfile = popen(command,"r")) == NULL){	//error 
		webLog("file=%s;func=%s;msg=open pipe %s error;",__FILE__,__FUNCTION__,syslogFilePath);
		return false; 
	}
	
	//创建日志链表的头指针
	struct Syslogs *p = (struct Syslogs *)malloc(sizeof(struct Syslogs));

	//初始化输出链表
	*list = p;
	p->syslog = NULL;
	p->next = NULL;
	//读取文件
	while(fgets(record,300,syslogfile)){
		//对记录进行校验, 
		if(!checkTableRecordValid(record,6)){
			continue;//跳过错误数据 
		}
		ret = sscanf(record,"%ld %s %ld %s %s %s",
						&tempLog.logId,
						&tempLog.username,
						&tempLog.createTime,
						&tempLog.isSuccess,
						&tempLog.action,
						&tempLog.loginIp);
		if(ret < 0){
			break;
		}
		/*
		*对各个条件进行逐一匹配,一旦有不符合条件
		*的就跳出本次循环,进入下轮循环
		*/
		
		//时间段范围
		if(tr->from != 0 || tr->to != 0){
			//只有时间段开始时间
			if(tr->from != 0 && tr->to == 0){
				if(tempLog.createTime < tr->from){
					continue;
				}
			}
			//只有时间段结束时间
			if(tr->from == 0 && tr->to != 0){
				if( tempLog.createTime > tr->to){
					continue;
				}
			}
			//时间段开始时间和结束时间均有
			if(tr->from != 0 && tr->to != 0) {
				if( tempLog.createTime < tr->from || tempLog.createTime > tr->to ){
					continue;
				}
			}

		}
		//日志ID
		if(condition->logId != 0 && condition->logId !=tempLog.logId){
			continue;
		}
		//用户名
		if(condition->username[0] != 0){
			//模糊搜索
			if(isUsernameMix){
				if(strstr(tempLog.username,username) == NULL){
					continue;
				}
			}
			//非模糊搜索
			else{
				if(strcmp(tempLog.username,username)){
					continue;
				}
			}
		}
		/*
		*0表示失败
		*1表示成功
		*2表示所有
		*当为2时,所有记录均符合这一要求,无须进行比较
		*/
		//操作结果
		if(condition->isSuccess[0] != 0){
			//不为2时
			if(strcmp(condition->isSuccess,"2")){
				if(strcmp(tempLog.isSuccess,condition->isSuccess)){
					continue;
				}
			}

		}
		//动作
		if(condition->action[0] != 0){
			if(strcmp(tempLog.action,condition->action)){
				continue;
			}
		}

		//登录IP
		if(condition->loginIp[0] != 0){
			if(strcmp(tempLog.loginIp,condition->loginIp)){
				continue;
			}
		}


		/*
		*分页条件
		*该记录还必须满足分页要求,即取第几条开始,共多少条记录
		*/
		if(page != NULL){
			++(page->total);//符合条件的记录计数自增
			//如果该记录不在分页要求的范围内,则转入下一轮循环
			if(page->total < page->start || page->total > page->end){
				continue;
			}
		}
		/*
		*已经符合所有条件,将记录添加到查询结果链表中
		*
		*/

		//为存放当前记录分配链表结点空间
		syslogs = (struct Syslogs *)malloc(sizeof(struct Syslogs));
		//将新结点加入链表中
		p->next = syslogs;
		//分配记录信息空间
		syslog = (struct Syslog*)malloc(sizeof(struct Syslog));
		//重置记录信息空间
		resetSyslog(syslog);
		/*
		*将记录的各字段值相应赋给告警数据结构的成员变量
		*/
		memcpy(syslog,&tempLog,sizeof(struct Syslog));
		//将该日志赋于当前结点的日志指针
		syslogs->syslog=syslog;
		//将当前结点的下一结点设置为空
		syslogs->next = NULL;
		//将p指针指向当前结点
		p = syslogs;
	}
	//关闭文件
	pclose(syslogfile);
	return true;
}
