//
// Created by huawei on 2025/12/16.
//

#ifndef STM32F407_MOTOR_CAN_H
#define STM32F407_MOTOR_CAN_H

/**
 * @brief  CAN 无刷电机控制模块初始化
 * @note   初始化电机控制所需的 PID 参数，并完成相关变量的初始配置。
 */
void motor_can_init(void);

/**
 * @brief  设置 CAN 无刷电机的目标转速
 * @param  rpm  目标转速
 */
void motor_can_set_vel(int rpm);

/**
 * @brief  设置 CAN 无刷电机的目标角度
 * @param  pos  目标角度，单位：度（°）
 * @note   函数内部将角度值转换为编码器对应的脉冲数，
 */
void motor_can_set_pos(int pos);

/**
 * @brief  CAN 无刷电机控制周期函数
 * @note   函数内部完成位置环和速度环 PID 计算，
 *         并通过 CAN 总线发送电机控制指令。
 */
void motor_can_control_loop(void);

/**
 * @brief  CAN 接收过滤器初始化
 * @note   配置 CAN 过滤器参数，并使能 FIFO0 接收，
 *         用于接收电机反馈帧数据。
 */
void CAN_Filter_Init(void);

#endif //STM32F407_MOTOR_CAN_H