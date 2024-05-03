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
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"

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
IoTTimer sessionTimer, inputTimer;
int neoNum, currentTime, lastTime, i, j, k;
bool onOff, endSesh;
bool sleepFlag = false;
int encPos, encToPixel, encMin, encMax, pixelMin, pixelMax, holdPixel;
int dipValue, dipOne, dipTwo, dipThree, dipFour;
String DateTime, TimeOnly, DateOnly, SessionName;
int breathingArray[16] = {19, 14, 9, 4, 3, 2, 1, 0, 5, 10, 15, 20, 21, 22, 23, 24};
int breathingColors[4] = {blue, teal, blue, teal};
int outsideSquare[16] = {19, 14, 9, 4, 3, 2, 1, 0, 5, 10, 15, 20, 21, 22, 23, 24};
int middleSquare[8] = {6, 7, 8, 11, 13, 16, 17, 18};
int centerSquare[1] = {12};
int centeringDelays[18] = {200, 350, 500, 650, 800, 950, 1100, 1250, 1400, 1550, 1700, 1850, 2000, 2200, 2400, 2600, 2800, 3000};
int racingNumbers[25] = {4, 3, 2, 1, 0, 5, 6, 7, 8, 9, 14, 13, 12, 11, 10, 15, 16, 17, 18, 19, 24, 23, 22, 21, 20};
int motivationNumbers[25] = {24,19,14,9,4,3,8,13,18,23,22,17,12,7,2,1,6,11,16,21,20,15,10,5,0};
int happyFace[9] = {3, 8, 1, 6, 14, 18, 17, 16, 10};

enum miniPixel {
  OUTER,
  MIDDLE,
  CENTER
};
miniPixel centering;

void makeSelection();
void pickOne();
void readySetGo();
void boxBreathing();
void findCenter();
void racingThoughts();
void maxMotivation();
void endSession();
void MQTT_connect();
bool MQTT_ping();
void publishValues();
void buttonInterrupt();
void sleepULP();
/************ Global State (you don't need to change this!) ***   ***************/ 
TCPClient TheClient; 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 
/****************************** Feeds ***************************************/ 
// Setup Feeds to publish or subscribe 
Adafruit_MQTT_Publish pubDateFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/todaysdate");
Adafruit_MQTT_Publish pubTimeFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/currenttime");
Adafruit_MQTT_Publish pubSessionFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sessiontype");

// setup() runs once, when the device is first turned on
void setup() {
    WiFi.connect();
  while (WiFi.connecting())
  {
    Serial.printf(",");
  }
Particle.connect();
  delay(10000);
  Time.zone(-7);
  Particle.syncTime();
Serial.begin(9600);
waitFor(Serial.isConnected, 10000);
pinMode (D13, INPUT);
attachInterrupt(D13, endSession, FALLING);
pinMode (D14, INPUT);
attachInterrupt(D14, endSession, FALLING);
pinMode (D18, INPUT);
attachInterrupt(D18, endSession, FALLING);
pinMode (D19, INPUT);
attachInterrupt(D19, endSession, FALLING);
pinMode (D16, INPUT);
attachInterrupt(D16, buttonInterrupt, FALLING);
  pixel.begin();
  pixel.setBrightness (BRI);
  pixel.clear();
  pixel.show();
  neoNum = 0;
  i = 0;
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
dipValue = 0;
endSesh = true;
lastTime = millis();
sessionTimer.startTimer(60000); //move into loop once modes fixed
}

void loop() {
MQTT_connect();
MQTT_ping();
DateTime = Time.timeStr();
TimeOnly = DateTime.substring(11, 19);
DateOnly = DateTime.substring(3,10);
// Serial.printf("Time is %s\n", TimeOnly.c_str());
// Serial.printf("Date is %s\n", DateOnly.c_str());
dipOne = digitalRead(D13);
dipTwo = digitalRead(D14);
dipThree = digitalRead(D19);
dipFour = digitalRead(D18);
dipValue = dipFour << 3 | dipThree << 2 | dipTwo << 1 | dipOne;
//Serial.printf("dipOne = %i, dipTwo = %i, dipThree = %i, dipFour = %i\n", dipOne, dipTwo, dipThree, dipFour);
Serial.printf("Decision value = %i\n", dipValue);
delay(3000);

switch (dipValue) {
  case 0:
  makeSelection();
  break;

  case 1:
  boxBreathing();
  break;

  case 2:
  findCenter();
  break;

  case 4:
  racingThoughts();
  break;

  case 8:
  maxMotivation();
  break;

  default:
  pickOne();
  break;
}
 if (sleepFlag){
  Serial.printf("Going to SLeep\n");
  sleepFlag = false;
  sleepULP();
 }

}

