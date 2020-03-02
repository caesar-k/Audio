/*
 * 9 button matrix 
 * on pins 0, 1, and 2 
 * should be able to play at the same time, 
 * but some tones will take precedence over others 
 *
 */

#include "Pizzicato_samples.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

const int NUMBUTTONS = 9; // number of buttons 
const int TOTAL_MIXERS = 3;

AudioSynthWavetable wavetable[NUMBUTTONS];
AudioMixer4 mixer[TOTAL_MIXERS]; 
AudioOutputI2S           i2s1;           //xy=360,98
AudioConnection patchCord[] = {
  {wavetable[0], 0, mixer[0], 0}, {wavetable[1], 0, mixer[0], 1}, {wavetable[2], 0, mixer[0],  2}, {wavetable[3], 0, mixer[0],  3}, {mixer[0], 0, mixer[TOTAL_MIXERS - 2], 0},
  {wavetable[4], 0, mixer[1], 0}, {wavetable[5], 0, mixer[1], 1}, {wavetable[6], 0, mixer[1],  2}, {wavetable[7], 0, mixer[1],  3}, {mixer[1], 0, mixer[TOTAL_MIXERS - 2], 1},
  {wavetable[8], 0, mixer[2], 0}, {mixer[2], 0, mixer[TOTAL_MIXERS - 2], 2},
  {mixer[TOTAL_MIXERS - 2], 0, mixer[TOTAL_MIXERS - 1], 0},
  {mixer[TOTAL_MIXERS - 1], 0, i2s1, 0},
  {mixer[TOTAL_MIXERS - 1], 0, i2s1, 1},
};
AudioControlSGTL5000     sgtl5000_1;     //xy=239,232

// notes/frequencies the wavetable plays at 
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
    wavetable[i].setInstrument(Pizzicato);
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
    wavetable[cycle].frequency(330);
    //waveform[cycle].frequency(tones[row - 3][0]);
    wavetable[cycle].amplitude(onamp);
  }
  else {
    waveform[cycle].amplitude(0);
  }
  if (digitalRead(1) == LOW) {
    Serial.println("Pressed 1");
    wavetable[cycle + 2].frequency(263);
    // waveform[cycle + 1].frequency(tones[row - 3][1]);
    wavetable[cycle + 1].amplitude(onamp);
  }
  else {
    wavetable[cycle + 1].amplitude(0);
  }
  if (digitalRead(2) == LOW) {
    Serial.println("Pressed 2");
    wavetable[cycle + 2].frequency(292);
    //wavetable[cycle + 2].frequency(tones[row - 3][2]);
    wavetable[cycle + 2].amplitude(onamp);
  }                                             
  else {
    wavetable[cycle + 2].amplitude(0);
  } 

  digitalWrite(row, HIGH);

  delayMicroseconds(10);

  row = ((row + 1) % 3) + 3;

  cycle = (cycle + 3) % 9;
  
}
