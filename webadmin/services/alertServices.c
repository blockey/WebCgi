/*
*alertServices.c
*告警服务接口
*/
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../base/base.h"
#include "alertServices.h"
#include "../dao/alertDao.h"
#include "../util/alertUtil.h"
#include "../util/pageUtil.h"
#include "../model/alertModel.h"
#include "../util/sysvar.h"
#include "../util/commonUtil.h"

boolean queryAlerts(char *params,char *actName,char *result){
	struct Alert alert ;		//告警	
	char value[1000];			//中间变量 
	char temp[300];				//临时变量 
	struct Alerts *alerts;		//告警信息链表
	struct Alerts *p;			//告警信息变量 	
	struct Alert *rAlert;		//返回链表中的告警信息
	struct Page page;			//分页
	//重围告警空间
	resetAlert(&alert);	
	//根据字符串参数设置User
	setAlertByString(&alert,params);
	//重置分页空间
	resetPage(&page);	
	//根据字符串参数设置Page
	setPageByString(&page,params);
	//计算分页数据
	calculatePage(&page);
	struct TimeRange tr;		//查询条件时间段范围
	//根据字符串参数设置TimeRange
	setTimeRangByString(&tr,params);
	//告警状态
	
	//调用DAO层进行查询
	if(!strcmp(actName,"queryAllAlerts")){
		
		if(!queryAllAlertsDao(&alert,&page,&tr,&alerts)){
			
			return false ;
		}
	}else if(!strcmp(actName,"queryCurrentAlerts")){
		
		if(!queryCurrentAlertsDao(&alert,&page,&tr,&alerts)){
			
			return false ;
		}
	}
	char list[4096] = "";		//查询列表
	//初始化
	strcpy(result,"");
	//设置返回信息
	sprintf(value,"actName=%s;result=true;",actName);
	strcat(result,value);
	//设置分页信息
	sprintf(value,"current=%d;pageSize=%d;total=%d;",page.current,page.pageSize,page.total);
	strcat(result,value);
	//设置告警记录
	strcat(result,"list=");
	p = alerts;
	//无符合条件的记录
	if(alerts->next == NULL){
		strcat(list,"none;");
		strcat(result,list);
		//释放链表内存
		releaseUsersMemory(alerts);
		return true;
	}
	
	//有符合条件的记录
	while(alerts->next){
		//取出链表下一结点
		alerts = alerts->next;
		if(alerts == NULL){
			break;
		}
		//取出当前结点的内容记录
		rAlert = alerts->alert;
		/*
		*设置返回数据
		*/
		//创建时间
		if(getValueByName(params,"begintime",value)){
			if(rAlert->begintime == 0){
					strcpy(value,"begintime_0,");	//表示该项无值
			}else{
		//转换成标准时间数据
				long2TimeStr(rAlert->begintime,temp);
				sprintf(value,"begintime_%s,",temp);
			}
			strcat(list,value);
		}
		//恢复时间
		if(getValueByName(params,"endtime",value)){
			if(rAlert->endtime == 0){
				strcpy(value,"endtime_0,");    //表示该项无值
			}else{
				//转换成标准时间数据
				long2TimeStr(rAlert->endtime,temp);
				sprintf(value,"endtime_%s,",temp);
			}
			strcat(list,value);
		}
		//告警级别
		if(getValueByName(params,"value",value)){
			if(rAlert->value == 0){
				sprintf(value,"value_一般告警,",temp);
			}else{
				sprintf(value,"value_严重告警,",temp);
			}
			strcat(list,value);
		}
		//告警信息
		if(getValueByName(params,"alertInfo",value)){
			getAlertDesc(rAlert,temp);
			sprintf(value,"alertInfo_%s,",temp);
			strcat(list,value);
		}
		//使用#来分隔每一条记录
		strcat(list,"#");
	}
	//使用;分隔每一项内容
	strcat(list,";");
	strcat(result,list);
	//释放链表内存
	releaseAlertsMemory(p);
	return true;

}

/*
*功能:告警信息服务
*@params  :in 请求参数
*@actName :in 请求动作
*@result  :out 输出结果
*操作成功,返回值,反之返回假
*/
boolean alertServices(char *params,char *actName,char *result){
	/*
	*根据请求参数的动作中调用相应实现 
	*
	*/
	if(!strcmp(actName,"deleteAllAlerts")){
		return deleteAllAlertsDao();
	}
	//查询当前告警信息
	else if(!strcmp(actName,"queryCurrentAlerts") ){
		return queryAlerts(params,actName,result);
	}
	//查询所有告警信息
	else if(!strcmp(actName,"queryAllAlerts") ){
		
		return queryAlerts(params,actName,result);
	}

	webLog("file=%s;func=%s;msg=no handling procedure for the actName %s;",__FILE__,__FUNCTION__,actName);
	return false;
}
