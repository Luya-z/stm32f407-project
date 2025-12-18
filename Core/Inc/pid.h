//
// Created by huawei on 2025/12/16.
//

#ifndef STM32F407_PID_H
#define STM32F407_PID_H

/**
 * @brief 位置式 PID 控制器结构体
 */
typedef struct {
    //PID参数
    float Kp;
    float Ki;
    float Kd;
    //输入量
    float target;
    float feedback;
    //内部状态
    float err;
    float err_last;
    float integral;
    //输出限制
    float output;
    float out_max;
}pid_t;

/**
 * @brief  初始化PID控制器参数（位置式）
 * @param  pid      PID结构体指针
 * @param  kp       比例系数
 * @param  ki       积分系数
 * @param  kd       微分系数
 * @param  max_out  输出最大限幅
 * @retval 无
 */
void pid_init(pid_t *pid,float kp,float ki,float kd,float max_out);

/**
 * @brief  执行一次PID计算
 * @param  pid       PID结构体指针
 * @param  target    目标值
 * @param  feedback  当前反馈值
 * @retval PID 输出值
 */
float pid_calc(pid_t *pid,float target,float feedback);

#endif //STM32F407_PID_H