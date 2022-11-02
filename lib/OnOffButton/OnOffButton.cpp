#include <Bounce.h>
#include <Wire.h>

#include <OnOffButton.hpp>

namespace drop
{
  void OnOffButton::Init(uint8_t pinNumber)
  {
    this->button = new Bounce(pinNumber, 15);
    pinMode(pinNumber, INPUT_PULLUP);
  }

  void OnOffButton::Update()
  {
    this->button->update();

    if (this->button->fallingEdge()) {
      this->on = !this->on;
      this->eventHandler(this->on);
    }
  }

  void OnOffButton::RegisterHandler(OnOffButtonEventHandler eventHandler) {
    this->eventHandler = eventHandler;
  }
}
