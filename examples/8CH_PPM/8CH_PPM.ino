/*
 *  Author:     Colin MacKenzie / guru-florida
 *  Version:    1.0.0
 *  Licence:    LGPL-3.0 (GNU Lesser General Public License version 3)
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
