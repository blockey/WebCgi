############################################################
#
#要生成的所有CGI文件
#分别为:
#用户登录
#验证用户登录
#退出系统
#读取用户信息
#设置用户信息
#读取运行日志
#读取操作日志
#读取告警信息
#设置硬件配置
#读取硬件配置
#GB2312编码
############################################################

#Define With the compiler library path.
export LD_LIBRARY_PATH=/eptn/buildroot/usr/lib

all: login.cgi \
	  checkLogin.cgi \
	  loginOut.cgi \
	  upload4.cgi	\
	  requpdate.cgi	\
	  getperm.cgi	\
	  getDevFiles.cgi	\
	  subGetDevFiles.cgi	\
	  getBinFiles.cgi	\
	  setDevFiles.cgi	\
	  subSetDevFiles.cgi \
	  progress.cgi \
	  setConfig.cgi


############################################################
#
#编译参数
#
############################################################
#编译器
#CC = gcc
#CC = /usr/local/arm/3.3.2/bin/arm-linux-gcc
#CC = arm-linux-gcc 
CC = /eptn/buildroot/usr/bin/arm-linux-gcc
#编译参数
#CFLAGS = -O -Wall 
#DLOG_ for log 
CFLAGS = -O -DLOG_
#引用库
# CLIBS = -lm
CLIBS = 



############################################################
#
#目标文件,根据功能划分为8个模块
#
############################################################
#用户信息目标文件
USEROBJS   =  util/userUtil.o \
			 	  dao/userDao.o \
			 	  services/userServices.o 
#会话信息目标文件
SESSIONOBJS = util/sessionUtil.o \
				  dao/sessionDao.o \
				  services/sessionServices.o
#共用模块目标文件
COMMONOBJS =  base/base.o \
				  util/adstring.o \
				  util/commonUtil.o \
				  lib/cgic.o \
				  util/lockFile.o

#权限管理目标文件
AUTHORITYOBJS = services/authorityServices.o


############################################################
#
#生成CGI
#
############################################################

login.cgi: 	action/login.o \
				${COMMONOBJS} \
				${USEROBJS} \
				${SESSIONOBJS} 
	${CC} ${CFLAGS} -o $@ $^ ${CLIBS} 
checkLogin.cgi:action/checkLogin.o \
				${COMMONOBJS} \
				${SESSIONOBJS}
	${CC} ${CFLAGS} -o $@ $^ ${CLIBS} 
loginOut.cgi:action/loginOut.o \
				${COMMONOBJS} \
				${SESSIONOBJS}
	${CC} ${CFLAGS} -o $@ $^ ${CLIBS} 
upload4.cgi: action/upload4.c lib/cgic.o \
				${COMMONOBJS}	\
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^	
requpdate.cgi: action/reqUpdate.c lib/cgic.o \
				${COMMONOBJS}	\
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^	

getDevFiles.cgi: action/getDevFiles.c lib/cgic.o \
				${COMMONOBJS}	\
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^	
subGetDevFiles.cgi: action/subGetDevFiles.c lib/cgic.o \
				${COMMONOBJS}	\
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^		
getBinFiles.cgi: action/getBinFiles.c lib/cgic.o \
				${COMMONOBJS}	\
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^		
setDevFiles.cgi: action/setDevFiles.c lib/cgic.o \
				${COMMONOBJS}	\
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^			
subSetDevFiles.cgi: action/subSetDevFiles.c lib/cgic.o \
				${COMMONOBJS}	\
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^	
progress.cgi: action/progress.c lib/cgic.o \
				${COMMONOBJS} \
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^	
upload2.cgi: action/upload2.c lib/cgic.o 
	${CC} ${CFLAGS} -o $@ $^
getperm.cgi:action/getperm1.c lib/cgic.o base/base.o \
				${COMMONOBJS}	
	${CC} ${CFLAGS} -o $@ $^        
setConfig.cgi: action/setConfig.c lib/cgic.o \
				${COMMONOBJS}	\
				${SESSIONOBJS} \
				${USEROBJS} \
				${AUTHORITYOBJS}
	${CC} ${CFLAGS} -o $@ $^		
############################################################
#
#编译隐式规则
#
############################################################
%.o:%.c
	${CC} ${CFLAGS} -c $< -o $@
############################################################
#
#删除目标文件
#
############################################################
clean:
	rm -f ./*.cgi \
			action/*.o \
			${COMMONOBJS} \
			${USEROBJS} \
			${SESSIONOBJS} \
	 		${AUTHORITYOBJS} 

