#include <Arduino.h>
#include <EffectGranular.hpp>

void EffectGranular::update(void)
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

	audio_block_t* outputBlock = allocate();
	if (!outputBlock) return;
  uint16_t readIndex = 0;

  readIndex = writeIndex - this->delaySampleLength;
  memcpy(outputBlock->data, &this->circularBlockBuffer[readIndex], min(AUDIO_BLOCK_SAMPLES, GRANULAR_BUFFER_LENGTH - readIndex) * sizeof(uint16_t));
  if (GRANULAR_BUFFER_LENGTH - readIndex < AUDIO_BLOCK_SAMPLES) {
    memcpy(&outputBlock->data[GRANULAR_BUFFER_LENGTH - readIndex], this->circularBlockBuffer, (AUDIO_BLOCK_SAMPLES - (GRANULAR_BUFFER_LENGTH - readIndex)) * sizeof(uint16_t));
  }

  release(inputBlock);

	transmit(outputBlock, 0);
  release(outputBlock);
}

