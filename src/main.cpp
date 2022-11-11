#include <Audio.h>
#include <SPI.h>


#include <AudioRouting.hpp>
#include <InterfaceEnvelope.hpp>
#include <EnvelopeIdentity.hpp>
#include <EnvelopeTriangle.hpp>
#include <ControlOnOffButton.hpp>
#include <ControlEncoder.hpp>
#include <PinMapping.hpp>

drop::ControlOnOffButton button;
drop::ControlEncoder encoderButton;

drop::ControlOnOffButton buttonDelay;
drop::ControlEncoder encoderDelay;

uint16_t intervalLength = 1000;

float knobMix = 0.0f;
elapsedMillis elapsed = 0;

drop::InterfaceEnvelope *envelopes[2] = { new drop::EnvelopeIdentity(), new drop::EnvelopeTriangle() };
int8_t envelopeCurrent = 0;
uint8_t envelopeCount = sizeof(envelopes) / sizeof(drop::InterfaceEnvelope *);

void buttonEnableHandler(bool on)
{
  if (on)
  {
    Serial.println("Effect on");
   	granular.Enable();
  }
  else
  {
    Serial.println("Effect off");
    granular.Disable();
  }
}

void buttonHandler(bool on)
{
  if (on)
  {
    Serial.println("Button on");
  }
  else
  {
    Serial.println("Button off");
  }
}

void encoderIntervalLengthHandler(int8_t direction)
{
  intervalLength += direction * 10;
  if (intervalLength < 50) {
    intervalLength = 50;
  }
  Serial.print("Interval length = ");
  Serial.print(intervalLength);
  Serial.println();
 	granular.SetInterval(intervalLength);
}

void encoderEnvelopeHandler(int8_t direction)
{
  envelopeCurrent = abs(envelopeCurrent + direction) % envelopeCount;
  drop::InterfaceEnvelope *envelope = envelopes[envelopeCurrent];
  granular.SetEnvelope(envelope);
  Serial.print("Envelope = ");
  Serial.print(envelope->GetName());
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  AudioMemory(10);

  button.Init(PIN_BUTTON_GRANULAR);
  button.RegisterHandler(buttonEnableHandler);
  encoderButton.Init(PIN_ENCODER_GRANULAR_1, PIN_ENCODER_GRANULAR_2);
  encoderButton.RegisterHandler(encoderIntervalLengthHandler);

  encoderDelay.Init(PIN_ENCODER_DELAY_1, PIN_ENCODER_DELAY_2);
  encoderDelay.RegisterHandler(encoderEnvelopeHandler);
  buttonDelay.Init(PIN_BUTTON_DELAY);
  buttonDelay.RegisterHandler(buttonHandler);

  granular.SetInterval(intervalLength);
  granular.SetGrainLength(500);
  granular.Disable();

  mixerGranular.gain(0, 1.0);
  mixerGranular.gain(1, 1.0);
  mixerGranular.gain(2, 1.0);
  mixerGranular.gain(3, 1.0);

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

  mixerMain.gain(0, 1.0 - knobMix);
  mixerMain.gain(1, knobMix);
}
