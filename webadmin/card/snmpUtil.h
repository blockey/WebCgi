/*
 * snmpUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPUTIL_H_
#define SNMPUTIL_H_
#include "../base/base.h"
/*
*功能:执行SNNPGET命令,提取原始数据
*@itemNames		:in 硬件参数名称
*@proRresult	:out 执行命令后结果
*return 成功,return true,else return false
*/
boolean snmpGetSrcData(char *itemNames,char *srcData);
/*
*功能:执行SNNPGET命令,提取转换后返回给用户端的数据
*@itemNames		:in		硬件参数名称
*@proRresult	:out	执行命令后结果
*return 成功,return true,else return false
*/
boolean snmpGetFinalData(char *itemNames,char *finalData);
/*
*功能:获取光口和电口的告警信息
*@itemNames :in		参数名称
*@data		:out	输出结果	
*return 成功,return true,else return false
*/
boolean getFTAlertData(char *itemNames,char *data);
/*
*功能:板卡统计
*@itemName	:in		请求参数名称
*@datas		:out	输出结果
*return ,success return true,else return false
*/
boolean getCardStatisticsData(char *itemName,char *datas);
/*
*功能:WEB版本信息
*@itemName	:in		请求参数名称
*@datas		:out	输出结果
*return ,success return true,else return false
*/
boolean getWebVersionData(char *itemName,char *datas);

#endif /* SNMPUTIL_H_ */