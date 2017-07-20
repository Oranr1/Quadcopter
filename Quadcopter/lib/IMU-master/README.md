# IMU using MPU6050

This library uses both the accelerometer and the gyro from the MPU6050 and calculates the angle using a Complementary Filter.

You need to install the MPU6050 library before using this library.
The MPU6050 library can be found here: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

## How to use?
You need to include the wire library and the IMU library
```c
#include "IMU.h"
#include "Wire.h"
```
Create an IMU object at pass the constructor the offsets of the gyro - X offset, Y offset, Z offset
```c
IMU imu(0, 0, 68);
```

Inside the setup function initialilze the wire library and the IMU variable:
```c
void setup() {
    Wire.begin();    
    imu.initialize();
}
```

To calculate the angle call the function calculate_angle from the loop function.
```c
void loop()
{
    imu.calculate_angle();
}
```

In order to use the X, Y and Z angles use the imu.angles property:
```c
imu.angles.x
imu.angles.y
imu.angles.z
```
