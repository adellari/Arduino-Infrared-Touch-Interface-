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
byte muxValuesA[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  1-8 
byte muxValuesB[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  9-17 
byte muxValuesC[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  18-24 



int muxValuesZ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte muxValuesZA[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  1-8 
byte muxValuesZB[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  9-17 
byte muxValuesZC[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding  18-24 

int calValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
byte calValuesA[] = {0,0,0,0,0,0,0,0};
byte calValuesB[] = {0,0,0,0,0,0,0,0};
byte calValuesC[] = {0,0,0,0,0,0,0,0};

int calValuesZ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
byte calValuesZA[] = {0,0,0,0,0,0,0,0};
byte calValuesZB[] = {0,0,0,0,0,0,0,0};
byte calValuesZC[] = {0,0,0,0,0,0,0,0};

bool lowSensors = false;
float lTouch = 0;
unsigned long lTtime = 0;
unsigned long lTclock = 0;
bool reset = true;

bool lowSensorsZY = false;
float lTouchZY = 0;
unsigned long lTtimeZY = 0;
unsigned long lTclockZY = 0;
bool resetZY = true;

int calibrate; 
int buttonPin = 14; 
bool printing; 
int lightSetting; 
int totalmuxA; 
int totalmuxB; 
int totalmuxC; 
int avgMuxA; 
int avgMuxB; 
int avgMuxC; 
int totalmuxZA; 
int totalmuxZB; 
int totalmuxZC; 
int avgMuxZA; 
int avgMuxZB; 
int avgMuxZC; 
int calibrated; 
int DarkAvg; 
int eight; 
bool expiredCal; 
bool firstWrng; 
bool autocalibration = false; 
int DarkTotal; 
int avgD; 
bool push; 
long OutputInterval = 1500; 
unsigned long rStart = 0;
long rIntrvl = 60000;
//long powerWait = 10000;
//long shadowWait = 3000;
//unsigned long powerOnTime = 0;
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
unsigned long expireStrt; 
unsigned long expireLst; 
bool aligned; 
bool pushed; 
double IRsetting; 
bool calibrationStored = false;
bool calibratedZ = false;
bool adjusted = false;
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
bool wrote = false;

bool cPrinted = true;
bool dPrinted = true;
bool written = true;
 
unsigned long StartTime; 
 
int LowY = 0;
int Low1Y = 0;
int Low2Y = 0;
int Low3Y = 0;
int Low4Y = 0;
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

int LowZY = 0;
int Low1ZY = 0;
int Low2ZY = 0;
int Low3ZY = 0;
int Low4ZY = 0;
int lTrendZY = 0;
float lastZY = 0;


int highTouch1ZY = 0;
int highTouch2ZY = 0;
int highTouch3ZY = 0;
int lowTouch1ZY = 0;
int lowTouch2ZY = 0;
int lowTouch3ZY = 0;
int lastDiffZY = 0;
int currentDiffZY = 0;
  
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
 
const uint64_t wAddress[] = {0xD31373D0C3LL}; 
 
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
 
/*void muxVals1() { 
  Serial.println(); 
  Serial.println(F("Values for multiplexer:")); 
  Serial.println(F("===========================")); 
  for (int a = 0; a < 8; a++) { 
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesA[a]); 
  } 
  Serial.println(F("===========================")); 
} 

void muxValsZ1() { 
  for (int a = 0; a < 8; a++) { 
    Serial.print(F("ZX"));
    Serial.print(a); 
    Serial.print(F(" = ")); 
    Serial.println(muxValuesZA[a]); 
  } 
  Serial.println(F("===========================")); 
}

void muxVals2() { 
  for (int b1 = 0; b1 < 8; b1++) { 
  for (int b = 8; b < 17; b++) { 
  if (b == 8 && b1 == 0){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesB[b1]); 
  } 
  if (b == 9 && b1 == 1){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesB[b1]); 
  } 
  if (b == 10 && b1 == 2){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesB[b1]); 
  } 
  if (b == 11 && b1 == 3){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesB[b1]); 
  } 
  if (b == 12 && b1 == 4){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesB[b1]); 
  } 
  if (b == 13 && b1 == 5){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesB[b1]); 
  } 
  if (b == 14 && b1 == 6){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesB[b1]); 
  } 
  if (b == 15 && b1 == 7){ 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesB[b1]); 
  } 
  } 
  }  
     
   
  Serial.println(F("===========================")); 
} 

void calVals1() { 
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
} 
*/
void calMuxDiffA() { 
  Serial.println(); 
  Serial.println(F("Current Difference Between Calibrated Values and Current Y Values:")); 
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
}

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

void muxValsZ2() { 
  for (int b1 = 0; b1 < 8; b1++) { 
  for (int b = 8; b < 17; b++) { 
  if (b == 8 && b1 == 0){
    Serial.print(F("ZY")); 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZB[b1]); 
  } 
  if (b == 9 && b1 == 1){ 
    Serial.print(F("ZY")); 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZB[b1]); 
  } 
  if (b == 10 && b1 == 2){ 
    Serial.print(F("ZY")); 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZB[b1]); 
  } 
  if (b == 11 && b1 == 3){ 
    Serial.print(F("ZY")); 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZB[b1]); 
  } 
  if (b == 12 && b1 == 4){ 
    Serial.print(F("ZY")); 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZB[b1]); 
  } 
  if (b == 13 && b1 == 5){ 
    Serial.print(F("ZY")); 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZB[b1]); 
  } 
  if (b == 14 && b1 == 6){ 
    Serial.print(F("ZY")); 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZB[b1]); 
  } 
  if (b == 15 && b1 == 7){ 
    Serial.print(F("ZY")); 
    Serial.print(b); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZB[b1]); 
  } 
  } 
  }  
     
   
  Serial.println(F("===========================")); 
} 
/* 
void muxVals3() { 
  for (int c1 = 0; c1 < 8; c1++) { 
  for (int c = 16; c < 24; c++) { 
  if (c == 16 && c1 == 0){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesC[c1]); 
  } 
  if (c == 17 && c1 == 1){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesC[c1]); 
  } 
  if (c == 18 && c1 == 2){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesC[c1]); 
  } 
  if (c == 19 && c1 == 3){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesC[c1]); 
  } 
  if (c == 20 && c1 == 4){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesC[c1]); 
  } 
  if (c == 21 && c1 == 5){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesC[c1]); 
  } 
  if (c == 22 && c1 == 6){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesC[c1]); 
  } 
  if (c == 23 && c1 == 7){ 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesC[c1]); 
  } 
  } 
  }  
     
   
  Serial.println(F("===========================")); 
} 

 void muxValsZ3() { 
  for (int c1 = 0; c1 < 8; c1++) { 
  for (int c = 16; c < 24; c++) { 
  if (c == 16 && c1 == 0){ 
    Serial.print(F("ZY"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 17 && c1 == 1){ 
    Serial.print(F("ZY"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 18 && c1 == 2){ 
    Serial.print(F("ZY"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 19 && c1 == 3){
    Serial.print(F("ZY")); 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 20 && c1 == 4){ 
    Serial.print(F("ZY"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 21 && c1 == 5){ 
    Serial.print(F("ZY"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 22 && c1 == 6){ 
    Serial.print(F("ZY"));
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  if (c == 23 && c1 == 7){
    Serial.print(F("ZY")); 
    Serial.print(c); 
    Serial.print(F(" = "));  
    Serial.println(muxValuesZC[c1]); 
  } 
  } 
  }  
     
   
  Serial.println(F("===========================")); 
} 
 
void PrintMuxA() { 
  Serial.print(F("The total for mux A is: ")); 
  Serial.print(totalmuxA); 
  Serial.println(); 
  Serial.print(F("Calibration is: ")); 
  if (pushed == true){ 
    Serial.print(F("Active")); 
  } 
  else{ Serial.print(F("Inactive")); 
  } 
  Serial.println(); 
  Serial.print(F("The average value for mux A is: ")); 
  Serial.print(avgMuxA); 
  Serial.println(); 
} 
 
 
 
void PrintMuxB() { 
  Serial.print(F("The total for mux B is: ")); 
  Serial.print(totalmuxB); 
  Serial.println(); 
  Serial.print(F("The average value for mux B is: ")); 
  Serial.print(avgMuxB); 
  Serial.println(); 
} 
 
 
 
void PrintMuxC() { 
  Serial.print(F("The total for mux C is: ")); 
  Serial.print(totalmuxC); 
  Serial.println(); 
  Serial.print(F("The average value for mux C is: ")); 
  Serial.print(avgMuxC); 
  Serial.println(); 
} 
*/ 
void PrintSystemVariables() { 
  Serial.print(F("The light setting is at: ")); 
  Serial.print(IRsetting); 
  Serial.println(); 
  Serial.print(F("The system is: ")); 
if(aligned == true){ 
  Serial.print(F("Aligned")); 
} 
else if(aligned != true){ 
  Serial.print(F("Not aligned")); 
} 
  /* 
if (seventeen != 0){ 
  Serial.println(); 
  Serial.print(F("The time for 17 is: ")); 
  Serial.print(seventeen); 
    } 
if (eighteen != 0){ 
  Serial.println(); 
  Serial.print(F("The time for 18 is: ")); 
  Serial.print(eighteen); 
    } 
if (nineteen != 0){ 
  Serial.println(); 
  Serial.print(F("The time for 19 is: ")); 
  Serial.print(nineteen); 
    } 
if (twenty != 0){ 
   Serial.println(); 
  Serial.print(F("The time for 20 is: ")); 
  Serial.print(twenty); 
    } 
if (twentyone != 0){ 
  Serial.println(); 
  Serial.print(F("The time for 21 is: ")); 
  Serial.print(twentyone); 
    } 
if (twentytwo != 0){ 
  Serial.println(); 
  Serial.print(F("The time for 22 is: ")); 
  Serial.print(twentytwo); 
    } 
if (twentythree != 0){ 
  Serial.println(); 
  Serial.print(F("The time for 23 is: ")); 
  Serial.print(twentythree); 
    } 
if (twentyfour != 0){ 
  Serial.println(); 
  Serial.print(F("The time for 24 is: ")); 
  Serial.print(twentyfour); 
    } 
if (twentyfive != 0){ 
  Serial.println(); 
  Serial.print(F("The time for 25 is: ")); 
  Serial.print(twentyfive); 
    } 
*/ 
  Serial.println(); 
  Serial.print(F("The average Dark time is: ")); 
  Serial.print(avgD); 
  Serial.println(); 
  Serial.print(F("The time is: ")); 
  Serial.print(CurrentTime); 
   
  Serial.println(); 
  Serial.print(F("The calibration expired at: ")); 
  Serial.print(expired); 
 
  Serial.println(); 
  Serial.print(F("The last warning was put out at: ")); 
  Serial.print(lastwrng); 
   
} 
/* 
  void LightIntervals(){ 
   
  if (DarkAvg = 17){ 
    seventeen = millis(); 
  } 
   else { seventeen = 0;} 
   
  if (DarkAvg = 18){ 
    eighteen = millis(); 
  } 
   else { eighteen = 0;} 
 
  if (DarkAvg = 19){ 
    nineteen = millis(); 
  } 
   else { nineteen = 0;} 
   
  if (DarkAvg = 20){ 
    twenty = millis(); 
  } 
   else { twenty = 0;} 
    
  if (DarkAvg = 21){ 
    twentyone = millis(); 
  } 
  else { twentyone = 0;} 
   
  if (DarkAvg = 22){ 
    twentytwo = millis(); 
  } 
   else { twentytwo = 0;} 
   
  if (DarkAvg = 23){ 
    twentythree = millis(); 
  } 
   else { twentythree = 0;} 
   
  if (DarkAvg = 24){ 
    twentyfour = millis(); 
  } 
   else { twentyfour = 0;} 
   
  if (DarkAvg  = 25){ 
    twentyfive = millis(); 
  } 
   else { twentyfive = 0;} 
} 
 
 
 
 
void singlePrint() { 
  Serial.println(F("===============================")); 
  Serial.print(F("Eight's value is: ")); 
  Serial.print(eight); 
  Serial.println(); 
  Serial.print(F("===============================")); 
} 
*/ 
 
