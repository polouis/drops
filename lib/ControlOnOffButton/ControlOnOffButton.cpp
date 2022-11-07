#include <Bounce.h>
#include <Wire.h>

#include <ControlOnOffButton.hpp>

namespace drop
{
  void ControlOnOffButton::Init(uint8_t pinNumber)
  {
    this->button = new Bounce(pinNumber, 15);
    pinMode(pinNumber, INPUT_PULLUP);
  }

  void ControlOnOffButton::Update()
  {
    this->button->update();

    if (this->button->fallingEdge()) {
      this->on = !this->on;
      this->eventHandler(this->on);
    }
  }

  void ControlOnOffButton::RegisterHandler(ControlOnOffButtonEventHandler eventHandler) {
    this->eventHandler = eventHandler;
  }
}
