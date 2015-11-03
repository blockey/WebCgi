/*
 * snmpSetUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPSETUTIL_H_
#define SNMPSETUTIL_H_
#include "../base/base.h"
/*
*功能:执行SNNPSET命令
*@itemName		:in		硬件参数名
*@itemValue		:in		要设置的硬件参数值
*@proRresult	:out	执行命令后命令输出结果
*return 成功,return true,else return false
*/
boolean setDeviceParametersUtil(char *itemNames,char *itemValues,char *parameters);
#endif /* SNMPSETUTIL_H_ */