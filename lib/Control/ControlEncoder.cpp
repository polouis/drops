#include <Bounce.h>
#include <Wire.h>

#include <ControlEncoder.hpp>

namespace drop
{
  void ControlEncoder::Init(uint8_t pinEncoder1, uint8_t pinEncoder2)
  {
    this->rotEnc = new ::Encoder(pinEncoder1, pinEncoder2);
  }

  void ControlEncoder::Update()
  {
    if (this->elapsed < this->debounce)
    {
      return;
    }
    this->elapsed = 0;
    long newPosition = this->rotEnc->read() / 4;
    if (newPosition != this->position) {
      uint16_t oldPosition = this->position;
      this->position = newPosition;
      if (this->eventHandler != NULL && oldPosition != INT16_MAX)
      {
        this->eventHandler(newPosition - oldPosition);
      }
    }
  }

  void ControlEncoder::RegisterHandler(ControlEncoderEventHandler eventHandler) {
    this->eventHandler = eventHandler;
  }
}
