#ifndef __DURATION_HPP__
#define __DURATION_HPP__

#include <AudioStream.h>

namespace drop
{
  class Duration
  {
  public: 
    Duration()
    {
      this->milliseconds = 0;
      this->samples = 0;
    }

    uint32_t GetMilliseconds()
    {
      return this->milliseconds;
    }

    uint32_t GetSamples()
    {
      return this->samples;
    }

    void SetMilliseconds(uint32_t milliseconds)
    {
      this->milliseconds = milliseconds;
      this->samples = milliseconds * AUDIO_SAMPLE_RATE_EXACT / 1000.0f;
    }


  private:
    uint32_t milliseconds;
    uint32_t samples;
  };
}

#endif // __DURATION_HPP__
