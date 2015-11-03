
/*
*snmpSetItemServices.c
*根据不同的请求参数调用不同的处理
*/

#include "snmpSetItemServices.h"
#include "snmpSetItemUtil.h"
#include "../base/base.h"

/*
*功能:获取要设置到设备的参数,包括 itemIndent,typeIndent ,itemValue
*@itemNames :in
*@itemValues :in
*@parameters :out
*return,if success return true,else return false 
*/
boolean getDeviceSetParameters(char *itemNames,char *itemValues,char *parameters){
	boolean ret = false;
	int i = 0;
	char cmd[100];
	/* Trap服务器 */
	if(strstr(itemNames,"trapServer")){
		return getTrapServerSetParameters(itemNames,itemValues,parameters);
	}
	//设置卡类形
	else if(strstr(itemNames,"saveSingleDeviceCardType")){
		return getSingleCardTypeSetParameters(itemNames,itemValues,parameters);	
	}
	//设置卡使用状态
	else if(strstr(itemNames,"singleDeviceCardUse")){
		return getSingleCardUseSetParameters(itemNames,itemValues,parameters);	
	}	
	//设置卡的其他属性
	else if(strstr(itemNames,"cardConfInfo")){
		return cardConfInfoSetParameters(itemNames,itemValues,parameters);	
	}
	//设置卡的远端信息
	else if(strstr(itemNames,"singleDeviceCardRemote")){
		return singleDeviceCardRemoteSetParameters(itemNames,itemValues,parameters);	
	}
	//设置卡的远端配置信息
	else if(strstr(itemNames,"remoteCardConfigInfo")){
		return remoteCardConfigInfoSetParameters(itemNames,itemValues,parameters);	
	}
	//设置卡的近端扩展配置信息
	else if(strstr(itemNames,"singleDeviceCardExt")){
		return singleDeviceCardExtSetParameters(itemNames,itemValues,parameters);	
	}
	//批量设置卡的远端地址信息
	else if(strstr(itemNames,"devicesCardRemote")){
		return devicesCardRemoteSetParameters(itemNames,itemValues,parameters);	
	}
	//批量设置卡的类型
	else if(strstr(itemNames,"devicesCardType")){
		return devicesCardTypeSetParameters(itemNames,itemValues,parameters);	
	}
	//批量设置卡的使用状态
	else if(strstr(itemNames,"devicesCardUse")){
		return devicesCardUseSetParameters(itemNames,itemValues,parameters);	
	}
	//设置机架的电源配置
	else if(strstr(itemNames,"cardPowerConfig")){
		return cardPowerConfigSetParameters(itemNames,itemValues,parameters);	
	}
	//设置snmp代理配置参数 
	else if(strstr(itemNames,"refleshAgentCfg")){
				/* 重启snmp代理 */
		//modify by zhaofeihua ,不重启代理 
		//shut down 
//		system("kill `ps |grep 's_agent' |awk '{print $1}'`");
		//sleep for 1 seconds 
//		sleep(1);
		//change dir 
//		chdir("/app/bin");
		//exec 
//		system("./s_agent &");
		
		if(refleshAgentCfgSetParameters(itemNames,itemValues,parameters)){
			//sleep(15);
			//system("/app/bin/checkArgs &");
			return true; 
		}else {
			return false; 
		}
		
	}
	//设置网络参数地址
	else if(strstr(itemNames,"netIpAddress")){
		ret = get_netSetup_Parameters(itemNames,itemValues,parameters);
		//设置成功
		if(ret){
			//将会话文件清空 
			sprintf(cmd,"cp -f %s %s",blankFilePath,sessionFilePath);
			system(cmd);
		}
		return ret;
	}
	//对网络设置进行转换
	else {
		return get_netSetup_Parameters(itemNames,itemValues,parameters);
	}
	

}