/*
 * 2 buttons, 2 waveforms, they should be able to play at the same time
 */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

AudioSynthWaveform waveform[2];
AudioOutputI2S           i2s1;           //xy=360,98
AudioOutputAnalogStereo  dacs1;          //xy=372,173
AudioConnection patchCord[] = {
  {waveform[0], 0, i2s1, 0}, {waveform[0], 0, dacs1, 0},
  {waveform[1], 0, i2s1, 1}, {waveform[1], 0, dacs1, 1},
};
AudioControlSGTL5000     sgtl5000_1;     //xy=239,232

Bounce button0 = Bounce(0, 15);

const int onamp = 10;

int cycle = 0;


void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
  
  pinMode(1, OUTPUT);

  pinMode(2, OUTPUT);

  

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.2); // caution: very loud - use oscilloscope only!
  
  waveform[0].begin(WAVEFORM_SINE);
  waveform[1].begin(WAVEFORM_SINE);

  Serial.println("Ready");
}

void loop() {
  button0.update();

  digitalWrite(cycle, LOW); 

  if (digitalRead(0) == LOW) {
    waveform[cycle - 1].frequency(440);
    waveform[cycle - 1].amplitude(onamp);
  }
  else {
    waveform[cycle - 1].amplitude(0);
  }

  digitalWrite(cycle, HIGH);

  cycle = cycle % 2 + 1;
  
}
