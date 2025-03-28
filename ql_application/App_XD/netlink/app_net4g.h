#ifndef _APP_NET4G_H_
#define _APP_NET4G_H_

#include <string.h>
#include <stdio.h>

#include "app_config.h"
#include "pb.h"
#include "wm_crypto_hard.h"

#if 1
#define NET4G_DEBUG(format, arg...) printf("[4G]" format "", ##arg)
#else
#define NET4G_DEBUG(format, arg...)
#endif


/******************************************DATA PARSE INDEX&NUM DEFINE***********************************************/

#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
//AT FOR DUAL CARD

#define  SIM_CMD_TEST               		"AT+LUIMSLOT=?\r\n"           //测试双卡功能
#define  SIM_CMD_CHECK              		"AT+LUIMSLOT?\r\n"            //查询当前使用的SIM卡
#define  SIM_CMD_CHANGE_1           		"AT+LUIMSLOT=1\r\n"           //切换为SIM卡1 
#define  SIM_CMD_CHANGE_2           		"AT+LUIMSLOT=2\r\n"           //切换为SIM卡2 
#define  NET_CMD_RF_OFF             		"AT+CFUN=0\r\n"				  // 关闭射频
#define  NET_CMD_RF_ON              		"AT+CFUN=1\r\n"				  // 打开射频
#define  SIM_AUTO_SELECT_ON         		"AT+LUIMDUAL=1\r\n"           //打开SIM卡优选开关
#define  SIM_AUTO_SELECT_OFF        		"AT+LUIMDUAL=0\r\n"            //关闭SIM卡优选开关
#define  SIM_AUTO_SELECT_CHECK      		"AT+LUIMDUAL?\r\n"            //关闭SIM卡优选开关

// AT CMD DEFINE
#define  NET_CMD_HAND          				"AT\r\n"                      // 握手指令，响应OK
// #define  NET_CMD_GET_MODULE_INFO    		"AT+LGMR=1\r\n"		          // 查询模块版本信息
#define  NET_CMD_GET_MODULE_INFO    		"AT+LGMR\r\n"		          // 查询模块版本信息  
#define  NET_CMD_GET_IMEI       			"AT+CGSN=1\r\n"               // 获取GPRS模块的IMEI码
#define  NET_CMD_GET_SN_NO          		"AT+CGSN=0\r\n"               // 获取sn序列号
#define  NET_CMD_GET_IP             		"AT+CGPADDR\r\n"			  // 读取IP地址
#define  NET_CMD_IS_SIM_OK		    		"AT+CPIN?\r\n"                // 查看是否识别到sim卡
#define  NET_CMD_GET_SIM_NO		    		"AT+LCCID\r\n"                // 查询sim卡卡号
#define  NET_CMD_GET_SIM_STATE      		"AT+CEREG?\r\n"               // 检查网络注册状态，1-已注册，2-正在搜索网络
#define  NET_CMD_GET_NET_STATE      		"AT+CGATT?\r\n"               // 查询 GPRS 是否附着
#define  NET_CMD_GET_NET_RSSI       		"AT+CSQ\r\n"				  // 信号强度
#define  NET_CMD_SET_BAUD       			"AT+IPR=115200\r\n"           // 要改变波特率     OK
#define  NET_CMD_CLOSE_ATE          		"ATE0\r\n"                    // 关闭指令回选功能  OK
#define  NET_CMD_OPEN_ATE           		"ATE1\r\n"                    // 开启指令回选功能  OK

#define  NET_CMD_LOCATION_PARA_GET			"AT+LBSPARA?\r\n"			  //读取定位坐标token
#define  NET_CMD_LOCATION_PARA_SET			"AT+LBSPARA=%s,%s\r\n"		  //设置定位坐标token
#define  NET_CMD_LOCATION					"AT+LBS=0\r\n"				  //读取定位坐标

#define  NET_CMD_GET_CELL	        		"AT+LCELL?\r\n"			  	  // 获取小区信息
#define  NET_CMD_GET_ECBCINFO	        	"AT+ECBCINFO=1\r\n"			  // 获取服务小区和邻近小区信息
// #define  NET_CMD_GET_ENG_SERVINGCELL	    "AT+ECSTATUS\r\n"			  // 获取主服务小区信息

//AT CMD HTTP-GET
// #define  NET_CMD_HTTPGET_URL_TEST    		"http://cmp-net-monitor.xiot.senthink.com/getPushData-2"
#define  NET_CMD_HTTPGET_SET_URL_CMD    	"AT+LHTTPURL=%d,30\r\n"   										//设置http URL指令，%d:url长度， 超时时间30s
#define  NET_CMD_HTTPGET_SET_URL_DATA   	"%s"        			  										//设置http URL 
#define  NET_CMD_HTTPGET_CONNECT_URL    	"AT+LHTTPGET=60\r\n"      										//建立http连接 ,超时时间60s
#define  NET_CMD_HTTPGET_GET_HTTP_RSP   	"AT+LHTTPREAD=60\r\n"         									//请求http数据
// #define  NET_CMD_HTTPGET_GET_HTTP_RSP   	"AT+LHTTPREAD\r\n"         										//请求http数据

#define  NET_CMD_HTTPGET_GET_RSP_RANGE  	"AT+LHTTPGETEX=60,%d,%d\r\n"  									//请求http数据
#define  NET_CMD_HTTPGET_CONFIG_NO_CACHE  	"AT+LHTTPCFG=cachetype,1\r\n" 									// 4g模块不缓存http rsp数据,直接返回，0-缓存,1-不缓存
#define  NET_CMD_HTTPGET_CONFIG_WITH_CACHE  "AT+LHTTPCFG=cachetype,0\r\n" 									// 4g模块缓存http rsp数据,，0-缓存,1-不缓存
#define  NET_CMD_HTTPGET_CONFIG_SSL_CTX_ID  "AT+LHTTPCFG=sslctxid,0\r\n" 									//配置ssl上下文ID

