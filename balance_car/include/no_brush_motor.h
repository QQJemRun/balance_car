#ifndef NO_BRUSH_MOTOR_H
#define NO_BRUSH_MOTOR_H

#include "Arduino.h"

float p_l                   = 0.001;        // l线性参数
float i_l                   = 0.001;        // l积分参数
float d_l                   = 0.001;        // l微分参数
float p_r                   = 0.001;        // r线性参数
float i_r                   = 0.001;        // r积分参数
float d_r                   = 0.001;        // r微分参数
float error_l               = 0.0;          // l本次误差
float last_error_l          = 0.0;          // l上次误差
float integral_l            = 0.0;          // l的积分值
float error_r               = 0.0;          // r本次误差
float last_error_r          = 0.0;          // r上次误差
float integral_r            = 0.0;          // r的积分值
int time_in                 = 20;           //  测速间隔

class no_brush_motor                        // motor基类
{
public:
    no_brush_motor(uint8_t pwm_pin, uint8_t direction_pin);
    ~no_brush_motor();
    uint8_t pwm_pin;                        // pwm管脚
    uint8_t direction_pin;                  // 方向管脚
    uint8_t pwm;
    void init();
    void set_direction(uint8_t direction);
    void set_pwm(uint8_t pwm);
};

no_brush_motor::no_brush_motor(uint8_t pwm_pin, uint8_t direction_pin)
{
    this->pwm_pin = pwm_pin;
    this->direction_pin = direction_pin;
    this->init();
}

no_brush_motor::~no_brush_motor()
{
}

void no_brush_motor::init()
{
    pinMode(this->pwm_pin, OUTPUT);
    pinMode(this->direction_pin, OUTPUT);
    this->set_pwm(0);
    this->set_direction(0);
}

void no_brush_motor::set_direction(uint8_t direction)
{
    if (direction == 0)
        digitalWrite(this->direction_pin, LOW);
    else
    {
        digitalWrite(this->direction_pin, HIGH);
    }
}

void no_brush_motor::set_pwm(uint8_t pwm)
{
    analogWriteFrequency(5000);
    analogWrite(this->pwm_pin, pwm);
    this->pwm = pwm;
}

int speed_motor_l = 0; // l脉冲次数
int speed_motor_r = 0; // r脉冲次数

void speed_count_l() // l中断计数
{
    speed_motor_l++;
}

void speed_count_r() // r中断计数
{
    speed_motor_r++;
}

// 左马达

class no_brush_motor_l : public no_brush_motor
{
private:
    uint8_t speed_pin;
    void init();

public:
    no_brush_motor_l(uint8_t pwm_pin, uint8_t direction_pin, uint8_t speed_pin) : no_brush_motor(pwm_pin, direction_pin)
    {
        this->speed_pin = speed_pin;
        this->init();
    }
    ~no_brush_motor_l();
    int get_speed_l();
    int speed_pid_l();
    void set_speed_l(int speed);
};

no_brush_motor_l::~no_brush_motor_l()
{
}
void no_brush_motor_l::init()
{
    pinMode(this->speed_pin, INPUT);
}
int no_brush_motor_l::get_speed_l()
{
    speed_motor_l = 0;
    attachInterrupt(digitalPinToInterrupt(this->speed_pin), speed_count_l, HIGH);
    long start_time = millis();
    while (millis() - start_time < time_in);
    detachInterrupt(digitalPinToInterrupt(this->speed_pin));
    return speed_motor_r;
}

void no_brush_motor_l::set_speed_l(int speed)
{
    error_l = abs(speed - this->get_speed_l());
    while (error_l > 10)
    {
        int speed_pwm = speed_pid_l();
        this->set_pwm(speed_pwm);
        last_error_l = error_l;
        error_l = abs(speed - this->get_speed_l());
    }
    integral_l = 0.0;
}

int no_brush_motor_l::speed_pid_l()
{
    float p1 = 0.0, p2 = 0.0, p3 = 0.0, p4 = 0.0;
    p1 = p_l * error_l;
    p2 = d_l * 1000 * (last_error_l - error_l) / time_in;
    integral_l = integral_l + error_l * time_in / 1000;
    p3 = i_l * (integral_l + error_l);
    p4 = 1 / (1 + exp(-(p1 + p2 + p3)));
    return p4;
}

// 右马达

class no_brush_motor_r : public no_brush_motor
{
private:
    uint8_t speed_pin;
    void init();

public:
    no_brush_motor_r(uint8_t pwm_pin, uint8_t direction_pin, uint8_t speed_pin) : no_brush_motor(pwm_pin, direction_pin)
    {
        this->speed_pin = speed_pin;
        this->init();
    }
    ~no_brush_motor_r();
    int get_speed_r();
    int speed_pid_r();
    void set_speed_r(int speed);
};
no_brush_motor_r::~no_brush_motor_r()
{
}
void no_brush_motor_r::init()
{
    pinMode(this->speed_pin, INPUT);
}
int no_brush_motor_r::get_speed_r()
{
    speed_motor_r = 0;
    attachInterrupt(digitalPinToInterrupt(this->speed_pin), speed_count_r, HIGH);
    long start_time = millis();
    while (millis() - start_time < time_in);
    detachInterrupt(digitalPinToInterrupt(this->speed_pin));
    return speed_motor_r;
}
void no_brush_motor_r::set_speed_r(int speed)
{
    error_r = abs(speed - this->get_speed_r());
    while (error_r > 10)
    {
        int speed_pwm = speed_pid_r();
        this->set_pwm(speed_pwm);
        last_error_r = error_r;
        error_r = abs(speed - this->get_speed_r());
    }
    integral_r = 0.0;
}
int no_brush_motor_r::speed_pid_r()
{
    float p1 = 0.0, p2 = 0.0, p3 = 0.0, p4 = 0.0;
    p1 = p_r * error_r;
    p2 = d_r * 1000 * (last_error_r - error_r) / time_in;
    integral_r = integral_r + error_r * time_in / 1000;
    p3 = i_r * (integral_r + error_r);
    p4 = 1 / (1 + exp(-(p1 + p2 + p3)));
    return p4;
}
#endif