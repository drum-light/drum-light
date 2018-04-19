// CS Final Project
// Anna Little ael35
// Tyler Luce (kirby_freak@mac.com)

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
unsigned int snareThreshold = 50;
Adafruit_NeoPixel snareLS = Adafruit_NeoPixel(SNARE_STRIP, LED1, NEO_RGBW); // LED strip associated w the snare
unsigned long snarePrevMillis = 0; // last time update

// tom1 vars
int tom1Current = 0;
int tom1Goal = 0;
int tom1ADC = 0;
int tom1Prev = 0;
unsigned int tom1Threshold = 50;
Adafruit_NeoPixel tom1LS = Adafruit_NeoPixel(TOM1_STRIP, LED2, NEO_RGBW);
unsigned long tom1PrevMillis = 0; // last time update

// tom2 vars
int tom2Current = 0;
int tom2Goal = 0;
int tom2ADC = 0;
int tom2Prev = 0;
unsigned int tom2Threshold = 50;
Adafruit_NeoPixel tom2LS = Adafruit_NeoPixel(TOM2_STRIP, LED3, NEO_RGBW);
unsigned long tom2PrevMillis = 0; // last time update

// bass vars
int bassCurrent = 0;
int bassGoal = 0;
int bassADC = 0;
int bassPrev = 0;
unsigned int bassThreshold = 50;
Adafruit_NeoPixel bassLS = Adafruit_NeoPixel(BASS_STRIP, LED4, NEO_RGBW);
unsigned long bassPrevMillis = 0; // last time update

// crash vars
int crashCurrent = 0;
int crashGoal = 0;
int crashADC = 0;
int crashPrev = 0;
unsigned int crashThreshold = 60;
Adafruit_NeoPixel crashLS = Adafruit_NeoPixel(CRASH_STRIP, LED5, NEO_RGBW);
unsigned long crashPrevMillis = 0; // last time update

unsigned int interval = 20;
unsigned int threshold = 50;

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

  basicDrumLogic(&snareLS, 'r', snareGoal, snareCurrent, snarePrev, currentMillis, snarePrevMillis, snareADC, snareThreshold, SNARE_STRIP);
  basicDrumLogic(&tom1LS, 'r', tom1Goal, tom1Current, tom1Prev, currentMillis, tom1PrevMillis, tom1ADC, tom1Threshold, TOM1_STRIP);
  basicDrumLogic(&tom2LS, 'r', tom2Goal, tom2Current, tom2Prev, currentMillis, tom2PrevMillis, tom2ADC, tom2Threshold, TOM2_STRIP);
  basicDrumLogic(&crashLS, 'r', crashGoal, crashCurrent, crashPrev, currentMillis, crashPrevMillis, crashADC, crashThreshold, CRASH_STRIP);
  basicDrumLogic(&bassLS, 'r', bassGoal, bassCurrent, bassPrev, currentMillis, bassPrevMillis, bassADC, bassThreshold, BASS_STRIP);

}
/* functions */

// Logic loop for a drum/strip pair
void basicDrumLogic(Adafruit_NeoPixel* strip, char color, int &goal, int &current, int &prev, unsigned long &currentMillis, unsigned long &prevMillis, int &adc, unsigned int &threshold, int stripLedCount) {
  //sensing if the drum was hit and adding lights for intensity of signal
  if (adc > threshold && ADC > prev && (adc - prev) > threshold) {
    // print voltage
    Serial.println(ADC);
    goal += 1;
    if (goal > stripLedCount) {
      goal = stripLedCount;
    }
  }

  prev = adc;

  if (currentMillis - prevMillis > interval) {
    prevMillis = currentMillis;
    bool movingUp = false;

    if (current < goal) {
      turnLightsOnFromBottom(color, 1, current, strip); // Tyler defined
      current += 1;
      movingUp = true;
    } else if (current > goal){
      turnLightsOffFromTop(1, current, strip); // Tyler defined
      current -= 1;
    }

    if (goal > 0 && !movingUp){
      (goal)--;
    }
  }
}

// Turns on this many lights from the bottom instantly
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

/* turns entire strip on a specific color and highest brightness */
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
