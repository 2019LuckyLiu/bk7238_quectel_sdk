#include "app_config.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "diot_pb.h"
#include "app_pb.h"
#include "route_pb.h"
#include "app_common.h"

uint8_t g_payload[500] = {0};
netWork_process_step_t net4G_process_steps = {0};

extern routeRsp  gstRouteRsp;

#if (AES128_ECB_HARD == 1)
psCipherContext_t ctx;
#endif


// uint8_t g_payload1[0x2000] = {0};
// uint8_t *buf1 = NULL;
/****************************************************************************************************************************************
*******************************USE lierda NT26E NT26K 4G MODULE
****************************************************************************************************************************************/

#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))

// 4g module at cmd tables 
const at_cmd_t module_4g_cmds[] = 
{
	//命令ID         	     	 			重发间隔        重发次数        AT命令码            		   			AT应答码 
	{CMD_ID_NONE,							0,			0,		NULL,								NULL},
	
// net config cmd  
	{CMD_ID_HANDSHAKE,    					500,     	30,     NET_CMD_HAND,    					NET_CMD_ACK_OK},//最大响应时间15S
    {CMD_ID_GET_MODULE_INFO,    			100,     	3,      NET_CMD_GET_MODULE_INFO,   			NET_CMD_ACK_OK},//最大响应时间5S
    {CMD_ID_GET_IMEI,       				100,     	3,      NET_CMD_GET_IMEI,     				NET_CMD_ACK_OK},//最大响应时间5S
    {CMD_ID_GET_SN_NO,    	    			100,     	3,      NET_CMD_GET_SN_NO,    				NET_CMD_ACK_OK},//最大响应时间5S
    {CMD_ID_GET_IP,           				100,        30,     NET_CMD_GET_IP,        				NET_CMD_ACK_OK},//最大响应时间5S
    {CMD_ID_IS_SIM_OK,          			100,        30,     NET_CMD_IS_SIM_OK,         			NET_CMD_ACK_OK},//最大响应时间25S 
    {CMD_ID_GET_SIM_NO,         			100,        30,     NET_CMD_GET_SIM_NO,        		 	NET_CMD_ACK_OK},//最大响应时间5S 
	{CMD_ID_GET_SIM_STATE,      			100,     	30,     NET_CMD_GET_SIM_STATE,     			NET_CMD_ACK_OK},//最大响应时间5S
	{CMD_ID_GET_NET_STATE,      			100,     	30,     NET_CMD_GET_NET_STATE,     			NET_CMD_ACK_OK},//最大响应时间75S
	{CMD_ID_GET_RSSI,       				100,     	3,      NET_CMD_GET_NET_RSSI,     			NET_CMD_ACK_OK},//最大响应时间5S
	{CMD_ID_SET_CLOSE_ATE,      			100,     	3,      NET_CMD_CLOSE_ATE,     				NET_CMD_ACK_OK},//最大响应时间5S
	{CMD_ID_SET_OPEN_ATE,      				100,     	3,      NET_CMD_OPEN_ATE,    				NET_CMD_ACK_OK},//最大响应时间5S
	{CMD_ID_SET_BAUD_RATE,      			100,     	3,      NET_CMD_SET_BAUD,     				NET_CMD_ACK_OK},//最大响应时间5S
	{CMD_ID_GET_LOCATION_PARA,      		100,     	1,      NET_CMD_LOCATION_PARA_GET,     		NET_CMD_ACK_OK},//最大响应时间5S
	{CMD_ID_SET_LOCATION_PARA,      		100,     	1,      NET_CMD_LOCATION_PARA_SET,     		NET_CMD_ACK_OK},//最大响应时间5S
	{CMD_ID_GET_LOCATION_ADDR,      		1000,     	1,      NET_CMD_LOCATION,     				NET_CMD_ACK_OK},//最大响应时间5S

	{CMD_ID_GET_CELL,      					1000,     	1,      NET_CMD_GET_CELL,     				NET_CMD_ACK_OK},//最大响应时间305S
	// {CMD_ID_GET_ENG_SERVINGCELL,      		2000,     	1,      NET_CMD_GET_ENG_SERVINGCELL,     	NET_CMD_ACK_OK},//最大响应时间305S

// device active (http-post)
	{CMD_ID_ACTIVE_HTTP_CONFIG_CACHE,   	100,     	3,      NET_CMD_HTTPGET_CONFIG_WITH_CACHE,  NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_SET_URL_CMD,    			500,     	3,      NET_CMD_HTTPPOST_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	{CMD_ID_ACTIVE_SET_URL_DATA,   			500,     	3,      NET_CMD_HTTPPOST_SET_URL_DATA,     	NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_SET_CONNECT_URL,  		1000,     	3,      NET_CMD_HTTPPOST_SET_BODY_DATA,     "+LHTTPGET: "},//NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_GET_HTTP_RSP,   			500,     	3,      NET_CMD_HTTPPOST_GET_HTTP_RSP,      NET_CMD_ACK_OK},

	// {CMD_ID_ACTIVE_HTTP_CONFIG_CACHE,   	100,     	3,      NET_CMD_HTTPGET_CONFIG_WITH_CACHE,  NET_CMD_ACK_OK},
	// {CMD_ID_ACTIVE_POST_SET_URL_CMD,    	500,     	3,      NET_CMD_HTTPPOST_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	// {CMD_ID_ACTIVE_POST_SET_URL_DATA,   	500,     	3,      NET_CMD_HTTPPOST_SET_URL_DATA,     	NET_CMD_ACK_OK},
	// {CMD_ID_ACTIVE_POST_HEADER_CONFIG,  	100,     	3,      NET_CMD_HTTPPOST_HEADER_CONFIG,     NET_CMD_ACK_OK},
	// {CMD_ID_ACTIVE_POST_HEADER_CONFIG_UN,  	100,     	3,      NET_CMD_HTTPPOST_HEADER_CONFIG_UN,  NET_CMD_ACK_OK},
	// {CMD_ID_ACTIVE_POST_SET_BODY_CMD,   	1000,     	3,      NET_CMD_HTTPPOST_SET_BODY_CMD,      NET_CMD_HTTP_RSP_CONNECT},
	// {CMD_ID_ACTIVE_POST_SET_BODY_DATA,  	500,     	3,      NET_CMD_HTTPPOST_SET_BODY_DATA,     NET_CMD_ACK_OK},
	// {CMD_ID_ACTIVE_POST_GET_HTTP_RSP,   	2000,     	3,      NET_CMD_HTTPPOST_GET_HTTP_RSP,     	NET_CMD_ACK_OK},
	
//active confirm (http-get)
	{CMD_ID_ACTIVE_CONFIRM_SET_URL_CMD, 	500,     	3,      NET_CMD_HTTPGET_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	{CMD_ID_ACTIVE_CONFIRM_SET_URL_DATA,	500,     	3,      NET_CMD_HTTPGET_SET_URL_DATA,     	NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_CONFIRM_CONNECT_URL, 	500,     	3,      NET_CMD_HTTPGET_CONNECT_URL,     	"+LHTTPGET: "},//NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_CONFIRM_GET_HTTP_RSP,	500,     	3,      NET_CMD_HTTPGET_GET_HTTP_RSP,     	NET_CMD_ACK_OK},

// devcie route (http-get)
	{CMD_ID_ROUTE_HTTP_CONFIG_CACHE,    	100,     	3,      NET_CMD_HTTPGET_CONFIG_WITH_CACHE,  NET_CMD_ACK_OK},
    {CMD_ID_ROUTE_HTTP_CONFIG_SSL_CTX_ID, 	1000,     	3,      NET_CMD_HTTPGET_CONFIG_SSL_CTX_ID,  NET_CMD_ACK_OK},
	{CMD_ID_ROUTE_SET_URL_CMD,      		500,     	3,      NET_CMD_HTTPGET_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	{CMD_ID_ROUTE_SET_URL_DATA,     		500,     	3,      NET_CMD_HTTPGET_SET_URL_DATA,     	NET_CMD_ACK_OK},
	{CMD_ID_ROUTE_CONNECT_URL,      		3000,     	3,      NET_CMD_HTTPGET_CONNECT_URL,     	"+LHTTPGET: "},//NET_CMD_ACK_OK},
	{CMD_ID_ROUTE_GET_HTTP_RSP,     		1000,     	3,      NET_CMD_HTTPGET_GET_HTTP_RSP,     	NET_CMD_ACK_OK},
	{CMD_ID_HTTP_CLOSE,      				500,     	3,      NET_CMD_HTTP_CLOSE,     			NET_CMD_ACK_OK},

// mqtt config
	{CMD_ID_MQTT_DISCONNECT,      			500,     	2,      NET_CMD_MQTT_DISCONNECT,     		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_NET_CLOSE,      			100,     	1,      NET_CMD_MQTT_NET_CLOSE,     		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_OPEN,      				500,     	3,      NET_CMD_MQTT_OPEN,     				"+LMQTTOPEN: "},
	// {CMD_ID_MQTT_CONFIG_PDP,      			100,     	3,      NET_CMD_MQTT_CONFIG_PDP,     		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_SESSION,      		100,     	3,      NET_CMD_MQTT_CONFIG_SESSION,     	NET_CMD_ACK_OK},
	{CMD_ID_MQTT_TCONFIG_VERSION,      		100,     	3,      NET_CMD_MQTT_TCONFIG_VERSION,     	NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_TIMEOUT,      		100,     	3,      NET_CMD_MQTT_CONFIG_TIMEOUT,     	NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_KEEPALIVE,      	100,     	3,      NET_CMD_MQTT_CONFIG_KEEPALIVE,     	NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_SENDMODE, 			100, 		3, 		NET_CMD_MQTT_CONFIG_SENDMODE, 		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_RECMODE, 			100, 		3, 		NET_CMD_MQTT_CONFIG_RECMODE, 		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONNECT,      				3000,     	3,      NET_CMD_MQTT_CONNECT,     			"+LMQTTCONN: "},//NET_CMD_ACK_OK},//"+LMQTTCONN: 0,0,0"},
	{CMD_ID_MQTT_SUB_TOPIC,      			1000,     	3,      NET_CMD_MQTT_SUB_TOPIC,     		"+LMQTTSUBUNSUB: 0,1,0,1"},

// FW OTA (http-get)
	{CMD_ID_OTA_HTTP_CONFIG_NO_CACHE,   	100,     	3,      NET_CMD_HTTPGET_CONFIG_NO_CACHE,    NET_CMD_ACK_OK},
	{CMD_ID_OTA_SET_URL_CMD,      			200,     	10,     NET_CMD_HTTPGET_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	{CMD_ID_OTA_SET_URL_DATA,     			200,     	10,     NET_CMD_HTTPGET_SET_URL_DATA,     	NET_CMD_ACK_OK},
	{CMD_ID_OTA_DOWNLOAD_RANGE,      		1000,     	1,      NET_CMD_HTTPGET_GET_RSP_RANGE,     	NET_CMD_ACK_OK},
	{CMD_ID_OTA_GET_HTTP_RSP,     			1000,       1,      NET_CMD_HTTPGET_GET_HTTP_RSP,     	NET_CMD_ACK_OK},
	{CMD_ID_OTA_HTTP_CLOSE,      			500,     	3,      NET_CMD_HTTP_CLOSE,     			NET_CMD_ACK_OK},

//dual sim card cmd
	{CMD_SIM_TEST,							1000,		3,	   SIM_CMD_TEST,						NET_CMD_ACK_OK},
	{CMD_SIM_CHECK, 						1000,		3,	   SIM_CMD_CHECK,						NET_CMD_ACK_OK},
	{CMD_SIM_CHANGE_1,						1000,		3,	   SIM_CMD_CHANGE_1,					NET_CMD_ACK_OK},
	{CMD_SIM_CHANGE_2,						1000,		3,	   SIM_CMD_CHANGE_2,					NET_CMD_ACK_OK},
	{CMD_ID_RF_ON,							1000,		3,	   NET_CMD_RF_ON,						NET_CMD_ACK_OK},
	{CMD_ID_RF_OFF, 						1000,		3,	   NET_CMD_RF_OFF,						NET_CMD_ACK_OK},
	{CMD_SIM_AUTO_SELECT_ON,				1000,		3,	   SIM_AUTO_SELECT_ON,					NET_CMD_ACK_OK},
	{CMD_SIM_AUTO_SELECT_OFF,				1000,		3,	   SIM_AUTO_SELECT_OFF, 				NET_CMD_ACK_OK},
	{CMD_SIM_AUTO_SELECT_CHECK, 			1000,		3,	   SIM_AUTO_SELECT_CHECK,				NET_CMD_ACK_OK},

};


static net4g_at_step_t net4g_config_command_steps[] =
{
     //step         id                success  fail
    { 0,	CMD_ID_HANDSHAKE,           1,	0  },
    { 1,	CMD_ID_SET_CLOSE_ATE,		2,	2  },
    { 2,	CMD_ID_GET_MODULE_INFO,     3,	33 },
    { 3,	CMD_ID_GET_IMEI,		    4,	33 },
    { 4,	CMD_ID_GET_SN_NO,           5,	33 },
    //上电先确认SIM2状态
    { 5,	CMD_ID_RF_OFF,        	    6,	33 },   
    { 6,	CMD_SIM_CHANGE_2,	        7,	7  },
	{ 7,	CMD_ID_RF_ON,	            8,	33 },
    { 8,	CMD_SIM_CHECK,		        9,	12 },
	{ 9,	CMD_ID_GET_SIM_NO, 	        10,	12 },
    { 10,	CMD_ID_GET_SIM_STATE,       11,	12 },
    { 11,	CMD_ID_GET_NET_STATE,       12,	12 },
    //确认SIM1状态，如果正常直接做业务
    { 12,	CMD_ID_RF_OFF,        	    13,	33 },   
    { 13,	CMD_SIM_CHANGE_1,	        14,	14 },
	{ 14,	CMD_ID_RF_ON,	            15,	33 },
    { 15,	CMD_SIM_CHECK,		        16,	19 },
	{ 16,	CMD_ID_GET_SIM_NO, 	        17,	19 },
    { 17,	CMD_ID_GET_SIM_STATE,       18,	19 },
    { 18,	CMD_ID_GET_NET_STATE,       26,	19 },
    //SIM1 状态异常，切回SIM2做业务
    { 19,	CMD_ID_RF_OFF,        	    20,	33 },   
    { 20,	CMD_SIM_CHANGE_2,	        21,	21 },
	{ 21,	CMD_ID_RF_ON,	            22,	33 },
    { 22,	CMD_SIM_CHECK,		        23,	33 },
	{ 23,	CMD_ID_GET_SIM_NO, 	        24,	33 },
    { 24,	CMD_ID_GET_SIM_STATE,       25,	33 },
    { 25,	CMD_ID_GET_NET_STATE,       26,	33 },

    { 26,	CMD_ID_MQTT_DISCONNECT,  	30,	30 },
	{ 27,	CMD_ID_GET_LOCATION_PARA,   29,	28 },
	{ 28,	CMD_ID_SET_LOCATION_PARA,   29,	29 },
    { 29,	CMD_ID_GET_LOCATION_ADDR,   30,	30 },
	
	{ 30,	CMD_ID_GET_CELL,       		31,	31 },
	// { 31,	CMD_ID_GET_ENG_SERVINGCELL, 32,	32 },
	
	{ 31,   CMD_ID_GET_RSSI,       		32,	33 },
	
    { 32,	CMD_ID_SUCCESS,     		32,	32 },
    { 33,	CMD_ID_ERROR,       		33,	33 }
};


//net config at cmd steps
static net4g_at_step_t net4g_get_cell_info_config_command_steps[] =
{
    // step         id                success  fail
	{ 0,	CMD_ID_GET_CELL,    			1,	1 },	//获取主服务小区和邻近小区信息
	{ 1,	CMD_ID_GET_RSSI,   				2,	3 },	//获取信号强度
    { 2,	CMD_ID_SUCCESS,     			2,	2 },
    { 3,	CMD_ID_ERROR,       			3,	3 }
};


// static net4g_at_step_t device_active_steps[] =
// {
//     // step         id                          success  fail
//     { 0,		CMD_ID_ACTIVE_HTTP_CONFIG_CACHE,    	1,	14  },
//     { 1,		CMD_ID_ROUTE_HTTP_CONFIG_SSL_CTX_ID,    2,	14  },
//     { 2,		CMD_ID_ACTIVE_POST_SET_URL_CMD,    		3,	14  },
//     { 3,		CMD_ID_ACTIVE_POST_SET_URL_DATA,		4,	14  },
//     { 4,		CMD_ID_ACTIVE_POST_HEADER_CONFIG,  		5,	14  },
//     { 5,		CMD_ID_ACTIVE_POST_SET_BODY_CMD,  		6,	14  },
//     { 6,		CMD_ID_ACTIVE_POST_SET_BODY_DATA,  		7,	14  },
//     { 7,		CMD_ID_ACTIVE_POST_GET_HTTP_RSP,   		8,	14  },
//     { 8,		CMD_ID_ACTIVE_POST_HEADER_CONFIG_UN,    9,	14  },
//     { 9,		CMD_ID_ACTIVE_CONFIRM_SET_URL_CMD,      10,	14  },
//     { 10,	CMD_ID_ACTIVE_CONFIRM_SET_URL_DATA,     11,	14  },
//     { 11,	CMD_ID_ACTIVE_CONFIRM_CONNECT_URL,      12,	14  },
//     { 12,	CMD_ID_ACTIVE_CONFIRM_GET_HTTP_RSP,     13,	14  },

//     { 13,	CMD_ID_SUCCESS,     					13,	13  },
//     { 14,	CMD_ID_ERROR,       					14,	14  }
// };

//device active steps , http post and get
static net4g_at_step_t device_active_steps[] =
{
    // step         id                success  fail
    { 0,	CMD_ID_ACTIVE_HTTP_CONFIG_CACHE,       1,	11 },
    { 1,	CMD_ID_ROUTE_HTTP_CONFIG_SSL_CTX_ID,   2,	11 },
    { 2,	CMD_ID_ACTIVE_SET_URL_CMD,             3,	11 },
    { 3,	CMD_ID_ACTIVE_SET_URL_DATA,	           4,	11 },
    { 4,	CMD_ID_ACTIVE_SET_CONNECT_URL,         5,	11 },
    { 5,	CMD_ID_ACTIVE_GET_HTTP_RSP,            6,	11 },
    { 6,	CMD_ID_ACTIVE_CONFIRM_SET_URL_CMD,     7,	11 },
    { 7,	CMD_ID_ACTIVE_CONFIRM_SET_URL_DATA,    8,	11 },
    { 8,	CMD_ID_ACTIVE_CONFIRM_CONNECT_URL,     9,	11 },
    { 9,	CMD_ID_ACTIVE_CONFIRM_GET_HTTP_RSP,    10,	11 },

    { 10,	CMD_ID_SUCCESS,     			       10,	10 },
    { 11,	CMD_ID_ERROR,       			       11,	11 }
};

//device route steps , http get
// static net4g_at_step_t device_route_steps[] =
// {
//     // step         id                           success fail
//     { 0,	CMD_ID_ACTIVE_POST_HEADER_CONFIG_UN,    1,	8 },
//     { 1,	CMD_ID_ROUTE_HTTP_CONFIG_CACHE,     	2,	8 },
//     { 2,	CMD_ID_ROUTE_HTTP_CONFIG_SSL_CTX_ID,    3,	8 },
//     { 3,	CMD_ID_ROUTE_SET_URL_CMD,     			4,	8 },
//     { 4,	CMD_ID_ROUTE_SET_URL_DATA,    			5,	8 },
//     { 5,	CMD_ID_ROUTE_CONNECT_URL,     			6,	8 },
//     { 6,	CMD_ID_ROUTE_GET_HTTP_RSP,    			7,	8 },

//     { 7,	CMD_ID_SUCCESS,     					7,	7 },
//     { 8,	CMD_ID_ERROR,       					8,	8 }
// };

//device route steps , http get
static net4g_at_step_t device_route_steps[] =
{
    // step         id                success  fail
	{ 0,	CMD_ID_ROUTE_HTTP_CONFIG_CACHE,     1,	6 },
    { 1,	CMD_ID_ROUTE_SET_URL_CMD,     		2,	6 },
    { 2,	CMD_ID_ROUTE_SET_URL_DATA,    		3,	6 },
    { 3,	CMD_ID_ROUTE_CONNECT_URL,     		4,	6 },
    { 4,	CMD_ID_ROUTE_GET_HTTP_RSP,    		5,	6 },

    { 5,	CMD_ID_SUCCESS,     		  		5,	5 },
    { 6,	CMD_ID_ERROR,       		  		6,	6 }
};


//device sign in steps , mqtt connect
// static net4g_at_step_t device_signIn_steps[] =
// {
//     // step         id                	success  fail
//     { 0,	CMD_ID_MQTT_CONFIG_SESSION,  	1,	8 },
//     { 1,	CMD_ID_MQTT_TCONFIG_VERSION,   	2,	8 },
//     { 2,	CMD_ID_MQTT_CONFIG_TIMEOUT,     3,	8 },
//     { 3,	CMD_ID_MQTT_CONFIG_KEEPALIVE,   4,	8 },
//     { 4,	CMD_ID_MQTT_OPEN,  				5,	8 },
//     { 5,	CMD_ID_MQTT_CONNECT,     		6,	8 },
//     { 6,	CMD_ID_MQTT_SUB_TOPIC,    		7,	8 },

//     { 7,	CMD_ID_SUCCESS,     			7,	7 },
//     { 8,	CMD_ID_ERROR,       			8,	8 }
// };

static net4g_at_step_t device_signIn_steps[] =
{
    // step         id                	success  fail
    { 0,	CMD_ID_MQTT_DISCONNECT,    		1,	1  },
    { 1,	CMD_ID_MQTT_NET_CLOSE,			2,	2  },
    { 2,	CMD_ID_MQTT_CONFIG_SENDMODE,	3,	12 },
    { 3,	CMD_ID_MQTT_CONFIG_SESSION,  	4,	12 },
    { 4,	CMD_ID_MQTT_TCONFIG_VERSION,   	5,	12 },
    { 5,	CMD_ID_MQTT_CONFIG_TIMEOUT,     6,	12 },
    { 6,	CMD_ID_MQTT_CONFIG_KEEPALIVE,   7,	12 },
	{ 7, 	CMD_ID_MQTT_CONFIG_RECMODE, 	8,  12 },
    { 8,	CMD_ID_MQTT_OPEN,  				9,	12 },
    { 9,	CMD_ID_MQTT_CONNECT,     		10,	12 },
    { 10,	CMD_ID_MQTT_SUB_TOPIC,    		11,	12 },

    { 11,	CMD_ID_SUCCESS,     			11,	11 },
    { 12,	CMD_ID_ERROR,       			12,	12 }
};



//device fw ota steps , http get
net4g_at_step_t device_ota_steps[] =
{
    // step         id                success  fail
    { 0,	CMD_ID_OTA_HTTP_CONFIG_NO_CACHE, 1,	6 },
    { 1,	CMD_ID_OTA_SET_URL_CMD,     	 2,	6 },
    { 2,	CMD_ID_OTA_SET_URL_DATA,    	 5,	6 },
    { 3,	CMD_ID_OTA_DOWNLOAD_RANGE,     	 5,	6 },
    { 4,	CMD_ID_OTA_GET_HTTP_RSP,    	 5,	6 },

    { 5,	CMD_ID_SUCCESS,     		  	 5,	5 },
    { 6,	CMD_ID_ERROR,       		  	 6,	6 }
};



/****************************************************************************************************************************************
*******************************USE QUECTEL EC800E-CN 4G MODULE
****************************************************************************************************************************************/
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800) || (NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801) || (NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))


// 4g module at cmd tables 
const at_cmd_t module_4g_cmds[] = 
{
	//命令ID         	     	 		重发间隔  重发次数  AT命令码            		   	    AT应答码 
	{CMD_ID_NONE,						0,			0,		NULL,								NULL},
	
// net config cmd
	{CMD_ID_HANDSHAKE,    				500,     	30,     NET_CMD_HAND,    					NET_CMD_ACK_OK},
//增加伪基站需要的指令

	{CMD_ID_RF_OFF,    				    500,     	1,      NET_CMD_RF_OFF,    					NET_CMD_ACK_OK},
	{CMD_ID_MIN_LEV,    				500,     	1,      NET_CMD_MIN_LEV,    				NET_CMD_ACK_OK},
	{CMD_ID_SET_FENCE,    				500,     	1,      NET_CMD_FENCE,    					NET_CMD_ACK_OK},
	{CMD_ID_RF_ON,    				    500,     	1,      NET_CMD_RF_ON,    					NET_CMD_ACK_OK},
    
    {CMD_ID_GET_MODULE_INFO,    		100,     	3,      NET_CMD_GET_MODULE_INFO,   			NET_CMD_ACK_OK},
    {CMD_ID_GET_IMEI,       			100,     	3,      NET_CMD_GET_IMEI,     				NET_CMD_ACK_OK},
    {CMD_ID_GET_SN_NO,    	    		100,     	3,      NET_CMD_GET_SN_NO,    				NET_CMD_ACK_OK},
    {CMD_ID_GET_IP,           			100,        30,     NET_CMD_GET_IP,        				NET_CMD_ACK_OK},
    {CMD_ID_IS_SIM_OK,          		100,        2,     	NET_CMD_IS_SIM_OK,         			NET_CMD_ACK_OK}, 
    {CMD_ID_GET_SIM_NO,         		100,        30,     NET_CMD_GET_SIM_NO,        		 	NET_CMD_ACK_OK}, 
	{CMD_ID_GET_SIM_STATE,      		100,     	30,     NET_CMD_GET_SIM_STATE,     			NET_CMD_ACK_OK},
	{CMD_ID_GET_NET_STATE,      		100,     	30,     NET_CMD_GET_NET_STATE,     			NET_CMD_ACK_OK},
	{CMD_ID_GET_RSSI,       			2000,     	3,      NET_CMD_GET_NET_RSSI,     			NET_CMD_ACK_OK},
	{CMD_ID_SET_CLOSE_ATE,      		100,     	3,      NET_CMD_CLOSE_ATE,     				NET_CMD_ACK_OK},
	{CMD_ID_SET_OPEN_ATE,      			100,     	3,      NET_CMD_OPEN_ATE,    				NET_CMD_ACK_OK},
	{CMD_ID_SET_BAUD_RATE,      		100,     	3,      NET_CMD_SET_BAUD,     				NET_CMD_ACK_OK},
	{CMD_ID_GET_LOCATION_PARA,      	100,     	1,      NET_CMD_LOCATION_PARA_GET,     		NET_CMD_ACK_OK},
	{CMD_ID_SET_LOCATION_PARA,      	300,     	1,      NET_CMD_LOCATION_PARA_SET,     		NET_CMD_ACK_OK},
	{CMD_ID_GET_LOCATION_ADDR,      	1000,     	1,      NET_CMD_LOCATION,     				"+QLBS: "},
	 
	{CMD_ID_SET_APN,      				3000,     	1,      NET_CMD_SET_APN,     				NET_CMD_ACK_OK},
	{CMD_ID_GET_QCELL,      			2000,     	1,      NET_CMD_GET_QCELL,     				NET_CMD_ACK_OK},//最大响应时间305S
	{CMD_ID_GET_QENG_SERVINGCELL,      	2000,     	1,      NET_CMD_GET_QENG_SERVINGCELL,     	NET_CMD_ACK_OK},//最大响应时间305S
	{CMD_ID_GET_QENG_NEIGHBOURCELL,     2000,     	1,      NET_CMD_GET_QENG_NEIGHBOURCELL,     NET_CMD_ACK_OK},//最大响应时间305S
	{CMD_ID_SET_PDP,      		        1000,     	1,      NET_CMD_SET_PDP,     				NET_CMD_ACK_OK},
	{CMD_ID_SET_DNS,      		        1000,     	1,      NET_CMD_IP_CONFIG,     				NET_CMD_ACK_OK},
#if 0
	{CMD_ID_SET_CFUN,      				100,     	1,      NET_CMD_SET_CFUN,     				NET_CMD_ACK_OK},
	{CMD_ID_SET_ECFREQ,      			100,     	1,      NET_CMD_SET_ECFREQ,     			NET_CMD_ACK_OK},
#endif

// device active (http-post)
	{CMD_ID_ACTIVE_SET_URL_CMD,    		500,     	3,      NET_CMD_HTTPPOST_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	{CMD_ID_ACTIVE_SET_URL_DATA,   		500,     	3,      NET_CMD_HTTPPOST_SET_URL_DATA,     	NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_SET_CONNECT_URL,  	500,     	3,      NET_CMD_HTTPPOST_SET_BODY_DATA,     NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_GET_HTTP_RSP,   		500,     	3,      NET_CMD_HTTPPOST_GET_HTTP_RSP,      NET_CMD_ACK_OK},
	//active confirm (http-get)
	{CMD_ID_ACTIVE_CONFIRM_SET_URL_CMD, 500,     	3,      NET_CMD_HTTPGET_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	{CMD_ID_ACTIVE_CONFIRM_SET_URL_DATA,500,     	3,      NET_CMD_HTTPGET_SET_URL_DATA,     	NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_CONFIRM_CONNECT_URL, 500,     	3,      NET_CMD_HTTPGET_CONNECT_URL,     	NET_CMD_ACK_OK},
	{CMD_ID_ACTIVE_CONFIRM_GET_HTTP_RSP,500,     	3,      NET_CMD_HTTPGET_GET_HTTP_RSP,     	NET_CMD_ACK_OK},

// devcie route (http-get)
	{CMD_ID_ROUTE_SET_URL_CMD,      	500,     	3,      NET_CMD_HTTPGET_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	{CMD_ID_ROUTE_SET_URL_DATA,     	500,     	3,      NET_CMD_HTTPGET_SET_URL_DATA,     	NET_CMD_ACK_OK},
	{CMD_ID_ROUTE_CONNECT_URL,      	1000,     	3,      NET_CMD_HTTPGET_CONNECT_URL,     	NET_CMD_ACK_OK},
	{CMD_ID_ROUTE_GET_HTTP_RSP,     	1000,     	5,      NET_CMD_HTTPGET_GET_HTTP_RSP,     	NET_CMD_ACK_OK},
	{CMD_ID_HTTP_CLOSE,      			500,     	3,      NET_CMD_HTTP_CLOSE,     			NET_CMD_ACK_OK},

// mqtt config
	{CMD_ID_MQTT_DISCONNECT,      		500,     	1,      NET_CMD_MQTT_DISCONNECT,     		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_NET_CLOSE,      		100,     	1,      NET_CMD_MQTT_NET_CLOSE,     		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_OPEN,      			500,     	3,      NET_CMD_MQTT_OPEN,     				"+QMTOPEN: 0,0"},
	{CMD_ID_MQTT_CONFIG_PDP,      		100,     	3,      NET_CMD_MQTT_CONFIG_PDP,     		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_SESSION,      	100,     	3,      NET_CMD_MQTT_CONFIG_SESSION,     	NET_CMD_ACK_OK},
	{CMD_ID_MQTT_TCONFIG_VERSION,      	100,     	3,      NET_CMD_MQTT_TCONFIG_VERSION,     	NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_TIMEOUT,      	100,     	3,      NET_CMD_MQTT_CONFIG_TIMEOUT,     	NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_KEEPALIVE,      100,     	3,      NET_CMD_MQTT_CONFIG_KEEPALIVE,     	NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONFIG_RECMODE, 		100, 		3, 		NET_CMD_MQTT_CONFIG_RECMODE, 		NET_CMD_ACK_OK},
	{CMD_ID_MQTT_CONNECT,      			1000,     	3,      NET_CMD_MQTT_CONNECT,     			"+QMTCONN: 0,0"},
	{CMD_ID_MQTT_SUB_TOPIC,      		500,     	3,      NET_CMD_MQTT_SUB_TOPIC,     		"+QMTSUB: 0,1,0"},

	// FW OTA (http-get)
	{CMD_ID_OTA_HTTP_CONFIG,      		100,     	3,      NET_CMD_HTTPOTA_CONFIG,          	NET_CMD_ACK_OK},
	{CMD_ID_OTA_SET_URL_CMD,      		200,     	10,     NET_CMD_HTTPGET_SET_URL_CMD,     	NET_CMD_HTTP_RSP_CONNECT},
	{CMD_ID_OTA_SET_URL_DATA,     		200,     	10,     NET_CMD_HTTPGET_SET_URL_DATA,     	NET_CMD_ACK_OK},
	{CMD_ID_OTA_DOWNLOAD_RANGE,      	1000,     	1,      NET_CMD_HTTPGET_GET_RSP_RANGE,     	NET_CMD_ACK_OK},
	{CMD_ID_OTA_GET_HTTP_RSP,     		1000,       1,      NET_CMD_HTTPGET_GET_HTTP_RSP,     	NET_CMD_ACK_OK},
	{CMD_ID_OTA_HTTP_CLOSE,      		500,     	3,      NET_CMD_HTTP_CLOSE,     			NET_CMD_ACK_OK},
    //DUAL CARD
    {CMD_SIM_CHECK,      		        3000,     	1,      NET_CMD_CARD_CHECK,          	        NET_CMD_ACK_OK},
    {CMD_SIM_CHANGE_1,      		    3000,     	3,      NET_CMD_CARD_SIM1,          	        NET_CMD_ACK_OK},
    {CMD_SIM_CHANGE_2,      		    3000,     	3,      NET_CMD_CARD_SIM2,          	        NET_CMD_ACK_OK},
};

#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800) || (NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801))

//net config at cmd steps
static net4g_at_step_t net4g_config_command_steps[] =
{
    // step         id                success  fail
    { 0,	CMD_ID_HANDSHAKE,           	1,	0 },	//AT握手
    { 1,	CMD_ID_SET_CLOSE_ATE,			2,	2 },	//关闭回显
    { 2,	CMD_ID_RF_OFF,              	3,	3 },	//关闭射频
    { 3,	CMD_ID_MIN_LEV,        	    	4,	4 },	//配置最小信号值
    { 4,	CMD_ID_SET_FENCE,           	5,	5 },	//配置电子围栏
    { 5,	CMD_ID_RF_ON,               	6,	6 },	//打开射频
    { 6,	CMD_ID_GET_MODULE_INFO,     	7,	38 },	//获取模块版本信息
    { 7,	CMD_ID_GET_IMEI,        		8,	38 },	//获取IMEI号
    { 8,	CMD_ID_GET_SN_NO,          		9,	38 },	//获取SN
    //上电先确认SIM卡2状态
    { 9,	CMD_SIM_CHANGE_2,           	10,	15 },	//切换到SIM卡2
    { 10,	CMD_SIM_CHECK,              	11,	15 },	//查询当前使用的SIM卡
    { 11,	CMD_ID_IS_SIM_OK,           	12,	15 },	//检测是否有SIM卡
    { 12,	CMD_ID_GET_SIM_NO,          	13,	15 },	//获取SIM卡卡号
    { 13,	CMD_ID_GET_SIM_STATE,       	14,	15 },   //获取SIM卡注册状态     
    { 14,	CMD_ID_GET_NET_STATE,       	15,	15 },	//获取4G网络附着情况
    //确认SIM卡1状态，并使用SIM卡1做业务
	{ 15,   CMD_SIM_CHANGE_1,		    	16, 21 },	//切换到SIM卡1
	{ 16,	CMD_SIM_CHECK,              	17,	21 },
	{ 17,	CMD_ID_IS_SIM_OK,           	18,	21 },
    { 18,	CMD_ID_GET_SIM_NO,          	19,	21 },
    { 19,	CMD_ID_GET_SIM_STATE,       	20,	21 },
    { 20,	CMD_ID_GET_NET_STATE,       	27,	21 },

	//SIM1 业务失败，重新切回SIM2
    { 21,	CMD_SIM_CHANGE_2,           	22,	38 },	//切换回SIM卡2
    { 22,	CMD_SIM_CHECK,              	23,	38 },
    { 23,	CMD_ID_IS_SIM_OK,           	24,	38 },
    { 24,	CMD_ID_GET_SIM_NO,          	25,	38 },
    { 25,	CMD_ID_GET_SIM_STATE,       	26,	38 },        
    { 26,	CMD_ID_GET_NET_STATE,       	27,	38 },

	{ 27,	CMD_ID_SET_PDP,       			28,	28 },	//激活PDP场景
    { 28,	CMD_ID_SET_DNS,       			29,	29 },	//配置DNS服务器地址
	{ 29,	CMD_ID_MQTT_DISCONNECT,  		34,	34 },	//MQTT断开连接
	{ 30,	CMD_ID_GET_LOCATION_PARA,   	32,	31 },	//获取定位参数
	{ 31,	CMD_ID_SET_LOCATION_PARA,   	32,	32 },	//设置定位参数
    { 32,	CMD_ID_GET_LOCATION_ADDR,   	33,	33 },	//获取定位


	{ 33,	CMD_ID_GET_QCELL,       		34,	34 },	//获取服务小区和邻近小区信息
	{ 34,	CMD_ID_GET_QENG_SERVINGCELL,    35,	35 },	//获取主服务小区信息
	{ 35,	CMD_ID_GET_QENG_NEIGHBOURCELL,  36,	36 },	//获取服务小区信息
	{ 36,	CMD_ID_GET_RSSI,       			37,	37 },	//获取信号强度


    { 37,	CMD_ID_SUCCESS,     			37,	37 },
    { 38,	CMD_ID_ERROR,       			38,	38 }
};

#elif (NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU)
//net config at cmd steps
static net4g_at_step_t net4g_config_command_steps[] =
{
    // step         id                success  fail
    { 0,	CMD_ID_HANDSHAKE,           	1,	0 },	//AT握手
    { 1,	CMD_ID_SET_CLOSE_ATE,			2,	2 },	//关闭回显
    // { 2,	CMD_ID_RF_OFF,              	3,	3 },	//关闭射频
    // { 3,	CMD_ID_MIN_LEV,        	    	4,	4 },	//配置最小信号值
    // { 4,	CMD_ID_SET_FENCE,           	5,	5 },	//配置电子围栏
    // { 5,	CMD_ID_RF_ON,               	6,	6 },	//打开射频
    { 2,	CMD_ID_GET_MODULE_INFO,     	3,	19 },	//获取模块版本信息
    { 3,	CMD_ID_GET_IMEI,        		4,	19 },	//获取IMEI号
    { 4,	CMD_ID_GET_SN_NO,          		5,	19 },	//获取SN
    //上电先确认SIM卡2状态
    // { 9,	CMD_SIM_CHANGE_2,           	10,	15 },	//切换到SIM卡2
    // { 10,	CMD_SIM_CHECK,              	11,	15 },	//查询当前使用的SIM卡
    // { 11,	CMD_ID_IS_SIM_OK,           	12,	15 },	//检测是否有SIM卡
    // { 12,	CMD_ID_GET_SIM_NO,          	13,	15 },	//获取SIM卡卡号
    // { 13,	CMD_ID_GET_SIM_STATE,       	14,	15 },   //获取SIM卡注册状态     
    // { 14,	CMD_ID_GET_NET_STATE,       	15,	15 },	//获取4G网络附着情况
    //确认SIM卡1状态，并使用SIM卡1做业务
	// { 15,   CMD_SIM_CHANGE_1,		    	16, 27 },	//切换到SIM卡1
	// { 16,	CMD_SIM_CHECK,              	17,	27 },
	{ 5,	CMD_ID_IS_SIM_OK,           	6,	19 },
    { 6,	CMD_ID_GET_SIM_NO,          	7,	19 },
	{ 7,	CMD_ID_SET_APN,          		8,	19 },
    { 8,	CMD_ID_GET_SIM_STATE,       	9,	19 },
    { 9,	CMD_ID_GET_NET_STATE,       	10,	19 },

	//SIM1 业务失败，重新切回SIM2
    // { 21,	CMD_SIM_CHANGE_2,           	22,	38 },	//切换回SIM卡2
    // { 22,	CMD_SIM_CHECK,              	23,	38 },
    // { 23,	CMD_ID_IS_SIM_OK,           	24,	38 },
    // { 24,	CMD_ID_GET_SIM_NO,          	25,	38 },
    // { 25,	CMD_ID_GET_SIM_STATE,       	26,	38 },        
    // { 26,	CMD_ID_GET_NET_STATE,       	27,	38 },

	// { 27,	CMD_ID_SET_PDP,       			28,	28 },	//激活PDP场景
    // { 28,	CMD_ID_SET_DNS,       			29,	29 },	//配置DNS服务器地址
	{ 10,	CMD_ID_MQTT_DISCONNECT,  		11,	11 },	//MQTT断开连接
	{ 11,	CMD_ID_GET_LOCATION_PARA,   	13,	12 },	//获取定位参数
	{ 12,	CMD_ID_SET_LOCATION_PARA,   	13,	14 },	//设置定位参数
    { 13,	CMD_ID_GET_LOCATION_ADDR,   	14,	14 },	//获取定位


	{ 14,	CMD_ID_GET_QCELL,       		15,	15 },	//获取服务小区和邻近小区信息
	{ 15,	CMD_ID_GET_QENG_SERVINGCELL,    16,	16 },	//获取主服务小区信息
	{ 16,	CMD_ID_GET_QENG_NEIGHBOURCELL,  17,	17 },	//获取服务小区信息
	{ 17,	CMD_ID_GET_RSSI,       			18,	19 },	//获取信号强度


    { 18,	CMD_ID_SUCCESS,     			18,	18 },
    { 19,	CMD_ID_ERROR,       			19,	19 }
};

#endif

//net config at cmd steps
static net4g_at_step_t net4g_get_cell_info_config_command_steps[] =
{
    // step         id                success  fail
	// { 0,	CMD_ID_GET_QCELL,       		1,	1 },	//获取服务小区和邻近小区信息
	{ 0,	CMD_ID_GET_QENG_SERVINGCELL,    1,	1 },	//获取主服务小区信息
	// { 2,	CMD_ID_GET_QENG_NEIGHBOURCELL,  3,	3 },	//获取服务小区信息
	{ 1,	CMD_ID_GET_RSSI,   				2,	3 },	//获取信号强度


    { 2,	CMD_ID_SUCCESS,     			2,	2 },
    { 3,	CMD_ID_ERROR,       			3,	3 }
};


//device active steps , http post and get
static net4g_at_step_t device_active_steps[] =
{
    // step         id                success  fail
    { 0,	CMD_ID_ACTIVE_SET_URL_CMD,             1,	0 },
    { 1,	CMD_ID_ACTIVE_SET_URL_DATA,	           2,	9 },
    { 2,	CMD_ID_ACTIVE_SET_CONNECT_URL,         3,	9 },
    { 3,	CMD_ID_ACTIVE_GET_HTTP_RSP,            4,	9 },
    { 4,	CMD_ID_ACTIVE_CONFIRM_SET_URL_CMD,     5,	9 },
    { 5,	CMD_ID_ACTIVE_CONFIRM_SET_URL_DATA,    6,	9 },
    { 6,	CMD_ID_ACTIVE_CONFIRM_CONNECT_URL,     7,	9 },
    { 7,	CMD_ID_ACTIVE_CONFIRM_GET_HTTP_RSP,    8,	9 },

    { 8,	CMD_ID_SUCCESS,     			       8,	8 },
    { 9,	CMD_ID_ERROR,       			       9,	9 }
};


//device route steps , http get
static net4g_at_step_t device_route_steps[] =
{
    // step         id                success  fail
    { 0,	CMD_ID_ROUTE_SET_URL_CMD,     1,	5 },
    { 1,	CMD_ID_ROUTE_SET_URL_DATA,    2,	5 },
    { 2,	CMD_ID_ROUTE_CONNECT_URL,     3,	5 },
    { 3,	CMD_ID_ROUTE_GET_HTTP_RSP,    4,	5 },

    { 4,	CMD_ID_SUCCESS,     			4,	4 },
    { 5,	CMD_ID_ERROR,       			5,	5 }
};



//device sign in steps , mqtt connect
static net4g_at_step_t device_signIn_steps[] =
{
    // step         id                	success  fail
    { 0,	CMD_ID_MQTT_DISCONNECT,    		1,	1  },
    { 1,	CMD_ID_MQTT_NET_CLOSE,			2,	2 },
    { 2,	CMD_ID_MQTT_CONFIG_PDP,   		3,	12 },
    { 3,	CMD_ID_MQTT_CONFIG_SESSION,  	4,	12 },
    { 4,	CMD_ID_MQTT_TCONFIG_VERSION,   	5,	12 },
    { 5,	CMD_ID_MQTT_CONFIG_TIMEOUT,     6,	12 },
    { 6,	CMD_ID_MQTT_CONFIG_KEEPALIVE,   7,	12 },
	{ 7, 	CMD_ID_MQTT_CONFIG_RECMODE, 	8,  12 },
    { 8,	CMD_ID_MQTT_OPEN,  				9,	12 },
    { 9,	CMD_ID_MQTT_CONNECT,     		10,	12 },
    { 10,	CMD_ID_MQTT_SUB_TOPIC,    		11,	12 },

    { 11,	CMD_ID_SUCCESS,     			11,	11 },
    { 12,	CMD_ID_ERROR,       			12,	12 }
};

//device fw ota steps , http get
net4g_at_step_t device_ota_steps[] =
{	
	  { 0,	CMD_ID_OTA_HTTP_CONFIG, 		1,	6 },
	  { 1,	CMD_ID_OTA_SET_URL_CMD, 		2,	6 },
	  { 2,	CMD_ID_OTA_SET_URL_DATA,		5,	6 },
	  { 3,	CMD_ID_OTA_DOWNLOAD_RANGE,		5,	6 },
	  { 4,	CMD_ID_OTA_GET_HTTP_RSP,		5,	6 },
	
	  { 5,	CMD_ID_SUCCESS, 				5,	5 },
	  { 6,	CMD_ID_ERROR,					6,	6 }

};

#endif





void init_net4g_work_steps(void)
{
	memset(&net4G_process_steps, 0, sizeof(net4G_process_steps));
	
	net4G_process_steps.netWork = NETWORK_WORK_STEP__INIT;
}


void set_net4g_work_step(NETWROK_WORK_STEP_E targetStep)
{
	net4G_process_steps.netWork = targetStep;
}


NETWROK_WORK_STEP_E get_net4g_work_step(void)
{
	return net4G_process_steps.netWork;
}





/****************************************************************************************************************************
*NET CONFIG
*****************************************************************************************************************************/

/*
*  return:  1-success, 0-fail
*/
//test by y04485
uint8_t get_atcmd_rsp_before_after_value(uint8_t *pu8SrcData, uint32_t u32SrcLen, uint8_t *pu8MiddleKey, uint8_t *pu8DestDataBefore, uint8_t *pu8DestDataAfter)
{
	uint8_t *dataBase = pu8SrcData;
	uint8_t *offset_head = pu8SrcData;
	uint32_t tempData_len = u32SrcLen;
    uint32_t tempData2_len = 0;
	uint8_t ret = 0;
	uint8_t u8i=0;
	// get target data header
	if (NULL != pu8MiddleKey)
	{
		offset_head = strstr(dataBase, pu8MiddleKey);
	}

    if (offset_head != NULL)
	{
        tempData_len = offset_head - dataBase;
//        printf("len[%d]\r\n", tempData_len);
        if (tempData_len > 0)
		{
			memcpy(pu8DestDataBefore, dataBase, tempData_len);
//            printf("test[%s]\r\n",pu8DestDataBefore);
            
            
            offset_head  += strlen(pu8MiddleKey);            

            if(u32SrcLen > 0)
            {
				tempData2_len = u32SrcLen - tempData_len - strlen(pu8MiddleKey);
            }
            else
            {
                for(u8i = 0; u8i < 10; u8i++)
                {
                    if (*(offset_head + u8i) == '\r' || *(offset_head + u8i) == '\n' || *(offset_head + u8i) == ' ' || *(offset_head + u8i) == '"' || *(offset_head + u8i) == '\0')
        			{
        				break;
        			}
        			else
        			{
        				continue;
        			}
                }
                tempData2_len = u8i;
                
            }

            if(tempData2_len > 0)
            {
                memcpy(pu8DestDataAfter, offset_head, tempData2_len);
                ret = 1;
            }
		}
    }

    return ret;
}


/*
*	fucntion: get_atcmd_rsp_value
*	desc: to find the value of the headKey form srcData, the result save to destData
*	return:  1-success, 0-fail
*/
uint8_t get_atcmd_rsp_value(uint8_t* srcData, uint32_t srcLen, uint8_t* headKey, uint8_t* tailKey, uint8_t* destData)
{	
	uint8_t *dataBase = srcData;
	uint8_t *offset_head = srcData;
	uint8_t *offset_tail = srcData;
	uint8_t tempData_len = srcLen;
	uint8_t ret = 0;
	uint8_t i = 0;

	//获取帧头关键字
	if(headKey != NULL)
	{
		offset_head = strstr(dataBase, headKey);
	}

	if(offset_head != NULL)//获取到帧头关键字指针
	{
		if(headKey != NULL)//帧头关键字不为空
		{
			offset_head += strlen(headKey); //去掉帧头
			tempData_len = srcLen - strlen(headKey);
		}
		
		//去除帧头后的无效字符
		for(i=0;i< tempData_len;i++)
		{
			if(*(offset_head+i) == '\r' 
				|| *(offset_head+i) == '\n' 
				|| *(offset_head+i) == ' ' 
				|| *(offset_head+i) == '"'
				||*(offset_head+i) == '\0')
			{
				continue;
			}
			else
			{
				break;
			}
		}
		offset_head += i;
	//	printf("[%s]header:%s",headKey, offset_head);
	}
	else //帧头关键字获取失败
	{
		return ret;
	}


	//获取帧尾关键字
	offset_tail = strstr(offset_head, tailKey);

	if(offset_tail != NULL)//获取到帧尾关键字指针
	{
		tempData_len = offset_tail - offset_head;

		//去除帧尾前的无效字符
		for(i=0; i<tempData_len; i++)
		{
			if(*(offset_tail-1-i) == '\r' 
				|| *(offset_tail-1-i) == '\n' 
				|| *(offset_tail-1-i) == ' ' 
				|| *(offset_tail-1-i) == '"'
				||*(offset_tail-1-i) == '\0')
			{
				continue;
			}
			else
			{
				break;
			}
		}
		offset_tail -= i;
	//	printf("[%s]tail:%s",tailKey, offset_tail);
	}
	else //帧尾关键字获取失败
	{
		return ret;
	}

	//get target data value
	if((offset_head)&&(offset_tail))
	{
		tempData_len = offset_tail - offset_head;
		if(tempData_len > 0)
		{
			memcpy(destData, offset_head, tempData_len);
			ret = 1;
		}
	}
	
	return ret;
}


static AT_RET_E net4g_get_module_info_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	uint8_t* appVer_p = NULL;
	uint32_t net4g_appVer = 0;
	
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, NULL, NET_CMD_RSP_TAIL_OK, target_data))
    	{

#if (NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)
			appVer_p = strstr(target_data, "NT26KCNA00NNA-L")+strlen("NT26KCNA00NNA-L");
			net4g_appVer = atoi(appVer_p);
			memset((char*)&g_deviceGlobalInfo.net4gVer, 0, sizeof(g_deviceGlobalInfo.net4gVer));
			sprintf(g_deviceGlobalInfo.net4gVer, "%d.%d.%d",net4g_appVer/1000000,net4g_appVer/100000%10,net4g_appVer/10000%10);

			if(strcmp(g_deviceGlobalInfo.net4gVer,gSysInfo_struct.net4G.net4gVer)!=0)
			{
				memset((char*)&gSysInfo_struct.net4G.net4gVer, 0, sizeof(gSysInfo_struct.net4G.net4gVer));
				sprintf(gSysInfo_struct.net4G.net4gVer, "%d.%d.%d",net4g_appVer/1000000,net4g_appVer/100000%10,net4g_appVer/10000%10);
				tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
			}

#elif (NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)
			appVer_p = strstr(target_data, "EC800ECNLCR") + strlen("EC800ECNLCR");
			for(uint8_t i=0; i<strlen(appVer_p); i++)
			{
				if(((*(appVer_p+i)>='A')&&(*(appVer_p+i)<='Z')) || ((*(appVer_p+i)>='a')&&(*(appVer_p+i)<='z')))
				{
					*(appVer_p+i) = '.';
				}
				else if((*(appVer_p+i)<'0') || (*(appVer_p+i)>'9'))
				{
					*(appVer_p+i) = '\0';
					break;
				}
			}
			sprintf(g_deviceGlobalInfo.net4gVer, "%s", appVer_p);

			if(strcmp(g_deviceGlobalInfo.net4gVer,gSysInfo_struct.net4G.net4gVer)!=0)
			{
				sprintf(gSysInfo_struct.net4G.net4gVer, "%s", appVer_p);
				tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
			}

#elif (NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)
			appVer_p = strstr(target_data, "EC801ECNCGR") + strlen("EC801ECNCGR");
			for(uint8_t i=0; i<strlen(appVer_p); i++)
			{
				if(((*(appVer_p+i)>='A')&&(*(appVer_p+i)<='Z')) || ((*(appVer_p+i)>='a')&&(*(appVer_p+i)<='z')))
				{
					*(appVer_p+i) = '.';
				}
				else if((*(appVer_p+i)<'0') || (*(appVer_p+i)>'9'))
				{
					*(appVer_p+i) = '\0';
					break;
				}
			}
			sprintf(g_deviceGlobalInfo.net4gVer, "%s", appVer_p);

			if(strcmp(g_deviceGlobalInfo.net4gVer,gSysInfo_struct.net4G.net4gVer)!=0)
			{
				sprintf(gSysInfo_struct.net4G.net4gVer, "%s", appVer_p);
				tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
			}

#elif (NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU)
			appVer_p = strstr(target_data, "EG800GEULDR") + strlen("EG800GEULDR");
			for(uint8_t i=0; i<strlen(appVer_p); i++)
			{
				if(((*(appVer_p+i)>='A')&&(*(appVer_p+i)<='Z')) || ((*(appVer_p+i)>='a')&&(*(appVer_p+i)<='z')))
				{
					*(appVer_p+i) = '.';
				}
				else if((*(appVer_p+i)<'0') || (*(appVer_p+i)>'9'))
				{
					*(appVer_p+i) = '\0';
					break;
				}
			}
			sprintf(g_deviceGlobalInfo.net4gVer, "%s", appVer_p);

			if(strcmp(g_deviceGlobalInfo.net4gVer,gSysInfo_struct.net4G.net4gVer)!=0)
			{
				sprintf(gSysInfo_struct.net4G.net4gVer, "%s", appVer_p);
				tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
			}	

#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
			// appVer_p = strstr(target_data, "NT26KCNA00NNA-L")+strlen("NT26KCNA00NNA-L");
			// if(appVer_p)
			// {
			// 	net4g_appVer = atoi(appVer_p);
			// }
			// else
			// {
				appVer_p = strstr(target_data, "NT26KCN")+strlen("NT26KCN");
				if(appVer_p)
				{
					net4g_appVer = atoi(appVer_p+8);
				}
			// }
			
			memset((char*)&g_deviceGlobalInfo.net4gVer, 0, sizeof(g_deviceGlobalInfo.net4gVer));
			sprintf(g_deviceGlobalInfo.net4gVer, "%d.%d.%d",net4g_appVer/1000000,net4g_appVer/100000%10,net4g_appVer/10000%10);	

			if(strcmp(g_deviceGlobalInfo.net4gVer,gSysInfo_struct.net4G.net4gVer)!=0)
			{
				memset((char*)&gSysInfo_struct.net4G.net4gVer, 0, sizeof(gSysInfo_struct.net4G.net4gVer));
				sprintf(gSysInfo_struct.net4G.net4gVer, "%d.%d.%d",net4g_appVer/1000000,net4g_appVer/100000%10,net4g_appVer/10000%10);
				tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
			}
#endif
			printf("[Global_4gVer]%d:%s\r\n",strlen(g_deviceGlobalInfo.net4gVer), g_deviceGlobalInfo.net4gVer);
			// printf("[gSysInfo_struct.net4G.net4gVer]%d:%s\r\n",strlen(gSysInfo_struct.net4G.net4gVer), gSysInfo_struct.net4G.net4gVer);

			return AT_OK;
		}
	}
            
    return AT_ERR;
}


static AT_RET_E net4g_get_module_imei_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, GET_IMEI_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	{
    		memset((char*)&gSysInfo_struct.net4G.imei, 0, sizeof(gSysInfo_struct.net4G.imei));
    		memcpy(gSysInfo_struct.net4G.imei, target_data, strlen(target_data));
			printf("[imei]%d:%s\r\n",strlen(gSysInfo_struct.net4G.imei), gSysInfo_struct.net4G.imei);
			return AT_OK;
		}
	}
            
    return AT_ERR;
}

