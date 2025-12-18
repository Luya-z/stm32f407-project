//
// Created by huawei on 2025/12/16.
//

#include "cmd.h"

#include <stdlib.h>
#include <string.h>

#include "motor_dc.h"
#include "motor_can.h"
#include "main.h"
#include "cmd_led.h"

extern UART_HandleTypeDef huart1;

static char rx_buf[64];//接收缓冲区
static uint8_t rx_ch;//存储刚接收到的字符
static int idx;//索引变量

uint8_t rx_data;

//初始化缓冲区
void cmd_init(void) {
    memset(rx_buf, 0, sizeof(rx_buf));
}

//开启串口中断接收
void cmd_uart_receive_it(void) {
    HAL_UART_Receive_IT(&huart1, &rx_ch, 1);
}

//串口接收回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        if (rx_ch == '\n') {
            rx_buf[idx] = '\0';
            idx = 0;
        }
        else {
            rx_buf[idx++] = rx_ch;
        }
        cmd_uart_receive_it();

        Command_Parse(rx_data);
        HAL_UART_Receive_IT(&huart1, &rx_data,1);
    }
}

void cmd_process(void) {
    if (rx_buf[0]==0) return;
    //切割字符串
    char *cmd = strtok(rx_buf, " ");
    char *arg = strtok(NULL, " ");
    //匹配命令
    if (!strcmp(cmd, "set_vel")) {
        if (arg==NULL) return;
        int v = atoi(arg);
        //调整电机驱动函数
        motor_can_set_vel(v);
        motor_dc_set_vel(v);
    }else if (!strcmp(cmd, "pos")) {
        if (arg==NULL) return;
        int pos = atoi(arg);
        motor_can_set_pos(pos);
        motor_dc_set_pos(pos);
    }
    //清空缓冲区
    memset(rx_buf, 0, sizeof(rx_buf));
}