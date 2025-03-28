#include "xd_simulate_485_uart.h"
#include "sys_config.h"
#include "ql_osi.h"

// 模拟串口接收缓冲区
static uint8_t rx_buffer[SIM_UART_BUFFER_SIZE];
static uint16_t rx_buffer_index = 0;

// 模拟串口控制结构体
typedef struct {
    uint8_t tx_buf[SIM_UART_BUFFER_SIZE];
    uint16_t tx_index;
    uint8_t tx_bit_pos;
    uint16_t tx_length;
    uint8_t tx_busy;
    uint8_t tx_enable;
    uint8_t rx_enable;
    uint8_t rx_bit_count;
    uint8_t rx_byte;
    uint8_t rx_start;
} sim_uart_ctrl_t;

static sim_uart_ctrl_t sim_ctrl = {
    .tx_busy = 0,
    .tx_enable = 0,
    .rx_enable = 1,
    .rx_bit_count = 0,
    .rx_byte = 0,
    .rx_start = 0
};

// 发送定时器回调函数
static void tx_timer_cb(uint8_t arg) {
    if (!sim_ctrl.tx_enable) return;

    uint32_t level;
    if (sim_ctrl.tx_bit_pos < START_BIT + DATA_BITS + STOP_BIT) {
        if (sim_ctrl.tx_bit_pos < START_BIT) {
            level = 0; // 起始位
        } else if (sim_ctrl.tx_bit_pos < START_BIT + DATA_BITS) {
            level = (sim_ctrl.tx_buf[sim_ctrl.tx_index] >> (sim_ctrl.tx_bit_pos - START_BIT)) & 0x01;
        } else {
            level = 1; // 停止位
        }
        ql_gpio_set_level(SIM_UART_TX_PIN, level ? QL_GPIO_LEVEL_HIGH : QL_GPIO_LEVEL_LOW);
        sim_ctrl.tx_bit_pos++;
    } else {
        sim_ctrl.tx_bit_pos = 0;
        sim_ctrl.tx_index++;
        if (sim_ctrl.tx_index >= sim_ctrl.tx_length) {
            sim_ctrl.tx_busy = 0;
            sim_ctrl.tx_enable = 0;
            ql_gpio_set_level(SIM_UART_TX_PIN, QL_GPIO_LEVEL_HIGH);
        }
    }
}

// 接收定时器回调函数，每半拍采样，同时解析数据
static void rx_timer_cb(uint8_t arg) {
    uint32_t level;
    uint8_t bit;

    if (!sim_ctrl.rx_enable) return;

    if (ql_gpio_get_level(SIM_UART_RX_PIN, &level) != QL_GPIO_SUCCESS) {
        return;
    }

    bit = (uint8_t)level;

    if (!sim_ctrl.rx_start) {
        if (bit == 0) { // 检测到起始位
            sim_ctrl.rx_start = 1;
            sim_ctrl.rx_bit_count = 0;
            sim_ctrl.rx_byte = 0;
        }
    } else {
        if (sim_ctrl.rx_bit_count < DATA_BITS) {
            sim_ctrl.rx_byte |= (bit << sim_ctrl.rx_bit_count);
            sim_ctrl.rx_bit_count++;
        } else if (sim_ctrl.rx_bit_count == DATA_BITS) {
            if (bit == 1) { // 检测到停止位
                if (rx_buffer_index < SIM_UART_BUFFER_SIZE) {
                    rx_buffer[rx_buffer_index++] = sim_ctrl.rx_byte;
                }
                sim_ctrl.rx_start = 0;
            } else {
                // 停止位错误，重新开始检测
                sim_ctrl.rx_start = 0;
            }
        }
    }
}

// 串口初始化函数
void sim_uart_init(void) {
    // 发送初始化
    ql_gpio_init(SIM_UART_TX_PIN, QL_GMODE_OUTPUT, QL_GPIO_LEVEL_HIGH);
    ql_TimerInit_us(TX_TIMER, 1000000 / (1 * SIM_UART_BAUDRATE), tx_timer_cb);
    ql_TimerStart(TX_TIMER);

    // 接收初始化
    ql_gpio_init(SIM_UART_RX_PIN, QL_GMODE_IN_PU, QL_GPIO_LEVEL_HIGH);
    ql_TimerInit_us(RX_TIMER, 1000000 / (1 * SIM_UART_BAUDRATE), rx_timer_cb);
    ql_TimerStart(RX_TIMER);
}

// 串口发送函数
sim_uart_status sim_uart_send(const uint8_t *data, uint16_t length) {
    if (sim_ctrl.tx_busy) return SIM_UART_BUSY;
    if (!data || length == 0 || length > SIM_UART_BUFFER_SIZE) return SIM_UART_INVALID_PARAM;

    uint16_t i;
    for (i = 0; i < length; i++) {
        sim_ctrl.tx_buf[i] = data[i];
    }
    sim_ctrl.tx_length = length;
    sim_ctrl.tx_index = 0;
    sim_ctrl.tx_bit_pos = 0;
    sim_ctrl.tx_busy = 1;
    sim_ctrl.tx_enable = 1;
    return SIM_UART_OK;
}

// 串口接收函数
uint16_t sim_uart_receive(uint8_t *buffer, uint16_t max_length) {
    uint16_t copied_length;
    if (!sim_ctrl.rx_enable) return 0;

    copied_length = (rx_buffer_index < max_length) ? rx_buffer_index : max_length;
    for (uint16_t i = 0; i < copied_length; i++) {
        buffer[i] = rx_buffer[i];
    }
    rx_buffer_index = 0;
    return copied_length;
}

// 使能接收函数
void sim_uart_enable_rx(uint8_t enable) {
    sim_ctrl.rx_enable = enable;
}

// 中止发送函数
void sim_uart_abort_tx(void) {
    sim_ctrl.tx_enable = 0;
    sim_ctrl.tx_busy = 0;
    ql_gpio_set_level(SIM_UART_TX_PIN, QL_GPIO_LEVEL_HIGH);
}    