//AT CMD HTTP-POST
#define  NET_CMD_HTTPPOST_SET_URL_CMD    	"AT+LHTTPURL=%d,30\r\n"   										//设置http URL指令，%d:url长度， 超时时间30s
#define  NET_CMD_HTTPPOST_SET_URL_DATA   	"%s"        			  										//设置http URL 
#define  NET_CMD_HTTPPOST_HEADER_CONFIG  	"AT+LHTTPCFG=\"requestheader\",1\r\n" 							//配置POST HEAD  , content-type:application/json
#define  NET_CMD_HTTPPOST_HEADER_CONFIG_UN  "AT+LHTTPCFG=\"requestheader\",0\r\n" 							//配置POST HEAD  , content-type:application/json
#define  NET_CMD_HTTPPOST_SET_BODY_CMD   	"AT+LHTTPPOST=%d,60,60\r\n"  									//POST命令, %d-body长度，，第一个60是此命令设置后等待输入数据体的超时时间为60秒，第二个60为响应超时时间
#define  NET_CMD_HTTPPOST_SET_BODY_DATA  	"%s"        			  										//设置http URL 
#define  NET_CMD_HTTPPOST_GET_HTTP_RSP   	"AT+LHTTPREAD=60\r\n"       									//请求http数据
#define  NET_CMD_HTTP_CLOSE            	 	"AT+LHTTPSTOP\r\n"          									//http断开连接

//AT CMD MQTT CONFIG
#define  NET_CMD_MQTT_DISCONNECT      		"AT+LMQTTDISC=0\r\n"               								//主动断开与mqtt服务器连接， 0=clientId
#define  NET_CMD_MQTT_NET_CLOSE         	"AT+LMQTTCLOSE=0\r\n"              								//关闭MQTT设备端网络
#define  NET_CMD_MQTT_OPEN         			"AT+LMQTTOPEN=0,\"%s\",%d\r\n"    								//clientId,addrHost,port: "AT+QMTOPEN=0,"mq.dian.so",1883\r\n"
//#define  NET_CMD_MQTT_CONFIG_PDP        	"AT+LMQTTCFG=\"pdpcid\",0,1\r\n"          						// PDP
#define  NET_CMD_MQTT_CONFIG_SESSION      	"AT+LMQTTCFG=\"session\",0,1\r\n"         						// Session
#define  NET_CMD_MQTT_TCONFIG_VERSION      	"AT+LMQTTCFG=\"version\",0,4\r\n"         						//0-clientId, 4-mqtt v3.1.1
#define  NET_CMD_MQTT_CONFIG_TIMEOUT      	"AT+LMQTTCFG=\"timeout\",0,%d,%d,0\r\n"   						//MQTT Timeout: clientId, Timeout, RetryTimes, TimeoutNotice
#define  NET_CMD_MQTT_CONFIG_KEEPALIVE    	"AT+LMQTTCFG=\"keepalive\",0,%d\r\n"     						//MQTT Keepalive, clientId, HeartbeatTime
#define  NET_CMD_MQTT_CONFIG_RECMODE 	    "AT+LMQTTCFG=\"recv/mode\",0,0,1\r\n"	   						// 配置服务器数据的接收模式
#define  NET_CMD_MQTT_CONFIG_SENDMODE 	    "AT+LMQTTCFG=\"send/mode\",0,0\r\n"	   							// 配置服务器数据的发送模式
#define  NET_CMD_MQTT_CONNECT         		"AT+LMQTTCONN=0,\"%s\",\"%s\",\"%s\"\r\n" 						// clientId,clientId, userNme,password
#define  NET_CMD_MQTT_SUB_TOPIC 			"AT+LMQTTSUBUNSUB=0,0,1,\"/device/%s/%s/#\",%d\r\n"				//Addedd by y04485	// 订阅主题：clientId
// #define  NET_CMD_MQTT_PUB_MSG 				"AT+LMQTTPUB=0,1,1,0,\"device/%s/%s/%s\",%d,\"%s\"\r\n" 		// MQTT发布，QOS�?RETAIN，主题，消息
#define  NET_CMD_MQTT_PUB_MSG 				"AT+LMQTTPUB=0,1,1,0,\"device/%s/%s/%s\",%d\r\n" 		// MQTT发布，QOS�?RETAIN，主题，消息


// #define  NET_CMD_MQTT_SUB_TOPIC             "AT+LMQTTSUBUNSUB=0,0,1,\"device/in/%s\",%d\r\n"   			  	// 订阅主题：clientId,mgsId,topic,qos
// #define  NET_CMD_MQTT_PUB_MSG         	   "AT+LMQTTPUB=0,1,1,0,\"device/out/%s\",%d,\"%s\"\r\n"         	// MQTT发布，QOS， RETAIN，主题，消息

#define NET_CMD_TOPIC_CONVERT 				"device/%s/%s/%s/%s"										  	//蓝牙通讯topic转换

//AT RSP DEFINE
#define  NET_CMD_ACK_OK             		"OK\r\n"
#define  GET_IMEI_RSP_HEAD  				"+CGSN:"                // 获取GPRS模块的IMEI码
#define  GET_SN_NO_RSP_HEAD 				"+CGSN:"                // 获取sn序列号
#define  GET_IP_RSP_HEAD   					"+CGPADDR:"			    // 读取IP地址
#define  IS_SIM_OK_RSP_HEAD		    		"+CPIN:"                // 查看是否识别到sim卡
#define  IS_SIM_OK_RSP_HEAD_ERR				"+CME ERROR:"			// 读sim卡错误
#define  GET_SIM_NO_RSP_HEAD				"+LCCID:"               // 查询sim卡卡号
#define  GET_SIM_STATE_RSP_HEAD     		"+CEREG:"               // 检查网络注册状态，1-已注册，2-正在搜索网络
#define  GET_NET_STATE_RSP_HEAD     		"+CGATT:"               // 查询 GPRS 是否附着
#define  GET_NET_RSSI_RSP_HEAD      		"+CSQ:"				    // 信号强度

