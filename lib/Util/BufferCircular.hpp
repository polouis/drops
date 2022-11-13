#ifndef __BUFFER_CIRCULAR_HPP__
#define __BUFFER_CIRCULAR_HPP__

#include <InterfaceBuffer.hpp>

namespace drop
{
  //#define GRANULAR_BUFFER_LENGTH (1034 * AUDIO_BLOCK_SAMPLES)//(3 * 44100) // 3 seconds mono
  #define GRANULAR_BUFFER_LENGTH (3 * 44100) // 3 seconds mono

  class BufferCircularHandler: public InterfaceBufferHandler
  {
    friend class BufferCircular;
    private:
      void SetStartIndex(uint32_t index)
      {
        this->startIndex = index;
      }

      uint32_t startIndex;
  };

  class BufferCircular : public InterfaceBuffer
  {
  public:
    BufferCircular()
    {
      this->writeIndex = 0;
      this->circularBlockBuffer = (int16_t*)malloc(sizeof(int16_t) * GRANULAR_BUFFER_LENGTH);
      if (this->circularBlockBuffer == NULL)
      {
        Serial.print("Failed to allocate : "); Serial.println(sizeof(int16_t) * GRANULAR_BUFFER_LENGTH);
      }
      memset(this->circularBlockBuffer, 0, sizeof(int16_t) * GRANULAR_BUFFER_LENGTH);
    }

    virtual InterfaceBufferHandler *CreateHandler() override;
    virtual void FeedBuffer(audio_block_t* block) override;
    virtual void ReadBuffer(audio_block_t *block, InterfaceBufferHandler *bufferHandler, int32_t offset, uint32_t length) override;

  private:
    int16_t *circularBlockBuffer;
    uint32_t writeIndex;
  };
}

#endif // __BUFFER_CIRCULAR_HPP__
