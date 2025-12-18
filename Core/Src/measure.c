
#include "measure.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

uint16_t adc_buf[ADC_BUF_LEN];
static uint8_t measured = 0;

void Measure_Process(void) {

    if (measured)return;

    uint32_t rising_cnt = 0;
    uint32_t last_index = 0;
    uint32_t period_num = 0;

    //上升沿检测
    for (uint32_t i= 1;i<ADC_BUF_LEN;i++) {
        if (adc_buf[i-1]<2048 && adc_buf[i]>=2048) {
            if (rising_cnt>0) {
                period_num+=(i-last_index);
            }
            last_index=i;
            rising_cnt++;
        }
    }
    if (rising_cnt<2) return;

    //平均周期
    float avg_period = (float)period_num/(rising_cnt-1);

    //adc采样频率=adc时钟/采样周期（84.0采集+12.5转换）
    float adc_fs = 21000000.0f/(84.0f+12.5f);

    //信号频率(采样率/一个周期的点数）
    float freq = adc_fs/avg_period;

    //串口输出
    char msg[64];
    snprintf(msg, sizeof(msg), "Freq=%f Hz\r\n", freq);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);

    measured = 1;
}