#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
/**
 * @brief 获取当前使能的SIM卡并保存到结构体
 * 
 * 收到模组返回当前使用的SIM卡，保存到结构体
 * 
 * @return void
 */

static AT_RET_E net4g_get_sim_using_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	uint8_t sim_flag;

    if(ret == AT_OK) 
    {

		if(get_atcmd_rsp_value(srcData,1, GET_SIM_USING_HEAD, NET_CMD_RSP_TAIL_OK, &sim_flag))
		{

			printf("using sim%c\r\n",sim_flag);

			if(sim_flag == '0')
			{
				g_deviceGlobalInfo.simFlag = 1;
				printf("g_deviceGlobalInfo.simFlag = %d\r\n\r\n",g_deviceGlobalInfo.simFlag);
			}
			else if(sim_flag == '1')
			{
				g_deviceGlobalInfo.simFlag = 2;
				printf("g_deviceGlobalInfo.simFlag = %d\r\n\r\n",g_deviceGlobalInfo.simFlag);
			}

			return AT_OK;
		}

	}
            
    return AT_ERR;
}
#endif


#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
/**
 * @brief 获取当前使能的SIM卡并保存到结构体
 * 
 * 收到模组返回当前使用的SIM卡，保存到结构体
 * 
 * @return void
 */

static AT_RET_E net4g_get_sim_using_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	uint8_t sim_flag;

    if(ret == AT_OK) 
    {

		if(get_atcmd_rsp_value(srcData,1, GET_SIM_USING_HEAD, NET_CMD_RSP_TAIL_OK, &sim_flag))
		{
			printf("using sim%c\r\n",sim_flag);

			if(sim_flag == '1')
			{
				g_deviceGlobalInfo.simFlag = 1;
				printf("g_deviceGlobalInfo.simFlag = %d\r\n",g_deviceGlobalInfo.simFlag);
			}
			else if(sim_flag == '2')
			{
				g_deviceGlobalInfo.simFlag = 2;
				printf("g_deviceGlobalInfo.simFlag = %d\r\n",g_deviceGlobalInfo.simFlag);
			}

			return AT_OK;
		}

	}
            
    return AT_ERR;
}
#endif



static AT_RET_E net4g_get_module_sn_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, GET_SN_NO_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	{
    		memset((char*)&gSysInfo_struct.net4G.sn, 0, sizeof(gSysInfo_struct.net4G.sn));
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
    		memcpy(gSysInfo_struct.net4G.sn, target_data, strlen(target_data));
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
			if(strlen(target_data)>15)
			{
				memcpy(gSysInfo_struct.net4G.sn, target_data, strlen(target_data)-2); //去掉移远SN中的0P
			}
			else
			{
				memcpy(gSysInfo_struct.net4G.sn, target_data, strlen(target_data)); //去掉移远SN中的0P
			}
			
#endif
			printf("[sn]%d:%s\r\n",strlen(gSysInfo_struct.net4G.sn), gSysInfo_struct.net4G.sn);
			return AT_OK;
		}
	}
            
    return AT_ERR;
}


static AT_RET_E net4g_is_sim_ok_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, IS_SIM_OK_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	{
			printf("[is_sim_ok]%d:%s\r\n",strlen(target_data), target_data);
			if(0==strcasecmp(target_data, "READY"))
			{
				return AT_OK;
			}
			else 
			{

			}
		}
		else if(get_atcmd_rsp_value(srcData,len, IS_SIM_OK_RSP_HEAD_ERR, NET_CMD_RSP_TAIL_OK, target_data))
    	{
			printf("[is_sim_err]%d:%s\r\n",strlen(target_data), target_data);
			if(0==strcasecmp(target_data, "10"))
			{

			}
				
		}
	}
            
    return AT_ERR;
}

static AT_RET_E net4g_get_sim_no_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	uint8_t sim_flag;
	sim_flag = g_deviceGlobalInfo.simFlag;
    if(ret == AT_OK) 
    {
		if(sim_flag == 1)
		{
			if(get_atcmd_rsp_value(srcData,len, GET_SIM_NO_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
			{
				printf("[read gsm iccid1]%d:%s\r\n",strlen(target_data), target_data);
				// printf("[iccid1]%d:%s\r\n",strlen(gSysInfo_struct.net4G.iccid), gSysInfo_struct.net4G.iccid);
				if(strcmp(gSysInfo_struct.net4G.iccid,target_data)!=0)
				{
					memset((char*)&gSysInfo_struct.net4G.iccid, 0, sizeof(gSysInfo_struct.net4G.iccid));
					memcpy(gSysInfo_struct.net4G.iccid, target_data, strlen(target_data));
					tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
					printf("[iccid1]%d:%s\r\n",strlen(gSysInfo_struct.net4G.iccid), gSysInfo_struct.net4G.iccid);
				}
				
				return AT_OK;
			}
		}
		else if(sim_flag ==2)
		{
			if(get_atcmd_rsp_value(srcData,len, GET_SIM_NO_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
			{
				printf("[read gsm iccid2]%d:%s\r\n",strlen(target_data), target_data);
				// printf("[iccid2]%d:%s\r\n",strlen(gSysInfo_struct.net4G.iccid2), gSysInfo_struct.net4G.iccid2);
				if(strcmp(gSysInfo_struct.net4G.iccid2,target_data)!=0)
				{
					memset((char*)&gSysInfo_struct.net4G.iccid2, 0, sizeof(gSysInfo_struct.net4G.iccid2));
					memcpy(gSysInfo_struct.net4G.iccid2, target_data, strlen(target_data));
					tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
					printf("[iccid2]%d:%s\r\n",strlen(gSysInfo_struct.net4G.iccid2), gSysInfo_struct.net4G.iccid2);
				}
				return AT_OK;
			}
		}
		else 
		{
			return AT_OK;
		}
	}
	else 
	{
		if(sim_flag == 1)
		{
			g_deviceGlobalInfo.sim1State = 2;
			printf("sim1State = %d\r\n",g_deviceGlobalInfo.sim1State);
		}
		else if(sim_flag ==2)
		{
			g_deviceGlobalInfo.sim2State = 2;
			printf("sim2State = %d\r\n",g_deviceGlobalInfo.sim2State);
		}
	}         
    return AT_ERR;
}


static AT_RET_E net4g_get_sim_state_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	uint8_t sim_flag;
	sim_flag = g_deviceGlobalInfo.simFlag;
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, GET_SIM_STATE_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	{
			printf("[get sim state]%d:%s\r\n",strlen(target_data), target_data);
			if((0==strcasecmp(target_data, "0,1"))||(0==strcasecmp(target_data, "0,5")))
			{
			    if(sim_flag == 1)
				{
					g_deviceGlobalInfo.sim1State = 1;
					printf("sim%d state %d",sim_flag,g_deviceGlobalInfo.sim1State);
				}
				else if (sim_flag == 2)
				{
					g_deviceGlobalInfo.sim2State = 1;
					printf("sim%d state %d",sim_flag,g_deviceGlobalInfo.sim2State);
				}
				return AT_OK;
			}
			else
			{
				if(sim_flag == 1)
				{
					g_deviceGlobalInfo.sim1State = 2;
					printf("sim%d net state %d",sim_flag,g_deviceGlobalInfo.sim1State);
				}
				else if (sim_flag == 2)
				{
					g_deviceGlobalInfo.sim2State = 2;
					printf("sim%d net state %d",sim_flag,g_deviceGlobalInfo.sim2State);
				}
			}
		}
	}
    return AT_ERR;
}


static AT_RET_E net4g_get_net_state_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	uint8_t sim_flag;
	sim_flag = g_deviceGlobalInfo.simFlag;
	
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, GET_NET_STATE_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	{
			printf("[get net state]%d:%s\r\n",strlen(target_data), target_data);
			if(0==strcasecmp(target_data, "1"))
			{
				if(sim_flag == 1)
				{
					g_deviceGlobalInfo.sim1State = 1;
					printf("sim%d net state %d\r\n",sim_flag,g_deviceGlobalInfo.sim1State);
				}
				else if (sim_flag == 2)
				{
					g_deviceGlobalInfo.sim2State = 1;
					printf("sim%d net state %d\r\n",sim_flag,g_deviceGlobalInfo.sim2State);
				}
				return AT_OK;
			}
			else
			{
				if(sim_flag == 1)
				{
					g_deviceGlobalInfo.sim1State = 2;
					printf("sim%d net state %d\r\n",sim_flag,g_deviceGlobalInfo.sim1State);
				}
				else if (sim_flag == 2)
				{
					g_deviceGlobalInfo.sim2State = 2;
					printf("sim%d net state %d\r\n",sim_flag,g_deviceGlobalInfo.sim2State);
				}
			}
		}
	}
            
    return AT_ERR;
}


