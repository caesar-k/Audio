/*
 * 9 button matrix 
 * on pins 0, 1, and 2 
 * should be able to play at the same time, 
 * but some tones will take precedence over others 
 *
 */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

const int NUMBUTTONS = 9; // number of buttons 

AudioSynthWaveform waveform[NUMBUTTONS];
AudioOutputI2S           i2s1;           //xy=360,98
AudioOutputAnalogStereo  dacs1;          //xy=372n,173
AudioConnection patchCord[] = {
  {waveform[0], 0, i2s1, 3}, {waveform[0], 0, dacs1, 3},
  {waveform[1], 0, i2s1, 4}, {waveform[1], 0, dacs1, 4},
  {waveform[2], 0, i2s1, 5}, {waveform[2], 0, dacs1, 5},
  
  {waveform[3], 1, i2s1, 3}, {waveform[3], 1, dacs1, 3},
  {waveform[4], 1, i2s1, 4}, {waveform[4], 1, dacs1, 4},
  {waveform[5], 1, i2s1, 5}, {waveform[5], 1, dacs1, 5},
  
  {waveform[6], 2, i2s1, 3}, {waveform[6], 2, dacs1, 3},
  {waveform[7], 2, i2s1, 4}, {waveform[7], 2, dacs1, 4},
  {waveform[8], 2, i2s1, 5}, {waveform[8], 2, dacs1, 5},
};
AudioControlSGTL5000     sgtl5000_1;     //xy=239,232

// notes/frequencies the waveform plays at 
int tones[3][3] = {
  {262, 294, 330},
  {349, 392, 440},
  {494, 523, 587},
};

Bounce buttons[] = {
  {0, 15},
  {1, 15},
  {2, 15},
};

int row = 3;
int cycle = 0;
const int onamp = 10;

void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(50);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.2); // caution: very loud - use oscilloscope only!

  for (int i = 0; i < NUMBUTTONS; i++) {
    waveform[i].begin(WAVEFORM_SINE);
  }

  Serial.println("Ready");
}

void loop() {
  buttons[0].update();
  buttons[1].update();
  buttons[2].update();

  Serial.println(digitalRead(2));

  digitalWrite(row, LOW);

  if (digitalRead(0) == LOW) {
    Serial.println("Pressed 0");
    waveform[cycle].frequency(330);
    //waveform[cycle].frequency(tones[row - 3][0]);
    waveform[cycle].amplitude(onamp);
  }
  else {
    waveform[cycle].amplitude(0);
  }
  if (digitalRead(1) == LOW) {
    Serial.println("Pressed 1");
    waveform[cycle + 2].frequency(263);
    // waveform[cycle + 1].frequency(tones[row - 3][1]);
    waveform[cycle + 1].amplitude(onamp);
  }
  else {
    waveform[cycle + 1].amplitude(0);
  }
  if (digitalRead(2) == LOW) {
    Serial.println("Pressed 2");
    waveform[cycle + 2].frequency(292);
    //waveform[cycle + 2].frequency(tones[row - 3][2]);
    waveform[cycle + 2].amplitude(onamp);
  }                                             
  else {
    waveform[cycle + 2].amplitude(0);
  } 

  digitalWrite(row, HIGH);

  delayMicroseconds(10);

  row = ((row + 1) % 3) + 3;

  cycle = (cycle + 3) % 9;
  
}
