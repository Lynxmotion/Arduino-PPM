/*  Copyright (c) 2022 Lynxmotion & Colin MacKenzie
 *  
 *  author: Colin F. MacKenzie <nospam2@colinmackenzie.net>
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
 *
 *  Description:  Example using the PPM library to acquire the values and plot them
 *  Use: To display the data, open the Arduino Serial Plotter
 */

// Include the PPM library
#include "ppm.h"

// PPM channel layout (update for your situation)
#define THROTTLE        3
#define ROLL            1
#define PITCH           2
#define YAW             4
#define SWITCH3WAY_1    5
#define BUTTON          6
#define SWITCH3WAY_2    7     // trim-pot for left/right motor mix  (face trim)
#define POT             8     // trim-pot on the (front left edge trim)

// Loop interval time
const long interval = 50;
unsigned long previousMillis = 0;

void setup()
{
  // Start the serial port to display data 
  Serial.begin(115200);

  // Start the PPM function on PIN A0
  ppm.begin(A0, false);
}

void loop()
{
  // Interval at which the PPM is updated
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Acquiring all the channels values
    short throttle      =   ppm.read_channel(THROTTLE);
    short roll          =   ppm.read_channel(ROLL);
    short pitch         =   ppm.read_channel(PITCH);
    short yaw           =   ppm.read_channel(YAW);
    short switch3way_1  =   ppm.read_channel(SWITCH3WAY_1);
    short button        =   ppm.read_channel(BUTTON);
    short switch3way_2  =   ppm.read_channel(SWITCH3WAY_2);
    short pot           =   ppm.read_channel(POT);

    // Print the values for the Arduino Serial Plotter
    Serial.print("Throttle:");        Serial.print(throttle);       Serial.print(" ");
    Serial.print("Roll:");            Serial.print(roll);           Serial.print(" ");
    Serial.print("Pitch:");           Serial.print(pitch);          Serial.print(" ");
    Serial.print("Yaw:");             Serial.print(yaw);            Serial.print(" ");
    Serial.print("Switch_3way_1:");   Serial.print(switch3way_1);   Serial.print(" ");
    Serial.print("Button:");          Serial.print(button);         Serial.print(" ");
    Serial.print("Switch_3way_2:");   Serial.print(switch3way_2);   Serial.print(" ");
    Serial.print("Pot:");             Serial.print(pot);            Serial.print(" ");
    Serial.println();
  }
}