static AT_RET_E net4g_get_net_rssi_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
    uint8_t target_data[50] = {0};
	uint8_t rssiValue = 0;
	
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, GET_NET_RSSI_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	{
    		rssiValue = atoi(target_data);
			if(rssiValue > 31)
			{
				rssiValue = 0;
			}
			g_deviceGlobalInfo.signal = (rssiValue*100)/31;
			printf("[get rssi ]: %d\r\n", g_deviceGlobalInfo.signal);

			g_deviceGlobalInfo.qeng_servingcell_info.RSSI = rssiValue;//转换拿到的值
			// printf("g_deviceGlobalInfo.qeng_servingcell_info.RSSI = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSSI);
			return AT_OK;
		}
	}
            
    return AT_ERR;
}


static AT_RET_E net4g_get_net_qcell_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
	uint8_t *p_target_data_start = 0;
	uint16_t target_data_start_offset = 0;
    uint8_t target_data[500] = {0};
	char *token;
    uint8_t currentIndex = 0;
    if(ret == AT_OK) 
    {
		g_deviceGlobalInfo.nearbts_count = 0;
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
    	// if(get_atcmd_rsp_value(srcData,len, GET_NET_CELL_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
		if(get_atcmd_rsp_value(srcData, len, GET_NET_CELL_RSP_HEAD, "\r\n", target_data))
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
		if(get_atcmd_rsp_value(srcData,len, GET_NET_QCELL_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
#endif 
    	{
			
			printf("[cell_info]: %s\r\n",target_data);

			token = strtok(target_data, ",");
			// printf("currentIndex = %d\r\n",currentIndex);
			// printf("token:%s\r\n",token);

			while (token != NULL) 
			{
				#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))

					if (currentIndex == GET_NET_LCELL_MCC)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.MCC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.MCC = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.MCC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_MNC)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.MNC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.MNC = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.MNC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_TAC)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.TAC = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.TAC = 0x%04x\r\n",g_deviceGlobalInfo.qeng_servingcell_info.TAC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_cellID)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.cellID = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.cellID = 0x%04x\r\n",g_deviceGlobalInfo.qeng_servingcell_info.cellID);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_PCI)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.PCI = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.PCI = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.PCI);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RX_lev)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.srxlev = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.srxlev = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.srxlev);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_EARFCN ) //当前服务小区频点
					{
						printf("Found string: %s\n", token);

						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RSRP)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.RSRP = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.RSRP = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSRP);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RSRQ)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.RSRQ = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.RSRQ = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSRQ);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_SNR)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.SINR = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.SINR = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.SINR);
						app_delay_ms(100);
					}
				
				#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
					if(currentIndex == GET_NET_QCELL_MCC)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.MCC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.MCC = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.MCC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_MNC)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.MNC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.MNC = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.MNC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_TAC)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.TAC = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.TAC = 0x%04x\r\n",g_deviceGlobalInfo.qeng_servingcell_info.TAC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_cellID)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.cellID = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.cellID = 0x%04x\r\n",g_deviceGlobalInfo.qeng_servingcell_info.cellID);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_PCI)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.PCI = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.PCI = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.PCI);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_RX_lev)
					{
						g_deviceGlobalInfo.qeng_servingcell_info.srxlev = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qeng_servingcell_info.srxlev = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.srxlev);
						app_delay_ms(100);
					}
				#endif 

				token = strtok(NULL, ",");
				currentIndex++;
				// printf("currentIndex = %d\r\n",currentIndex);
				// printf("token:%s\r\n",token);
			}
			// return AT_OK;
		}

		currentIndex = 0;
		memset(target_data,0,sizeof(target_data));

		target_data_start_offset = 0;
		p_target_data_start = strstr(srcData, "neighbourcell");
		if(p_target_data_start!=NULL)
		{
			target_data_start_offset = p_target_data_start - srcData;
			printf("[p_target_data_start]: %d\r\n",p_target_data_start);
			printf("[srcData]: %d\r\n",srcData);
			printf("[target_data_start_offset]: %d\r\n",target_data_start_offset);
		}
		else
		{
			return AT_OK;
		}


		if(get_atcmd_rsp_value(srcData+target_data_start_offset, len-target_data_start_offset, "neighbourcell", "\r\n", target_data))
		{
			printf("[neighbourcell_info]: %s\r\n",target_data);

			token = strtok(target_data, ",");
			// printf("currentIndex = %d\r\n",currentIndex);
			// printf("token:%s\r\n",token);

			g_deviceGlobalInfo.nearbts_count = 1;

			while (token != NULL) 
			{
				#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
					if (currentIndex == 2)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.MCC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.MCC = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.MCC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_MNC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.MNC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.MNC = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.MNC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_TAC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.TAC = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.TAC = 0x%04x\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.TAC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_cellID)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.cellID = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.cellID = 0x%04x\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.cellID);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_PCI)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.PCI = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.PCI = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.PCI);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RX_lev)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.RX_lev = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.RX_lev = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.RX_lev);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_EARFCN ) //当前服务小区频点
					{
						printf("Found string: %s\n", token);

						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RSRP)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.RSRP = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.RSRP = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.RSRP);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RSRQ)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.RSRQ = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.RSRQ = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.RSRQ);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_SNR)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.SINR = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.SINR = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.SINR);
						app_delay_ms(100);
					}
				
				#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))

					if (currentIndex == GET_NET_QCELL_MCC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.MCC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.MCC = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.MCC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_MNC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.MNC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.MNC = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.MNC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_TAC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.TAC = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.TAC = 0x%04x\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.TAC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_cellID)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.cellID = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.cellID = 0x%04x\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.cellID);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_PCI)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.PCI = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.PCI = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.PCI);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_RX_lev)
					{
						g_deviceGlobalInfo.qcell_neighbourcell_info.RX_lev = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell_info.RX_lev = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell_info.RX_lev);
						app_delay_ms(100);
					}

				#endif 

				token = strtok(NULL, ",");
				currentIndex++;

				// printf("currentIndex = %d\r\n",currentIndex);
				// printf("token:%s\r\n",token);
			}
		}

		currentIndex = 0;
		memset(target_data,0,sizeof(target_data));

		p_target_data_start = strstr(srcData+target_data_start_offset+13, "neighbourcell");
		if(p_target_data_start!=NULL)
		{
			target_data_start_offset = p_target_data_start - srcData;
			printf("[p_target_data_start]: %d\r\n",p_target_data_start);
			printf("[srcData]: %d\r\n",srcData);
			printf("[target_data_start_offset]: %d\r\n",target_data_start_offset);
		}
		else
		{
			return AT_OK;
		}


		if(get_atcmd_rsp_value((srcData+target_data_start_offset), len-target_data_start_offset, "neighbourcell", "\r\n", target_data))
		{

			printf("[neighbourcell_2_info]: %s\r\n",target_data);

			token = strtok(target_data, ",");
			// printf("currentIndex = %d\r\n",currentIndex);
			// printf("token:%s\r\n",token);

			g_deviceGlobalInfo.nearbts_count = 2;
			while (token != NULL) 
			{
				#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
					if (currentIndex == 2)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.MCC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.MCC = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.MCC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_MNC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.MNC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.MNC = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.MNC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_TAC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.TAC = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.TAC = 0x%04x\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.TAC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_cellID)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.cellID = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.cellID = 0x%04x\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.cellID);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_PCI)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.PCI = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.PCI = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.PCI);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RX_lev)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.RX_lev = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.RX_lev = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.RX_lev);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_EARFCN ) //当前服务小区频点
					{
						printf("Found string: %s\n", token);

						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RSRP)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.RSRP = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.RSRP = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.RSRP);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_RSRQ)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.RSRQ = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.RSRQ = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.RSRQ);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_LCELL_SNR)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.SINR = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.SINR = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.SINR);
						app_delay_ms(100);
					}
				
				#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))

					if (currentIndex == GET_NET_QCELL_MCC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.MCC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.MCC = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.MCC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_MNC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.MNC = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.MNC = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.MNC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_TAC)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.TAC = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.TAC = 0x%04x\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.TAC);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_cellID)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.cellID = strtol(token,NULL,16);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.cellID = 0x%04x\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.cellID);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_PCI)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.PCI = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.PCI = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.PCI);
						app_delay_ms(100);
					}
					else if (currentIndex == GET_NET_QCELL_RX_lev)
					{
						g_deviceGlobalInfo.qcell_neighbourcell2_info.RX_lev = strtol(token,NULL,10);//转换拿到的值
						printf("g_deviceGlobalInfo.qcell_neighbourcell2_info.RX_lev = %d\r\n",g_deviceGlobalInfo.qcell_neighbourcell2_info.RX_lev);
						app_delay_ms(100);
					}

				#endif 

				token = strtok(NULL, ",");
				currentIndex++;

				// printf("currentIndex = %d\r\n",currentIndex);
				// printf("token:%s\r\n",token);
			}
		}

		return AT_OK;
	}
            
    return AT_ERR;
}




#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
/**
 * @brief 获取当前获取主服务小区信息
 * 
 * RSRP:接收信号强度
 * TAC：区域跟踪码
 * CELLID：基站编号
 * 移远模组需要通过该指令拿到RSRP
 * @return void
 */

static AT_RET_E net4g_get_net_qeng_serving_cell_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
     uint8_t target_data[500] = {0};
	char *token;
    uint8_t currentIndex = 0;
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, GET_NET_QENG_SERVING_CELL_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	{
			//printf("[cell_info]: %s\r\n",target_data);
			token = strtok(target_data, ",");
			while (token != NULL) 
			{
				if(currentIndex == GET_NET_QENG_servingcell_is_tdd)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.is_tdd[0] = token[1];
					g_deviceGlobalInfo.qeng_servingcell_info.is_tdd[1] = token[2];
					g_deviceGlobalInfo.qeng_servingcell_info.is_tdd[2] = token[3];
					g_deviceGlobalInfo.qeng_servingcell_info.is_tdd[3] = 0;
					printf("g_deviceGlobalInfo.qeng_servingcell_info.is_tdd = %s\r\n",g_deviceGlobalInfo.qeng_servingcell_info.is_tdd);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_MCC)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.MCC = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.MCC = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.MCC);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_MNC)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.MNC = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.MNC = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.MNC);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_cellID)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.cellID = strtol(token,NULL,16);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.cellID = 0x%04x\r\n",g_deviceGlobalInfo.qeng_servingcell_info.cellID);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_PCI)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.PCI = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.PCI = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.PCI);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_EARFCN)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.EARFCN = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.EARFCN = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.EARFCN);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_freq_band_ind)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.freq_band_ind = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.freq_band_ind = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.freq_band_ind);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_UL_bandwidth)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.UL_bandwidth = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.UL_bandwidth = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.UL_bandwidth);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_DL_bandwidth)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.DL_bandwidth = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.DL_bandwidth = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.DL_bandwidth);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_TAC)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.TAC = strtol(token,NULL,16);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.TAC = 0x%04x\r\n",g_deviceGlobalInfo.qeng_servingcell_info.TAC);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_RSRP)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.RSRP = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.RSRP = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSRP);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_RSRQ)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.RSRQ = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.RSRQ = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSRQ);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_RSSI)
				{
					// g_deviceGlobalInfo.qeng_servingcell_info.RSSI = strtol(token,NULL,10);//转换拿到的值
					// printf("g_deviceGlobalInfo.qeng_servingcell_info.RSSI = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSSI);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_SINR)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.SINR = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.SINR = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.SINR);
					app_delay_ms(100);
				}
				else if (currentIndex == GET_NET_QENG_servingcell_srxlev)
				{
					g_deviceGlobalInfo.qeng_servingcell_info.srxlev = strtol(token,NULL,10);//转换拿到的值
					printf("g_deviceGlobalInfo.qeng_servingcell_info.srxlev = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.srxlev);
					app_delay_ms(100);
				}

				// if(currentIndex == GET_NET_QENG_servingcell_RSRP)
				// {
				// 	g_deviceGlobalInfo.cell_rsrp = strtol(token,NULL,10);//转换拿到的值		
				// 	printf("RSRP = %d\r\n",g_deviceGlobalInfo.cell_rsrp);
				// 	app_delay_ms(100);
				// }
				token = strtok(NULL, ",");
				//printf("currentIndex = %d\r\n",currentIndex);
				currentIndex++;
			}
			return AT_OK;
		}
    }
							
	return AT_ERR;
}


static AT_RET_E net4g_get_net_qeng_neighbour_cell_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
     uint8_t target_data[500] = {0};
	char *token;
    uint8_t currentIndex = 0;
    if(ret == AT_OK) 
    {
    	if(get_atcmd_rsp_value(srcData,len, GET_NET_QENG_NEIGHBOURCELL_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	{
			//printf("[cell_info]: %s\r\n",target_data);
			token = strtok(target_data, ",");
			while (token != NULL) 
			{
				// if (currentIndex == GET_NET_QENG_neighbourcell_EARFCN)
				// {
				// 	g_deviceGlobalInfo.qeng_neighbourcell_info.EARFCN = strtol(token,NULL,10);//转换拿到的值
				// 	printf("g_deviceGlobalInfo.qeng_neighbourcell_info.EARFCN = %d\r\n",g_deviceGlobalInfo.qeng_neighbourcell_info.EARFCN);
				// 	app_delay_ms(100);
				// }
				// else if (currentIndex == GET_NET_QENG_neighbourcell_PCI)
				// {
				// 	g_deviceGlobalInfo.qeng_neighbourcell_info.PCI = strtol(token,NULL,10);//转换拿到的值
				// 	printf("g_deviceGlobalInfo.qeng_neighbourcell_info.PCI = %d\r\n",g_deviceGlobalInfo.qeng_neighbourcell_info.PCI);
				// 	app_delay_ms(100);
				// }
				// else if (currentIndex == GET_NET_QENG_neighbourcell_RSRP)
				// {
				// 	g_deviceGlobalInfo.qeng_neighbourcell_info.RSRP = strtol(token,NULL,10);//转换拿到的值
				// 	printf("g_deviceGlobalInfo.qeng_neighbourcell_info.RSRP = %d\r\n",g_deviceGlobalInfo.qeng_neighbourcell_info.RSRP);
				// 	app_delay_ms(100);
				// }
				// else if (currentIndex == GET_NET_QENG_neighbourcell_RSRQ)
				// {
				// 	g_deviceGlobalInfo.qeng_neighbourcell_info.RSRQ = strtol(token,NULL,10);//转换拿到的值
				// 	printf("g_deviceGlobalInfo.qeng_neighbourcell_info.RSRQ = %d\r\n",g_deviceGlobalInfo.qeng_neighbourcell_info.RSRQ);
				// 	app_delay_ms(100);
				// }

				token = strtok(NULL, ",");
				//printf("currentIndex = %d\r\n",currentIndex);
				currentIndex++;
			}
			return AT_OK;
		}
    }
							
	return AT_ERR;
}

#endif



#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
/**
 * @brief 获取当前获取主服务小区信息
 * 
 * RSRP:接收信号强度
 * TAC：区域跟踪码
 * CELLID：基站编号
 * 移远模组需要通过该指令拿到RSRP
 * @return void
 */

static AT_RET_E net4g_get_net_qeng_serving_cell_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
     uint8_t target_data[500] = {0};
	char *token;
    uint8_t currentIndex = 0;
    if(ret == AT_OK) 
    {
    // 	if(get_atcmd_rsp_value(srcData,len, GET_NET_QENG_SERVING_CELL_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    // 	{
	// 		token = strtok(target_data, ",");
	// 		while (token != NULL) 
	// 		{
	// 			if(currentIndex == GET_NET_QENG_servingcell_is_tdd)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.is_tdd[0] = token[1];
	// 				g_deviceGlobalInfo.qeng_servingcell_info.is_tdd[1] = token[2];
	// 				g_deviceGlobalInfo.qeng_servingcell_info.is_tdd[2] = token[3];
	// 				g_deviceGlobalInfo.qeng_servingcell_info.is_tdd[3] = 0;
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.is_tdd = %s\r\n",g_deviceGlobalInfo.qeng_servingcell_info.is_tdd);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_MCC)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.MCC = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.MCC = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.MCC);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_MNC)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.MNC = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.MNC = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.MNC);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_cellID)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.cellID = strtol(token,NULL,16);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.cellID = 0x%04x\r\n",g_deviceGlobalInfo.qeng_servingcell_info.cellID);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_PCI)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.PCI = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.PCI = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.PCI);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_EARFCN)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.EARFCN = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.EARFCN = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.EARFCN);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_freq_band_ind)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.freq_band_ind = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.freq_band_ind = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.freq_band_ind);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_UL_bandwidth)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.UL_bandwidth = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.UL_bandwidth = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.UL_bandwidth);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_DL_bandwidth)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.DL_bandwidth = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.DL_bandwidth = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.DL_bandwidth);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_TAC)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.TAC = strtol(token,NULL,16);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.TAC = 0x%04x\r\n",g_deviceGlobalInfo.qeng_servingcell_info.TAC);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_RSRP)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.RSRP = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.RSRP = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSRP);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_RSRQ)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.RSRQ = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.RSRQ = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSRQ);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_RSSI)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.RSSI = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.RSSI = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSSI);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_SINR)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.SINR = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.SINR = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.SINR);
	// 				app_delay_ms(100);
	// 			}
	// 			else if (currentIndex == GET_NET_QENG_servingcell_srxlev)
	// 			{
	// 				g_deviceGlobalInfo.qeng_servingcell_info.srxlev = strtol(token,NULL,10);//转换拿到的值
	// 				printf("g_deviceGlobalInfo.qeng_servingcell_info.srxlev = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.srxlev);
	// 				app_delay_ms(100);
	// 			}

	// 			token = strtok(NULL, ",");
	// 			currentIndex++;
	// 		}
	// 		return AT_OK;
	// 	}
    }
							
	return AT_ERR;
}


static AT_RET_E net4g_get_net_qeng_neighbour_cell_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
     uint8_t target_data[500] = {0};
	char *token;
    uint8_t currentIndex = 0;
    if(ret == AT_OK) 
    {
    	// if(get_atcmd_rsp_value(srcData,len, GET_NET_QENG_NEIGHBOURCELL_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
    	// {
		// 	token = strtok(target_data, ",");
		// 	while (token != NULL) 
		// 	{
		// 		if (currentIndex == GET_NET_QENG_neighbourcell_EARFCN)
		// 		{
		// 			g_deviceGlobalInfo.qeng_neighbourcell_info.EARFCN = strtol(token,NULL,10);//转换拿到的值
		// 			printf("g_deviceGlobalInfo.qeng_neighbourcell_info.EARFCN = %d\r\n",g_deviceGlobalInfo.qeng_neighbourcell_info.EARFCN);
		// 			app_delay_ms(100);
		// 		}
		// 		else if (currentIndex == GET_NET_QENG_neighbourcell_PCI)
		// 		{
		// 			g_deviceGlobalInfo.qeng_neighbourcell_info.PCI = strtol(token,NULL,10);//转换拿到的值
		// 			printf("g_deviceGlobalInfo.qeng_neighbourcell_info.PCI = %d\r\n",g_deviceGlobalInfo.qeng_neighbourcell_info.PCI);
		// 			app_delay_ms(100);
		// 		}
		// 		else if (currentIndex == GET_NET_QENG_neighbourcell_RSRQ)
		// 		{
		// 			g_deviceGlobalInfo.qeng_neighbourcell_info.RSRQ = strtol(token,NULL,10);//转换拿到的值
		// 			printf("g_deviceGlobalInfo.qeng_neighbourcell_info.RSRQ = %d\r\n",g_deviceGlobalInfo.qeng_neighbourcell_info.RSRQ);
		// 			app_delay_ms(100);
		// 		}
		// 		else if (currentIndex == GET_NET_QENG_neighbourcell_RSRP)
		// 		{
		// 			g_deviceGlobalInfo.qeng_neighbourcell_info.RSRP = strtol(token,NULL,10);//转换拿到的值
		// 			printf("g_deviceGlobalInfo.qeng_neighbourcell_info.RSRP = %d\r\n",g_deviceGlobalInfo.qeng_neighbourcell_info.RSRP);
		// 			app_delay_ms(100);
		// 		}

		// 		if(currentIndex == GET_NET_QENG_servingcell_RSRP)
		// 		{
		// 			g_deviceGlobalInfo.cell_rsrp = strtol(token,NULL,10);//转换拿到的值		
		// 			printf("RSRP = %d\r\n",g_deviceGlobalInfo.cell_rsrp);
		// 			app_delay_ms(100);
		// 		}
		// 		token = strtok(NULL, ",");
		// 		currentIndex++;
		// 	}
		// 	return AT_OK;
		// }
    }
							
	return AT_ERR;
}

#endif



static AT_RET_E net4g_get_location_configPara(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
/* +LBSPARA: http://locator-aep.xiot.senthink.com:80/locator/v0.1/locate,B84E427D95B1DF4A3F49B18DDF714C72 */

	uint8_t target_data[150] = {0};
	uint8_t lbs_url[100] = {0};
	uint8_t lbs_token[50] = {0};
	uint8_t* addr_comma = NULL;
	
    if(ret == AT_OK) 
    {
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
    	if(get_atcmd_rsp_value(srcData,len, "+LBSPARA: ", "\r\n", target_data))
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
		if(get_atcmd_rsp_value(srcData,len, "+QLBSCFG:", "\r\n", target_data))
#endif
    	{
			memcpy(lbs_url, target_data, strlen(target_data));
			addr_comma = strchr(lbs_url, ',');
			if(addr_comma != NULL)
			{
				memcpy(lbs_url, lbs_url, addr_comma-lbs_url);
				lbs_url[addr_comma-lbs_url] = '\0';
				strcpy(lbs_token, addr_comma+1);
				//printf("url   = %s\r\n", lbs_url);
				//printf("token = %s\r\n", lbs_token);
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
				if(0==strcmp(lbs_token,XD_LOCATION_PARA_TOKEN))
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
				if(NULL!= strstr(lbs_token, "****************"))
#endif
				{
					return AT_OK;
				}
			}
		}
	}
            
    return AT_ERR;
}


void net4g_location_config_cmd_pack(uint8_t *destData, uint16_t destLen)
{
	memset(destData, 0, destLen);
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))    
    snprintf(destData, destLen,NET_CMD_LOCATION_PARA_SET, "http://locator-aep.xiot.senthink.com:80/locator/v0.1/locate", XD_LOCATION_PARA_TOKEN);
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU)) 
	snprintf(destData, destLen,NET_CMD_LOCATION_PARA_SET, XD_LOCATION_PARA_TOKEN);
#endif
}


uint8_t* get_location_addr(void)
{
	return g_deviceGlobalInfo.location_addr;
}

static AT_RET_E net4g_get_net_location_addr(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
/*
[rsp]
+LBS: 120.0151728501375,30.28261134288196

OK

*/
	uint8_t target_data[50] = {0};
	uint8_t addr_long[25] = {0};
	uint8_t addr_lati[25] = {0};
	uint8_t* addr_comma = NULL;
	uint8_t rssiValue = 0;
	
    if(ret == AT_OK) 
    {
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))       
    	if(get_atcmd_rsp_value(srcData,len, "+LBS: ", "\r\n", target_data))
    	{
    		memset(g_deviceGlobalInfo.location_addr, 0, sizeof(g_deviceGlobalInfo.location_addr));
			memcpy(g_deviceGlobalInfo.location_addr, target_data, strlen(target_data));
			if(strlen(g_deviceGlobalInfo.location_addr) > 21)
			{
				addr_comma = strchr(g_deviceGlobalInfo.location_addr, ',');
				if(addr_comma != NULL)
				{
					memcpy(addr_long, g_deviceGlobalInfo.location_addr, addr_comma-g_deviceGlobalInfo.location_addr);
					strcpy(addr_lati, addr_comma+1);
					memset(g_deviceGlobalInfo.location_addr, 0, sizeof(g_deviceGlobalInfo.location_addr));
					memcpy(g_deviceGlobalInfo.location_addr, addr_lati, 10);
					memcpy(g_deviceGlobalInfo.location_addr+10, ",", 1);
					memcpy(g_deviceGlobalInfo.location_addr+11, addr_long, 10);
					printf("[localtion]: %s\r\n",g_deviceGlobalInfo.location_addr);
					return AT_OK;
				}
			}
		}
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))   
		if(get_atcmd_rsp_value(srcData,len, "+QLBS", "\r\n", target_data))
		{
			if(get_at_rsp_keywords(target_data, ":", 2, addr_lati))
			{
				memset(target_data, 0, sizeof(target_data));
				get_atcmd_rsp_value(srcData,len, "+QLBS", "\r\n", target_data);
				if(get_at_rsp_keywords(target_data, ":", 3, addr_long))
				{
					snprintf(g_deviceGlobalInfo.location_addr,sizeof(g_deviceGlobalInfo.location_addr), "%s,%s", addr_lati, addr_long);
					printf("[localtion]: %s\r\n",g_deviceGlobalInfo.location_addr);
					return AT_OK;
				}
			}
		}
#endif
	}
    return AT_ERR;
}


AT_RET_E at_cmd_send_check(at_cmd_t *cmd, at_cmd_handler_t cb_func)
{
	AT_RET_E ret = AT_ERR;
	AT_RET_E cbRet = AT_ERR;
	uint16_t recvLen = 0;
	uint8_t i;
	
	if(cmd == NULL)
	{
		return ret;
	}

	for(i=0; i < cmd->retry_times; i++)
	{
		//send cmd
		network_uart_send(cmd->p_cmd, strlen(cmd->p_cmd));
		app_delay_ms(cmd->interval_ms);

		//recv rsp
		memset(gNet4g_dataStruct.recv.fifo.data, 0, sizeof(gNet4g_dataStruct.recv.fifo.data));
		recvLen = network_uart_recv(gNet4g_dataStruct.recv.fifo.data, sizeof(gNet4g_dataStruct.recv.fifo.data));
		if(recvLen> 0)
		{
			NET4G_DEBUG("[AT] RECV_%d: %s\r\n", recvLen, gNet4g_dataStruct.recv.fifo.data);
			if(NULL != strstr(gNet4g_dataStruct.recv.fifo.data, cmd->p_ack))
			{
				// uint8_t* serverCmdPtr = strstr(gNet4g_dataStruct.recv.fifo.data, NET_MQTT_CTRL_CMD_HEAD); // slot forbidden cmd
				// if(NULL != serverCmdPtr)
				// {
				// 	memcpy(gNet4g_dataStruct.recv.fifo.data, serverCmdPtr, strlen(serverCmdPtr));
				// 	gNet4g_dataStruct.recv.fifo.data[strlen(serverCmdPtr)] = '\0';

				// 	if(get_at_rsp_keywords( gNet4g_dataStruct.recv.fifo.data, NET_MQTT_CTRL_CMD_HEAD, NET4G_CMD_DATA_KEYWORD_INDEX, gNet4g_dataStruct.recv.fifo.data))
				// 	{
				// 		memset(g_payload, 0, sizeof(g_payload));
				// 		cloud_cmd_data_decode(gNet4g_dataStruct.recv.fifo.data, 19, g_deviceGlobalInfo.mqtt.token, g_payload);
				// 		cloud_cmd_json_parse(g_payload);
				// 		memset(g_payload, 0, sizeof(g_payload));
				// 	}
				// 	else
				// 	{
				// 		NET4G_DEBUG("get slot forbid cmd error\r\n");
				// 	}
				// }
				
				if(NULL != cb_func)
				{
					cbRet  = cb_func(AT_OK, gNet4g_dataStruct.recv.fifo.data, recvLen);
					if(cbRet == AT_OK)
					{
						ret = AT_OK;
						break;
					}
				}
				else
				{
					ret = AT_OK;
					break;
				}
			}
			memset(gNet4g_dataStruct.recv.fifo.data, 0, sizeof(gNet4g_dataStruct.recv.fifo.data));
		}
	}
	return ret;
}