#define  GET_NET_QENG_SERVING_CELL_RSP_HEAD " +ECBCINFO:"			// 主服务小区信息

#define  GET_NET_CELL_RSP_HEAD      	 	"+LCELL:"				// 主服务和邻近小区信息
// #define  GET_NET_CELL_RSP_HEAD      	 	"+ECBCINFOSC:"			// 主服务和邻近小区信息
#define  GET_NET_SERVING_CELL_RSP_HEAD      "+ECBCINFOSC:"			// 主服务小区信息
#define  GET_NET_NEIGHBOURCELL_RSP_HEAD     "+ECBCINFOSC:"			// 邻近小区信息


#define  GET_NET_ECBCINFO_EARFCN            0
#define  GET_NET_ECBCINFO_PCI              	1
#define  GET_NET_ECBCINFO_RSRP             	2
#define  GET_NET_ECBCINFO_RSRQ             	3
#define  GET_NET_ECBCINFO_MCC              	4
#define  GET_NET_ECBCINFO_MNC              	5
#define  GET_NET_ECBCINFO_CELLID            6
#define  GET_NET_ECBCINFO_TAC              	7


#define  GET_NET_LCELL_MCC              	2
#define  GET_NET_LCELL_MCC              	2
#define  GET_NET_LCELL_MNC              	3
#define  GET_NET_LCELL_TAC              	4
#define  GET_NET_LCELL_cellID              	5
#define  GET_NET_LCELL_PCI              	6
#define  GET_NET_LCELL_RX_lev              	7
#define  GET_NET_LCELL_EARFCN              	8
#define  GET_NET_LCELL_RSRP              	9
#define  GET_NET_LCELL_RSRQ              	10
#define  GET_NET_LCELL_SNR              	11

#define  GET_NET_CELL_RSRP              	2
#define  GET_NET_CELL_ID                	6
#define  GET_NET_CELL_TAC               	7



#define  GET_SIM_USING_HEAD          		"+LUIMSLOT:"             //当前使用的SIM卡

#define  NET_CMD_RSP_TAIL_OK      			"OK"					// AT响应的尾符号
#define  NET_CMD_RSP_TAIL_ENTER     		"\r\n"					// AT响应的尾符号
#define  NET_CMD_HTTP_RSP_CONNECT    		"CONNECT"   			//设置http URL指令，%d:url长度， 超时时间30s

#define  NET_ROUTE_ACTIVE_RSP_HEAD       	NULL//"CONNECT"//NULL
#define  NET_ROUTE_ACTIVE_RSP_TAIL       	"\r\nOK"

#define  NET_MQTT_CTRL_CMD_HEAD          	"+LMQTTURC:"
#define  NET_MQTT_CTRL_CMD_TAIL          	"\r\n"

#define  NET_MQTT_DISCONNECT_CMD_HEAD    	"+LMQTTURC: STATS"
#define  NET_MQTT_DISCONNECT_CMD_TAIL    	"\r\n"

#define  NET_MQTT_PUB_MIDDLE_HEAD        	"\r\n> "
#define  NET_MQTT_PUB_MIDDLE_TAIL        	"\r\n> "
#define  NET_MQTT_PUB_RSP_HEAD           	"+LMQTTPUB:"
#define  NET_MQTT_PUB_RSP_TAIL           	"\r\n"




#define  NET4G_CMD_DATA_KEYWORD_INDEX     	5

#define XD_LOCATION_PARA_TOKEN				"1B906A53A8B5C7A621A3018957FEA72D"

