// CS Final Project
// Anna Little ael35
// Tyler Luce (kirbyfreak@gmail.com)

#include <Adafruit_NeoPixel.h>
#include <float.h>
//#include "functions.cpp"
#ifdef __AVR__
  #include <avr/power.h>
#endif

/* Constants */
// LED pins
#define LED1 11 // snare
#define LED2 10 // tom1
#define LED3 9 // tom2
#define LED4 6 // bass
#define LED5 5 // crash
// Sensor Pins
#define SNARE A0 // Piezo input
#define TOM1 A1
#define TOM2 A2
#define BASS A3
#define CRASH A4
// LED strips
#define SNARE_STRIP 11   // top of shade
#define TOM1_STRIP 10   // body of lamp
#define TOM2_STRIP 11   // body of lamp
#define CRASH_STRIP 17   // bottom of shade
#define BASS_STRIP 10   // inside shade

/**** global vars ****/
// snare vars
int snareCurrent = 0; // current number of LEDs on
int snareGoal = 0; // goal of how many to light up
int snareADC = 0; // reading from sensor
int snarePrev = 0;  // previous reading
Adafruit_NeoPixel snareLS = Adafruit_NeoPixel(SNARE_STRIP, LED1, NEO_RGBW); // LED strip associated w the snare
unsigned long snarePrevMillis = 0; // last time update

// tom1 vars
int tom1Current = 0;
int tom1Goal = 0;
int tom1ADC = 0;
int tom1Prev = 0;
Adafruit_NeoPixel tom1LS = Adafruit_NeoPixel(TOM1_STRIP, LED2, NEO_RGBW);
unsigned long tom1PrevMillis = 0; // last time update

// tom2 vars
int tom2Current = 0;
int tom2Goal = 0;
int tom2ADC = 0;
int tom2Prev = 0;
Adafruit_NeoPixel tom2LS = Adafruit_NeoPixel(TOM2_STRIP, LED3, NEO_RGBW);
unsigned long tom2PrevMillis = 0; // last time update

// bass vars
int bassCurrent = 0;
int bassGoal = 0;
int bassADC = 0;
int bassPrev = 0;
Adafruit_NeoPixel bassLS = Adafruit_NeoPixel(BASS_STRIP, LED4, NEO_RGBW);
unsigned long bassPrevMillis = 0; // last time update

// crash vars
int crashCurrent = 0;
int crashGoal = 0;
int crashADC = 0;
int crashPrev = 0;
Adafruit_NeoPixel crashLS = Adafruit_NeoPixel(CRASH_STRIP, LED5, NEO_RGBW);
unsigned long crashPrevMillis = 0; // last time update

unsigned int interval = 20;
unsigned int threshhold = 50;

void setup() {
  Serial.begin(9600);
  // init light strips
  snareLS.begin();
  tom1LS.begin();
  tom2LS.begin();
  bassLS.begin();
  crashLS.begin();
  // push light settings to be seen
  snareLS.show();
  tom1LS.show();
  tom2LS.show();
  bassLS.show();
  crashLS.show();
}

void loop() {
  unsigned long currentMillis = millis();
  snareADC = analogRead(SNARE);
  tom1ADC = analogRead(TOM1);
  tom2ADC = analogRead(TOM2);
  bassADC = analogRead(BASS);
  crashADC = analogRead(CRASH);

  //sensing if snare was hit and adding lights for intensity of signal
  if (snareADC > threshhold && snareADC > snarePrev && (snareADC - snarePrev) > threshhold) {
    // print voltage
    Serial.println(snareADC);
    snareGoal += 1;
    if (snareGoal > SNARE_STRIP) {
      snareGoal = SNARE_STRIP;
    }
  }

  snarePrev = snareADC;

  if (currentMillis - snarePrevMillis > interval) {
    snarePrevMillis = currentMillis;
    bool movingUp = false;

    if (snareCurrent < snareGoal) {
      turnLightsOnFromBottom('r', 1, snareCurrent, &snareLS); // Tyler defined
      snareCurrent += 1;
      movingUp = true;
    } else if (snareCurrent > snareGoal){
      turnLightsOffFromTop(1, snareCurrent, &snareLS); // Tyler defined
      snareCurrent -= 1;
    }

    if (snareGoal > 0 && !movingUp){
      snareGoal--;
    }
  }

  //sensing if tom1 was hit and adding lights for intensity of signal
  if (tom1ADC > threshhold && tom1ADC > tom1Prev && (tom1ADC - tom1Prev) > threshhold) {
    // print voltage
    Serial.println(tom1ADC);
    tom1Goal += 1;
    if (tom1Goal > TOM1_STRIP) {
      tom1Goal = TOM1_STRIP;
    }
  }

  tom1Prev = tom1ADC;

  if (currentMillis - tom1PrevMillis > interval) {
    tom1PrevMillis = currentMillis;
    bool movingUp = false;

    if (tom1Current < tom1Goal) {
      turnLightsOnFromBottom('g', 1, tom1Current, &tom1LS); // Tyler defined
      tom1Current += 1;
      movingUp = true;
    } else if (tom1Current > tom1Goal){
      turnLightsOffFromTop(1, tom1Current, &tom1LS); // Tyler defined
      tom1Current -= 1;
    }

    if (tom1Goal > 0 && !movingUp){
      tom1Goal--;
    }
  }

  //sensing if tom2 was hit and adding lights for intensity of signal
  if (tom2ADC > 30 && tom2ADC > tom2Prev && (tom2ADC - tom2Prev) > threshhold) {
    // print voltage
    Serial.println(tom2ADC);
    tom2Goal += 1;
    if (tom2Goal > TOM2_STRIP) {
      tom2Goal = TOM2_STRIP;
    }
  }

  tom2Prev = tom2ADC;

  if (currentMillis - tom2PrevMillis > interval) {
    tom2PrevMillis = currentMillis;
    bool movingUp = false;

    if (tom2Current < tom2Goal) {
      turnLightsOnFromBottom('r', 1, tom2Current, &tom2LS); // Tyler defined
      tom2Current += 1;
      movingUp = true;
    } else if (tom2Current > tom2Goal){
      turnLightsOffFromTop(1, tom2Current, &tom2LS); // Tyler defined
      tom2Current -= 1;
    }

    if (tom2Goal > 0 && !movingUp){
      tom2Goal--;
    }
  }

  //sensing if crash was hit and adding lights for intensity of signal
  if (crashADC > (threshhold + 10) && crashADC > crashPrev && (crashADC - crashPrev) > (threshhold + 10)) {
    // print voltage
    Serial.println(crashADC);
    crashGoal += 1;
    if (crashGoal > CRASH_STRIP) {
      crashGoal = CRASH_STRIP;
    }
  }

  crashPrev = crashADC;

  if (currentMillis - crashPrevMillis > interval) {
    crashPrevMillis = currentMillis;
    bool movingUp = false;

    if (crashCurrent < crashGoal) {
      turnLightsOnFromBottom('b', 1, crashCurrent, &crashLS); // Tyler defined
      crashCurrent += 1;
      movingUp = true;
    } else if (crashCurrent > crashGoal){
      turnLightsOffFromTop(1, crashCurrent, &crashLS); // Tyler defined
      crashCurrent -= 1;
    }

    if (crashGoal > 0 && !movingUp){
      crashGoal--;
    }
  }

  //sensing if bass was hit and adding lights for intensity of signal
  if (bassADC > threshhold && bassADC > bassPrev && (bassADC - bassPrev) > threshhold) {
    // print voltage
    Serial.println(bassADC);
    bassGoal += 1;
    if (bassGoal > BASS_STRIP) {
      bassGoal = BASS_STRIP;
    }
  }

  bassPrev = bassADC;

  if (currentMillis - bassPrevMillis > interval) {
    bassPrevMillis = currentMillis;
    bool movingUp = false;

    if (bassCurrent < bassGoal) {
      turnLightsOnFromBottom(1, bassCurrent, &bassLS); // Tyler defined
      bassCurrent += 1;
      movingUp = true;
    } else if (bassCurrent > bassGoal){
      turnLightsOffFromTop('w', 1, bassCurrent, &bassLS); // Tyler defined
      bassCurrent -= 1;
    }

    if (bassGoal > 0 && !movingUp){
      bassGoal--;
    }
  }
}

