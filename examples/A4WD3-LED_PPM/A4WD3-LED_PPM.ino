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
 *  Description:  Example using the PPM library control the A4WD3-LED on the 3way switch & button. 
 *  CH5: Pos1 = LED OFF / Pos2 = LED Low beam / Pos3 = High beam
 *  CH6: Default = Red LEDs OFF / Pressed = Red LEDs ON
 */

// Include the PPM library
  #include "ppm.h"

// PPM channel layout (update for your situation)
  #define SWITCH3WAY_1  5
  #define BUTTON        6

// Loop interval time
  const long interval = 50;
  unsigned long previousMillis  = 0;

// Define the pins for the LEDs
  int whiteLEDpin = 3;
  int redLEDpin = 2;

void setup()
{
  // Set the type of pins
  pinMode(whiteLEDpin, OUTPUT);
  pinMode(redLEDpin, OUTPUT);

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
    short switch3way_1 = ppm.read_switch(SWITCH3WAY_1, 3);
    short button = ppm.read_switch(BUTTON);

    switch (switch3way_1) {
      case 0:
        analogWrite(whiteLEDpin, 0);
        break;
      case 1:
        analogWrite(whiteLEDpin, 50);
        break;
      case 2:
        analogWrite(whiteLEDpin, 255);
        break;
    }
     switch (button) {
      case 0:
        digitalWrite(redLEDpin, HIGH);
        break;
      case 1:
        digitalWrite(redLEDpin, LOW);
        break;
    } 
  }
}
