#ifndef __ENVELOPE_TRIANGLE_HPP__
#define __ENVELOPE_TRIANGLE_HPP__

#include <stdint.h>
#include <InterfaceEnvelope.hpp>

namespace drop
{
  class EnvelopeTriangle: public InterfaceEnvelope
  {
  public:
    virtual String GetName() override
    {
      return "triangle";
    }

    virtual int16_t Compute(int16_t inputValue, uint16_t position, uint16_t length) override
    {
      uint16_t halfPeriod = length / 2;
      if (position <= halfPeriod)
      {
        return position * inputValue / halfPeriod;
      }
      else
      {
        return (length - position) * inputValue / halfPeriod;
      }
    }
  };
}

#endif // __ENVELOPE_TRIANGLE_HPP__
