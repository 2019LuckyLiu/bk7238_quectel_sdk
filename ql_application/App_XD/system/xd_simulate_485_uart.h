#ifndef __XD_SIMULATE_485_UART_H__
#define __XD_SIMULATE_485_UART_H__

#include "uart_pub.h"
#include "ql_debug.h"
#include "ql_type.h"
#include "ql_gpio.h"
#include "ql_timer.h"
#include <stdint.h>

#define SIM_UART_BAUDRATE       9600
#define SIM_UART_BUFFER_SIZE    200
#define BIT_BUFFER_SIZE         SIM_UART_BUFFER_SIZE * 8  // 对应字节数的bit数
#define SIM_UART_TX_PIN         QL_GPIO22
#define SIM_UART_RX_PIN         QL_GPIO21
#define TX_TIMER                QL_TIMER_2
#define RX_TIMER                QL_TIMER_0

// 模拟串口配置
#define START_BIT               1
#define DATA_BITS               8
#define STOP_BIT                1

typedef enum {
    SIM_UART_OK = 0,
    SIM_UART_BUSY,
    SIM_UART_INVALID_PARAM,
    SIM_UART_RX_DISABLED
} sim_uart_status;

void sim_uart_init(void);
sim_uart_status sim_uart_send(const uint8_t *data, uint16_t length);
uint16_t sim_uart_receive(uint8_t *buffer, uint16_t max_length);
void sim_uart_enable_rx(uint8_t enable);
void sim_uart_abort_tx(void);

#endif /* __XD_SIMULATE_485_UART_H__ */    