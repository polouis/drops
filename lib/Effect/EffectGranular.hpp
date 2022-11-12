#ifndef __EFFECT_GRANULAR_HPP___
#define __EFFECT_GRANULAR_HPP___

#include <Arduino.h>
#include <AudioStream.h>
#include <utility/dspinst.h>
//#include <InterfaceEnvelope.hpp>
#include <EnvelopeTriangle.hpp>
#include <EnvelopeIdentity.hpp>


#define GRANULAR_BUFFER_LENGTH (3 * 44100) // 3 seconds mono

namespace drop
{
  enum Envelope
  {
    Identity,
    Triangle,
  };

  enum State
  {
    Idle,
    Playing,
  };

  struct PlayContext // playing context
  {
    uint16_t startIndex; // Start index of the grain in the circular Buffer
    uint16_t sampleRead; // Total number of samples read since the grain is playing. Resets to zero zhen looping in freeze mode
    State state;
  };

  class EffectGranular : public AudioStream
  {
  public:
    EffectGranular() : AudioStream(1, inputQueueArray) {
      this->writeIndex = 0;
      this->voices = 4;
      this->direction = 1;
      this->circularBlockBuffer = (int16_t*)malloc(sizeof(int16_t) * GRANULAR_BUFFER_LENGTH);
      if (this->circularBlockBuffer == NULL)
      {
        Serial.print("Failed to allocate : "); Serial.println(sizeof(int16_t) * GRANULAR_BUFFER_LENGTH);
      }
      memset(this->circularBlockBuffer, 0, sizeof(int16_t) * GRANULAR_BUFFER_LENGTH);
    }

    /*
     * CONTROL
     */

    void SetInterval(uint16_t milliseconds) {
      if (milliseconds < 0.0f) milliseconds = 0.0f;
      this->intervalMilliLength = milliseconds;
      this->intervalSampleLength = milliseconds * AUDIO_SAMPLE_RATE_EXACT / 1000.0f;

      for (uint8_t i = 0; i < this->voices; i++)
      {
        this->grains[i].state = State::Idle;
      }
      this->elapsedTrigger = 0;
    }

    void SetGrainLength(uint16_t milliseconds) {
      this->grainSampleLength = milliseconds * AUDIO_SAMPLE_RATE_EXACT / 1000.0f;
    }

    void Enable()
    {
      this->elapsedTrigger = 0;
      this->active = true;
    }

    void Disable() {
      this->active = false;
    }

    /*
     * FX
     */

    void SetEnvelope(InterfaceEnvelope *envelope);

    /*
     * BUFFER
     */
    void FeedBuffer();
    void ReadBuffer(audio_block_t *block, PlayContext* grain, uint16_t length);

  private:
    virtual void update(void) override;
    void PlayGrain(PlayContext *grain);
    // Effect parameters
    elapsedMillis elapsedTrigger;
    PlayContext grains[4];
    uint16_t intervalMilliLength;
    uint16_t intervalSampleLength;
    uint8_t voices;
    uint16_t grainSampleLength = 0;
    InterfaceEnvelope *envelope = NULL;
    uint8_t direction; // Playing direction 1 is forward, -1 is backward. TODO : to implement
    bool freeze; // In freeze mode, the voice does not go back to idle, the grain loops. TODO : cannot loop forever since the bufferis only 3s or 4s
    // Buffer
    int16_t *circularBlockBuffer;
    uint16_t writeIndex;
    // Needed by Audio lib
    audio_block_t *inputQueueArray[1];
  };
}

#endif // __EFFECT_GRANULAR_HPP___
