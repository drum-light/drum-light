#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Constants
const int PIXEL_PIN = 6;  // Adafruit output
const int DRUM1_PIN = A2; // Piezo output
const int DRUM1_MAX_PIXELS = 12; // Max LEDs on a Pixel

// Globals
int drum1LightCurrent = 0;
int drum1LightGoal = 0;
int drum1ADC;
int drum1Previous = 0;

unsigned long previousMillis = 0; // last time update
long interval = 20; // interval at which to do something (milliseconds)

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel drum1LightStrip = Adafruit_NeoPixel(12, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(DRUM1_PIN), turnOff, RISING);
  drum1LightStrip.begin();
  //colorWipe(drum1LightStrip.Color(0, 0, 255), 50);
  drum1LightStrip.show(); // Initialize all pixels to 'off'
}

void loop() 
{
  unsigned long currentMillis = millis();
  // Read Drum Sensors ADC value in, and convert them to voltages
  drum1ADC = analogRead(DRUM1_PIN);
  
  //  float piezoV = piezoADC / 1023.0 * 5.0;

  // Sensing if drums are hit
  if (drum1ADC > 30 && drum1ADC > drum1Previous && (drum1ADC - drum1Previous) > 30) {
     // Print the voltage.
    Serial.println(drum1ADC);
    drum1LightGoal += 1;
    if (drum1LightGoal > DRUM1_MAX_PIXELS) {
      drum1LightGoal = DRUM1_MAX_PIXELS;
    }

    
    //colorWipe(drum1LightStrip.Color(0, 0, 255), 50); // Blue
    //colorWipe(drum1LightStrip.Color(0, 0, 0), 50); // Off
  }
  drum1Previous = drum1ADC;

  // Handle lights changing
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    bool movingUp = false;

    // Move lights towards goal
    if (drum1LightCurrent < drum1LightGoal) {
      turnLightsOnFromBottom(1, drum1LightCurrent, &drum1LightStrip);
      drum1LightCurrent += 1;
      movingUp = true;
    } else if (drum1LightCurrent > drum1LightGoal) {
      //Serial.println(drum1LightCurrent);
      turnLightsOffFromTop(1, drum1LightCurrent, &drum1LightStrip);
      drum1LightCurrent -= 1;
    }

    if (drum1LightGoal > 0 && !movingUp) {
        drum1LightGoal--;
      }

    
    
    // Start reducing lights
    
  }
  
}

// Turns on this many lights from the bottom insantly
void turnLightsOnFromBottom(uint32_t numToTurnOn, int currentAmountOn, Adafruit_NeoPixel* strip) {
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
    strip->setPixelColor(i, strip->Color(0, 0, 255));
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
    strip->setPixelColor(i, strip->Color(0, 0, 0));
  }

//  for (uint16_t i=currentAmountOn; i<=goal; i++) {
//    Serial.println("current");
//    Serial.println(i);
//    strip->setPixelColor(strip->numPixels() - i, strip->Color(0, 0, 255));
//  }
  strip->show();
}

void turnOff() {
  for(uint16_t i=0; i<drum1LightStrip.numPixels(); i++) {
    drum1LightStrip.setPixelColor(drum1LightStrip.Color(0, 0, 0), 0);
    drum1LightStrip.show();
  }
  drum1LightStrip.show();
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<drum1LightStrip.numPixels(); i++) {
    int piezoADC = analogRead(DRUM1_PIN);
    drum1LightStrip.setPixelColor(i, c);
    drum1LightStrip.show();
    delay(wait);
  }
}
