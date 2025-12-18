
#ifndef STM32F407_CMD_LED_H
#define STM32F407_CMD_LED_H

#include "main.h"

/**
 * @brief LED 命令解析函数
 *
 * 当检测到换行符（\\r 或 \\n）时，认为一条命令接收完成，
 * 并根据命令内容调用 LED 控制接口函数，实现 LED 模式切换。
 *
 * 本模块通常与串口接收中断配合使用，
 * 在 UART 接收完成回调函数中逐字节调用该函数。
 *
 * @param c 通过串口接收到的单个字符
 */
void Command_Parse(uint8_t c);

#endif //STM32F407_CMD_LED_H