void MQTT_connect() {
  int8_t ret;
 
  // Return if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.printf("Error Code %s\n",mqtt.connectErrorString(ret));
       Serial.printf("Retrying MQTT connection in 5 seconds...\n");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds and try again
  }
  Serial.printf("MQTT Connected!\n");
}

bool MQTT_ping() {
  static unsigned int last;
  bool pingStatus;

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      pingStatus = mqtt.ping();
      if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
  return pingStatus;
}

void endSession() {
 endSesh = false;
}

void makeSelection() {
  display.clearDisplay();
  pixel.clear();
  pixel.show();
  display.setCursor(0,0);
  display.printf("Select\n");
  display.printf("Mode\n");
  display.display();
}

void pickOne(){
  pixel.clear();
  pixel.show();
  display.clearDisplay();
  display.setCursor(0,0);
  display.printf("Pick Only\n");
  display.printf("One\n");
  display.display();
}

void readySetGo() {     // ready set go green light
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
  if (!endSesh){
    publishValues();
    return;
   }
}

void boxBreathing() {
  // box breathing, pixel moves every 1 second
SessionName = "Box Breathing";
endSesh = true;
neoNum = 0;
display.clearDisplay();
display.setCursor(0,0);
display.printf("Box\n"); //OLED screen
display.printf("Breathing\n");
display.display();
delay(500);
display.clearDisplay();
sessionTimer.startTimer(60000);
readySetGo();
display.display();
while(!sessionTimer.isTimerReady()){
//currentTime = millis();
if (millis() - lastTime > 1000) {
  pixel.setPixelColor(breathingArray[neoNum], breathingColors[neoNum/4]);
  pixel.show();
    if ((neoNum % 4) == 3) {
    pixel.clear();
    delay(500);
    pixel.show();
    }
  neoNum = neoNum + 1;
lastTime = millis();
          if (!endSesh){
            publishValues();
      return;
      }
  }
    if (neoNum == 16){
      neoNum = 0;
    }
  }

if (sessionTimer.isTimerReady()){
  pixel.clear();
for (i=0; i<9; i++) {
  pixel.setPixelColor(happyFace[i],orange);
}
  pixel.show();
  display.setCursor(0,0);
  display.printf("Feeling\n");
  display.printf("Better?\n");
  display.display();
  inputTimer.startTimer(3000);
  if (inputTimer.isTimerReady()){
  display.clearDisplay();
  pixel.clear();
  pixel.show();
  display.display();
  }
  if (!endSesh){
    publishValues();
    return;
  }
}
}

void findCenter() {
//  Centering Squares
SessionName = "Finding Center";
endSesh = true;
display.clearDisplay();
  display.setCursor(0,0);
display.printf("Find Your\n"); //OLED screen
display.printf("Center\n");
display.display();
delay(500);
display.clearDisplay();
centering = OUTER;
i=0;
//j=0;
sessionTimer.startTimer(60000);
readySetGo();
display.display();
while(!sessionTimer.isTimerReady()){
 if ((millis() - lastTime) > centeringDelays[i%18]){
    lastTime = millis();
    switch (centering) {
      case OUTER:
        pixel.clear();
        for (k = 0; k<16; k++) {
        pixel.setPixelColor(outsideSquare[k], carrot);
         }
        pixel.show();
          if (!endSesh){
          publishValues();
          return;
        }
        centering = MIDDLE;
        break;
      
      case MIDDLE:
      pixel.clear();
        for (k = 0; k<8; k++) {
        pixel.setPixelColor(middleSquare[k], chocolate);
         }
        pixel.show();
                  if (!endSesh){
          publishValues();
          return;
        }
        centering = CENTER;
        break;
      
      case CENTER:
      pixel.clear();
        for (k = 0; k<1; k++) {
        pixel.setPixelColor(centerSquare[k], yellow);
        }
        pixel.show();
                  if (!endSesh){
          publishValues();
          return;
        }
        centering = OUTER;
        i++;
        break;
    }
  }
}

if (sessionTimer.isTimerReady()){
  pixel.clear();
for (i=0; i<9; i++) {
  pixel.setPixelColor(happyFace[i],orange);
}
  pixel.show();
  display.setCursor(0,0);
  display.printf("Feeling\n");
  display.printf("Better?\n");
  display.display();
  inputTimer.startTimer(3000);
  if (inputTimer.isTimerReady()){
  display.clearDisplay();
  pixel.clear();
  pixel.show();
  display.display();
  }
  if (!endSesh){
    publishValues();
    return;
  }
}
}

