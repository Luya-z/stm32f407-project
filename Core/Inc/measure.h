//
// Created by huawei on 2025/12/17.
//

#ifndef STM32F407_MEASURE_H
#define STM32F407_MEASURE_H

#include "main.h"
#define ADC_BUF_LEN 1024
extern uint16_t adc_buf[ADC_BUF_LEN];

/**
 * @brief 信号测量与频率计算处理函数
 *
 * 该函数对 ADC 采样缓冲区中的数据进行处理，
 * 通过检测信号的上升沿，计算信号的平均周期，进而得到被测信号的频率。
 *
 * 本函数通常在主循环或定时任务中被周期性调用
 */
void Measure_Process(void);

#endif //STM32F407_MEASURE_H