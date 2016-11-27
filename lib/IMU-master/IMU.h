#ifndef IMU_EXPORTS_H
#define IMU_EXPORTS_H
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

typedef struct {
    float x;
    float y;
    float z;
} IMU__3_axis_angles_t;

class IMU {
    public:
        IMU__3_axis_angles_t angles;

        IMU(int gyro_x_offset, int gyro_y_offset, int gyro_z_offset);
	void initialize(void);
        void calculate_angle();
    private:
        MPU6050 accelgyro;
        uint32_t timer;
        double dt;
        int gyro_x_offset;
        int gyro_y_offset;
        int gyro_z_offset;
};


#endif /* IMU_EXPORTS_H */
