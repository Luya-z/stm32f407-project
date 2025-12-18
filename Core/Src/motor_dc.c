//
// Created by huawei on 2025/12/16.
//
#include "motor_dc.h"
#include "main.h"
#include "pid.h"

#define M3508_RAW_PER_ROUND 8192   // 单圈等效脉冲数
#define DEG_PER_ROUND       360    // 单圈度数

extern  TIM_HandleTypeDef htim4; //PWM
extern TIM_HandleTypeDef htim2;  //编码器

static pid_t pid_speed; //速度环PID
static pid_t pid_pos;   //位置环PID

//目标值变量，由外部命令（串口）设置
static int target_speed = 0;
static int target_pos = 0;

void motor_dc_init(void) {
    pid_init(&pid_speed,1.0f,0.01f,0.0f,1000);
    pid_init(&pid_pos,2.0f,0.0f,0.0f,300);
}

//外部接口
//设置目标速度
void motor_dc_set_vel(int rpm) {
    target_speed = rpm;
}

//设置目标角度
void motor_dc_set_pos(int pos) {
    target_pos = (float)pos * M3508_RAW_PER_ROUND / DEG_PER_ROUND;
}

void motor_dc_control_loop(void) {
    static int last_cnt = 0;
    //读取定时器2当前计数值
    int cnt = __HAL_TIM_GET_COUNTER(&htim2);
    //计算速度(脉冲差）
    int speed = cnt-last_cnt;
    last_cnt = cnt;
    //位置环
    float vel_ref = pid_calc(&pid_pos,target_pos,cnt);
    //速度环，输出最终驱动电压
    float pwm = pid_calc(&pid_speed,vel_ref,speed);
    //输出pwm控制电机
    if (pwm>=0) {
        __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,pwm);
    }
    else {
        __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,-pwm);
    }
}