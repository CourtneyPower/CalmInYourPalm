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

const int PIXELCOUNT = 25;
const int BRI = 5;
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
int neoNum;
IoTTimer boxBreathingTimer;

// setup() runs once, when the device is first turned on
void setup() {
  pixel.begin();
  pixel.setBrightness (BRI);
  pixel.clear();
  pixel.show();
  neoNum = 0;
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // box breathing

boxBreathingTimer.startTimer(1000);
if (boxBreathingTimer.isTimerReady()) {
  pixel.setPixelColor(neoNum, teal);
  pixel.show();
neoNum = neoNum + 1;
}
}