/*
 * snmpGetItemUtil.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPGETITEMUTIL_H_
#define SNMPGETITEMUTIL_H_
#include "../base/base.h"

/*
*功能:对ucDeviceLinkStatus数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void deviceLinkStatus_DTF(char *itemName,char *data);
/*
*功能: 获取系统重启状态
*@ itemName :in
*@ data :in out
*return 
*/
void refleshStatus_DTF(char *itemName,char *data);
/*
*功能:对trapServer类数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void trapServer_DTF(char *itemName,char *data);
/*
*功能:对snmpUpdateDate数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void snmpUpdateDate_DTF(char *itemName,char *data);
/*
*功能:对getDeviceTime数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void getDeviceTime_DTF(char *itemName,char *data);
/*
*功能:对readSingleDeviceCardType数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void readSingleDeviceCardType_DTF(char *itemName,char *data);
/*
*功能:对saveSingleDeviceCardType数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void saveSingleDeviceCardType_DTF(char *itemName,char *data);
/*
*功能:对devicesCardType数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void devicesCardType_DTF(char *itemName,char *data);

/*
*功能:对cardPowerConfig数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void cardPowerConfig_DTF(char *itemName,char *data);
/*
*功能:对singleDeviceCardRemoteConnStatus数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardRemoteConnStatus_DTF(char *itemName,char *data);
/*
*功能:对singleDeviceCardUse数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardUse_DTF(char *itemName,char *data);
/*
*功能:对devicesCardUse数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void devicesCardUse_DTF(char *itemName,char *data);
/*
*功能:对singleCardVersion数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void singleCardVersion_DTF(char *itemName,char *data);
/*
*功能:对singleDeviceCardExt数据转换,只有IP113S有扩展信息
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardExt_DTF(char *itemName,char *data);
/*
*功能:对联级设备卡人其他属性进行数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void cardConfInfo_DTF(char *itemName,char *data);
/*
*功能:对remoteCardConfigInfo数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void remoteCardConfigInfo_DTF(char *itemName,char *data);
/*
*功能:对cardStatusInfo数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void cardStatusInfo_DTF(char *itemName,char *data);
/*
*功能:对devicesCardRemote数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void devicesCardRemote_DTF(char *itemName,char *data);
/*
*功能:对singleDeviceCardRemote数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void singleDeviceCardRemote_DTF(char *itemName,char *data);
/*
*功能:板卡统计
*@itemName :in
*@finalData :out
*return ,success return true,else return false
*/
boolean cardStatistics_DTF(char *itemName,char *finalData);
/*
*功能:对联级设备卡人其他属性进行数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void cardStatusCfg_DTF(char *itemName,char *data);
/*
*功能:对statistics_cardTypes数据转换
*@ itemName :in
*@ data :in out
*return 
*/
void statistics_cardTypes_DTF(char *itemName,char *data);
/*
*功能:通用数据转换
*@ itemName :in
*@ itemType :in
*@ data :in out
*return 
*/
void Genal_DTF(char *itemName,char *itemType ,char *data);


#endif /* SNMPGETITEMUTIL_H_ */