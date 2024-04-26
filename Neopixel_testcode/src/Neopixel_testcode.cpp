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
const int BRI = 3;
// Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

#define PIXEL_COUNT 25
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, PIXEL_TYPE);
int neoNum, currentTime, lastTime, i, j, k;
IoTTimer sessionTimer;
int breathingArray[16] = {19, 14, 9, 4, 3, 2, 1, 0, 5, 10, 15, 20, 21, 22, 23, 24};
int breathingColors[4] = {blue, teal, blue, teal};
int outsideSquare[16] = {19, 14, 9, 4, 3, 2, 1, 0, 5, 10, 15, 20, 21, 22, 23, 24};
int middleSquare[8] = {6, 7, 8, 11, 13, 16, 17, 18};
int centerSquare[1] = {12};
int centeringDelays[18] = {200, 350, 500, 650, 800, 950, 1100, 1250, 1400, 1550, 1700, 1850, 2000, 2200, 2400, 2600, 2800, 3000};

enum miniPixel {
  OUTER,
  MIDDLE,
  CENTER
};
miniPixel centering;
// setup() runs once, when the device is first turned on
void setup() {
  pixel.begin();
  pixel.setBrightness (BRI);
  pixel.clear();
  pixel.show();
  neoNum = 0;
  // ready set go
for (i=0; i<6; i++) {
  if ((i % 2) == 0) {
    pixel.setPixelColor(24, green);
  pixel.show();
  } else {
  pixel.clear();
  pixel.show();
  }
  delay(300);
  Serial.printf("i is %i\n", i);
}

  lastTime = millis();
}

void loop() {

// box breathing
currentTime = millis();
// if (currentTime - lastTime > 1000) {
//   pixel.setPixelColor(breathingArray[neoNum], breathingColors[neoNum/4]);
//   pixel.show();
//     if ((neoNum % 4) == 3) {
//     pixel.clear();
//     delay(500);
//     pixel.show();
//     }
//   neoNum = neoNum + 1;
// lastTime = millis();
// }
// if (neoNum == 16){
//   neoNum = 0;
// }

// Centering Squares
centering = OUTER;
i=0;
//j=0;
sessionTimer.startTimer(120000);
while(!sessionTimer.isTimerReady()){
 if ((millis() - lastTime) > centeringDelays[i%18]){
  //if ((millis() - lastTime) > (2000/i)){
    lastTime = millis();
    switch (centering) {
      case OUTER:
        pixel.clear();
        for (k = 0; k<16; k++) {
        pixel.setPixelColor(outsideSquare[k], carrot);
         }
        pixel.show();
        centering = MIDDLE;
        break;
      
      case MIDDLE:
      pixel.clear();
        for (k = 0; k<8; k++) {
        pixel.setPixelColor(middleSquare[k], chocolate);
         }
        pixel.show();
        centering = CENTER;
        break;
      
      case CENTER:
      pixel.clear();
        for (k = 0; k<1; k++) {
        pixel.setPixelColor(centerSquare[k], navy);
        }
        pixel.show();
        centering = OUTER;
        //i--;
        i++;
        break;
    }
  // if (i == 0){
  // i = 10;
  // }
  }
}

}
