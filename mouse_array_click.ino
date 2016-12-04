//#if ARDUINO > 10605
#include <Mouse.h>
//#endif //ARDUINO > 10605
#include <SPI.h>
//#include <MouseTo.h>
#include <RF24.h>
#include <NRF24L01.h>

bool ended = false;

float previousXcoordinate = 0;
float previousXpoint = 0;
float previousYcoordinate = 0;
float previousYpoint = 0;
float previousZXcoordinate = 0;
float previousZYcoordinate = 0;
unsigned long Clock = 0;
unsigned long lastTouchTime = 0;
unsigned long lastReceivedTime = 0;
unsigned long startTouchTime = 0;
unsigned long lastYtime = 0;
unsigned long lastXtime = 0;
int notReceived = 0;
int Click = 0;

int xArray[] = {0,0,0};
int yArray[] = {0,0,0};

RF24 radio(9,10);
const uint64_t rAddress[] = {0xD31373D0A1LL, 0xD31373D0B2LL, 0xD313373D0C3LL, 0xD31373D0D4LL};

void setup() {
  //MouseTo.setCorrectionFactor(1);
  //MouseTo.setScreenResolution(1600, 900);
  Serial.begin(9600);
  while(!Serial){}
  radio.begin();
  radio.setPALevel (RF24_PA_LOW); 
  radio.openReadingPipe(1, rAddress[3]);
  radio.openReadingPipe(2, rAddress[0]);
  radio.openReadingPipe(3, rAddress[1]);
  radio.openReadingPipe(4, rAddress[2]);

  radio.startListening();

}

  float xMoveFactor = 10.229167;
  float yMoveFactor = 5.9166667;
  float xScaleRes = .3117143;
  float yScaleRes = .3155556;
  int xMaxScreen = 491;
  int yMaxScreen = 284;

