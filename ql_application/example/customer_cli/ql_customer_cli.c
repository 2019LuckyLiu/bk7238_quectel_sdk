#include "wlan_cli_pub.h"
#include "ql_debug.h"
#include "ql_uart.h"

#if CFG_DEBUG_USE_UART2
int uart_print_port = QL_UART_PORT_2;
#else
int uart_print_port = QL_UART_PORT_1;
#endif
int uart_print_baud = QL_UART_BAUD_921600;

/* Allows customers to add custom cli commands for production testing                       
*/
                                                                         				 
#define CUS_VERSION "FCM242DAAR01A01.bin" //Replace with the actual version
void customer_version(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	ql_dbg_log("get customer version \n");
	ql_dbg_log(CUS_VERSION);
}


const struct cli_command customer_clis[] =
{
	{"get_version", "get version", customer_version},
													//add command
};


int customer_cli_register(void)
{
	return cli_register_commands(customer_clis, sizeof(customer_clis) / sizeof(struct cli_command));
}