PROCESS_STEP_RET_E module_4g_config_para_process(void)
{
	AT_RET_E ret = AT_ERR;
	uint8_t cmd_data_buff[150] = {0};
	
	if(net4G_process_steps.netConfig >= sizeof(net4g_config_command_steps)/sizeof(net4g_config_command_steps[0]))
    {
        NET4G_DEBUG("[4G_config] step error %d\n", net4G_process_steps.netConfig);
        return PROCESS_ERROR;
    }
	
	at_cmd_t cmd = module_4g_cmds[net4g_config_command_steps[net4G_process_steps.netConfig].id]; 
	// NET4G_DEBUG("[4G_config] net4G_process_steps.netConfig = %d\n", net4G_process_steps.netConfig);
	// NET4G_DEBUG("[4G_config] net4g_config_command_steps[net4G_process_steps.netConfig].id= %d\n", net4g_config_command_steps[net4G_process_steps.netConfig].id);
	// NET4G_DEBUG("[4G_config] cmd.p_cmd= %s\n", cmd.p_cmd);

	switch(net4g_config_command_steps[net4G_process_steps.netConfig].id)
	{
	    case CMD_SIM_CHECK://获取当前使用的SIM卡
			ret = at_cmd_send_check(&cmd, net4g_get_sim_using_cb);
			break;

		case CMD_ID_GET_MODULE_INFO:
			ret = at_cmd_send_check(&cmd, net4g_get_module_info_cb);
			break;

		case CMD_ID_GET_IMEI:
			ret = at_cmd_send_check(&cmd, net4g_get_module_imei_cb);
			break;

		case CMD_ID_GET_SN_NO:
			at_cmd_send_check(&cmd, net4g_get_module_sn_cb);

		//增加逻辑 如果是断网重连的状态（标志位被使能，则直接进行切卡逻辑）net4G_process_steps.netConfig直接改

		#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
	        NET4G_DEBUG("[Using  first]SIM %d\r\n",gSysInfo_struct.net4G.first_sim);
            if(net4g_reconnect_flag == 1)
			{
				net4g_reconnect_flag = 0;
				
				//如果此时使用SIM1联网
				if (g_deviceGlobalInfo.simFlag == 1)
				{
					g_deviceGlobalInfo.sim1State = 2;
					printf("sim1 state = %d\r\n",g_deviceGlobalInfo.sim1State);
					net4G_process_steps.netConfig = 19;//跳转到SIM2做业务的步骤
					break;
				}
				else if(g_deviceGlobalInfo.simFlag == 2)
				{
					g_deviceGlobalInfo.sim2State = 2;
					printf("sim2 state = %d\r\n",g_deviceGlobalInfo.sim2State);
					net4G_process_steps.netConfig = 12;//跳转到SIM1做业务的步骤
					break;
				}

				ret =5;
			}

			if(gSysInfo_struct.net4G.first_sim == 1)
			{
				net4G_process_steps.netConfig = 12;
				ret =5;
			}
			else if (gSysInfo_struct.net4G.first_sim == 2)
			{
				net4G_process_steps.netConfig = 5;
				ret =5;
			}
//			else if (ret == 5)
//			{
//				//未配置优先级的但是断网的情况，避免ret值被修改为AT_OK 
//			}
			else
			{
				ret = AT_OK;
			}
		#endif

		#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801))
             
			if(net4g_reconnect_flag == 1)	//发生了断网重连
			{
				net4g_reconnect_flag = 0;
				ret =5;
				if (g_deviceGlobalInfo.simFlag == 1)//如果此时使用SIM1联网
				{
					g_deviceGlobalInfo.sim1State = 2;
					printf("sim1 state = %d\r\n",g_deviceGlobalInfo.sim1State);
					net4G_process_steps.netConfig = 21;//跳转到SIM2做业务的步骤
					break;
				}
				else if(g_deviceGlobalInfo.simFlag == 2)//如果此时使用SIM2联网
				{
					g_deviceGlobalInfo.sim2State = 2;
					printf("sim2 state = %d\r\n",g_deviceGlobalInfo.sim2State);
					net4G_process_steps.netConfig = 15;//跳转到SIM1做业务的步骤
					break;
				}
			}
		
			if(gSysInfo_struct.net4G.first_sim == 1)		//SIM卡优先级 优先1
			{
				ret =5;
				net4G_process_steps.netConfig = 15;
			}
			else if (gSysInfo_struct.net4G.first_sim == 2)	//SIM卡优先级 优先2
			{
				ret =5;
				net4G_process_steps.netConfig = 9;
			}
//      	else if (ret == 5)
//			{
//				//未配置优先级的但是断网的情况，避免ret值被修改为AT_OK 
//			}
			else
			{
				ret = AT_OK;
			}
		#endif

		#if (NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU)
			ret = AT_OK;
		#endif

			break;	
		
		case CMD_ID_IS_SIM_OK:
			ret = at_cmd_send_check(&cmd, net4g_is_sim_ok_cb);
			break;
		
		case CMD_ID_GET_SIM_NO:
			ret = at_cmd_send_check(&cmd, net4g_get_sim_no_cb);
			break;
		
		case CMD_ID_GET_SIM_STATE:
			ret = at_cmd_send_check(&cmd, net4g_get_sim_state_cb);
			break;
		
		case CMD_ID_GET_NET_STATE:
			ret = at_cmd_send_check(&cmd, net4g_get_net_state_cb);
			
		#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
			//配置了SIM2优先，且SIM2状态正常，并且成功读取到了SIM卡1的卡号则结束SIM号获取流程，开始正常业务。
			if ((ret == AT_OK)&&(gSysInfo_struct.net4G.first_sim == 2)&&(strlen(gSysInfo_struct.net4G.iccid)<32))
			{
				 ret = 5;
				 net4G_process_steps.netConfig =  26;	
			}
		#endif
			
		#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801))
			if ((ret == AT_OK)&&(gSysInfo_struct.net4G.first_sim == 2)&&(strlen(gSysInfo_struct.net4G.iccid)<32))
			{
				 ret = 5;
				 net4G_process_steps.netConfig =  27;	
			}
		#endif 
			break;


		#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
			case CMD_ID_GET_CELL:
				ret = at_cmd_send_check(&cmd, net4g_get_net_qcell_cb);
				break;
			// case CMD_ID_GET_ENG_SERVINGCELL:
			// 	ret = at_cmd_send_check(&cmd, net4g_get_net_qeng_serving_cell_cb);
			// 	break;
		#endif


		#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
		case CMD_ID_GET_QCELL:
			ret = at_cmd_send_check(&cmd, net4g_get_net_qcell_cb);
			break;
		case CMD_ID_GET_QENG_SERVINGCELL:
			ret = at_cmd_send_check(&cmd, net4g_get_net_qeng_serving_cell_cb);
			break;
		case CMD_ID_GET_QENG_NEIGHBOURCELL:
			ret = at_cmd_send_check(&cmd, net4g_get_net_qeng_neighbour_cell_cb);
			break;
		#endif

		case CMD_ID_GET_RSSI:
			ret = at_cmd_send_check(&cmd, net4g_get_net_rssi_cb);
			break;
		
		case CMD_ID_GET_LOCATION_PARA:
			ret = at_cmd_send_check(&cmd, net4g_get_location_configPara);
			break;

		case CMD_ID_SET_LOCATION_PARA:
			net4g_location_config_cmd_pack(cmd_data_buff, sizeof(cmd_data_buff));
			printf("set cmd: %s\r\n", cmd_data_buff);
			cmd.p_cmd = cmd_data_buff;
			ret = at_cmd_send_check(&cmd, NULL);
			break;

		case CMD_ID_GET_LOCATION_ADDR:
			ret = at_cmd_send_check(&cmd, net4g_get_net_location_addr);
			break;
		
		default:
			ret = at_cmd_send_check(&cmd, NULL);
			break;
	}

	if(AT_OK == ret)
    {
		net4G_process_steps.netConfig = net4g_config_command_steps[net4G_process_steps.netConfig].s_jump;
	}
	else if (ret == 5)
	{

	}
    else
    {
        net4G_process_steps.netConfig = net4g_config_command_steps[net4G_process_steps.netConfig].f_jump;
    }

	// NET4G_DEBUG("[4G_config] net4G_process_steps.netConfig = %d\n", net4G_process_steps.netConfig);

	if(net4g_config_command_steps[net4G_process_steps.netConfig].id == CMD_ID_SUCCESS)
	{
		net4G_process_steps.netConfig = 0;
		return PROCESS_DONE;
	}
	else if(net4g_config_command_steps[net4G_process_steps.netConfig].id == CMD_ID_ERROR)
	{
		net4G_process_steps.netConfig = 0;
		return PROCESS_ERROR;
	}

	return PROCESS_WAIT;
}



/****************************************************************************************************************************
*ACTIVE AND ROUTE
*****************************************************************************************************************************/
extern activeRsp gstActiveRsp;
extern char *pucOriginSecret;

static AT_RET_E net4g_active_post_rsp_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
	uint8_t target_data[200] = {0};
	uint8_t buffer[200] = {0};
	uint8_t t_value[20] = {0};
	AT_RET_E func_ret = AT_ERR;

	if (get_atcmd_rsp_value(srcData, len, NET_ROUTE_ACTIVE_RSP_HEAD, NET_ROUTE_ACTIVE_RSP_TAIL, target_data))
	{
#if 1
		
        char md5[33];
        char sign[17];
        
        printf("111len[%d]\r\n", strlen(target_data));
		memset(buffer, 0, 200);
		memcpy(buffer, target_data, strlen(target_data));
		activeRsp rsp = {0};
		pb_istream_t istream = pb_istream_from_buffer(buffer, strlen(buffer));
		if (pb_decode(&istream, activeRsp_fields, &rsp) == false)
		{
			fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&istream));
            printf("ACTIVE post rsp decoding failed\r\n");
			return func_ret; /* Failure */
		}

		printf("after decode key[%s] Secret[%s] No[%s] sign[%s] \r\n", rsp.data.deviceKey, rsp.data.deviceSecret, rsp.data.deviceNo, rsp.data.sign);

		memcpy(&gstActiveRsp, &rsp, sizeof(rsp));

		memcpy(gSysLoginInfo.DeviceKey, rsp.data.deviceKey, strlen(rsp.data.deviceKey));
		memcpy(gSysLoginInfo.DeviceSecret, rsp.data.deviceSecret, strlen(rsp.data.deviceSecret));
		memcpy(gSysLoginInfo.deviceNo, rsp.data.deviceNo, strlen(rsp.data.deviceNo));

		pb_release(activeRsp_fields, &rsp);

		printf("66test key[%s] Secret[%s] No[%s] sign[%s] t[%lld]\r\n", gSysLoginInfo.DeviceKey, gSysLoginInfo.DeviceSecret, gSysLoginInfo.deviceNo, gstActiveRsp.data.sign, gstActiveRsp.data.t);

        memset(buffer, 0, sizeof(buffer));
    	snprintf((char *)buffer, sizeof(buffer), "deviceKey=%s||deviceSecret=%s||t=%lld%s", gSysLoginInfo.DeviceKey, gSysLoginInfo.DeviceSecret, gstActiveRsp.data.t, pucOriginSecret);

    	memset(md5, 0, sizeof(md5));
    	get_md5_32_str((unsigned char *)buffer, strlen(buffer), (unsigned char *)md5);


    	memset(sign, 0, sizeof(sign));
    	memcpy(sign, md5 + 8, 16);

        printf("sign[%s]\r\n", sign);
        if(0 == memcmp(sign, gstActiveRsp.data.sign, 16))
        {
    		gSysLoginInfo.activeFlag = 0xBB;
			tls_fls_write(DEVICE_LOGIN_INFO_FLASH_ADDR, (u8*)&gSysLoginInfo, sizeof(gSysLoginInfo));
			tls_fls_write(DEVICE_LOGIN_INFO_BACK_FLASH_ADDR, (u8*)&gSysLoginInfo, sizeof(gSysLoginInfo));
			
            printf("ACTIVE success\r\n");
			func_ret = AT_OK;
        }
        else
        {
            printf("ACTIVE fail\r\n");
        }
        


		
#else
		memset(buffer, 0, 200);
		memcpy(buffer, target_data, strlen(target_data));
		activeRsp rsp = {0};
		pb_ostream_t ostream = pb_ostream_from_buffer(buffer, strlen(target_data));
		if (pb_decode(&ostream, activeRsp_fields, &rsp) == false)
		{
			fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&ostream));
			// return 1; /* Failure */
		}

		printf("解码后 key[%s] Secret[%s] sign[%s] \r\n", rsp.data.deviceKey, rsp.data.deviceSecret, rsp.data.sign);

		pb_release(activeRsp_fields, &rsp);
#endif

		// func_ret = device_active_post_rsp_cb(target_data, t_value);
		//}
	}
	else
	{
		NET4G_DEBUG("[active_rsp]: data error\r\n");
	}

	return func_ret;
}


static AT_RET_E net4g_active_confirm_rsp_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
	uint8_t target_data[200] = {0};
	uint8_t buffer[200] = {0};
	AT_RET_E func_ret = AT_ERR;

	if (get_atcmd_rsp_value(srcData, len, NET_ROUTE_ACTIVE_RSP_HEAD, NET_ROUTE_ACTIVE_RSP_TAIL, target_data))
	{

		printf("333len[%d]\r\n", strlen(target_data));

		memset(buffer, 0, 200);
		memcpy(buffer, target_data, strlen(target_data));
		activeConfirmRsp stConfirmrsp = {0};
		pb_istream_t istream = pb_istream_from_buffer(buffer, strlen(target_data));
		if (pb_decode(&istream, activeConfirmRsp_fields, &stConfirmrsp) == false)
		{
			fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&istream));
            printf("[active_confirm_rsp]: data Decoding failed\r\n");
            pb_release(activeConfirmRsp_fields, &stConfirmrsp);
            return func_ret; /* Failure */
		}

		printf("[yes] Confirm success[%d] code[%ld] msg[%s] \r\n", stConfirmrsp.success, stConfirmrsp.code, stConfirmrsp.msg);

        if(1 == stConfirmrsp.success)
        {
            gSysLoginInfo.activeFlag = 0xAA;
            tls_fls_write(DEVICE_LOGIN_INFO_FLASH_ADDR, (u8*)&gSysLoginInfo, sizeof(gSysLoginInfo));
            tls_fls_write(DEVICE_LOGIN_INFO_BACK_FLASH_ADDR, (u8*)&gSysLoginInfo, sizeof(gSysLoginInfo));
            gSysInfo_struct.net4G.activeFlag =0xAA;
            tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));

			// BT_MUSIC_get_bt_info();													//获取蓝牙音箱蓝牙信息

			func_ret = AT_OK;
        }
        else
        {
            gSysLoginInfo.activeFlag = 0;
        }

		pb_release(activeConfirmRsp_fields, &stConfirmrsp);

		// func_ret = device_active_confirm_rsp_cb(target_data);
	}
	else
	{
		NET4G_DEBUG("[active_confirm_rsp]: data error\r\n");
	}

	return func_ret;
}


static AT_RET_E device_route_rsp_cb(AT_RET_E ret, uint8_t *srcData, uint32_t len)
{
	uint8_t target_data[200] = {0};
	uint8_t buffer[200] = {0};
	AT_RET_E func_ret = AT_ERR;

	if (get_atcmd_rsp_value(srcData, len, NET_ROUTE_ACTIVE_RSP_HEAD, NET_ROUTE_ACTIVE_RSP_TAIL, target_data))
	{
		//func_ret = device_route_rsp_parse(target_data);
		printf("route rsp len[%d]\r\n", strlen(target_data));

		memset(buffer, 0, 200);
		memcpy(buffer, target_data, strlen(target_data));

        // printf("\r\n");
        // for(uint8_t u8i=0; u8i<strlen(buffer);u8i++)
        // {
        //     printf("0x%02x", buffer[u8i]);//test by y04485
        // }
        // printf("\r\n");

        
        
		routeRsp stRoutersp = {0};
		pb_istream_t istream = pb_istream_from_buffer(buffer, strlen(target_data));
		if (pb_decode(&istream, routeRsp_fields, &stRoutersp) == false)
		{
			fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&istream));
            NET4G_DEBUG("[route_rsp]: data Decoding failed\r\n");
            pb_release(routeRsp_fields, &stRoutersp);
		    return func_ret; /* Failure */
		}


        memcpy(&gstRouteRsp, &stRoutersp, sizeof(routeRsp));

		printf("[yes1] route success[%d] code[%ld] msg[%s] \r\n", gstRouteRsp.success, gstRouteRsp.code, gstRouteRsp.msg);
		// printf("[yes1] route broker[%s] t[%lld] sign[%s] keepalive[%ld] reactive[%ld] maxReportDelay[%ld]\r\n", \
		// 	gstRouteRsp.data.broker, gstRouteRsp.data.t, gstRouteRsp.data.sign, \
		// 	gstRouteRsp.data.keepalive, gstRouteRsp.data.reactive,gstRouteRsp.data.maxReportDelay);

       

        uint8_t u8szMqttAddrTemp[32] = {0};
        char ucszMqttPortTemp[10] = {0};
        uint16_t u16MqttPort =0;

        memset(u8szMqttAddrTemp, 0 ,sizeof(u8szMqttAddrTemp));
        memset(ucszMqttPortTemp, 0 ,sizeof(ucszMqttPortTemp));
        if(get_atcmd_rsp_before_after_value(gstRouteRsp.data.broker,0, ":", u8szMqttAddrTemp, ucszMqttPortTemp))
        {
            // printf("addrtest[%s],u8port[%s]\r\n", u8szMqttAddrTemp, ucszMqttPortTemp);
            u16MqttPort = atoi(ucszMqttPortTemp);
        }

		g_deviceGlobalInfo.mqtt.port = 0;
		g_deviceGlobalInfo.mqtt.timerReportInterval = 0;
		g_deviceGlobalInfo.mqtt.retryTimes = 0;
		g_deviceGlobalInfo.mqtt.retryInterval = 0;
		g_deviceGlobalInfo.mqtt.HBInterval = 0;
		
        memset((char*)&g_deviceGlobalInfo.mqtt.addr, 0, sizeof(g_deviceGlobalInfo.mqtt.addr));
		memset((char*)&g_deviceGlobalInfo.mqtt.password, 0, sizeof(g_deviceGlobalInfo.mqtt.password));
		memset((char*)&g_deviceGlobalInfo.mqtt.token, 0, sizeof(g_deviceGlobalInfo.mqtt.token));
		memset((char*)&g_deviceGlobalInfo.mqtt.secretKey, 0, sizeof(g_deviceGlobalInfo.mqtt.secretKey));

        memcpy(g_deviceGlobalInfo.mqtt.addr, u8szMqttAddrTemp, strlen(u8szMqttAddrTemp));
        g_deviceGlobalInfo.mqtt.port = u16MqttPort;
        g_deviceGlobalInfo.mqtt.HBInterval = gstRouteRsp.data.keepalive;
        g_deviceGlobalInfo.mqtt.retryInterval = 60;
        g_deviceGlobalInfo.mqtt.retryTimes = 3;

		if((gSysInfo_struct.cubbyInterval>300)&&(gSysInfo_struct.cubbyInterval<0xffff))
		{
			g_deviceGlobalInfo.mqtt.timerReportInterval = gSysInfo_struct.cubbyInterval; 
		}
		else
		{
			g_deviceGlobalInfo.mqtt.timerReportInterval = 1800; 			//定时间隔半小时
			gSysInfo_struct.cubbyInterval = 1800;
		}
		
		// printf("g_deviceGlobalInfo.mqtt.timerReportInterval:%d\r\n", g_deviceGlobalInfo.mqtt.timerReportInterval);
		// printf("gSysInfo_struct.cubbyInterval:%d\r\n", gSysInfo_struct.cubbyInterval);

        printf("Mqtt addrtest[%s],u16port[%d], keepalive[%d],retryInterval[%d],retryTimes[%d]\r\n\r\n", \
                g_deviceGlobalInfo.mqtt.addr, g_deviceGlobalInfo.mqtt.port, g_deviceGlobalInfo.mqtt.HBInterval,\
                g_deviceGlobalInfo.mqtt.retryInterval, g_deviceGlobalInfo.mqtt.retryTimes);

#if (DEVICE_REBOOT_4G_CONTROL_TYPE == DEVICE_REBOOT_4G_NO_POWER_OFF)
		memset((char*)&gSysLoginInfo.addr, 0, sizeof(gSysLoginInfo.addr));
		memcpy(gSysLoginInfo.addr, u8szMqttAddrTemp, strlen(u8szMqttAddrTemp));
		gSysLoginInfo.port = g_deviceGlobalInfo.mqtt.port;
		gSysLoginInfo.HBInterval = g_deviceGlobalInfo.mqtt.HBInterval;
        gSysLoginInfo.retryInterval = g_deviceGlobalInfo.mqtt.retryInterval;
        gSysLoginInfo.retryTimes = g_deviceGlobalInfo.mqtt.retryTimes;
		gSysLoginInfo.timerReportInterval = g_deviceGlobalInfo.mqtt.timerReportInterval;
#endif
		

        if((1== gstRouteRsp.success) && (1 == gstRouteRsp.data.reactive))
        {
            NET4G_DEBUG("[route_rsp]:code error:reActive\r\n");
            gSysLoginInfo.activeFlag = 0; //TODO reactive

			// memset((u8*)&gSysLoginInfo, 0, sizeof(gSysLoginInfo));
			// tls_fls_write(DEVICE_LOGIN_INFO_FLASH_ADDR, (u8*)&gSysLoginInfo, sizeof(gSysLoginInfo));
			// device_reboot();

            func_ret = AT_ERR;
        }
        else
        {
			func_ret = AT_OK;
        }

		pb_release(routeRsp_fields, &stRoutersp);
	}
	else
	{
		NET4G_DEBUG("[route_rsp]: data error\r\n");
        func_ret = AT_ERR;
	}

	return func_ret;
}


AT_RET_E device_active_process(void)
{
	static uint8_t payload_data[500] = {0};
	uint8_t cmd_data_buff[500] = {0};

	at_cmd_handler_t at_callback_func = NULL;
	
	// if((gSysLoginInfo.activeFlag == 0xBB)&&(strlen(gSysLoginInfo.cloudKey)>0)&&(net4G_process_steps.deviceActive == 0))
	if((gSysLoginInfo.activeFlag == 0xBB)&&(net4G_process_steps.deviceActive == 0))
	{
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
		net4G_process_steps.deviceActive = 6;
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
		net4G_process_steps.deviceActive = 4;
#endif
	}

	if(net4G_process_steps.deviceActive >= sizeof(device_active_steps)/sizeof(device_active_steps[0]))
    {
        NET4G_DEBUG("[active_step]: step error %d\n", net4G_process_steps.deviceActive);
        return PROCESS_ERROR;
    }
	

	at_cmd_t activeCmd = module_4g_cmds[device_active_steps[net4G_process_steps.deviceActive].id]; 
	// NET4G_DEBUG("[active_step] net4G_process_steps.deviceActive = %d\n", net4G_process_steps.deviceActive);
	// NET4G_DEBUG("[active_step]:ID[%d] ,%s\r\n", activeCmd.state, activeCmd.p_cmd); // test by y04485
	switch(device_active_steps[net4G_process_steps.deviceActive].id)
	{
		case CMD_ID_ACTIVE_SET_URL_CMD:
			// NET4G_DEBUG("111[cmd]\r\n"); // test by y04485
			device_active_url_pack(payload_data, sizeof(payload_data));
			
			// snprintf((char *)payload_data, sizeof(payload_data), "%s", NET_CMD_HTTPGET_URL_TEST);
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_HTTPGET_SET_URL_CMD, strlen(payload_data));//指令长度
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ACTIVE_SET_URL_DATA:
			// NET4G_DEBUG("222\r\n"); // test by y04485
			memcpy(cmd_data_buff, payload_data, sizeof(payload_data));//指令内容
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ACTIVE_SET_CONNECT_URL:
			// NET4G_DEBUG("555\r\n"); // test by y04485
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), "%s", NET_CMD_HTTPGET_CONNECT_URL);//建立连接
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ACTIVE_GET_HTTP_RSP:
			// NET4G_DEBUG("666\r\n"); // test by y04485
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), "%s", NET_CMD_HTTPGET_GET_HTTP_RSP);//请求数据
			activeCmd.p_cmd = cmd_data_buff;
			at_callback_func = net4g_active_post_rsp_cb; // commented test by y04485
			break;
			
		case CMD_ID_ACTIVE_CONFIRM_SET_URL_CMD://激活确认
			// NET4G_DEBUG("777\r\n"); // test by y04485
			device_active_confirm_url_pack(payload_data, sizeof(payload_data));
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_HTTPGET_SET_URL_CMD, strlen(payload_data));//指令长度
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ACTIVE_CONFIRM_SET_URL_DATA://激活确认
			// NET4G_DEBUG("888\r\n"); // test by y04485
			memcpy(cmd_data_buff, payload_data, sizeof(payload_data));//指令内容
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ACTIVE_CONFIRM_CONNECT_URL://激活确认 建立连接
			// NET4G_DEBUG("888\r\n"); // test by y04485
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), "%s", NET_CMD_HTTPGET_CONNECT_URL);
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ACTIVE_CONFIRM_GET_HTTP_RSP://激活确认 请求数据
			// NET4G_DEBUG("bbb\r\n"); // test by y04485
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), "%s",NET_CMD_HTTPGET_GET_HTTP_RSP);
			activeCmd.p_cmd = cmd_data_buff;
			at_callback_func = net4g_active_confirm_rsp_cb;//激活确认回调
			break;
			
		default:
			break;
	}

	 
    if(AT_OK == at_cmd_send_check(&activeCmd, at_callback_func))
    {
		net4G_process_steps.deviceActive = device_active_steps[net4G_process_steps.deviceActive].s_jump;
	}
    else
    {
        net4G_process_steps.deviceActive = device_active_steps[net4G_process_steps.deviceActive].f_jump;
    }

	if(device_active_steps[net4G_process_steps.deviceActive].id == CMD_ID_SUCCESS)
	{
		net4G_process_steps.deviceActive = 0;
		return PROCESS_DONE;
	}
	else if(device_active_steps[net4G_process_steps.deviceActive].id == CMD_ID_ERROR)
	{
		net4G_process_steps.deviceActive = 0;
		return PROCESS_ERROR;
	}

	return PROCESS_WAIT;
}



PROCESS_STEP_RET_E  device_route_process(void)
{
	static uint8_t payload_data[150] = {0};
	uint8_t cmd_data_buff[150] = {0};

	at_cmd_handler_t at_callback_func = NULL;

	if(net4G_process_steps.connectRoute >= sizeof(device_route_steps)/sizeof(device_route_steps[0]))
    {
        NET4G_DEBUG("[route_step]: step error %d\n", net4G_process_steps.connectRoute);
        return PROCESS_ERROR;
    }
	

	at_cmd_t activeCmd = module_4g_cmds[device_route_steps[net4G_process_steps.connectRoute].id]; 

	switch(device_route_steps[net4G_process_steps.connectRoute].id)
	{
		case CMD_ID_ROUTE_SET_URL_CMD:

			device_route_url_pack(payload_data, sizeof(payload_data));
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_HTTPGET_SET_URL_CMD, strlen(payload_data));
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ROUTE_SET_URL_DATA:

			memcpy(cmd_data_buff, payload_data, sizeof(payload_data));
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ROUTE_CONNECT_URL:
			
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), "%s", NET_CMD_HTTPGET_CONNECT_URL);
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_ROUTE_GET_HTTP_RSP:

			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), "%s",NET_CMD_HTTPGET_GET_HTTP_RSP);
			activeCmd.p_cmd = cmd_data_buff;
			at_callback_func = device_route_rsp_cb;
			break;
			
		default:
			break;
	}

	 
    if(AT_OK == at_cmd_send_check(&activeCmd, at_callback_func))
    {
		net4G_process_steps.connectRoute = device_route_steps[net4G_process_steps.connectRoute].s_jump;
	}
    else
    {
        net4G_process_steps.connectRoute = device_route_steps[net4G_process_steps.connectRoute].f_jump;
    }

	if(device_route_steps[net4G_process_steps.connectRoute].id == CMD_ID_SUCCESS)
	{
		net4G_process_steps.connectRoute = 0;
		return PROCESS_DONE;
	}
	else if(device_route_steps[net4G_process_steps.connectRoute].id == CMD_ID_ERROR)
	{
		net4G_process_steps.connectRoute = 0;
		net4g_reconnect_flag = 1;
		return PROCESS_ERROR;
	}

	return PROCESS_WAIT;
}


PROCESS_STEP_RET_E module_4g_active_route_process(void)
{
	static NET4G_ACTIVE_ROUTE_STEP active_route_step = ACTIVE_ROUTE_STEP_CHECK;
	PROCESS_STEP_RET_E route_ret = PROCESS_WAIT;
	PROCESS_STEP_RET_E step_ret = PROCESS_WAIT;
	
	switch (active_route_step)
	{
		case ACTIVE_ROUTE_STEP_CHECK: // read flash, check the device registered or not
			active_route_step = check_active_or_route()?ACTIVE_ROUTE_STEP_ROUTE:ACTIVE_ROUTE_STEP_ACTIVE;
			break;
		
		case ACTIVE_ROUTE_STEP_ACTIVE: // active
			step_ret = device_active_process();
			if(step_ret == PROCESS_DONE)
			{
				active_route_step = ACTIVE_ROUTE_STEP_ROUTE;
			}
			else if(step_ret == PROCESS_ERROR)
			{
			
			}
			break;
			
		case ACTIVE_ROUTE_STEP_ROUTE: //route
			step_ret = device_route_process();
			if(step_ret == PROCESS_DONE)
			{
				active_route_step = ACTIVE_ROUTE_STEP_END;
			}
			else if(step_ret == PROCESS_ERROR)
			{
				active_route_step = ACTIVE_ROUTE_STEP_CHECK;
				route_ret = PROCESS_ERROR;
			}
			break;
			
		case ACTIVE_ROUTE_STEP_END:
			
			active_route_step = ACTIVE_ROUTE_STEP_CHECK;
			route_ret =  PROCESS_DONE;
			break;
			
		default:
			break;
	}
	
	return route_ret;
}





/****************************************************************************************************************************
*XD CLOUD SIGN IN
*****************************************************************************************************************************/
extern char *pucProductkey;

PROCESS_STEP_RET_E module_4g_cloud_logIn_process(void)
{
	uint8_t cmd_data_buff[300] = {0};

	if(net4G_process_steps.cloudLogin >= sizeof(device_signIn_steps)/sizeof(device_signIn_steps[0]))
    {
        NET4G_DEBUG("[login]step ERROR %d\n", net4G_process_steps.cloudLogin);
        return PROCESS_ERROR;
    }
	// NET4G_DEBUG("[login]step  %d\n", net4G_process_steps.cloudLogin);
	at_cmd_t activeCmd = module_4g_cmds[device_signIn_steps[net4G_process_steps.cloudLogin].id]; 
    // NET4G_DEBUG("[login_step]:3333ID[%d] ,%s\r\n", activeCmd.state, activeCmd.p_cmd); // test by y04485


	switch(device_signIn_steps[net4G_process_steps.cloudLogin].id)
	{
		case CMD_ID_MQTT_CONFIG_TIMEOUT:
			// NET4G_DEBUG("L111\r\n"); // test by y04485
		  //snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff),  NET_CMD_MQTT_CONFIG_TIMEOUT, g_deviceGlobalInfo.mqtt.retryInterval, g_deviceGlobalInfo.mqtt.retryTimes);//commented test by y04485
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff),  NET_CMD_MQTT_CONFIG_TIMEOUT, 60, 3); //test by y04485
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_MQTT_CONFIG_KEEPALIVE:
			// NET4G_DEBUG("L222\r\n"); // test by y04485
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_CONFIG_KEEPALIVE, g_deviceGlobalInfo.mqtt.HBInterval);
			activeCmd.p_cmd = cmd_data_buff;
			break;

		case CMD_ID_MQTT_CONFIG_RECMODE:
            // NET4G_DEBUG("L222\r\n"); // test by y04485
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_CONFIG_RECMODE);
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_MQTT_OPEN:
			// NET4G_DEBUG("L333\r\n"); // test by y04485
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_OPEN, g_deviceGlobalInfo.mqtt.addr, g_deviceGlobalInfo.mqtt.port);
			//snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_OPEN, "levi-v2.seven.dian-dev.com", 1883);//test by y04485
			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		case CMD_ID_MQTT_CONNECT:
			// NET4G_DEBUG("L444\r\n"); // test by y04485
			
//			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_CONNECT, gSysLoginInfo.cloudId , gSysLoginInfo.cloudId, g_deviceGlobalInfo.mqtt.password);
//			activeCmd.p_cmd = cmd_data_buff;
#if 0
    		device_active_url_pack(payload_data, sizeof(payload_data));
    		snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_HTTPGET_SET_URL_CMD, strlen(payload_data));
    		activeCmd.p_cmd = cmd_data_buff;
            memcpy(cmd_data_buff, payload_data, sizeof(payload_data));
		    activeCmd.p_cmd = cmd_data_buff;
#endif
			APP_NETWORK_MqttConnData();
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_CONNECT, gSysLoginInfo.clientId , gSysLoginInfo.DeviceKey, g_deviceGlobalInfo.mqtt.password);
			activeCmd.p_cmd = cmd_data_buff;
			
			break;
			
		case CMD_ID_MQTT_SUB_TOPIC:
			// NET4G_DEBUG("L555\r\n"); // test by y04485
			//snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_SUB_TOPIC, gSysLoginInfo.clientId, 1); 
			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_SUB_TOPIC, pucProductkey, gSysLoginInfo.DeviceKey,1); 
			activeCmd.p_cmd = cmd_data_buff;
			
