/*
 * sysvar.h
 *
 *  Created on: 26-12月-08
 *      Author: root
 */

#ifndef 	SYSVAR_H_
#define 	SYSVAR_H_
#include "../base/base.h"


/*
*功能:根据actName获取该动作的描述
*@actName
*@desc
*success return true,else return false
*/
boolean getActionDesc(char *actName,char *desc);
/*
*功能:获取用户权限描述
*@category :in
*@desc :out
*return 
*/
void getCategoryDesc(char *category,char *desc);
#endif /* SYSVAR_H_ */
