#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// 11/20/2015 ML
// Tested with Adafruit NeoPixel Ring-12 and Jewel-7 using Sparkfun Arduino Uno board.
// READ NOTE BELOW ABOUT ADDING RESISTOR.  Works fine with a 470R.

#define LEDS 10
#define PIN 2
// Following added from https://gist.github.com/suhajdab/9716028
#define Pixels 10

// Constructor: Adafruit_NeoPixel(number of LEDs, pin number, LED type)
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// NeoPixel Methods/Functions
// setPixelColor(uint16_t n, uint32_t c) From 'packed' 32-bit RGB color
// setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) Separate RGB components
// setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
// setPin
// 0 = max brightness 1 = min brightness (off), 255 = just below max brightness.
// setBrightness(uint8_t b)
// numPixels(void) const
// Query color from previously-set pixel (returns packed 32-bit RGB value)
// getPixelColor(uint16_t n) const
// Convert separate R,G,B into packed 32-bit RGB color.
// Color(uint8_t r, uint8_t g, uint8_t b)
// Convert separate R,G,B,W into packed 32-bit WRGB color.
// Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w)

// uint_8    1 byte -> [0-255] or [0x00-0xFF]
// uint_16   2 bytes -> [0-65535] or [0x0000-0xFFFF]
// uint32_t  4 bytes -> [0-4294967295] or [0x00000000-0xFFFFFFFF]
//
// 24 bits sent to each LED in string
//

// Following added from https://gist.github.com/suhajdab/9716028
float redStates[Pixels];
float blueStates[Pixels];
float greenStates[Pixels];
float fadeRate = 0.96;


void setup() {

  strip.begin();
  strip.setBrightness(50); //adjust brightness here 0 - 255
  strip.show(); // Initialize all pixels to 'off'

  // Following added from https://gist.github.com/suhajdab/9716028
  for(uint16_t l = 0; l < Pixels; l++) {
    redStates[l] = 0;
    greenStates[l] = 0;
    blueStates[l] = 0;
  }
}


void loop() {
  // Some example procedures showing how to display to the pixels:

  // Send a theater pixel chase in...
  theaterChase(strip.Color(0, 127, 0), 100); // Green
  theaterChase(strip.Color(127, 0, 0), 100); // Red
  //theaterChase(strip.Color(127, 127, 127), 100); // White
  //theaterChase(strip.Color(0, 0, 127), 100); // Blue
  
  // Turn on LEDs one at a time 
  colorWipe(strip.Color(0, 255, 0), 250); // Green
  colorWipe(strip.Color(255, 0, 0), 250); // Red
  colorWipe(strip.Color(0, 255, 0), 250); // Green
  //Twinkle (wait)
  for (uint16_t k=0; k<2000; k++) { twinkle(20); }
    delay(1000);
  backcolorWipe(strip.Color(255, 0, 0), 250); // Red
  backcolorWipe(strip.Color(0, 255, 0), 250); // Green
  colorWipe(strip.Color(192, 192, 192), 250); // White
    delay(5000);
  backcolorWipe(strip.Color(255, 0, 0), 100); // Red
 //colorWipe(strip.Color(0, 0, 255), 80); // Blue
  //colorWipe(strip.Color(255, 0, 0), 80); // Red
  //colorWipe(strip.Color(0, 255, 0), 80); // Green
  //colorWipe(strip.Color(0, 0, 255), 80); // Blue
  
  // All LEDs cycled together through the rainbow
  rainbow(100);
  rainbow(100);

 // colorWipe(strip.Color(255, 0, 0), 80); // Red
 // colorWipe(strip.Color(0, 255, 0), 80); // Green

  // reverserainbow(50);
  
  // Theater Chase, cycle through the colors
  theaterChaseRainbow(40);
  // Cycle a rainbow of colors through the LEDs
  //rainbowCycle(10);

  // Modulate brightness of a Single Color
  //singlecolor(50);

  // Modulate brightness between Red & Green
  dualcolor(50);

  // Cycle a rainbow of colors through the LEDs
  rainbowCycle(50);

  // number, twinkle color, background color, delay
//  twinkleRand(12,strip.Color(255,0,0),strip.Color(0,255,0),1000);

}
  
//////////////////////////////////////////////////////

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint8_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color, backwards
void backcolorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=strip.numPixels(); i+1>0;  i-- ) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


// All on, glide through rainbow of colors
void rainbow(uint8_t wait) {
  for(uint16_t j=0; j<256; j++) {    // j is the color loop
    for(uint8_t i=0; i<strip.numPixels(); i++) {  // i is the LED loop
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// All on, reverse glide through rainbow of colors
// Something wrong - first LED not correct
void reverserainbow(uint8_t wait) {
  for(uint16_t j=0; j<256; j++) {    // j is the color loop
    for(uint16_t i=strip.numPixels(); i>0; i--) {  // i is the LED loop
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  for(uint16_t j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(uint8_t i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (uint8_t j=0; j<10; j++) {  //do 10 cycles of chasing
    for (uint8_t q=0; q < 3; q++) {
      for (uint8_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint8_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (uint16_t j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (uint8_t q=0; q < 3; q++) {
      for (uint8_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint8_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Color brightness loop - Red - ML
void singlecolor(uint8_t wait) {
    for (uint16_t j=0; j<256; j++) {
      for (uint8_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(j,0,0));
        strip.show();
    }
    delay(wait);
  }
}

// Dual Color brightness loop - ML
void dualcolor(uint8_t wait) {
    for (uint16_t j=1; j<256; j=j+4) {
      for (uint8_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(256-j,j,0));
        strip.show();
    }
    delay(wait);
  }
  delay(10000);
    for (uint16_t j=1; j<256; j=j+4) {
      for (uint8_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(j,256-j,0));
        strip.show();
    }
    delay(wait);
  }
  delay(10000);
}


// Following added from https://gist.github.com/suhajdab/9716028
void twinkle (uint16_t wait) {
    if (random(20) == 1) {
      uint16_t i = random(Pixels);
      if (redStates[i] < 1 && greenStates[i] < 1 && blueStates[i] < 1) {
        redStates[i] = random(256);
        greenStates[i] = random(256);
        blueStates[i] = random(256);
      }
    }
    for(uint16_t l = 0; l < Pixels; l++) {
      if (redStates[l] > 1 || greenStates[l] > 1 || blueStates[l] > 1) {
        strip.setPixelColor(l, redStates[l], greenStates[l], blueStates[l]);
        
        if (redStates[l] > 1) {
          redStates[l] = redStates[l] * fadeRate;
        } else {
          redStates[l] = 0;
        }
        
        if (greenStates[l] > 1) {
          greenStates[l] = greenStates[l] * fadeRate;
        } else {
          greenStates[l] = 0;
        }
        
        if (blueStates[l] > 1) {
          blueStates[l] = blueStates[l] * fadeRate;
        } else {
          blueStates[l] = 0;
        }
        
      } else {
        strip.setPixelColor(l, 0, 0, 0);
      }
    }
    strip.show();
    delay(wait);
}

// number, twinkle color, background color, delay
// twinkleRand(5,strip.Color(255,255,255),strip.Color(255, 0, 100),90);

// twinkle random number of pixels
void twinkleRand(int num,uint32_t c,uint32_t bg,int wait) {
  // set background
   stripSet(bg,0);
   // for each num
   for (int i=0; i<num; i++) {
     strip.setPixelColor(random(strip.numPixels()),c);
   }
  strip.show();
  delay(wait);
}

// quickly set the entire strip a color
void stripSet(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  // move the show outside the loop
  strip.show();
  delay(wait);
}
