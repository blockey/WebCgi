/*
*sysvar.c
*系统变量,类数据库查询
*/
#include <string.h>
#include <stdio.h>
#include "../base/base.h"
#include "sysvar.h"
#include "../util/adstring.h"

/*
*功能:根据actName获取该动作的描述
*@actName
*@desc
*success return true,else return false
*/
boolean getActionDesc(char *actName,char *desc){
	////////////////////////////////////////////////
	//登录 退出
	if(!strcmp(actName,"login")){
		strcpy(desc,"登录");
		return true;
	}
	if(!strcmp(actName,"loginOut")){
		strcpy(desc,"退出");
		return true;
	}
	///////////////////////////////////////////////
	//用户管理
	if(!strcmp(actName,"saveUser")){
		strcpy(desc,"添加新用户");
		return true;
	}
	if(!strcmp(actName,"deleteUsers")){
		strcpy(desc,"注消用户");
		return true;
	}
	if(!strcmp(actName,"activeUsers")){
		strcpy(desc,"激活用户");
		return true;
	}
	if(!strcmp(actName,"updateUser")){
		strcpy(desc,"更新用户");
		return true;
	}
	if(!strcmp(actName,"updatePersonalInfo")){
		strcpy(desc,"更新个人资料");
		return true;
	}
	///////////////////////////////////////////////
	//告警管理
	if(!strcmp(actName,"deleteAllAlerts")){
		strcpy(desc,"清空历史告警");
		return true;
	}
	////////////////////////////////////////////////
	//设备管理	
	if(!strcmp(actName,"updateSystemTime")){
		strcpy(desc,"更新系统时间");
		return true;
	}
	if(!strcmp(actName,"updateNetParam")){
		strcpy(desc,"更新网络参数");
		return true;
	}
	if(!strcmp(actName,"updateTrapServer")){
		strcpy(desc,"更新Trap服务器参数");
		return true;
	}
	if(!strcmp(actName,"updateSNMP")){
		strcpy(desc,"更新社区信息");
		return true;
	}
	if(!strcmp(actName,"deleteAlerts")){
		strcpy(desc,"删除告警信息");
		return true;
	}
	if(!strcmp(actName,"refleshAgentCfg")){
		strcpy(desc,"启用最新设备参数");
		return true;
	}
	if(!strcmp(actName,"uploadSnmpCfg")){
		strcpy(desc,"导入机架及板卡参数配置文件");
		return true;
	}
	if(!strcmp(actName,"downloadSnmpCfg")){
		strcpy(desc,"导出机架及板卡参数配置文件");
		return true;
	}
	////////////////////////////////////////////////////
	//板卡设置
	//板卡类型
	if(!strcmp(actName,"updateCardType")){
		strcpy(desc,"更新板卡类型");
		return true;
	}
	//板卡使用状态
	if(!strcmp(actName,"updateCardUse")){
		strcpy(desc,"更新板卡使用状态");
		return true;
	}
	//板卡其他属性
	if(!strcmp(actName,"updateConfInfo")){
		strcpy(desc,"更新板卡其他属性");
		return true;
	}
	//远端信息
	if(!strcmp(actName,"updateCardRemote")){
		strcpy(desc,"更新板卡远端信息");
		return true;
	}
	//远端配置信息
	if(!strcmp(actName,"updateRemoteConfig")){
		strcpy(desc,"更新板卡远端其他属性");
		return true;
	}
	//近端扩展配置信息
	if(!strcmp(actName,"updateCardExt")){
		strcpy(desc,"更新板卡近端扩展配置信息");
		return true;
	}
	//批量远端信息
	if(!strcmp(actName,"updateRemoteAddrs")){
		strcpy(desc,"批量更新远端地址");
		return true;
	}
	//批量板卡类型
	if(!strcmp(actName,"updateCardTypes")){
		strcpy(desc,"批量更新板卡类型");
		return true;
	}
	//批量板卡使用状态
	if(!strcmp(actName,"updateCardUses")){
		strcpy(desc,"批量更新板卡使用状态");
		return true;
	}
	//机架电源配置
	if(!strcmp(actName,"updatePowerConfig")){
		strcpy(desc,"更新机架电源配置");
		return true;
	}
	strcpy(desc,"未知操作");
	webLog("file=%s;func=%s;msg=no action for actname %s;",__FILE__,__FUNCTION__,actName);
	return false;
	
}
/*
*功能:获取用户权限描述
*@category	:in
*@desc		:out
*return 
*/
void getCategoryDesc(char *category,char *desc){
	if(!strcmp(category,"1")){
		strcpy(desc,"系统管理员");
	}
	else if(!strcmp(category,"2")){
		strcpy(desc,"操作员");
	}
	else{
		strcpy(desc,"来宾帐户");
	}
}
