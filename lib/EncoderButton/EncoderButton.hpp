#include <Bounce.h>
#include <Encoder.h>
#include <Wire.h>

#include <InterfaceControl.hpp>

namespace drop
{
  typedef void (*EncoderButtonEventHandler)(long position);

  class EncoderButton : public InterfaceControl
  {
    private:
      EncoderButtonEventHandler eventHandler;
      Encoder* rotEnc;
      long position = -999;
      uint32_t debounce = 4;

    public :
      void Init(uint8_t pinEncoder1, uint8_t pinEncoder2);
      void Update() override;
      void RegisterHandler(EncoderButtonEventHandler eventHandler);
  };
}
