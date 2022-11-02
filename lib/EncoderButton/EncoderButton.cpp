#include <Bounce.h>
#include <Wire.h>

#include <EncoderButton.hpp>

namespace drop
{
  void EncoderButton::Init(uint8_t pinEncoder1, uint8_t pinEncoder2)
  {
    this->rotEnc = new Encoder(pinEncoder1, pinEncoder2);
  }

  void EncoderButton::Update()
  {
    // specific to encoder
    static uint32_t timeCapture = 0;
    if ((millis() - timeCapture) < this->debounce)
    {
        return;
    }
    timeCapture = millis();
    long newPosition;
    newPosition = this->rotEnc->read() / 4;
    if (newPosition != this->position) {
      this->position = newPosition;
      if (this->eventHandler != NULL)
      {
        this->eventHandler(newPosition);
      }
    }
    // if a character is sent from the serial monitor,
    // reset both back to zero.
    if (Serial.available()) {
        Serial.read();
        Serial.println("Reset both knobs to zero");
        this->rotEnc->write(0);
    }
  }

  void EncoderButton::RegisterHandler(EncoderButtonEventHandler eventHandler) {
    this->eventHandler = eventHandler;
  }
}
