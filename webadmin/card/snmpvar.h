/*
 * SNMPVAR.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPVAR_H_
#define SNMPVAR_H_
#include "../base/base.h"
#define GLOBAL_MAX_CARDNUM 16
//无卡
#define BD_CARD_TYPE_NULL	0xFF		//无卡
//电源
#define BD_CARD_TYPE_POWER	0xA0	//电源卡

//收发器卡类型
//--I/O卡
#define BD_CARD_TYPE_IO		0xFA		//有IO卡，不能自动识别，当得到此卡类型后，即以网管设置的卡类型取代

#define BD_CARD_TYPE_B		0xA1		//B--主芯片：8993
#define BD_CARD_TYPE_C		0xA2		//C--主芯片：9301
#define BD_CARD_TYPE_F		0xA3		//F--主芯片：9331A
#define BD_CARD_TYPE_G		0xA4		//G--主芯片：6652
#define BD_CARD_TYPE_H		0xA5		//H--主芯片：IP1113/IP113C/IP113C*2
//--串口卡
#define BD_CARD_TYPE_I		0x01		//I--千兆收发器（主芯片：88E6122）
#define BD_CARD_TYPE_J		0x10		//J--F3（主芯片：F3）
#define BD_CARD_TYPE_K		0x02		//K--IP113S（主芯片：IP113S）
/*5字节卡软件版本信息*/
/*第一个字节  0x03   卡类型*/
/*第二个字节  0x20   版本号*/
/*第三个字节  0x07   版本修订年*/
/*第四个字节  0x0B   版本修订月*/
/*第五个字节  0x16   版本修订日*/

typedef struct tagCARD_STATUS
{
	unsigned char		ucCardVersion[5];	/*5字节卡软件版本信息（只有自动识别的卡才有：I/J/K及电源卡）*/   
	unsigned char		ucCardstatus[16];	/*16字节状态信息*/
} CARD_STATUS;

/*远程信息*/
typedef struct tagCARD_REMOTEINFO
{   
	unsigned char		ucCardRemote[40];	/*40字节远程信息*/
} CARD_REMOTEINFO;
typedef struct tagCARD_CONFIG
{   
	unsigned char		ucCardConf[16];	/*16字节配置信息*/
} CARD_CONFIG;
/*
*功能:获取MIB变量标识符
*@ itemName :in		硬件参数名称
*@ indents	:out	硬件参数标识符
* return 找到标识符,return true,else return false
*/
boolean getItemIndent(char *itemName,char *indents);
/*
*功能:硬件参数类型
*@ typeName :in		硬件参数名称
*@ itemType :out	硬件参数类型
* return 找到变量类型,return true,else return false
*/
boolean getItemtype(char *itemName,char *itemType);
/*
*功能:获取硬件参数类型标识符
*@ itemType		:in		硬件参数类型
*@ typeIndents	:out	硬件参数类型标识符
* return 找到标识符,return true,else return false
*/
boolean getItemtypeIndents(char *itemType,char *typeIndents);
/*
*功能:根据类型名获取该类型标识
*@cardType :in	板卡类型
*return 该类型标识
*/
int getCardTypeId(char *cardType);
#endif /* SNMPVAR_H_ */