void timeloop(){ 
currentCal = millis(); 
} 

bool calArray (){
unsigned long prodCalA;
unsigned long prodCalB;
unsigned long prodCalC;
LowY = 0;
Low1Y = 0;
Low2Y = 0;
Low3Y = 0;
Low4Y = 0;

LowZY = 0;
Low1ZY = 0;
Low2ZY = 0;
Low3ZY = 0;
Low4ZY = 0;

int threesY = 0;
int twoosY = 0;
int onesY = 0;
int zeroesY = 0;

int threesZY = 0;
int twoosZY = 0;
int onesZY = 0;
int zeroesZY = 0;

bool low = true;
bool paired = false;
/*
for (int x=0; x<8; x++){
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
  if (a == 0) ++zeroesY;
  if (a == 1) ++onesY;
  if (a == 2) ++twoosY;
  if (a == 3) ++threesY;
}

for (int b:calValuesB){
  if (b == 0) ++zeroesY;
  if (b == 1) ++onesY;
  if (b == 2) ++twoosY;
  if (b == 3) ++threesY;
}

for (int c:calValuesC){
  if (c == 0) ++zeroesY;
  if (c == 1) ++onesY;
  if (c == 2) ++twoosY;
  if (c == 3) ++threesY;
}

for (int a:calValuesZA){
  if (a == 0) ++zeroesZY;
  if (a == 1) ++onesZY;
  if (a == 2) ++twoosZY;
  if (a == 3) ++threesZY;
}

for (int b:calValuesZB){
  if (b == 0) ++zeroesZY;
  if (b == 1) ++onesZY;
  if (b == 2) ++twoosZY;
  if (b == 3) ++threesZY;
}

for (int c:calValuesZC){
  if (c == 0) ++zeroesZY;
  if (c == 1) ++onesZY;
  if (c == 2) ++twoosZY;
  if (c == 3) ++threesZY;
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
  //if (zeroesY /*+ onesY + twoosY + threesY  + zeroesZY + onesZY*/ /*+ twoosZY + threesZY*/ == 0){
  calibrationStored = true;
  paired = true;   
  low = false;  
Serial.println();
Serial.print(F("Calibration Successful! "));
  //}
  //else {
    //calibrationStored = false;
    //low = true;
    //paired = false;

Serial.println();
Serial.print(F("Calibration Failed!"));

Serial.println();
Serial.print(F("There are this many zeroes for Y: "));
Serial.print(zeroesY);

Serial.println();
Serial.print(F("There are this many ones for Y: "));
Serial.print(onesY);
    
Serial.println();
Serial.print(F("There are this many twoos for Y: "));
Serial.print(twoosY);
  
Serial.println();
Serial.print(F("There are this many threes for Y: "));
Serial.print(threesY);

Serial.println();
Serial.print(F("There are this many zeroes for ZY: "));
Serial.print(zeroesY);

Serial.println();
Serial.print(F("There are this many zeroes for ZY: "));
Serial.print(onesZY);

Serial.println();
Serial.print(F("There are this many zeroes for ZY: "));
Serial.print(twoosZY);

Serial.println();
Serial.print(F("There are this many zeroes for ZY: "));
Serial.print(threesZY);
//}
return paired; 
}

