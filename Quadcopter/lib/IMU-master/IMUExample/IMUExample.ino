#include "IMU.h"
#include "Wire.h"
IMU imu(0, 0, 68);

void setup() {
    Serial.begin(115200);
    Wire.begin();    
    imu.initialize();
}


void loop() {
    imu.calculate_angle();
    Serial.print(imu.angles.x);
    Serial.print(",");
    Serial.print(imu.angles.y);
    Serial.print(",");
    Serial.println(imu.angles.z);
}
