#ifndef __EFFECT_GRANULAR_HPP___
#define __EFFECT_GRANULAR_HPP___

#include <Arduino.h>
#include <AudioStream.h>
#include <utility/dspinst.h>
#include <InterfaceEnvelope.hpp>
#include <EnvelopeTriangle.hpp>
#include <EnvelopeIdentity.hpp>

#define GRANULAR_BUFFER_BLOCK_COUNT 1000
#define GRANULAR_BUFFER_LENGTH (AUDIO_BLOCK_SAMPLES * GRANULAR_BUFFER_BLOCK_COUNT)

namespace drop
{
  enum State
  {
    Idle,
    Playing,
    Skip,
  };

  struct Grain
  {
    uint16_t startIndex; // Start index of the grain in the circular Buffer
    uint16_t sampleRead; // Total number of samples read since the grain is playing. Resets to zero zhen looping in freeze mode
    uint16_t length; // Number of samples in the grain
    uint8_t direction; // Playing direction 1 is forward, -1 is backward. TODO : to implement
    bool freeze; // In freeze mode, the voice does not go back to idle, the grain loops. TODO : cannot loop forever since the bufferis only 3s or 4s
    State state;
  };

  class EffectGranular : public AudioStream
  {
  public:
    EffectGranular() : AudioStream(1, inputQueueArray) {
      memset(this->circularBlockBuffer, 0, sizeof(this->circularBlockBuffer));
      this->writeIndex = 0;
      this->voices = 1;
    }

    void Delay(uint16_t milliseconds) {
      if (milliseconds < 0.0f) milliseconds = 0.0f;
      this->delayLength = milliseconds;

      for (uint8_t i = 0; i < this->voices; i++)
      {
        this->grains[i].state = State::Idle;
      }
    }

    void Enable()
    {
      this->active = true;
    }

    void Disable() {
      this->active = false;
    }
    
    virtual void update(void);
    void FeedBuffer();
    void ReadBuffer(audio_block_t *block, Grain* grain, uint16_t length);

  private:
    Grain grains[1];
    uint16_t delayLength;
    uint16_t writeIndex;
    int16_t circularBlockBuffer[GRANULAR_BUFFER_LENGTH];
    audio_block_t *inputQueueArray[1];
    uint8_t voices;
    InterfaceEnvelope *envelope = new EnvelopeTriangle();
  };
}

#endif // __EFFECT_GRANULAR_HPP___
