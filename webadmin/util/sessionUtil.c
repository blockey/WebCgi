#include <stdlib.h>
#include "../model/sessionModel.h"
#include "sessionUtil.h"

/*
*功能：重置User数据
*@ session :in out
*return
*/
void resetSession(struct Session *session){
	//用户名
	session->username[0]=0;
	//会话ID
	session->sessionId = 0;
	//最后访问时间
	session->acessTime = 0;
	//登录IP
	session->loginIp[0] = 0;
}
/*
*功能：释放链表内存
*users :in out
*return 
*/
void releaseSessionsMemory(struct Sessions *sessions){
	struct Sessions *p;		//链表指针
	//逐个结点释放
	while(sessions){
		free(sessions->session);
		p = sessions;
		sessions = sessions->next;
		free(p);
	}
}

