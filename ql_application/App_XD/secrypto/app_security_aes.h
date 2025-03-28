//
// Created by YangYongbao on 2017/3/27.
//

#ifndef FIRMWARE_AES_H
#define FIRMWARE_AES_H

#include <stdint.h>

#ifndef CBC_EN
#define CBC_EN 1
#endif

#ifndef ECB_EN
#define ECB_EN 1
#endif

#if defined(ECB_EN) && ECB_EN

void AES128_ECB_encrypt(const uint8_t* input, const uint8_t* key, uint8_t *output);  //AES128����
void AES128_ECB_decrypt(const uint8_t* input, const uint8_t* key, uint8_t *output);  //AES128����

#endif // #if defined(ECB_EN) && ECB_EN

#if defined(CBC_EN) && CBC_EN

void AES128_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
void AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);

#endif // #if defined(CBC_EN) && CBC_EN

/**
 * aes加密
 * @param input 将要加密的字符串
 * @param key 加密密钥
 * @param output 加密后的字符串
 * @return
 */
int aes128_ecb_encrypt(uint8_t* input, uint16_t inputLen, uint8_t* key, uint8_t *output);
int aes128_ecb_encrypt1(uint8_t* input,uint8_t* key, uint8_t *output,uint16_t len);

/**
 * aes解密
 * @param input 将要解密的字符串
 * @param base_len 解密的数据长度
 * @param key 解密密钥
 * @param output 解密后的数据
 * @return 返回pkcs码
 */
uint8_t aes128_ecb_decrypt(const uint8_t* input, int base_len, const uint8_t* key, uint8_t *output); //AES128����

/**
 * aes加密，生成16进制输出
 * @param input 将要加密的字符串
 * @param key 加密密钥
 * @param flag 转换的结果标志（为0时，转换的结果为小写；非0时，转换的结果为大写）
 * @param output 加密后的十六进制字符串
 */
void aes128_ecb_encrypt_hex(uint8_t* input,uint8_t* key, int flag, char *output);

#endif //FIRMWARE_AES_H
