#include <Bounce.h>
#include <Encoder.h>
#include <Wire.h>

#include <InterfaceControl.hpp>

namespace drop
{
  typedef void (*EncoderButtonEventHandler)(int8_t direction);

  class Encoder : public InterfaceControl
  {
    private:
      EncoderButtonEventHandler eventHandler;
      ::Encoder* rotEnc;
      int16_t position = INT16_MAX;
      uint32_t debounce = 4;
      elapsedMillis elapsed = 0;

    public:
      void Init(uint8_t pinEncoder1, uint8_t pinEncoder2);
      void Update() override;
      void RegisterHandler(EncoderButtonEventHandler eventHandler);
  };
}
