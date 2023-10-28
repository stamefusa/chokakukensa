/***************************************************
DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/product-1121.html>
 
 ***************************************************
 This example shows the basic function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
int val = 0, pre_val = 0;
int state = 0;
long state2_start = 0;

void printDetail(uint8_t type, int value);

void setup()
{  
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  pinMode(5, INPUT);
  
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30
}

void loop()
{
  val = digitalRead(5);
  if (val == 1 && pre_val == 0) {
    if (state == 0) { // 開始前に押すと1kHzを流す
      Serial.println("state is 1");
      myDFPlayer.loop(1);
      state = 1;
    } else if (state == 1) { // 4kHzを流す
      Serial.println("state is 2");
      state2_start = millis();
      myDFPlayer.play(2);
      state = 2;
    } else if (state == 2) { // 止める
      Serial.println("state is 0");
      myDFPlayer.pause();
      state = 0;
    }
  }

  // myDFPlayer.loop(2)が動かないので自前で実装する
  if (state == 2 && millis() - state2_start > 3000) {
    Serial.println("Replay 4kHz.");
    state2_start = millis();
    myDFPlayer.play(2);
  }

  pre_val = val;

  delay(100);
}