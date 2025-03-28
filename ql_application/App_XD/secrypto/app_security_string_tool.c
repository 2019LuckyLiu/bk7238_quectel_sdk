//
// Created by YangYongbao on 2017/4/22.
//

#include "app_security_string_tool.h"

unsigned char hex_str_lower[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
unsigned char hex_str_upper[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void ascii2hex(uint8_t *ascii, uint16_t ascii_len, int flag, uint8_t *result)
{
    uint16_t hex_index = 0;

    for (uint16_t index=0; index<ascii_len; index++) {
        // 十进制转16进制字符串
        if (flag == 0) {
            result[hex_index++] = hex_str_lower[(((int) ascii[index] & 0xf0) >> 4)]; // 高8位
            result[hex_index++] = hex_str_lower[((int) ascii[index] & 0x0f)]; // 低8位
        } else {
            result[hex_index++] = hex_str_upper[(((int) ascii[index] & 0xf0) >> 4)]; // 高8位
            result[hex_index++] = hex_str_upper[((int) ascii[index] & 0x0f)]; // 低8位
        }
    }
}



/***********************************************************************
* 功能：将一个十六进制字节串转换成ASCII码表示的十六进制字符串
* 输入参数：pHex----十六进制数字节串首地址
*           pAscii---转换后ASCII码表示的十六进制字符串首地址
*           nLen---要转换的十六进制数长度（字节数）
* 注：转换后的结果全部是大写ASCII码表示的十六进制数
************************************************************************/
void HexToAscii(unsigned char *pHex, unsigned char *pAscii, int nLen)
{
    unsigned char Nibble[2];
    unsigned int i,j;
    for (i = 0; i < nLen; i++)
    {
        Nibble[0] = (pHex[i] & 0xF0) >> 4;
        Nibble[1] = pHex[i] & 0x0F;
        for (j = 0; j < 2; j++)
        {
            if (Nibble[j] < 10)
            {            
                Nibble[j] += 0x30;
            }
            else
            {
                if (Nibble[j] < 16)
                {
                    Nibble[j] = Nibble[j] - 10 + 'A';
                }
            }
            *pAscii++ = Nibble[j];
        }
    }
}
