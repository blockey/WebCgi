/*
 * snmpSetItemMeta.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPSETITEMMETA_H_
#define SNMPSETITEMMETA_H_
#include "../base/base.h"
/*
*功能:根据itemName和itemValue处理出SingleCardType请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean getSingleCardTypeSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出SingleCardUse请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean getSingleCardUseSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出singleDeviceCardRemote请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean singleDeviceCardRemoteSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出devicesCardRemote请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean devicesCardRemoteSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出devicesCardType请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean devicesCardTypeSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出devicesCardUse请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean devicesCardUseSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出cardPowerConfig请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false 
*/
boolean cardPowerConfigSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出remoteCardConfigInfo请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false
*说明:只有F3和IP113S两种板卡有远端配置信息
*/
boolean remoteCardConfigInfoSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出singleDeviceCardExt请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false
*说明:只有IP113S板卡有近端扩展信息
*/
boolean singleDeviceCardExtSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出getCardConfInfo请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false
*/
boolean getCardConfInfoSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:根据itemName和itemValue处理出getTrapServer请求所需要的值
*@ itemName		:in		硬件参数名
*@ itemValue	:in		要设置的硬件参数值
*@ saveData		:out	输出要保存到硬件的结果
*return , if success return true,else return false
*/
boolean getTrapServerSetData(char *itemName,char *itemValue,char *saveData);
/*
*功能:Trap地址和端口进行处理,获取16进制字符串
*@ addr			:in
*@ itemValue	:out
* return 
*/
void trap2hexstr(NETWORK_TRAP_ADDR *addr,char *itemValue);
#endif /* SNMPSETITEMMETA_H_ */
