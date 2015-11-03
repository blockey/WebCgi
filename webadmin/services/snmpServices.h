/*
 * snmpServices.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPSERVICES_H_
#define SNMPSERVICES_H_
#include "../base/base.h"
/*
*功能:读取设备参数
*@itemNames :in 要读取的参数项集
*@parameters :out 从设备读出的参数
*return ,if success ,return true,else return false
*/
boolean getDeviceParameters(char *itemNames,char *parameters);
/*
*功能:设置设备参数
*@itemNames :in 设置的参数名集
*@itemValues :in 设置的参数值集
*@parameters :out 设置后从设备读出的参数
*return ,if success ,return true,else return false
*/
boolean setDeviceParameters(char *itemNames,char *itemValues,char *parameters);
#endif /* SNMPSERVICES_H_ */
