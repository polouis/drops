#ifndef __CONTROL_ON_OFF_BUTTON_HPP__
#define __CONTROL_ON_OFF_BUTTON_HPP__

#include <Bounce.h>
#include <Wire.h>

#include <InterfaceControl.hpp>

namespace drop
{
  typedef void (*ControlOnOffButtonEventHandler)(bool);

  class ControlOnOffButton : public InterfaceControl
  {
    private:
      ::Bounce* button;
      bool on = false;
      ControlOnOffButtonEventHandler eventHandler;

    public :
      void Init(uint8_t pin);
      void Update() override;
      void RegisterHandler(ControlOnOffButtonEventHandler eventHandler);
  };
}

#endif // __CONTROL_ON_OFF_BUTTON_HPP__
