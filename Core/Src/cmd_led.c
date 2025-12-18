//
// Created by huawei on 2025/12/13.
//

#include "cmd_led.h"
#include "led.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "usart.h"


static char cmd_buf[32];
static uint8_t idx = 0;

void Command_Parse(uint8_t c) {
    if (c == '\r' || c == '\n') {
        cmd_buf[idx] = '\0';
        idx = 0;
        if (strncmp(cmd_buf, "set_mode_on", 11) == 0) {
            LED_Set_Mode(LED_ON,0);
        }else if (strncmp(cmd_buf, "set_mode_off", 12) == 0) {
            LED_Set_Mode(LED_OFF,0);
        }else if (strncmp(cmd_buf, "set_mode_blink", 14) == 0) {
            int freq = atoi(&cmd_buf[15]);
            LED_Set_Mode(LED_BLINK,freq);
        }else if (strncmp(cmd_buf, "set_mode_breathe", 16) == 0) {
            int freq = atoi(&cmd_buf[17]);
            LED_Set_Mode(LED_BREATHE,freq);
        }
        //回显
        char msg[64];
        sprintf(msg,"OK:%s\r\n",cmd_buf);
        HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),10);

        return;//防止\n存进数组
    }

    if (idx<=sizeof(cmd_buf)) {
        cmd_buf[idx++] = c;
    }

}