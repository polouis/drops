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

    for (uint8_t i = 0; i < this->voices; i++) {
      if (this->grains[i].state == State::Idle)
      {
        this->grains[i].state = State::Playing;
        this->grains[i].direction = 1;
        this->grains[i].length = this->delayLength * AUDIO_SAMPLE_RATE_EXACT / 1000.0f;
        this->grains[i].startIndex = (this->writeIndex - this->grains[i].length) % GRANULAR_BUFFER_LENGTH;
        this->grains[i].sampleRead = 0;
        //Serial.print("start playing "); Serial.println(this->grains[i].length);
        //Serial.print("start index "); Serial.println(this->grains[i].startIndex);
      }

      if (this->grains[i].state == State::Playing)
      {
        audio_block_t* outputBlock = allocate();
        if (!outputBlock) return;
        
        uint16_t samplesToRead = min(AUDIO_BLOCK_SAMPLES, this->grains[i].length - this->grains[i].sampleRead);
        this->ReadBuffer(outputBlock, &this->grains[i], samplesToRead);

        if (this->envelope != NULL)
        {
          for (uint32_t j = 0; j < AUDIO_BLOCK_SAMPLES; j++)
          {
            outputBlock->data[j] = this->envelope->Compute(outputBlock->data[j], this->grains[i].sampleRead + j, this->grains[i].length);
          }
        }

        this->grains[i].sampleRead += samplesToRead;
        if (this->grains[i].sampleRead >= this->grains[i].length)
        {
          this->grains[i].state = State::Skip;
          //Serial.println("finished playing");
          this->grains[i].sampleRead = 0;
        }

        transmit(outputBlock, 0);
        release(outputBlock);
      }
      else if (this->grains[i].state == State::Skip)
      {
        this->grains[i].sampleRead += AUDIO_BLOCK_SAMPLES;
        if (this->grains[i].sampleRead >= this->grains[i].length)
        {
          this->grains[i].state = State::Idle;
        }
      }
    }
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
