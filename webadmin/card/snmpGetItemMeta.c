#include <string.h>
#include "snmpvar.h"
#include "../util/adstring.h"
#include "snmpGetItemMeta.h"
#include <stdio.h>
#include <stdlib.h>

/*
*功能:获取告警信息
*@ itemName :in
*@ data :in out
*return 
*/
void getAlertInfo(const CARD_STATUS status[],char *outData){
	char buffer[50]="";		//字符放放
	char binstr[9];			//二进制字符串
	int i;
	/*
	*有告警则用1表示
	*无告警则用0表示
	*/
	//设置输出结果
	strcpy(outData,"alertInfo=");
	
	//卡类型
	for(i = 0;i< GLOBAL_MAX_CARDNUM;i++){
		//电源
		if(status[i].ucCardVersion[0] == BD_CARD_TYPE_POWER){
			//将该字节转换为二进制
			int2binstr(status[i].ucCardstatus[0],binstr);
			//电压
			if(binstr[5]== '1'){
				strcat(buffer,"1,");
			}else{
				strcat(buffer,"0,");
			}			
			//温度
			if(binstr[4]== '1'){
				strcat(buffer,"1,");
			}else{
				strcat(buffer,"0,");
			}
			//湿度
			if(binstr[3]== '1'){
				strcat(buffer,"1,");
			}else{
				strcat(buffer,"0,");
			}
			//风扇1
			if(binstr[2]== '1'){
				strcat(buffer,"1,");
			}else{
				strcat(buffer,"0,");
			}
			//风扇2
			if(binstr[1]== '1'){
				strcat(buffer,"1,");
			}else{
				strcat(buffer,"0,");
			}
			strcat(outData,buffer);
			break;
		}
	}
	//用;分隔每项信息
	strcat(outData,";");
	if(!strcmp(outData,"alertInfo=;")){
		strcpy(outData,"alertInfo=0,0,0,0,0,;");
	}
}
/*
*功能:获取电源数据
*@ itemName :in
*@ data :in out
*return 
*/
void getPowerInfo(const CARD_STATUS status[],char *outData){
	char buffer[50];	//中间变量
	char value[5];		//临时变量
	int temp;			//临时变量
	int i;
	//设置输出数据
	strcpy(outData,"powerInfo=");
	//卡类型
	for(i = 0;i< GLOBAL_MAX_CARDNUM;i++){
		//电源
		if(status[i].ucCardVersion[0] == BD_CARD_TYPE_POWER){
			//电压
			sprintf(buffer,"%X.%02X,",status[i].ucCardstatus[1],status[i].ucCardstatus[2]);
			strcat(outData,buffer);
			//温度
			sprintf(buffer,"%d,",status[i].ucCardstatus[3]);
			strcat(outData,buffer);
			//湿度
			sprintf(buffer,"%d,",status[i].ucCardstatus[4]);
			strcat(outData,buffer);
			//风扇1
			sprintf(buffer,"%d,",status[i].ucCardstatus[6]*256+status[i].ucCardstatus[5]);
			strcat(outData,buffer);
			//风扇2
			sprintf(buffer,"%d,",status[i].ucCardstatus[8]*256+status[i].ucCardstatus[7]);
			strcat(outData,buffer);
			break;
		}
	}
	//用;分隔每项信息
	strcat(outData,";");
	//出现异常
	if(!strcmp(outData,"powerInfo=;")){
		strcpy(outData,"powerInfo=0.00,0,0,0,0,;");
	}
}
/*
*功能:获取远端信息
*@ itemName :in
*@ data :in out
*return 
*/
void getRemoteInfo(const CARD_STATUS status[],char *outData){
	char buffer[50];		//存放字符串

	int i;
	//设置输出数据
	strcpy(outData,"remotepoint=");
	//卡类型
	for(i = 0;i< GLOBAL_MAX_CARDNUM;i++){
		//表示第几块卡
		sprintf(buffer,"%d ",i+1);
		strcat(outData,buffer);
		//无卡
		if(status[i].ucCardVersion[0] == BD_CARD_TYPE_NULL){
			strcat(outData,"0,");//表示无远端 
			continue;
		}
		//F3
		if(status[i].ucCardVersion[0] == BD_CARD_TYPE_J){
			//近端和远端的FX都亮,则表明有远端
			if( (status[i].ucCardstatus[0] & 0x10) || (status[i].ucCardstatus[2] & 0x02)){
				strcat(outData,"0 ");//表示没远端 
			}else{
				
				strcat(outData,"1 ");//表示有远端 
				strcat(outData,"J(F3) ");//表示F3远端 
				//webLog("%02X,%02X",status[i].ucCardstatus[1],status[i].ucCardstatus[2]);
				//FL
				if(status[i].ucCardstatus[2] & 0x02){
					strcat(outData,"0 ");
				}else{
					strcat(outData,"1 ");
				}
				//F/H
				if((status[i].ucCardstatus[2] & 0x04) == 0 && status[i].ucCardstatus[1] & 0x01){
					strcat(outData,"1 ");
				}else{
					strcat(outData,"0 ");
				}
				//FX
				if(status[i].ucCardstatus[2] & 0x02){
					strcat(outData,"0 ");
				}else{
					strcat(outData,"1 ");
				}
				//PW
				strcat(outData,"1 ");
				//COL
				strcat(outData,"0 ");
				//TP
				if(status[i].ucCardstatus[2] & 0x04){
					strcat(outData,"0 ");
				}else{
					strcat(outData,"1 ");
				}
				//SPEED
				if(status[i].ucCardstatus[2] & 0x04){ //TX connect 
					//100 OFF
					strcat(outData,"0 ");
					//10 ON 
					strcat(outData,"0 ");
				}else{//Tx not connect 
					
					if((status[i].ucCardstatus[1] & 0x06) == 0 ){
						//100 OFF
						strcat(outData,"0 ");
						//10 ON 
						strcat(outData,"1 ");
						
					}else if((status[i].ucCardstatus[1] & 0x06) == 0x04) {
						//100 ON
						strcat(outData,"1 ");
						//10 OFF 
						strcat(outData,"0 ");
						
					}else{//no define 
						//100 OFF
						strcat(outData,"0 ");
						//10 ON 
						strcat(outData,"0 ");
						
					}
					
				}
				
			}
			strcat(outData,",");//该卡数据结束
			continue;
		}//end if
		//IP113S
		
		if(status[i].ucCardVersion[0] == BD_CARD_TYPE_K){
			//远端掉电
			if(status[i].ucCardstatus[1] & 0x04){
				strcat(outData,"1 ");//表示有远端 ,但掉电
				strcat(outData,"K(IP113S) ");//表示IP113S远端 
				//FL
				strcat(outData,"0 ");
				//F/H
				strcat(outData,"0 ");
				//FX
				strcat(outData,"0 ");
				//PW
				strcat(outData,"0 ");
				//OAM
				strcat(outData,"0 ");
				//TP
				strcat(outData,"0 ");
				//SPD
				strcat(outData,"0 ");
				//TL
				strcat(outData,"0 ");
			}
			else {//远端供电正常
				//表示有远端 
				if((status[i].ucCardstatus[0] & 0x04 ) && (status[i].ucCardstatus[6] & 0x04)){
					strcat(outData,"1 ");
					strcat(outData,"K(IP113S) ");//表示IP113S远端 
					//FL
					if(status[i].ucCardstatus[6] & 0x04){
						strcat(outData,"1 ");
					}else{
						strcat(outData,"0 ");
					}
					//F/H
					if(status[i].ucCardstatus[6] & 0x20 && status[i].ucCardstatus[6] & 0x08){
						strcat(outData,"1 ");
					}else{
						strcat(outData,"0 ");
					}
					//FX
					if(status[i].ucCardstatus[6] & 0x04){
						strcat(outData,"1 ");
					}else{
						strcat(outData,"0 ");
					}
					//PW
					strcat(outData,"1 ");
					//OAM
					if(status[i].ucCardstatus[6] & 0x02){
						strcat(outData,"1 ");
					}else{
						strcat(outData,"0 ");
					}
					//TP
					if(status[i].ucCardstatus[6] & 0x20){
						strcat(outData,"1 ");
					}else{
						strcat(outData,"0 ");
					}
					//SPD
					if(status[i].ucCardstatus[6] & 0x20 && status[i].ucCardstatus[6] & 0x10){
						strcat(outData,"1 ");
					}else{
						strcat(outData,"0 ");
					}
					//TL
					if(status[i].ucCardstatus[6] & 0x20){
						strcat(outData,"1 ");
					}else{
						strcat(outData,"0 ");
					}
				}else{
					//没远端 
					strcat(outData,"0 ");
				}
			}
			
			strcat(outData,",");//该卡数据结束
			continue;
		}//end if
		strcat(outData,"0,");//表示没远端 
	}// end for
	strcat(outData,";");
	
}


