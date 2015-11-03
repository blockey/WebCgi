/*
 * snmpSetServices.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPSETSERVICES_H_
#define SNMPSETSERVICES_H_
#include "../base/base.h"
/*
*功能:设置设备参数
*@ itemNames	:in	硬件参数名
*@ itemValues	:in	要设置的硬件参数值
*@ parameters	:out设置后的硬件参数值
*if success return true, else return false;
*/
boolean setDeviceParametersService(char *itemNames,char *itemValues,char *parameters);
#endif /* SNMPSETSERVICES_H_ */