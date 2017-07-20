#include "IMU.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <string.h>

IMU::IMU(int gyro_x_offset, int gyro_y_offset, int gyro_z_offset)
{
	this->gyro_x_offset = gyro_x_offset;
	this->gyro_y_offset = gyro_y_offset;
	this->gyro_z_offset = gyro_z_offset;
}

void IMU::initialize(void)
{
    this->accelgyro.initialize();
    this->accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_500);
    this->accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
    this->accelgyro.setDLPFMode(MPU6050_DLPF_BW_5);
    memset(&this->angles, 0, sizeof(this->angles));
    this->timer = micros();
    this->dt = 0;
}

void IMU::calculate_angle()
{
    int16_t ax, ay, az, gx, gy, gz;
    this->accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    double acc_angle_x_sq = sq((double)ay);
    double acc_angle_y_sq = sq((double)ax);
    double acc_angle_z_sq = sq((double)az);
    float acc_y_angle = (atan2(ax, sqrt(acc_angle_x_sq + acc_angle_z_sq)) + PI) * RAD_TO_DEG -180;
    float acc_x_angle = (atan2(ay, sqrt(acc_angle_z_sq + acc_angle_y_sq)) + PI) * RAD_TO_DEG -180;

    float x_angle = -acc_x_angle;
    float y_angle = acc_y_angle;
    float x_velocity = (-gx - this->gyro_x_offset) / 65.5;
    float y_velocity = (-gy - this->gyro_y_offset) / 65.5;
    float z_velocity = (-gz - this->gyro_z_offset) / 65.5;


    //Complementary filter
    this->dt = ((double)(micros() - this->timer) / 1000000);

    this->angles.x = 0.99*(this->angles.x + x_velocity * this->dt) + 0.01 * x_angle;
    this->angles.y = 0.99*(this->angles.y + y_velocity * this->dt) + 0.01 * y_angle;
    this->angles.z = this->angles.z + z_velocity * this->dt;

    this->timer = micros();
}
