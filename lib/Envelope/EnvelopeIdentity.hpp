#ifndef __ENVELOPE_IDENTITY_HPP__
#define __ENVELOPE_IDENTITY_HPP__

#include <stdint.h>
#include <InterfaceEnvelope.hpp>

namespace drop
{
  class EnvelopeIdentity: public InterfaceEnvelope
  {
  public:
    virtual String GetName() override
    {
      return "identity";
    }

    virtual int16_t Compute(int16_t inputValue, uint16_t position, uint16_t length) override
    {
      return inputValue;
    }
  };
}

#endif // __ENVELOPE_IDENTITY_HPP__
