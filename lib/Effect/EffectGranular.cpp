#include <Arduino.h>
#include <EffectGranular.hpp>

namespace drop
{
  void EffectGranular::update(void)
  {
    if (this->enabled == false)
    {
      return;
    }

    audio_block_t* inputBlock = receiveReadOnly();
    if (inputBlock == NULL) {
      return;
    }
    this->buffer->FeedBuffer(inputBlock);
    release(inputBlock);

    uint16_t periodLength = 100;
    for (uint8_t i = 0; i < this->voices; i++) {
      //Serial.print(this->elapsedTrigger);Serial.print(" i:");Serial.print(i);Serial.print(" state:");Serial.println(this->grains[i].state);
      if (this->playContexts[i].state == State::Idle && this->elapsedTrigger >= periodLength)
      {
        //Serial.print("start playing voice:");Serial.print(i);Serial.print(" t:");Serial.println(this->elapsedTrigger);
        this->PlayGrain(&this->playContexts[i]);
        this->elapsedTrigger = 0;
      }

      if (this->playContexts[i].state == State::Playing)
      {
        audio_block_t* outputBlock = allocate();
        if (!outputBlock) return;
        
        uint32_t offset = - this->delayLength.GetSamples() + this->playContexts[i].readSamples;
        int32_t samplesToRead = this->grainLength.GetSamples() - this->playContexts[i].readSamples;
        samplesToRead = min(AUDIO_BLOCK_SAMPLES, samplesToRead);
        this->buffer->ReadBuffer(outputBlock, this->playContexts[i].handler, offset, samplesToRead);

        if (this->envelope != NULL)
        {
          for (uint8_t j = 0; j < AUDIO_BLOCK_SAMPLES; j++)
          {
            outputBlock->data[j] = this->envelope->Compute(outputBlock->data[j], this->playContexts[i].readSamples + j, this->grainLength.GetSamples());
          }
        }
        this->playContexts[i].readSamples += samplesToRead;

        if (this->playContexts[i].readSamples >= this->grainLength.GetSamples())
        {
          this->playContexts[i].state = State::Idle;
          //Serial.print("stopping i:");Serial.println(i);
          this->playContexts[i].readSamples = 0;
        }

        transmit(outputBlock, i);
        release(outputBlock);
      }
    }
  }

  void EffectGranular::PlayGrain(PlayContext *context)
  {
    context->readSamples = 0;
    context->state = State::Playing;
    context->delaySamples = random(this->delayLength.GetSamples());
    context->handler = this->buffer->CreateHandler();
  }

  void EffectGranular::SetEnvelope(InterfaceEnvelope* envelope)
  {
    this->envelope = envelope;
  }

}
