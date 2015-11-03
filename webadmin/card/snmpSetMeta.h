/*
 * snmpSetMeta.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPSETMETA_H_
#define SNMPSETMETA_H_

/*
*功能:根据机架和设备卡位置获取卡类型
*@devId		:in		机架编号 从0开始
*@cardId	:in		板卡编号 从1开始
*@cardType	:out	板卡类型 输出
*return
*/
void getCardTypeByPosition(int devId,int cardId,char *cardType);
/*
*功能:获取启用参数状态 
*正在启用,返回true,启用完成,返回false
*/
int getRefleshStatus();
#endif /* SNMPSETMETA_H_ */



