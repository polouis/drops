#ifndef __DROP_GRANULAR_HPP___
#define __DROP_GRANULAR_HPP___
#include "Arduino.h"
#include "AudioStream.h"
#include "utility/dspinst.h"

#define GRANULAR_BUFFER_BLOCK_COUNT 1000
#define GRANULAR_BUFFER_LENGTH (AUDIO_BLOCK_SAMPLES * GRANULAR_BUFFER_BLOCK_COUNT)

class EffectGranular : public AudioStream
{
public:
	EffectGranular() : AudioStream(1, inputQueueArray) {
	}
	void delay(uint16_t milliseconds) {
		this->writeIndex = 0;
		memset(this->circularBlockBuffer, 0, sizeof(this->circularBlockBuffer));
		if (milliseconds < 0.0f) milliseconds = 0.0f;
    this->delayLength = milliseconds;
    this->delaySampleLength = this->delayLength * AUDIO_SAMPLE_RATE_EXACT / 1000.0f;
    this->active = true;
	}
	void disable() {
    this->active = false;
	}
	virtual void update(void);
private:
	bool active;
  uint16_t delayLength;
  uint16_t delaySampleLength;
  uint16_t writeIndex;
	int16_t circularBlockBuffer[GRANULAR_BUFFER_LENGTH];
	audio_block_t *inputQueueArray[1];
};

#endif // __DROP_GRANULAR_HPP___
