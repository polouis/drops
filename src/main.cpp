#include <Audio.h>
#include <SPI.h>

#include "AudioRouting.hpp"
#include <OnOffButton.hpp>
#include <Encoder.hpp>
#include <PinMapping.hpp>

drop::OnOffButton button;
drop::Encoder encoderButton;

drop::OnOffButton buttonDelay;
drop::Encoder encoderDelay;

uint16_t delayLength = 300;

float knobMix = 0.0f;
elapsedMillis elapsed = 0;

void buttonHandler(bool on)
{
  if (on)
  {
   	granular.Enable();
  }
  else
  {
    granular.Disable();
  }
}

void buttonDelayHandler(bool on)
{
  if (on)
  {
    Serial.println("Effect delay on");
  }
  else
  {
    Serial.println("Effect delay off");
  }
}

void encoderButtonHandler(int8_t direction)
{
  delayLength += direction * 10;
  if (delayLength < 50) {
    delayLength = 50;
  }
  Serial.print("Delay length = ");
  Serial.print(delayLength);
  Serial.println();
 	granular.Delay(delayLength);
}

void encoderDelayHandler(int8_t direction)
{
  Serial.print("Encoder delay direction = ");
  Serial.print(direction);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  AudioMemory(10);

  button.Init(PIN_BUTTON_GRANULAR);
  button.RegisterHandler(buttonHandler);
  encoderButton.Init(PIN_ENCODER_GRANULAR_1, PIN_ENCODER_GRANULAR_2);
  encoderButton.RegisterHandler(encoderButtonHandler);

  encoderDelay.Init(PIN_ENCODER_DELAY_1, PIN_ENCODER_DELAY_2);
  encoderDelay.RegisterHandler(encoderDelayHandler);
  buttonDelay.Init(PIN_BUTTON_DELAY);
  buttonDelay.RegisterHandler(buttonDelayHandler);

  granular.Disable();

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);
}

void loop() {
  buttonDelay.Update();
  encoderDelay.Update();

  button.Update();
  encoderButton.Update();

  // read knobs, scale to 0-1.0 numbers
  //knobDelayLength = (float)analogRead(A2) / 1023.0;
  knobMix = (float)analogRead(A3) / 1023.0;

  if (elapsed > 1000) {
    elapsed = 0;
    //Serial.print("Balance = "); Serial.println(knobMix);
    //Serial.print("Audio CPU usage = "); Serial.println(AudioProcessorUsage());
  }

  mixer1.gain(0, 1.0 - knobMix);
  mixer1.gain(1, knobMix);
}
