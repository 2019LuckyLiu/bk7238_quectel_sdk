#include "xd_simulate_485_uart.h"
#include "sys_config.h"
#include "ql_osi.h"
#include "ql_type.h"
#include "ql_gpio.h"
#include "ql_timer.h"

#define TASK_STACK_SIZE     4096
#define SEND_INTERVAL_MS    5000

static void uart_task(void *param)
{
    uint32_t last_send = 0;
    uint8_t init_flag = 1;
    const uint8_t test_data[] = "UART Test\n";
    uint8_t rx_buf[SIM_UART_BUFFER_SIZE];
    uint16_t recv_len;

    // sim_uart_init();

    while (1) {
        uint32_t now = rtos_get_time();

        // 定时发送
        if (init_flag || (now - last_send >= SEND_INTERVAL_MS)) {
            if (SIM_UART_OK == sim_uart_send(test_data, sizeof(test_data) - 1)) {
                last_send = now;
                init_flag = 0;
            }
        }

        // 持续接收处理
        recv_len = sim_uart_receive(rx_buf, sizeof(rx_buf));
        if (recv_len > 0) {
            // 回显接收数据
            sim_uart_send(rx_buf, recv_len);
        }

        ql_rtos_task_sleep_ms(1000);
    }
}

void app_slave_uart_task(void)
{
    ql_task_t task;
    ql_rtos_task_create(&task, TASK_STACK_SIZE, THD_EXTENDED_APP_PRIORITY,
                        "uart_task", uart_task, NULL);
}    