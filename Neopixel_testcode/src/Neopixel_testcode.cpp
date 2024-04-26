/* 
 * Project Neopixel test code_capstone
 * Author: Courtney Power
 * Date: 
 * Testing the various neopixel arrays needed for the device
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "neopixel.h"
#include "IoTClassroom_CNM.h"
#include "Colors.h"
#include "IoTTimer.h"

// const int PIXELCOUNT = 25;
const int BRI = 5;
// Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

#define PIXEL_COUNT 25
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, PIXEL_TYPE);
int neoNum, currentTime, lastTime;
IoTTimer boxBreathingTimer;
int breathingArray[16] = {19, 14, 9, 4, 3, 2, 1, 0, 5, 10, 15, 20, 21, 22, 23, 24};

// setup() runs once, when the device is first turned on
void setup() {
  pixel.begin();
  pixel.setBrightness (BRI);
  pixel.clear();
  pixel.show();
  neoNum = 0;
  lastTime = millis();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // box breathing


currentTime = millis();
if (currentTime - lastTime > 1000) {
// boxBreathingTimer.startTimer(1000);
// if (boxBreathingTimer.isTimerReady()) {
  pixel.setPixelColor(breathingArray[neoNum], teal);
  pixel.show();
neoNum = neoNum + 1;
// }
lastTime = millis();
}
if (neoNum == 16) {
  neoNum = 0;
  pixel.clear();
  pixel.show();
}
// for (neoNum = 0; neoNum < PIXEL_COUNT; neoNum ++) {
//   pixel.setPixelColor (neoNum, green);
//   pixel.show();
//   }
//   pixel.clear();
//   pixel.show();
}