/*
*功能:获取远端卡连接状态 
*@ itemName :in
*@ data :in out
*return 
*/
void getRemoteConnStatus(const CARD_STATUS status,char *outData ){
	/*
	*输出数据,on 表示远端连接 off 表示远端断开 
	*/
	int i;
	//设置输出数据
	strcpy(outData,"singleDeviceCardRmtConnStatus=");
	
	//卡类型
	//for(i = 0;i< GLOBAL_MAX_CARDNUM;i++){
		//无卡
		if(status.ucCardVersion[0] == BD_CARD_TYPE_NULL){
			strcat(outData,"off");
		}
		
		//有IO卡，不能自动识别，当得到此卡类型后，即以网管设置的卡类型取代
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_IO){
			strcat(outData,"off");
		}
		//B--主芯片：8993
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_B){
			//FX ON ,so remote must be connected 
			if(status.ucCardstatus[0] & 0x08){
				strcat(outData,"on");
					
			}else{
				strcat(outData,"off");
			}
			
		}
		//C--主芯片：9301
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_C){
			//FX ON ,so remote must be connected 
			if(status.ucCardstatus[0] & 0x08){
				strcat(outData,"on");
					
			}else{
				strcat(outData,"off");
			}
		}
		//F--主芯片：9331A
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_F){
			
			//FX ON ,so remote must be connected 
			if(status.ucCardstatus[0] & 0x08){
				strcat(outData,"on");
			}else{
				strcat(outData,"off");
			}
		}
		//G--主芯片：6652
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_G){
			
			//FX ON ,so remote must be connected 
			if(status.ucCardstatus[0] & 0x04 || status.ucCardstatus[0] & 0x08){
				strcat(outData,"on");
			}else{
				strcat(outData,"off");
			}
			
		}
		//H--主芯片：IP1113/IP113C/IP113C*2
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_H){
			//FX ON ,so remote must be connected 
			if(status.ucCardstatus[0] & 0x08){
				strcat(outData,"on");
			}else{
				strcat(outData,"off");
			}
		}
		//I--千兆收发器（主芯片：88E6122）
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_I){
			
			//FX ON ,so remote must be connected 
			if(status.ucCardstatus[0] & 0x02){
				strcat(outData,"on");
			}else{
				strcat(outData,"off");
			}
			
		}
		//J--F3（主芯片：F3）
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_J){
			
			//FX ON ,so remote must be connected 
			if(status.ucCardstatus[0] & 0x10 ){
				//FX OFF
				strcat(outData,"off");
			}else{
				//FX ON
				strcat(outData,"on");
			}
			
		}
		//K--IP113S（主芯片：IP113S）
		else if(status.ucCardVersion[0] == BD_CARD_TYPE_K){
			
			//FX ON ,so remote must be connected 
			if(status.ucCardstatus[0] & 0x04 ){
				strcat(outData,"on");
			}else{
				strcat(outData,"off");
			}
			
		}
		//error!
		else {
			strcat(outData,"off");
			webLog("file=%s;func=%s;msg=can't find the infomation of card type %d ;",__FILE__,__FUNCTION__,status.ucCardVersion[0]);
			
		}
		
	//}
	strcat(outData,";");
}



