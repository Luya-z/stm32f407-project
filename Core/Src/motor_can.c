//
// Created by huawei on 2025/12/16.
//

#include "motor_can.h"
#include "pid.h"
#include "main.h"

#define M3508_RAW_PER_ROUND 8192   // 单圈等效脉冲数
#define DEG_PER_ROUND       360    // 单圈度数

extern CAN_HandleTypeDef hcan1;

static pid_t pid_speed; //速度环PID
static pid_t pid_pos;   //位置环PID

static int target_speed = 0;
static int target_pos = 0;
static int motor_speed = 0; //实际转速
static int motor_angle = 0; //实际角度
static uint16_t last_encoder = 0;

//CAN电机多圈绝对角度计算
static void motor_can_update_angle(uint16_t encoder) {
    //当前机械角度与上次的差值
    int diff = encoder-last_encoder;
    if (diff>4096) diff-= 8192;       //电机正转溢出
    else if (diff<-4096) diff+= 8192; //电机反转溢出

    motor_angle+=diff;
    last_encoder=encoder;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx; //CAN接收帧头结构体
    uint8_t data[8]; //存储电机反馈的原始数组
    HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&rx,data);
    if (rx.StdId == 0x201) {
        uint16_t encoder = (data[0]<<8)|data[1]; //电机单圈机械角度
        int16_t speed = (int16_t)((data[2]<<8)|data[3]);
        motor_speed = speed;
        motor_can_update_angle(encoder);
    }
}

//外部接口
void motor_can_init(void) {
    pid_init(&pid_speed,5.0f,0.05f,0.0f,16000);
    pid_init(&pid_pos,3.0f,0.0f,0.0f,1000);
}

//外部接口
//设置目标速度
void motor_can_set_vel(int rpm) {
    target_speed = rpm;
}

//设置目标角度
void motor_can_set_pos(int pos) {
    target_pos = (float)pos * M3508_RAW_PER_ROUND / DEG_PER_ROUND;
}

void motor_can_control_loop(void) {
    //位置环,输出应达到的速度
    float vel_ref = pid_calc(&pid_pos,target_pos,motor_angle);
    //速度环，输出需要的电流大小
    int current = pid_calc(&pid_speed,vel_ref,motor_speed);
    //can控制指令发送
    CAN_TxHeaderTypeDef tx;
    uint8_t data[8]={0};

    tx.StdId = 0x200;
    tx.IDE = CAN_ID_STD;
    tx.RTR = CAN_RTR_DATA;
    tx.DLC = 8;
    //将32位 int current拆分成2个8字节
    data[0] = current >> 8;//高8位
    data[1] = current;     //低8位

    uint32_t mailbox;
    HAL_CAN_AddTxMessage(&hcan1, &tx, data, &mailbox);
}

void CAN_Filter_Init(void)
{
    CAN_FilterTypeDef filter;

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;

    filter.FilterIdHigh = 0x0000;
    filter.FilterIdLow  = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow  = 0x0000;

    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation = ENABLE;

    HAL_CAN_ConfigFilter(&hcan1, &filter);
}