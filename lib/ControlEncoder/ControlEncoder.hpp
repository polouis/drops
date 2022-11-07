#ifndef __CONTROL_ENCODER__
#define __CONTROL_ENCODER__

#include <Bounce.h>
#include <Encoder.h>
#include <Wire.h>

#include <InterfaceControl.hpp>

namespace drop
{
  typedef void (*ControlEncoderEventHandler)(int8_t direction);

  class ControlEncoder : public InterfaceControl
  {
    private:
      ControlEncoderEventHandler eventHandler;
      ::Encoder* rotEnc;
      int16_t position = INT16_MAX;
      uint32_t debounce = 4;
      elapsedMillis elapsed = 0;

    public:
      void Init(uint8_t pinEncoder1, uint8_t pinEncoder2);
      void Update() override;
      void RegisterHandler(ControlEncoderEventHandler eventHandler);
  };
}

#endif // __CONTROL_ENCODER__