/*
*功能:获取灯状态
*@ itemName :in
*@ data :in out
*return 
*/
void getLightStatus(const CARD_STATUS status[],char *outData ){
	char lightBuffer[15];		//灯信息
	int i;
	/*
	*1表示灯亮
	*0表示灯灭
	*/
	//设置输出数据
	strcpy(outData,"light=");
	
	//卡类型
	for(i = 0;i< GLOBAL_MAX_CARDNUM;i++){
		//无卡
		if(status[i].ucCardVersion[0] == BD_CARD_TYPE_NULL){
			sprintf(lightBuffer,"%s ,","0 0 0 0 0 0 0 0");
		}
		//电源
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_POWER){
			//int2binstr(status[i].ucCardstatus[0],binstr);
			strcpy(lightBuffer,"");
			//POWER1
			if(status[i].ucCardstatus[0] & 0x01){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//POWER2
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			strcat(lightBuffer,",");	
		}
		//有IO卡，不能自动识别，当得到此卡类型后，即以网管设置的卡类型取代
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_IO){
			sprintf(lightBuffer,"%s ,","0 0 0 0 0 0 0 0");
		}
		//B--主芯片：8993
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_B){
			//int2binstr(status[i].ucCardstatus[0],binstr);
			strcpy(lightBuffer,"");
			//10M?
			if(status[i].ucCardstatus[0] & 0x01){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			
			//100M ?
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			
			//TP
			if((status[i].ucCardstatus[0] & 0x02) || (status[i].ucCardstatus[0] & 0x01)){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			
			//COL temp set 0
			strcat(lightBuffer,"0 ");
			//Power always on set 1
			strcat(lightBuffer,"1 ");
			//FX
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			
			// no light
			strcat(lightBuffer,"0 ");
			//FLA
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			
			strcat(lightBuffer,",");
		}
		//C--主芯片：9301
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_C){
			//int2binstr(status[i].ucCardstatus[0],binstr);
			strcpy(lightBuffer,"");
			//100A?
			if((status[i].ucCardstatus[0] & 0x02 ) ){
				if(status[i].ucCardstatus[0] & 0x01){
					strcat(lightBuffer,"1 ");	
					
				}else{
					strcat(lightBuffer,"0 ");	
				}
				
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//100L ?
			if((status[i].ucCardstatus[0] & 0x02 ) ){
				if(status[i].ucCardstatus[0] & 0x01){
					strcat(lightBuffer,"1 ");	
					
				}else{
					strcat(lightBuffer,"0 ");	
				}
				
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//TP
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			
			//COL temp set 0
			strcat(lightBuffer,"0 ");
			//Power always on set 1
			strcat(lightBuffer,"1 ");
			//FX
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			
			// FA hava data 
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			//FL
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");	
					
			}else{
				strcat(lightBuffer,"0 ");	
			}
			
			strcat(lightBuffer,",");
		}
		//F--主芯片：9331A
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_F){
			//int2binstr(status[i].ucCardstatus[0],binstr);
			strcpy(lightBuffer,"");
			
			//F/H
			if((status[i].ucCardstatus[0] & 0x02 ) && (status[i].ucCardstatus[0] & 0x20 )){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//100LA
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//TP
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//AUTOL temp set 0
			strcat(lightBuffer,"0 ");
			//Power always on set 1
			strcat(lightBuffer,"1 ");
			//FX
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//FLT
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			// FLA
			strcat(lightBuffer,"0 ");
			strcat(lightBuffer,",");
			
		}
		//G--主芯片：6652
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_G){
			//
 			strcpy(lightBuffer,"");
			//10L 100L  解决10M 100M灯同时亮 
			if(status[i].ucCardstatus[0] & 0x01){
				//10L ON 
				strcat(lightBuffer,"1 ");
				//100L OFF 
				strcat(lightBuffer,"0 ");
			}else{
				//10L OFF 
				strcat(lightBuffer,"0 ");
				//100L ==== 
				if(status[i].ucCardstatus[0] & 0x02){
					//100L ON
					strcat(lightBuffer,"1 ");
				}else{
					//100L OFF 
					strcat(lightBuffer,"0 ");
				}
			}
			
			//100L
//			if(status[i].ucCardstatus[0] & 0x02){
//				strcat(lightBuffer,"1 ");
//			}else{
//				strcat(lightBuffer,"0 ");
//			}
			
			//TP
			if(status[i].ucCardstatus[0] & 0x01 || status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//ACT temp set 0
			strcat(lightBuffer,"0 ");
			//Power always on set 1
			strcat(lightBuffer,"1 ");
			//FX
			if(status[i].ucCardstatus[0] & 0x04 || status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//F10L
			if(status[i].ucCardstatus[0] & 0x04){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//F100L
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			strcat(lightBuffer,",");
		}
		//H--主芯片：IP1113/IP113C/IP113C*2
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_H){
			//int2binstr(status[i].ucCardstatus[0],binstr);
			strcpy(lightBuffer,"");
			//SPD
			
			if(status[i].ucCardstatus[0] & 0x01){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//TLA
			
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//TP
			
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//COL temp set 0
			strcat(lightBuffer,"0 ");
			//Power always on set 1
			strcat(lightBuffer,"1 ");
			//FX
			
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//F/H
			
			if((status[i].ucCardstatus[0] & 0x02) && (status[i].ucCardstatus[0] & 0x40)){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//FLA
			
			if(status[i].ucCardstatus[0] & 0x08){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			strcat(lightBuffer,",");
		}
		//I--千兆收发器（主芯片：88E6122）
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_I){
			//int2binstr(status[i].ucCardstatus[0],binstr);
			strcpy(lightBuffer,"");
			//TP电口未连接,则速率灯不亮
			
			if(status[i].ucCardstatus[0] & 0x04){
				//
				if((status[i].ucCardstatus[0] & 0xC0) == 0x80){
					//1000M ON 
					strcat(lightBuffer,"1 ");
					//100M OFF 
					strcat(lightBuffer,"0 ");
				}else if((status[i].ucCardstatus[0] & 0xC0) == 0x40){
					//1000M OFF
					strcat(lightBuffer,"0 ");
					//100M ON
					strcat(lightBuffer,"1 ");
				}else{
					//10M ON
					//1000M OFF
					strcat(lightBuffer,"0 ");
					//100M ON
					strcat(lightBuffer,"0 ");
				}
			}else{
				//1000M off
				strcat(lightBuffer,"0 ");
				//100M off
				strcat(lightBuffer,"0 ");
			}
			
			//TP
			if(status[i].ucCardstatus[0] & 0x04){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//TLA
			if(status[i].ucCardstatus[0] & 0x04){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//Power always on set 1
			strcat(lightBuffer,"1 ");
			//FX
			
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//F/H
			if((status[i].ucCardstatus[0] & 0x04) && (status[i].ucCardstatus[0] & 0x20)){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			//FLA
			if(status[i].ucCardstatus[0] & 0x02){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			strcat(lightBuffer,",");
		}
		//J--F3（主芯片：F3）
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_J){
			//int2binstr(status[i].ucCardstatus[0],binstr);
			strcpy(lightBuffer,"");
			//电口连接上才亮
			if(status[i].ucCardstatus[0] & 0x08){
				//10M OFF
				strcat(lightBuffer,"0 ");
				//100M OFF
				strcat(lightBuffer,"0 ");
			}else{
				
				if(status[i].ucCardstatus[0] & 0x02){
					//10M OFF
					strcat(lightBuffer,"0 ");
					//100M ON 
					strcat(lightBuffer,"1 ");
				}else{
					//10M ON
					strcat(lightBuffer,"1 ");
					//100M OFF
					strcat(lightBuffer,"0 ");
				}
			}
			
			//TP
			
			if(status[i].ucCardstatus[0] & 0x08){
				//TP OFF
				strcat(lightBuffer,"0 ");
			}else{
				//TP ON
				strcat(lightBuffer,"1 ");
			}
			//COL temp set 0
			strcat(lightBuffer,"0 ");
			//Power always on set 1
			strcat(lightBuffer,"1 ");
			//FX
			if(status[i].ucCardstatus[0] & 0x10 ){
				//FX ON
				strcat(lightBuffer,"0 ");
			}else{
				//FX OFF
				strcat(lightBuffer,"1 ");
				
			}
			//电口连接上才有意义
			//F/H
			if(status[i].ucCardstatus[0] & 0x08 ){
				//OFF 
				strcat(lightBuffer,"0 ");
				
			}else{
			   //OF
				if(status[i].ucCardstatus[0] & 0x01 ){
					// ON 
					strcat(lightBuffer,"1 ");
				}else{
					//OFF 
					strcat(lightBuffer,"0 ");
				}
			}
			
			//FLA
			if(status[i].ucCardstatus[0] & 0x10 ){
				strcat(lightBuffer,"0 ");
			}else{
				strcat(lightBuffer,"1 ");
			}
			strcat(lightBuffer,",");
		}
		//K--IP113S（主芯片：IP113S）
		else if(status[i].ucCardVersion[0] == BD_CARD_TYPE_K){
			//int2binstr(status[i].ucCardstatus[0],binstr);
			strcpy(lightBuffer,"");
			//TLA
			
			if(status[i].ucCardstatus[0] & 0x20 ){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//SPD
			
			if(status[i].ucCardstatus[0] & 0x10 ){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//TP
			
			if(status[i].ucCardstatus[0] & 0x20 ){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//OAM 
			
			if(status[i].ucCardstatus[0] & 0x02 ){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//Power 
			
			if(status[i].ucCardstatus[0] & 0x80 ){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//FX
			
			if(status[i].ucCardstatus[0] & 0x04 ){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//F/H
			
			if((status[i].ucCardstatus[0] & 0x20) && (status[i].ucCardstatus[0] & 0x08) ){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			//FLA
			if(status[i].ucCardstatus[0] & 0x04 ){
				strcat(lightBuffer,"1 ");
			}else{
				strcat(lightBuffer,"0 ");
			}
			
			strcat(lightBuffer,",");
		}
		//error!
		else {
			//无数据
			sprintf(lightBuffer,"%s,","0 0 0 0 0 0 ");
			webLog("file=%s;func=%s;msg=can't find the infomation of card type %d ;",__FILE__,__FUNCTION__,status[i].ucCardVersion[0]);
			
		}
		strcat(outData,lightBuffer);	
	}
	strcat(outData,";");
}
/*
*功能:获取卡类型
*@ status	:in
*@ outData	:out
*return 
*/
void getCardType(const CARD_STATUS status[],char *outData ){
	char tempstr[70]="";		//临时变量
	int i;
	//设置返回数据
	strcpy(outData,"type=");
	//卡类型
	for(i = 0;i< GLOBAL_MAX_CARDNUM;i++){
		//获取卡的字符描述
		getCardTypeByCategory(status[i].ucCardVersion[0],tempstr);
		strcat(tempstr,",");
		strcat(outData,tempstr);	
	}
	strcat(outData,";");
}

/*
*功能:获取卡类型
*@ status :in
*@ outData :out
*return 
*/
void getStatistiesCardTypes(const CARD_STATUS status[],char *outData ){
	char tempstr[70]="";
	int i;
	strcpy(outData,"");
	//卡类型
	for(i = 0;i< GLOBAL_MAX_CARDNUM;i++){
		//获取卡类型的字符描述
		getCardTypeAndDescByCategory(status[i].ucCardVersion[0],tempstr);
		strcat(tempstr," ");
		strcat(outData,tempstr);	
	}
	
}
/*
*功能:根据typeId获取卡类型的字符描述
*@ typeId :in
*@ type :out
*return 
*/
void getCardTypeByCategory(int typeId,char *type){
		
		//无卡
		if(typeId == BD_CARD_TYPE_NULL){
			sprintf(type,"%s","none");
		}
		//电源
		else if(typeId == BD_CARD_TYPE_POWER){
			sprintf(type,"%s","power");
			
		}
		//有IO卡，不能自动识别，当得到此卡类型后，即以网管设置的卡类型取代
		else if(typeId == BD_CARD_TYPE_IO){
			sprintf(type,"%s","undefine");
		}
		//B--主芯片：8993
		else if(typeId == BD_CARD_TYPE_B){
			sprintf(type,"%s","8993");
		}
		//C--主芯片：9301
		else if(typeId == BD_CARD_TYPE_C){
			sprintf(type,"%s","9301");
		}
		//F--主芯片：9331A
		else if(typeId == BD_CARD_TYPE_F){
			sprintf(type,"%s","9331");
		}
		//G--主芯片：6652
		else if(typeId == BD_CARD_TYPE_G){
			sprintf(type,"%s","6652");
		}
		//H--主芯片：IP1113/IP113C/IP113C*2
		else if(typeId == BD_CARD_TYPE_H){
			sprintf(type,"%s","IP113");
		}
		//I--千兆收发器（主芯片：88E6122）
		else if(typeId == BD_CARD_TYPE_I){
			sprintf(type,"%s","1000M");
		}
		//J--F3（主芯片：F3）
		else if(typeId == BD_CARD_TYPE_J){
			sprintf(type,"%s","F3");
		}
		//K--IP113S（主芯片：IP113S）
		else if(typeId == BD_CARD_TYPE_K){
			sprintf(type,"%s","IP113S");
		}
		//error!,defaule none 
		else {
			sprintf(type,"%s","none");
			webLog("file=%s;func=%s;msg=can't find the infomation of type id %02x;",__FILE__,__FUNCTION__,typeId);
			
		}
}

/*
*功能:根据typeId获取卡类型的字符描述 B(8993) 形式
*@ typeId :in
*@ type :out
*return 
*/
void getCardTypeAndDescByCategory(int typeId,char *type){
		
		//无卡
		if(typeId == BD_CARD_TYPE_NULL){
			sprintf(type,"%s","none");
		}
		//电源
		else if(typeId == BD_CARD_TYPE_POWER){
			sprintf(type,"%s","power");
			
		}
		//有IO卡，不能自动识别，当得到此卡类型后，即以网管设置的卡类型取代
		else if(typeId == BD_CARD_TYPE_IO){
			sprintf(type,"%s","undefine");
		}
		//B--主芯片：8993
		else if(typeId == BD_CARD_TYPE_B){
			sprintf(type,"%s","B(8993)");
		}
		//C--主芯片：9301
		else if(typeId == BD_CARD_TYPE_C){
			sprintf(type,"%s","C(9301)");
		}
		//F--主芯片：9331A
		else if(typeId == BD_CARD_TYPE_F){
			sprintf(type,"%s","F(9331)");
		}
		//G--主芯片：6652
		else if(typeId == BD_CARD_TYPE_G){
			sprintf(type,"%s","G(6652)");
		}
		//H--主芯片：IP1113/IP113C/IP113C*2
		else if(typeId == BD_CARD_TYPE_H){
			sprintf(type,"%s","H(IP113)");
		}
		//I--千兆收发器（主芯片：88E6122）
		else if(typeId == BD_CARD_TYPE_I){
			sprintf(type,"%s","I(1000M)");
		}
		//J--F3（主芯片：F3）
		else if(typeId == BD_CARD_TYPE_J){
			sprintf(type,"%s","J(F3)");
		}
		//K--IP113S（主芯片：IP113S）
		else if(typeId == BD_CARD_TYPE_K){
			sprintf(type,"%s","K(IP113S)");
		}
		//error!
		else {
			sprintf(type,"%s","none");
			webLog("file=%s;func=%s;msg=can't find the infomation of type id %02x;",__FILE__,__FUNCTION__,typeId);
			
		}
}
/*
*功能:获取上/下行的带宽M
*@str1 :in 第一个字节
*@str2 :in 第二个字节
*return 结果
*/
int getRunRateM(char *str1,char *str2){
	char str[8];
	
	int rates[7] = {64,32,16,8,4,2,1};//每一位代表的带宽
	int i,j;
	int rate=0;
	//高位部分
	for(i = 0,j=4;j<8;i++,j++){
		str[i] = str1[j];
	}
	//低位部分
	for(j=0;i<7;j++,i++){
		str[i]=str2[j];
	}
	str[7]=0;
	//计算
	for(i = 0;i<7;i++){
		if(str[i] == '1'){
			rate += rates[i];
		}
	}
	return rate;
}
/*
*功能:设置上/下行的带宽M
*@conf2 :out ucCardConf[2]
*@conf3 :out ucCardConf[3]
*return 
*/
void setRunRateM(int rate,unsigned char *conf2,unsigned char *conf3){
	int rates[7] = {64,32,16,8,4,2,1};  //每一位代表的带宽
	char str[8];		
	int i = 0;
	unsigned char setBit2=0x08;
	unsigned char setBit3=0x80;
	for( i =0;i<7;i++){
		if(rate >=rates[i]){
			rate = rate - rates[i];
			str[i]='1';
		}else{
			str[i] = '0';
		}	
	}
	str[i]='\0';
	
	
	for(i = 0;i<7;i++){
		if(i < 4){
			if(str[i] == '1'){
				(*conf2) |= setBit2;
				
			}else{
				(*conf2) &= ~setBit2;
				
			}
			setBit2 >>= 1;
		}else{
			if(str[i] == '1'){
				(*conf3) |= setBit3;
				
			}else{
				(*conf3) &= ~setBit3;
				
			}
			setBit3 >>= 1;
		}
	}
	
}

/*
*功能:获取上/下行的带宽K
*@str :in 第一个字节
*return 结果
*/
int getRunRateK(char *str){
	int rates[5]={512,256,128,64,32};
	int i ,j;
	int rate = 0;
	for(i = 0,j=3;i<5;i++,j++){
		if(str[j] == '1'){
			rate += rates[i];
		}
	}
	return rate/32; //显示的是n*32K
}
/*
*功能:设置上/下行的带宽M
*@rate :out 速率
*@conf :out config[]
*return
*/
void setRunRateK(int rate,unsigned char *conf){
	rate = rate * 32;//显示的是n*32K
	int rates[5]={512,256,128,64,32};
	int i ;
	int pos=3;
	unsigned char setBit = 0x10;
	//从第三位开始
	for( i =0;i<5;i++){
		
		if(rate >=rates[i]){
			rate = rate - rates[i];
			
			(*conf) |= setBit; 
		}else{
			
			(*conf) &= ~setBit; 
		}
		setBit >>= 1;
		
	}
}
/*
*功能：统计指定类型板卡的数量
*@str :in
*@sub :in
*@result :out 输出结果
*return 
*/
void getAmountOfCardType(char *str,char *sub,char *result){
	char subItem[20];		//子串
	int count = 0;			//个数
	int i = 0;
	while(getSubItem(str,subItem,i++)){
		if(!strcmp(subItem,sub)){
			++count;
		}
	}
	//重新设置类型,以输出.用户端须指明卡类型
	/*
	if(!strcmp(sub,"8993")){
		sprintf(sub,"B(8993)");
	}else if(!strcmp(sub,"9301")){
		sprintf(sub,"C(9301)");
	}else if(!strcmp(sub,"9331")){
		sprintf(sub,"F(9331)");
	}else if(!strcmp(sub,"6652")){
		sprintf(sub,"G(6652)");
	}else if(!strcmp(sub,"IP113")){
		sprintf(sub,"H(IP113)");
	}else if(!strcmp(sub,"1000M")){
		sprintf(sub,"I(1000M)");
	}else if(!strcmp(sub,"F3")){
		sprintf(sub,"J(F3)");
	}else if(!strcmp(sub,"IP113S")){
		sprintf(sub,"K(IP113S)");
	}
	*/
	sprintf(result,"%s %d;",sub,count);
}
