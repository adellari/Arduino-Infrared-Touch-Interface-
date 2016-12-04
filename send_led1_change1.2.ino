#include <SPI.h> 
#include<nRF24L01.h> 
#include<RF24.h> 
 
 
//enclosed is the code for multiplexer switching 
byte controlPinsLoneMux[] = {B00000000, //using switched digital pins to set the mux pins from 0-7 //enclosed is the code for multiplexer switching 
                             B10000000, //7.6.5.4.3.2.1.0 
                             B01000000, 
                             B11000000, 
                             B00100000, 
                             B10100000, 
                             B01100000, 
                             B11100000 
                            }; 
int muxValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int muxValuesA[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  1-8 
int muxValuesB[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  9-17 
int muxValuesC[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  18-24 


int muxValuesZ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte muxValuesZA[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  1-8 
byte muxValuesZB[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  9-17 
byte muxValuesZC[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  18-24 

int calValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int calValuesA[] = {0,0,0,0,0,0,0,0};
int calValuesB[] = {0,0,0,0,0,0,0,0};
int calValuesC[] = {0,0,0,0,0,0,0,0};

int calValuesZ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int calValuesZA[] = {0,0,0,0,0,0,0,0};
int calValuesZB[] = {0,0,0,0,0,0,0,0};
int calValuesZC[] = {0,0,0,0,0,0,0,0};

bool lowSensors = false;
float lTouch = 0;
unsigned long lTtime = 0;
unsigned long lTclock = 0;
bool reset = true;

bool lowSensorsZX = false;
float lTouchZX = 0;
unsigned long lTtimeZX = 0;
unsigned long lTclockZX = 0;
bool resetZX = true;
 
int calibrate; 
int buttonPin = 14; 
bool printing; 
int calibrated; 
int eight; 
bool expiredCal; 
bool firstWrng; 
bool autocalibration = false; 
bool push; 
long OutputInterval = 1500; 
unsigned long rStart = 0;
long rIntrvl = 60000;
unsigned long BaseTime; 
unsigned long CurrentTime; 
unsigned long PreviousTime; 
unsigned long expired; 
bool expire; 
bool printrecal; 
unsigned long lastwrng; 
unsigned long calStart; 
unsigned long wrngStrt; 
unsigned int wrngIntrvl = 900000 /*30000*/; 
unsigned int expireIntrvl =  7200000 /*60000*/; 
bool adjusted = false;
unsigned long expireStrt; 
unsigned long expireLst; 
bool aligned; 
bool pushed; 
double IRsetting; 
bool calibrationStored = false;
bool calibratedZ = false;
unsigned long powerOnTime = 0;
long selfAdjTime = 500;
bool selfCal = false;

bool n = false;
bool t = false;
bool c = false;
bool d = false;
bool b = false;
bool z = false;
bool y = false;
bool r = false;

bool cPrinted = true;
bool dPrinted = true;
bool wrote = false;

unsigned long StartTime; 

int LowX = 0;
int Low1X = 0;
int Low2X = 0;
int Low3X = 0;
int Low4X = 0;
int lTrend = 0;
float lastX = 0;
float lastY = 0;

int highTouch1 = 0;
int highTouch2 = 0;
int highTouch3 = 0;
int lowTouch1 = 0;
int lowTouch2 = 0;
int lowTouch3 = 0;
int lastDiff = 0;
int currentDiff = 0;

int LowZX = 0;
int Low1ZX = 0;
int Low2ZX = 0;
int Low3ZX = 0;
int Low4ZX = 0;
int lTrendZX = 0;
float lastZX = 0;

int highTouch1ZX = 0;
int highTouch2ZX = 0;
int highTouch3ZX = 0;
int lowTouch1ZX = 0;
int lowTouch2ZX = 0;
int lowTouch3ZX = 0;
int lastDiffZX = 0;
int currentDiffZX = 0;

/* 
unsigned long twentytwo = 0; 
unsigned long twentythree = 0; 
unsigned long twentyfour = 0; 
unsigned long twentyfive = 0; 
unsigned long twentyone = 0; 
unsigned long twenty = 0; 
unsigned long nineteen = 0; 
unsigned long eighteen = 0; 
unsigned long seventeen = 0; 
*/ 
const int calTime = 2000; 
unsigned long currentCal; 
 
RF24 radio(8,9); 
 
const uint64_t wAddress[] = {0xD31373D0B2LL}; 
 
//const byte rxAddr[6] = "TRAN1"; 
 
void setup() { 
  StartTime = micros(); 
  Serial.begin(9600); 
  radio.begin(); 
  radio.setPALevel (RF24_PA_LOW);
  radio.setRetries(10,8); 
  radio.openWritingPipe(wAddress[0]); 
  radio.stopListening(); 
  DDRD = B11100000; //setting digital pins 6-4 to HIGH 
  BaseTime = millis(); 
  //pinMode(buttonPin, INPUT); 
  //pinMode(6, OUTPUT); 
  //pinMode(5, OUTPUT); 
  //pinMode(4, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(14, OUTPUT);  //receivers
  digitalWrite(14, HIGH); 
  pinMode(15, OUTPUT);  //receivers
  digitalWrite(15, HIGH); 
} 
 
void setPinMux(int outputMuxPin) { 
  PORTD = controlPinsLoneMux[outputMuxPin];       //setting sewdxa{;;e'f}digital 4-6 to follow the truth table instructions 
 
} 

void muxVals() {
  Serial.println();
  Serial.println(F("Values for multiplexer:"));
  Serial.println(F("==========================="));
  for (int a = 0; a < 24; a++) {
    Serial.print(a);
    Serial.print(F(" = "));
    Serial.println(muxValues[a]);
  }
  Serial.println(F("==========================="));
} 

void muxValsZ() {
  Serial.println();
  Serial.println(F("Values for multiplexer Z:"));
  Serial.println(F("==========================="));
  for (int a = 0; a < 24; a++) {
    Serial.print(a);
    Serial.print(F(" = "));
    Serial.println(muxValuesZ[a]);
  }
  Serial.println(F("==========================="));
} 
 
void calVals() { 
  Serial.println(); 
  Serial.println(F("Calibrated Values:")); 
  Serial.println(F("===========================")); 
  for (int a = 0; a < 24; a++) { 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(calValues[a]); 
  } 
  Serial.println(F("===========================")); 
} 

/*void calVals1() { 
  Serial.println(); 
  Serial.println(F("Calibrated Values:")); 
  Serial.println(F("===========================")); 
  for (int a = 0; a < 8; a++) { 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(calValuesA[a]); 
  } 
  Serial.println(F("===========================")); 
} 
 
void calVals2() { 
  for (int b1 = 0; b1 < 8; b1++) { 
  for (int b = 8; b < 17; b++) { 
  if (b == 8 && b1 == 0){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(calValuesB[b1]); 
  } 
  if (b == 9 && b1 == 1){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(calValuesB[b1]); 
  } 
  if (b == 10 && b1 == 2){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(calValuesB[b1]); 
  } 
  if (b == 11 && b1 == 3){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(calValuesB[b1]); 
  } 
  if (b == 12 && b1 == 4){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(calValuesB[b1]); 
  } 
  if (b == 13 && b1 == 5){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(calValuesB[b1]); 
  } 
  if (b == 14 && b1 == 6){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(calValuesB[b1]); 
  } 
  if (b == 15 && b1 == 7){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(calValuesB[b1]); 
  } 
  } 
  }  
     
   
  Serial.println(F("===========================")); 
} 
 
void calVals3() { 
  for (int c1 = 0; c1 < 8; c1++) { 
  for (int c = 16; c < 24; c++) { 
  if (c == 16 && c1 == 0){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(calValuesC[c1]); 
  } 
  if (c == 17 && c1 == 1){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(calValuesC[c1]); 
  } 
  if (c == 18 && c1 == 2){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(calValuesC[c1]); 
  } 
  if (c == 19 && c1 == 3){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(calValuesC[c1]); 
  } 
  if (c == 20 && c1 == 4){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(calValuesC[c1]); 
  } 
  if (c == 21 && c1 == 5){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(calValuesC[c1]); 
  } 
  if (c == 22 && c1 == 6){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(calValuesC[c1]); 
  } 
  if (c == 23 && c1 == 7){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(calValuesC[c1]); 
  } 
  } 
  }  
     
   
  Serial.println(F("===========================")); 
} */
void calMuxDiff() { 
  Serial.println(); 
  Serial.println(F("Current Difference Between Calibrated Values and Current Values:")); 
  Serial.println(F("===========================")); 
 for (int a = 0; a < 24; a++){ 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValues[a] - calValues[a]); 
 }
  Serial.println(F("==========================="));
}

/*void calMuxDiffA() { 
  Serial.println(); 
  Serial.println(F("Current Difference Between Calibrated Values and Current Values:")); 
  Serial.println(F("===========================")); 
 for (int a = 0; a < 8; a++){ 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesA[a] - calValuesA[a]); 
 }
  Serial.println(F("==========================="));
}

void calMuxDiffB(){
 for (int a = 0; a < 8; a++){
  for (int b = 8; b <16; b++){ 
  if ( a == 0 && b == 8){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesB[a] - calValuesB[a]);
  } 
  if ( a == 1 && b == 9){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesB[a] - calValuesB[a]);
  } 
  if ( a == 2 && b == 10){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesB[a] - calValuesB[a]);
  } 
  if ( a == 3 && b == 11){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesB[a] - calValuesB[a]);
  } 
  if ( a == 4 && b == 12){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesB[a] - calValuesB[a]);
  } 
  if ( a == 5 && b == 13){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesB[a] - calValuesB[a]);
  } 
  if ( a == 6 && b == 14){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesB[a] - calValuesB[a]);
  } 
  if ( a == 7 && b == 15){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesB[a] - calValuesB[a]);
  } 
 }
 }
  Serial.println(F("==========================="));
  
}

void calMuxDiffC(){
 for (int a = 0; a < 8; a++){ 
  for (int b = 16; b < 24; b++){
   if ( a == 0 && b == 16){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesC[a] - calValuesC[a]); 
   }
   if ( a == 1 && b == 17){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesC[a] - calValuesC[a]); 
   }
   if ( a == 2 && b == 18){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesC[a] - calValuesC[a]); 
   }
   if ( a == 3 && b == 19){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesC[a] - calValuesC[a]); 
   }
   if ( a == 4 && b == 20){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesC[a] - calValuesC[a]); 
   }
   if ( a == 5 && b == 21){
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesC[a] - calValuesC[a]); 
   }
   if ( a == 6 && b == 22){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesC[a] - calValuesC[a]); 
   }
   if ( a == 7 && b == 23){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesC[a] - calValuesC[a]); 
   }
 }
 }
  Serial.println(F("===========================")); 
}*/

void calMuxDiffZA() { 
  Serial.println(); 
  Serial.println(F("Current Difference Between Calibrated Values and Current ZY Values:")); 
  Serial.println(F("===========================")); 
 for (int a = 0; a < 8; a++){ 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZA[a] - calValuesA[a]); 
 }
  Serial.println(F("===========================")); 
}

void calMuxDiffZB(){
 for (int a = 0; a < 8; a++){
  for (int b = 8; b <16; b++){ 
  if ( a == 0 && b == 8){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 1 && b == 9){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 2 && b == 10){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 3 && b == 11){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 4 && b == 12){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 5 && b == 13){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 6 && b == 14){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
  if ( a == 7 && b == 15){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZB[a] - calValuesZB[a]);
  } 
 }
 }
  Serial.println(F("==========================="));
  
}

void calMuxDiffZC(){
 for (int a = 0; a < 8; a++){ 
  for (int b = 16; b < 24; b++){
   if ( a == 0 && b == 16){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 1 && b == 17){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 2 && b == 18){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 3 && b == 19){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 4 && b == 20){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 5 && b == 21){
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 6 && b == 22){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
   if ( a == 7 && b == 23){
    Serial.print(b); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZC[a] - calValuesZC[a]); 
   }
 }
 }
  Serial.println(F("===========================")); 
}

  void muxValsZ3() { 
  for (int c1 = 0; c1 < 8; c1++) { 
  for (int c = 16; c < 24; c++) { 
  if (c == 16 && c1 == 0){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 17 && c1 == 1){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 18 && c1 == 2){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 19 && c1 == 3){
    Serial.print(F("ZX")); 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 20 && c1 == 4){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 21 && c1 == 5){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 22 && c1 == 6){ 
    Serial.print(F("ZX"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 23 && c1 == 7){
    Serial.print(F("ZX")); 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  } 
  }  
     
   
  Serial.println(F("===========================")); 
} 
 

 
void timeloop(){ 
currentCal = millis(); 
} 

bool calArray (){
unsigned long prodCalA;
unsigned long prodCalB;
unsigned long prodCalC;
LowX = 0;
Low1X = 0;
Low2X = 0;
Low3X = 0;
Low4X = 0;

LowZX = 0;
Low1ZX = 0;
Low2ZX = 0;
Low3ZX = 0;
Low4ZX = 0;

int threesX = 0;
int twoosX = 0;
int onesX = 0;
int zeroesX = 0;

int threesZX = 0;
int twoosZX = 0;
int onesZX = 0;
int zeroesZX = 0;

bool low = true;
bool paired = false;
/*
for (int X=0; x<8; x++){
  if (muxValuesA[x] > 3){
  calValuesA[x] = muxValuesA[x]; 
}
else {
  lowSensors == true;
  if (Low == 0){
    Low = x;
  }
  if (Low != 0){
    Low1 = x;
  }
  if (Low != 0 && Low1 != 0){
    Low2 = x;
  }
  if (Low != 0 && Low1 != 0 && Low2 != 0){
    Low3 = x;
  }
  if (Low != 0 && Low1 != 0 && Low2 != 0 && Low3 != 0){
    Low4 = x;
  }
}
}
*/
 /*Assign Calibration Values A*/

 //calValuesA[0] = muxValuesA[0];
 //calValuesA[1] = muxValuesA[1];
 //calValuesA[2] = muxValuesA[2];
 //calValuesA[3] = muxValuesA[3];
 //calValuesA[4] = muxValuesA[4];
 //calValuesA[5] = muxValuesA[5];
 //calValuesA[6] = muxValuesA[6];
 //calValuesA[7] = muxValuesA[7];

 /*Assign Calibration Values B*/
 //calValuesB[0] = muxValuesB[0];
 //calValuesB[1] = muxValuesB[1];
 //calValuesB[2] = muxValuesB[2];
 //calValuesB[3] = muxValuesB[3];
 //calValuesB[4] = muxValuesB[4];
 //calValuesB[5] = muxValuesB[5];
 //calValuesB[6] = muxValuesB[6];
 //calValuesB[7] = muxValuesB[7];

 /*Assign Calibration Values C*/
 //calValuesC[0] = muxValuesC[0];
 //calValuesC[1] = muxValuesC[1];
 //calValuesC[2] = muxValuesC[2];
 //calValuesC[3] = muxValuesC[3];
 //calValuesC[4] = muxValuesC[4];
 //calValuesC[5] = muxValuesC[5];
 //calValuesC[6] = muxValuesC[6];
 //calValuesC[7] = muxValuesC[7];

/* Assign Calibration Values ZA*/
 //calValuesZA[0] = muxValuesZA[0];
 //calValuesZA[1] = muxValuesZA[1];
 //calValuesZA[2] = muxValuesZA[2];
 //calValuesZA[3] = muxValuesZA[3];
 //calValuesZA[4] = muxValuesZA[4];
 //calValuesZA[5] = muxValuesZA[5];
 //calValuesZA[6] = muxValuesZA[6];
 //calValuesZA[7] = muxValuesZA[7];

 /*Assign Calibration Values ZB*/
 //calValuesZB[0] = muxValuesZB[0];
 //calValuesZB[1] = muxValuesZB[1];
 //calValuesZB[2] = muxValuesZB[2];
 //calValuesZB[3] = muxValuesZB[3];
 //calValuesZB[4] = muxValuesZB[4];
 //calValuesZB[5] = muxValuesZB[5];
 //calValuesZB[6] = muxValuesZB[6];
 //calValuesZB[7] = muxValuesZB[7];

 /*Assign Calibration Values ZC*/
 //calValuesZC[0] = muxValuesZC[0];
 //calValuesZC[1] = muxValuesZC[1];
 //calValuesZC[2] = muxValuesZC[2];
 //calValuesZC[3] = muxValuesZC[3];
 //calValuesZC[4] = muxValuesZC[4];
 //calValuesZC[5] = muxValuesZC[5];
 //calValuesZC[6] = muxValuesZC[6];
 //calValuesZC[7] = muxValuesZC[7];

   
   calValues[0] = muxValues[0];
   calValues[1] = muxValues[1];
   calValues[2] = muxValues[2];
   calValues[3] = muxValues[3];
   calValues[4] = muxValues[4];
   calValues[5] = muxValues[5];
   calValues[6] = muxValues[6];
   calValues[7] = muxValues[7];
   calValues[8] = muxValues[8];
   calValues[9] = muxValues[9];
   calValues[10] = muxValues[10];
   calValues[11] = muxValues[11];
   calValues[12] = muxValues[12];
   calValues[13] = muxValues[13];
   calValues[14] = muxValues[14];
   calValues[15] = muxValues[15];
   calValues[16] = muxValues[16];
   calValues[17] = muxValues[17];
   calValues[18] = muxValues[18];
   calValues[19] = muxValues[19];
   calValues[20] = muxValues[20];
   calValues[21] = muxValues[21];
   calValues[22] = muxValues[22];
   calValues[23] = muxValues[23];

   calValuesZ[0] = muxValuesZ[0];
   calValuesZ[1] = muxValuesZ[1];
   calValuesZ[2] = muxValuesZ[2];
   calValuesZ[3] = muxValuesZ[3];
   calValuesZ[4] = muxValuesZ[4];
   calValuesZ[5] = muxValuesZ[5];
   calValuesZ[6] = muxValuesZ[6];
   calValuesZ[7] = muxValuesZ[7];
   calValuesZ[8] = muxValuesZ[8];
   calValuesZ[9] = muxValuesZ[9];
   calValuesZ[10] = muxValuesZ[10];
   calValuesZ[11] = muxValuesZ[11];
   calValuesZ[12] = muxValuesZ[12];
   calValuesZ[13] = muxValuesZ[13];
   calValuesZ[14] = muxValuesZ[14];
   calValuesZ[15] = muxValuesZ[15];
   calValuesZ[16] = muxValuesZ[16];
   calValuesZ[17] = muxValuesZ[17];
   calValuesZ[18] = muxValuesZ[18];
   calValuesZ[19] = muxValuesZ[19];
   calValuesZ[20] = muxValuesZ[20];
   calValuesZ[21] = muxValuesZ[21];
   calValuesZ[22] = muxValuesZ[22];
   calValuesZ[23] = muxValuesZ[23];

for (int a:calValuesA){
  if (a == 0) ++zeroesX;
  if (a == 1) ++onesX;
  if (a == 2) ++twoosX;
  if (a == 3) ++threesX;
}

for (int b:calValuesB){
  if (b == 0) ++zeroesX;
  if (b == 1) ++onesX;
  if (b == 2) ++twoosX;
  if (b == 3) ++threesX;
}

for (int c:calValuesC){
  if (c == 0) ++zeroesX;
  if (c == 1) ++onesX;
  if (c == 2) ++twoosX;
  if (c == 3) ++threesX;
}

for (int a:calValuesZA){
  if (a == 0) ++zeroesZX;
  if (a == 1) ++onesZX;
  if (a == 2) ++twoosZX;
  if (a == 3) ++threesZX;
}

for (int b:calValuesZB){
  if (b == 0) ++zeroesZX;
  if (b == 1) ++onesZX;
  if (b == 2) ++twoosZX;
  if (b == 3) ++threesZX;
}

for (int c:calValuesZC){
  if (c == 0) ++zeroesZX;
  if (c == 1) ++onesZX;
  if (c == 2) ++twoosZX;
  if (c == 3) ++threesZX;
}
/*
 prodCalA = calValuesA[0] * calValuesA[1] * calValuesA[2] * 
  calValuesA[3] * calValuesA[4] * calValuesA[5] * 
  calValuesA[6] * calValuesA[7];

 prodCalB = calValuesB[0] * calValuesB[1] * calValuesB[2] * calValuesB[3] * 
  calValuesB[4] * calValuesB[5] * calValuesB[6] * 
  calValuesB[7];

prodCalC = calValuesC[0] * calValuesC[1] * calValuesC[2] * 
  calValuesC[3] * calValuesC[4] * calValuesC[5] * 
  calValuesC[6] * calValuesC[7];
*/  
  //if (/*zeroesX +*/ onesX /*+ twoosX + threesX + zeroesZX + onesZX*/ /*+ twoosZX + threesZX*/ == 0){
  calibrationStored = true;
  paired = true;   
  low = false;  
Serial.println();
Serial.print(F("Calibration Successful! "));
 // }
  //else {
    //calibrationStored = false;
   // low = true;
   // paired = false;

Serial.println();
Serial.print("Calibration Failed!");

Serial.println();
Serial.print("There are this many zeroes for X: ");
Serial.print(zeroesX);

Serial.println();
Serial.print("There are this many ones for X: ");
Serial.print(onesX);
    
Serial.println();
Serial.print("There are this many twoos for X: ");
Serial.print(twoosX);
  
Serial.println();
Serial.print("There are this many threes for X: ");
Serial.print(threesX);

Serial.println();
Serial.print("There are this many zeroes for ZX: ");
Serial.print(zeroesZX);

Serial.println();
Serial.print("There are this many ones for ZX: ");
Serial.print(onesZX);
    
Serial.println();
Serial.print("There are this many twoos for ZX: ");
Serial.print(twoosZX);
  
Serial.println();
Serial.print("There are this many threes for ZX: ");
Serial.print(threesZX);
//}
return paired; 
}

void loop() { 

/*if(CurrentTime >= powerOnTime + 20000 && selfCal == false){
  autocalibration = true;
  powerOnTime = millis();
}

if(powerOnTime != 0 && CurrentTime - selfAdjTime >= powerOnTime && selfCal == false){
  r = true;
  selfCal = true;
}*/

  
lTclock = millis();

char input = 'O';
bool written = true;
int ones = 0;
int twoos = 0;
int threes = 0;
int fours = 0;
int fives = 0;
int sixes = 0;
int sevens = 0;
int less1 = 0;
int less2 = 0;
int less3 = 0;
int greater1 = 0;
int greater2 = 0;
int greater3 = 0;

int onesZ = 0;
int twoosZ = 0;
int threesZ = 0;
int foursZ = 0;
int fivesZ = 0;
int sixesZ = 0;
int sevensZ = 0;
int less1Z = 0;
int less2Z = 0;
int less3Z = 0;
int greater1Z = 0;
int greater2Z = 0;
int greater3Z = 0;

float TouchedX = {0};
float TouchedZX = {0};
bool TouchX = false;
bool TouchZX = false;
bool tapped = false;  
unsigned long tappedTime = NULL;  
bool fingerDown = false;  

double X1 = 0;   
double X2 = 0;   
double X3 = 0;  
double X4 = 0;  
double X5 = 0;   
double X6 = 0;  
double X7 = 0;  
double X8 = 0;   
double X9 = 0;  
double X10 = 0;  
double X11 = 0;   
double X12 = 0;  
double X13 = 0;  
double X14 = 0;  
double X15 = 0;  
double X16 = 0;  
double X17 = 0;   
double X18 = 0;  
double X19 = 0;   
double X20 = 0;  
double X21 = 0;   
double X22 = 0;  
double X23 = 0;   
double X24 = 0;  


double ZX1 = 0;   
double ZX2 = 0;   
double ZX3 = 0;  
double ZX4 = 0;  
double ZX5 = 0;   
double ZX6 = 0;  
double ZX7 = 0;  
double ZX8 = 0;   
double ZX9 = 0;  
double ZX10 = 0;  
double ZX11 = 0;   
double ZX12 = 0;  
double ZX13 = 0;  
double ZX14 = 0;  
double ZX15 = 0;  
double ZX16 = 0;  
double ZX17 = 0;   
double ZX18 = 0;  
double ZX19 = 0;   
double ZX20 = 0;  
double ZX21 = 0;   
double ZX22 = 0;  
double ZX23 = 0; 
double ZX24 = 0; 
 
bool Xhalf1 = false;
bool Xhalf2 = false;
bool Xhalf3 = false;
bool Xhalf4 = false;
bool Xhalf5 = false;
bool Xhalf6 = false;
bool Xhalf7 = false;
bool Xhalf8 = false;
bool Xhalf9 = false;
bool Xhalf10 = false;
bool Xhalf11 = false;
bool Xhalf12 = false;
bool Xhalf13 = false;
bool Xhalf14 = false;
bool Xhalf15 = false;
bool Xhalf16 = false;
bool Xhalf17 = false;
bool Xhalf18 = false;
bool Xhalf19 = false;
bool Xhalf20 = false;
bool Xhalf21 = false;
bool Xhalf22 = false;
bool Xhalf23 = false;
bool Xhalf24 = false;

bool ZXhalf1 = false;
bool ZXhalf2 = false;
bool ZXhalf3 = false;
bool ZXhalf4 = false;
bool ZXhalf5 = false;
bool ZXhalf6 = false;
bool ZXhalf7 = false;
bool ZXhalf8 = false;
bool ZXhalf9 = false;
bool ZXhalf10 = false;
bool ZXhalf11 = false;
bool ZXhalf12 = false;
bool ZXhalf13 = false;
bool ZXhalf14 = false;
bool ZXhalf15 = false;
bool ZXhalf16 = false;
bool ZXhalf17 = false;
bool ZXhalf18 = false;
bool ZXhalf19 = false;
bool ZXhalf20 = false;
bool ZXhalf21 = false;
bool ZXhalf22 = false;
bool ZXhalf23 = false;
bool ZXhalf24 = false;

float touchedValuesX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float touchedValuesZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float sendingX[] = {0,0}; 
float sendingZX[] = {0,0};     

 
//char sending = totalmuxC;   
bool sent = false;
 
const char text [] = "Hello World!"; 
byte message = 3; 
 
//if(!radio.write(&message, 1/*sizeof(text)*/));{ 
 // Serial.println(); 
  //Serial.print(F("Failed to send!")); 
//} 
//if(radio.write(&sending, 1/*sizeof(text)*/));{  
  //Serial.println(); 
  //Serial.print(F("Success sending byte!")); 
  //sent = true;
//} 
 
CurrentTime = millis(); 
calibrate = digitalRead(buttonPin); 
     
if (/*calibrate == HIGH ||*/ autocalibration == true){ 
expire = false; 
autocalibration = false; 
calStart = millis(); 
expireStrt = millis(); 
push = true; 
pushed = true; 
expire = false; 
firstWrng = false; 
//digitalWrite(14, LOW); //receivers 
calibrated = 2; 
calArray();
//BaseTime = millis(); 

/*if (!calArray){
  Serial.println();
  Serial.print(F("Sensors are not aligned!"));
}

else {
  Serial.println();
  Serial.print(F("Sensors are aligned!"));
}
*/ 
} 
   
if(calibrate == HIGH){ // added for the recal printing glitch 
 printrecal = false;   
  } 
 
/*if (pushed == true){ 
  timeloop(); 
} 
 
if (currentCal <= calStart + calTime && calibrate != HIGH ){ 
    pushed = false; 
} 
 */
 while (pushed == true){ 
  currentCal = millis(); 
  BaseTime = millis(); 
  //if (printing == false){ 
Serial.flush(); 
Serial.println();   
if (printrecal == false){ 
Serial.print(F("Calibrating..."));  
} 
else { 
  Serial.print(F("Auto-recalibrating...")); 
}  
if (currentCal <= calStart + calTime /*&& calibrate != HIGH*/ ){   
    //digitalWrite(14, HIGH); 
    pushed = false; 
    //digitalWrite(10, LOW); 
    //break;  
      
  } 
 //} 
 }
   

if (CurrentTime - expireStrt >= 7200000 /*30000*/ && CurrentTime - expireStrt >= expireIntrvl && firstWrng != true ){  
//digitalWrite(10, LOW); 
Serial.println(); 
Serial.print(F("It has been 2 hours since the last calibration, you should consider recalibrating.")); 
expireStrt = millis(); 
expired = millis(); 
expire = true; 
firstWrng = true; 
wrngStrt = millis(); 
calibrated = 1; 
} 
 
if (firstWrng == true && CurrentTime - wrngIntrvl >= wrngStrt ){ 
Serial.println(); 
Serial.print(F("Recalibration recommended!")); 
wrngStrt = millis(); 
lastwrng = millis(); 
 
    } 
 
 if (expire == true && CurrentTime - 1860000 /*63000*/ >= expired && expired > 1) /* && fingerdown == false*/{ // expired > 1 added for gltch 
  autocalibration = true; 
  printrecal = true; 
 }  
 
if (expire == true && CurrentTime - 7200000 >= expired){ 
  calibrated = 0; 
} 
for(int b = 8; b < 24; b ++){
 for (int a = 0; a < 8; a++) { 
    setPinMux(a);
    muxValuesZ[a] = analogRead(2);
    muxValues[a] = analogRead(5);
    if( a == 0 && b == 8){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 1 && b == 9){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 2 && b == 10){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 3 && b == 11){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 4 && b == 12){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 5 && b == 13){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 6 && b == 14){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 7 && b == 15){
      muxValues[b] =  analogRead(6);
      muxValuesZ[b] =  analogRead(3);
    }
    if( a == 0 && b == 16){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 1 && b == 17){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 2 && b == 18){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 3 && b == 19){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 4 && b == 20){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 5 && b == 21){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 6 && b == 22){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }
    if( a == 7 && b == 23){
      muxValues[b] =  analogRead(7);
      muxValuesZ[b] =  analogRead(4);
    }


    //for(int b = 8; a <16; a++){
    //setPinMux(a);
    //muxValuesZB[a] = analogRead(3);  
    //muxValuesB[a] = analogRead(6);   
   //} 
   //for(int c = 16; c < 24; c++){
    //setPinMux(a);
   //muxValuesZC[c] = analogRead(4);
    //muxValuesC[c] = analogRead(7); 
   //}
 }
 /*
   muxValues[0] = muxValuesA[0];
   muxValues[1] = muxValuesA[1];
   muxValues[2] = muxValuesA[2];
   muxValues[3] = muxValuesA[3];
   muxValues[4] = muxValuesA[4];
   muxValues[5] = muxValuesA[5];
   muxValues[6] = muxValuesA[6];
   muxValues[7] = muxValuesA[7];
   muxValues[8] = muxValuesB[0];
   muxValues[9] = muxValuesB[1];
   muxValues[10] = muxValuesB[2];
   muxValues[11] = muxValuesB[3];
   muxValues[12] = muxValuesB[4];
   muxValues[13] = muxValuesB[5];
   muxValues[14] = muxValuesB[6];
   muxValues[15] = muxValuesB[7];
   muxValues[16] = muxValuesC[0];
   muxValues[17] = muxValuesC[1];
   muxValues[18] = muxValuesC[2];
   muxValues[19] = muxValuesC[3];
   muxValues[20] = muxValuesC[4];
   muxValues[21] = muxValuesC[5];
   muxValues[22] = muxValuesC[6];
   muxValues[23] = muxValuesC[7];

   muxValuesZ[0] = muxValuesZA[0];
   muxValuesZ[1] = muxValuesZA[1];
   muxValuesZ[2] = muxValuesZA[2];
   muxValuesZ[3] = muxValuesZA[3];
   muxValuesZ[4] = muxValuesZA[4];
   muxValuesZ[5] = muxValuesZA[5];
   muxValuesZ[6] = muxValuesZA[6];
   muxValuesZ[7] = muxValuesZA[7];
   muxValuesZ[8] = muxValuesZB[0];
   muxValuesZ[9] = muxValuesZB[1];
   muxValuesZ[10] = muxValuesZB[2];
   muxValuesZ[11] = muxValuesZB[3];
   muxValuesZ[12] = muxValuesZB[4];
   muxValuesZ[13] = muxValuesZB[5];
   muxValuesZ[14] = muxValuesZB[6];
   muxValuesZ[15] = muxValuesZB[7];
   muxValuesZ[16] = muxValuesZC[0];
   muxValuesZ[17] = muxValuesZC[1];
   muxValuesZ[18] = muxValuesZC[2];
   muxValuesZ[19] = muxValuesZC[3];
   muxValuesZ[20] = muxValuesZC[4];
   muxValuesZ[21] = muxValuesZC[5];
   muxValuesZ[22] = muxValuesZC[6];
   muxValuesZ[23] = muxValuesZC[7];
*/   
  } 
 

/* 
for (int t1 = 0; t1 > 8; t1++){ 
totalmuxA += muxValuesA[t1];   
  } 
*/ 
   
//DarkTotal = totalmuxA + totalmuxB + totalmuxC; 
//DarkAvg = DarkTotal/24; 
//avgD = DarkAvg; 
 
//  eight = analogRead(2); 
   
//byte touchedValuesX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int touched1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched4[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched5[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched6[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched3[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int fourTouches = 0;
int threeTouches = 0;
int fiveTouches = 0;
int twoTouches = 0;
int oneTouches = 0;
int fourTouch = 0;
int threeTouch = 0;
int fiveTouch = 0;
int twoTouch = 0;
int oneTouch = 0;
float touchedValuesY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float rTouchedValuesY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool noPrint = false;
int group[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
float adjustedGroup[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedDiff[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedCalibrated[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float groupCoef[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int low[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int bLow[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int highestVariance = 0;
int highestPoint = 0;
int calibratedHighestPoint = 0;
bool done = false;
int touch = 25;
int down = 0;
int currentHighestGroup = -1;
float percentTouch[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowCons[] = {0,0,0};
int highCons[] = {0,0,0};
int replaced[] = {0,0,0,0};
int calculatedReplace = 0;
bool addHalf = false;
bool subHalf = false;
int secondTouch = 50;

float rtouchedValuesX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int touched1ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touchedZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched4ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched5ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched6ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched3ZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int fourTouchesZX = 0;
int threeTouchesZX = 0;
int fiveTouchesZX = 0;
int twoTouchesZX = 0;
int oneTouchesZX = 0;
int fourTouchZX = 0;
int threeTouchZX = 0;
int fiveTouchZX = 0;
int twoTouchZX = 0;
int oneTouchZX = 0;
float touchedValuesZXY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float rTouchedValuesZXY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool noPrintZX = false;
int groupZX[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
float adjustedGroupZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedDiffZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedCalibratedZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float groupCoefZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowZX[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int bLowZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int highestVarianceZX =0;
int highestPointZX = 0;
int calibratedHighestPointZX = 0;
bool doneZX = false;
int touchZ = 25;
int downZX = 0;
int currentHighestGroupZX = 0;
float percentTouchZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowConsZX[] = {0,0,0};
int highConsZX[] = {0,0,0};
int replacedZX[] = {0,0,0,0};
int calculatedReplaceZX = 0;
bool addHalfZX = false;
bool subHalfZX = false;
int secondTouchZX = 50;
float rtouchedValuesZX[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

if (calibrationStored == true){ 

   for (int a = 0; a <24; a++){
    
 if (muxValues[a] <= calValues[a] - 4){
n = false;
down++;  
  if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] != -1 && low[6] != -1 && low[7] != -1 && low[8] != -1 && low[9] == -1){

low[9] = a;

group[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 10: ");
//Serial.print(low[9]);

}

if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] != -1 && low[6] != -1 && low[7] != -1 && low[8] == -1 && low[9] == -1){

low[8] = a;

group[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 9: ");
//Serial.print(low[8]);

}

if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] != -1 && low[6] != -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){

low[7] = a;

group[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 8: ");
//Serial.print(low[7]);

}

if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] != -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){

low[6] = a;

group[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 7: ");
//Serial.print(low[6]);

}

if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] != -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){

low[5] = a;

group[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 6: ");
//Serial.print(low[5]);

}

  if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] != -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[4] = a;
    group[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 5: ");
  //Serial.print(low[4]);
  }
  if (low[0] != -1 && low[1] != -1 && low[2] != -1 && low[3] == -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[3] = a;
    group[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 4: ");
  //Serial.print(low[3]);
  }
  if (low[0] != -1 && low[1] != -1 && low[2] == -1 && low[3] == -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[2] = a;
    group[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 3: ");
  //Serial.print(low[2]);
  }
  if (low[0] != -1 && low[1] == -1 && low[2] == -1 && low[3] == -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[1] = a;
    group[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 2: ");
  //Serial.print(low[1]);
  }
  if (low[0] == -1 && low[1] == -1 && low[2] == -1 && low[3] == -1 && low[4] == -1 && low[5] == -1 && low[6] == -1 && low[7] == -1 && low[8] == -1 && low[9] == -1){
    low[0] = a;
    group[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 1: ");
  //Serial.print(low[0]);
  }
  done = true;
 }
  }

  
if(done == true){

if(lTtime + 50 >= lTclock && lTtime > 0 && lTclock > 0){

if(low[2] != -1){
  for(int a = 0; a < 10; a++){
    if(low[a] >= lTouch + 3 && lTouch != 0 || low[a] <= lTouch - 3 && lTouch != 0){
      //for(int b = 0; b < 24; b++){
      //if(group[b] == muxValues[low[a]]){
        //group[b] = -1;
      //}
      //}
      low[a] = -1;
    }
  }
}
}
  
  for(int a = 0; a < 24; a++){
    if (group[a] > currentHighestGroup){
    currentHighestGroup = group[a];
    highestPoint = a; 
    }
  }
/*  
  Serial.println();
  Serial.print("Highest at: ");
  Serial.print(highestPoint);
  Serial.print(" With: ");
  Serial.print(currentHighestGroup);
  */
calibratedHighestPoint = calValues[highestPoint];
  //Serial.println();
  //Serial.print("Calibrated Highest Point at: ");
  //Serial.print(calibratedHighestPoint);
  
 if(currentHighestGroup > 0.00){
  
  if(low[0] > -1){
 groupCoef[low[0]] = calibratedHighestPoint/calValues[low[0]]; 
  //Serial.println();
  //Serial.print("Coefficient 1: ");
  //Serial.print(groupCoef[low[0]]);
 }
  
  if(low[1] > -1){
 groupCoef[low[1]] = calibratedHighestPoint/calValues[low[1]];
  //Serial.println();
  //Serial.print("Coefficient 2: ");
  //Serial.print(groupCoef[low[1]]);
 }
 
  if(low[2] > -1){
 groupCoef[low[2]] = calibratedHighestPoint/calValues[low[2]];
  //Serial.println();
  //Serial.print("Coefficient 3: ");
  //Serial.print(groupCoef[low[2]]);
  }

  if(low[3] > -1){
 groupCoef[low[3]] = calibratedHighestPoint/calValues[low[3]];
  //Serial.println();
  //Serial.print("Coefficient 4: ");
  //Serial.print(groupCoef[low[3]]);
  }
  
  if(low[4] > -1){
 groupCoef[low[4]] = calibratedHighestPoint/calValues[low[4]];
  //Serial.println();
  //Serial.print("Coefficient 4: ");
  //Serial.print(groupCoef[low[4]]);
  }
  if(low[5] > -1){
groupCoef[low[5]] = calibratedHighestPoint/calValues[low[5]];

//Serial.println();

//Serial.print("Coefficient 6: ");

//Serial.print(groupCoef[low[5]]);
}

if(low[6] > -1){
groupCoef[low[6]] = calibratedHighestPoint/calValues[low[6]];

//Serial.println();

//Serial.print("Coefficient 7: ");

//Serial.print(groupCoef[low[6]]);
}

if(low[7] > -1){
groupCoef[low[7]] = calibratedHighestPoint/calValues[low[7]];

//Serial.println();

//Serial.print("Coefficient 8: ");

//Serial.print(groupCoef[low[7]]);
}

if(low[8] > -1){
groupCoef[low[8]] = calibratedHighestPoint/calValues[low[8]];

//Serial.println();

//Serial.print("Coefficient 9: ");

//Serial.print(groupCoef[low[8]]);
}

if(low[9] > -1){

groupCoef[low[9]] = calibratedHighestPoint/calValues[low[9]];

//Serial.println();

//Serial.print("Coefficient 10: ");

//Serial.print(groupCoef[low[9]]);
}

 for (int a = 0; a < 24; a++){
  if(groupCoef[a] > 0.00){
    if(group[a] > -1){
      adjustedGroup[a] = groupCoef[a] * group[a];
    }
    adjustedCalibrated[a] = calValues[a] * groupCoef[a];
    adjustedDiff[a] = adjustedCalibrated[a] - adjustedGroup[a]; 
   }
 }

for(int a = 0; a < 24; a++){

if (adjustedDiff[a] > adjustedDiff[touch]){

touch = a;

}
}

//Serial.println();
//Serial.print("Secondary touch: ");
//Serial.print(calculatedReplace);

for (int a = 0; a < 24; a++){
  if (adjustedDiff[a] > adjustedDiff[secondTouch] && adjustedDiff[a] != adjustedDiff[touch]){
secondTouch = a;
}
}

if(secondTouch < 50){
Serial.println();
Serial.print("secondary touch: ");
Serial.print(secondTouch+1);
}

Serial.println();
Serial.print("Last Touch: ");
Serial.print(lTouch+1);

if(currentDiff > 0){
  lastDiff = currentDiff;
  currentDiff = adjustedDiff[touch];
}

if(currentDiff == 0){
  currentDiff = adjustedDiff[touch];
}

for(int a = 0; a <24; a++){
  if(adjustedDiff[a] > calculatedReplace && adjustedDiff[a] < touch){
    calculatedReplace = a;
  }
}



lTrend = touch - lTouch;


currentDiff = adjustedDiff[touch];

int difference = lTouch - touch; 
int rdifference = sqrt(sq(difference));
  
if(lTtime + 50 >= lTclock && lTtime > 0 && lTclock > 0){
///int closeTouch = 0;
int closeGroup[] = {-1,-1,-1,-1,-1};
//int highestClose = 0;
//int closeCoef[] = {0,0,0,0,0,0};
//int calHighestClose = 0;
//int adjCalClose[] = {0,0,0,0,0};
//int adjCloseGroup[] = {0,0,0,0,0};
int closeTouch = -1;
int rcloseTouch = 0;

if(lTouch >= 2){
closeGroup[0] = lTouch - 2;
}
if(lTouch >= 1){
closeGroup[1] = lTouch - 1;
}

closeGroup[2] = lTouch;
if(lTouch <= 22){
closeGroup[3] = lTouch + 1;
}
if(lTouch <= 21){
closeGroup[4] = lTouch + 2;
}

for (int a = 0; a < 5; a++){
  if(closeGroup[a] != -1){
 if(adjustedDiff[closeGroup[a]] > adjustedDiff[closeTouch]){
  closeTouch = a;

  if(closeTouch == 0){
    rcloseTouch = lTouch - 2;
  }
  if(closeTouch == 1){
    rcloseTouch = lTouch - 1;
  }
  if(closeTouch == 2){
    rcloseTouch = lTouch;
  }
  if(closeTouch == 3){
    rcloseTouch = lTouch + 1;
  }
  if(closeTouch == 4){
    rcloseTouch = lTouch + 2;
  }
 }
  }
}

if(rcloseTouch == lTouch + 2 || rcloseTouch == lTouch - 2){
  int closeTouch2 =0;
  int rcloseTouchRep =0;
   for (int a = 1; a < 4; a++){
    if(closeGroup[a] != -1){
if(adjustedDiff[closeGroup[a]] > adjustedDiff[closeTouch2]){
  closeTouch2 = a;
}
    }
}
if(closeTouch == 1){
  rcloseTouchRep = lTouch - 1;
  //rcloseTouch = lTouch - 1;
}
if(closeTouch == 2){
 rcloseTouchRep = lTouch; 
 //rcloseTouch = lTouch;
}
if(closeTouch == 3){
  rcloseTouchRep = lTouch + 1;
  //rcloseTouch = lTouch + 1;
}
}

if(touch >= lTouch + 3 || touch <= lTouch - 3 && lTouch != 0){
  touch = rcloseTouch;
}

/*
if (low[lTouch - 2] != 0){
  closeGroup[0] = low[lTouch - 2];
}
if (low[lTouch - 1] != 0){
  closeGroup[1] = low[lTouch - 1];
}
if (low[lTouch] != 0){
  closeGroup[2] = low[lTouch];
}
if (low[lTouch + 1] != 0){
  closeGroup[3] = low[lTouch + 1];
}
if (low[lTouch + 2] != 0){
  closeGroup[4] = low[lTouch + 2];
}

for (int a = 0; a<4; a++){
  if(closeGroup[a] > highestClose){
    highestClose = closeGroup;
  }
}

calHighestClose = calValues[highestClose];

if(closeGroup[0] != 0){
closeCoef[0] = calHighestClose/calValues[lTouch - 2];
adjCloseGroup[0] = closeCoef[0] * closeGroup[0];
adjCalClose[0] = calValues[lTouch - 2] * closeCoef[0];
adjCloseDiff[0] = adjCalClose[0] - adjCloseGroup[0];
}

if(closeGroup[1] != 0){
closeCoef[1] = calHighestClose/calValues[lTouch - 1];
adjCloseGroup[1] = closeCoef[1] * closeGroup[1];
adjCalClose[1] = calValues[lTouch - 1] * closeCoef[1];
adjCloseDiff[1] = adjCalClose[1] - adjCloseGroup[1];
}

closeCoef[2] = calHighestClose/calValues[lTouch];
adjCloseGroup[2] = closeCoef[2] * closeGroup[2];
adjCalClose[2] = calValues[lTouch] * closeCoef[2];
adjCloseDiff[2 = adjCalClose[2] - adjCloseGroup[2];

if(closeGroup[3] != 0){
closeCoef[3] = calHighestClose/calValues[lTouch + 1];
adjCloseGroup[3] = closeCoef[3] * closeGroup[3];
adjCalClose[3] = calValues[lTouch + 1] * closeCoef[3];
adjCloseDiff[3] = adjCalClose[3] - adjCloseGroup[3];
}
if(closeGroup[4] != 0){
closeCoef[4] = calHighestClose/calValues[lTouch + 2];
adjCloseGroup[4] = closeCoef[4] * closeGroup[4];
adjCalClose[4] = calValues[lTouch + 2] * closeCoef[4];
adjCloseDiff[4] = adjCalClose[4] - adjCloseGroup[4];
}

for (int a = 0; a < 5; a++){
  if(adjCloseDiff[a] > adjustedDiff[closeTouch]){
    closeTouch = a;
  }
}*/
//if(touch != 23){
//if(touch >= lTouch + 3 && lTouch != 0 || touch <= lTouch - 3 && lTouch != 0){
  //if(secondTouch - 1 <= lTouch + rdifference && secondTouch + 1 >= lTouch - rdifference){ 
  //Serial.println(secondTouch);
  //touch = secondTouch;
  //Serial.println();
  //Serial.print("Touch changed to secondary touch");
  //}
//}
//}


  currentDiff = adjustedDiff[touch];


//Serial.println();
//Serial.print("Current Difference: ");
//Serial.print(currentDiff);

//Serial.println();
//Serial.print("Last Difference: ");
//Serial.print(lastDiff);
if(touch == lTouch){
  if(currentDiff - lastDiff > 0 && lastDiff > 0){
    addHalf = true;
  }
  if(lastDiff - currentDiff > 0 && lastDiff > 0){
    subHalf = true;
  }
}





//else{

//lTouch = touch;

//reset = false;

//}

}
  
  if(touch < 25 && noPrint == false){
  Serial.println();
  Serial.print("Touch at: ");
  if(addHalf == false && subHalf == false){
  Serial.print(touch + 1);
  sendingX[1] = touch + 1;
  }
  if(addHalf == true){
    Serial.print(touch + 1.5);
    sendingX[1] = touch + 1.5;
  }
  if(subHalf == true){
    Serial.print(touch + .5);
    sendingX[1] = touch + .5;
  }
  lTouch = touch;

  sendingX[0] = 0;
  /*if (sendingX[1] != 0){
  if(!radio.write(sendingX, sizeof(sendingX))){
    Serial.println();
    Serial.print(F("~~~~~~~~~~~~Failed to Send X!~~~~~~~~~~~~~~~~~"));
  }
  else {
    Serial.println();
    Serial.print("~~~~~~~~~~~~~~Sent touch: ");
    Serial.print(sendingX[1]);
  }
 }*/
  }
 
 
}
lTtime =  millis();
}
}

 if(lTtime + 2000 <= lTclock && lTtime > 0 && lTclock > 0){
lTouch = 0;
}


if(calibratedZ == true){



   for (int a = 0; a <24; a++){
    
 if (muxValuesZ[a] <= calValuesZ[a] - 2){
z = false;
downZX++;  
  if (lowZX[0] != 0 && lowZX[1] != 0 && lowZX[2] != 0 && lowZX[3] != 0 && lowZX[4] != 0 && lowZX[5] != 0 && lowZX[6] != 0 && lowZX[7] != 0 && lowZX[8] != 0 && lowZX[9] == 0){

lowZX[9] = a;

groupZX[a] = muxValuesZ[a];

//Serial.println();
//Serial.print("stored 10: ");
//Serial.print(lowZX[9]);

}

if (lowZX[0] != 0 && lowZX[1] != 0 && lowZX[2] != 0 && lowZX[3] != 0 && lowZX[4] != 0 && lowZX[5] != 0 && lowZX[6] != 0 && lowZX[7] != 0 && lowZX[8] == 0 && lowZX[9] == 0){

lowZX[8] = a;

groupZX[a] = muxValuesZ[a];

//Serial.println();
//Serial.print("stored 9: ");
//Serial.print(lowZX[8]);

}

if (lowZX[0] != 0 && lowZX[1] != 0 && lowZX[2] != 0 && lowZX[3] != 0 && lowZX[4] != 0 && lowZX[5] != 0 && lowZX[6] != 0 && lowZX[7] == 0 && lowZX[8] == 0 && lowZX[9] == 0){

lowZX[7] = a;

groupZX[a] = muxValuesZ[a];

//Serial.println();
//Serial.print("stored 8: ");
//Serial.print(lowZX[7]);

}

if (lowZX[0] != 0 && lowZX[1] != 0 && lowZX[2] != 0 && lowZX[3] != 0 && lowZX[4] != 0 && lowZX[5] != 0 && lowZX[6] == 0 && lowZX[7] == 0 && lowZX[8] == 0 && lowZX[9] == 0){

lowZX[6] = a;

groupZX[a] = muxValuesZ[a];

//Serial.println();
//Serial.print("stored 7: ");
//Serial.print(lowZX[6]);

}

if (lowZX[0] != 0 && lowZX[1] != 0 && lowZX[2] != 0 && lowZX[3] != 0 && lowZX[4] != 0 && lowZX[5] == 0 && lowZX[6] == 0 && lowZX[7] == 0 && lowZX[8] == 0 && lowZX[9] == 0){

lowZX[5] = a;

groupZX[a] = muxValuesZ[a];

//Serial.println();
//Serial.print("stored 6: ");
//Serial.print(lowZX[5]);

}

  if (lowZX[0] != 0 && lowZX[1] != 0 && lowZX[2] != 0 && lowZX[3] != 0 && lowZX[4] == 0){
    lowZX[4] = a;
    groupZX[a] = muxValuesZ[a];
  //Serial.println();
  //Serial.print("stored 5: ");
  //Serial.print(lowZX[4]);
  }
  if (lowZX[0] != 0 && lowZX[1] != 0 && lowZX[2] != 0 && lowZX[3] == 0 && lowZX[4] == 0){
    lowZX[3] = a;
    groupZX[a] = muxValuesZ[a];
  //Serial.println();
  //Serial.print("stored 4: ");
  //Serial.print(lowZX[3]);
  }
  if (lowZX[0] != 0 && lowZX[1] != 0 && lowZX[2] == 0 && lowZX[3] == 0 && lowZX[4] == 0){
    lowZX[2] = a;
    groupZX[a] = muxValuesZ[a];
  //Serial.println();
  //Serial.print("stored 3: ");
  //Serial.print(lowZX[2]);
  }
  if (lowZX[0] != 0 && lowZX[1] == 0 && lowZX[2] == 0 && lowZX[3] == 0 && lowZX[4] == 0){
    lowZX[1] = a;
    groupZX[a] = muxValuesZ[a];
  //Serial.println();
  //Serial.print("stored 2: ");
  //Serial.print(lowZX[1]);
  }
  if (lowZX[0] == 0 && lowZX[1] == 0 && lowZX[2] == 0 && lowZX[3] == 0 && lowZX[4] == 0){
    lowZX[0] = a;
    groupZX[a] = muxValuesZ[a];
  //Serial.println();
  //Serial.print("stored 1: ");
  //Serial.print(lowZX[0]);
  }
  done = true;
 }
  }

  
if(done == true){
  for(int a = 0; a < 24; a++){
    if (groupZX[a] > currentHighestGroupZX){
    currentHighestGroupZX = groupZX[a];
    highestPointZX = a; 
    }
  }
/*  
  Serial.println();
  Serial.print("Highest at: ");
  Serial.print(highestPointZX);
  Serial.print(" With: ");
  Serial.print(currentHighestGroupZX);
  */
calibratedHighestPointZX = calValuesZ[highestPointZX];
  //Serial.println();
  //Serial.print("Calibrated Highest Point at: ");
  //Serial.print(calibratedHighestPointZX);
  
 if(currentHighestGroupZX > 0.00){
  
  if(lowZX[0] > 0){
 groupCoefZX[lowZX[0]] = calibratedHighestPointZX/calValuesZ[lowZX[0]]; 
  //Serial.println();
  //Serial.print("Coefficient 1: ");
  //Serial.print(groupCoefZX[lowZX[0]]);
 }
  
  if(lowZX[1] > 0){
 groupCoefZX[lowZX[1]] = calibratedHighestPointZX/calValuesZ[lowZX[1]];
  //Serial.println();
  //Serial.print("Coefficient 2: ");
  //Serial.print(groupCoefZX[lowZX[1]]);
 }
 
  if(lowZX[2] > 0){
 groupCoefZX[lowZX[2]] = calibratedHighestPointZX/calValuesZ[lowZX[2]];
  //Serial.println();
  //Serial.print("Coefficient 3: ");
  //Serial.print(groupCoefZX[lowZX[2]]);
  }

  if(lowZX[3] > 0){
 groupCoefZX[lowZX[3]] = calibratedHighestPointZX/calValuesZ[lowZX[3]];
  //Serial.println();
  //Serial.print("Coefficient 4: ");
  //Serial.print(groupCoefZX[lowZX[3]]);
  }
  
  if(lowZX[4] > 0){
 groupCoefZX[lowZX[4]] = calibratedHighestPointZX/calValuesZ[lowZX[4]];
  //Serial.println();
  //Serial.print("Coefficient 4: ");
  //Serial.print(groupCoefZX[lowZX[4]]);
  }
  if(lowZX[5] > 0){
groupCoefZX[lowZX[5]] = calibratedHighestPointZX/calValuesZ[lowZX[5]];

//Serial.println();

//Serial.print("Coefficient 6: ");

//Serial.print(groupCoefZX[lowZX[5]]);
}

if(lowZX[6] > 0){
groupCoefZX[lowZX[6]] = calibratedHighestPointZX/calValuesZ[lowZX[6]];

//Serial.println();

//Serial.print("Coefficient 7: ");

//Serial.print(groupCoefZX[lowZX[6]]);
}

if(lowZX[7] > 0){
groupCoefZX[lowZX[7]] = calibratedHighestPointZX/calValuesZ[lowZX[7]];

//Serial.println();

//Serial.print("Coefficient 8: ");

//Serial.print(groupCoefZX[lowZX[7]]);
}

if(lowZX[8] > 0){
groupCoefZX[lowZX[8]] = calibratedHighestPointZX/calValuesZ[lowZX[8]];

//Serial.println();

//Serial.print("Coefficient 9: ");

//Serial.print(groupCoefZX[lowZX[8]]);
}

if(lowZX[9] > 0){

groupCoefZX[lowZX[9]] = calibratedHighestPointZX/calValuesZ[lowZX[9]];

//Serial.println();

//Serial.print("Coefficient 10: ");

//Serial.print(groupCoefZX[lowZX[9]]);
}

 for (int a = 0; a < 24; a++){
  if(groupCoefZX[a] > 0.00){
    if(groupZX[a] > 0){
      adjustedGroupZX[a] = groupCoefZX[a] * groupZX[a];
    }
    adjustedCalibrated[a] = calValuesZ[a] * groupCoefZX[a];
    adjustedDiffZX[a] = adjustedCalibrated[a] - adjustedGroupZX[a]; 
   }
 }

for(int a = 1; a < 24; a++){

if (adjustedDiffZX[a] > adjustedDiffZX[touchZ]){

TouchZX = a;

}
}

//Serial.println();
//Serial.print("Secondary touchZ: ");
//Serial.print(calculatedReplaceZX);

for (int a = 1; a < 24; a++){
  if (adjustedDiffZX[a] > adjustedDiffZX[secondTouchZX] && adjustedDiffZX[a] != adjustedDiffZX[touchZ]){
secondTouchZX = a;
}
}

if(secondTouchZX > 0){
Serial.println();
Serial.print("secondary touchZ: ");
Serial.print(secondTouchZX + 25);
}

Serial.println();
Serial.print("Last touchZ: ");
Serial.print(lTouchZX + 25);

if(currentDiffZX > 0){
  lastDiffZX = currentDiffZX;
  currentDiffZX = adjustedDiffZX[touchZ];
}

if(currentDiffZX == 0){
  currentDiffZX = adjustedDiffZX[touchZ];
}

for(int a = 0; a <24; a++){
  if(adjustedDiffZX[a] > calculatedReplaceZX && adjustedDiffZX[a] < touchZ){
    calculatedReplaceZX = a;
  }
}



lTrendZX = touchZ - lTouchZX;


currentDiffZX = adjustedDiffZX[touchZ];

int differenceZX = lTouchZX - touchZ; 
int rdifferenceZXZX = sqrt(sq(differenceZX));
  
if(lTtimeZX + 50 >= lTclockZX && lTtimeZX > 0 && lTclockZX > 0){
///int closeTouchZX = 0;
int closegroupZX[] = {0,0,0,0,0};
//int highestClose = 0;
//int closeCoefZX[] = {0,0,0,0,0,0};
//int calHighestClose = 0;
//int adjCalCloseZX[] = {0,0,0,0,0};
//int adjClosegroupZX[] = {0,0,0,0,0};
int closeTouchZX = 0;
int rcloseTouchZX = 0;

closegroupZX[0] = lTouchZX - 2;
closegroupZX[1] = lTouchZX - 1;
closegroupZX[2] = lTouchZX;
closegroupZX[3] = lTouchZX + 1;
closegroupZX[4] = lTouchZX + 2;

for (int a = 0; a < 5; a++){
 if(adjustedDiffZX[closegroupZX[a]] > adjustedDiffZX[closeTouchZX]){
  closeTouchZX = a;
  if(closeTouchZX == 0){
    rcloseTouchZX = lTouchZX - 2;
  }
  if(closeTouchZX == 1){
    rcloseTouchZX = lTouchZX - 1;
  }
  if(closeTouchZX == 2){
    rcloseTouchZX = lTouchZX;
  }
  if(closeTouchZX == 3){
    rcloseTouchZX = lTouchZX + 1;
  }
  if(closeTouchZX == 4){
    rcloseTouchZX = lTouchZX + 2;
  }
 }
}


if(touchZ >= lTouchZX + 3 || touchZ <= lTouchZX - 3 && lTouchZX != 0){
  touchZ = rcloseTouchZX;
}

/*
if (lowZX[lTouchZX - 2] != 0){
  closegroupZX[0] = lowZX[lTouchZX - 2];
}
if (lowZX[lTouchZX - 1] != 0){
  closegroupZX[1] = lowZX[lTouchZX - 1];
}
if (lowZX[lTouchZX] != 0){
  closegroupZX[2] = lowZX[lTouchZX];
}
if (lowZX[lTouchZX + 1] != 0){
  closegroupZX[3] = lowZX[lTouchZX + 1];
}
if (lowZX[lTouchZX + 2] != 0){
  closegroupZX[4] = lowZX[lTouchZX + 2];
}

for (int a = 0; a<4; a++){
  if(closegroupZX[a] > highestClose){
    highestClose = closegroupZX;
  }
}

calHighestClose = calValuesZ[highestClose];

if(closegroupZX[0] != 0){
closeCoefZX[0] = calHighestClose/calValuesZ[lTouchZX - 2];
adjClosegroupZX[0] = closeCoefZX[0] * closegroupZX[0];
adjCalCloseZX[0] = calValuesZ[lTouchZX - 2] * closeCoefZX[0];
adjCloseDiffZX[0] = adjCalCloseZX[0] - adjClosegroupZX[0];
}

if(closegroupZX[1] != 0){
closeCoefZX[1] = calHighestClose/calValuesZ[lTouchZX - 1];
adjClosegroupZX[1] = closeCoefZX[1] * closegroupZX[1];
adjCalCloseZX[1] = calValuesZ[lTouchZX - 1] * closeCoefZX[1];
adjCloseDiffZX[1] = adjCalCloseZX[1] - adjClosegroupZX[1];
}

closeCoefZX[2] = calHighestClose/calValuesZ[lTouchZX];
adjClosegroupZX[2] = closeCoefZX[2] * closegroupZX[2];
adjCalCloseZX[2] = calValuesZ[lTouchZX] * closeCoefZX[2];
adjCloseDiffZX[2 = adjCalCloseZX[2] - adjClosegroupZX[2];

if(closegroupZX[3] != 0){
closeCoefZX[3] = calHighestClose/calValuesZ[lTouchZX + 1];
adjClosegroupZX[3] = closeCoefZX[3] * closegroupZX[3];
adjCalCloseZX[3] = calValuesZ[lTouchZX + 1] * closeCoefZX[3];
adjCloseDiffZX[3] = adjCalCloseZX[3] - adjClosegroupZX[3];
}
if(closegroupZX[4] != 0){
closeCoefZX[4] = calHighestClose/calValuesZ[lTouchZX + 2];
adjClosegroupZX[4] = closeCoefZX[4] * closegroupZX[4];
adjCalCloseZX[4] = calValuesZ[lTouchZX + 2] * closeCoefZX[4];
adjCloseDiffZX[4] = adjCalCloseZX[4] - adjClosegroupZX[4];
}

for (int a = 0; a < 5; a++){
  if(adjCloseDiffZX[a] > adjustedDiffZX[closeTouchZX]){
    closeTouchZX = a;
  }
}*/
//if(touchZ != 23){
//if(touchZ >= lTouchZX + 3 && lTouchZX != 0 || touchZ <= lTouchZX - 3 && lTouchZX != 0){
  //if(secondTouchZX - 1 <= lTouchZX + rdifferenceZXZX && secondTouchZX + 1 >= lTouchZX - rdifferenceZXZX){ 
  //Serial.println(secondTouchZX);
  //TouchZX = secondTouchZX;
  //Serial.println();
  //Serial.print("TouchZX changed to secondary touchZ");
  //}
//}
//}


  currentDiffZX = adjustedDiffZX[touchZ];


//Serial.println();
//Serial.print("Current Difference: ");
//Serial.print(currentDiffZX);

//Serial.println();
//Serial.print("Last Difference: ");
//Serial.print(lastDiffZX);
if(touchZ == lTouchZX){
  if(currentDiffZX - lastDiffZX > 0 && lastDiffZX > 0){
    addHalfZX  = true;
  }
  if(lastDiffZX - currentDiffZX > 0 && lastDiffZX > 0){
    subHalfZX  = true;
  }
}





//else{

//lTouchZX = touchZ;

//reset = false;

//}

}
  
  if(touchZ > 0 && noPrintZX == false){
  Serial.println();
  Serial.print("TouchZX at: ");
  if(addHalfZX  == false && subHalfZX  == false){
  Serial.print(touchZ + 25);
  lTouchZX = touchZ + 25;
  }
  if(addHalfZX  == true){
    Serial.print(touchZ + 25.5);
    lTouchZX = touchZ + 25.5;
  }
  if(subHalfZX  == true){
    Serial.print(touchZ +24.5);
    lTouchZX = touchZ + 24.5;
  }
  lTouchZX = touchZ;
  }
 
 
}
lTtimeZX =  millis();
}


}

if (TouchX == true && r == false || TouchZX == true && r == false){
  for (int a = 0; a < 24; a++){
    if (touchedValuesX[a] != 0){
      TouchedX = a; 
      sendingX[0] = 0;
      sendingX[1] = TouchedX;
      if (sendingX[1] != 0){
   if(!radio.write(sendingX, sizeof(sendingX))){
    Serial.println();
    Serial.print(F("~~~~~~~~~~~~Failed to Send X!~~~~~~~~~~~~~~~~~"));
    }
   else {
    Serial.println();
    Serial.print("~~~~~~~~~~~~~~Sent touch: ");
    Serial.print(sendingX[1]);
  }
 }
    }
  }
  for (int a = 0; a < 24; a++){
    if (touchedValuesZX[a] != 0){
      TouchedZX = a; 
    }/*
 if (TouchedX != 0){
  sendingX[0] = 0;
  sendingX[1] = TouchedX;
 }*/
 if (TouchedZX != 0){
  sendingZX[0] = 1;
  sendingZX[1] = TouchedZX;
 }
  }


 /*if (sendingZX[1] != 0){
  if(!radio.write(sendingZX, sizeof(sendingZX))){
    Serial.println();
    Serial.print(F("~~~~~~~~~~~~~~~~Failed to Send ZX!"));
  }
  else {
    Serial.println();
    Serial.print("~~~~~~~~~~~~~~~~~~~Sent touch: ");
    Serial.print(sendingZX[1]);
  }
 }*/
} 

if (TouchX == true || TouchZX == true){ 
  fingerDown = true; 
} 
     
  
  /*else if( lightSetting == 22 && DarkTotal >= 502 || lightSetting == 23 && DarkTotal >= ){ 
    aligned = true; 
  } 
 
  if (aligned != true){ 
    lightSetting != DarkAvg; 
  } 
   
  LightIntervals(); 
*/ 

if (Serial.available()){
  input = Serial.read();
  if (input == 'n'){
    n = true;
    t = false;
    c = false;
    d = false;
    z = false;
    b = false;
    y = false;
    r = false;
    wrote = false;
  }
  if (input == 'z'){
    z = true;
    n = false;
    t = false;
    c = false;
    d = false;
    b = false;
    y = false;
    r = false;
    wrote = false;
  }
 if (input == 't'){
  t = true;
  n = false;
  c = false;
  d = false;
  z = false;
  b = false;
  y = false;
  r = false;
  wrote = false;
  }
  
  if (input == 'd'){
  dPrinted = false;
  d = true;
  n = false;
  c = false;
  t = false;
  z = false;
  b = false;
  y = false;
  r = false;
  wrote = false;
  }
  
  if (input == 'r'){
    if(adjusted == true){
    rStart = millis();
  Serial.println();
  Serial.print("Starting self-touch adjustment!"); 
  Serial.println();
  Serial.print("Hands off the touch area please!");
  }
  if(wrote == false){
    written = false;
  }
  else{
    written = true;
  }
  cPrinted = false;
  
  r = true;
  n = false;
  t = false;
  d = false;
  z = false;
  
  //y = false;
  //c = false;
}

/*  if (input == 'y'){
  dPrinted = false;
  y = true
  d = false;
  n = false;
  c = false;
  t = false;
  z = false;
  b = false;
  }
*/

 if (input == 'b'){
  dPrinted = false;
  d = true;
  n = false;
  c = false;
  t = false;
  z = false;
  y = false;
  r = false;
  wrote = false;
  }
  
if (input == 'c'){
  cPrinted = false;
  c = true;
  n = false;
  t = false;
  d = false;
  z = false;
  y = false;
  r = false;
  wrote = false;

}

if (input == 'a'){
  autocalibration = true;
}
}

if (n == true){
  if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*/){ 
    muxVals();
    //muxValsZ();
  //muxVals1(); 
  //muxVals2(); 
  //muxVals3(); 
  BaseTime = CurrentTime; 
  printing = true; 
 // singlePrint(); 
 // delay(500); 
}
else {printing = false;} 
} 

if (z == true){
  if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*/){ 
  muxValsZ();
  BaseTime = CurrentTime; 
  printing = true; 
 // singlePrint(); 
 // delay(500); 
}
else {printing = false;} 
} 


//if (t == true){
  //if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*/){
  //if (fingerDown == true){
  //Serial.println();
 // Serial.print(F("Touch detected at: "));
  /*for (int g:touchedValues){
    if (X1 == 1){
      Serial.print(F(" 1 ")); 
    }
    if (X2 == 2){
      Serial.print(F(" 2 ")); 
    }
    if (X3 == 3){
      Serial.print(F(" 3 ")); 
    }
    if (X4 == 4){
      Serial.print(F(" 4 ")); 
    }
    if (X5 == 5){
      Serial.print(F(" 5 ")); 
    }
    if (X6 == 6){
      Serial.print(F(" 6 ")); 
    }
     if (X7 == 7){
      Serial.print(F(" 7 ")); 
    }
    if (X8 == 8){
      Serial.print(F(" 8 ")); 
    }
    if (X9 == 9){
      Serial.print(F(" 9 ")); 
    }
    if (X10 == 10){
      Serial.print(F(" 10 ")); 
    }
    if (X11 == 11){
      Serial.print(F(" 11 ")); 
    }
    if (X12 == 12){
      Serial.print(F(" 12 ")); 
    }
     if (X13 == 13){
      Serial.print(F(" 13 ")); 
    }
    if (X14 == 14){
      Serial.print(F(" 14 ")); 
    }
    if (X15 == 15){
      Serial.print(F(" 15 ")); 
    }
    if (X16 == 16){
      Serial.print(F(" 16 ")); 
    }
    if (X17 == 17){
      Serial.print(F(" 17 ")); 
    }
    if (X18 == 18){
      Serial.print(F(" 18 ")); 
    }
     if (X19 == 19){
      Serial.print(F(" 19 ")); 
    }
    if (X20 == 20){
      Serial.print(F(" 20 ")); 
    }
    if (X21 == 21){
      Serial.print(F(" 21 ")); 
    }
    if (X22 == 22){
      Serial.print(F(" 22 ")); 
    }
    if (X23 == 23){
      Serial.print(F(" 23 ")); 
    }
    if (X24 == 24){
      Serial.print(F(" 24 ")); 
    }
  } */
/*
   if (X1 == 1){
      Serial.print(F(" 1 ")); 
    }
    if (X2 == 2){
      Serial.print(F(" 2 ")); 
    }
    if (X3 == 3){
      Serial.print(F(" 3 ")); 
    }
    if (X4 == 4){
      Serial.print(F(" 4 ")); 
    }
    if (X5 == 5){
      Serial.print(F(" 5 ")); 
    }
    if (X6 == 6){
      Serial.print(F(" 6 ")); 
    }
     if (X7 == 7){
      Serial.print(F(" 7 ")); 
    }
    if (X8 == 8){
      Serial.print(F(" 8 ")); 
    }
    if (X9 == 9){
      Serial.print(F(" 9 ")); 
    }
    if (X10 == 10){
      Serial.print(F(" 10 ")); 
    }
    if (X11 == 11){
      Serial.print(F(" 11 ")); 
    }
    if (X12 == 12){
      Serial.print(F(" 12 ")); 
    }
     if (X13 == 13){
      Serial.print(F(" 13 ")); 
    }
    if (X14 == 14){
      Serial.print(F(" 14 ")); 
    }
    if (X15 == 15){
      Serial.print(F(" 15 ")); 
    }
    if (X16 == 16){
      Serial.print(F(" 16 ")); 
    }
    if (X17 == 17){
      Serial.print(F(" 17 ")); 
    }
    if (X18 == 18){
      Serial.print(F(" 18 ")); 
    }
     if (X19 == 19){
      Serial.print(F(" 19 ")); 
    }
    if (X20 == 20){
      Serial.print(F(" 20 ")); 
    }
    if (X21 == 21){
      Serial.print(F(" 21 ")); 
    }
    if (X22 == 22){
      Serial.print(F(" 22 ")); 
    }
    if (X23 == 23){
      Serial.print(F(" 23 ")); 
    }
    if (X24 == 24){
      Serial.print(F(" 24 ")); 
    }
}
else {
  Serial.println();
  Serial.print(F("No touch detected!"));
}

BaseTime = CurrentTime; 
printing = true; 
}
else {printing = false;} 
}
*/
if (c == true && cPrinted == false){  
     calVals();
  //calVals1(); 
  //calVals2(); 
  //calVals3(); 
  cPrinted = true; 
 // singlePrint(); 
 // delay(500); 
}

if (d == true){
  if (calibrationStored == true){
  if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*/){ 
    calMuxDiff();
  //calMuxDiffA(); 
  //calMuxDiffB(); 
  //calMuxDiffC(); 
  BaseTime = CurrentTime; 
  printing = true; 
 // singlePrint(); 
 // delay(500); 
}
else {printing = false;} 
  }

  else {
  if (dPrinted == false){
  Serial.println();
  Serial.print(F("The X sensors have not been calibrated hence there is no difference available!"));
  dPrinted = true;
  }
} 
}

if (b == true){
  if (calibrationStored == true){
  if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*/){ 
  calMuxDiffZA(); 
  calMuxDiffZB(); 
  calMuxDiffZC(); 
  BaseTime = CurrentTime; 
  printing = true; 
 // singlePrint(); 
 // delay(500); 
}
else {printing = false;} 
  }

  else {
  if (dPrinted == false){
  Serial.println();
  Serial.print(F("The sensors ZX have not been calibrated hence there is no difference available!"));
  dPrinted = true;
  }
} 
}

if (r == true){
if(CurrentTime - rStart >= rIntrvl && CurrentTime > rStart){
  n = true;
  r = false;
  Serial.println();
  Serial.print("Ending self-touch adjustment!");
  adjusted = true;
  digitalWrite(10, LOW);
}
if(written == false){
  digitalWrite(10, HIGH);
  rStart = millis();
  Serial.println();
  Serial.print("Starting self-touch adjustment!"); 
  Serial.println();
  Serial.print("Hands off the touch area please!");
  written = true;
  wrote = true;
  
}
for(int a=0; a<24; a++){
 if(muxValues[a] <= calValues[a] - 2){
  calValues[a] = muxValues[a];
 }
// if(muxValues[a] > calValues[a]){
 //calValues[a] = muxValues[a];
 //}
  
}
}

lTclock = millis();

}

//if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*/){ 
  //muxVals1(); 
  //muxVals2(); 
  //muxVals3(); 
 //PrintMuxA(); 
 //PrintMuxB(); 
 //PrintMuxC(); 
  //PrintSystemVariables(); 
  //if (sent == true){
      //Serial.println();
      //Serial.print(F("Sucess Sending Byte!"));
  //}
/*if (Touch == true){
  Serial.println();
  Serial.print(F("Touch detected at: "));
  for (int g:touchedValues){
    if (g == 1){
      Serial.print(F(" 1 ")); 
    }
    if (g == 2){
      Serial.print(F(" 2 ")); 
    }
    if (g == 3){
      Serial.print(F(" 3 ")); 
    }
    if (g == 4){
      Serial.print(F(" 4 ")); 
    }
    if (g == 5){
      Serial.print(F(" 5 ")); 
    }
    if (g == 6){
      Serial.print(F(" 6 ")); 
    }
     if (g == 7){
      Serial.print(F(" 7 ")); 
    }
    if (g == 8){
      Serial.print(F(" 8 ")); 
    }
    if (g == 9){
      Serial.print(F(" 9 ")); 
    }
    if (g == 10){
      Serial.print(F(" 10 ")); 
    }
    if (g == 11){
      Serial.print(F(" 11 ")); 
    }
    if (g == 12){
      Serial.print(F(" 12 ")); 
    }
     if (g == 13){
      Serial.print(F(" 13 ")); 
    }
    if (g == 14){
      Serial.print(F(" 14 ")); 
    }
    if (g == 15){
      Serial.print(F(" 15 ")); 
    }
    if (g == 16){
      Serial.print(F(" 16 ")); 
    }
    if (g == 17){
      Serial.print(F(" 17 ")); 
    }
    if (g == 18){
      Serial.print(F(" 18 ")); 
    }
     if (g == 19){
      Serial.print(F(" 19 ")); 
    }
    if (g == 20){
      Serial.print(F(" 20 ")); 
    }
    if (g == 21){
      Serial.print(F(" 21 ")); 
    }
    if (g == 22){
      Serial.print(F(" 22 ")); 
    }
    if (g == 23){
      Serial.print(F(" 23 ")); 
    }
    if (g == 24){
      Serial.print(F(" 24 ")); 
    }
  }
}
  
/*if (lowSensors){
    if (Low != 0){
    Serial.println();
    Serial.print(F("The sensors that are low are: "));
    Serial.print(Low);
    if (Low1 != 0){
    Serial.print(F(" , "));
    Serial.print(Low1);
    if (Low2 != 0){
    Serial.print(F(" , "));  
    Serial.print(Low2);
    if (Low3 != 0){    
    Serial.print(F(" , "));
    Serial.print(Low3);
    if (Low4 != 0){
    Serial.print(F(" , "));
    Serial.print(Low4);
       }
      }
     }
    }
  }
}
*/ 
  //BaseTime = CurrentTime; 
  //printing = true; 
 // singlePrint(); 
 // delay(500); 
//} 
 
//else {printing = false;} 
//} 
 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~IMPORTANT TO INCLUDE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
  if (sent = true){
      Serial.println();
      Serial.print(F("Sucess Sending Byte!"));
  }
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
*/
 
 
 
 
 
//MAKE lightSetting A VALUE THAT DOES NOT ALWAYS DEPEND ON DarkTotal 
 
 
 
/* have a calibration button that takes avg of all values  
sets it as the base for different times of the day when the values  
may differ. dark ( night ): 17-19, light ( day ): 22-24 
CAN PUT BASETIME = millis after calibration is executed to avoid the non printing glitch 
*/ 
 
// THE auto recalibration glitch is most likely due to expired being 0 on the first approach 
 
 
 
 
 
 
 
 


