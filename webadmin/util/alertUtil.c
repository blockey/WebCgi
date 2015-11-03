/*
*alertUtil.c
*告警信息工具
*/
#include <stdlib.h>
#include "../model/alertModel.h"
#include "alertUtil.h"
#include "../base/base.h"
#include "adstring.h"
/*
*功能：重置告警数据
*@ user :in out
*return
*/
void resetAlert(struct Alert *alert){
	//告警类型
	alert->Specific =0;
	//机架
	alert->address = 0;
	//槽位
	alert->slot=0;
	//远近端
	alert->flag= 0;
	//端口
	alert->port=0;
	//创建时间
	alert->begintime=0;
	//恢复时间
	alert->endtime=0;
	//告警级别
	alert->value=0;
	
}
/*
*功能:根据请求参数设置告警信息
*@user	:out
*@str	:in 字符串参数
*return 
*/
void setAlertByString(struct Alert *alert,char *str){
	
	char value[300];
	//告警类型
	if(getValueByName(str,"Specific",value)){
		alert->Specific = atol(value);
	}
	//机架
	if(getValueByName(str,"address",value)){
		alert->address = atol(value);
	}
	//槽位
	if(getValueByName(str,"slot",value)){
		alert->slot = atol(value);
	}
	//远近端
	if(getValueByName(str,"flag",value)){
		alert->flag = atol(value);
	}
	//端口
	if(getValueByName(str,"port",value)){
		alert->port = atol(value);
	}
	//创建时间
	if(getValueByName(str,"begintime",value)){
		alert->begintime = atol(value);
	}
	//恢复时间
	if(getValueByName(str,"endtime",value)){
		alert->endtime = atol(value);
	}
	//告警级别
	if(getValueByName(str,"value",value)){
		alert->value = atol(value);
	}
	
}
/*
*功能：释放链表内存
*users :in out
*return 
*/
void releaseAlertsMemory(struct Alerts *alerts){
	struct Alerts *p;
	while(alerts){
		free(alerts->alert);
		p = alerts;
		alerts = alerts->next;
		free(p);
	}
}
/*
*功能:根据告警类型设备相应描述
*alert
*@desc
*success return true,else return false
*/
boolean getAlertDesc(struct Alert *alert,char *desc){	
	char point[10];		//远近端
	//设备电源关闭
	if(alert->Specific == 1){			
		sprintf(desc,"机架%d电源%d关闭",alert->address+1,alert->port+1);
	}
	//电源电压告警上限
	else if(alert->Specific == 2){
		sprintf(desc,"机架%d电源%d电压高于上限",alert->address+1,alert->port+1);
	}
	//电源电压告警下限
	else if(alert->Specific == 3){
		sprintf(desc,"机架%d电源%d电压低于下限",alert->address+1,alert->port+1);
	}
	//环境温度告警上限
	else if(alert->Specific == 4){
		sprintf(desc,"机架%d环境温度高于上限",alert->address+1);
	}
	//环境温度告警下限
	else if(alert->Specific == 5){
		sprintf(desc,"机架%d环境温度低于下限",alert->address+1);
	}
	//环境湿度告警上限
	else if(alert->Specific == 6){
		sprintf(desc,"机架%d环境湿度高于上限",alert->address+1);
	}
	//环境湿度告警下限
	else if(alert->Specific == 7){
		sprintf(desc,"机架%d环境湿度低于下限",alert->address+1);
	}
	//风扇转速告警上限
	else if(alert->Specific == 8){
		sprintf(desc,"机架%d风扇%d转速高于上限",alert->address+1,alert->port+1);
	}
	//风扇转速告警下限
	else if(alert->Specific == 9){
		sprintf(desc,"机架%d风扇%d转速低于下限",alert->address+1,alert->port+1);
	}
	//拔出设备卡告警
	else if(alert->Specific == 10){
		if(alert->flag == 0){
			strcpy(point,"近端");
		}else{
			strcpy(point,"远端");
		}
		sprintf(desc,"拔出机架%d的槽位%d中的设备卡(%s)",alert->address+1,alert->slot+1,point);
	}
	//插入设备卡告警
	else if(alert->Specific == 11){
		if(alert->flag == 0){
			strcpy(point,"近端");
		}else{
			strcpy(point,"远端");
		}
		sprintf(desc,"插入机架%d的槽位%d中的设备卡(%s)",alert->address+1,alert->slot+1,point);
	}
	//设备电口告警
	else if(alert->Specific == 12){
		if(alert->flag == 0){
			strcpy(point,"近端");
		}else{
			strcpy(point,"远端");
		}
		sprintf(desc,"机架%d的槽位%d中的电口告警(%s)",alert->address+1,alert->slot+1,point);
	}
	//设备光口告警
	else if(alert->Specific == 13){
		if(alert->flag == 0){
			strcpy(point,"近端");
		}else{
			strcpy(point,"远端");
		}
		sprintf(desc,"机架%d的槽位%d中的光口告警(%s)",alert->address+1,alert->slot+1,point);
		
	}
	//拔出联级设备告警
	else if(alert->Specific == 14){
		sprintf(desc,"拔出第%d级级联设备",alert->address+1);
	}
	//插入联级设备告警
	else if(alert->Specific == 15){
		sprintf(desc,"插入第%d级级联设备",alert->address+1);
	}
	//插入联级设备告警
	else if(alert->Specific == 16){
		if(alert->flag == 0){
			strcpy(point,"近端");
		}else{
			strcpy(point,"远端");
		}
		sprintf(desc,"机架%d的槽位%d的设备掉电告警(%s)",alert->address+1,alert->slot+1,point);
	}
	//其他可能的新告警
	else{
		strcpy(desc,"未知告警");
		webLog("file=%s;func=%s;msg=can't not find the specific %d;",__FILE__,__FUNCTION__,alert->Specific);
		return false;
	}
	if(alert->endtime !=0){
		strcat(desc,"(已恢复)");
	}	
	return true;
}




