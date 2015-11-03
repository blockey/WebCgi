#ifndef SYSLOGMODEL_H_
#define SYSLOGMODEL_H_
//系统运行日志
struct Syslog{
	long logId;
	char username[40];
	long createTime;
	char isSuccess[2];
	char action[128];
	char loginIp[20];
};
struct Syslogs{
	struct Syslog *syslog;
	struct Syslogs *next;
};

#endif /* SYSLOGMODEL_H_ */
