/*
*authorityServices.c
*权限服务接口
*
*/
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../base/base.h"
#include "userServices.h"
#include "authorityServices.h"
#include "../model/userModel.h"
#include "../util/userUtil.h"
/*
*功能:验证用户进行操作是否具备相应权限
*@actName :in
*@username :in
*return ,具备权限 return true ,else return false
*/
boolean isAuthorityValid(char *actName,char *username){
	struct User user;		//用户
	//重置用户空间
	resetUser(&user);
	//设置用户名
	strcpy(user.username,username);
	//获取该用户信息
	getUserService(&user);
	
	/*
	*用户管理 只有系统管理员有此权限
	*/
	//添加用户
	if(!strcmp(actName,"saveUser")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//检查用户名是否已经存在 
	if(!strcmp(actName,"checkUsername")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//注销用户
	else if(!strcmp(actName,"deleteUsers")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//激活用户
	else if(!strcmp(actName,"activeUsers")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//更新用户
	else if(!strcmp(actName,"updateUser")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}	
	//查询用户
	else if(!strcmp(actName,"queryUsers")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//查看用户信息
	else if(!strcmp(actName,"getUserInfo")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//清空告警,来宾用户无权限 
	else if(!strcmp(actName,"deleteAllAlerts")){
		if(!strcmp(user.category,"3")){
			return false;
		}
	}
	//导入snmp配置文件 ,只有系统管理员有权限 
	else if(!strcmp(actName,"uploadSnmpCfg")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//导入snmp配置文件 ,只有系统管理员有权限 
	else if(!strcmp(actName,"uploadUpdatePkg")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//导入snmp配置文件 ,只有系统管理员有权限 
	else if(!strcmp(actName,"getProgress")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//导出snmp配置文件 ,只有系统管理员有权限 
	else if(!strcmp(actName,"downloadSnmpCfg")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//启用新的snmp代理配置文件 ,只有系统管理员有权限 
	else if(!strcmp(actName,"refleshAgentCfg")){
		if(strcmp(user.category,"1")){
			return false;
		}
	}
	//个人信息查看与修改 所有用户都有权限
	else if(!strcmp(actName,"updatePersonalInfo")){
		return true;
	}
	else if(!strcmp(actName,"getUserPersonalInfo")){
		return true;
	}
	///////////////////////////////////////////////////////
	//对设备进行操作 只有来宾帐户没权限
	else{
		if(!strcmp(user.category,"3")){
			return false;
		}
	}
	
	return true;
}