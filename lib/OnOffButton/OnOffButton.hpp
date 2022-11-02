#include <Bounce.h>
#include <Wire.h>

#include <InterfaceControl.hpp>

namespace drop
{
  typedef void (*OnOffButtonEventHandler)(bool);

  class OnOffButton : public InterfaceControl
  {
    private:
      ::Bounce* button;
      bool on = false;
      OnOffButtonEventHandler eventHandler;

    public :
      void Init(uint8_t pin);
      void Update() override;
      void RegisterHandler(OnOffButtonEventHandler eventHandler);
  };
}
