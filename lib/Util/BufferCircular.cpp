#include <BufferCircular.hpp>

namespace drop
{
  InterfaceBufferHandler *BufferCircular::CreateHandler()
  {
    BufferCircularHandler* handler = new BufferCircularHandler();
    handler->SetStartIndex(this->writeIndex);
    return handler;
  }

  void BufferCircular::FeedBuffer(audio_block_t* block)
  {
    uint32_t remainingBufferSamples = GRANULAR_BUFFER_LENGTH - this->writeIndex;
    if (remainingBufferSamples >= AUDIO_BLOCK_SAMPLES)
    {
      memcpy(&this->circularBlockBuffer[this->writeIndex], block->data, AUDIO_BLOCK_SAMPLES * sizeof(uint16_t));
      this->writeIndex += AUDIO_BLOCK_SAMPLES;
    }
    else
    {
      memcpy(&this->circularBlockBuffer[this->writeIndex], block->data, remainingBufferSamples * sizeof(uint16_t));
      memcpy(&this->circularBlockBuffer[0], block->data, AUDIO_BLOCK_SAMPLES * (AUDIO_BLOCK_SAMPLES - remainingBufferSamples));
      this->writeIndex = AUDIO_BLOCK_SAMPLES - remainingBufferSamples;
    }
  }

  void BufferCircular::ReadBuffer(audio_block_t *block, InterfaceBufferHandler *iHandler, int32_t offset, uint32_t length)
  {
    BufferCircularHandler *handler = (BufferCircularHandler*)iHandler;
    for (uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
      uint32_t readIndex = (handler->startIndex + offset + i) % GRANULAR_BUFFER_LENGTH;
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
