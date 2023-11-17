#ifndef MPU6050_h
#define MPU6050_h

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDR 0x68

#define MPU_DEVICE_ID_REG 0x75
#define MPU_PWR_MGMT1_REG 0x6B
#define MPU_PWR_MGMT2_REG 0x6C
#define MPU_SELF_TESTX_REG 0x0D
#define MPU_SELF_TESTY_REG 0x0E
#define MPU_SELF_TESTZ_REG 0x0F
#define MPU_SELF_TESTA_REG 0x10
#define MPU_SAMPLE_RATE_REG 0x19
#define MPU_CFG_REG 0x1A
#define MPU_GYRO_CFG_REG 0x1B
#define MPU_ACCEL_CFG_REG 0x1C
#define MPU_MOTION_DET_REG 0x1F
#define MPU_FIFO_EN_REG 0x23
#define MPU_I2CMST_CTRL_REG 0x24
#define MPU_I2CSLV0_ADDR_REG 0x25
#define MPU_I2CSLV0_REG 0x26
#define MPU_I2CSLV0_CTRL_REG 0x27
#define MPU_I2CSLV1_ADDR_REG 0x28
#define MPU_I2CSLV1_REG 0x29
#define MPU_I2CSLV1_CTRL_REG 0x2A
#define MPU_I2CSLV2_ADDR_REG 0x2B
#define MPU_I2CSLV2_REG 0x2C
#define MPU_I2CSLV2_CTRL_REG 0x2D
#define MPU_I2CSLV3_ADDR_REG 0x2E
#define MPU_I2CSLV3_REG 0x2F
#define MPU_I2CSLV3_CTRL_REG 0x30
#define MPU_I2CSLV4_ADDR_REG 0x31
#define MPU_I2CSLV4_REG 0x32
#define MPU_I2CSLV4_DO_REG 0x33
#define MPU_I2CSLV4_CTRL_REG 0x34
#define MPU_I2CSLV4_DI_REG 0x35

#define MPU_I2CMST_STA_REG 0x36
#define MPU_INTBP_CFG_REG 0x37
#define MPU_INT_EN_REG 0x38
#define MPU_INT_STA_REG 0x3A

#define MPU_ACCEL_XOUTH_REG 0x3B
#define MPU_ACCEL_XOUTL_REG 0x3C
#define MPU_ACCEL_YOUTH_REG 0x3D
#define MPU_ACCEL_YOUTL_REG 0x3E
#define MPU_ACCEL_ZOUTH_REG 0x3F
#define MPU_ACCEL_ZOUTL_REG 0x40

#define MPU_TEMP_OUTH_REG 0x41
#define MPU_TEMP_OUTL_REG 0x42

#define MPU_GYRO_XOUTH_REG 0x43
#define MPU_GYRO_XOUTL_REG 0x44
#define MPU_GYRO_YOUTH_REG 0x45
#define MPU_GYRO_YOUTL_REG 0x46
#define MPU_GYRO_ZOUTH_REG 0x47
#define MPU_GYRO_ZOUTL_REG 0x48

#define MPU_I2CSLV0_DO_REG 0x63
#define MPU_I2CSLV1_DO_REG 0x64
#define MPU_I2CSLV2_DO_REG 0x65
#define MPU_I2CSLV3_DO_REG 0x66

#define MPU_I2CMST_DELAY_REG 0x67
#define MPU_SIGPATH_RST_REG 0x68
#define MPU_MDETECT_CTRL_REG 0x69
#define MPU_USER_CTRL_REG 0x6A
#define MPU_PWR_MGMT1_REG 0x6B
#define MPU_PWR_MGMT2_REG 0x6C
#define MPU_FIFO_CNTH_REG 0x72
#define MPU_FIFO_CNTL_REG 0x73
#define MPU_FIFO_RW_REG 0x74
#define MPU_DEVICE_ID_REG 0x75

#define MPU_ADDR_ADDR 0x68

// class MPU6050

class MPU6050
{
public:
    MPU6050();
    MPU6050(int sclPin, int sdaPin);                                   // 构造
    void MPU_Get_Gyroscope(int16_t *gx, int16_t *gy, int16_t *gz);     // 取加速度
    void MPU_Get_Accelerometer(int16_t *ax, int16_t *ay, int16_t *az); // 取角速度
    void MPU_Get_Angle(float *anglex, float *angley, float *anglez);
    void MPU_Set_Gyro_Fsr(int fsr);
    void MPU_Set_Accel_Fsr(int fsr);
    void MPU_Set_LPF(int lpf);
    void MPU_Set_Rate(int rate);

private:
    void Write_Mpu6050_REG(int reg, int dat);
    int Read_Mpu6050_REG(int reg);
    void Read_Mpu6050_Len(int reg, int len, char *buffer);
    int MPU_Init();
    float LPF_Angle(float angle, float previous);
    float anglex_filt;
    float angley_filt;
    float anglez_filt;
};

MPU6050::MPU6050(int sclPin, int sdaPin)
{
    delay(1000);
    Serial.print("scl");
    if (Wire.begin(sdaPin, sclPin, 100000) == false)
    {
        while (1)
        {
            Serial.print("初始化失败_1");
            delay(1000);
        }
    }
    else
    {
        if (MPU_Init() == false)
            while (1)
            {
                Serial.print("初始化失败_2");
                delay(1000);
            }
        else
            Serial.print("初始化成功");
    }
}

