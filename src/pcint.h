/* This code was ripped from Crazy People by Mike Cook April 2009
 * http://www.thebox.myzen.co.uk/Hardware/Crazy_People.html
 * 
 * My fixes:
 *   * PCattachInterrupt was not properly determining the callback function slot for Analog pins
 */

#include "pins_arduino.h"
/*
 * an extension to the interrupt support for arduino.
 * add pin change interrupts to the external interrupts, giving a way
 * for users to have interrupts drive off of any pin.
 * Refer to avr-gcc header files, arduino source and atmega datasheet.
 *
 * Theory: all IO pins on Atmega168 are covered by Pin Change Interrupts.
 * The PCINT corresponding to the pin must be enabled and masked, and
 * an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
 * must use some logic to actually implement a per-pin interrupt service.
 */

/* Pin to interrupt map:
 * D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
 * D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
 * A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
 */



/*
 * attach an interrupt to a specific pin using pin change interrupts.
 * First version only supports CHANGE mode.
 */
void PCattachInterrupt(uint8_t pin, void (*userFunc)(void), int mode);

void PCdetachInterrupt(uint8_t pin);