typedef enum NET4G_AT_CMD_ID_
{
    CMD_ID_NONE = 0,
	
//AT 4G MODULE CONFIG		
    CMD_ID_HANDSHAKE,                   	//AT握手
    CMD_ID_GET_MODULE_INFO,             	//读取模块型号信息
    CMD_ID_GET_IMEI,                    	//读取模块IMEI号
    CMD_ID_GET_SN_NO,                   	//读取模块SN号
    CMD_ID_GET_IP,                      	//读取模块IP地址
    CMD_ID_IS_SIM_OK,                   	//检测是否有SIM卡
    CMD_ID_GET_SIM_NO,                  	//读取SIM卡卡号
    CMD_ID_GET_SIM_STATE,               	//读取SIM卡注册状态
    CMD_ID_GET_NET_STATE,               	//查看4G网络是否附着
	CMD_ID_GET_RSSI,						//读取信号网络强度
	CMD_ID_SET_CLOSE_ATE,					//
	CMD_ID_SET_OPEN_ATE,
	CMD_ID_SET_BAUD_RATE,
	CMD_ID_GET_LOCATION_PARA,
	CMD_ID_SET_LOCATION_PARA,
	CMD_ID_GET_LOCATION_ADDR,
	CMD_ID_GET_CELL,                   		//获取服务小区和邻区信息
	// CMD_ID_GET_ENG_SERVINGCELL,       		//获取主服务小区信息
	// CMD_ID_GET_QENG_NEIGHBOURCELL,			//获取邻区信息

//AT CMD ACTIVE (HTTP POST/GET)
	CMD_ID_ACTIVE_HTTP_CONFIG_CACHE,
	CMD_ID_ACTIVE_SET_URL_CMD,	   			//设置http URL指令�?d:url长度�?超时时间30s
	CMD_ID_ACTIVE_SET_URL_DATA,	   			//设置http URL
	CMD_ID_ACTIVE_SET_CONNECT_URL, 			//设置http URL
	CMD_ID_ACTIVE_GET_HTTP_RSP,	   			//请求http数据

	// CMD_ID_ACTIVE_HTTP_CONFIG_CACHE,
	// CMD_ID_ACTIVE_POST_SET_URL_CMD,   		//设置http URL指令，%d:url长度， 超时时间30s
	// CMD_ID_ACTIVE_POST_SET_URL_DATA,   		//设置http URL 
	// CMD_ID_ACTIVE_POST_HEADER_CONFIG,  		//配置POST HEAD  ,
	// CMD_ID_ACTIVE_POST_HEADER_CONFIG_UN,  	//配置POST HEAD  ,
	// CMD_ID_ACTIVE_POST_SET_BODY_CMD,  		//POST命令, %d-body长度，，第一个60是此命令设置后等待输入数据体的超时时间为60秒，第二个60为响应超时时间
	// CMD_ID_ACTIVE_POST_SET_BODY_DATA, 		//设置http URL 
	// CMD_ID_ACTIVE_POST_GET_HTTP_RSP,  		//请求http数据

//AT CMD HTTP-GET
	CMD_ID_ACTIVE_CONFIRM_SET_URL_CMD,   	//设置http URL指令，%d:url长度， 超时时间30s
	CMD_ID_ACTIVE_CONFIRM_SET_URL_DATA,   	//设置http URL 
	CMD_ID_ACTIVE_CONFIRM_CONNECT_URL,    	//建立http连接 ,超时时间60s
	CMD_ID_ACTIVE_CONFIRM_GET_HTTP_RSP,   	//请求http数据

//AT CMD ROUTE (HTTP-GET)
	CMD_ID_ROUTE_HTTP_CONFIG_CACHE,
	CMD_ID_ROUTE_HTTP_CONFIG_SSL_CTX_ID,    //设置httpSSL上下文ID
	CMD_ID_ROUTE_SET_URL_CMD,   			//设置http URL指令，%d:url长度， 超时时间30s
	CMD_ID_ROUTE_SET_URL_DATA,   			//设置http URL 
	CMD_ID_ROUTE_CONNECT_URL,    			//建立http连接 ,超时时间60s
	CMD_ID_ROUTE_GET_HTTP_RSP,   			//请求http数据
	CMD_ID_HTTP_CLOSE,        				//http断开连接

//AT CMD MQTT
	CMD_ID_MQTT_DISCONNECT,      			//主动断开与mqtt服务器连接， 0=clientId
	CMD_ID_MQTT_NET_CLOSE,         	        //关闭MQTT设备端网络
	CMD_ID_MQTT_OPEN,         				//clientId,addrHost,port: "AT+QMTOPEN=0,"mq.dian.so",1883\r\n"
	// CMD_ID_MQTT_CONFIG_PDP,        	     	//PDP
	CMD_ID_MQTT_CONFIG_SESSION,      	    //Session
	CMD_ID_MQTT_TCONFIG_VERSION,      	    //0-clientId, 4-mqtt v3.1.1
	CMD_ID_MQTT_CONFIG_TIMEOUT,      		//MQTT Timeout: clientId, Timeout, RetryTimes, TimeoutNotice
	CMD_ID_MQTT_CONFIG_KEEPALIVE,    	    //MQTT Keepalive, clientId, HeartbeatTime
	CMD_ID_MQTT_CONFIG_SENDMODE,
	CMD_ID_MQTT_CONFIG_RECMODE,
	CMD_ID_MQTT_CONNECT,         			//clientId,clientId, userNme,password
	CMD_ID_MQTT_SUB_TOPIC,             	    //订阅主题：clientId,mgsId,topic,qos

//OTA DOWNLOAD (HTTP-GET)
	CMD_ID_OTA_HTTP_CONFIG_NO_CACHE,   		//配置4g模块，http get rsp 立即响应，无缓存 
	CMD_ID_OTA_SET_URL_CMD,   				//设置http URL指令，%d:url长度， 超时时间30s
	CMD_ID_OTA_SET_URL_DATA,   				//设置http URL 
	CMD_ID_OTA_DOWNLOAD_RANGE,    			//建立http连接 ,超时时间60s
	CMD_ID_OTA_GET_HTTP_RSP,   				//请求http数据
	CMD_ID_OTA_HTTP_CLOSE,        			//http断开连接

//DUAL SIM CARD
	CMD_SIM_TEST,							//测试是否支持双卡功能
	CMD_SIM_CHECK,							//查询当前使用的SIM卡
	CMD_SIM_CHANGE_1,						//切换到SIM卡1 
	CMD_SIM_CHANGE_2,						//切换到SIM卡2 
	CMD_ID_RF_ON,							//打开模组射频
	CMD_ID_RF_OFF,							//关闭模组射频
	CMD_SIM_AUTO_SELECT_ON, 				//打开SIM卡优选开关
	CMD_SIM_AUTO_SELECT_OFF,				//关闭SIM卡优选开关
	CMD_SIM_AUTO_SELECT_CHECK,				//查询SIM卡优选开关状态



    CMD_ID_SUCCESS,                     	//成功
    CMD_ID_ERROR                        	//错误
}NET4G_AT_CMD_ID_E;


#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))

//AT FOR DUAL CARD
#define NET_CMD_CARD_CHECK        "AT+QDSIM?\r\n"                //查询当前使用的 USIM 卡槽
#define NET_CMD_CARD_SIM1         "AT+QDSIM=0\r\n"               //切换 USIM 卡至卡槽 1
#define NET_CMD_CARD_SIM2         "AT+QDSIM=1\r\n"               //切换 USIM 卡至卡槽 2


