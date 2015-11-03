/*
 * snmpSetItemServices.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPSETITEMSERVICES_H_
#define SNMPSETITEMSERVICES_H_
#include "../base/base.h"

/*
*功能:获取要设置到设备的参数,包括 itemIndent,typeIndent ,itemValue
*@itemNames :in
*@itemValues :in
*@parameters :out
*return,if success return true,else return false 
*/
boolean getDeviceSetParameters(char *itemNames,char *itemValues,char *parameters);

#endif /* SNMPSETITEMSERVICES_H_ */