/* functions */
// Turns on this many lights from the bottom insantly
void turnLightsOnFromBottom(char c, uint32_t numToTurnOn, int currentAmountOn, Adafruit_NeoPixel* strip) {
//  Serial.println("turning on");

  signed int goal = currentAmountOn+numToTurnOn - 1;
  if (goal > strip->numPixels() - 1) {
    goal = strip->numPixels() - 1;
  }

//  Serial.println(currentAmountOn);
//  Serial.println(goal);

for (uint16_t i=currentAmountOn; i<=goal; i++) {
//    Serial.println("current");
//    Serial.println(i);
  switch(c){
    case 'r':
      strip->setPixelColor(i, strip->Color(0, 255, 0, 10));
    case 'g':
      strip->setPixelColor(i, strip->Color(255, 0, 0, 0));
    case 'b':
      strip->setPixelColor(i, strip->Color(0, 0, 255, 0));
    case 'w':
      strip->setPixelColor(i, strip->Color(0, 0, 0, 255));
  }
}
strip->show();
}

// Turns off this many lights from the top instantly
void turnLightsOffFromTop(uint32_t numToTurnOff, int currentAmountOn, Adafruit_NeoPixel* strip) {
//  Serial.println("turning off");

  signed int goal = currentAmountOn-numToTurnOff;
  if (goal < 0) {
    goal = 0;
  }

//  Serial.println(currentAmountOn-1);
//  Serial.println(goal);

  for (signed int i=currentAmountOn-1; i>=goal; i--) {
//    Serial.println("current");
//    Serial.println(i);
    strip->setPixelColor(i, strip->Color(0, 0, 0, 0));
  }

//  for (uint16_t i=currentAmountOn; i<=goal; i++) {
//    Serial.println("current");
//    Serial.println(i);
//    strip->setPixelColor(strip->numPixels() - i, strip->Color(0, 0, 255));
//  }
  strip->show();
}

/* turns entire strip on a specifc color and highest brightness */
void turnStripOn(char c, int numLEDs, Adafruit_NeoPixel* strip){
  for (uint16_t i=0; i > numLEDs; i++) {
    switch(c){
      case 'r':
        strip->setPixelColor(i, strip->Color(0, 255, 0, 10));
      case 'g':
        strip->setPixelColor(i, strip->Color(255, 0, 0, 0));
      case 'b':
        strip->setPixelColor(i, strip->Color(0, 0, 255, 0));
      case 'w':
        strip->setPixelColor(i, strip->Color(0, 0, 0, 255));
    }
  }
  strip->show();
}

void fadeColor(int fadeFactor, char c, int numLEDs, Adafruit_NeoPixel* strip){
  int level = 255 - fadeFactor;
  for (uint16_t i=0; i > numLEDs; i++) {
    switch(c){
      case 'r':
        strip->setPixelColor(i, strip->Color(0, level, 0, 0));
      case 'g':
        strip->setPixelColor(i, strip->Color(level, 0, 0, 0));
      case 'b':
        strip->setPixelColor(i, strip->Color(0, 0, level, 0));
      case 'w':
        strip->setPixelColor(i, strip->Color(0, 0, 0, level));
    }
  }
  strip->show();
}
