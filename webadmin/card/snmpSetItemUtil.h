/*
 * snmpSetItemUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPSETITEMUTIL_H_
#define SNMPSETITEMUTIL_H_
#include "../base/base.h"
/*
*功能:获取Trap服务器的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean getTrapServerSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡类型的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean getSingleCardTypeSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡使用状态的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean getSingleCardUseSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡使用状态的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean devicesCardUseSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取机架的电源配置设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean cardPowerConfigSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡其他参数的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean cardConfInfoSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡其他参数的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean devicesCardRemoteSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡其他参数的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean devicesCardTypeSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡远端信息的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean singleDeviceCardRemoteSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡近端扩展信息设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean singleDeviceCardExtSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取设备卡远端信息的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean remoteCardConfigInfoSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:设置snmp代理配置参数 
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean refleshAgentCfgSetParameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取一般网络设置的设置参数
*@itemNames		:in		硬件参数名
*@itemValues	:in		要设置的硬件参数值
*@parameters	:out	输出可以设置到硬件的十六进制数据
*return ,if success return true,else return false;
*/
boolean get_netSetup_Parameters(char *itemNames,char *itemValues,char *parameters);
/*
*功能:获取MIB标识符
*@ itemName :in
*@ itemIndent :out
*@ typeIndent :out
*if success ,return true,else return false
*/
boolean getIndentifies(char *itemName,char *itemIndent,char *typeIndent);


#endif /* SNMPSETITEMUTIL_H_ */