void MPU6050::Write_Mpu6050_REG(int reg, int dat)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(reg);
    Wire.write(dat);
    Wire.endTransmission();
}
int MPU6050::Read_Mpu6050_REG(int reg)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 1);
    return Wire.read();
}
void MPU6050::Read_Mpu6050_Len(int reg, int len, char *buffer)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, len);
    while (Wire.available())
    {
        *buffer++ = Wire.read();
    }
}
void MPU6050::MPU_Set_Gyro_Fsr(int fsr)
{
    switch (fsr)
    {
    case 250:
        fsr = 0x00;
        break;
    case 500:
        fsr = 0x08;
        break;
    case 1000:
        fsr = 0x10;
        break;
    case 2000:
        fsr = 0x18;
        break;
    default:
        fsr = 0x00;
        break;
    }
    Write_Mpu6050_REG(MPU_GYRO_CFG_REG, fsr << 3);
}
void MPU6050::MPU_Set_Accel_Fsr(int fsr)
{
    switch (fsr)
    {
    case 2:
        fsr = 0x00;
        break;
    case 4:
        fsr = 0x08;
        break;
    case 8:
        fsr = 0x10;
        break;
    case 16:
        fsr = 0x18;
        break;
    default:
        fsr = 0x00;
        break;
    }
    Write_Mpu6050_REG(MPU_ACCEL_CFG_REG, fsr << 3);
}
void MPU6050::MPU_Set_LPF(int lpf)
{
    switch (lpf)
    {
    case 188:
        lpf = 0x01;
        break;
    case 98:
        lpf = 0x02;
        break;
    case 42:
        lpf = 0x03;
        break;
    case 20:
        lpf = 0x04;
        break;
    case 10:
        lpf = 0x05;
        break;
    case 5:
        lpf = 0x06;
        break;
    default:
        lpf = 0x01;
        break;
    }
    Write_Mpu6050_REG(MPU_CFG_REG, lpf << 3);
}
void MPU6050::MPU_Set_Rate(int rate)
{
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    int data = int(1000 / rate - 1);
    Write_Mpu6050_REG(MPU_SAMPLE_RATE_REG, data);
    MPU_Set_LPF(rate / 2);
}
int MPU6050::MPU_Init()
{
    Write_Mpu6050_REG(MPU_PWR_MGMT1_REG, 0x80);
    delay(100);
    Write_Mpu6050_REG(MPU_PWR_MGMT1_REG, 0x00);
    MPU_Set_Gyro_Fsr(3);
    MPU_Set_Accel_Fsr(0);
    MPU_Set_Rate(50);
    Write_Mpu6050_REG(MPU_INT_EN_REG, 0x00);
    Write_Mpu6050_REG(MPU_USER_CTRL_REG, 0x00);
    Write_Mpu6050_REG(MPU_FIFO_EN_REG, 0x00);
    Write_Mpu6050_REG(MPU_INTBP_CFG_REG, 0x80);
    int res = Read_Mpu6050_REG(MPU_DEVICE_ID_REG);
    if (res == 68)
    {
        Write_Mpu6050_REG(MPU_PWR_MGMT1_REG, 0x01);
        Write_Mpu6050_REG(MPU_PWR_MGMT2_REG, 0x00);
        MPU_Set_Rate(50);
    }
    else
        return 1;
    return 0;
}
void MPU6050::MPU_Get_Gyroscope(int16_t *gx, int16_t *gy, int16_t *gz)
{
    char buf[6] = {0};
    Read_Mpu6050_Len(MPU_GYRO_XOUTH_REG, 6, buf);
    *gx = (buf[0] << 8) | buf[1];
    *gy = (buf[2] << 8) | buf[3];
    *gz = (buf[4] << 8) | buf[5];
}
void MPU6050::MPU_Get_Accelerometer(int16_t *ax, int16_t *ay, int16_t *az)
{
    char buf[6] = {0};
    Read_Mpu6050_Len(MPU_ACCEL_XOUTH_REG, 6, buf);
    *ax = (buf[0] << 8) | buf[1];
    *ay = (buf[2] << 8) | buf[3];
    *az = (buf[4] << 8) | buf[5];
}
void MPU6050::MPU_Get_Angle(float *anglex, float *angley, float *anglez)
{
    int16_t ax, ay, az;
    MPU_Get_Accelerometer(&ax, &ay, &az);
    int16_t gx, gy, gz;
    MPU_Get_Gyroscope(&gx, &gy, &gz);
    // Calculate pitch angle
    *anglex = atan2(ay, az)*180/3.141592;

    // Calculate roll angle
    *angley = atan2(-ax, az)*180/3.141592;

    // Calculate yaw angle
    *anglez = atan2(gz, gy)*180/3.141592;
    // Filter the angle values
    *anglex = LPF_Angle(*anglex, this->anglex_filt);
    *angley = LPF_Angle(*angley, this->angley_filt);
    *anglez = LPF_Angle(*anglez, this->anglez_filt);

    // Update the filtered angle values
    this->anglex_filt = *anglex;
    this->angley_filt = *angley;
    this->anglez_filt = *anglez;
}

float MPU6050::LPF_Angle(float angle, float previous)
{
    // Calculate the angle difference between the current angle and the previous angle
    float angle_diff = angle - previous;

    // Calculate the filtered angle by applying a linear low pass filter
    float filtered_angle = previous + angle_diff * 0.5;

    // Return the filtered angle
    return filtered_angle;
}
#endif