#ifndef __INTERFACE_ENVELOPE_HPP__
#define __INTERFACE_ENVELOPE_HPP__

#include <stdint.h>
#include <String.h>

namespace drop
{
  class InterfaceEnvelope
  {
  public:
    virtual String GetName() = 0;
    virtual int16_t Compute(int16_t inputValue, uint16_t position, uint16_t length) = 0;
  };
}

#endif // __INTERFACE_ENVELOPE_HPP__
