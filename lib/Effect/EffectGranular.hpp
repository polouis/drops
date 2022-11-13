#ifndef __EFFECT_GRANULAR_HPP___
#define __EFFECT_GRANULAR_HPP___

#include <Arduino.h>
#include <AudioStream.h>
#include <utility/dspinst.h>
#include <InterfaceBuffer.hpp>
#include <EnvelopeTriangle.hpp>
#include <EnvelopeIdentity.hpp>

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
    //uint16_t startIndex; // Start index of the grain in the circular Buffer
    InterfaceBufferHandler *handler;
    uint32_t delaySamples; // offset from window end
    uint32_t readSamples; // Total number of samples read since the grain is playing. Resets to zero zhen looping in freeze mode
    State state;
  };

  class EffectGranular : public AudioStream
  {
  public:
    EffectGranular() : AudioStream(1, inputQueueArray) {
      this->voices = 4;
      this->direction = 1;
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
        this->playContexts[i].state = State::Idle;
      }
      this->elapsedTrigger = 0;
    }

    void SetGrainLength(uint16_t milliseconds) {
      this->grainSampleLength = milliseconds * AUDIO_SAMPLE_RATE_EXACT / 1000.0f;
    }

    void Enable()
    {
      this->elapsedTrigger = 0;
      this->enabled = true;
    }

    void Disable() {
      this->enabled = false;
    }

    /*
     * FX
     */

    void SetEnvelope(InterfaceEnvelope *envelope);

    /*
     * BUFFER
     */
    void setBuffer(InterfaceBuffer *buffer) {
      this->buffer = buffer;
    }

  private:
    virtual void update(void) override;
    void PlayGrain(PlayContext *grain);

    bool enabled = false;
    elapsedMillis elapsedTrigger;

    // Effect parameters
    PlayContext playContexts[4];
    uint16_t intervalMilliLength;
    uint16_t intervalSampleLength;
    uint8_t voices;
    uint32_t grainSampleLength = 0;
    InterfaceEnvelope *envelope = NULL;
    uint8_t direction; // Playing direction 1 is forward, -1 is backward. TODO : to implement
    bool freeze; // In freeze mode, the voice does not go back to idle, the grain loops. TODO : cannot loop forever since the bufferis only 3s or 4s

    // Needed by Audio lib
    audio_block_t *inputQueueArray[1];

    // Buffer
    InterfaceBuffer *buffer = NULL;
  };
}

#endif // __EFFECT_GRANULAR_HPP___
