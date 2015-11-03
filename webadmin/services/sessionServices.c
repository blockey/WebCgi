/*
*sessionservices.c
*会话服务接口
*/
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../base/base.h"
#include "sessionServices.h"
#include "../dao/sessionDao.h"
#include "../util/sessionUtil.h"
#include "../model/sessionModel.h"
#include "../util/commonUtil.h"

/*
*功能:确定该session是否有效
*@ sessionId :in 
*@ username  :in
* return 如果session有效,返回真
* 说明:如果会话存在且未过期则返回真
*/
boolean isSessionValid(long sessionId,char *username){
	struct Session session;		//会话
	//重置结构空间
	resetSession(&session);
	//设置会话ID
	session.sessionId = sessionId;
	//查询会话,会话存在 
	if(getSessionDao(&session)){
		//用户名相同
		if(!strcmp(session.username,username)){
			//会话未过期
			if(time(NULL) - session.acessTime < sessionExpireTime){
				return true;
			}else{
				//会话过期则将其删除
				deleteSessionService(session.sessionId);
			}
		}
	}
	return false;
}
/*
 * 功能，保存会话信息
 * @ username :in
 * @ loginIp :in
 * return sessionId
 */
boolean saveSessionService(char *username,char *loginIp){
	
	if(username == NULL || loginIp == NULL){
		webLog("file=%s;func=%s;msg=username or loginIp is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	struct Session session ;	//会话
	//重置数据结构空间
	resetSession(&session);		
	//设置当前时间为会话访问时间
	session.acessTime = time(NULL);
	//设置访问时间为会话ID
	session.sessionId = session.acessTime;
	//获取登录IP
	strcpy(session.loginIp,loginIp);
	//设置用户名
	strcpy(session.username,username);
	//调用dao保存会话成功
	if(saveSessionDao(&session)){
		return true;
	}
	//保存会话失败
	return false;
}
/*
*功能:根据sessionId 更新最后访问时间,将过期的会话删除
*sessionId :in
*return ,success return true,else return false
*/
boolean updateSessionService(long sessionId){
	//会话ID为空
	if(sessionId == 0){
		webLog("file=%s;func=%s;msg=sessionId is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	struct Session session;			//会话
	//重置结构空间
	resetSession(&session);	
	//设置会话ID
	session.sessionId = sessionId;
	//查询该会话信息
	getSessionDao(&session);
	//会话未过期,则更新会话的最后访问时间
	if(time(NULL) - session.acessTime < sessionExpireTime){
		session.acessTime = time(NULL);
		if(updateSessionDao(&session)){
			return true;
		}
	}
	//会话已过期,则删除该会话
	else{
		deleteSessionDao(&session);
		return false;
	}
	
}
/*
*功能:根据sessionId 删除会话
*@ sessionId :in out
*return 
*/
boolean deleteSessionService(long sessionId){
	if(sessionId == 0){
		webLog("file=%s;func=%s;msg=sessionId is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	struct Session session;		//会话
	//重置数据空间
	resetSession(&session);		
	//设置会话ID
	session.sessionId = sessionId;
	//调用DAO删除会话
	//删除成功
	if(deleteSessionDao(&session)){
		return true;
	}
	//删除失败
	else{
		return false;
	}
}
/*
*功能：根据数据结构中设置的数据作为条件查询符合条件的会话
*@conditions :in　在这里设置要查询会话的条件
*return 
*说明:查询条件字符串采用"name1=value1;name2=value2;"的形式,支持模糊查询
*暂未用到
*/
boolean querySessionsService(char *conditions){
	if(conditions == NULL){
		webLog("file=%s;func=%s;msg=query conditions is null ;",__FILE__,__FUNCTION__);
		return false;
	}
	char value[200];				//中间变量
	struct Session querySession;	//用于查询的会话结构体
	struct Sessions *sessions;		//返回结果链表
	//重置会话结构体
	resetSession(&querySession);
	//根据请求参数设置查询结构体的成员变量 
	//会话ID
	if(getValueByName(conditions,"sessionId",value)){
		querySession.sessionId = atol(value);
	}
	//用户名
	if(getValueByName(conditions,"username",value)){
		strcpy(querySession.username,value);
	}
	//最后访问时间
	if(getValueByName(conditions,"acessTime",value)){
		querySession.acessTime = atol(value);
	}
	//登录IP
	if(getValueByName(conditions,"loginIp",value)){
		strcpy(querySession.loginIp,value);
	}	
	if(querySessionsDao(&querySession,&sessions)){
		
		//根据际需求处理 
		return true;
	}
}
/*
*功能:确定该用户是否登录
*@ username in 
* return 如果已登录,返回真
*/
boolean isUserLogin(char *username){
	if(username == NULL){
		webLog("file=%s;func=%s;msg=username is null;",__FILE__,__FUNCTION__);
		return false;
	}
	struct Session sessionQuery;		//查询会话
	struct Sessions *sessions;			//会话链表
	//重置会话
	resetSession(&sessionQuery);		
	//设置用户名
	strcpy(sessionQuery.username,username);
	//调用DAO查询
	if(getSessionDao(&sessionQuery)){
		//会话未过期
//		if(time(NULL) - sessionQuery.acessTime < sessionExpireTime){
//			return true;
//		}else{
			//会话过期则将其删除
			deleteSessionService(sessionQuery.sessionId);
//		}
	}
	return false;
}
/*
*功能:如果用户已登录,则删除该用户会话
*@ username in 
* return 如果已登录,返回真
*/
boolean deleteLastLogin(char *username){
	if(username == NULL){
		webLog("file=%s;func=%s;msg=username is null;",__FILE__,__FUNCTION__);
		return false;
	}
	struct Session sessionQuery;		//查询会话
	struct Sessions *sessions;			//会话链表
	//重置会话
	resetSession(&sessionQuery);		
	//设置用户名
	strcpy(sessionQuery.username,username);
	//调用DAO查询
	if(getSessionDao(&sessionQuery)){
//		//会话未过期
//		if(time(NULL) - sessionQuery.acessTime < getSessionExpireTime()){
//			return true;
//		}else{
		//会话过期则将其删除
		//如果用户已登录,则删除该用户会话,modify by zhaofeihua 2011-5-12 ,
		deleteSessionService(sessionQuery.sessionId);
//		}
	}
	return false;
}
/*
*功能:通过sessionId或username获取会话各字段的值
*@session :in out
*success return true,else return false
*说明:如果sessionId username同时存在,则只匹配sessionId
*/
boolean getSessionService(struct Session *session){
	
	if(session->sessionId == 0 && session->username == NULL){
		webLog("file=%s;func=%s;msg=sessionId and username are both null;",__FILE__,__FUNCTION__);
		return false;
	}
	//调用DAO查询
	if(getSessionDao(session)){
		return true;
	}
	return false;
}



