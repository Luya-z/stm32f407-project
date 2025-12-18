//
// Created by huawei on 2025/12/16.
//
#include "pid.h"

//初始化
void pid_init(pid_t *pid,float kp,float ki,float kd,float max_out) {
    //传入参数存入结构体
    pid->Kd = kd;
    pid->Ki = ki;
    pid->Kp = kp;
    pid->out_max = max_out;
    //清零
    pid->integral = 0;
}

//计算
float pid_calc(pid_t *pid,float target,float feedback) {
    //获取输入
    pid->target=target;
    pid->feedback=feedback;
    //计算误差
    pid->err = target-feedback;
    //计算积分
    pid->integral += pid->err;
    //计算微分
    float derivative =  pid->err - pid->err_last;

    pid->output = pid->Kp * pid->err + pid->Ki * pid->integral + pid->Kd * derivative;
    //限幅
    if (pid->output > pid->out_max) {
        pid->output = pid->out_max;
    }else if (pid->output < -pid->out_max) {
        pid->output = -pid->out_max;
    }

    pid->err_last = pid->err;

    return pid->output;
}

