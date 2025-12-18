//
// Created by huawei on 2025/12/18.
//

#ifndef STM32F407_LED_H
#define STM32F407_LED_H

#include "main.h"

/**
 * @brief LED 工作模式枚举
 * - LED_OFF     : LED 熄灭
 * - LED_ON      : LED 常亮
 * - LED_BLINK   : LED 按指定频率闪烁
 * - LED_BREATHE : LED 呼吸灯
 */
typedef  enum {
    LED_OFF=0,
    LED_ON,
    LED_BLINK,
    LED_BREATHE
}LED_Mode_t;

/**
 * @brief 设置 LED 工作模式及参数
 * @param mode  LED 工作模式，取值为 LED_Mode_t 枚举类型
 * @param param LED 模式参数，用于控制闪烁或呼吸效果的快慢
 */
void LED_Set_Mode(LED_Mode_t mode,uint16_t param);

/**
 * @brief LED 状态机处理函数
 */
void LED_Task();

#endif //STM32F407_LED_H