//			snprintf((char *)cmd_data_buff, sizeof(cmd_data_buff), NET_CMD_MQTT_SUB_TOPIC, gSysLoginInfo.cloudId, 1);
//			activeCmd.p_cmd = cmd_data_buff;
			break;
			
		default:
			break;
	}


	 
    if(AT_OK == at_cmd_send_check(&activeCmd, NULL))
    {
		net4G_process_steps.cloudLogin = device_signIn_steps[net4G_process_steps.cloudLogin].s_jump;
	}
    else
    {
        net4G_process_steps.cloudLogin = device_signIn_steps[net4G_process_steps.cloudLogin].f_jump;
    }

	if(device_signIn_steps[net4G_process_steps.cloudLogin].id == CMD_ID_SUCCESS)
	{
		net4G_process_steps.cloudLogin = 0;
		return PROCESS_DONE;
	}
	else if(device_signIn_steps[net4G_process_steps.cloudLogin].id == CMD_ID_ERROR)
	{
		net4G_process_steps.cloudLogin = 0;
		return PROCESS_ERROR;
	}

	return PROCESS_WAIT;
}







/****************************************************************************************************************************
*XD CLOUD COMMUNICATION
*****************************************************************************************************************************/


s16 net4g_uart_recv_cb(u16 len)
{
	uint8_t recv_len = 0;
	
	if(gNet4g_dataStruct.recv.fifo.tail+len >= NET_4G_UART_RECV_FIFO_SIZE)
	{
		recv_len = NET_4G_UART_RECV_FIFO_SIZE - gNet4g_dataStruct.recv.fifo.tail;
		network_uart_recv(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.tail, recv_len);
		
		recv_len =  len - recv_len;
		network_uart_recv(gNet4g_dataStruct.recv.fifo.data, recv_len);
		gNet4g_dataStruct.recv.fifo.tail = recv_len;
		
	}
	else
	{
		network_uart_recv(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.tail, len);
		gNet4g_dataStruct.recv.fifo.tail += len;
	}
	gNet4g_dataStruct.recv.fifo.data[gNet4g_dataStruct.recv.fifo.tail] = '\0';

	//printf("r%d", len);
    return WM_SUCCESS;
}


PROCESS_STEP_RET_E module_4g_get_cell_info_process(void)
{
	AT_RET_E ret = AT_ERR;
	
	if(net4G_process_steps.getCellInfoStep >= sizeof(net4g_get_cell_info_config_command_steps)/sizeof(net4g_get_cell_info_config_command_steps[0]))
    {
        NET4G_DEBUG("[4G_getCellInfoStep] step error %d\n", net4G_process_steps.getCellInfoStep);
        return PROCESS_ERROR;
    }
	
	at_cmd_t cmd = module_4g_cmds[net4g_get_cell_info_config_command_steps[net4G_process_steps.getCellInfoStep].id]; 
	switch(net4g_get_cell_info_config_command_steps[net4G_process_steps.getCellInfoStep].id)
	{
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
		case CMD_ID_GET_CELL:
			network_uart_send(cmd.p_cmd, strlen(cmd.p_cmd));
			app_delay_ms(cmd.interval_ms);
			ret = AT_OK;
			break;
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
		case CMD_ID_GET_QENG_SERVINGCELL:
			network_uart_send(cmd.p_cmd, strlen(cmd.p_cmd));
			app_delay_ms(cmd.interval_ms);
			ret = AT_OK;
			break;

		// case CMD_ID_GET_QENG_NEIGHBOURCELL:
		// 	ret = at_cmd_send_check(&cmd, net4g_get_net_qeng_neighbour_cell_cb);
		// 	break;

		// case CMD_ID_GET_QCELL:
		// 	ret = at_cmd_send_check(&cmd, net4g_get_net_qcell_cb);
		// 	break;	
#endif


		case CMD_ID_GET_RSSI:
			network_uart_send(cmd.p_cmd, strlen(cmd.p_cmd));
			app_delay_ms(cmd.interval_ms);
			ret = AT_OK;
			break;
		
		default:
			ret = at_cmd_send_check(&cmd, NULL);
			break;
	}

	if(AT_OK == ret)
    {
		net4G_process_steps.getCellInfoStep = net4g_get_cell_info_config_command_steps[net4G_process_steps.getCellInfoStep].s_jump;
	}
    else
    {
        net4G_process_steps.getCellInfoStep = net4g_get_cell_info_config_command_steps[net4G_process_steps.getCellInfoStep].f_jump;
    }

	if(net4g_get_cell_info_config_command_steps[net4G_process_steps.getCellInfoStep].id == CMD_ID_SUCCESS)
	{
		net4G_process_steps.getCellInfoStep = 0;
		net4g_get_cell_info_flag = 0;
		return PROCESS_DONE;
	}
	else if(net4g_get_cell_info_config_command_steps[net4G_process_steps.getCellInfoStep].id == CMD_ID_ERROR)
	{
		net4G_process_steps.getCellInfoStep = 0;
		net4g_get_cell_info_flag = 0;
		return PROCESS_ERROR;
	}

	return PROCESS_WAIT;
}



uint8_t get_key_data_index(uint8_t* srcData, uint16_t headIndex, uint16_t tailIndex, uint8_t*keyWord, uint16_t* sameKey_index)
{
	uint16_t i = 0, ii=0;
	uint8_t j = 0;
	uint8_t k = 0;
	uint8_t sameKey_count = 0;
	uint8_t keyWordLen = strlen(keyWord);
	
	if(tailIndex > headIndex)
	{
		for(i=headIndex; i < tailIndex; i++)
		{
			if(srcData[i] == keyWord[0])
			{
				for(j=0; j < keyWordLen; j++)
				{
					if(srcData[i + j] == keyWord[j])
					{
						if(sameKey_count == 0)
						{
							*sameKey_index = i;
						}
						sameKey_count++;
					}
					else
					{
						sameKey_count = 0;
						break;
					}
				}
				if(sameKey_count == strlen(keyWord))
				{
					//NET4G_DEBUG("__0__getKey_%d_%d\r\n", i,j);
					return 1;
				}
			}
		}
	}
	else  if(tailIndex < headIndex)
	{
		for(i=headIndex; i < NET_4G_UART_RECV_FIFO_SIZE; i++)
		{
			if(srcData[i] == keyWord[0])
			{
				if(i+keyWordLen > NET_4G_UART_RECV_FIFO_SIZE)
				{
					keyWordLen = NET_4G_UART_RECV_FIFO_SIZE - i;
					//printf("1keyWordLen = %d\r\n", keyWordLen);
				}
				
				for(j=0; j < keyWordLen; j++)
				{
					if(srcData[i + j] == keyWord[j])
					{
						if(sameKey_count == 0)
						{
							*sameKey_index = i;
						}
						sameKey_count++;
						//printf("1get%d_%d_%d__%02x\r\n",sameKey_count, i,j, srcData[i + j]);
					}
					else
					{
						sameKey_count = 0;
						break;
					}
				}
				
				if(sameKey_count == strlen(keyWord))
				{
					//NET4G_DEBUG("__1__getKey_%d_%d\r\n", i,j);
					return 1;
				}
			}
		}
		if(sameKey_count > 0)
		{
			k=0;
			keyWordLen = strlen(keyWord);
			//printf("\r\n2keyWordLen = %d, j=%d\r\n", keyWordLen, j);
			for(ii=0; ii < tailIndex; ii++)
			{
				//printf("2-%02x-%02x\r\n", srcData[ii], keyWord[j]);
				if(srcData[ii] == keyWord[j])
				{
					for(; j < keyWordLen; j++)
					{
						if(srcData[ii + k] == keyWord[j])
						{
							sameKey_count++;
							//printf("2get%d_%d_%d_%d__%02x\r\n",sameKey_count, ii,j,k, srcData[ii + k]);
						}
						else
						{
							//printf("2reset sameKey_count %d\r\n",sameKey_count);
							sameKey_count = 0;
							break;
						}
						//printf("2-i=%d,j=%d, k=%d\r\n", ii, j, k);
						k++;
					}
					if(sameKey_count == strlen(keyWord))
					{
						//*sameKey_index = i;
						//NET4G_DEBUG("__2__getKey_%d_%d\r\n", i,j);
						//printf("2get key ok %d\r\n", *sameKey_index);
						return 1;
					}
				}
			}
		}
		else
		{
			for(i=0; i < tailIndex; i++)
			{
				if(srcData[i] == keyWord[0])
				{
					for(j=0; j < keyWordLen; j++)
					{
						if(srcData[i + j] == keyWord[j])
						{
							if(sameKey_count == 0)
							{
								*sameKey_index = i;
							}
							sameKey_count++;
							//printf("3get%d_%d_%d__%02x\r\n",sameKey_count, i,j, srcData[i + j]);
						}
						else
						{
							//printf("3reset sameKey_count %d\r\n",sameKey_count);
							sameKey_count = 0;
							break;
						}
						//printf("3-i=%d,j=%d\r\n", i, j);
					}
					if(sameKey_count == strlen(keyWord))
					{
						*sameKey_index = i;
						//NET4G_DEBUG("__3__getKey_%d_%d\r\n", i,j);
						//printf("3get key ok %d\r\n", i);
						return 1;
					}
				}
			}
		}
	}
	
	return 0;
}


uint8_t recv_cmd_from_fifo(uint8_t* keyHeader, uint8_t* keyTail, uint8_t* destCmdData)
{
	uint16_t dataLen = 0;
	static uint16_t cmdHead_index = 0;
	static uint16_t cmdTail_index = 0;
	static uint8_t cmd_get_step = 0;
	uint8_t returnValue = 0;
	uint8_t ret = 0;


	switch(cmd_get_step)
	{
		case 0: //get start
			if(gNet4g_dataStruct.recv.fifo.tail != gNet4g_dataStruct.recv.fifo.head)
			{
				dataLen = gNet4g_dataStruct.recv.fifo.tail > gNet4g_dataStruct.recv.fifo.head ? gNet4g_dataStruct.recv.fifo.tail-gNet4g_dataStruct.recv.fifo.head:\
					(NET_4G_UART_RECV_FIFO_SIZE-gNet4g_dataStruct.recv.fifo.head+gNet4g_dataStruct.recv.fifo.tail);
				if(dataLen >= strlen(keyHeader))
				{
					// NET4G_DEBUG("goto get head_%d_%d\r\n", gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail);
					cmdHead_index = 0;
					cmdTail_index = 0;
					cmd_get_step = 1;
				}
				else if(dataLen > 0)
				{
					ret = 2; // get head fail
				}
			}
			break;
			
		case 1: //get head

			returnValue = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, keyHeader, &cmdHead_index);
			if(returnValue==1)
			{
				gNet4g_dataStruct.recv.fifo.head = cmdHead_index;
				cmd_get_step = 2;
				// NET4G_DEBUG("get head ok: %d\r\n",gNet4g_dataStruct.recv.fifo.head);
			}
			else
			{
				cmd_get_step = 0;
				ret = 2; // get head fail
				// printf("hf_%d_%d_%s\r\n",gNet4g_dataStruct.recv.fifo.head,gNet4g_dataStruct.recv.fifo.tail, keyHeader);
				// printf("data_%d=%s\r\n",gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head);
			}
			break;
			
		case 2: //get tail

			returnValue = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, keyTail, &cmdTail_index);
			if(returnValue==1)
			{
				cmdTail_index+= strlen(keyTail);
				cmd_get_step = 3;
				// NET4G_DEBUG("get tail ok: %d\r\n", cmdTail_index);
			}
			else
			{
				cmd_get_step = 0;
				ret = 3; //get tail fail
			}
			break;
			
		case 3: //get cmd

			memset(destCmdData, 0, NET_4G_UART_RECV_CMD_SIZE);
			if(cmdTail_index > cmdHead_index)
			{
				gNet4g_dataStruct.recv.recvCmdLen = cmdTail_index-cmdHead_index;
				memcpy(destCmdData, gNet4g_dataStruct.recv.fifo.data+cmdHead_index, gNet4g_dataStruct.recv.recvCmdLen);
				
				#if 0
				printf("recv_cmd_%d: ", gNet4g_dataStruct.recv.recvCmdLen);
				for(uint16_t i=0;i<gNet4g_dataStruct.recv.recvCmdLen;i++)
				{
					printf("%02x ", destCmdData[i]);
				}
				printf("\r\n");
				#endif
			}
			else
			{
				memcpy(destCmdData, gNet4g_dataStruct.recv.fifo.data+cmdHead_index, NET_4G_UART_RECV_FIFO_SIZE- cmdHead_index);
				memcpy(destCmdData+(NET_4G_UART_RECV_FIFO_SIZE-cmdHead_index), gNet4g_dataStruct.recv.fifo.data, cmdTail_index);
				gNet4g_dataStruct.recv.recvCmdLen = NET_4G_UART_RECV_FIFO_SIZE- cmdHead_index + cmdTail_index;

				#if 0
				printf("recv_cmd_%d: ", gNet4g_dataStruct.recv.recvCmdLen);
				for(uint16_t i=0;i<gNet4g_dataStruct.recv.recvCmdLen;i++)
				{
					printf("%02x ", destCmdData[i]);
				}
				printf("\r\n");
				#endif
			}

			gNet4g_dataStruct.recv.fifo.head = cmdTail_index;
			cmd_get_step = 0;
			ret = 1;
			break;
			
		default:
			cmd_get_step = 0;
			break;
	}
	
	return ret;
}


uint8_t get_net4G_recv_cmd(void)
{
	uint8_t ret  = 0;
	uint16_t cmdHead_index = 0;
	uint8_t  target_data[200] = {0};
	uint16_t recvLen = 0;
	uint8_t rssiValue = 0;

// #if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
// 	if(gNet4g_dataStruct.recv.check_pubAckFlag)//检查pubAck标志
// #elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
	if(gNet4g_dataStruct.recv.check_pubMiddleFlag)//移远中间过程应答检测标志
	{
		ret = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, NET_MQTT_PUB_MIDDLE_HEAD, &cmdHead_index);
		if(ret == 1)//收到应答
		{
			gNet4g_dataStruct.recv.check_pubMiddleFlag = 0;	//清除中间过程应答检查标志
			gNet4g_dataStruct.recv.pubMiddleFlag = 1;		//收到中间过程应答标志置位
		//	gNet4g_dataStruct.recv.fifo.head = cmdHead_index +strlen(NET_MQTT_PUB_MIDDLE_HEAD);
			printf("get_>_ok:%d\r\n", cmdHead_index);
		}
	}
	else if(gNet4g_dataStruct.recv.check_pubAckFlag)//检查pubAck标志
// #endif
	{
		ret = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, NET_MQTT_PUB_RSP_HEAD, &cmdHead_index);
		if(ret == 1)
		{
			gNet4g_dataStruct.recv.check_pubAckFlag = 0;	//清除pub应答检查标志
			gNet4g_dataStruct.recv.pubAckFlag = 1;			//收到pub应答标志置位
		//	gNet4g_dataStruct.recv.fifo.head = cmdHead_index +strlen(NET_MQTT_PUB_RSP_HEAD);
			printf("\r\n");
			NET4G_DEBUG("[PUB ACK]:%d\r\n", cmdHead_index);
		}
	}
	else
	{
		ret = recv_cmd_from_fifo(NET_MQTT_CTRL_CMD_HEAD, NET_MQTT_CTRL_CMD_TAIL, gNet4g_dataStruct.recv.recvCmd); //mqtt ctrl cmd
		if(ret==1)//接收数据成功
		{
			memset(gNet4g_dataStruct.recv.cmdQ[gNet4g_dataStruct.recv.qTail].qData,  0, NET_4G_UART_RECV_CMD_SIZE);
			memcpy(gNet4g_dataStruct.recv.cmdQ[gNet4g_dataStruct.recv.qTail].qData, gNet4g_dataStruct.recv.recvCmd, gNet4g_dataStruct.recv.recvCmdLen);
			gNet4g_dataStruct.recv.cmdQ[gNet4g_dataStruct.recv.qTail].qLen = gNet4g_dataStruct.recv.recvCmdLen;

			#if 0
			printf("recv_cmdQ_%d: ", gNet4g_dataStruct.recv.cmdQ[gNet4g_dataStruct.recv.qTail].qLen);
			for(uint16_t i=0;i<gNet4g_dataStruct.recv.cmdQ[gNet4g_dataStruct.recv.qTail].qLen;i++)
			{
				printf("%02x ", gNet4g_dataStruct.recv.cmdQ[gNet4g_dataStruct.recv.qTail].qData[i]);
			}
			printf("\r\n");
			#endif

			gNet4g_dataStruct.recv.qTail++;
			gNet4g_dataStruct.recv.qTail %= NET_4G_UART_RECV_QUEU_SIZE;

		}
		else if(ret==2)//get head fail
		{
			ret = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, NET_MQTT_DISCONNECT_CMD_HEAD, &cmdHead_index);
			if(ret==1)
			{
				NET4G_DEBUG("[MQTT_DISONNECT]\r\n");//MQTT断开连接
				ret = 10;
			}
			else if((NULL != strstr(gNet4g_dataStruct.recv.fifo.data, "boot.rom")) || (NULL != strstr(gNet4g_dataStruct.recv.fifo.data, "Lierda")))
			{
				memset(gNet4g_dataStruct.recv.fifo.data, 0,NET_4G_UART_RECV_FIFO_SIZE);
				gNet4g_dataStruct.recv.fifo.head = 0;
				gNet4g_dataStruct.recv.fifo.tail = 0;
				NET4G_DEBUG("[4G_REBOOT] 4g module reboot\r\n");//模块重启
				ret = 10;
			}

#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))

			recvLen = 0;
			if(gNet4g_dataStruct.recv.fifo.tail>=gNet4g_dataStruct.recv.fifo.head)
			{
				recvLen = gNet4g_dataStruct.recv.fifo.tail - gNet4g_dataStruct.recv.fifo.head;
			}
			else
			{
				recvLen = (NET_4G_UART_RECV_FIFO_SIZE - gNet4g_dataStruct.recv.fifo.head) + gNet4g_dataStruct.recv.fifo.tail;
			}

			
			if(NULL != strstr(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, "+LCELL:"))
			{
				printf("LCELL_data_%d=%s\r\n",gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head);
				app_delay_ms(50);
				ret = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, GET_NET_CELL_RSP_HEAD, &cmdHead_index);
				if(ret==1)
				{  
					net4g_get_net_qcell_cb(AT_OK, gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, recvLen);
				    // if(get_atcmd_rsp_value(gNet4g_dataStruct.recv.fifo.data, recvLen, GET_NET_CELL_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
					// {
					// 	NET4G_DEBUG("[+QENG]: %s\r\n",target_data);
					// }
				}
			}

           	
			if(NULL != strstr(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, "+CSQ:"))
			{
				printf("CSQdata_%d=%s\r\n",gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head);
				app_delay_ms(50);
				ret = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, GET_NET_RSSI_RSP_HEAD, &cmdHead_index);
				if(ret==1)
				{  
				    if(get_atcmd_rsp_value(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, recvLen, GET_NET_RSSI_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
					{
						rssiValue = atoi(target_data);
					}
					NET4G_DEBUG("[RSSI] rssi = %d\r\n",rssiValue);
					NET4G_DEBUG("[RSSI target_data] rssi = %s\r\n",target_data);
					if(rssiValue!=0)
					{
						g_deviceGlobalInfo.signal = (rssiValue*100)/31;
					}
					NET4G_DEBUG("[g_deviceGlobalInfo.signal ]: %d\r\n", g_deviceGlobalInfo.signal);

					g_deviceGlobalInfo.qeng_servingcell_info.RSSI = rssiValue;//转换拿到的值
					// printf("g_deviceGlobalInfo.qeng_servingcell_info.RSSI = %d\r\n",g_deviceGlobalInfo.qeng_servingcell_info.RSSI);

					InfoGsmDataPubTopic(deviceReportType_intervalReport, 0, 0);					/* GsmData数据上报 */
				}
			}
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
			else if(NULL != strstr(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, "+QIURC:"))
			{
				app_delay_ms(50);
				ret = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, NET_MQTT_DISCONNECT_CMD_HEAD, &cmdHead_index);
				if(ret==1)
				{
					NET4G_DEBUG("[MQTT_DISONNECT] retry\r\n");
					ret = 10;
				}
			}
	
			recvLen = 0;
			if(gNet4g_dataStruct.recv.fifo.tail>=gNet4g_dataStruct.recv.fifo.head)
			{
				recvLen = gNet4g_dataStruct.recv.fifo.tail - gNet4g_dataStruct.recv.fifo.head;
			}
			else
			{
				recvLen = (NET_4G_UART_RECV_FIFO_SIZE - gNet4g_dataStruct.recv.fifo.head) + gNet4g_dataStruct.recv.fifo.tail;
			}

			printf("CSQdata_%d=%s\r\n",gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head);
			if(NULL != strstr(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, "+CSQ:"))
			{
				app_delay_ms(50);
                
				ret = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, GET_NET_RSSI_RSP_HEAD, &cmdHead_index);
				if(ret==1)
				{  
				    if(get_atcmd_rsp_value(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, recvLen, GET_NET_RSSI_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
					{
						rssiValue = atoi(target_data);
					}
					NET4G_DEBUG("[RSSI] rssi = %d\r\n",rssiValue);
					NET4G_DEBUG("[RSSI] rssi = %s\r\n",target_data);
					if(rssiValue!=0)
					{
						g_deviceGlobalInfo.signal = (rssiValue*100)/31;
					}
					NET4G_DEBUG("[GLOBAL rssi ]: %d\r\n", g_deviceGlobalInfo.signal);

					g_deviceGlobalInfo.qeng_servingcell_info.RSSI = rssiValue;//转换拿到的值
				}
			}
			
			printf("QENGdata_%d=%s\r\n",gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head);
			if(NULL != strstr(gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, "+QENG:"))
			{
				app_delay_ms(50);

				ret = get_key_data_index(gNet4g_dataStruct.recv.fifo.data,gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail, GET_NET_QENG_SERVING_CELL_RSP_HEAD, &cmdHead_index);
				if(ret==1)
				{  
					net4g_get_net_qeng_serving_cell_cb(AT_OK, gNet4g_dataStruct.recv.fifo.data+gNet4g_dataStruct.recv.fifo.head, recvLen);
				    // if(get_atcmd_rsp_value(gNet4g_dataStruct.recv.fifo.data, recvLen, GET_NET_QENG_SERVING_CELL_RSP_HEAD, NET_CMD_RSP_TAIL_OK, target_data))
					// {
					// 	NET4G_DEBUG("[+QENG]: %s\r\n",target_data);
					// }
					InfoGsmDataPubTopic(deviceReportType_intervalReport, 0, 0);					/* GsmData数据上报 */
				}
			}
#endif	
			gNet4g_dataStruct.recv.fifo.head = gNet4g_dataStruct.recv.fifo.tail;

		}
		else if(ret==3)
		{
			NET4G_DEBUG("get_cmdTail_fail_%d-%d\r\n", gNet4g_dataStruct.recv.fifo.head, gNet4g_dataStruct.recv.fifo.tail);
		}	
	}
	return ret;
}



uint8_t get_at_rsp_keywords(uint8_t* srcData, uint8_t* srcHeader,uint8_t keyNum, uint8_t* destData)
{
	uint8_t* pKeyWord = NULL;
	uint8_t key_count = 0;
	uint16_t key_len = 0;
	uint16_t i = 0;
	uint8_t ret = 0;
	
	if((srcData == NULL) || (destData == NULL))
	{
		return 0;
	}

	pKeyWord = strtok(srcData+strlen(srcHeader), ",");
	while(pKeyWord != NULL)
	{
		key_count++;
		if(key_count == keyNum)
		{
			if(*(pKeyWord+0) == '"')
			{
				pKeyWord++;
			}
			key_len = strlen(pKeyWord);

			for(i=strlen(pKeyWord)-1; i>=0; i--)
			{
				if(*(pKeyWord+i) == '\r' 
					|| *(pKeyWord+i) == '\n' 
					|| *(pKeyWord+i) == ' ' 
					|| *(pKeyWord+i) == '"'
					||*(pKeyWord+i) == '\0')
				{
					key_len--;
					continue;
				}
				else
				{
					break;
				}
			}
			
			memcpy(destData, pKeyWord, key_len);
			destData[key_len] = '\0';
			ret = 1;
		}
		
		pKeyWord = strtok(NULL, ",");
	}
	return ret;
}


uint8_t get_at_rsp_keywords_pb(uint8_t* srcData, uint8_t* srcHeader,uint8_t keyNum, uint8_t* destData1, uint8_t* Datalen, uint8_t* destData2)
{
	uint8_t* pKeyWord = NULL;
	uint8_t key_count = 0;
	uint16_t key_len = 0;
	uint16_t pbdata_len = 0;
	uint16_t i = 0;
	uint8_t ret = 0;
	uint8_t destData2_offset = 0;
	
	if((srcData == NULL) || (destData1 == NULL))
	{
		return 0;
	}

	pKeyWord = strtok(srcData+strlen(srcHeader), ",");//从srcData+strlen(srcHeader)开始将原始数据用“，”分解为一组字符串

	// NET4G_DEBUG("key_count %d = %s\r\n",key_count,pKeyWord);

	while(pKeyWord != NULL)
	{
		key_count++;
		if(key_count == keyNum)//关键字个数
		{
			if(*(pKeyWord+0) == '"')
			{
				pKeyWord++;
			}
			key_len = strlen(pKeyWord);

			for(i=strlen(pKeyWord)-1; i>=0; i--)
			{
				if(*(pKeyWord+i) == '\r' 
					|| *(pKeyWord+i) == '\n' 
					|| *(pKeyWord+i) == ' ' 
					|| *(pKeyWord+i) == '"'
					||*(pKeyWord+i) == '\0')
				{
					key_len--;
					continue;
				}
				else
				{
					break;
				}
			}
			
			memcpy(destData1, pKeyWord, key_len);
			destData1[key_len] = '\0';
			
			pKeyWord = strtok(NULL, ",");
			destData2_offset = strlen(pKeyWord);
			memcpy(Datalen,pKeyWord,destData2_offset);
			// NET4G_DEBUG("key_count %d Datalen = %s\r\n",key_count,Datalen);

			pbdata_len = atoi(Datalen);

			destData2_offset = destData2_offset + 1;//偏移一个空
			if(*(pKeyWord+destData2_offset) == '"')
			{
				pKeyWord++;
			}
			
			// NET4G_DEBUG("key_count %d pbdata_len = %d\r\n",key_count,pbdata_len);
			// for(i=0; i<pbdata_len; i++)
			// {
			// 	printf("0x%02x ", pKeyWord[i+destData2_offset]);
			// }
			// printf("\r\n");
			
			memcpy(destData2, pKeyWord+destData2_offset, pbdata_len);
			destData2[pbdata_len] = '\0';
			ret = 1;
		}
		pKeyWord = strtok(NULL, ",");
		// NET4G_DEBUG("key_count %d = %s\r\n",key_count,pKeyWord);
	}
	return ret;
}


uint8_t get_clientIdList_keywords_pb(uint8_t* srcData, uint8_t keyNum)
{
	uint8_t* pKeyWord = NULL;
	uint8_t key_count = 0;
	uint8_t ret = 0;
	

	if(srcData == NULL)
	{
		return 0;
	}

	key_count = 0;

	if(keyNum>=1)
	{
		pKeyWord = strtok(srcData, ",");							//从srcData开始将原始数据用“,”分解为一组字符串
		// NET4G_DEBUG("key_count %d = %s\r\n",key_count,pKeyWord);

		gSysInfo_struct.clientIdInfo.clientIdListBuf[key_count] = atoi(pKeyWord);
		// NET4G_DEBUG("destData1[%d] = %d\r\n",key_count, gSysInfo_struct.clientIdInfo.clientIdListBuf[key_count]);

		if(keyNum>=2)
		{
			for(key_count=1; key_count<keyNum; key_count++)
			{
				pKeyWord = strtok(NULL, ",");
				// NET4G_DEBUG("key_count %d = %s\r\n",key_count,pKeyWord);

				gSysInfo_struct.clientIdInfo.clientIdListBuf[key_count] = atoi(pKeyWord);
				// NET4G_DEBUG("destData1[%d] = %d\r\n",key_count, gSysInfo_struct.clientIdInfo.clientIdListBuf[key_count]);
			}
		}
		ret = 1;
	}

	return ret;
}




// void device_app_ver_rsp_upto_server(void) //rsp of 115=50
// {
// 	location_addr_info_upto_server();
// }



void save_pub_msg_to_queue(uint8_t* topic1,uint8_t* pb_data,uint16_t pb_len)
{
	uint16_t i = 0;

	memset(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData, 0, NET_4G_UART_SEND_QUEU_DATA_SIZE);
//    strcat(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData, "3.0");
//    strcat(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData, sign);
    // strcat(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData, pb_data);
	for(i=0; i<pb_len; i++)
	{
		gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData[i] = pb_data[i];
	}

	gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdLen = pb_len;
	strcpy(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].topic,topic1);
	//  printf("sendDATA %s\r\n",gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData);
	gNet4g_dataStruct.send.qTail++;
	//  printf("gNet4g_dataStruct.send.qTail = %d\r\n",gNet4g_dataStruct.send.qTail);
	gNet4g_dataStruct.send.qTail %= NET_4G_UART_SEND_QUEU_SIZE;
}

//void save_pub_msg_to_queue(uint8_t* sign, uint8_t* data)
//{
//	memset(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData, 0, NET_4G_UART_SEND_QUEU_DATA_SIZE);
//    strcat(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData, "3.0");
//    strcat(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData, sign);
//    strcat(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData, data);
//	gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdLen = strlen(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData);
//
//	//NET4G_DEBUG("____encodeData_%d-%d = %s\r\n\r\n",gNet4g_dataStruct.send.qTail,gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdLen, gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qTail].cmdData);
//	//printf("[RSP]_insert_pub_queue %d\r\n",gNet4g_dataStruct.send.qTail);
//	gNet4g_dataStruct.send.qTail++;
//	gNet4g_dataStruct.send.qTail %= NET_4G_UART_SEND_QUEU_SIZE;
//}

uint8_t reset_pub_msg_queue(void)
{
	gNet4g_dataStruct.send.qHead++;
	gNet4g_dataStruct.send.qHead %= NET_4G_UART_SEND_QUEU_SIZE;
/*	
	if(gNet4g_dataStruct.send.qHead != gNet4g_dataStruct.send.qTail)
	{
		app_delay_ms(50);
	}
*/	
}


uint8_t get_pub_msg_flag(void)
{
	if(gNet4g_dataStruct.send.qTail != gNet4g_dataStruct.send.qHead)
	{
		return 1;
	}
	return 0;
}

