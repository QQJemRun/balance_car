#include <Arduino.h>
#include <Wire.h>
#include "MPU6050.h"
// #include "no_brush_motor.h"
#include "udp_controler.h"
#include <string.h>

String ssid = "BalanceCar";
String password = "12345678";

// int pin_2 = 22;
// no_brush_motor motor_l();
// no_brush_motor motor_r();
MPU6050 *mpu;
udp_controler *udp_control;

void setup()
{
    // pinMode(pin_2, OUTPUT);
    Serial.begin(115200);
    mpu = new MPU6050(16,17);
    udp_control = new udp_controler(ssid, password);
}

void loop()
{
    // mpu6050测试
    // int16_t ax, ay, az;
    // int16_t gx, gy, gz;
    // float pitch, roll, yaw;
    // mpu->MPU_Get_Accelerometer(&ax, &ay, &az);
    // mpu->MPU_Get_Gyroscope(&gx, &gy, &gz);
    // mpu->MPU_Get_Angle(&pitch, &roll, &yaw);
    // Serial.print("加速度: ");
    // Serial.print(ax);
    // Serial.print(", ");
    // Serial.print(ay);
    // Serial.print(", ");
    // Serial.print(az);
    // Serial.print(" 角速度: ");
    // Serial.print(gx);
    // Serial.print(", ");
    // Serial.print(gy);
    // Serial.print(", ");
    // Serial.print(gz);
    // Serial.print(" 姿态角: ");
    // Serial.print(pitch);
    // Serial.print(", ");
    // Serial.print(roll);
    // Serial.print(", ");
    // Serial.println(yaw);pitch*180/3.1415, roll*180/3.1415, yaw*180/3.1415
    // Serial.println();
    float pitch, roll, yaw;
    mpu->MPU_Get_Angle(&pitch, &roll, &yaw);
    Serial.printf("姿态: pit = %f\t rol = %f\t yaw = %f\n", pitch, roll, yaw);
    char buf[1024];
    sprintf(buf,"姿态: pit = %f\t rol = %f\t yaw = %f\n", pitch, roll, yaw);
    String pac(buf);
    udp_control->send_data(pac,IPAddress(192,168,4,2),5555);
    // udp_control->send_data("")
    // digitalWrite(pin_2, HIGH);
    // delay(500);
    // digitalWrite(pin_2, LOW);
    delay(500);
}