void loop() { 

lTclock = millis();

/*if(CurrentTime >= powerOnTime + 20000 && selfCal == false && CurrentTime > 0){
  autocalibration = true;
  powerOnTime = millis();
}

if(powerOnTime != 0 && CurrentTime - selfAdjTime >= powerOnTime && selfCal == false){
  r = true;
  selfCal = true;
}*/


char input = 'O';
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

double TouchedY = 0;
double TouchedZY = 0;
bool TouchY = false;
bool TouchZY = false;
bool tapped = false;  
unsigned long tappedTime = NULL;  
bool fingerDown = false;  

double Y25 = 0;   
double Y26 = 0;   
double Y27 = 0;  
double Y28 = 0;  
double Y29 = 0;   
double Y30 = 0;  
double Y31 = 0;  
double Y32 = 0;   
double Y33 = 0;  
double Y34 = 0;  
double Y35 = 0;   
double Y36 = 0;  
double Y37 = 0;  
double Y38 = 0;  
double Y39 = 0;  
double Y40 = 0;  
double Y41 = 0;   
double Y42 = 0;  
double Y43 = 0;   
double Y44 = 0;  
double Y45 = 0;   
double Y46 = 0;  
double Y47 = 0;   
double Y48 = 0;  


double ZY1 = 0;   
double ZY2 = 0;   
double ZY3 = 0;  
double ZY4 = 0;  
double ZY5 = 0;   
double ZY6 = 0;  
double ZY7 = 0;  
double ZY8 = 0;   
double ZY9 = 0;  
double ZY10 = 0;  
double ZY11 = 0;   
double ZY12 = 0;  
double ZY13 = 0;  
double ZY14 = 0;  
double ZY15 = 0;  
double ZY16 = 0;  
double ZY17 = 0;   
double ZY18 = 0;  
double ZY19 = 0;   
double ZY20 = 0;  
double ZY21 = 0;   
double ZY22 = 0;  
double ZY23 = 0; 
double ZY24 = 0;   

bool Yhalf25 = false;
bool Yhalf26 = false;
bool Yhalf27 = false;
bool Yhalf28 = false;
bool Yhalf29 = false;
bool Yhalf30 = false;
bool Yhalf31 = false;
bool Yhalf32 = false;
bool Yhalf33 = false;
bool Yhalf34 = false;
bool Yhalf35 = false;
bool Yhalf36 = false;
bool Yhalf37 = false;
bool Yhalf38 = false;
bool Yhalf39 = false;
bool Yhalf40 = false;
bool Yhalf41 = false;
bool Yhalf42 = false;
bool Yhalf43 = false;
bool Yhalf44 = false;
bool Yhalf45 = false;
bool Yhalf46 = false;
bool Yhalf47 = false;
bool Yhalf48 = false;

bool ZYhalf1 = false;
bool ZYhalf2 = false;
bool ZYhalf3 = false;
bool ZYhalf4 = false;
bool ZYhalf5 = false;
bool ZYhalf6 = false;
bool ZYhalf7 = false;
bool ZYhalf8 = false;
bool ZYhalf9 = false;
bool ZYhalf10 = false;
bool ZYhalf11 = false;
bool ZYhalf12 = false;
bool ZYhalf13 = false;
bool ZYhalf14 = false;
bool ZYhalf15 = false;
bool ZYhalf16 = false;
bool ZYhalf17 = false;
bool ZYhalf18 = false;
bool ZYhalf19 = false;
bool ZYhalf20 = false;
bool ZYhalf21 = false;
bool ZYhalf22 = false;
bool ZYhalf23 = false;
bool ZYhalf24 = false;
bool sent = false;

float sendingY[] = {0,0};   
float sendingZ[] = {0,0};

 
 
 
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
  IRsetting = (totalmuxA + totalmuxB + totalmuxC)/24; 
  currentCal = millis(); 
  //digitalWrite(10, HIGH); 
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

 for (int a = 0; a < 8; a++) { 
    setPinMux(a);
   setPinMux(a); 
    muxValuesZA[a] = analogRead(2); 
    muxValuesZB[a] = analogRead(3); 
    muxValuesZC[a] = analogRead(4);
    muxValuesA[a] = analogRead(5); 
    muxValuesB[a] = analogRead(6); 
    muxValuesC[a] = analogRead(7); 
    
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
int touch = 24;
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
int touched1ZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touchedZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched4ZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched5ZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched6ZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int touched3ZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int fourTouchesZY = 0;
int threeTouchesZY = 0;
int fiveTouchesZY = 0;
int twoTouchesZY = 0;
int oneTouchesZY = 0;
int fourTouchZY = 0;
int threeTouchZY = 0;
int fiveTouchZY = 0;
int twoTouchZY = 0;
int oneTouchZY = 0;
float touchedValuesZYY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float rTouchedValuesZYY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool noPrintZY = false;
int groupZY[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
float adjustedGroupZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedDiffZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float adjustedCalibratedZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float groupCoefZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowZY[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int bLowZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int highestVarianceZY =0;
int highestPointZY = 0;
int calibratedHighestPointZY = 0;
bool doneZY = false;
int touchZ = 24;
int downZY = 0;
int currentHighestGroupZY = -1;
float percentTouchZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowConsZY[] = {0,0,0};
int highConsZY[] = {0,0,0};
int replacedZY[] = {0,0,0,0};
int calculatedReplaceZY = 0;
bool addHalfZY = false;
bool subHalfZY = false;
int secondTouchZY = 50;
float rtouchedValuesZY[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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
      for(int b = 0; b < 24; b++){
      if(group[b] == muxValues[low[a]]){
        group[b] = -1;
      }
      }
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

//if(secondTouch < 50){
//Serial.println();
//Serial.print("secondary touch: ");
//Serial.print(secondTouch + 25);
//}

Serial.println();
Serial.print("Touch: ");
Serial.print(touch);

//Serial.println();
//Serial.print("Last Touch: ");
//Serial.print(lTouch + 25);

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


if(touch >= lTouch + 3 && lTouch != 0 || touch <= lTouch - 3 && lTouch != 0){
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
  
  if(touch < 24 && noPrint == false){
  Serial.println();
  Serial.print("Touch at: ");
  if(addHalf == false && subHalf == false){
  Serial.print(touch + 25);
  sendingY[1] = touch + 25;
  }
  if(addHalf == true){
    Serial.print(touch + 25.5);
    sendingY[1] = touch + 25.5;
  }
  if(subHalf == true){
    Serial.print(touch + 24.5);
    sendingY[1] = touch + 24.5;
  }

    sendingY[0] = 0;
  /*if (sendingY[1] != 0){
  if(!radio.write(sendingY, sizeof(sendingY))){
    Serial.println();
    Serial.print(F("~~~~~~~~~~~~Failed to Send Y!~~~~~~~~~~~~~~~~~"));
  }
  else {
    Serial.println();
    Serial.print("~~~~~~~~~~~~~~Sent touch: ");
    Serial.print(sendingY[1]);
  }
 }*/
 lTouch = touch;
  }
 
 
}
lTtime =  millis();
}
}

 if(lTtime + 2000 <= lTclock && lTtime > 0 && lTclock > 0){
lTouch = 0;
}


if(calibratedStored == true){



   for (int a = 0; a <24; a++){
    
 if (muxValuesZ[a] <= calValuesZ[a] - 2){
z = false;
downZY++;  
   if (lowZY[0] != -1 && lowZY[1] != -1 && lowZY[2] != -1 && lowZY[3] != -1 && lowZY[4] != -1 && lowZY[5] != -1 && lowZY[6] != -1 && lowZY[7] != -1 && lowZY[8] != -1 && lowZY[9] == -1){

lowZY[9] = a;

groupZY[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 10: ");
//Serial.print(lowZY[9]);

}

if (lowZY[0] != -1 && lowZY[1] != -1 && lowZY[2] != -1 && lowZY[3] != -1 && lowZY[4] != -1 && lowZY[5] != -1 && lowZY[6] != -1 && lowZY[7] != -1 && lowZY[8] == -1 && lowZY[9] == -1){

lowZY[8] = a;

groupZY[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 9: ");
//Serial.print(lowZY[8]);

}

if (lowZY[0] != -1 && lowZY[1] != -1 && lowZY[2] != -1 && lowZY[3] != -1 && lowZY[4] != -1 && lowZY[5] != -1 && lowZY[6] != -1 && lowZY[7] == -1 && lowZY[8] == -1 && lowZY[9] == -1){

lowZY[7] = a;

groupZY[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 8: ");
//Serial.print(lowZY[7]);

}

if (lowZY[0] != -1 && lowZY[1] != -1 && lowZY[2] != -1 && lowZY[3] != -1 && lowZY[4] != -1 && lowZY[5] != -1 && lowZY[6] == -1 && lowZY[7] == -1 && lowZY[8] == -1 && lowZY[9] == -1){

lowZY[6] = a;

groupZY[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 7: ");
//Serial.print(lowZY[6]);

}

if (lowZY[0] != -1 && lowZY[1] != -1 && lowZY[2] != -1 && lowZY[3] != -1 && lowZY[4] != -1 && lowZY[5] == -1 && lowZY[6] == -1 && lowZY[7] == -1 && lowZY[8] == -1 && lowZY[9] == -1){

lowZY[5] = a;

groupZY[a] = muxValues[a];

//Serial.println();
//Serial.print("stored 6: ");
//Serial.print(lowZY[5]);

}

  if (lowZY[0] != -1 && lowZY[1] != -1 && lowZY[2] != -1 && lowZY[3] != -1 && lowZY[4] == -1 && lowZY[5] == -1 && lowZY[6] == -1 && lowZY[7] == -1 && lowZY[8] == -1 && lowZY[9] == -1){
    lowZY[4] = a;
    groupZY[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 5: ");
  //Serial.print(lowZY[4]);
  }
  if (lowZY[0] != -1 && lowZY[1] != -1 && lowZY[2] != -1 && lowZY[3] == -1 && lowZY[4] == -1 && lowZY[5] == -1 && lowZY[6] == -1 && lowZY[7] == -1 && lowZY[8] == -1 && lowZY[9] == -1){
    lowZY[3] = a;
    groupZY[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 4: ");
  //Serial.print(lowZY[3]);
  }
  if (lowZY[0] != -1 && lowZY[1] != -1 && lowZY[2] == -1 && lowZY[3] == -1 && lowZY[4] == -1 && lowZY[5] == -1 && lowZY[6] == -1 && lowZY[7] == -1 && lowZY[8] == -1 && lowZY[9] == -1){
    lowZY[2] = a;
    groupZY[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 3: ");
  //Serial.print(lowZY[2]);
  }
  if (lowZY[0] != -1 && lowZY[1] == -1 && lowZY[2] == -1 && lowZY[3] == -1 && lowZY[4] == -1 && lowZY[5] == -1 && lowZY[6] == -1 && lowZY[7] == -1 && lowZY[8] == -1 && lowZY[9] == -1){
    lowZY[1] = a;
    groupZY[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 2: ");
  //Serial.print(lowZY[1]);
  }
  if (lowZY[0] == -1 && lowZY[1] == -1 && lowZY[2] == -1 && lowZY[3] == -1 && lowZY[4] == -1 && lowZY[5] == -1 && lowZY[6] == -1 && lowZY[7] == -1 && lowZY[8] == -1 && lowZY[9] == -1){
    lowZY[0] = a;
    groupZY[a] = muxValues[a];
  //Serial.println();
  //Serial.print("stored 1: ");
  //Serial.print(lowZY[0]);
  }
  done = true;
 }
  }

  
if(done == true){
  for(int a = 0; a < 24; a++){
    if (groupZY[a] > currentHighestGroupZY){
    currentHighestGroupZY = groupZY[a];
    highestPointZY = a; 
    }
  }
/*  
  Serial.println();
  Serial.print("Highest at: ");
  Serial.print(highestPointZY);
  Serial.print(" With: ");
  Serial.print(currentHighestGroupZY);
  */
calibratedHighestPointZY = calValuesZ[highestPointZY];
  //Serial.println();
  //Serial.print("Calibrated Highest Point at: ");
  //Serial.print(calibratedHighestPointZY);
  
 if(currentHighestGroupZY > 0.00){
  
  if(lowZY[0] > -1){
 groupCoefZY[lowZY[0]] = calibratedHighestPointZY/calValuesZ[lowZY[0]]; 
  //Serial.println();
  //Serial.print("Coefficient 1: ");
  //Serial.print(groupCoefZY[lowZY[0]]);
 }
  
  if(lowZY[1] > -1){
 groupCoefZY[lowZY[1]] = calibratedHighestPointZY/calValuesZ[lowZY[1]];
  //Serial.println();
  //Serial.print("Coefficient 2: ");
  //Serial.print(groupCoefZY[lowZY[1]]);
 }
 
  if(lowZY[2] > -1){
 groupCoefZY[lowZY[2]] = calibratedHighestPointZY/calValuesZ[lowZY[2]];
  //Serial.println();
  //Serial.print("Coefficient 3: ");
  //Serial.print(groupCoefZY[lowZY[2]]);
  }

  if(lowZY[3] > -1){
 groupCoefZY[lowZY[3]] = calibratedHighestPointZY/calValuesZ[lowZY[3]];
  //Serial.println();
  //Serial.print("Coefficient 4: ");
  //Serial.print(groupCoefZY[lowZY[3]]);
  }
  
  if(lowZY[4] > -1){
 groupCoefZY[lowZY[4]] = calibratedHighestPointZY/calValuesZ[lowZY[4]];
  //Serial.println();
  //Serial.print("Coefficient 4: ");
  //Serial.print(groupCoefZY[lowZY[4]]);
  }
  if(lowZY[5] > -1){
groupCoefZY[lowZY[5]] = calibratedHighestPointZY/calValuesZ[lowZY[5]];

//Serial.println();

//Serial.print("Coefficient 6: ");

//Serial.print(groupCoefZY[lowZY[5]]);
}

if(lowZY[6] > -1){
groupCoefZY[lowZY[6]] = calibratedHighestPointZY/calValuesZ[lowZY[6]];

//Serial.println();

//Serial.print("Coefficient 7: ");

//Serial.print(groupCoefZY[lowZY[6]]);
}

if(lowZY[7] > -1){
groupCoefZY[lowZY[7]] = calibratedHighestPointZY/calValuesZ[lowZY[7]];

//Serial.println();

//Serial.print("Coefficient 8: ");

//Serial.print(groupCoefZY[lowZY[7]]);
}

if(lowZY[8] > -1){
groupCoefZY[lowZY[8]] = calibratedHighestPointZY/calValuesZ[lowZY[8]];

//Serial.println();

//Serial.print("Coefficient 9: ");

//Serial.print(groupCoefZY[lowZY[8]]);
}

if(lowZY[9] > -1){

groupCoefZY[lowZY[9]] = calibratedHighestPointZY/calValuesZ[lowZY[9]];

//Serial.println();

//Serial.print("Coefficient 10: ");

//Serial.print(groupCoefZY[lowZY[9]]);
}

 for (int a = 0; a < 24; a++){
  if(groupCoefZY[a] > 0.00){
    if(groupZY[a] > 0){
      adjustedGroupZY[a] = groupCoefZY[a] * groupZY[a];
    }
    adjustedCalibrated[a] = calValuesZ[a] * groupCoefZY[a];
    adjustedDiffZY[a] = adjustedCalibrated[a] - adjustedGroupZY[a]; 
   }
 }

for(int a = 1; a < 24; a++){

if (adjustedDiffZY[a] > adjustedDiffZY[touchZ]){

TouchZY = a;

}
}

//Serial.println();
//Serial.print("Secondary touchZ: ");
//Serial.print(calculatedReplaceZY);

for (int a = 1; a < 24; a++){
  if (adjustedDiffZY[a] > adjustedDiffZY[secondTouchZY] && adjustedDiffZY[a] != adjustedDiffZY[touchZ]){
secondTouchZY = a;
}
}

if(secondTouchZY > 0){
Serial.println();
Serial.print("secondary touchZ: ");
Serial.print(secondTouchZY + 25);
}

Serial.println();
Serial.print("Last touchZ: ");
Serial.print(lTouchZY + 25);

if(currentDiffZY > 0){
  lastDiffZY = currentDiffZY;
  currentDiffZY = adjustedDiffZY[touchZ];
}

if(currentDiffZY == 0){
  currentDiffZY = adjustedDiffZY[touchZ];
}

for(int a = 0; a <24; a++){
  if(adjustedDiffZY[a] > calculatedReplaceZY && adjustedDiffZY[a] < touchZ){
    calculatedReplaceZY = a;
  }
}



lTrendZY = touchZ - lTouchZY;


currentDiffZY = adjustedDiffZY[touchZ];

int differenceZY = lTouchZY - touchZ; 
int rdifferenceZYZY = sqrt(sq(differenceZY));
  
if(lTtimeZY + 50 >= lTclockZY && lTtimeZY > 0 && lTclockZY > 0){
///int closeTouchZY = 0;
int closegroupZY[] = {0,0,0,0,0};
//int highestClose = 0;
//int closeCoefZY[] = {0,0,0,0,0,0};
//int calHighestClose = 0;
//int adjCalCloseZY[] = {0,0,0,0,0};
//int adjClosegroupZY[] = {0,0,0,0,0};
int closeTouchZY = 0;
int rcloseTouchZY = 0;

closegroupZY[0] = lTouchZY - 2;
closegroupZY[1] = lTouchZY - 1;
closegroupZY[2] = lTouchZY;
closegroupZY[3] = lTouchZY + 1;
closegroupZY[4] = lTouchZY + 2;

for (int a = 0; a < 5; a++){
 if(adjustedDiffZY[closegroupZY[a]] > adjustedDiffZY[closeTouchZY]){
  closeTouchZY = a;
  if(closeTouchZY == 0){
    rcloseTouchZY = lTouchZY - 2;
  }
  if(closeTouchZY == 1){
    rcloseTouchZY = lTouchZY - 1;
  }
  if(closeTouchZY == 2){
    rcloseTouchZY = lTouchZY;
  }
  if(closeTouchZY == 3){
    rcloseTouchZY = lTouchZY + 1;
  }
  if(closeTouchZY == 4){
    rcloseTouchZY = lTouchZY + 2;
  }
 }
}


if(touchZ >= lTouchZY + 3 || touchZ <= lTouchZY - 3 && lTouchZY != 0){
  touchZ = rcloseTouchZY;
}

/*
if (lowZY[lTouchZY - 2] != 0){
  closegroupZY[0] = lowZY[lTouchZY - 2];
}
if (lowZY[lTouchZY - 1] != 0){
  closegroupZY[1] = lowZY[lTouchZY - 1];
}
if (lowZY[lTouchZY] != 0){
  closegroupZY[2] = lowZY[lTouchZY];
}
if (lowZY[lTouchZY + 1] != 0){
  closegroupZY[3] = lowZY[lTouchZY + 1];
}
if (lowZY[lTouchZY + 2] != 0){
  closegroupZY[4] = lowZY[lTouchZY + 2];
}

for (int a = 0; a<4; a++){
  if(closegroupZY[a] > highestClose){
    highestClose = closegroupZY;
  }
}

calHighestClose = calValuesZ[highestClose];

if(closegroupZY[0] != 0){
closeCoefZY[0] = calHighestClose/calValuesZ[lTouchZY - 2];
adjClosegroupZY[0] = closeCoefZY[0] * closegroupZY[0];
adjCalCloseZY[0] = calValuesZ[lTouchZY - 2] * closeCoefZY[0];
adjCloseDiffZY[0] = adjCalCloseZY[0] - adjClosegroupZY[0];
}

if(closegroupZY[1] != 0){
closeCoefZY[1] = calHighestClose/calValuesZ[lTouchZY - 1];
adjClosegroupZY[1] = closeCoefZY[1] * closegroupZY[1];
adjCalCloseZY[1] = calValuesZ[lTouchZY - 1] * closeCoefZY[1];
adjCloseDiffZY[1] = adjCalCloseZY[1] - adjClosegroupZY[1];
}

closeCoefZY[2] = calHighestClose/calValuesZ[lTouchZY];
adjClosegroupZY[2] = closeCoefZY[2] * closegroupZY[2];
adjCalCloseZY[2] = calValuesZ[lTouchZY] * closeCoefZY[2];
adjCloseDiffZY[2 = adjCalCloseZY[2] - adjClosegroupZY[2];

if(closegroupZY[3] != 0){
closeCoefZY[3] = calHighestClose/calValuesZ[lTouchZY + 1];
adjClosegroupZY[3] = closeCoefZY[3] * closegroupZY[3];
adjCalCloseZY[3] = calValuesZ[lTouchZY + 1] * closeCoefZY[3];
adjCloseDiffZY[3] = adjCalCloseZY[3] - adjClosegroupZY[3];
}
if(closegroupZY[4] != 0){
closeCoefZY[4] = calHighestClose/calValuesZ[lTouchZY + 2];
adjClosegroupZY[4] = closeCoefZY[4] * closegroupZY[4];
adjCalCloseZY[4] = calValuesZ[lTouchZY + 2] * closeCoefZY[4];
adjCloseDiffZY[4] = adjCalCloseZY[4] - adjClosegroupZY[4];
}

for (int a = 0; a < 5; a++){
  if(adjCloseDiffZY[a] > adjustedDiffZY[closeTouchZY]){
    closeTouchZY = a;
  }
}*/
//if(touchZ != 23){
//if(touchZ >= lTouchZY + 3 && lTouchZY != 0 || touchZ <= lTouchZY - 3 && lTouchZY != 0){
  //if(secondTouchZY - 1 <= lTouchZY + rdifferenceZYZY && secondTouchZY + 1 >= lTouchZY - rdifferenceZYZY){ 
  //Serial.println(secondTouchZY);
  //TouchZY = secondTouchZY;
  //Serial.println();
  //Serial.print("TouchZY changed to secondary touchZ");
  //}
//}
//}


  currentDiffZY = adjustedDiffZY[touchZ];


//Serial.println();
//Serial.print("Current Difference: ");
//Serial.print(currentDiffZY);

//Serial.println();
//Serial.print("Last Difference: ");
//Serial.print(lastDiffZY);
if(touchZ == lTouchZY){
  if(currentDiffZY - lastDiffZY > 0 && lastDiffZY > 0){
    addHalfZY  = true;
  }
  if(lastDiffZY - currentDiffZY > 0 && lastDiffZY > 0){
    subHalfZY  = true;
  }
}





//else{

//lTouchZY = touchZ;

//reset = false;

//}

}
  
  if(touchZ > 0 && noPrintZY == false){
  Serial.println();
  Serial.print("TouchZY at: ");
  if(addHalfZY  == false && subHalfZY  == false){
  Serial.print(touchZ + 25);
  lTouchZY = touchZ + 25;
  }
  if(addHalfZY  == true){
    Serial.print(touchZ + 25.5);
    lTouchZY = touchZ + 25.5;
  }
  if(subHalfZY  == true){
    Serial.print(touchZ +24.5);
    lTouchZY = touchZ + 24.5;
  }
  lTouchZY = touchZ;
  }
 
 
}
lTtimeZY =  millis();
}


}
 
     if (TouchY == true || TouchZY == true){
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

/*  if (input == 'y'){
  dPrinted = false;
  y = true
  d = false;
  n = false;
  c = false;
  t = false;
  z = false;
  b = false;
  r = false;
  wrote = false;
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
  //z = false;
  //y = false;
  //c = false;
}

if (input == 'a'){
  autocalibration = true;
}
}

if (n == true){
  if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*/){ 
    muxVals();
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
  //muxValsZ1(); 
  //muxValsZ2(); 
  //muxValsZ3(); 
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
  //Serial.print(F("Touch detected at: "));
  /*for (int g:touchedValuesY){
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

  /* if (Y1 == 1){
      Serial.print(F(" 1 ")); 
    }
    if (Y2 == 2){
      Serial.print(F(" 2 ")); 
    }
    if (Y3 == 3){
      Serial.print(F(" 3 ")); 
    }
    if (Y4 == 4){
      Serial.print(F(" 4 ")); 
    }
    if (Y5 == 5){
      Serial.print(F(" 5 ")); 
    }
    if (Y6 == 6){
      Serial.print(F(" 6 ")); 
    }
     if (Y7 == 7){
      Serial.print(F(" 7 ")); 
    }
    if (Y8 == 8){
      Serial.print(F(" 8 ")); 
    }
    if (Y9 == 9){
      Serial.print(F(" 9 ")); 
    }
    if (Y10 == 10){
      Serial.print(F(" 10 ")); 
    }
    if (Y11 == 11){
      Serial.print(F(" 11 ")); 
    }
    if (Y12 == 12){
      Serial.print(F(" 12 ")); 
    }
     if (Y13 == 13){
      Serial.print(F(" 13 ")); 
    }
    if (Y14 == 14){
      Serial.print(F(" 14 ")); 
    }
    if (Y15 == 15){
      Serial.print(F(" 15 ")); 
    }
    if (Y16 == 16){
      Serial.print(F(" 16 ")); 
    }
    if (Y17 == 17){
      Serial.print(F(" 17 ")); 
    }
    if (Y18 == 18){
      Serial.print(F(" 18 ")); 
    }
     if (Y19 == 19){
      Serial.print(F(" 19 ")); 
    }
    if (Y20 == 20){
      Serial.print(F(" 20 ")); 
    }
    if (Y21 == 21){
      Serial.print(F(" 21 ")); 
    }
    if (Y22 == 22){
      Serial.print(F(" 22 ")); 
    }
    if (Y23 == 23){
      Serial.print(F(" 23 ")); 
    }
    if (Y24 == 24){
      Serial.print(F(" 24 ")); 
    }*/
//}
//else {
  //Serial.println();
  //Serial.print(F("No touch detected!"));
//}

//BaseTime = CurrentTime; 
//printing = true; 
//}
//else {printing = false;} 
//}

if (c == true && cPrinted == false){ 
  calVals(); 
  //calVals1(); 
  //calVals2(); 
  //calVals3(); 
  cPrinted = true; 
 // singlePrint(); 
 // delay(500); 
}

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
  calMuxDiffA(); 
  calMuxDiffB(); 
  calMuxDiffC(); 
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
  Serial.print(F("The sensors Y have not been calibrated hence there is no difference available!"));
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
  Serial.print(F("The sensors ZY have not been calibrated hence there is no difference available!"));
  dPrinted = true;
  }
} 
}

if (r == true){
  if(CurrentTime - rStart >= rIntrvl && CurrentTime > rStart){
    lTouch = 0;
  n = true;
  r = false;
  Serial.println();
  Serial.print("Ending self-touch adjustment!");
  adjusted = true;
  digitalWrite(10, LOW);
}

if(written == false){
  rStart = millis();
  Serial.println();
  Serial.print("Starting self-touch adjustment!"); 
  Serial.println();
  Serial.print("Hands off the touch area please!");
  Serial.println(rStart);
  Serial.println(CurrentTime);
  written = true;
  wrote = true;
  digitalWrite(10, HIGH);
}

for(int a=0; a<24; a++){
 if(muxValues[a] <= calValues[a] - 2){
  calValues[a] = muxValues[a];
 }
// if(muxValues[a] > calValues[a]){
 //calValues[a] = muxValues[a];
 //}
  
}

 //if(muxValues[a] > calValues[a]){
 // calValues[a] = muxValues[a];
 //}
  
 
}


  /*if (calibrationStored == true){
  if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*///){ 
 
//}

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
  for (int g:touchedValuesY){
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
 
 
 
 
//MAKE lightSetting A VALUE THAT DOES NOT ALWAYS DEPEND ON DarkTotal 
 
 
 
/* have a calibration button that takes avg of all values  
sets it as the base for different times of the day when the values  
may differ. dark ( night ): 17-19, light ( day ): 22-24 
CAN PUT BASETIME = millis after calibration is executed to avoid the non printing glitch 
*/ 
 
// THE auto recalibration glitch is most likely due to expired being 0 on the first approach 
 
 
 
 
 
 
 
 


