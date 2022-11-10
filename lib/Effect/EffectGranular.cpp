#include <Arduino.h>
#include <EffectGranular.hpp>

namespace drop
{
  void EffectGranular::update(void)
  {
    if (this->active == false)
    {
      return;
    }

    this->FeedBuffer();

    uint16_t period = 100;
    for (uint8_t i = 0; i < this->voices; i++) {
      //Serial.print(this->elapsedTrigger);Serial.print(" i:");Serial.print(i);Serial.print(" state:");Serial.println(this->grains[i].state);
      if (this->grains[i].state == State::Idle && this->elapsedTrigger >= period)
      {
        this->PlayGrain(&this->grains[i]);
        //Serial.print("start playing voice:");Serial.print(i);Serial.print(" t:");Serial.println(this->elapsedTrigger);
        this->elapsedTrigger = 0;
      }

      if (this->grains[i].state == State::Playing)
      {
        audio_block_t* outputBlock = allocate();
        if (!outputBlock) return;
        
        uint16_t samplesToRead = min(AUDIO_BLOCK_SAMPLES, this->grainSampleLength - this->grains[i].sampleRead);
        this->ReadBuffer(outputBlock, &this->grains[i], samplesToRead);

        if (this->envelope != NULL)
        {
          for (uint8_t j = 0; j < AUDIO_BLOCK_SAMPLES; j++)
          {
            outputBlock->data[j] = this->envelope->Compute(outputBlock->data[j], this->grains[i].sampleRead + j, this->grainSampleLength);
          }
        }
        this->grains[i].sampleRead += samplesToRead;

        if (this->grains[i].sampleRead >= this->grainSampleLength)
        {
          this->grains[i].state = State::Idle;
          //Serial.print("stopping i:");Serial.println(i);
          this->grains[i].sampleRead = 0;
        }

        transmit(outputBlock, 0);
        release(outputBlock);
      }
    }
  }

  void EffectGranular::PlayGrain(Grain *grain)
  {
    grain->state = State::Playing;
    grain->direction = 1;
    grain->startIndex = (this->writeIndex - this->intervalSampleLength + random(this->intervalSampleLength)) % GRANULAR_BUFFER_LENGTH;
    grain->sampleRead = 0;
  }

  void EffectGranular::SetEnvelope(InterfaceEnvelope* envelope)
  {
    this->envelope = envelope;
  }

  // BUFFER SECTION
  void EffectGranular::FeedBuffer()
  {
    if (this->active == false) {
      return;
    }

    audio_block_t* inputBlock = receiveReadOnly();
    if (inputBlock == NULL) {
      return;
    }

    memcpy(&this->circularBlockBuffer[this->writeIndex], inputBlock->data, AUDIO_BLOCK_SAMPLES * sizeof(uint16_t));

    this->writeIndex += AUDIO_BLOCK_SAMPLES;
    if (this->writeIndex >= GRANULAR_BUFFER_LENGTH) {
      this->writeIndex = 0;
    }
    release(inputBlock);
  }

  void EffectGranular::ReadBuffer(audio_block_t *block, Grain* grain, uint16_t length)
  {
    for (uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
      uint16_t readIndex = (grain->startIndex + grain->sampleRead + i) % GRANULAR_BUFFER_LENGTH;
      if (i >= length)
      {
        block->data[i] = 0;
      }
      else
      {
        block->data[i] = this->circularBlockBuffer[readIndex];
      }
    }
  }

}
