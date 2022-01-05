/*  Copyright (c) 2022 Lynxmotion & Colin MacKenzie
 *  
 *  author: Eric Nantel
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *  
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *  
 *    - Neither the name of the Lynxmotion nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
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