void loop() {
  Clock = millis();
  bool nRecord = false;
  float rByte[] = {0,0};
  float gotByteX[] = {0,0};
  float gotByteY[] = {0,0};
  float xMove = 0;
  float yMove = 0;
  float zxMove = 0;
  float zyMove = 0;
  float xMoveTimes = 0;
  float yMoveTimes = 0;
  float yCenter = 24.00; //use it for the movement of the first touch
  float xCenter = 24.00;
  float xmove = 0;
  float ymove = 0;
  bool lClick = false;
  bool rClick = false;
  byte pipeID = 0;
  float xPoint = 0;
  float yPoint = 0;

if(lastReceivedTime + 1000<= Clock && lastReceivedTime > 0){ // time adjusts period between abs movements when finger is lifted
  previousXpoint = 0;
  previousYpoint = 0;
  
  xArray[0] = 0;
  xArray[1] = 0;
  xArray[2] = 0;
  yArray[0] = 0;
  yArray[1] = 0;
  yArray[2] = 0;
}
 
  if(radio.available(&pipeID)){
    lastReceivedTime = millis();
    //stopRefresh = true;
      radio.read(rByte, sizeof(rByte));
    if(previousYpoint > 0 && Clock < lastTouchTime + 120){
      if(rByte[1] == 2){
    if(rByte[0] <= previousYpoint - 3 || rByte[0] >= previousYpoint + 3 && previousYpoint != 0){
      nRecord = true;
    }
      }
      if(rByte[1] == 1){
      if(rByte[0] <= previousXpoint - 3 || rByte[0] >= previousXpoint + 3 && previousXpoint != 0){
      nRecord = true;
    }
      }
   }

   
      if (rByte[1] == 1){ 
  gotByteX[0] = rByte[0];
   gotByteX[1] = rByte[1];
   if(xArray[0] != 0 && xArray[1] != 0 && xArray[2] == 0 && nRecord == false){
    xArray[2] = rByte[0];
    //stopRefresh = true;
    xPoint = (xArray[2]  + xArray[1] + xArray[0])/3;
    xmove = xPoint * xMoveFactor;
    xMoveTimes = xmove/127;
   }
   if(xArray[0] != 0 && xArray[1] == 0 && xArray[2] == 0 && nRecord == false){
    xArray[1] = rByte[0];
    //stopRefresh = true;
   }
   if(xArray[0] == 0 && xArray[1] == 0 && xArray[2] == 0 && nRecord == false){
    xArray[0] = rByte[0];
    //stopRefresh = true;
   }
   if(xmove != 0){
         Serial.println();
         Serial.print("got X coordinate: ");
         Serial.print(xmove);
         Serial.println();
         Serial.print("xMovetimes: ");
         Serial.print(xMoveTimes);
   }
      } 
      
      if (rByte[1] == 2){
        
   gotByteY[0] = rByte[0];
   gotByteY[1] = rByte[1];
   if(yArray[0] != 0 && yArray[1] != 0 && yArray[2] == 0 && nRecord == false){
    yArray[2] = rByte[0];
    //stopRefresh = true;
    yPoint = (yArray[2]  + yArray[1] + yArray[0])/3;
    ymove = (yPoint * yMoveFactor) * -1;
    yMoveTimes = abs(ymove)/127;
   }
   if(yArray[0] != 0 && yArray[1] == 0 && yArray[2] == 0 && nRecord == false){
    yArray[1] = rByte[0];
    //stopRefresh = true;
   }
   if(yArray[0] == 0 && yArray[1] == 0 && yArray[2] == 0 && nRecord == false){
    yArray[0] = rByte[0];
    //stopRefresh = true;
   }

if(ymove != 0){
        Serial.println();
        Serial.print("got Y coordinate: ");
        Serial.print(ymove);
        Serial.println();
        Serial.print("yMovetimes: ");
        Serial.print(yMoveTimes);
}

  }
    if(ymove != 0 && Clock < lastYtime + 100 && lastYtime != 0){
    if(ymove >= previousYcoordinate + 12){
      ymove = 0;
    }
    if(ymove <= previousYcoordinate - 12){
      ymove = 0;
    }
  }
  if(xmove != 0 && Clock < lastXtime + 100 && lastXtime != 0){
    if(xmove >= previousXcoordinate + 22.5){
      xmove = 0;
    }
     if(xmove <= previousXcoordinate  - 22.5){
      xmove = 0;
    }
  }
  if(ymove != 0){
    lastYtime = millis();
  }
  if(xmove != 0){
    lastXtime = millis();
  }
  if(ymove != 0 || xmove != 0){
      lastTouchTime = millis();
  }
  }
   else{
    if(notReceived > 100000){
      notReceived = 0;
    }
    notReceived++;
   }

if(notReceived >= 3 && startTouchTime != 0){ //if after 3 cycles there are no touches
  //Mouse.begin();
  Mouse.release(MOUSE_LEFT);
  Mouse.release(MOUSE_RIGHT);
if(startTouchTime > 0){
  if(Clock - startTouchTime <= 120 && Clock - startTouchTime > 50){ // click if the touch was between 50-120 MS
    Mouse.click(MOUSE_LEFT);
    Mouse.end();
    Click = 1;
    startTouchTime = 0;
  }
}
previousXpoint = 0;
previousYpoint = 0;
  xArray[0] = 0;
  xArray[1] = 0;
  xArray[2] = 0;
  yArray[0] = 0;
  yArray[1] = 0;
  yArray[2] = 0;
}

  if(xmove > 0.00){
    if(previousXpoint == 0){
  Mouse.begin();
   startTouchTime = millis();
  if (ended == true){
Serial.println();
Serial.print(F("Starting X mouse control!"));
}
for (int a = 0; a < 9; a++){
Mouse.move(-127, 0, 0); 
}
if(xMoveTimes >= 1){
Mouse.move(127,0, 0);
if(xMoveTimes < 2 && xMoveTimes != 1){
Mouse.move(xmove - 127, 0, 0);
}
  if(xMoveTimes >= 2){
 Mouse.move(127,0, 0);
 if(xMoveTimes < 3 && xMoveTimes != 2){
  Mouse.move(xmove - 254, 0, 0);
 }
  }
  if(xMoveTimes >= 3){
 Mouse.move(127,0, 0);
 if(xMoveTimes > 4 && xMoveTimes != 4){
  Mouse.move(xmove - 381, 0, 0);
 }
  }
}
  
if(xMoveTimes < 1){
Mouse.move(xmove,0, 0);
}  
    }
    if(previousXpoint > 0){
    Mouse.begin();
      float XcoordChange = (xPoint - previousXpoint) * (xMoveFactor + 1.5);
      float coordMoveTimes = XcoordChange/ 127;
if(XcoordChange > 0){ //moving right positive values     
      if(coordMoveTimes < 1){
        Mouse.move(XcoordChange, 0, 0);
      }
      if(coordMoveTimes >= 1){
Mouse.move(127,0, 0);
if(coordMoveTimes < 2 && coordMoveTimes != 1){
Mouse.move(XcoordChange - 127, 0, 0);
}
  if(coordMoveTimes >= 2){
 Mouse.move(127,0, 0);
 if(coordMoveTimes< 3 && coordMoveTimes != 2){
  Mouse.move(XcoordChange - 254, 0, 0);
 }
  }
  if(coordMoveTimes>= 3){
 Mouse.move(127,0, 0);
 if(coordMoveTimes > 4 && coordMoveTimes != 4){
  Mouse.move(XcoordChange - 381, 0, 0);
 }
  }
}
    }
 if(XcoordChange < 0){ //moving left negative values     
      if(coordMoveTimes > -1){
        Mouse.move(XcoordChange, 0, 0);
      }
      if(coordMoveTimes <= -1){
Mouse.move(-127,0, 0);
if(coordMoveTimes > -2 && coordMoveTimes != -1){
Mouse.move(XcoordChange + 127, 0, 0);
}
  if(coordMoveTimes <= -2){
 Mouse.move(-127,0, 0);
 if(coordMoveTimes > -3 && coordMoveTimes != -2){
  Mouse.move(XcoordChange + 254, 0, 0);
 }
  }
  if(coordMoveTimes <= -3){
 Mouse.move(-127,0, 0);
 if(coordMoveTimes > -4 && coordMoveTimes != -4){
  Mouse.move(XcoordChange + 381, 0, 0);
 }
  }
}
    }
    }

   previousXcoordinate = xmove;
   previousXpoint = xPoint;
xArray[0] = 0;
xArray[1] = 0;
xArray[2] = 0;
}

if(ymove < 0.00){
  if(previousYpoint == 0){
    startTouchTime = millis();
  Mouse.begin();
  if (ended == true){
Serial.println();
Serial.print(F("Starting Y mouse control!"));
}
for (int a = 0; a < 9; a++){
Mouse.move(0, 127, 0); 
}
if(yMoveTimes >= 1){
  
Mouse.move(0,-127, 0);
if(yMoveTimes < 2 && yMoveTimes != 1){
Mouse.move(0, ymove + 127, 0);
}
  if(yMoveTimes >= 2){
 Mouse.move(0, -127, 0);
 if(yMoveTimes < 3 && yMoveTimes != 2){
  Mouse.move(0, ymove + 254, 0);
 }
  }
  if(yMoveTimes >= 3){
 Mouse.move(0, -127, 0);
 if(yMoveTimes > 4 && yMoveTimes != 3){
  Mouse.move(0, ymove + 381, 0);
 }
  }
}
  
if(yMoveTimes < 1){
Mouse.move(0, ymove, 0);
}
Mouse.press(MOUSE_LEFT);
 }

    if(previousYpoint > 0){
    Mouse.begin();
      float YcoordChange = (yPoint - previousYpoint) * (yMoveFactor + 3);
      float coordMoveTimes = YcoordChange/ 127;
if(YcoordChange < 0){      
      if(coordMoveTimes < 1){
        Mouse.move(0, abs(YcoordChange), 0);
      }
      if(coordMoveTimes >= 1){
Mouse.move(127,0, 0);
if(coordMoveTimes < 2 && coordMoveTimes != 1){
Mouse.move(0, abs(YcoordChange) - 127, 0);
}
  if(coordMoveTimes >= 2){
 Mouse.move(127,0, 0);
 if(coordMoveTimes< 3 && coordMoveTimes != 2){
  Mouse.move(0, abs(YcoordChange) - 254, 0);
 }
  }
  if(coordMoveTimes >= 3){
 Mouse.move(127,0, 0);
 if(coordMoveTimes > 4 && coordMoveTimes != 4){
  Mouse.move(0, abs(YcoordChange) - 381, 0);
 }
  }
}
    }
 if(YcoordChange > 0){      
      if(coordMoveTimes > -1){
        Mouse.move( 0, -YcoordChange, 0);
      }
      if(coordMoveTimes <= -1){
Mouse.move(0,-127, 0);
if(coordMoveTimes > -2 && coordMoveTimes != -1){
Mouse.move( 0, (YcoordChange + 127) * - 1, 0);
}
  if(coordMoveTimes <= -2){
 Mouse.move(0, -127, 0);
 if(coordMoveTimes > -3 && coordMoveTimes != -2){
  Mouse.move( 0, (YcoordChange + 254) * - 1, 0);
 }
  }
  if(coordMoveTimes <= -3){
 Mouse.move(0, -127, 0);
 if(coordMoveTimes > -4 && coordMoveTimes != -4){
  Mouse.move( 0, (YcoordChange + 381) * -1, 0);
 }
  }
}
    }
    }
   
  previousYcoordinate = ymove;
  previousYpoint = yPoint;
yArray[0] = 0;
yArray[1] = 0;
yArray[2] = 0;
}  

}


