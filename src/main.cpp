// Granular Effect Example - Pitch shift or freeze sound
//
// This example is meant to be used with 3 buttons (pin 0,
// 1, 2) and 2 knobs (pins 16/A2, 17/A3), which are present
// on the audio tutorial kit.
//   https://www.pjrc.com/store/audio_tutorial_kit.html
//
// Data files to put on your SD card can be downloaded here:
//   http://www.pjrc.com/teensy/td_libs_AudioDataFiles.html
//
// This example code is in the public domain.

#include <Audio.h>
#include <SPI.h>

#include "AudioRouting.hpp"
#include <OnOffButton.hpp>
#include <EncoderButton.hpp>
#include <PinMapping.hpp>

drop::OnOffButton button;
drop::EncoderButton encoderButton;

#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

float knobDelayLength = 0.0f;
float knobMix = 0.0f;
elapsedMillis elapsed = 0;

void buttonHandler(bool on)
{
  if (on)
  {
    float msec = 25.0 + (knobDelayLength * 975.0);
    Serial.print("Effect on with delay = ");
    Serial.print(msec);
    Serial.println("ms");
   	granular.delay(msec);
  }
  else
  {
    Serial.println("Granular off");
    //granular.stop();
    granular.disable();
  }
}

void encoderButtonHandler(long position)
{
  Serial.print("Encoder position = ");
  Serial.print(position);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  AudioMemory(10);

  button.Init(PIN_BUTTON_GRANULAR);
  button.RegisterHandler(buttonHandler);
  encoderButton.Init(PIN_ENCODER_GRANULAR_1, PIN_ENCODER_GRANULAR_2);
  encoderButton.RegisterHandler(encoderButtonHandler);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);
}

void loop() {
  button.Update();
  encoderButton.Update();

  // read knobs, scale to 0-1.0 numbers
  knobDelayLength = (float)analogRead(A2) / 1023.0;
  knobMix = (float)analogRead(A3) / 1023.0;

  if (elapsed > 1000) {
    elapsed = 0;
    Serial.print("Balance = ");
    Serial.println(knobMix);
  }

  mixer1.gain(0, 1.0 - knobMix);
  mixer1.gain(1, knobMix);

}