// AT CMD DEFINE
#define  NET_CMD_HAND          		"AT\r\n"                      // 握手指令，响应OK
#define  NET_CMD_GET_MODULE_INFO    "AT+CGMR\r\n"		          // 查询模块版本信息  
#define  NET_CMD_GET_IMEI       	"AT+CGSN\r\n"                 // 获取GPRS模块的IMEI码
#define  NET_CMD_GET_SN_NO          "AT+EGMR=0,5\r\n"             	// 获取sn序列号
#define  NET_CMD_GET_IP             "AT+CGPADDR\r\n"			  // 读取IP地址
#define  NET_CMD_IS_SIM_OK		    "AT+CPIN?\r\n"                // 查看是否识别到sim卡
#define  NET_CMD_GET_SIM_NO		    "AT+QCCID\r\n"                // 查询sim卡卡号
#define  NET_CMD_GET_SIM_STATE      "AT+CEREG?\r\n"               // 检查网络注册状态，1-已注册，2-正在搜索网络
#define  NET_CMD_GET_NET_STATE      "AT+CGATT?\r\n"               // 查询 GPRS 是否附着
#define  NET_CMD_GET_NET_RSSI       "AT+CSQ\r\n"				  // 信号强度
#define  NET_CMD_SET_BAUD       	"AT+IPR=115200\r\n"           // 要改变波特率     OK
#define  NET_CMD_CLOSE_ATE          "ATE0\r\n"                    // 关闭指令回选功能  OK
#define  NET_CMD_OPEN_ATE           "ATE1\r\n"                    // 开启指令回选功能  OK

//锁基站
#if 0
#define  NET_CMD_SET_CFUN           "AT+CFUN=0\r\n"				  // 锁基站指令
#define  NET_CMD_SET_ECFREQ         "AT+ECFREQ=2,38400,105\r\n"   // 锁基站指令864775062031671
#define  NET_CMD_SET_ECFREQ         "AT+ECFREQ=2,41134,312\r\n"   // 锁基站指令864775062002771
#endif
#define  NET_CMD_RF_OFF          					"AT+CFUN=0\r\n"				  										//关闭射频
#define  NET_CMD_RF_ON           					"AT+CFUN=1\r\n"				  										//打开射频
#define  NET_CMD_MIN_LEV         					"AT+ECCFG=\"qrxlevmin\",-105\r\n"    								//配置小区门限值
#define  NET_CMD_FENCE           					"AT+QBLACKCELLCFGEX=1,65535,1,600,1,5,5,600\r\n"					//配置电子围栏

#define  NET_CMD_LOCATION_PARA_GET					"AT+QLBSCFG=\"token\"\r\n"	  										//读取定位坐标token
#define  NET_CMD_LOCATION_PARA_SET					"AT+QLBSCFG=\"token\",\"%s\"\r\n"									//设置定位坐标token
#define  NET_CMD_LOCATION							"AT+QLBS\r\n"				  										//定位坐标

//SET OVERSEA APN
#define  NET_CMD_SET_APN							"AT+QICSGP=1,1,\"orange.m2m.spec\",\"\",\"\",1\r\n"					//配置APNorange.m2m.spec

#define  NET_CMD_GET_QCELL	        				"AT+QCELL?\r\n"			  											//获取主服务和邻近小区信息
#define  NET_CMD_GET_QENG_SERVINGCELL	        	"AT+QENG=\"servingcell\"\r\n"										//获取主服务小区信息(RSRP)
#define  NET_CMD_GET_QENG_NEIGHBOURCELL	        	"AT+QENG=\"neighbourcell\"\r\n"										//获取邻近小区信息
#define  NET_CMD_SET_PDP           					"AT+QIACT=1\r\n"      												//激活PDP上下文
#define  NET_CMD_IP_CONFIG         					"AT+QIDNSCFG=1,\"114.114.114.114\",\"8.8.8.8\"\r\n"					//配置域名服务器和IP

//AT CMD HTTP-GET
#define  NET_CMD_HTTPGET_SET_URL_CMD    			"AT+QHTTPURL=%d,30\r\n"   											//设置http URL指令，%d:url长度， 超时时间30s
#define  NET_CMD_HTTPGET_SET_URL_DATA   			"%s"        			  											//设置http URL 
#define  NET_CMD_HTTPGET_CONNECT_URL    			"AT+QHTTPGET=60\r\n"     											//建立http连接 ,超时时间60s
#define  NET_CMD_HTTPGET_GET_HTTP_RSP   			"AT+QHTTPREAD=60\r\n"         										//请求http数据
#define  NET_CMD_HTTPGET_GET_RSP_RANGE  			"AT+QHTTPGETEX=60,%d,%d\r\n"  										//请求http数据

//AT CMD HTTP-POST
#define  NET_CMD_HTTPPOST_SET_URL_CMD    			"AT+QHTTPURL=%d,30\r\n"   											//设置http URL指令，%d:url长度， 超时时间30s
#define  NET_CMD_HTTPPOST_SET_URL_DATA   			"%s"        			  											//设置http URL 
#define  NET_CMD_HTTPPOST_HEADER_CONFIG  			"AT+QHTTPCFG=\"header\",\"Content-type: application/protobuf\"\r\n" //"AT+QHTTPCFG=\"contenttype\",4\r\n"//配置POST HEAD  , content-type:application/json
#define  NET_CMD_HTTPPOST_SET_BODY_CMD   			"AT+QHTTPPOST=%d,60,60\r\n"  										//POST命令, %d-body长度，，第一个60是此命令设置后等待输入数据体的超时时间为60秒，第二个60为响应超时时间
#define  NET_CMD_HTTPPOST_SET_BODY_DATA  			"%s"        			  											//设置http URL 
#define  NET_CMD_HTTPPOST_GET_HTTP_RSP   			"AT+QHTTPREAD=60\r\n"       										//请求http数据
#define  NET_CMD_HTTP_CLOSE            	 			"AT+HTTPCLOSE\r\n"          										//http断开连接

