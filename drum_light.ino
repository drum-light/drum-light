#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

const int PIXEL_PIN = 6;  // Adafruit output
const int PIEZO_PIN = A2; // Piezo output

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(PIEZO_PIN), turnOff, RISING);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() 
{
  // Read Piezo ADC value in, and convert it to a voltage
  int piezoADC = analogRead(PIEZO_PIN);
//  float piezoV = piezoADC / 1023.0 * 5.0;
  if (piezoADC > 9.0) {
    Serial.println(piezoADC); // Print the voltage.
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    colorWipe(strip.Color(0, 0, 0), 50); // Off
  }
}

void turnOff() {
  Serial.print("turning off");
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(strip.Color(0, 0, 0), 0);
    strip.show();
  }
  strip.show();
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    int piezoADC = analogRead(PIEZO_PIN);
    Serial.println(piezoADC);
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
