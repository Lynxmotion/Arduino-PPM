/*
 *  Author:       Colin F. MacKenzie <nospam2@colinmackenzie.net>
 *  maintainers:  Eric Nantel
 *  Version:      1.0.1
 *  Licence:      LGPL-3.0 (GNU Lesser General Public License version 3)
 *  
 *  Desscription: Simple way to use a PPM Radio Controlled Receiver
 */

#pragma once

// up to 16 channels of PPM, but true number will be auto-detected
#define CHANNEL_AMOUNT 16

// 2.5ms of space will indicate a reset condition back to channel 1
#define DETECTION_SPACE 2500

class PPM {
  public:

    PPM();
  
    void begin(int pin, bool invert);

    void stop();

    inline short pin() const { return pin_; }
    
    inline short count() const { return count_; }

    inline short cycle() const { return cycle_; }

    inline short invert() const { return invert_; }

    inline short read_channel(short channel) {
      return (channel && channel <= CHANNEL_AMOUNT)
        ? channels[channel-1]
        : 1500;
    }

    short read_throttle(short channel, short zero_point=1200);
    
    short read_reversable_throttle(short channel, short zero_point=1200,  short _deadband_ = 0);
    
    short read_midstick(short channel, short _deadband_=0);
    
    short read_switch(short channel, short bands=2);
  
  protected:
    short* channels;
    byte count_;   // how many channels we read
    short pin_;
    bool invert_;
    short cycle_;  // how many full PPM reads we've done (useful to detect when new data is ready, but will wrap)

    static void __vector_ppm_interrupt();
};

// for now, only one PPM allowed until we can
// support the same ISR attached to multiple pins
extern PPM ppm;

short deadband(short v, short bandwidth);

short clamp(short min_, short v, short max_);
