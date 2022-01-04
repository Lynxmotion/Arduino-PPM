/*
 *  Author:     Eric Nantel / DiaLFonZo
 *  Version:    1.0.0
 *  Licence:    LGPL-3.0 (GNU Lesser General Public License version 3)
 *  
 *  Description:  Example using the PPM library to read the first 4 channels and update 4 servo outputs
 *  Use: Load the code and connect the Receiver & Servo to the corresponding pins
 */

// Include the PPM library
#include "ppm.h"

// Include the Servo library
#include <Servo.h>

// create servo object to control a servo
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;

// PPM channel layout (update for your situation)
#define THROTTLE        3
#define ROLL            1
#define PITCH           2
#define YAW             4

// Loop interval time
const long interval = 50;
unsigned long previousMillis = 0;

void setup()
{
  // Start the serial port to display data 
  Serial.begin(115200);

  // Start the PPM function on PIN A0
  ppm.begin(A0, false);

  // attaches the servo on pins to the servo objects
  servo_1.attach(3);
  servo_2.attach(5); 
  servo_3.attach(6); 
  servo_4.attach(9);   
}

void loop()
{
  // Interval at which the PPM is updated
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    short throttle  =   ppm.read_channel(THROTTLE);
    short roll      =   ppm.read_channel(ROLL);
    short pitch     =   ppm.read_channel(PITCH);
    short yaw       =   ppm.read_channel(YAW);
    servo_1.writeMicroseconds(throttle);
    servo_2.writeMicroseconds(roll);
    servo_3.writeMicroseconds(pitch);
    servo_4.writeMicroseconds(yaw);
  }
}
