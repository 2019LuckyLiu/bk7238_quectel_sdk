/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
//#include "include.h"
#include "ql_type.h"
#include "ql_debug.h"
#include "ql_fs.h"
#include "ql_ef.h"
#include "ql_gpio.h"
#include "ql_i2c.h"
#include "ql_pwm.h"
#include "ql_spi_flash.h"
#include "ql_timer.h"
#include "ql_watchdog.h"
#include "ql_uart.h"
#include "rtos_pub.h"
#include "uart_pub.h"
#include "mem_pub.h"
#include "ql_osi.h"

// XD init...
#include "app_task_slave_uart.h"


void quec_app_main(void * argv)
{
  ql_fs_init();
  ql_ef_init();
  sim_uart_init();
  
  ql_gpio_demo_thread_creat();
  app_slave_uart_task();
}

  