//AT CMD HTTP-OTA
#define  NET_CMD_HTTPOTA_CONFIG          			"AT+QHTTPCFG=\"rspout/auto\",1\r\n"  								//关闭输出 HTTPS 响应头信息。


//AT CMD MQTT CONFIG
#define  NET_CMD_MQTT_DISCONNECT      				"AT+QMTDISC=0\r\n"               									//主动断开与mqtt服务器连接， 0=clientId
#define  NET_CMD_MQTT_NET_CLOSE         			"AT+QMTCLOSE=0\r\n"              									//关闭MQTT设备端网络
#define  NET_CMD_MQTT_OPEN         					"AT+QMTOPEN=0,\"%s\",%d\r\n"    									//clientId,addrHost,port: "AT+QMTOPEN=0,"mq.dian.so",1883\r\n"
#define  NET_CMD_MQTT_CONFIG_PDP        			"AT+QMTCFG=\"pdpcid\",0,1\r\n"          							// PDP
#define  NET_CMD_MQTT_CONFIG_SESSION      			"AT+QMTCFG=\"session\",0,1\r\n"         							// Session
#define  NET_CMD_MQTT_TCONFIG_VERSION      			"AT+QMTCFG=\"version\",0,4\r\n"         							//0-clientId, 4-mqtt v3.1.1
#define  NET_CMD_MQTT_CONFIG_TIMEOUT      			"AT+QMTCFG=\"timeout\",0,%d,%d,0\r\n"   							//MQTT Timeout: clientId, Timeout, RetryTimes, TimeoutNotice
#define  NET_CMD_MQTT_CONFIG_KEEPALIVE    			"AT+QMTCFG=\"keepalive\",0,%d\r\n"     								//MQTT Keepalive, clientId, HeartbeatTime
#define  NET_CMD_MQTT_CONFIG_RECMODE 	    		"AT+QMTCFG=\"recv/mode\",0,0,1\r\n"	   								// 配置服务器数据的接收模式
#define  NET_CMD_MQTT_CONNECT         				"AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n" 							// clientId,clientId, userNme,password
#define  NET_CMD_MQTT_SUB_TOPIC 					"AT+QMTSUB=0,1,\"/device/%s/%s/#\",%d\r\n"							//Addedd by y04485	// 订阅主题：clientId
#define  NET_CMD_MQTT_PUB_MSG 						"AT+QMTPUBEX=0,1,1,0,\"device/%s/%s/%s\",%d\r\n" 					// MQTT发布，QOS�?RETAIN，主题，消息

//#define  NET_CMD_MQTT_SUB_TOPIC           		"AT+QMTSUB=0,1,\"device/in/%s\",%d\r\n"   							// 订阅主题：clientId,mgsId,topic,qos
//#define  NET_CMD_MQTT_PUB_MSG         			"AT+QMTPUBEX=0,1,1,0,\"device/out/%s\",%d\r\n" 						// MQTT发布，QOS， RETAIN，主题，消息



#define NET_CMD_TOPIC_CONVERT 						"device/%s/%s/%s/%s"												//蓝牙通讯topic转换



//AT RSP DEFINE
#define  NET_CMD_ACK_OK             	 			 "OK\r\n"
#define  GET_IMEI_RSP_HEAD  			 			 NULL                	// 获取GPRS模块的IMEI码
#define  GET_SN_NO_RSP_HEAD 			 			 "+EGMR:"               // 获取sn序列号
#define  GET_IP_RSP_HEAD   				 			 "+CGPADDR:"			// 读取IP地址
#define  IS_SIM_OK_RSP_HEAD		    	 			 "+CPIN:"               // 查看是否识别到sim卡
#define  IS_SIM_OK_RSP_HEAD_ERR			 			 "+CME ERROR:"			// 读sim卡错误
#define  GET_SIM_NO_RSP_HEAD			 			 "+QCCID:"              // 查询sim卡卡号
#define  GET_SIM_STATE_RSP_HEAD     	 			 "+CEREG:"              // 检查网络注册状态，1-已注册，2-正在搜索网络
#define  GET_NET_STATE_RSP_HEAD     	 			 "+CGATT:"              // 查询 GPRS 是否附着
#define  GET_NET_RSSI_RSP_HEAD      	 			 "+CSQ:"				// 信号强度

#define  GET_NET_QCELL_RSP_HEAD      	 			 "+QCELL:"				// 主服务和邻近小区信息
#define  GET_NET_QENG_SERVING_CELL_RSP_HEAD      	 "+QENG:"				// 主服务小区信息
#define  GET_NET_QENG_NEIGHBOURCELL_RSP_HEAD      	 "+QENG:"				// 邻近小区信息


#define  NET_CMD_RSP_TAIL_OK      		 			 "OK"					// AT响应的尾符号
#define  NET_CMD_RSP_TAIL_ENTER     	 			 "\r\n"					// AT响应的尾符号
#define  NET_CMD_HTTP_RSP_CONNECT   	 			 "CONNECT"   			//设置http URL指令，%d:url长度， 超时时间30s

#define  NET_ROUTE_ACTIVE_RSP_HEAD      			 "CONNECT"
#define  NET_ROUTE_ACTIVE_RSP_TAIL      			 "OK"

#define  NET_MQTT_CTRL_CMD_HEAD         			 "+QMTRECV:"
#define  NET_MQTT_CTRL_CMD_TAIL         			 "\r\n"

#define  NET_MQTT_DISCONNECT_CMD_HEAD   			 "+QMTSTAT: 0,1"
#define  NET_MQTT_DISCONNECT_CMD_TAIL   			 "\r\n"

