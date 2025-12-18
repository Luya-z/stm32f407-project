//
// Created by huawei on 2025/12/16.
//

#ifndef STM32F407_CMD_H
#define STM32F407_CMD_H

/**
 * @brief  串口命令模块初始化
 */
void cmd_init(void);

/**
 * @brief  开启串口中断接收
 */
void cmd_uart_receive_it(void);

/**
 * @brief  串口命令处理函数
 * @note   对已接收完成的命令字符串进行解析，
 *         根据命令内容调用对应的电机控制接口，
 *         实现速度或位置的在线控制。
 */
void cmd_process(void);

#endif //STM32F407_CMD_H