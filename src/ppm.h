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
