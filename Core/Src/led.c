//
// Created by huawei on 2025/12/13.
//

#include "led.h"

#include "tim.h"

//LED模式
static LED_Mode_t LED_mode = LED_OFF;

//频率：闪烁快慢；呼吸快慢
static uint16_t LED_param = 0;

static uint32_t last_ms = 0;
static uint16_t breath_duty = 0;
//呼吸方向，=1变亮；=-1变暗
static int breath_dir = 1;

//设置LED模式、频率
void LED_Set_Mode(LED_Mode_t mode,uint16_t param) {
    LED_mode = mode;
    LED_param = param;
}

//LED状态机，主循环调用
void LED_Task() {

    uint32_t now=HAL_GetTick();

    switch (LED_mode) {
        case LED_OFF:
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
            break;

        case LED_ON:
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 999);
            break;

        case LED_BLINK:
            //间隔大于半个周期时改变状态
            if (now-last_ms > 500/LED_param) {
                last_ms = now;
                static int state = 0;
                state =!state;
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, state?999:0);
            }
            break;

        case LED_BREATHE:
            //每5ms改变亮度，视觉上连续
            if (now-last_ms > 5) {
                last_ms = now;
                breath_duty += breath_dir*(1+LED_param);
                if (breath_duty>=999) {
                    breath_duty = 999;
                    breath_dir = -1;
                }else if (breath_duty<=0) {
                    breath_duty = 0;
                    breath_dir = 1;
                }
            }
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, breath_duty);
    }
}
