#include <Audio.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=139.28571428571425,384.9999999999999
AudioEffectGranular      granular1;      //xy=307.42855072021484,430.7142524719238
AudioMixer4              mixer1;         //xy=488.2856979370117,375.5713920593262
AudioOutputI2S           i2s1;           //xy=653.8570785522461,470.0000057220459
AudioConnection          patchCord1(i2s2, 0, mixer1, 0);
AudioConnection          patchCord2(i2s2, 0, granular1, 0);
AudioConnection          patchCord3(granular1, 0, mixer1, 1);
AudioConnection          patchCord4(mixer1, 0, i2s1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=210.14285278320312,500.1428527832031
// GUItool: end automatically generated code
