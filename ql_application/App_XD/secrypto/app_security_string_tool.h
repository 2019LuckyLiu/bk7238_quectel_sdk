//
// Created by YangYongbao on 2017/4/22.
//

#ifndef FIRMWARE_STRING_TOOL_H
#define FIRMWARE_STRING_TOOL_H

#include "stdint.h"

/**
 * ascii值转为十六进制
 * @param ascii 要转换的ascii数据
 * @param ascii_len 要转换的ascii数据长度
 * @param flag 转换的结果标志（为0时，转换的结果为小写；非0时，转换的结果为大写）
 * @param result 转换的十六进制结果
 */
void ascii2hex(uint8_t *ascii, uint16_t ascii_len, int flag, uint8_t *result);

/***********************************************************************
* 功能：将一个十六进制字节串转换成ASCII码表示的十六进制字符串
* 输入参数：pHex----十六进制数字节串首地址
*           pAscii---转换后ASCII码表示的十六进制字符串首地址
*           nLen---要转换的十六进制数长度（字节数）
* 注：转换后的结果全部是大写ASCII码表示的十六进制数
************************************************************************/
void HexToAscii(unsigned char *pHex, unsigned char *pAscii, int nLen);

#endif //FIRMWARE_STRING_TOOL_H
