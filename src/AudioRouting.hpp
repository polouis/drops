#include <Audio.h>
#include <EffectGranular.hpp>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=163.28571319580078,338
drop::EffectGranular     granular;       //xy=331,432
AudioMixer4              mixer;         //xy=496.28570556640625,349.5713806152344
AudioMixer4              mixerGranular;  //xy=496.28570556640625,349.5713806152344
AudioOutputI2S           i2s1;           //xy=744.8570556640625,356


// Dry
AudioConnection          patchCord1(i2s2, 0, mixer, 0);
AudioConnection          patchCord2(mixer, 0, i2s1, 0);
AudioConnection          patchCord3(mixer, 0, i2s1, 1);

AudioConnection          patchCord4(i2s2, 0, granular, 0);
AudioConnection          patchCord5(granular, 0, mixerGranular, 0);
AudioConnection          patchCord6(granular, 1, mixerGranular, 1);
AudioConnection          patchCord7(granular, 2, mixerGranular, 2);
AudioConnection          patchCord8(granular, 3, mixerGranular, 3);

AudioConnection          patchCord9(mixerGranular, 0, mixer, 1);

AudioControlSGTL5000     sgtl5000_1;     //xy=175.14285278320312,477.142822265625
// GUItool: end automatically generated code
