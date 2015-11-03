/*
 * snmpGetServices.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPGETSERVICES_H_
#define SNMPGETSERVICES_H_
#include "../base/base.h"
/*
*功能:读取设备经处理,符合返回到页面的参数
*@itemNames :in 要读取的参数项集
*@parameters :out 从设备读出的参数
*return ,if success ,return true,else return false
*/
boolean getDeviceParametersService(char *itemNames,char *parameters);
/*
*功能:读取设备经处理,返回从设备读出的原始参数
*@itemNames :in 要读取的参数项集
*@parameters :out 从设备读出的原始参数
*return ,if success ,return true,else return false
*/
boolean getDeviceSrcParameterService(char *itemNames,char *srcParameters);
/*
*功能:读取设备经处理,符合返回到页面的参数
*@itemNames :in 要读取的参数项集
*@parameters :out 从设备读出的参数
*return ,if success ,return true,else return false
*/
boolean getSysInfoService(char *itemNames,char *parameters);
#endif /* SNMPGETSERVICES_H_ */