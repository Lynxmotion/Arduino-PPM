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
 */

#include <Arduino.h>
#include "ppm.h"
#include "pcint.h"
#include "avr/interrupt.h"

#define USE_PCINT

// our ISR for detecting pin change on PPM pin
ISR(__vector_ppm_interrupt);

PPM ppm;

PPM::PPM()
  : count_(0), pin_(0), invert_(false), cycle_(0)
{
}

void PPM::begin(int pin, bool invert)
{
  pinMode(pin, INPUT_PULLUP);
#ifdef USE_PCINT
  PCattachInterrupt(pin, __vector_ppm_interrupt, CHANGE);
#else
  attachInterrupt(digitalPinToInterrupt(pin), __vector_ppm_interrupt, CHANGE);
#endif
  count_ = 0;
  pin_ = pin;
  invert_ = invert;
}

void PPM::stop()
{
  if(pin_) {
#ifdef USE_PCINT
    PCdetachInterrupt(pin_);
#else
    detachInterrupt(digitalPinToInterrupt(pin_));
#endif
  }
}

short deadband(short v, short bandwidth) 
{
  return (bandwidth==0)
    ? v
    : (v < 0)
      ? (v > -bandwidth)
        ? 0
        : (v + bandwidth)
      : (v < bandwidth)
        ? 0
        : (v - bandwidth);
}

short PPM::read_throttle(short channel, short zero_point)
{
  short v = read_channel(channel);
  return (v < zero_point)
      ? 0
      : v;
}

short PPM::read_reversable_throttle(short channel, short zero_point,  short _deadband_)
{
  short v = read_channel(channel);
  return deadband(v - zero_point, _deadband_);
}

short PPM::read_midstick(short channel, short _deadband_)
{
  short v = read_channel(channel) - 1500;
  return deadband(v, _deadband_);
}

short PPM::read_switch(short channel, short bands)
{
  short v = read_channel(channel) - 1000;
  short bandwidth = 1000 / bands;
  short n = 0;
  while (v > bandwidth) {
    // this is likely faster than division
    // typically we'd have 3 subtractions max for a 3-way switch
    v -= bandwidth;
    n++;
  }
  return n;
}

short clamp(short min_, short v, short max_)
{
  return (v < min_)
    ? min_
    : (v > max_)
      ? max_
      : v;
}

void PPM::__vector_ppm_interrupt()
{
  static short channels[CHANNEL_AMOUNT * 2];  // declare 2 pages so we write to one while the client can read from the other
  static short* ch_write_base = channels;     // current page write position
  static byte i;
  static unsigned long int t_old;

  // read the pin, ignore falling transitions because the PPM time is the sum of 
  // the high and low pulse. I.e. the pulse initiates the channel timer and the
  // next pulse ends the timer and starts the next channel timer.
  bool pinHigh = digitalRead(ppm.pin())==HIGH;
  if(pinHigh == ppm.invert()) 
    return;

  // calculate the pulse width for this channel
  unsigned long int t = micros(); //store time value a when pin value falling/rising
  unsigned long int dt = t - t_old; //calculating time inbetween two peaks
  t_old = t;

  // detect last channels and go back to first
  if ((dt > DETECTION_SPACE) || (i > CHANNEL_AMOUNT)) {
    ppm.channels = ch_write_base;
    ppm.count_ = i;
    ppm.cycle_++;
    i = 0;

    // switch pages
    ch_write_base = (ch_write_base == channels)
      ? (channels + CHANNEL_AMOUNT)  // write to second page
      : channels;                    // write to first page
  } else
    ch_write_base[i++] = dt;
}