uint8_t net4g_send_data_process(void)
{
	static uint8_t process_work_step = 0;
	static uint16_t wait_time_count = 0;
	static uint8_t data_send_fail_times = 0;
	static uint8_t data_send_wait_rsp1_fail_times = 0;

	uint8_t wait_time_num = 100;

	uint8_t recvLen = 0;
	uint8_t ret = 0;
	uint16_t topic_send_len = 0;
	uint16_t i = 0;
	
	switch(process_work_step)
	{
		case 0: //send pub cmd
			memset(gNet4g_dataStruct.send.sendCmd, 0, NET_4G_UART_SEND_CMD_SIZE);
			topic_send_len = 0;

#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
			// snprintf(gNet4g_dataStruct.send.sendCmd, NET_4G_UART_SEND_CMD_SIZE,NET_CMD_MQTT_PUB_MSG, pucProductkey, &gSysLoginInfo.DeviceKey[0],\
			// 	gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].topic, gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdLen);
			// topic_send_len = strlen(gNet4g_dataStruct.send.sendCmd);
			// memcpy(gNet4g_dataStruct.send.sendCmd+topic_send_len, gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdData, gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdLen);
				

			// network_uart_send(gNet4g_dataStruct.send.sendCmd, topic_send_len+gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdLen);
			// NET4G_DEBUG("[PUB][%d] %s\r\n", strlen(gNet4g_dataStruct.send.sendCmd), gNet4g_dataStruct.send.sendCmd);

			// gNet4g_dataStruct.recv.check_pubAckFlag = 1;
			// gNet4g_dataStruct.recv.pubAckFlag = 0;
			// process_work_step = 3;


			snprintf(gNet4g_dataStruct.send.sendCmd, NET_4G_UART_SEND_CMD_SIZE,NET_CMD_MQTT_PUB_MSG, pucProductkey, &gSysLoginInfo.DeviceKey[0],\
				gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].topic, gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdLen);

			network_uart_send(gNet4g_dataStruct.send.sendCmd, strlen(gNet4g_dataStruct.send.sendCmd));
			// NET4G_DEBUG("[PUB][%d] %s\r\n", strlen(gNet4g_dataStruct.send.sendCmd), gNet4g_dataStruct.send.sendCmd);

			gNet4g_dataStruct.recv.check_pubMiddleFlag = 1;
			gNet4g_dataStruct.recv.pubMiddleFlag = 0;//清除接收到中间应答消息标志，等待接收
			process_work_step = 1;

#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
			snprintf(gNet4g_dataStruct.send.sendCmd, NET_4G_UART_SEND_CMD_SIZE,NET_CMD_MQTT_PUB_MSG, pucProductkey, &gSysLoginInfo.DeviceKey[0],\
				gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].topic, gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdLen);

			network_uart_send(gNet4g_dataStruct.send.sendCmd, strlen(gNet4g_dataStruct.send.sendCmd));
			// NET4G_DEBUG("[PUB][%d] %s\r\n", strlen(gNet4g_dataStruct.send.sendCmd), gNet4g_dataStruct.send.sendCmd);

			gNet4g_dataStruct.recv.check_pubMiddleFlag = 1;
			gNet4g_dataStruct.recv.pubMiddleFlag = 0;//清除接收到中间应答消息标志，等待接收
			process_work_step = 1;
#endif
			wait_time_count = 0;//清除超时等待计时
			break;
			
		case 1: //wait recv '>' 

			if(gNet4g_dataStruct.recv.pubMiddleFlag == 1)//接收成功
			{
				gNet4g_dataStruct.recv.check_pubMiddleFlag = 0;//成功，标志清零
				gNet4g_dataStruct.recv.pubMiddleFlag = 0;//成功，标志清零
				process_work_step = 2;					
			}
			else
			{
				wait_time_count++;
				if(net4G_process_steps.netWork == NETWORK_WORK_STEP__INIT)
				{
					wait_time_num = 10;
				}
				else
				{
					wait_time_num = 100;
				}

				if(wait_time_count < wait_time_num)//100
				{
					NET4G_DEBUG("wait_>_timeout: %d\r\n",wait_time_count);
					app_delay_ms(50);
				}
				else
				{
					gNet4g_dataStruct.recv.check_pubMiddleFlag = 0;//成功，标志清零
					gNet4g_dataStruct.recv.pubMiddleFlag = 0;//成功，标志清零
					process_work_step = 0;

					data_send_wait_rsp1_fail_times++;
					if(net4G_process_steps.netWork == NETWORK_WORK_STEP__INIT)
					{
						if(data_send_wait_rsp1_fail_times>=1)
						{
							data_send_wait_rsp1_fail_times = 0;
							ret = 2; //send error 4 times, reconect network
							NET4G_DEBUG("ret = 2;\r\n");
						}
					}
					else
					{
						if(data_send_wait_rsp1_fail_times>=4)
						{
							data_send_wait_rsp1_fail_times = 0;
							ret = 2; //send error 4 times, reconect network
							NET4G_DEBUG("ret = 2;\r\n");
						}
						else
						{
							ret = 1;
							NET4G_DEBUG("ret = 1;\r\n");
						}
					}
					
				}
			}
			break;

		case 2: //pub data
			NET4G_DEBUG("[PUB]msgId:%d\r\n", gNet4g_dataStruct.send.qHead);
			// NET4G_DEBUG("[PUB]cmdLen:%d\r\n", gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdLen);
			// NET4G_DEBUG("[PUB]cmdData:\r\n");
			// for(i=0; i<gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdLen; i++)
			// {
			// 	printf("%02x ", gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdData[i]);
			// }
			// NET4G_DEBUG("\r\n");

			network_uart_send(gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdData, gNet4g_dataStruct.send.queue[gNet4g_dataStruct.send.qHead].cmdLen);
			
			
			gNet4g_dataStruct.recv.check_pubAckFlag = 1;
			gNet4g_dataStruct.recv.pubAckFlag = 0;
			
			net4g_process_report_monitor_times = 0;//定时上报监测计时清零，成功上报了，则不进行主板复位
			
			wait_time_count = 0;
			process_work_step = 3;
			break;
			
		case 3: //wait rsp ok

			wait_time_count++;
			if(wait_time_count%100==0)
			{
				if(gNet4g_dataStruct.recv.pubAckFlag == 1)
				{
					process_work_step = 0;
					ret = 1;
					gNet4g_dataStruct.recv.check_pubAckFlag = 0;
					gNet4g_dataStruct.recv.pubAckFlag = 0;
					data_send_fail_times = 0;
					net4g_process_report_monitor_times = 0;//定时上报监测计时清零，成功上报了，则不进行主板复位
				}
				else
				{
					if(wait_time_count < 1500)//15000
					{
						app_delay_ms(50);
						NET4G_DEBUG("wait_pub_rsp_timeout: %d\r\n",wait_time_count);
					}
					else
					{
						NET4G_DEBUG("wait 4g rsp timeout\r\n");
						wait_time_count = 0;
						gNet4g_dataStruct.recv.check_pubAckFlag = 0;
						gNet4g_dataStruct.recv.pubAckFlag = 0;
						process_work_step = 0;

						data_send_fail_times++;
						if(data_send_fail_times>=4)
						{
							data_send_fail_times = 0;
							ret = 1; //send error 4 times, reconect network
						}
						else
						{
							ret = 1;
						}
					}
				}
			}
			break;
	}
	return ret;
}

void xd_str_strip_inregular_char(char *buf)
{
    if(!buf)
        return;
    if(strlen(buf) >= 2048)
        return;

    char *index = buf;
    while(*index != '\0')
    {
        if(*index == '{' || *index == '}' || *index == ' ' || *index == '[' || *index == ']'
           || *index == '.' || *index == '-' || *index == '_' || *index == '/'
           || *index == '\"' || *index == '\'' || *index == ',' || *index == '\r' || *index == '\n'
           || *index == ':' || (*index >= '0' &&  *index <= '9')
           || (*index >= 'a' &&  *index <= 'z') || (*index >= 'A' &&  *index <= 'Z'))
        {
            index++;
        }
        else
        {
            *index = '\0';
            return;
        }
    }

}