#define  NET_MQTT_PUB_MIDDLE_HEAD       			 "\r\n> "
#define  NET_MQTT_PUB_MIDDLE_TAIL       			 "\r\n> "
#define  NET_MQTT_PUB_RSP_HEAD          			 "+QMTPUBEX:"
#define  NET_MQTT_PUB_RSP_TAIL          			 "\r\n"
#define  GET_SIM_USING_HEAD          				 "+QDSIM: "
//解析基站信息
#define  GET_NET_QENG_servingcell_is_tdd             3
#define  GET_NET_QENG_servingcell_MCC	             4
#define  GET_NET_QENG_servingcell_MNC	             5
#define  GET_NET_QENG_servingcell_cellID	         6
#define  GET_NET_QENG_servingcell_PCI	             7
#define  GET_NET_QENG_servingcell_EARFCN	         8
#define  GET_NET_QENG_servingcell_freq_band_ind	     9
#define  GET_NET_QENG_servingcell_UL_bandwidth	     10
#define  GET_NET_QENG_servingcell_DL_bandwidth	     11
#define  GET_NET_QENG_servingcell_TAC			     12
#define  GET_NET_QENG_servingcell_RSRP			     13
#define  GET_NET_QENG_servingcell_RSRQ			     14
#define  GET_NET_QENG_servingcell_RSSI			     15
#define  GET_NET_QENG_servingcell_SINR			     16
#define  GET_NET_QENG_servingcell_srxlev		     17




#define  GET_NET_QENG_neighbourcell_EARFCN	         2
#define  GET_NET_QENG_neighbourcell_PCI	             3
#define  GET_NET_QENG_neighbourcell_RSRP			 4
#define  GET_NET_QENG_neighbourcell_RSRQ			 5
#define  GET_NET_QENG_neighbourcell_RSSI			 6
#define  GET_NET_QENG_neighbourcell_SINR			 7

#define  GET_NET_QCELL_MCC              			 2
#define  GET_NET_QCELL_MNC              			 3
#define  GET_NET_QCELL_TAC              			 4
#define  GET_NET_QCELL_cellID              			 5
#define  GET_NET_QCELL_PCI              			 6
#define  GET_NET_QCELL_RX_lev              			 7

// #define  GET_NET_CELL_RSRP              			 13
// #define  GET_NET_CELL_ID                			 5
// #define  GET_NET_CELL_TAC               			 4

#define  NET4G_CMD_DATA_KEYWORD_INDEX   			 4

#define XD_LOCATION_PARA_TOKEN						 "0112w9RLs64w6446"

typedef enum NET4G_AT_CMD_ID_
{
    CMD_ID_NONE = 0,
//AT 4G MODULE CONFIG		
    CMD_ID_HANDSHAKE,                   	//AT握手
    
    CMD_ID_RF_OFF,                      	//关闭射频
	CMD_ID_MIN_LEV,                     	//配置最小信号值
	CMD_ID_SET_FENCE,                   	//配置电子围栏
	CMD_ID_RF_ON,                       	//打开射频 
	
    CMD_ID_GET_MODULE_INFO,             	//读取模块型号信息
    CMD_ID_GET_IMEI,                    	//读取模块IMEI号
    CMD_ID_GET_SN_NO,                   	//读取模块SN号
    CMD_ID_GET_IP,                      	//读取模块IP地址
    CMD_ID_IS_SIM_OK,                   	//检测是否有SIM卡
    CMD_ID_GET_SIM_NO,                  	//读取SIM卡卡号
    CMD_ID_GET_SIM_STATE,               	//读取SIM卡注册状态
    CMD_ID_GET_NET_STATE,               	//查看4G网络是否附着
	CMD_ID_GET_RSSI,						//读取信号网络强度
	CMD_ID_SET_CLOSE_ATE,
	CMD_ID_SET_OPEN_ATE,
	CMD_ID_SET_BAUD_RATE,
	CMD_ID_GET_LOCATION_PARA,
	CMD_ID_SET_LOCATION_PARA,
	CMD_ID_GET_LOCATION_ADDR,
	
	CMD_ID_SET_APN,
	CMD_ID_GET_QCELL,                   	//获取服务小区和邻区信息
	CMD_ID_GET_QENG_SERVINGCELL,       		//获取主服务小区信息
	CMD_ID_GET_QENG_NEIGHBOURCELL,			//获取邻区信息
	CMD_ID_SET_PDP,                   		//激活 PDP 场景
	CMD_ID_SET_DNS,                  		//配置 DNS 服务器地址
	
#if 0//设定基站信息	
	CMD_ID_SET_CFUN,
	CMD_ID_SET_ECFREQ,
#endif

//AT CMD ACTIVE (HTTP POST/GET)
	CMD_ID_ACTIVE_SET_URL_CMD,	   			//设置http URL指令�?d:url长度�?超时时间30s
	CMD_ID_ACTIVE_SET_URL_DATA,	   			//设置http URL
	CMD_ID_ACTIVE_SET_CONNECT_URL, 			//设置http URL
	CMD_ID_ACTIVE_GET_HTTP_RSP,	   			//请求http数据
	
//	CMD_ID_ACTIVE_POST_SET_URL_CMD,   		//设置http URL指令，%d:url长度， 超时时间30s
//	CMD_ID_ACTIVE_POST_SET_URL_DATA,   		//设置http URL 
//	CMD_ID_ACTIVE_POST_HEADER_CONFIG,  		//配置POST HEAD  , content-type:application/json
//	CMD_ID_ACTIVE_POST_SET_BODY_CMD,  		//POST命令, %d-body长度，，第一个60是此命令设置后等待输入数据体的超时时间为60秒，第二个60为响应超时时间
//	CMD_ID_ACTIVE_POST_SET_BODY_DATA, 		//设置http URL 
//	CMD_ID_ACTIVE_POST_GET_HTTP_RSP,  		//请求http数据

	//AT CMD HTTP-GET
	CMD_ID_ACTIVE_CONFIRM_SET_URL_CMD,   	//设置http URL指令，%d:url长度， 超时时间30s
	CMD_ID_ACTIVE_CONFIRM_SET_URL_DATA,   	//设置http URL 
	CMD_ID_ACTIVE_CONFIRM_CONNECT_URL,    	//建立http连接 ,超时时间60s
	CMD_ID_ACTIVE_CONFIRM_GET_HTTP_RSP,   	//请求http数据
//AT CMD ROUTE (HTTP-GET)
	CMD_ID_ROUTE_SET_URL_CMD,   			//设置http URL指令，%d:url长度， 超时时间30s
	CMD_ID_ROUTE_SET_URL_DATA,   			//设置http URL 
	CMD_ID_ROUTE_CONNECT_URL,    			//建立http连接 ,超时时间60s
	CMD_ID_ROUTE_GET_HTTP_RSP,   			//请求http数据
	CMD_ID_HTTP_CLOSE,        				//http断开连接


//AT CMD MQTT
	CMD_ID_MQTT_DISCONNECT,      			//主动断开与mqtt服务器连接， 0=clientId
	CMD_ID_MQTT_NET_CLOSE,         	        //关闭MQTT设备端网络
	CMD_ID_MQTT_OPEN,         				//clientId,addrHost,port: "AT+QMTOPEN=0,"mq.dian.so",1883\r\n"
	CMD_ID_MQTT_CONFIG_PDP,        	     	//PDP
	CMD_ID_MQTT_CONFIG_SESSION,      	    //Session
	CMD_ID_MQTT_TCONFIG_VERSION,      	    //0-clientId, 4-mqtt v3.1.1
	CMD_ID_MQTT_CONFIG_TIMEOUT,      		//MQTT Timeout: clientId, Timeout, RetryTimes, TimeoutNotice
	CMD_ID_MQTT_CONFIG_KEEPALIVE,    	    //MQTT Keepalive, clientId, HeartbeatTime
	CMD_ID_MQTT_CONFIG_RECMODE,
	CMD_ID_MQTT_CONNECT,         			//clientId,clientId, userNme,password
	CMD_ID_MQTT_SUB_TOPIC,             	    //订阅主题：clientId,mgsId,topic,qos

//OTA DOWNLOAD (HTTP-GET)
	CMD_ID_OTA_HTTP_CONFIG,   	        	//配置4g模块，启用输出 HTTPS 响应头信息。
	CMD_ID_OTA_SET_URL_CMD,   				//设置http URL指令，%d:url长度， 超时时间30s
	CMD_ID_OTA_SET_URL_DATA,   				//设置http URL 
	CMD_ID_OTA_DOWNLOAD_RANGE,    			//建立http连接 ,超时时间60s
	CMD_ID_OTA_GET_HTTP_RSP,   				//请求http数据
	CMD_ID_OTA_HTTP_CLOSE,        			//http断开连接
//DUAL SIM_CARD
    CMD_SIM_CHECK,                      	//查询当前使用的SIM卡
	CMD_SIM_CHANGE_1,						//切换到SIM卡1 
	CMD_SIM_CHANGE_2,	                	//切换到SIM卡2 


    CMD_ID_SUCCESS,                     	//成功
    CMD_ID_ERROR                        	//错误
}NET4G_AT_CMD_ID_E;

