/*
* alertModel.h
* 定义告警信息数据结构
*/
#ifndef ALERTMODEL_H_
#define ALERTMODEL_H_
struct Alert{
	
	int Specific;     //告警类型
	int address;      //机架
	int slot;         //槽位
	int flag ;        //远近端 0是近端, 1表远端
	int port;         //端口,只用于电源,表示电源1或电源2
	long begintime;   //告警创建时间
	long endtime;     //告警恢复时间
	int value;        //告警值 1表示严重告警,0表示一般告警
};

/*
*告警信息链表定义
*/
struct Alerts{
	struct Alert *alert;    //告警信息指针
	struct Alerts *next;    //下一个告警信息指针
	struct Alerts *last;		//上一个告警信息
};

#endif /* ALERTMODEL_H_ */
