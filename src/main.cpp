#include <Audio.h>
#include <Arduino.h>
#include <MCP23017.h>

#include <AudioRouting.hpp>
#include <PinMapping.hpp>

#include <BufferCircular.hpp>
#include <InterfaceEnvelope.hpp>
#include <EnvelopeIdentity.hpp>
#include <EnvelopeTriangle.hpp>
#include <ControlOnOffButton.hpp>
#include <ControlEncoder.hpp>

drop::ControlOnOffButton buttonEnable;
drop::ControlEncoder encoderButton;

drop::ControlOnOffButton buttonUnused;
drop::ControlEncoder encoderDelay;

uint16_t intervalLength = 1000;
uint16_t grainLength = 250;

float knobMix = 0.0f;
elapsedMillis elapsed = 0;

drop::BufferCircular *buffer;
drop::InterfaceEnvelope *envelopes[2] = { new drop::EnvelopeIdentity(), new drop::EnvelopeTriangle() };
int8_t envelopeCurrent = 0;
uint8_t envelopeCount = sizeof(envelopes) / sizeof(drop::InterfaceEnvelope *);

MCP23017 mcp1;

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

void buttonUnusedHandler(bool on)
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

  mcp1.begin(MCP23017_ADDR_1);

  buttonEnable.Init(PIN_BUTTON_GRANULAR);
  buttonEnable.RegisterHandler(buttonEnableHandler);
  encoderButton.Init(PIN_ENCODER_GRANULAR_1, PIN_ENCODER_GRANULAR_2);
  encoderButton.RegisterHandler(encoderIntervalLengthHandler);

  encoderDelay.Init(PIN_ENCODER_DELAY_1, PIN_ENCODER_DELAY_2);
  encoderDelay.RegisterHandler(encoderEnvelopeHandler);
  buttonUnused.Init(PIN_BUTTON_DELAY);
  buttonUnused.RegisterHandler(buttonUnusedHandler);

  buffer = new drop::BufferCircular();
  granular.setBuffer(buffer);
  granular.SetInterval(intervalLength);
  granular.SetGrainLength(grainLength);
  granular.Disable();

  mixerGranular.gain(0, 1.0);
  mixerGranular.gain(1, 1.0);
  mixerGranular.gain(2, 1.0);
  mixerGranular.gain(3, 1.0);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);
}

void loop() {
  buttonUnused.Update();
  encoderDelay.Update();

  buttonEnable.Update();
  encoderButton.Update();

  // read knobs, scale to 0-1.0 numbers
  //knobDelayLength = (float)analogRead(A2) / 1023.0;
  knobMix = (float)analogRead(A3) / 1023.0;

  if (elapsed > 5000) {
    elapsed = 0;
    Serial.print("CPU usage = "); Serial.println(AudioProcessorUsage());
    Serial.print("mem usage = "); Serial.println(AudioMemoryUsage());
  }

  mixerMain.gain(0, 1.0 - knobMix);
  mixerMain.gain(1, knobMix);
}
