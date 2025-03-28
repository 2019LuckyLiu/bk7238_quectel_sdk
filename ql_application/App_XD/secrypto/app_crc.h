#ifndef __APP_CRC_H__
#define __APP_CRC_H__

#include "app_config.h"

unsigned int crc32(char *buff, int len, unsigned int  crc);
unsigned int crc32_com(unsigned char *buff, unsigned int size);
uint16_t CRC16_Modbus_check(uint8_t* data, uint16_t len, uint8_t flag);

#endif


