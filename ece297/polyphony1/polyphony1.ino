
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

AudioSynthWaveform       waveform1;      //xy=171,84
AudioSynthWaveform       waveform2;      //xy=178,148
AudioOutputI2S           i2s1;           //xy=360,98
AudioOutputAnalogStereo  dacs1;          //xy=372,173
AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
AudioConnection          patchCord2(waveform1, 0, dacs1, 0);
AudioConnection          patchCord3(waveform2, 0, i2s1, 1);
AudioConnection          patchCord4(waveform2, 0, dacs1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=239,232

Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);
Bounce button2 = Bounce(2, 15);

int current_waveform=0;

const int onamp = 10;

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.1); // caution: very loud - use oscilloscope only!
  
  waveform1.begin(WAVEFORM_SINE);
  waveform2.begin(WAVEFORM_SINE);

  waveform1.frequency(262);
  waveform2.frequency(392);
}

void loop() {
  waveform1.amplitude(onamp);
  delay(500);
  waveform2.amplitude(onamp);
  delay(500);
  waveform1.amplitude(0);
  delay(500);
  waveform2.amplitude(0);
  delay(500);
}
