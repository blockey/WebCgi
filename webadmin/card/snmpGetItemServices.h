/*
 * snmpGetItemServices.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef SNMPGETITEMSERVICES_H_
#define SNMPGETITEMSERVICES_H_

/*
*功能:对字符串进行特定处理,以适合输出到界面
*@ itemName :in
*@ itemType :in
*@ data :in out
*return 转换成功 return true,else return false
*/
void dataConveter(char *itemName,char *itemType ,char *data);

#endif /* SNMPGETITEMSERVICES_H_ */