#endif



typedef enum
{
	ACTIVE_ROUTE_STEP_CHECK = 0,
	ACTIVE_ROUTE_STEP_ACTIVE,
	ACTIVE_ROUTE_STEP_ROUTE,
	ACTIVE_ROUTE_STEP_END
} NET4G_ACTIVE_ROUTE_STEP;




typedef struct 
{
    uint8_t step;               // current step
    NET4G_AT_CMD_ID_E id;       // current cmd id
    uint8_t s_jump;             // jump to when succeed
    uint8_t f_jump;             // jump to when failed
}net4g_at_step_t;


typedef AT_RET_E (*at_cmd_handler_t)(AT_RET_E ret,uint8_t *data, uint32_t len);

typedef struct
{
    NET4G_AT_CMD_ID_E state;					
    uint16_t interval_ms;			//AT指令重发间隔
    uint8_t retry_times;			//AT指令重发次数
    char *p_cmd;					//AT指令命令码				
    char *p_ack;					//AT指令应答
}at_cmd_t;


#if (AES128_ECB_HARD == 1)
extern psCipherContext_t ctx;
#endif


void init_net4g_work_steps(void);
void set_net4g_work_step(NETWROK_WORK_STEP_E targetStep);
NETWROK_WORK_STEP_E get_net4g_work_step(void);

s16 net4g_uart_recv_cb(u16 len);
PROCESS_STEP_RET_E module_4g_config_para_process(void);
PROCESS_STEP_RET_E module_4g_active_route_process(void);
PROCESS_STEP_RET_E module_4g_cloud_logIn_process(void);
PROCESS_STEP_RET_E module_4g_communication_process(void);
PROCESS_STEP_RET_E module_4g_error_handle_process(void);
PROCESS_STEP_RET_E device_reboot_module_4g_communication_process(void);


uint8_t get_atcmd_rsp_value(uint8_t* srcData, uint32_t srcLen, uint8_t* headKey, uint8_t* tailKey, uint8_t* destData);

uint8_t* get_location_addr(void);

uint8_t recv_cmd_from_fifo(uint8_t* keyHeader, uint8_t* keyTail, uint8_t* destCmdData);
uint8_t get_at_rsp_keywords(uint8_t* srcData, uint8_t* srcHeader,uint8_t keyNum, uint8_t* destData);

//void save_pub_msg_to_queue(uint8_t* sign, uint8_t* data);

void save_pub_msg_to_queue(uint8_t* topic1,uint8_t* pb_data,uint16_t pb_len);

PROCESS_STEP_RET_E net4G_work_process(void);

#endif


