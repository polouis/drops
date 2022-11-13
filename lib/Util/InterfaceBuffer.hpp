#ifndef __INTERFACE_BUFFER_HPP__
#define __INTERFACE_BUFFER_HPP__

#include <Arduino.h>
#include <AudioStream.h>

namespace drop
{
  class InterfaceBufferHandler
  {
  };

  class InterfaceBuffer
  {
  public:
    virtual InterfaceBufferHandler *CreateHandler() = 0;
    virtual void FeedBuffer(audio_block_t* block) = 0;
    virtual void ReadBuffer(audio_block_t *block, InterfaceBufferHandler *bufferHandler, int32_t offset, uint32_t length) = 0;
  };
}

#endif // __INTERFACE_BUFFER_HPP__
