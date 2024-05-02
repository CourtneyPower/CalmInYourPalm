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
#include "Button.h"
#include "IoTTimer.h"
#include "encoder.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// const int PIXELCOUNT = 25;
const int BRI = 3;
// Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
#define PIXEL_COUNT 25
#define PIXEL_TYPE WS2812B

SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

const int OLED_RESET = -1;
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, PIXEL_TYPE);
Encoder myEnc(D8, D9);
Button encSwitch(D16);
int neoNum, currentTime, lastTime, i, j, k;
bool onOff;
int encPos, encToPixel, encMin, encMax, pixelMin, pixelMax;
IoTTimer sessionTimer;
int breathingArray[16] = {19, 14, 9, 4, 3, 2, 1, 0, 5, 10, 15, 20, 21, 22, 23, 24};
int breathingColors[4] = {blue, teal, blue, teal};
int outsideSquare[16] = {19, 14, 9, 4, 3, 2, 1, 0, 5, 10, 15, 20, 21, 22, 23, 24};
int middleSquare[8] = {6, 7, 8, 11, 13, 16, 17, 18};
int centerSquare[1] = {12};
int centeringDelays[18] = {200, 350, 500, 650, 800, 950, 1100, 1250, 1400, 1550, 1700, 1850, 2000, 2200, 2400, 2600, 2800, 3000};
int racingNumbers[25] = {4, 3, 2, 1, 0, 5, 6, 7, 8, 9, 14, 13, 12, 11, 10, 15, 16, 17, 18, 19, 24, 23, 22, 21, 20};
enum miniPixel {
  OUTER,
  MIDDLE,
  CENTER
};
miniPixel centering;

enum runMode {
  boxBreathing,
  findingCenter,
  racingThoughts,
  maxMotivation,
};
runMode modeProgram;
void readySetGo();

// setup() runs once, when the device is first turned on
void setup() {
Serial.begin(9600);
waitFor(Serial.isConnected, 15000);
  pixel.begin();
  pixel.setBrightness (BRI);
  pixel.clear();
  pixel.show();
  neoNum = 0;

 display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //initialize OLED
 display.display();
 delay(1000);
 display.clearDisplay();
 display.display();
 display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(0,0);
  encPos = 0;
  encMin = 0;
  encMax = 95;
  pixelMin = 0;
  pixelMax = 24;
  lastTime = millis();
  sessionTimer.startTimer(60000);
}

void loop() {

// display.printf("Box\n"); //OLED screen
// display.printf("Breathing\n");
// display.display();
// delay(1000);
// display.clearDisplay();
// display.display();
// // box breathing, pixel moves every 1 second
// readySetGo();
// while(!sessionTimer.isTimerReady()){
// currentTime = millis();
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
// }

// Centering Squares
// display.printf("Find Your\n"); //OLED screen
// display.printf("Center\n");
// display.display();
// delay(1000);
// display.clearDisplay();
// display.display();
// centering = OUTER;
// i=0;
// //j=0;
// readySetGo();
// // sessionTimer.startTimer(60000);
// while(!sessionTimer.isTimerReady()){
//  if ((millis() - lastTime) > centeringDelays[i%18]){
//     lastTime = millis();
//     switch (centering) {
//       case OUTER:
//         pixel.clear();
//         for (k = 0; k<16; k++) {
//         pixel.setPixelColor(outsideSquare[k], carrot);
//          }
//         pixel.show();
//         centering = MIDDLE;
//         break;
      
//       case MIDDLE:
//       pixel.clear();
//         for (k = 0; k<8; k++) {
//         pixel.setPixelColor(middleSquare[k], chocolate);
//          }
//         pixel.show();
//         centering = CENTER;
//         break;
      
//       case CENTER:
//       pixel.clear();
//         for (k = 0; k<1; k++) {
//         pixel.setPixelColor(centerSquare[k], yellow);
//         }
//         pixel.show();
//         centering = OUTER;
//         i++;
//         break;
//     }
//   }
// }

encPos = myEnc.read();
Serial.printf("encoder position is %i\n", encPos);
encToPixel = map(encPos, 0, 95, 0, 24);
if (encPos < encMin) {
  encPos = encMin;
  myEnc.write(encMin);
}
if (encPos > encMax) {
  encPos = encMax;
  myEnc.write(encMax);
}
if (encToPixel < pixelMin) {
  encToPixel = pixelMin;
}
if (encToPixel > pixelMax) {
  encToPixel = pixelMax;
}

pixel.setPixelColor(racingNumbers[encToPixel],red);
pixel.show();
pixel.clear();
pixel.show();



// if (sessionTimer.isTimerReady()){
//   pixel.clear();
//   pixel.show();
//   display.printf("Feel\n");
//   display.printf("Better\n");
//   display.display();
// }
//// if switch is FALLING, transmit date/time/mode to mqtt, go to low power mode (ie: I'm done)
////if no input after 3 seconds, restart session timer for current mode, clear display
// display.clearDisplay();
// display.display();
}

void readySetGo() {
    // ready set go green light
int i;
for (i=0; i<6; i++) {
  if ((i % 2) == 0) {
    pixel.setPixelColor(24, green); //sets pixel #24 to green 
  pixel.show();
  } else {
  pixel.clear();
  pixel.show();
  }
  delay(300);
}
}