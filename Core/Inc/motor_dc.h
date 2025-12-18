//
// Created by huawei on 2025/12/16.
//

#ifndef STM32F407_MOTOR_DC_H
#define STM32F407_MOTOR_DC_H

/**
 * @brief 有刷直流电机转动方向枚举
 */
typedef enum {
    DC_STOP = 0,//停止
    DC_FORWARD, //正转
    DC_REVERSE  //反转
}dc_dir_t;

/**
 * @brief  初始化直流电机控制模块
 * @retval 无
 */
void motor_dc_init(void) ;

/**
 * @brief  设置直流电机目标转速
 * @param  rpm  目标转速
 * @retval 无
 */
void motor_dc_set_vel(int rpm);

/**
 * @brief  设置直流电机目标转动位置
 * @param  pos  目标位置
 * @retval 无
 */
void motor_dc_set_pos(int pos);

/**
 * @brief  直流电机控制周期函数
 * @note   在定时器中断或主循环中执行
 * @retval 无
 */
void motor_dc_control_loop(void);

#endif //STM32F407_MOTOR_DC_H