void racingThoughts() {
  SessionName = "Racing Thoughts";
  endSesh = true;
display.clearDisplay();
display.setCursor(0,0);
display.printf("Slow\n"); //OLED screen
display.printf("Down\n");
display.display();
delay(500);
readySetGo();
sessionTimer.startTimer(60000);
display.clearDisplay();
display.display();
i = 0;
while(!sessionTimer.isTimerReady()){
 if ((millis() - lastTime) > centeringDelays[i]){
 // Serial.printf("Racing delay is %ims\n", centeringDelays[i]);
  lastTime = millis();
encPos = myEnc.read();
//Serial.printf("encoder position is %i\n", encPos);
encToPixel = map(encPos, 0, 95, 0, 24);
    if (encToPixel != holdPixel) {
      pixel.clear();
      pixel.show();
    }
      if (!endSesh){
    publishValues();
    return;
  }
// if (encPos <= encMin) {
//   encPos = encMax;
//   myEnc.write(encMax);
// }
    if (encPos >= encMax) {
      encPos = encMin;
      myEnc.write(encMin);
    }
  pixel.clear();
  pixel.setPixelColor(racingNumbers[encToPixel],red);
    holdPixel = encToPixel;
  pixel.setPixelColor(racingNumbers[neoNum], purple);
    if (encToPixel == neoNum){
      pixel.setPixelColor(racingNumbers[neoNum], green);
    }
  pixel.show();

    if (neoNum == 24){
      neoNum = 0;
      i++;
      if (i == 18){
        i = 0;
      }
    }
  neoNum = neoNum + 1;
}
}
if (sessionTimer.isTimerReady()){
  pixel.clear();
for (i=0; i<9; i++) {
  pixel.setPixelColor(happyFace[i],orange);
}
  pixel.show();
  display.setCursor(0,0);
  display.printf("Feeling\n");
  display.printf("Better?\n");
  display.display();
  inputTimer.startTimer(3000);
  if (inputTimer.isTimerReady()){
  display.clearDisplay();
  pixel.clear();
  pixel.show();
  display.display();
  }
  if (!endSesh){
    publishValues();
    return;
  }
}
}

void maxMotivation(){
  SessionName = "Max Motivation";
  endSesh = true;
display.clearDisplay();
display.setCursor(0,0);
display.printf("Energy\n"); //OLED screen
display.printf("Boost\n");
display.display();
delay(500);
sessionTimer.startTimer(60000);
readySetGo();
display.clearDisplay();
display.display();
i = 4;
neoNum = 0;
while(!sessionTimer.isTimerReady()){
 if ((millis() - lastTime) > centeringDelays[i]){
 // Serial.printf("Racing delay is %ims\n", centeringDelays[i]);
  lastTime = millis();
encPos = myEnc.read();
//Serial.printf("encoder position is %i\n", encPos);
encToPixel = map(encPos, 0, 95, 0, 24);
    if (encToPixel != holdPixel) {
      pixel.clear();
      pixel.show();
    }
      if (!endSesh){
    publishValues();
    return;
  }
if (encPos <= encMin) {
  encPos = encMax;
  myEnc.write(encMax);
}
    if (encPos >= encMax) {
      encPos = encMin;
      myEnc.write(encMin);
    }
  pixel.clear();
  pixel.setPixelColor(motivationNumbers[encToPixel],turquoise);
    holdPixel = encToPixel;
  pixel.setPixelColor(motivationNumbers[neoNum], magenta);
    if (encToPixel == neoNum){
      pixel.setPixelColor(motivationNumbers[neoNum], green);
    }
  pixel.show();

    if (neoNum == 24){
      neoNum = 0;
      i--;
      if (i == 0){
        i = 6;
      }
    }
  neoNum = neoNum + 1;
}
}
if (sessionTimer.isTimerReady()){
  pixel.clear();
for (i=0; i<9; i++) {
  pixel.setPixelColor(happyFace[i],orange);
}
  pixel.show();
  display.setCursor(0,0);
  display.printf("Feeling\n");
  display.printf("Better?\n");
  display.display();
  inputTimer.startTimer(3000);
  if (inputTimer.isTimerReady()){
  display.clearDisplay();
  pixel.clear();
  pixel.show();
  display.display();
  }
  if (!endSesh){
    publishValues();
    return;
  }
}
}

void publishValues() {
  if(mqtt.Update()) {
     pubDateFeed.publish(DateOnly);
     pubTimeFeed.publish(TimeOnly);
     Serial.printf("publishing Date %s Time %s\n", DateOnly.c_str(), TimeOnly.c_str());
     pubSessionFeed.publish(SessionName);
     Serial.printf("Session is %s\n", SessionName.c_str());
  }
}

void sleepULP() {
  SystemSleepConfiguration config;
  config.mode (SystemSleepMode::ULTRA_LOW_POWER).gpio(D16, FALLING);
  SystemSleepResult result = System.sleep(config);
  delay(1000);
  if (result.wakeupReason () == SystemSleepWakeupReason::BY_GPIO) {
    Serial.printf("Awaked by GPIO %i\n", result.wakeupPin());
  }
}

void buttonInterrupt() {
  sleepFlag = true;
}