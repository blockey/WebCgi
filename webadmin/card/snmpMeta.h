/*
 * snmpMeta.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPMETA_H_
#define SNMPMETA_H_
#include "../base/base.h"

/*
*功能:提取命令执行结果
*@ str :out 结果
*return 执行成功 返回true 
*/
boolean getCommandSrcData(char *itemNames,char *str);
/*
*功能:提取命令执行结果
*@ str :out 结果
*return 执行成功 返回true 
*/
boolean getCommandFinalResult(char *itemNames,char *str);

#endif /* SNMPMETA_H_ */