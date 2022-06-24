/*  Copyright (c) 2022 Lynxmotion
 *  
 *  author: Eric Nantel
 *  maintainers: Eric Nantel
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
 */

// Include the PPM library
#include "ppm.h"

// Include the LSS library
#include <LSS.h>

// PPM channel layout (update for your situation)
#define THROTTLE    3
#define RUDDER      4
#define PITCH       2
#define ROLL        1

// Variable to store the PPM values per channel
short throttle;
short rudder;
short pitch;
short roll;

// consider channel at 0 until at least this much is sensed
const int INPUT_DEADBAND = 10;             

// Loop interval time
const long interval = 10;
unsigned long previousMillis = 0;

#define LSS_BAUD  (LSS_DefaultBaud)
// Choose the proper serial port for your platform

#define LSS_SERIAL  (Serial)  // ex: Many Arduino boards
//#define LSS_SERIAL  (Serial1) // ex: Teensy

// Create the LSS objects and assign them IDs
LSS myLSS_1 = LSS(1);
LSS myLSS_2 = LSS(2);
LSS myLSS_3 = LSS(3);
LSS myLSS_4 = LSS(4);

void setup() {
  // Initialize the LSS bus
  LSS::initBus(LSS_SERIAL, LSS_BAUD);

  Serial.write("#254RESET\r");   
  delay(2000);  // wait 2 sec for reboot

  // Start the PPM function on PIN A3
  ppm.begin(A3, false);

  // Enable Motion Profile
  Serial.println("#254EM0");  // EM0 = motion profile disabled

  // LED Color
  Serial.println("#254LED3"); // 3 = blue
}

void loop() {
  // Interval at which the PPM & LSS Output are updated
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    PPMupdate();
    LSSupdate();
  }
}

// Updating all the PPM values to each channel Variable
void PPMupdate()
{ 
  throttle    =   map(ppm.read_midstick(THROTTLE, INPUT_DEADBAND), -500, 500, -900, 900);
  roll        =   map(ppm.read_midstick(ROLL, INPUT_DEADBAND), -500, 500, -900, 900);
  pitch       =   map(ppm.read_midstick(PITCH, INPUT_DEADBAND), -500, 500, -900, 900);
  rudder      =   map(ppm.read_midstick(RUDDER, INPUT_DEADBAND), -500, 500, -900, 900);
}

// Updating the positions of each LSS's
void LSSupdate()
{ 
  myLSS_1.move(throttle);
  myLSS_2.move(roll);
  myLSS_3.move(pitch);
  myLSS_4.move(rudder);
}