uint8_t RspAnalyze(uint8_t* topic, uint8_t* pbdata,uint16_t datalen)
{
	uint8_t* pKeyWord = NULL;
	//uint8_t  test1[100] = {0};
	uint8_t buffer[200] = {0};
	uint8_t key_count = 0;
	uint16_t key_len = 0;
	uint16_t i = 0;
	uint8_t ret = 0;
	uint8_t k = 0;
	uint8_t DeviceSlotId = 0;

	PB_BYTES_ARRAY_T(100) pbCmdByte;
	PB_BYTES_ARRAY_T(100) nfcpwd;

	char *device = "device";
	char *operation1_command = "command";

	char *operation2_slot = "slot";
	char *operation2_cubby  = "cubby";
	char *operation2_config = "config";
	char *operation2_device = "device";
	char *operation2_ota = "ota";
	char *operation2_lowpower = "lowpower";

	char *operation3_ota_mcu = "mcu";
	char *operation3_ota_cubby = "cubby";
	char *operation3_ota_btSpeaker = "btSpeaker";
	char *operation3_ota_gsm = "gsm";



	char *characteristic25 = "QUERY";
	char *characteristic27 = "CLEAR";

	printf("recv: %s >>>\r\n", topic);
	// printf("datalen: %d \r\n", datalen);
	
	printf("datalen=%d\r\n",datalen);
	for(uint8_t k=0;k<datalen;k++)
	{
		printf("%02x ",pbdata[k]);
	}
	printf("\r\n");


	if((topic == NULL) || (pbdata == NULL) || (datalen >= 200))
	{
		return 0;
	}

	ret = 0;

	pKeyWord = strtok(topic, "/");
	if (strcmp(pKeyWord,device) == 0) 
	{
		pKeyWord = strtok(NULL, "/");
		if (strcmp(pKeyWord,pucProductkey) == 0) 
		{
			pKeyWord = strtok(NULL, "/");
			if (strcmp(pKeyWord,&gSysLoginInfo.DeviceKey[0]) == 0) 
			{
				pKeyWord = strtok(NULL, "/");
				if (strcmp(pKeyWord,operation1_command) == 0) 
				{
					pKeyWord = strtok(NULL, "/");
					if (strcmp(pKeyWord,operation2_slot) == 0) 
					{
						ret = 0;
						printf("command/slot\r\n");

printf("%s %d \n\r\n",__FUNCTION__,__LINE__);//duanyy
						// printf(" ---> GetHeap:%d\n",tls_mem_get_avail_heapsize());
						// buf1 = (uint8_t *)malloc(0x2000);
						// if(buf1 != NULL)
						// 	buf1 = "0123456";
						// printf("%s",buf1);
						// printf(" ---> GetHeap:%d\n",tls_mem_get_avail_heapsize());
						// free(buf1);
						// printf("%s",buf1);
						// printf(" ---> GetHeap:%d\n",tls_mem_get_avail_heapsize());
						// g_payload1[0] = 1;


						memset(buffer, 0, 200);
printf("%s %d \n\r\n",__FUNCTION__,__LINE__);//duanyy
						memcpy(buffer,pbdata,datalen);
printf("%s %d \n\r\n",__FUNCTION__,__LINE__);//duanyy
						memset(pbCmdByte.bytes, 0, 100);
						memset(nfcpwd.bytes, 0, 100);
printf("%s %d \n\r\n",__FUNCTION__,__LINE__);//duanyy
						SlotCommand SlotCommandMsg = {0};

						SlotCommandMsg.pbCmd.arg  = &pbCmdByte;
						SlotCommandMsg.pbCmd.funcs.decode = &bytes_decode_callback;
printf("%s %d \n\r\n",__FUNCTION__,__LINE__);//duanyy
						SlotCommandMsg.nfcpwd.arg  = &nfcpwd;
						SlotCommandMsg.nfcpwd.funcs.decode = &bytes_decode_callback;
printf("%s %d \n\r\n",__FUNCTION__,__LINE__);//duanyy

						printf("%s %d \n\r\n",__FUNCTION__,__LINE__); //duanyy
						pb_istream_t istream = pb_istream_from_buffer(buffer, datalen);
						printf("%s %d \n\r\n",__FUNCTION__,__LINE__); //duanyy

						if (pb_decode(&istream, SlotCommand_fields, &SlotCommandMsg) == false) 
						{
							printf("Decoding failed:\n");
							return 0;
						}
						printf("command/slot Decoding success!!!:\n");
						printf("SlotCommandMsg.has_header:%d\n", SlotCommandMsg.has_header);
						printf("SlotCommandMsg.header.opCode:%d\n", SlotCommandMsg.header.opCode);
						printf("SlotCommandMsg.header.seconds:%d\n", SlotCommandMsg.header.seconds);
						printf("SlotCommandMsg.header.traceId:%d\n", SlotCommandMsg.header.traceId);
						printf("SlotCommandMsg.header.protocol:%d\n", SlotCommandMsg.header.protocol);

						printf("SlotCommandMsg.command:%d\n", SlotCommandMsg.command);
						printf("SlotCommandMsg.addr:%d\n", SlotCommandMsg.addr);
						printf("SlotCommandMsg.slotId:%d\n", SlotCommandMsg.slotId);
						printf("SlotCommandMsg.pbCmd.size:%d\n", pbCmdByte.size);

						ret = 0;

						memset(pbCmdByteUartSendBuf[((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1)], 0, 21);
						memset(nfcpwdUartSendBuf[((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1)], 0, 21);

						// printf("%s %d \n\r\n",__FUNCTION__,__LINE__);//duanyy
						if(SlotCommandMsg.header.opCode > g_deviceGlobalInfo.CloudopCode) //检查云端指令编号 
						{
							g_deviceGlobalInfo.CloudopCode = SlotCommandMsg.header.opCode;
						}
						else
						{
							if((SlotCommandMsg.header.opCode < g_deviceGlobalInfo.CloudopCode)&&(g_deviceGlobalInfo.CloudopCode == 0xFFFFFFFF))
							{
								g_deviceGlobalInfo.CloudopCode = SlotCommandMsg.header.opCode;
							}
							else
							{
								printf("SlotCommandMsg.header.opCode <= g_deviceGlobalInfo.CloudopCode\n");
								return 0;
							}
						}

						if((SlotCommandMsg.header.traceId>=0)&&(SlotCommandMsg.header.traceId<=0xFFFF))
						{
							g_deviceGlobalInfo.CloudTraceId = SlotCommandMsg.header.traceId;
						}

						if(pbCmdByte.size<21)
						{
							pbCmdByteUartSendBuf[((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1)][0] = pbCmdByte.size;
							for(i=0; i<pbCmdByte.size; i++)
							{
								pbCmdByteUartSendBuf[((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1)][1+i] = pbCmdByte.bytes[i];
								printf("SlotCommandMsg.pbCmd.bytes[%d]:0x%02x\n", i, pbCmdByte.bytes[i]);
							}
						}

						printf("SlotCommandMsg.timeout:%d\n", SlotCommandMsg.timeout);

						printf("SlotCommandMsg.nfcpwd.size:%d\n", nfcpwd.size);
						if(nfcpwd.size<21)
						{
							nfcpwdUartSendBuf[((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1)][0] = nfcpwd.size;
							for(i=0; i<nfcpwd.size; i++)
							{
								nfcpwdUartSendBuf[((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1)][1+i] = nfcpwd.bytes[i];
								printf("SlotCommandMsg.nfcpwd.bytes[%d]:0x%02x\n", i, nfcpwd.bytes[i]);
							}
						}

						if(SlotCommandMsg.command==SLOT_CMD_SINGLE_SLOT_INFO_CHECK_RSP)		//单仓上报
						{
							Wt_NodeEvent_Ff((((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1) << 8) | NODE_SINGLE_SLOT_UP_EVT);	//单仓上报
						}
						else if(SlotCommandMsg.command==SLOT_CMD_PRE_OPEN_SLOT_LEND_CHECK_RSP)	//预开仓指令
						{
							Wt_NodeEvent_Ff((((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1) << 8) | NODE_LEND_CHECK_EVT);	//租借检查
						}
						else if(SlotCommandMsg.command==SLOT_CMD_OPEN_SLOT_RSP)	//开仓指令
						{
							Wt_NodeEvent_Ff((((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1) << 8) | NODE_LEND_OPEN_EVT);		//开仓
						}
						else if(SlotCommandMsg.command==SLOT_CMD_CANCEL_OPEN_SLOT_RSP)	//取消开仓
						{
							DeviceSlotId = ((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1);

							g_deviceGlobalInfo.lengCheckTime[DeviceSlotId] = 0;
							g_deviceGlobalInfo.batLendingFlag &= ~((0x01<<DeviceSlotId)&0xFFFFFFFF);		//清除租借检查标志
							printf("g_deviceGlobalInfo.lengCheckTime[%d]:%d\r\n", DeviceSlotId, g_deviceGlobalInfo.lengCheckTime[DeviceSlotId]);
							printf("g_deviceGlobalInfo.batLendingFlag:%08x\r\n", (g_deviceGlobalInfo.batLendingFlag>>DeviceSlotId)&0x01);

						}
						else if(SlotCommandMsg.command==SLOT_CMD_FORCE_OPEN_SLOT_RSP)	//强制开仓指令
						{
							Wt_NodeEvent_Ff((((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1) << 8) | NODE_FORCE_OPEN_EVT);	//强制开仓
						}
						else if(SlotCommandMsg.command==SLOT_CMD_READ_NFC_ENCIPHERED_DATA_RSP)	//读取单仓NFC加密数据
						{
							Wt_NodeEvent_Ff((((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1) << 8) | NODE_RAD_NFC_ENCIPHERED_DATA_EVT);	//读取单仓NFC加密数据
						}
						else if(SlotCommandMsg.command==SLOT_CMD_READ_NFC_PLAINTEXT_DATA_RSP)	//读取单仓NFC明文数据
						{
							Wt_NodeEvent_Ff((((SlotCommandMsg.addr-1)*6 + SlotCommandMsg.slotId-1) << 8) | NODE_RAD_NFC_PLAINTEXT_DATA_EVT);	//读取单仓NFC明文数据
						}
						else if(SlotCommandMsg.command==SLOT_CMD_READ_NFC_SUCCESS_PLAY_AUDIO)	//读取单仓NFC加密数据成功播报语音
						{
							make_play_tone_cmd_data(0x40, 0x04); //归还成功
						}
						else if(SlotCommandMsg.command==SLOT_CMD_READ_NFC_FAIL_PLAY_AUDIO)	//读取单仓NFC加密数据失败播报语音
						{
							make_play_tone_cmd_data(0x40, 0x05); //归还失败
							make_play_tone_cmd_data(0x40, 0x07); //请扫码归还
						}
						
						return ret;
					}
					else if (strcmp(pKeyWord,operation2_cubby) == 0) 
					{
						ret = 0;
						printf("command/cubby\r\n");

						memset(buffer, 0, 200);

						memcpy(buffer,pbdata,datalen);

						CubbyCommand CubbyCommandMsg = {0};

						pb_istream_t istream = pb_istream_from_buffer(buffer, datalen);
						if (pb_decode(&istream, CubbyCommand_fields, &CubbyCommandMsg) == false) 
						{
							printf("Decoding failed:\n");
							return 0;
						}
						printf("command/cubby Decoding success!!!:\n");
						printf("CubbyCommandMsg.has_header:%d\n", CubbyCommandMsg.has_header);
						printf("CubbyCommandMsg.header.opCode:%d\n", CubbyCommandMsg.header.opCode);
						printf("CubbyCommandMsg.header.seconds:%d\n", CubbyCommandMsg.header.seconds);
						printf("CubbyCommandMsg.header.traceId:%d\n", CubbyCommandMsg.header.traceId);
						printf("CubbyCommandMsg.header.protocol:%d\n", CubbyCommandMsg.header.protocol);
						printf("CubbyCommandMsg.command:%d\n", CubbyCommandMsg.command);
						printf("CubbyCommandMsg.addr:%d\n", CubbyCommandMsg.addr);

						if(CubbyCommandMsg.header.opCode > g_deviceGlobalInfo.CloudopCode) //检查云端指令编号 
						{
							g_deviceGlobalInfo.CloudopCode = CubbyCommandMsg.header.opCode;
						}
						else
						{
							if((CubbyCommandMsg.header.opCode < g_deviceGlobalInfo.CloudopCode)&&(g_deviceGlobalInfo.CloudopCode == 0xFFFFFFFF))
							{
								g_deviceGlobalInfo.CloudopCode = CubbyCommandMsg.header.opCode;
							}
							else
							{
								printf("CubbyCommandMsg.header.opCode <= g_deviceGlobalInfo.CloudopCode\n");
								return 0;
							}
						}

						if((CubbyCommandMsg.header.traceId>=0)&&(CubbyCommandMsg.header.traceId<=0xFFFF))
						{
							g_deviceGlobalInfo.CloudTraceId = CubbyCommandMsg.header.traceId;
						}

						if(CubbyCommandMsg.command==SLOT_CMD_SINGLE_SLAVE_INFO_CHECK_RSP)	//单列上报
						{
							Wt_NodeEvent_Ff(((CubbyCommandMsg.addr - 1) << 8) | NODE_SINGLE_CUBBY_UP_EVT);	//单列上报
						}
						else if(CubbyCommandMsg.command==SLOT_CMD_ALL_SLOT_INFO_CHECK_RSP)	//全仓上报
						{
							Wt_NodeEvent_Ff((0xFF << 8) | NODE_ALL_SLOT_UP_EVT);	//全仓上报
						}
						return ret;
					}
					else if (strcmp(pKeyWord,operation2_config) == 0) 
					{
						ret = 0;
						printf("command/config\r\n");

						memset(buffer, 0, 200);

						memcpy(buffer,pbdata,datalen);

						ConfigCommand ConfigCommandMsg = {0};

						pb_istream_t istream = pb_istream_from_buffer(buffer, datalen);
						if (pb_decode(&istream, ConfigCommand_fields, &ConfigCommandMsg) == false) 
						{
							printf("Decoding failed:\n");
							return 0;
						}
						printf("command/Config Decoding success!!!:\n");
						NETWORK_DEBUG("ConfigCommandMsg.has_header:%d\n", ConfigCommandMsg.has_header);
						NETWORK_DEBUG("ConfigCommandMsg.header.opCode:%d\n", ConfigCommandMsg.header.opCode);
						NETWORK_DEBUG("ConfigCommandMsg.header.seconds:%d\n", ConfigCommandMsg.header.seconds);
						NETWORK_DEBUG("ConfigCommandMsg.header.traceId:%d\n", ConfigCommandMsg.header.traceId);
						NETWORK_DEBUG("ConfigCommandMsg.header.protocol:%d\n", ConfigCommandMsg.header.protocol);

						NETWORK_DEBUG("ConfigCommandMsg.command:%d\n", ConfigCommandMsg.command);

						if(ConfigCommandMsg.header.opCode > g_deviceGlobalInfo.CloudopCode) //检查云端指令编号 
						{
							g_deviceGlobalInfo.CloudopCode = ConfigCommandMsg.header.opCode;
						}
						else
						{
							printf("ConfigCommandMsg.header.opCode:%d\n",ConfigCommandMsg.header.opCode);
							printf("g_deviceGlobalInfo.CloudopCode:%d\n",g_deviceGlobalInfo.CloudopCode);
							if((ConfigCommandMsg.header.opCode < g_deviceGlobalInfo.CloudopCode)&&(g_deviceGlobalInfo.CloudopCode == 0xFFFFFFFF))
							{
								g_deviceGlobalInfo.CloudopCode = ConfigCommandMsg.header.opCode;
							}
							else
							{
								printf("ConfigCommandMsg.header.opCode <= g_deviceGlobalInfo.CloudopCode\n");
								return 0;
							}
						}

						if((ConfigCommandMsg.header.traceId>=0)&&(ConfigCommandMsg.header.traceId<=0xFFFF))
						{
						}
						else
						{
							ConfigCommandMsg.header.traceId = 0;
						}


						memset(ConfigCommandFlag, 0, sizeof(ConfigCommandFlag));

						if(ConfigCommandMsg.command == 24)/* 全量上报 24 */
						{
							ret = 0;
							ConfigCommandFlag[0] = ConfigCommandMsg.command;//记录当前命令
							InfoDeviceConfigDataPubTopic(deviceReportType_operationReport, ConfigCommandMsg.header.traceId, ConfigCommandMsg.command);
							return ret;
						}
						else if(ConfigCommandMsg.command == 25)/* 查询 25 特定值 */
						{	
							ret = 0;
							ConfigCommandFlag[0] = ConfigCommandMsg.command;//记录当前命令
							if(ConfigCommandMsg.has_deviceConfig == 1)//有设备参数需要查询
							{
								NETWORK_DEBUG("ConfigCommandMsg.has_deviceConfig:%d\n", ConfigCommandMsg.has_deviceConfig);
								ConfigCommandFlag[1] = 1;

								if(ConfigCommandMsg.deviceConfig.bleEnable == 9)//查询BLE使能状态
								{
									ConfigCommandFlag[2] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleEnable:%d\n", ConfigCommandMsg.deviceConfig.bleEnable);
								}

								if(ConfigCommandMsg.deviceConfig.wifiEnable == 9)//查询wifi使能状态
								{
									ConfigCommandFlag[3] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.wifiEnable:%d\n", ConfigCommandMsg.deviceConfig.wifiEnable);
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.ssid, characteristic25) >= 0)//查询wifi名称
								{
									ConfigCommandFlag[4] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.ssid:%s\n", ConfigCommandMsg.deviceConfig.ssid);
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.password, characteristic25) >= 0)//查询wifi密码
								{
									ConfigCommandFlag[5] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.password:%s\n", ConfigCommandMsg.deviceConfig.password);
								}

								if(ConfigCommandMsg.deviceConfig.remember == 9)//查询WiFi名称和WiFi密码保存标志
								{
									ConfigCommandFlag[6] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.remember:%d\n", ConfigCommandMsg.deviceConfig.remember);
								}

								if(ConfigCommandMsg.deviceConfig.gsmActiveSubId == 9)//查询当前优先激活SIM卡槽位号
								{
									ConfigCommandFlag[7] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.gsmActiveSubId:%d\n", ConfigCommandMsg.deviceConfig.gsmActiveSubId);
								}

								if(ConfigCommandMsg.deviceConfig.cubbyInterval == 9)//查询列信息上报定时间隔
								{
									ConfigCommandFlag[8] = 1;									
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyInterval:%d\n", ConfigCommandMsg.deviceConfig.cubbyInterval);
								}

								if(ConfigCommandMsg.deviceConfig.cubbyHeaderInterval == 9)//查询列表信息上报间隔
								{
									ConfigCommandFlag[9] = 1;									
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyHeaderInterval:%d\n", ConfigCommandMsg.deviceConfig.cubbyHeaderInterval);
								}

								if(ConfigCommandMsg.deviceConfig.bleSpeakerVolume == 9)//查询蓝牙音箱音量
								{
									ConfigCommandFlag[10] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerVolume:%d\n", ConfigCommandMsg.deviceConfig.bleSpeakerVolume);
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.bleSpeakerPairCode, characteristic25) >= 0)//查询蓝牙音箱配对码
								{
									ConfigCommandFlag[11] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerPairCode:%s\n", ConfigCommandMsg.deviceConfig.bleSpeakerPairCode);
								}

								if(ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable == 9)//查询蓝牙音箱广播开启状态
								{
									ConfigCommandFlag[12] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable:%d\n", ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable);
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.clientIdList, characteristic25) >= 0)//查询运营商列表
								{
									ConfigCommandFlag[13] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.clientIdList:%s\n", ConfigCommandMsg.deviceConfig.clientIdList);
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.bleSpeakerName, characteristic25) >= 0)//查询蓝牙音箱名称
								{
									ConfigCommandFlag[14] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerName:%s\n", ConfigCommandMsg.deviceConfig.bleSpeakerName);
								}

								if((ConfigCommandMsg.deviceConfig.cubbyDisable_count>0)&&(ConfigCommandMsg.deviceConfig.cubbyDisable_count<=4))//查询仓位禁用信息
								{
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable_count:%d\n", ConfigCommandMsg.deviceConfig.cubbyDisable_count);

									for(i=0;i<ConfigCommandMsg.deviceConfig.cubbyDisable_count;i++)
									{
										NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable[%d].addr:%d\n", i, ConfigCommandMsg.deviceConfig.cubbyDisable[i].addr);
										NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable[%d].slotDisable:%s\n", i, ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable);
									}

									if(ConfigCommandMsg.deviceConfig.cubbyDisable[0].addr>=1)
									{
										if(strcmp(ConfigCommandMsg.deviceConfig.cubbyDisable[0].slotDisable, characteristic25) >= 0)
										{
											ConfigCommandFlag[15] = 1;
										}
									}
								}

								InfoDeviceConfigDataPubTopic(deviceReportType_operationReport, ConfigCommandMsg.header.traceId, ConfigCommandMsg.command);
							}
							return ret;
						}
						else if(ConfigCommandMsg.command == 26)/* 设置 26 特定值 */
						{
							ret = 0;
							ConfigCommandFlag[0] = ConfigCommandMsg.command;//记录当前命令
							if(ConfigCommandMsg.has_deviceConfig == 1)//有设备参数需要配置
							{
								NETWORK_DEBUG("ConfigCommandMsg.has_deviceConfig:%d\n", ConfigCommandMsg.has_deviceConfig);
								ConfigCommandFlag[1] = 1;

								if(ConfigCommandMsg.deviceConfig.bleEnable == 1)//使能BLE
								{
									ConfigCommandFlag[2] = 1;

									gSysInfo_struct.btInfo.bleEnableFlag = 1;
									app_ble_enable();
									
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleEnable:%d\n", ConfigCommandMsg.deviceConfig.bleEnable);
								}
								else if(ConfigCommandMsg.deviceConfig.bleEnable == 2)//禁用BLE
								{
									ConfigCommandFlag[2] = 1;

									gSysInfo_struct.btInfo.bleEnableFlag = 0;
									app_ble_disable();

									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleEnable:%d\n", ConfigCommandMsg.deviceConfig.bleEnable);
								}

								if(ConfigCommandMsg.deviceConfig.wifiEnable == 1)//使能wifi
								{
									ConfigCommandFlag[3] = 1;

									gSysInfo_struct.wifi.wifiEnableFlag = 1;
									if(netWorkStatus.wifi_state != 2)			//如果设置使能wifi，不清除wifi信息，此时wifi处于连接状态时，状态为2连接不变
									{
										netWorkStatus.wifi_state = 1;
									}
									ret = 1;//返回 使能了wifi

									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.wifiEnable:%d\n", ConfigCommandMsg.deviceConfig.wifiEnable);
								}
								else if(ConfigCommandMsg.deviceConfig.wifiEnable == 2)//禁用wifi
								{
									ConfigCommandFlag[3] = 1;

									gSysInfo_struct.wifi.wifiEnableFlag = 0;
									netWorkStatus.wifi_state = 0;
									ret = 2;//返回 禁用了wifi

									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.wifiEnable:%d\n", ConfigCommandMsg.deviceConfig.wifiEnable);
								}

								if(ConfigCommandMsg.deviceConfig.gsmActiveSubId == 1)//当前优先激活SIM卡1槽位
								{
									ConfigCommandFlag[7] = 1;

									gSysInfo_struct.net4G.first_sim = ConfigCommandMsg.deviceConfig.gsmActiveSubId;
									
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.gsmActiveSubId:%d\n", ConfigCommandMsg.deviceConfig.gsmActiveSubId);
									// Wt_NodeEvent_Ff(NODE_SIM_CONFIG_EVT);
								}
								else if(ConfigCommandMsg.deviceConfig.gsmActiveSubId == 2)//当前优先激活SIM卡2槽位
								{
									ConfigCommandFlag[7] = 1;

									gSysInfo_struct.net4G.first_sim = ConfigCommandMsg.deviceConfig.gsmActiveSubId;
									
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.gsmActiveSubId:%d\n", ConfigCommandMsg.deviceConfig.gsmActiveSubId);
									// Wt_NodeEvent_Ff(NODE_SIM_CONFIG_EVT);
								}

								if(ConfigCommandMsg.deviceConfig.cubbyInterval != 0)//列信息上报定时间隔
								{
									ConfigCommandFlag[8] = 1;

									gSysInfo_struct.cubbyInterval = ConfigCommandMsg.deviceConfig.cubbyInterval;
									
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyInterval:%d\n", ConfigCommandMsg.deviceConfig.cubbyInterval);
								}

								if(ConfigCommandMsg.deviceConfig.cubbyHeaderInterval != 0)//列表信息上报间隔
								{
									ConfigCommandFlag[9] = 1;

									gSysInfo_struct.cubbyHeaderInterval = ConfigCommandMsg.deviceConfig.cubbyHeaderInterval;
									
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyHeaderInterval:%d\n", ConfigCommandMsg.deviceConfig.cubbyHeaderInterval);
								}

								if(ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable == 1)//蓝牙音箱广播开启
								{
									ConfigCommandFlag[12] = 1;

									g_deviceGlobalInfo.BT_ADVstate = 1;

									Wt_NodeEvent_Ff(NODE_BT_CMD_BTMUSIC_ADV_SWITCH);

									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable:%d\n", ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable);
								}
								else if(ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable == 2)//蓝牙音箱广播关闭
								{
									ConfigCommandFlag[12] = 1;

									g_deviceGlobalInfo.BT_ADVstate = 0;

									Wt_NodeEvent_Ff(NODE_BT_CMD_BTMUSIC_ADV_SWITCH);

									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable:%d\n", ConfigCommandMsg.deviceConfig.bleSpeakerBroadEnable);
								}

								if(ConfigCommandMsg.deviceConfig.bleSpeakerVolume != 0)//蓝牙音箱音量
								{
									ConfigCommandFlag[10] = 1;

									if((ConfigCommandMsg.deviceConfig.bleSpeakerVolume >= 1) && (ConfigCommandMsg.deviceConfig.bleSpeakerVolume <= 100))
									{
										gSysInfo_struct.btInfo.btToneVolume = ConfigCommandMsg.deviceConfig.bleSpeakerVolume;									
										g_deviceGlobalInfo.BT_ToneVol = gSysInfo_struct.btInfo.btToneVolume;
									}
									else
									{
									}
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerVolume:%d\n", ConfigCommandMsg.deviceConfig.bleSpeakerVolume);
								}

								if((strlen(ConfigCommandMsg.deviceConfig.bleSpeakerName) != 0)&&((ConfigCommandMsg.deviceConfig.bleSpeakerName[0]!=0x0e)&&(ConfigCommandMsg.deviceConfig.bleSpeakerName[1]!=0)))//设置蓝牙音箱名称
								{
									ConfigCommandFlag[14] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerName:%s\n", ConfigCommandMsg.deviceConfig.bleSpeakerName);

									memset(gSysInfo_struct.btInfo.btName, 0, sizeof(gSysInfo_struct.btInfo.btName));
									memcpy(gSysInfo_struct.btInfo.btName, ConfigCommandMsg.deviceConfig.bleSpeakerName, strlen(ConfigCommandMsg.deviceConfig.bleSpeakerName));
								}

								if((strlen(ConfigCommandMsg.deviceConfig.bleSpeakerPairCode) != 0)
								&&((ConfigCommandMsg.deviceConfig.bleSpeakerPairCode[0]!=0x0e)
								 &&(ConfigCommandMsg.deviceConfig.bleSpeakerPairCode[1]!=0)))//蓝牙音箱配对码
								{
									ConfigCommandFlag[11] = 1;

									if(strlen(ConfigCommandMsg.deviceConfig.bleSpeakerPairCode)>=4)
									{
										memset(g_deviceGlobalInfo.BTpinCode, 0, sizeof(g_deviceGlobalInfo.BTpinCode));
										memcpy(g_deviceGlobalInfo.BTpinCode, ConfigCommandMsg.deviceConfig.bleSpeakerPairCode, strlen(ConfigCommandMsg.deviceConfig.bleSpeakerPairCode));
										
										Wt_NodeEvent_Ff(NODE_BT_CMD_SET_BTPIN_CODE);

										NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerPairCode:%s\n", ConfigCommandMsg.deviceConfig.bleSpeakerPairCode);
									}
								}

								if((strlen(ConfigCommandMsg.deviceConfig.clientIdList) != 0)&&((ConfigCommandMsg.deviceConfig.clientIdList[0]!=0x0E)&&(ConfigCommandMsg.deviceConfig.clientIdList[1]!=0x00)))
								{
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.clientIdList:%s\n", ConfigCommandMsg.deviceConfig.clientIdList);
									ConfigCommandFlag[13] = 1;
									
									get_clientIdList_keywords_pb(ConfigCommandMsg.deviceConfig.clientIdList, 10);

									Wt_NodeEvent_Ff((0xFF << 8) | NODE_CLIENT_ID_LIST_SET_EVT);

									for(k=0;k<10;k++)
									{
										NETWORK_DEBUG("gSysInfo_struct.clientIdInfo.clientIdListBuf[%d] = %d\r\n",k, gSysInfo_struct.clientIdInfo.clientIdListBuf[k]);
									}
								}

								//设置从控仓位禁用信息
								if((ConfigCommandMsg.deviceConfig.cubbyDisable_count>0)&&(ConfigCommandMsg.deviceConfig.cubbyDisable_count<=4))
								{
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable_count:%d\n", ConfigCommandMsg.deviceConfig.cubbyDisable_count);
									ConfigCommandFlag[15] = 1;

									for(i=0;i<ConfigCommandMsg.deviceConfig.cubbyDisable_count;i++)
									{
										NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable[%d].addr:%d\n", i, ConfigCommandMsg.deviceConfig.cubbyDisable[i].addr);
										NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable[%d].slotDisable:%s\n", i, ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable);

										if((strlen(ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable) != 0)&&((ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable[0]!=0x0E)&&(ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable[1]!=0x00)))
										{
											for(k=0;k<6;k++)
											{
												NETWORK_DEBUG("gSysInfo_struct.slot.slotForbiddenFlag:0x%08x\n", gSysInfo_struct.slot.slotForbiddenFlag);
												if(ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable[k] == '1')
												{
													NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable[%d].slotDisable[%d] = %d\n", i, k, ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable[k]);
													gSysInfo_struct.slot.slotForbiddenFlag = (1<<(k+(ConfigCommandMsg.deviceConfig.cubbyDisable[i].addr - 1)*6)) | gSysInfo_struct.slot.slotForbiddenFlag;
												}
												else if(ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable[k] == '2')
												{
													NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable[%d].slotDisable[%d] = %d\n", i, k, ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable[k]);
													gSysInfo_struct.slot.slotForbiddenFlag = (~(1<<(k+(ConfigCommandMsg.deviceConfig.cubbyDisable[i].addr - 1)*6))) & gSysInfo_struct.slot.slotForbiddenFlag;
												}
												NETWORK_DEBUG("gSysInfo_struct.slot.slotForbiddenFlag:0x%08x\n", gSysInfo_struct.slot.slotForbiddenFlag);
											}
										}
									}
									
									gSysInfo_struct.slot.forbiddenSetFlag = 0xAA;
									NETWORK_DEBUG("gSysInfo_struct.slot.forbiddenSetFlag:%d\n", gSysInfo_struct.slot.forbiddenSetFlag);

									Wt_NodeEvent_Ff((0xFF << 8) | NODE_BAT_CHARGE_CTRL_EVT);
								}

								if((strlen(ConfigCommandMsg.deviceConfig.ssid) > 0)&&((ConfigCommandMsg.deviceConfig.ssid[0]!=0x0e)&&(ConfigCommandMsg.deviceConfig.ssid[1]!=0)))//wifi名称
								{
									ConfigCommandFlag[4] = 1;

									memset(gSysInfo_struct.wifi.password, 0, sizeof(gSysInfo_struct.wifi.password));
									memset(gSysInfo_struct.wifi.ssid, 0, sizeof(gSysInfo_struct.wifi.ssid));
									memcpy(gSysInfo_struct.wifi.ssid, ConfigCommandMsg.deviceConfig.ssid, strlen(ConfigCommandMsg.deviceConfig.ssid));

									gSysInfo_struct.wifi.wifiConfigFlag = 0xAA;

									if((ConfigCommandFlag[3] == 1)&&(gSysInfo_struct.wifi.wifiEnableFlag == 1))//配置了wifi使能
									{
										ret = 7;//返回 设置了wif使能并且设置了wif名称
									}
									else if((ConfigCommandFlag[3] == 1)&&(gSysInfo_struct.wifi.wifiEnableFlag == 0))//优先返回禁用wifi
									{
										ret = 5;//返回 设置了禁用wifi并且设置了wifi名称
									}
									else
									{
										ret = 3;//返回 设置了wifi名称
									}

									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.ssid:%s\n", ConfigCommandMsg.deviceConfig.ssid);
								}

								if((strlen(ConfigCommandMsg.deviceConfig.password) > 0)&&((ConfigCommandMsg.deviceConfig.password[0]!=0x0e)&&(ConfigCommandMsg.deviceConfig.password[1]!=0)))//wifi密码
								{
									ConfigCommandFlag[5] = 1;

									memset(gSysInfo_struct.wifi.password, 0, sizeof(gSysInfo_struct.wifi.password));
									memcpy(gSysInfo_struct.wifi.password, ConfigCommandMsg.deviceConfig.password, strlen(ConfigCommandMsg.deviceConfig.password));
									
									gSysInfo_struct.wifi.wifiConfigFlag = 0xAA;
									
									if((ConfigCommandFlag[3] == 1)&&(ConfigCommandFlag[4] == 1)&&(gSysInfo_struct.wifi.wifiEnableFlag == 1))//配置了wifi使能
									{
										ret = 9;//返回 设置了wifi使能并且设置了wifi名称和wifi密码
									}
									else if((ConfigCommandFlag[3] == 1)&&(ConfigCommandFlag[4] == 1)&&(gSysInfo_struct.wifi.wifiEnableFlag == 0))//优先返回  禁用wifi
									{
										ret = 10;//返回 禁用了wifi 设置了wifi名称和wifi密码
									}
									else if((ConfigCommandFlag[3] == 1)&&(gSysInfo_struct.wifi.wifiEnableFlag == 1))//配置了wifi使能
									{
										ret = 8;//返回 设置了wifi使能并且设置了wifi密码
									}
									else if((ConfigCommandFlag[3] == 1)&&(gSysInfo_struct.wifi.wifiEnableFlag == 0))//优先返回  禁用wifi
									{
										ret = 6;//返回 禁用了wifi 设置了wifi密码
									}
									else
									{
										ret = 4;//返回 设置了wifi密码
									}
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.password:%s\n", ConfigCommandMsg.deviceConfig.password);
								}

								if(ConfigCommandMsg.deviceConfig.remember == 1)//保存WiFi名称和WiFi密码
								{
									ConfigCommandFlag[6] = 1;
									gSysInfo_struct.wifi.remember = 1;
									gSysInfo_struct.wifi.isSaveFlash = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.remember:%d\n", ConfigCommandMsg.deviceConfig.remember);
								}
								else if(ConfigCommandMsg.deviceConfig.remember == 2)//不保存WiFi名称和WiFi密码
								{
									ConfigCommandFlag[6] = 1;
									gSysInfo_struct.wifi.remember = 0;
									gSysInfo_struct.wifi.isSaveFlash = 0;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.remember:%d\n", ConfigCommandMsg.deviceConfig.remember);
								}

								if((ConfigCommandFlag[2] == 1)	//配置了BLE使能
								||(ConfigCommandFlag[3] == 1)	//配置了wifi使能
								||(ConfigCommandFlag[4] == 1)	//配置了wifi名称
								||(ConfigCommandFlag[5] == 1)	//配置了wifi密码
								||(ConfigCommandFlag[6] == 1)	//配置了是否保存wifi名称和密码
								||(ConfigCommandFlag[7] == 1)	//配置了优先激活SIM卡号
								||(ConfigCommandFlag[8] == 1)	//配置了列信息上报定时间隔
								||(ConfigCommandFlag[9] == 1)	//配置了列表信息上报间隔
								||(ConfigCommandFlag[10] == 1)	//配置了蓝牙音箱音量
								||(ConfigCommandFlag[13] == 1)	//配置了运营商列表信息
								||(ConfigCommandFlag[14] == 1)	//配置了蓝牙音箱名称
								||(ConfigCommandFlag[15] == 1)	//配置了从控仓位禁用信息
								)
								{
									tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
								}

								InfoDeviceConfigDataPubTopic(deviceReportType_operationReport, ConfigCommandMsg.header.traceId, ConfigCommandMsg.command);
							}
							return ret;
						}
						else if(ConfigCommandMsg.command == 27)/* 清除 27 特定值 */
						{
							ret = 0;
							ConfigCommandFlag[0] = ConfigCommandMsg.command;//记录当前命令

							if(ConfigCommandMsg.has_deviceConfig == 1)//有设备参数需要清除
							{
								NETWORK_DEBUG("ConfigCommandMsg.has_deviceConfig:%d\n", ConfigCommandMsg.has_deviceConfig);
								ConfigCommandFlag[1] = 1;

								if(strcmp(ConfigCommandMsg.deviceConfig.ssid, characteristic27) >= 0)//清除wifi名称
								{
									ConfigCommandFlag[4] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.ssid:%s\n", ConfigCommandMsg.deviceConfig.ssid);
																
									memset(gSysInfo_struct.wifi.ssid, 0, sizeof(gSysInfo_struct.wifi.ssid));

									gSysInfo_struct.wifi.wifiConfigFlag = 0;

									ret = 11;//清除wifi名称
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.password, characteristic27) >= 0)//清除wifi密码
								{
									ConfigCommandFlag[5] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.password:%s\n", ConfigCommandMsg.deviceConfig.password);

									memset(gSysInfo_struct.wifi.password, 0, sizeof(gSysInfo_struct.wifi.password));

									gSysInfo_struct.wifi.wifiConfigFlag = 0;

									if(ConfigCommandFlag[4] == 1)
									{
										ret = 13;//清除wifi名称和密码
									}
									else
									{
										ret = 12;//清除wifi密码
									}
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.bleSpeakerPairCode, characteristic27) >= 0)//清除蓝牙音箱配对码
								{
									ConfigCommandFlag[11] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerPairCode:%s\n", ConfigCommandMsg.deviceConfig.bleSpeakerPairCode);
									g_deviceGlobalInfo.BTpinCode[0] = '2';
									g_deviceGlobalInfo.BTpinCode[1] = '2';
									g_deviceGlobalInfo.BTpinCode[2] = '2';
									g_deviceGlobalInfo.BTpinCode[3] = '2';
									g_deviceGlobalInfo.BTpinCode[4] = 0;
									memset(g_deviceGlobalInfo.BTpinCode, 0, sizeof(g_deviceGlobalInfo.BTpinCode));

									Wt_NodeEvent_Ff(NODE_BT_CMD_SET_BTPIN_CODE);
									Wt_NodeEvent_Ff(NODE_BT_CMD_CLEAN_CONNECT_INFO);
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.clientIdList, characteristic27) >= 0)
								{
									ConfigCommandFlag[13] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.clientIdList:%s\n", ConfigCommandMsg.deviceConfig.clientIdList);

									for(k=0;k<10;k++)
									{
										gSysInfo_struct.clientIdInfo.clientIdListBuf[k] = 99;
										NETWORK_DEBUG("gSysInfo_struct.clientIdInfo.clientIdListBuf[%d] = %d\r\n",k, gSysInfo_struct.clientIdInfo.clientIdListBuf[k]);
									}
									Wt_NodeEvent_Ff((0xFF << 8) | NODE_CLIENT_ID_LIST_SET_EVT);
								}

								if(strcmp(ConfigCommandMsg.deviceConfig.bleSpeakerName, characteristic27) >= 0)//清除蓝牙音箱名称
								{
									ConfigCommandFlag[14] = 1;
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.bleSpeakerName:%s\n", ConfigCommandMsg.deviceConfig.bleSpeakerName);
									memset(gSysInfo_struct.btInfo.btName, 0, sizeof(gSysInfo_struct.btInfo.btName));
								}

								//清除从控仓位禁用信息
								if((ConfigCommandMsg.deviceConfig.cubbyDisable_count>0)&&(ConfigCommandMsg.deviceConfig.cubbyDisable_count<=4))
								{
									NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable_count:%d\n", ConfigCommandMsg.deviceConfig.cubbyDisable_count);
									for(i=0;i<ConfigCommandMsg.deviceConfig.cubbyDisable_count;i++)
									{
										NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable[%d].addr:%d\n", i, ConfigCommandMsg.deviceConfig.cubbyDisable[i].addr);
										NETWORK_DEBUG("ConfigCommandMsg.deviceConfig.cubbyDisable[%d].slotDisable:%s\n", i, ConfigCommandMsg.deviceConfig.cubbyDisable[i].slotDisable);
									}

									if(ConfigCommandMsg.deviceConfig.cubbyDisable[0].addr>0)
									{
										if(strcmp(ConfigCommandMsg.deviceConfig.cubbyDisable[0].slotDisable, characteristic27) >= 0)
										{
											ConfigCommandFlag[15] = 1;
											gSysInfo_struct.slot.slotForbiddenFlag = 0;
											gSysInfo_struct.slot.forbiddenSetFlag = 0;

											Wt_NodeEvent_Ff((0xFF << 8) | NODE_BAT_CHARGE_CTRL_EVT);
										}
									}

									NETWORK_DEBUG("gSysInfo_struct.slot.slotForbiddenFlag:0x%08x\n", gSysInfo_struct.slot.slotForbiddenFlag);
									NETWORK_DEBUG("gSysInfo_struct.slot.forbiddenSetFlag:%d\n", gSysInfo_struct.slot.forbiddenSetFlag);
								}

								if((ConfigCommandFlag[4] == 1)	//配置了wifi名称
								||(ConfigCommandFlag[5] == 1)	//配置了wifi密码
								||(ConfigCommandFlag[13] == 1)	//配置了运营商列表信息
								||(ConfigCommandFlag[14] == 1)	//配置了蓝牙音箱名称
								||(ConfigCommandFlag[15] == 1)	//配置了从控仓位禁用信息
								)
								{
									tls_fls_write(DEVICE_COFNIG_PARA_INFO_FLASH_ADDR, (u8*)&gSysInfo_struct, sizeof(gSysInfo_struct));
								}

								InfoDeviceConfigDataPubTopic(deviceReportType_operationReport, ConfigCommandMsg.header.traceId, ConfigCommandMsg.command);
							}
							return ret;
						}
						return ret;
					}
					else if (strcmp(pKeyWord,operation2_device) == 0) 
					{
						ret = 0;
						printf("command/device\r\n");

						memset(buffer, 0, 200);

						memcpy(buffer,pbdata,datalen);

						DeviceCommand DeviceCommandMsg = {0};

						pb_istream_t istream = pb_istream_from_buffer(buffer, datalen);
						if (pb_decode(&istream, DeviceCommand_fields, &DeviceCommandMsg) == false) 
						{
							printf("Decoding failed:\n");
							return 0;
						}
						printf("command/device Decoding success!!!:\n");
						printf("DeviceCommandMsg.has_header:%d\n", DeviceCommandMsg.has_header);
						printf("DeviceCommandMsg.header.opCode:%d\n", DeviceCommandMsg.header.opCode);
						printf("DeviceCommandMsg.header.seconds:%d\n", DeviceCommandMsg.header.seconds);
						printf("DeviceCommandMsg.header.traceId:%d\n", DeviceCommandMsg.header.traceId);
						printf("DeviceCommandMsg.header.protocol:%d\n", DeviceCommandMsg.header.protocol);

						printf("DeviceCommandMsg.command:%d\n", DeviceCommandMsg.command);

						if(DeviceCommandMsg.header.opCode > g_deviceGlobalInfo.CloudopCode) //检查云端指令编号 
						{
							g_deviceGlobalInfo.CloudopCode = DeviceCommandMsg.header.opCode;
						}
						else
						{
							if((DeviceCommandMsg.header.opCode < g_deviceGlobalInfo.CloudopCode)&&(g_deviceGlobalInfo.CloudopCode == 0xFFFFFFFF))
							{
								g_deviceGlobalInfo.CloudopCode = DeviceCommandMsg.header.opCode;
							}
							else
							{
								printf("DeviceCommandMsg.header.opCode <= g_deviceGlobalInfo.CloudopCode\n");
								// return 0;
							}
						}

						if((DeviceCommandMsg.header.traceId>=0)&&(DeviceCommandMsg.header.traceId<=0xFFFF))
						{
						}
						else
						{
							DeviceCommandMsg.header.traceId = 0;
						}


						if(DeviceCommandMsg.command == 9)//上报CubbyHeader数据
						{
							InfoDeviceListPubTopic(deviceReportType_operationReport, DeviceCommandMsg.header.traceId, DeviceCommandMsg.command);		/* CubbyHeader数据上报 从控列数据上报 */
						}
						else if(DeviceCommandMsg.command == 10)//清除蓝牙音箱配对信息
						{
							Wt_NodeEvent_Ff(NODE_BT_CMD_CLEAN_CONNECT_INFO);
						}
						else if(DeviceCommandMsg.command == 11)//重新路由
						{
							
						}
						else if(DeviceCommandMsg.command == 12)//设备重启
						{
							device_reboot();
						}
						else if(DeviceCommandMsg.command == 13)//设备关机
						{
							device_poweroff();
						}
						else if(DeviceCommandMsg.command == 14)//重新激活
						{
							memset((u8*)&gSysLoginInfo, 0, sizeof(gSysLoginInfo));
							tls_fls_write(DEVICE_LOGIN_INFO_FLASH_ADDR, (u8*)&gSysLoginInfo, sizeof(gSysLoginInfo));
							device_reboot();
						}
						else if(DeviceCommandMsg.command == 15)//ping pang
						{
							if(net4G_process_steps.netWork == NETWORK_WORK_STEP__NORMAL)
							{
								InfoDeviceDataPubTopic(deviceReportType_operationReport, DeviceCommandMsg.header.traceId, DeviceCommandMsg.command);
							}
							else
							{
								ret = 14;
							}
						}
						else if(DeviceCommandMsg.command == 16)//上报版本信息
						{
							InfoDeviceVersionPubTopic(deviceReportType_operationReport, DeviceCommandMsg.header.traceId, DeviceCommandMsg.command, 0);		/* DeviceVersion数据上报 */
						}
						else if(DeviceCommandMsg.command == 18)//上报wifi信息
						{
							InfoWiFiDataPubTopic(deviceReportType_operationReport, DeviceCommandMsg.header.traceId, DeviceCommandMsg.command);			/* WiFiData数据上报 */
						}
						else if(DeviceCommandMsg.command == 19)//上报4G信息
						{
							InfoGsmDataPubTopic(deviceReportType_operationReport, DeviceCommandMsg.header.traceId, DeviceCommandMsg.command);			/* GsmData数据上报 */
						}
						else if(DeviceCommandMsg.command == 20)//上报租借蓝牙信息
						{
							InfoBleDataPubTopic(deviceReportType_operationReport, DeviceCommandMsg.header.traceId, DeviceCommandMsg.command);			/* 蓝牙模块数据上报 */
						}
						else if(DeviceCommandMsg.command == 21)//重新配置从控地址
						{
							Wt_NodeEvent_Ff(NODE_SLAVE_NO_SET_EVT);
						}
						else if(DeviceCommandMsg.command == 36)//重启测试
						{
							device_reboot_test();
						}


						
						DeviceCommandMsg.header.traceId = 0;	//清零TraceId
						return ret;
					}
					else if (strcmp(pKeyWord,operation2_ota) == 0) 
					{
						ret = 0;
						printf("command/ota\r\n");

						memset(buffer, 0, 200);

						memcpy(buffer,pbdata,datalen);

						Ota OtaMsg = {0};

						pb_istream_t istream = pb_istream_from_buffer(buffer, datalen);
						if (pb_decode(&istream, Ota_fields, &OtaMsg) == false) 
						{
							printf("Decoding failed:\n");
							return 0;
						}
						printf("command/ota Decoding success!!!:\n");
						printf("OtaMsg.has_header:%d\n", OtaMsg.has_header);
						printf("OtaMsg.header.opCode:%d\n", OtaMsg.header.opCode);
						printf("OtaMsg.header.seconds:%d\n", OtaMsg.header.seconds);
						printf("OtaMsg.header.traceId:%d\n", OtaMsg.header.traceId);
						printf("OtaMsg.header.protocol:%d\n", OtaMsg.header.protocol);

						printf("OtaMsg.command:%d\n", OtaMsg.command);
						printf("OtaMsg.versionType:%s\n", OtaMsg.versionType);
						// printf("OtaMsg.url:%s\n", OtaMsg.url);
						printf("OtaMsg.addr:%d\n", OtaMsg.addr);
						printf("OtaMsg.crc:%d\n", OtaMsg.crc);
						printf("OtaMsg.size:%d\n", OtaMsg.size);
						printf("OtaMsg.version:%s\n", OtaMsg.version);
						printf("OtaMsg.hv:%s\n", OtaMsg.hv);

						if(OtaMsg.header.opCode > g_deviceGlobalInfo.CloudopCode) //检查云端指令编号 
						{
							g_deviceGlobalInfo.CloudopCode = OtaMsg.header.opCode;
						}
						else
						{
							if((OtaMsg.header.opCode < g_deviceGlobalInfo.CloudopCode)&&(g_deviceGlobalInfo.CloudopCode == 0xFFFFFFFF))
							{
								g_deviceGlobalInfo.CloudopCode = OtaMsg.header.opCode;
							}
							else
							{
								printf("OtaMsg.header.opCode <= g_deviceGlobalInfo.CloudopCode\n");
								// return 0;
							}
						}


						if((OtaMsg.header.traceId>=0)&&(OtaMsg.header.traceId<=0xFFFF))
						{
							g_deviceGlobalInfo.CloudTraceId = OtaMsg.header.traceId;
						}

						// if((OtaMsg.command==29)&&(g_deviceGlobalInfo.DeviceLowPowerMode != 1)&&(g_deviceGlobalInfo.DeviceNOPowerMode != 1))	//升级 非低功耗模式
						if((OtaMsg.command==29)&&(g_deviceGlobalInfo.DeviceNOPowerMode != 1))	//升级 非低功耗模式
						{
							if((strcmp(OtaMsg.versionType,operation3_ota_mcu) == 0)&&(0 == strcmp(OtaMsg.hv, main_SnHardSoftVer.main_HardVer))) //主控
							{
								//升级类型 主控
								g_ota_para_t.type = UPDATE_MASTER_CTRL_MODULE;

								//硬件版本
								if(0 == strcmp(OtaMsg.hv, main_SnHardSoftVer.main_HardVer))
								{
									g_ota_para_t.hardVer = atoi(OtaMsg.hv);
								}

								//从控地址
								g_ota_para_t.addr = 0;
							}
							else if(strcmp(OtaMsg.versionType,operation3_ota_cubby) == 0) //从控
							{
								//升级类型 从控
								g_ota_para_t.type = UPDATE_SLAVE_BOARD_MODULE;
								
								//硬件版本
								// if(0 == strcmp(OtaMsg.hv, CUBBY_HARDWARE_VERSION_SN_HV))
								{
									g_ota_para_t.hardVer = atoi(OtaMsg.hv);
								}

								//从控地址
								if(((OtaMsg.addr>0)&&(OtaMsg.addr<=4))||(OtaMsg.addr == 0x0F))
								{
									g_ota_para_t.addr = OtaMsg.addr;
								}
								else
								{
									g_ota_para_t.addr = 0;
								}
							}
							else if((strcmp(OtaMsg.versionType,operation3_ota_btSpeaker) == 0)&&(0 == strcmp(OtaMsg.hv, main_SnHardSoftVer.main_HardVer)))  //蓝牙音箱
							{
								//升级类型 蓝牙音箱
								g_ota_para_t.type = UPDATE_BT_MUSIC_MODULE;

								//硬件版本
								if(0 == strcmp(OtaMsg.hv, main_SnHardSoftVer.main_HardVer))
								{
									g_ota_para_t.hardVer = atoi(OtaMsg.hv);
								}

								//从控地址，主控为0
								g_ota_para_t.addr = 0;
							}
							else if((strcmp(OtaMsg.versionType,operation3_ota_gsm) == 0)&&(0 == strcmp(OtaMsg.hv, main_SnHardSoftVer.main_HardVer)))  //4G模块
							{
								//升级类型 4G模块
								g_ota_para_t.type = UPDATE_4G_BOARD;

								//硬件版本
								if(0 == strcmp(OtaMsg.hv, main_SnHardSoftVer.main_HardVer))
								{
									g_ota_para_t.hardVer = atoi(OtaMsg.hv);
								}

								//从控地址，主控为0
								g_ota_para_t.addr = 0;
							}


							//下载链接
							memcpy(g_ota_para_t.url, OtaMsg.url, strlen(OtaMsg.url));

							//固件crc
							g_ota_para_t.crc = OtaMsg.crc;

							//固件大小
							g_ota_para_t.size = OtaMsg.size;

							//固件版本号
							memcpy(g_ota_para_t.ver, OtaMsg.version, strlen(OtaMsg.version));

							//开始升级标志
							g_ota_para_t.otaFlag = 1;

							printf("g_ota_para_t.type:%d\n", g_ota_para_t.type);
							// printf("g_ota_para_t.url:%s\n", g_ota_para_t.url);
							printf("g_ota_para_t.crc:%d\n", g_ota_para_t.crc);
							printf("g_ota_para_t.size:%d\n", g_ota_para_t.size);
							printf("g_ota_para_t.ver:%s\n", g_ota_para_t.ver);
							printf("g_ota_para_t.hardVer:%d\n", g_ota_para_t.hardVer);
							printf("g_ota_para_t.addr:%d\n", g_ota_para_t.addr);

							app_ble_disable(); //disable ble
							printf("[CMD][OTA] goto download fw.\r\n");
						}
						// if((g_deviceGlobalInfo.DeviceLowPowerMode != 1)&&(g_deviceGlobalInfo.DeviceNOPowerMode != 1))
						if(g_deviceGlobalInfo.DeviceNOPowerMode != 1)
						{
							//应答cmd 不上报type
							InfoDeviceVersionPubTopic(deviceReportType_operationReport, OtaMsg.header.traceId, OtaMsg.command, 0);		/* DeviceVersion数据上报 */
						}
						else
						{
							InfoLowPowerDataPubTopic(deviceReportType_operationReport, 0, 0);		/* 设备低功耗模式数据上报 */
						}
						
						return ret;
					}
					else if (strcmp(pKeyWord,operation2_lowpower) == 0) 
					{
						ret = 0;
						printf("command/lowpower\r\n");

						memset(buffer, 0, 200);

						memcpy(buffer,pbdata,datalen);

						LowpowerCommand LowpowerMsg = {0};

						pb_istream_t istream = pb_istream_from_buffer(buffer, datalen);
						if (pb_decode(&istream, LowpowerCommand_fields, &LowpowerMsg) == false) 
						{
							printf("Decoding failed:\n");
							return 0;
						}
						printf("command/Lowpower Decoding success!!!:\n");
						printf("LowpowerMsg.has_header:%d\n", LowpowerMsg.has_header);
						printf("LowpowerMsg.header.opCode:%d\n", LowpowerMsg.header.opCode);
						printf("LowpowerMsg.header.seconds:%d\n", LowpowerMsg.header.seconds);
						printf("LowpowerMsg.header.traceId:%d\n", LowpowerMsg.header.traceId);
						printf("LowpowerMsg.header.protocol:%d\n", LowpowerMsg.header.protocol);

						printf("LowpowerMsg.command:%d\n", LowpowerMsg.command);
						printf("LowpowerMsg.lowpower:%d\n", LowpowerMsg.lowpower);


						if(LowpowerMsg.header.opCode > g_deviceGlobalInfo.CloudopCode) //检查云端指令编号 
						{
							g_deviceGlobalInfo.CloudopCode = LowpowerMsg.header.opCode;
						}
						else
						{
							if((LowpowerMsg.header.opCode < g_deviceGlobalInfo.CloudopCode)&&(g_deviceGlobalInfo.CloudopCode == 0xFFFFFFFF))
							{
								g_deviceGlobalInfo.CloudopCode = LowpowerMsg.header.opCode;
							}
							else
							{
								printf("LowpowerMsg.header.opCode <= g_deviceGlobalInfo.CloudopCode\n");
								// return 0;
							}
						}


						if((LowpowerMsg.header.traceId>=0)&&(LowpowerMsg.header.traceId<=0xFFFF))
						{
							g_deviceGlobalInfo.CloudTraceId = LowpowerMsg.header.traceId;
						}

						if(LowpowerMsg.command==33)	//低功耗模式设置
						{
							if(LowpowerMsg.lowpower == 1) //1进入低功耗
							{
								g_deviceGlobalInfo.DeviceLowPowerMode = 1;
								g_deviceGlobalInfo.DeviceNOPowerMode = 1; //进入无220V供电模式
							}
							else if(LowpowerMsg.lowpower == 2) //2退出低功耗
							{
								g_deviceGlobalInfo.DeviceLowPowerMode = 2;
								g_deviceGlobalInfo.DeviceNOPowerMode = 2; //退出无220V供电模式
							}
						}

						// Wt_NodeEvent_Ff((0xF3 << 8) | NODE_LOW_POWER_MODE_UP_EVT);	//设备低功耗模式上报 0xF3操作上报
						//应答cmd
						InfoLowPowerDataPubTopic(deviceReportType_operationReport, LowpowerMsg.header.traceId, LowpowerMsg.command);		/* 设备低功耗模式数据上报 */
						return ret;
					}
				}
			}
		}
	}
}


PROCESS_STEP_RET_E module_4g_communication_process(void)
{
	uint32_t aes_data_len = 0;
	uint8_t* pCmdData = NULL;
	static uint32_t ota_start_delay_time = 0;
	static uint32_t lendTimeout_wait_time = 0;
	uint8_t pb_parse_ret = 0;
	PROCESS_STEP_RET_E net4g_process_ret = PROCESS_WAIT;
	uint8_t sendRet = 0;

	if(get_net4G_recv_cmd() == 10)//MQTT断开连接和模块重启，会断网重连
	{
		net4G_process_steps.communicateStep = 0;//4G流程工步，通讯工步，恢复默认起始工步
		return PROCESS_NET_DISCONNECT;			//网络断开连接
	}

	//OTA升级
	if(g_ota_para_t.otaFlag == 1)
	{
		if(g_deviceGlobalInfo.batLendingFlag > 0)//有租借事件，延时等待租借事件完成再进行OTA流程
		{
			if(lendTimeout_wait_time++ >= 6000)
			{
				lendTimeout_wait_time = 0;
				g_deviceGlobalInfo.batLendingFlag = 0;
			}
		}
		else
		{
			if(ota_start_delay_time == 500) //wait 500ms upload start ota event to xd cloud
			{
				// create_device_ota_start_rsp();//上报OTA开始事件

				//固件升级应答 29  应答类型 type  固件开始下载
				InfoDeviceVersionPubTopic(deviceReportType_operationReport, 0, 29, 1);		/* DeviceVersion数据上报 */
			}
			else if(ota_start_delay_time == 1500)
			{
				network_uart_send(NET_CMD_MQTT_DISCONNECT, strlen(NET_CMD_MQTT_DISCONNECT)); //mqtt disconnect

				if(g_ota_para_t.addr == 0)
				{
					Wt_NodeEvent_Ff((0xF0 << 8) | NODE_SLAVE_LED_CTRL_EVT);  //slot led blink
				}
				else if(g_ota_para_t.addr == 0x0f)
				{
					Wt_NodeEvent_Ff((0xF0 << 8) | NODE_SLAVE_LED_CTRL_EVT);  //slot led blink
				}
				else if((g_ota_para_t.addr >= 1)&&(g_ota_para_t.addr <= 4))
				{
					Wt_NodeEvent_Ff((((g_ota_para_t.addr-1)*6) << 8) | NODE_SLAVE_LED_CTRL_EVT);  //slot led blink
				}
			}
			else if(ota_start_delay_time == 5000)
			{
				ota_start_delay_time = 0;
				set_net4g_work_step(NETWORK_WORK_STEP__OTA); //goto ota process
			}
			ota_start_delay_time++;
		}
	}
	
	switch(net4G_process_steps.communicateStep)
	{
		case 0: //config recv cb
			register_network_uart_cb(&net4g_uart_recv_cb);

			Wt_NodeEvent_Ff((0xFF << 8) | NODE_BAT_POWER_CHECK_EVT);//battery power get
			
			Wt_NodeEvent_Ff((0xFF << 8) | NODE_DEVICE_INFO_EVT);  //设备信息上报
			
			set_qr_code_led(1);//二维码背光灯常亮

			net4G_process_steps.communicateStep = 1;
			
			break;
			
		case 1: //wait recv 4g module cmd
			if(gNet4g_dataStruct.recv.qHead != gNet4g_dataStruct.recv.qTail)
			{
				pCmdData = gNet4g_dataStruct.recv.cmdQ[gNet4g_dataStruct.recv.qHead].qData;
				
				uint8_t ucchar[100] = {0};
				uint8_t ucdata[200] = {0};
				uint8_t ucdatalen[4] = {0};
				uint8_t aes_decode_pkcsLen = 0;
				uint16_t ucdatalen_atoi = 0;
				
#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
				if(get_at_rsp_keywords_pb(pCmdData, NET_MQTT_CTRL_CMD_HEAD, 4, ucchar,ucdatalen,ucdata))
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
				if(get_at_rsp_keywords_pb(pCmdData, NET_MQTT_CTRL_CMD_HEAD, 3, ucchar,ucdatalen,ucdata))
#endif
				{
					memset(g_payload, 0, sizeof(g_payload));

					ucdatalen_atoi = atoi(ucdatalen);
					
#if (AES128_ECB_HARD == 1)
					tls_crypto_aes_encrypt_decrypt(&ctx, ucdata, g_payload, ucdatalen_atoi, CRYPTO_WAY_DECRYPT);
					aes_decode_pkcsLen = g_payload[ucdatalen_atoi - 1];
#elif (AES128_ECB_HARD == 0)
					aes_decode_pkcsLen = aes128_ecb_decrypt(ucdata, ucdatalen_atoi,  g_deviceGlobalInfo.mqtt.secretKey, g_payload);
#endif

					NET4G_DEBUG("aes128_ecb_decrypt_msg_len： %d\n", ucdatalen_atoi);
					NET4G_DEBUG("aes128_ecb_decrypt_aes_decode_pkcsLen： %d\n", aes_decode_pkcsLen);
					// NET4G_DEBUG("strlen(g_payload)： %d\n", strlen(g_payload));
					if(aes_decode_pkcsLen <= ucdatalen_atoi)
					{
						if(aes_decode_pkcsLen<100)
						{
							pb_parse_ret = RspAnalyze(ucchar,g_payload, (ucdatalen_atoi - aes_decode_pkcsLen)); //命令解析
						}
						else
						{
							pb_parse_ret = RspAnalyze(ucchar,g_payload, ucdatalen_atoi); //命令解析
						}
						
					}
					else
					{
						pb_parse_ret = 0;
					}
					
					
					if((1==pb_parse_ret)	//使能了wifi
					 ||(3==pb_parse_ret)	//设置了wifi名称
					 ||(4==pb_parse_ret)	//设置了wifi密码
					 ||(7==pb_parse_ret)	//使能了wifi 设置了wif名称
					 ||(8==pb_parse_ret)	//使能了wifi 设置了wifi密码
					 ||(9==pb_parse_ret)	//使能了wifi 设置了wifi名称和wifi密码
					)
					{
						if((gSysInfo_struct.wifi.wifiConfigFlag == 0xAA)&&(gSysInfo_struct.wifi.wifiEnableFlag == 1)&&(strlen(gSysInfo_struct.wifi.ssid) > 0))
						{
							net4g_process_ret = PROCESS_NET_TYPE_WIFI;//切换到wifi
						}
					}
					else if((2==pb_parse_ret)	//返回 禁用了wifi
					 ||(5==pb_parse_ret)		//返回 禁用了wifi 设置了wifi名称
					 ||(6==pb_parse_ret)		//返回 禁用了wifi 设置了wifi密码
					 ||(10==pb_parse_ret)		//禁用了wifi 设置了wifi名称和wifi密码
					 ||(11==pb_parse_ret)		//清除wifi名称
					 ||(12==pb_parse_ret)		//清除wifi密码
					 ||(13==pb_parse_ret)		//清除wifi名称和密码
					)
					{
						net4g_process_ret = PROCESS_WAIT;
					}

					gNet4g_dataStruct.recv.qHead++;
					gNet4g_dataStruct.recv.qHead %= NET_4G_UART_RECV_QUEU_SIZE;
				}
				else
				{
					gNet4g_dataStruct.recv.qHead++;
					gNet4g_dataStruct.recv.qHead %= NET_4G_UART_RECV_QUEU_SIZE;

					if(0 == strcmp(pCmdData, NET_MQTT_DISCONNECT_CMD_HEAD))//设备断网
					{
						NET4G_DEBUG("[mqtt] disconnect\r\n");
						net4G_process_steps.communicateStep = 0;
						net4g_process_ret = PROCESS_NET_DISCONNECT;
					}
				}
			}
			else
			{
				if(get_pub_msg_flag())
				{
					net4G_process_steps.communicateStep = 2;
					NET4G_DEBUG("goto send cmd !\r\n");	
				}
				else
				{
					if(net4g_get_cell_info_flag==1)
					{
						module_4g_get_cell_info_process();  //获取小区信息
					}
				}
			}
		
			break;

		case 2: //pub
			sendRet = net4g_send_data_process();
			if(sendRet == 1)//数据发送成功，等待接收
			{
				reset_pub_msg_queue();
				net4G_process_steps.communicateStep = 1;
				NET4G_DEBUG("goto recv cmd !\r\n\r\n\r\n");		
			}
			else if(sendRet == 2)//发送数据设备断网
			{
				net4G_process_steps.communicateStep = 0;
				return PROCESS_NET_DISCONNECT;
			}

			break;
	}
	return net4g_process_ret;
}


PROCESS_STEP_RET_E device_reboot_module_4g_communication_process(void)
{
	// uint32_t aes_data_len = 0;
	uint8_t* pCmdData = NULL;
	static uint8_t ping_send_flag = 1;
	uint8_t pb_parse_ret = 0;
	PROCESS_STEP_RET_E net4g_process_ret = PROCESS_WAIT;
	uint8_t sendRet = 0;

	if(get_net4G_recv_cmd() == 10)//MQTT断开连接和模块重启，会断网重连
	{
		net4G_process_steps.communicateStep = 0;//4G流程工步，通讯工步，恢复默认起始工步
		return PROCESS_NET_DISCONNECT;			//网络断开连接
	}

	if(ping_send_flag == 1)
	{
		InfoDeviceDataPingPubTopic(deviceReportType_exceptionalReport, 0, 0, 3001);
		ping_send_flag = 0;
	}
	
	switch(net4G_process_steps.communicateStep)
	{
		case 0: //config recv cb
			register_network_uart_cb(&net4g_uart_recv_cb);
			net4G_process_steps.communicateStep = 1;

			break;

		case 1: //wait recv 4g module cmd
			if(gNet4g_dataStruct.recv.qHead != gNet4g_dataStruct.recv.qTail)
			{
				pCmdData = gNet4g_dataStruct.recv.cmdQ[gNet4g_dataStruct.recv.qHead].qData;

				uint8_t ucchar[100] = {0};
				uint8_t ucdata[200] = {0};
				uint8_t ucdatalen[4] = {0};
				uint8_t aes_decode_pkcsLen = 0;
				uint16_t ucdatalen_atoi = 0;

#if ((NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26E)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_NT26K)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_CE109))
				if(get_at_rsp_keywords_pb(pCmdData, NET_MQTT_CTRL_CMD_HEAD, 4, ucchar,ucdatalen,ucdata))
#elif ((NET_4G_MODULE_TYPE == NET_4G_MODULE_EC800)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EC801)||(NET_4G_MODULE_TYPE == NET_4G_MODULE_EG800GEU))
				if(get_at_rsp_keywords_pb(pCmdData, NET_MQTT_CTRL_CMD_HEAD, 3, ucchar,ucdatalen,ucdata))
#endif
				{
					memset(g_payload, 0, sizeof(g_payload));

					ucdatalen_atoi = atoi(ucdatalen);

#if (AES128_ECB_HARD == 1)
					tls_crypto_aes_encrypt_decrypt(&ctx, ucdata, g_payload, ucdatalen_atoi, CRYPTO_WAY_DECRYPT);
					aes_decode_pkcsLen = g_payload[ucdatalen_atoi - 1];
#elif (AES128_ECB_HARD == 0)
					aes_decode_pkcsLen = aes128_ecb_decrypt(ucdata, ucdatalen_atoi,  g_deviceGlobalInfo.mqtt.secretKey, g_payload);
#endif

					NET4G_DEBUG("aes128_ecb_decrypt_msg_len： %d\n", ucdatalen_atoi);
					NET4G_DEBUG("aes128_ecb_decrypt_aes_decode_pkcsLen： %d\n", aes_decode_pkcsLen);
					// NET4G_DEBUG("strlen(g_payload)： %d\n", strlen(g_payload));

					if(aes_decode_pkcsLen <= ucdatalen_atoi)
					{
						if(aes_decode_pkcsLen<100)
						{
							pb_parse_ret = RspAnalyze(ucchar,g_payload, (ucdatalen_atoi - aes_decode_pkcsLen)); //命令解析
						}
						else
						{
							pb_parse_ret = RspAnalyze(ucchar,g_payload, ucdatalen_atoi); //命令解析
						}
					}
					else
					{
						pb_parse_ret = 0;
					}

					// if((1==pb_parse_ret)	//使能了wifi
					//  ||(3==pb_parse_ret)	//设置了wifi名称
					//  ||(4==pb_parse_ret)	//设置了wifi密码
					//  ||(7==pb_parse_ret)	//使能了wifi 设置了wif名称
					//  ||(8==pb_parse_ret)	//使能了wifi 设置了wifi密码
					//  ||(9==pb_parse_ret)	//使能了wifi 设置了wifi名称和wifi密码
					// )
					// {
					// 	if((gSysInfo_struct.wifi.wifiConfigFlag == 0xAA)&&(gSysInfo_struct.wifi.wifiEnableFlag == 1)&&(strlen(gSysInfo_struct.wifi.ssid) > 0))
					// 	{
					// 		net4g_process_ret = PROCESS_NET_TYPE_WIFI;//切换到wifi
					// 	}
					// }
					// else if((2==pb_parse_ret)	//返回 禁用了wifi
					//  ||(5==pb_parse_ret)		//返回 禁用了wifi 设置了wifi名称
					//  ||(6==pb_parse_ret)		//返回 禁用了wifi 设置了wifi密码
					//  ||(10==pb_parse_ret)		//禁用了wifi 设置了wifi名称和wifi密码
					//  ||(11==pb_parse_ret)		//清除wifi名称
					//  ||(12==pb_parse_ret)		//清除wifi密码
					//  ||(13==pb_parse_ret)		//清除wifi名称和密码
					// )
					// {
					// 	net4g_process_ret = PROCESS_WAIT;
					// }

					if(14==pb_parse_ret)
					{
						net4g_process_ret = PROCESS_END;
					}
					else
					{
						NET4G_DEBUG("[mqtt] ping err !! mqtt disconnect\r\n");
						net4G_process_steps.communicateStep = 0;
						net4g_process_ret = PROCESS_NET_DISCONNECT;
					}

					gNet4g_dataStruct.recv.qHead++;
					gNet4g_dataStruct.recv.qHead %= NET_4G_UART_RECV_QUEU_SIZE;
				}
				else
				{
					gNet4g_dataStruct.recv.qHead++;
					gNet4g_dataStruct.recv.qHead %= NET_4G_UART_RECV_QUEU_SIZE;

					if(0 == strcmp(pCmdData, NET_MQTT_DISCONNECT_CMD_HEAD))//设备断网
					{
						NET4G_DEBUG("[mqtt] disconnect\r\n");
						net4G_process_steps.communicateStep = 0;
						net4g_process_ret = PROCESS_NET_DISCONNECT;
					}
				}
			}
			else
			{
				if(get_pub_msg_flag())
				{
					net4G_process_steps.communicateStep = 2;
					NET4G_DEBUG("goto send cmd !\r\n");	
				}
				else
				{
					if(net4g_get_cell_info_flag==1)
					{
						module_4g_get_cell_info_process();  //获取小区信息
					}
				}
			}

			break;

		case 2: //pub
			sendRet = net4g_send_data_process();
			if(sendRet == 1)//数据发送成功，等待接收
			{
				reset_pub_msg_queue();
				net4G_process_steps.communicateStep = 1;
				NET4G_DEBUG("goto recv cmd !\r\n\r\n\r\n");		
			}
			else if(sendRet == 2)//发送数据设备断网
			{
				reset_pub_msg_queue();
				net4G_process_steps.communicateStep = 0;
				return PROCESS_NET_DISCONNECT;
			}

			break;
	}
	return net4g_process_ret;
}



PROCESS_STEP_RET_E module_4g_fw_download_process(void)
{
	static uint8_t process_work_step = OTA_STEP_INIT;
	PROCESS_STEP_RET_E ret = PROCESS_WAIT;

	switch(process_work_step)
	{
		case OTA_STEP_INIT:

			if(g_ota_para_t.type == UPDATE_4G_BOARD)
			{
				process_work_step = OTA_STEP_DOWNLOAD_END;
			}
			else
			{
				process_work_step = OTA_STEP_DOWNLOAD_DATA;
			}
											
			register_network_uart_cb(NULL);
			printf("ota init\r\n");
			break;

		case OTA_STEP_DOWNLOAD_DATA:						//下载固件包
			ret = OTA_4G_DownloadData();
			if(ret == PROCESS_DONE)
			{
				process_work_step = OTA_STEP_DOWNLOAD_END;
				printf("ota download finish\r\n");
			}
			else if(ret == PROCESS_ERROR)
			{
				process_work_step = OTA_STEP_INIT;
				printf("ota download fail\r\n");
			}
			else
			{
				// TODO: 升级超时处理
			}
			break;

		case OTA_STEP_DOWNLOAD_END:
			process_work_step = OTA_STEP_INIT;
			ret = PROCESS_DONE;
			break;
	}
	
	return ret;
}



PROCESS_STEP_RET_E module_4g_error_handle_process(void)
{
	static uint8_t work_steps = 0;
	static uint32_t wait_timeout = 0;
	PROCESS_STEP_RET_E ret = PROCESS_WAIT;

	switch(work_steps)
	{
		case 0: //4gmodule poweroff
			net4g_module_power_ctrl(0); 	
			work_steps = 1;
			NET4G_DEBUG("4G module powerOFF\r\n");
			break;

		case 1: //wait 2s
			wait_timeout++;

			if(net4g_process_route_retry_times < 5)			//路由重试次数
			{
				if(wait_timeout == 2000)//wait 2s
				{
					work_steps = 2;
				}
			}
			else if((net4g_process_route_retry_times >= 5)&&(net4g_process_route_retry_times < 15))
			{
				if(wait_timeout == 10000)//wait 10s
				{
					work_steps = 2;
				}
			}
			else if(net4g_process_route_retry_times >= 15)
			{
				if(wait_timeout >= 300000)//wait 5min
				{
					work_steps = 2;
				}
				
				if(wait_timeout % 10000 == 0)
				{
					NET4G_DEBUG("4G module delay powerON_%d\r\n", wait_timeout);
				}
			}
			break;
			
		case 2: //goto reconnect net
			NET4G_DEBUG("4G module delay powerON goto reconnect net\r\n");
			work_steps = 0;
			wait_timeout = 0;
			ret = PROCESS_DONE;
			break;
	}
	
	return ret;
}






extern netWork_process_step_t netWifi_process_steps;
PROCESS_STEP_RET_E net4G_work_process(void)
{
	PROCESS_STEP_RET_E ret = PROCESS_WAIT;
	PROCESS_STEP_RET_E net4G_work_ret = PROCESS_WAIT;

	static uint8_t net4G_wait_report_ret = PROCESS_WAIT;
	static uint16_t net4G_wait_report_delay = 0;
	
	switch(net4G_process_steps.netWork)
	{
		case NETWORK_WORK_STEP__INIT:
			net4g_module_power_ctrl(1); //4g module power on
#if (DEVICE_REBOOT_4G_CONTROL_TYPE == DEVICE_REBOOT_4G_NO_POWER_OFF)
			ret = device_reboot_module_4g_communication_process();		//网络通讯
			if(ret == PROCESS_NET_TYPE_WIFI)				//配置了wifi名称和密码或者开启了wifi开关
			{
				net4G_wait_report_ret = PROCESS_NET_TYPE_WIFI;
				net4G_wait_report_delay = 1;
			}
			else if(ret ==PROCESS_NET_DISCONNECT)			//网络断开连接
			{
				register_network_uart_cb(NULL);
				net4G_process_steps.netWork = NETWORK_WORK_STEP__NETCONFIG;	//断网重连，网络流程重新开始
				NET4G_DEBUG("net disconnect\r\n");
			}
			else if(ret == PROCESS_END)
			{
				slave_addr_config_event = 0;
				powerOn_flag = 1;
				register_network_uart_cb(&net4g_uart_recv_cb);
				Wt_NodeEvent_Ff((0xFF << 8) | NODE_BAT_POWER_CHECK_EVT);//battery power get
				set_qr_code_led(1);//二维码背光灯常亮
				net4G_process_steps.netWork = NETWORK_WORK_STEP__NORMAL;//
				net4G_process_steps.communicateStep = 1;//4G流程工步，通讯工步，恢复默认起始工步
				node_status_syn_enable_flag = 1;
				NET4G_DEBUG("normal communication\r\n");

			}
			else if(ret != PROCESS_WAIT)
			{
				register_network_uart_cb(NULL);
				net4G_process_steps.netWork = NETWORK_WORK_STEP__NETCONFIG;//配网工步
				NET4G_DEBUG("net error\r\n");
			}


			if(net4G_wait_report_delay>0)
			{
				app_delay_ms(500);
				net4G_wait_report_delay++;
				NETWIFI_DEBUG("net4G_wait_report_delay = %d\r\n", net4G_wait_report_delay);
				if(net4G_wait_report_delay>=9)//9  4S 延时等待配置上报完成
				{
					net4G_wait_report_delay = 0;

					if(net4G_wait_report_ret == PROCESS_NET_TYPE_WIFI)
					{
						network_uart_send(NET_CMD_MQTT_DISCONNECT, strlen(NET_CMD_MQTT_DISCONNECT)); //mqtt disconnect主动断开与服务器的MQTT连接
						app_delay_ms(500);
						net4g_module_power_ctrl(0); //4g module powerOff
						// if(netWorkStatus.wifi_state == 2)
						// {
						// 	app_mqtt_disconnect(); //mqtt disconnect
						// 	app_delay_ms(500);
						// }
						if(netWorkStatus.wifi_state != 0)
						{
							set_wifi_connect_status(0); 				//设置wifi为断开连接状态
						}
						netWifi_process_steps.communicateStep = 0;		//产生断网重连事件
						
						net4G_work_ret = PROCESS_NET_TYPE_WIFI;			//返回要切换的网络类型
						net4g_process_retry_times = 0;					//重试次数
						net4g_process_route_retry_times = 0;			//路由重试次数
						// forced_set_qr_code_led_to_blink(); 				//切换网络时，二维码灯光强行闪烁

						net4G_wait_report_ret = PROCESS_WAIT;
						device_reboot();
					}
				}
			}
#elif (DEVICE_REBOOT_4G_CONTROL_TYPE == DEVICE_REBOOT_4G_POWER_OFF)
			net4G_process_steps.netWork = NETWORK_WORK_STEP__NETCONFIG;//配网工步
#endif
			net4g_process_retry_times = 0;					//重试次数

			break;
			
			
		case NETWORK_WORK_STEP__NETCONFIG:   						// config net info and check net connect status
			ret = module_4g_config_para_process();					// 配网
			if(ret == PROCESS_DONE)
			{
				net4G_process_steps.netWork++;
				NET4G_DEBUG("active and route\r\n");
			}
			else if(ret != PROCESS_WAIT)
			{
				net4G_process_steps.netWork = NETWORK_WORK_STEP__ERROR;
			}
			break;
			

		case NETWORK_WORK_STEP__ACTIVE_ROUTE:						// device active or route to xd-cloud server
			ret = module_4g_active_route_process();					// 激活路由
			if(ret == PROCESS_DONE)
			{
				net4G_process_steps.netWork++;
				NET4G_DEBUG("cloud login\r\n");
				net4g_process_route_retry_times = 0;				//路由重试次数
			}
			else if(ret != PROCESS_WAIT)
			{
				NET4G_DEBUG("active and route fail\r\n");
				net4G_process_steps.netWork = NETWORK_WORK_STEP__ERROR;
				net4g_process_route_retry_times++;					//路由重试次数
				if(net4g_process_route_retry_times>=15)
				{
					net4g_process_route_retry_times = 15;			//路由重试次数
				}
			}
			break;

			
		case NETWORK_WORK_STEP__LOGIN:								// device sign in to xd-cloud server
			ret = module_4g_cloud_logIn_process();					// 登录
			if(ret == PROCESS_DONE)
			{
				net4g_reconnect_flag = 1;
				net4G_process_steps.netWork++;
				netWorkStatus.net_type=NET_TYPE_4G;
				NET4G_DEBUG("normal communication\r\n");
				net4g_process_route_retry_times = 0;				//路由重试次数
				net4g_get_cell_info_time = 1;
			}
			else if(ret != PROCESS_WAIT)
			{
				net4G_process_steps.netWork = NETWORK_WORK_STEP__ERROR;
				net4g_process_route_retry_times++;					//路由重试次数
				if(net4g_process_route_retry_times>=15)
				{
					net4g_process_route_retry_times = 15;			//路由重试次数
				}
			}
			break;
			

		case NETWORK_WORK_STEP__NORMAL:						//communication status after net connected
			ret = module_4g_communication_process();		//网络通讯
			if(ret == PROCESS_NET_TYPE_WIFI)				//配置了wifi名称和密码或者开启了wifi开关
			{
				// network_uart_send(NET_CMD_MQTT_DISCONNECT, strlen(NET_CMD_MQTT_DISCONNECT)); //mqtt disconnect主动断开与服务器的MQTT连接
				// app_delay_ms(500);
				// net4g_module_power_ctrl(0); //4g module powerOff

				net4G_wait_report_ret = PROCESS_NET_TYPE_WIFI;
				net4G_wait_report_delay = 1;
			}
			else if(ret ==PROCESS_NET_DISCONNECT)			//网络断开连接
			{
				register_network_uart_cb(NULL);
				net4G_process_steps.netWork = NETWORK_WORK_STEP__NETCONFIG;	//断网重连，网络流程重新开始
				net4g_module_power_ctrl(1); 								//4g module power on
							
				net4g_process_retry_times++;
				if(net4g_process_retry_times>=2)				//重试次数
				{
					if(netWorkStatus.wifi_state == 2)
					{
						app_mqtt_disconnect(); //mqtt disconnect
						app_delay_ms(500);
					}
					net4g_process_retry_times = 0;
					net4G_process_steps.netWork = NETWORK_WORK_STEP__INIT;	//断网重连，网络流程重新开始
#if (DEVICE_REBOOT_4G_CONTROL_TYPE == DEVICE_REBOOT_4G_NO_POWER_OFF)
					register_network_uart_cb(&net4g_uart_recv_cb);
#endif
					// set_network_task_step(NETWORK_TASK_STEP__NET_CHECK);
					device_reboot();
				}

				NET4G_DEBUG("net disconnect\r\n");
			}
			else if(ret != PROCESS_WAIT)
			{
				net4G_process_steps.netWork = NETWORK_WORK_STEP__ERROR;		//网络流程错误
				NET4G_DEBUG("net error\r\n");
			}

			if(net4G_wait_report_delay>0)
			{
				app_delay_ms(500);
				net4G_wait_report_delay++;
				NETWIFI_DEBUG("net4G_wait_report_delay = %d\r\n", net4G_wait_report_delay);
				if(net4G_wait_report_delay>=9)//9  4S 延时等待配置上报完成
				{
					net4G_wait_report_delay = 0;

					if(net4G_wait_report_ret == PROCESS_NET_TYPE_WIFI)
					{
						network_uart_send(NET_CMD_MQTT_DISCONNECT, strlen(NET_CMD_MQTT_DISCONNECT)); //mqtt disconnect主动断开与服务器的MQTT连接
						app_delay_ms(500);
						net4g_module_power_ctrl(0); //4g module powerOff
						// if(netWorkStatus.wifi_state == 2)
						// {
						// 	app_mqtt_disconnect(); //mqtt disconnect
						// 	app_delay_ms(500);
						// }
						if(netWorkStatus.wifi_state != 0)
						{
							set_wifi_connect_status(0); 				//设置wifi为断开连接状态
						}
						netWifi_process_steps.communicateStep = 0;		//产生断网重连事件
						
						net4G_work_ret = PROCESS_NET_TYPE_WIFI;			//返回要切换的网络类型
						net4g_process_retry_times = 0;					//重试次数
						net4g_process_route_retry_times = 0;			//路由重试次数
						// forced_set_qr_code_led_to_blink(); 				//切换网络时，二维码灯光强行闪烁

						net4G_wait_report_ret = PROCESS_WAIT;
						device_reboot();
					}
				}
			}

			break;
			

		case NETWORK_WORK_STEP__OTA:
			ret = module_4g_fw_download_process();			//升级
			if(ret == PROCESS_DONE)
			{
				NET4G_DEBUG("[ota] download finish\r\n");
				net4G_work_ret = PROCESS_FW_UPGRADE;
				
			}
			else if(ret != PROCESS_WAIT)
			{
				NET4G_DEBUG("[ota] download fail %d\r\n", ret);
				// device_reboot();
				ota_device_reboot();
			}
			break;
			
			
		case NETWORK_WORK_STEP__ERROR:							// error and other events detec
			ret = module_4g_error_handle_process();				// GPRS断电2S，重新联网
			if(ret != PROCESS_WAIT)
			{
				net4g_module_power_ctrl(1); 								//4g module power on
				register_network_uart_cb(NULL);
				net4G_process_steps.netWork = NETWORK_WORK_STEP__NETCONFIG;	//断网重连，网络流程重新开始
				
				net4g_process_retry_times++;
				if(net4g_process_retry_times>=1)				//重试次数
				{
					net4g_process_retry_times = 0;
					
					net4G_process_steps.netWork = NETWORK_WORK_STEP__INIT;	//断网重连，网络流程重新开始
#if (DEVICE_REBOOT_4G_CONTROL_TYPE == DEVICE_REBOOT_4G_NO_POWER_OFF)
					register_network_uart_cb(&net4g_uart_recv_cb);
#endif
					if((gSysInfo_struct.wifi.wifiConfigFlag == 0xAA)&&(gSysInfo_struct.wifi.wifiEnableFlag == 1))
					{
						device_reboot();
					}
					else
					{
						device_reboot();
					}
					
				}
				NET4G_DEBUG("net4G_process_steps.netWork %d \r\n", net4G_process_steps.netWork);
			}
			break;
			
			
		default:
#if (DEVICE_REBOOT_4G_CONTROL_TYPE == DEVICE_REBOOT_4G_NO_POWER_OFF)
			register_network_uart_cb(&net4g_uart_recv_cb);
#endif					
			net4G_process_steps.netWork = NETWORK_WORK_STEP__INIT;		//断网重连，网络流程重新开始
			break;
	}

	return net4G_work_ret;
}


