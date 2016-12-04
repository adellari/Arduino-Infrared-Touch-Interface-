#include<SPI.h>

#include<nRF24L01.h>

#include<RF24.h>

//enclosed is the code for multiplexer switching

byte controlPinsLoneMux[] = {B00000000, //using switched digital pins to set the mux pins from 0-7 //enclosed is the code for multiplexer switching

B00010000, //7.6.5.4.3.2.1.0

B00001000,

B00011000,

B00000100,

B00010100,

B00001100,

B00011100

};

int muxValuesA[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding 1-8
int muxValuesB[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding 9-17
int muxValuesC[] = {0, 0, 0, 0, 0, 0, 0, 0}; //holding 18-24
float muxValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float calValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int calValuesA[] = {0,0,0,0,0,0,0,0};
int calValuesB[] = {0,0,0,0,0,0,0,0};
int calValuesC[] = {0,0,0,0,0,0,0,0};
int reading = 0;
float lTouch = 0;
unsigned long lTtime = 0;
unsigned long lTclock = 0;
bool reset = true;
int calibrate;
int buttonPin = 17;

bool printing;
int lightSetting;
int totalmuxA;
int totalmuxB;
int totalmuxC;
char avgMuxA;
char avgMuxB;
char avgMuxC;
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
unsigned long BaseTime;
unsigned long CurrentTime;
unsigned long PreviousTime;
unsigned long expired;
bool expire;
bool printrecal;
unsigned long lastwrng;
unsigned long calStart;
unsigned long wrngStrt;
unsigned int wrngIntrvl = /*900000*/ 30000;
unsigned int expireIntrvl = /*7200000*/ 60000;
unsigned long expireStrt;
unsigned long expireLst;
bool aligned;
bool pushed;
double IRsetting;
unsigned long StartTime;
const int rxOutputInt = 400;
bool calibrationStored = false;
bool adjusted = false;

bool n = false;
bool t = false;
bool c = false;
bool d = false;
bool r = false;
bool s = false;

bool cPrinted = true;
bool dPrinted = true;
bool wrote = false;
bool written = true;
unsigned long lastSend;
int Low = 0;
int Low1 = 0;
int Low2 = 0;
int Low3 = 0;
int Low4 = 0;
int lTrend = 0;
float lastX = 0;
float lastY = 0;
unsigned long offTime;
unsigned long lastOFFtime;
unsigned long onTime;
unsigned long touchTime;
unsigned long lastRead = 0;
int highTouch1 = 0;
int highTouch2 = 0;
int highTouch3 = 0;
int lowTouch1 = 0;
int lowTouch2 = 0;
int lowTouch3 = 0;
int lastDiff = 0;
int currentDiff = 0;
int decrease = 0;
int increase = 0;
int times = 0;

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

bool firstTX = true;

RF24 radio(8, 9);

const uint64_t rAddress[] = {0xD31373D0A1LL, 0xD31373D0B2LL, 0xD313373D0C3LL};

const uint64_t wAddress = 0xD31373D0D4LL;

//const byte rAddress1[6] = "TRAN1";

//const byte rAddress2[6] = "TRAN2";

//const byte rAddress3[6] = "TRAN3";

void setup() {

StartTime = micros();

Serial.begin(9600);

radio.begin();

radio.setPALevel (RF24_PA_LOW);

DDRD = B00011100; //setting digital pins 6-4 to OUTPUT

BaseTime = millis();

// pinMode(buttonPin, INPUT);

//pinMode(5, OUTPUT);

//digitalWrite(5, HIGH);

//pinMode(6, OUTPUT);

//pinMode(5, OUTPUT);

// pinMode(4, OUTPUT);

pinMode(10, OUTPUT);

pinMode(14, OUTPUT);

digitalWrite(14, HIGH);

pinMode(15, OUTPUT);

digitalWrite(15, HIGH);

radio.openReadingPipe(1, rAddress[0]);

radio.openReadingPipe(2, rAddress[1]);

radio.openReadingPipe(3, rAddress[2]);

//radio.openReadingPipe(2, rAddress[1]);

//radio.openReadingPipe(3, rAddress[2]);

radio.openWritingPipe(wAddress);

// radio.openWritingPipe(1, wAddress[0]);

//radio.openWritingPipe(2, wAddress[1]);

// radio.openWritingPipe(3, wAddress[2]);

//radio.startListening();

}

void setPinMux(int outputMuxPin) {

PORTD = controlPinsLoneMux[outputMuxPin]; //setting sewdxa{;;e'f}digital 4-6 to follow the truth table instructions

}

void muxVals() {

Serial.println();

Serial.println(F("Values for multiplexer:"));

Serial.println(F("==========================="));

for (int a = 0; a < 24; a++) {

Serial.print(a);

Serial.print(F(" = "));

Serial.println(/*calValues[a]/ */muxValues[a]);

}

Serial.println(F("==========================="));

}

void muxVals1() {

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

void calMuxDiffA() {

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

Serial.print(a);

Serial.print(F(" = "));

Serial.println(muxValuesB[a] - calValuesB[a]);

}

Serial.println(F("==========================="));

}

void calMuxDiffC(){

for (int a = 0; a < 8; a++){

Serial.print(a);

Serial.print(F(" = "));

Serial.println(muxValuesC[a] - calValuesC[a]);

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

if (DarkAvg = 25){

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

Low = 0;

Low1 = 0;

Low2 = 0;

Low3 = 0;

Low4 = 0;

int threes = 0;

int twoos = 0;

int ones = 0;

int zeroes = 0;

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

calValuesA[0] = muxValuesA[0];

calValuesA[1] = muxValuesA[1];

calValuesA[2] = muxValuesA[2];

calValuesA[3] = muxValuesA[3];

calValuesA[4] = muxValuesA[4];

calValuesA[5] = muxValuesA[5];

calValuesA[6] = muxValuesA[6];

calValuesA[7] = muxValuesA[7];

/*Assign Calibration Values B*/

calValuesB[0] = muxValuesB[0];

calValuesB[1] = muxValuesB[1];

calValuesB[2] = muxValuesB[2];

calValuesB[3] = muxValuesB[3];

calValuesB[4] = muxValuesB[4];

calValuesB[5] = muxValuesB[5];

calValuesB[6] = muxValuesB[6];

calValuesB[7] = muxValuesB[7];

/*Assign Calibration Values C*/

calValuesC[0] = muxValuesC[0];

calValuesC[1] = muxValuesC[1];

calValuesC[2] = muxValuesC[2];

calValuesC[3] = muxValuesC[3];

calValuesC[4] = muxValuesC[4];

calValuesC[5] = muxValuesC[5];

calValuesC[6] = muxValuesC[6];

calValuesC[7] = muxValuesC[7];

calValues[0] = calValuesA[0];

calValues[1] = calValuesA[1];

calValues[2] = calValuesA[2];

calValues[3] = calValuesA[3];

calValues[4] = calValuesA[4];

calValues[5] = calValuesA[5];

calValues[6] = calValuesA[6];

calValues[7] = calValuesA[7];

calValues[8] = calValuesB[0];

calValues[9] = calValuesB[1];

calValues[10] = calValuesB[2];

calValues[11] = calValuesB[3];

calValues[12] = calValuesB[4];

calValues[13] = calValuesB[5];

calValues[14] = calValuesB[6];

calValues[15] = calValuesB[7];

calValues[16] = calValuesC[0];

calValues[17] = calValuesC[1];

calValues[18] = calValuesC[2];

calValues[19] = calValuesC[3];

calValues[20] = calValuesC[4];

calValues[21] = calValuesC[5];

calValues[22] = calValuesC[6];

calValues[23] = calValuesC[7];

for (int a:calValuesA){

if (a == 0) ++zeroes;

if (a == 1) ++ones;

if (a == 2) ++twoos;

if (a == 3) ++threes;

}

for (int b:calValuesB){

if (b == 0) ++zeroes;

if (b == 1) ++ones;

if (b == 2) ++twoos;

if (b == 3) ++threes;

}

for (int c:calValuesC){

if (c == 0) ++zeroes;

if (c == 1) ++ones;

if (c == 2) ++twoos;

if (c == 3) ++threes;

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

//if (zeroes /*+ ones + twoos + threes*/ == 0){

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

//Serial.println();

//Serial.print(F("Calibration Failed!"));

//for(int a = 0; a < 8; a++){

//if(nonCalibrated[a] == true){

//Serial.println();

//Serial.print("This point couldn't be calibrated: ");

//Serial.print(a);

//}

//}

Serial.println();

Serial.print(F("There are this many zeroes: "));

Serial.print(zeroes);

Serial.println();

Serial.print(F("There are this many ones: "));

Serial.print(ones);

Serial.println();

Serial.print(F("There are this many twoos: "));

Serial.print(twoos);

/*Serial.println();

Serial.print(F("There are this many threes: "));

Serial.print(threes);

*/

//}
autocalibration = false;
return paired;

}

void loop() {

lTclock = millis();

char input = 'O';

int readInt = 100;

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

double Y1 = 0;

double Y2 = 0;

double Y3 = 0;

double Y4 = 0;

double Y5 = 0;

double Y6 = 0;

double Y7 = 0;

double Y8 = 0;

double Y9 = 0;

double Y10 = 0;

double Y11 = 0;

double Y12 = 0;

double Y13 = 0;

double Y14 = 0;

double Y15 = 0;

double Y16 = 0;

double Y17 = 0;

double Y18 = 0;

double Y19 = 0;

double Y20 = 0;

double Y21 = 0;

double Y22 = 0;

double Y23 = 0;

double Y24 = 0;

bool Yhalf1 = false;

bool Yhalf2 = false;

bool Yhalf3 = false;

bool Yhalf4 = false;

bool Yhalf5 = false;

bool Yhalf6 = false;

bool Yhalf7 = false;

bool Yhalf8 = false;

bool Yhalf9 = false;

bool Yhalf10 = false;

bool Yhalf11 = false;

bool Yhalf12 = false;

bool Yhalf13 = false;

bool Yhalf14 = false;

bool Yhalf15 = false;

bool Yhalf16 = false;

bool Yhalf17 = false;

bool Yhalf18 = false;

bool Yhalf19 = false;

bool Yhalf20 = false;

bool Yhalf21 = false;

bool Yhalf22 = false;

bool Yhalf23 = false;

bool Yhalf24 = false;

int Touch = NULL;

bool tapped = false;

unsigned long tappedTime = NULL;

bool fingerDown = false;

unsigned long sendInterval = 20;

unsigned long rxOutputStrtUL;

unsigned long rxOutputStrtUR;

unsigned long rxOutputStrtLR;

unsigned long RXlastOut;

bool ULdone;

bool URdone;

bool LRdone;

float xCoordinate[2] = {0, 0};

float yCoordinate[2] = {0, 0};

float zxCoordinate[2] = {0, 0};

float zyCoordinate[2] = {0, 0};

float test[] = {5,9};

CurrentTime = millis();

//calibrate = digitalRead(buttonPin);

byte pipeID = 0;

byte holdRX;

float rByte[2] = {0,0};

int UL;

int UR;

int LL;

int LR;

bool received = false;

bool receivedUL = false;

bool receivedUR = false;

bool receivedLR = false;

bool globalTouch = false;

//if (CurrentTime >= sendInterval + lastSend){

radio.startListening();

/*if(radio.available(&pipeID)){

//Serial.println("received");

//Serial.print(pipeID);

if (lastOFFtime == 0){

lastOFFtime = millis();

}

onTime = millis();

received = true;

globalTouch = true;

radio.read(rByte, sizeof(rByte));

if (pipeID == 1){

//rxOutputStrtUL = millis();

//UL = rByte;

xCoordinate[0] = rByte[1];

xCoordinate[1] = 1;

//Serial.println("received 1");

}

if (pipeID == 2 && rByte[0] == 0){

//rxOutputStrtUL = millis();

//UL = rByte;

xCoordinate[0] = rByte[1];

xCoordinate[1] = 1;

//Serial.println("received 2");

}

if (pipeID == 2 && rByte[0] == 1){

//rxOutputStrtUL = millis();

//UL = rByte;

zxCoordinate[0] = rByte[1];

zxCoordinate[1] = 3;

//Serial.println("received 2");

}

if (pipeID == 3 && rByte[0] == 0){

//rxOutputStrtUL = millis();

//UL = rByte;

yCoordinate[0] = rByte[1];

yCoordinate[1] = 2;

//Serial.println("received 3");

}

if (pipeID == 3 && rByte[0] == 1){

//rxOutputStrtUL = millis();

//UL = rByte;

zyCoordinate[0] = rByte[1];

zyCoordinate[1] = 4;

//Serial.println(F("received 3"));

}

lastSend = millis();

//radio.stopListening();

}

if(yCoordinate[0] != 0){

radio.stopListening();

if(radio.write(yCoordinate, sizeof(yCoordinate))){

radio.startListening();

Serial.println("sent Y!");

}

else{

//Serial.println("failed sending Y");

radio.startListening();

}

}

if(xCoordinate[0] != 0){

radio.stopListening();

if(radio.write(xCoordinate, sizeof(xCoordinate))){

radio.startListening();

Serial.println("sent X!");

}

else{

Serial.println("failed sending X");

radio.startListening();

}

}

if(zxCoordinate[0] != 0){

radio.stopListening();

if(radio.write(zxCoordinate, sizeof(zxCoordinate))){

radio.startListening();

}

else{

Serial.println("failed sending ZX");

radio.startListening();

}

}

if(zyCoordinate[0] != 0){

radio.stopListening();

if(radio.write(zyCoordinate, sizeof(zyCoordinate))){

radio.startListening();

}

else{

Serial.println("failed sending ZY");

radio.startListening();

}

}*/

//}

/*

else {

radio.stopListening();

if(radio.write(test, sizeof(test))){

Serial.println();

Serial.print("Sent test byte");

}

if (xCoordinate[0] != 0 || yCoordinate[0] != 0){

Serial.println(rByte[1]);

if(!radio.write(xCoordinate, sizeof(xCoordinate))){

Serial.println();

Serial.print(F("Failed to send X coordinate: "));

Serial.print(xCoordinate[0]);

}

else {

Serial.println("sent the X coordinate");

}

if(!radio.write(yCoordinate, sizeof(xCoordinate))){

Serial.println();

Serial.print(F("Failed to send Y coordinate: ")); //put start touch time in the if there is a message received and have it run once and after the touch and calculations clear it

Serial.print(yCoordinate[0]);

}

}

}



if (globalTouch == false && Touch == false){

offTime = millis();

if(onTime - lastOFFtime <= 150 && onTime > 0 && lastOFFtime > 0){

tapped = true;

lastOFFtime = 0;

radio.stopListening();

float clicked[] = {0,5};

radio.write(clicked, sizeof(clicked));

}

}*/

// if (pipeID == 1){

//###if (ULdone == true || firstTX == true){

// rxOutputStrtUL = millis();

//###}

//###radio.read(&UL, 1);

//UL = rByte;

//receivedUL = true;

//Serial.println();

//Serial.print(F("Sensors UL: "));

//Serial.print(UL/*rByte*/);

//}

//if (pipeID == 2){

//###if (URdone == true || firstTX == true){

//rxOutputStrtUR = millis();

//###}

//###radio.read(&UR, 1);

//UR = rByte;

// receivedUR = true;

// Serial.println();

//Serial.print(F("Sensors UR: "));

//Serial.print(UR/*rByte*/);

//}

//if (pipeID == 3){

//###if (LRdone == true || firstTX == true){

//rxOutputStrtLR = millis();

//###}

//LR = rByte;

//receivedLR = true;

//Serial.println();

//Serial.print(F("Sensors LR: "));

//Serial.print(LR/*rByte*/);

//}

/*

if (receivedUL = true && CurrentTime - rxOutputInt <= rxOutputStrtUL || UL!= 0 && RXlastOut != 0 && CurrentTime - rxOutputInt <= RXlastOut){

Serial.println();

Serial.print(F("Sensors UL: "));

Serial.print(UL/*rByte);

ULdone = true;

RXlastOut = millis();

}

if (receivedUR = true && CurrentTime - rxOutputInt <= rxOutputStrtUR || UR!= 0 && RXlastOut != 0 && CurrentTime - rxOutputInt <= RXlastOut){

Serial.println();

Serial.print(F("Sensors UR: "));

Serial.print(UR/*rByte);

URdone = true;

RXlastOut = millis();

}

if (receivedLR = true && CurrentTime - rxOutputInt <= rxOutputStrtLR || LR!= 0 && RXlastOut != 0 && CurrentTime - rxOutputInt <= RXlastOut){

Serial.println();

Serial.print(F("Sensors LR: "));

Serial.print(LR/*rByte);

LRdone = true;

RXlastOut = millis();

}

*/

/*

Serial.println();

Serial.print(F("PipeID = "));

Serial.print(pipeID);

}

if (radio.available(&pipeID)){

holdRX = pipeID;

if(holdRX == rAddress1){

radio.read(&UL , sizeof(rByte));

}

}

if(holdRX == rAddress2){

radio.read(&UR , sizeof(rByte));

}

if(holdRX == rAddress3){

radio.read(&LR , sizeof(rByte));

}

received = true;

}

*/

if (/*calibrate == HIGH ||*/ autocalibration == true){

expire = false;



calStart = millis();

expireStrt = millis();

push = true;

pushed = true;

expire = false;

firstWrng = false;

//digitalWrite(18, LOW); //emitters

calibrated = 2;

calArray();
autocalibration = false;
/*if (!calArray){

Serial.println();

Serial.print(F("Sensors are not aligned!"));

}

else {

Serial.println();

Serial.print(F("Sensors are aligned!"));

}

*/

//BaseTime = millis();

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

//digitalWrite(18, HIGH);

pushed = false;

//digitalWrite(10, LOW);

//break;

}

//}

}

if (CurrentTime > expireStrt && CurrentTime - expireStrt >= 7200000 /*30000*/ && CurrentTime - expireStrt >= expireIntrvl && firstWrng != true ){
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

//if(lastRead + readInt <= CurrentTime){

for (int a = 0; a < 8; a++) {

setPinMux(a);

muxValuesA[a] = analogRead(5);

muxValuesB[a] = analogRead(6);

muxValuesC[a] = analogRead(7);

//reading = analogRead(2);

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

lastRead = millis();

}

//}

totalmuxA = (muxValuesA[0] + muxValuesA[1] + muxValuesA[2] +

muxValuesA[3] + muxValuesA[4] + muxValuesA[5] + muxValuesA[6]

+ muxValuesA[7]);

avgMuxA = totalmuxA/8;

totalmuxB = (muxValuesB[0] + muxValuesB[1] + muxValuesB[2] +

muxValuesB[3] + muxValuesB[4] + muxValuesB[5] + muxValuesB[6]

+ muxValuesB[7]);

avgMuxB = totalmuxB/8;

totalmuxC = (muxValuesC[0] + muxValuesC[1] + muxValuesC[2] +

muxValuesC[3] + muxValuesC[4] + muxValuesC[5] + muxValuesC[6]

+ muxValuesC[7]);

avgMuxC = totalmuxC/8;

/*

for (int t1 = 0; t1 > 8; t1++){

totalmuxA += muxValuesA[t1];

}

*/

//DarkTotal = totalmuxA + totalmuxB + totalmuxC;

//DarkAvg = DarkTotal/24;

//avgD = DarkAvg;

// eight = analogRead(2);

//if (DarkTotal >= 408 && DarkTotal <= 720){

//lightSetting = DarkAvg;

//}

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
float lastMoves[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lowCons[] = {0,0,0};
int highCons[] = {0,0,0};
int replaced[] = {0,0,0,0};
int calculatedReplace = 0;
bool addHalf = false;
bool subHalf = false;
int secondTouch = 50;

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

if(times > 23){
  times = 0;
  decrease = 0;
  increase = 0;
}
else{
  for(int a = 0; a < 24; a++){
    if(lastMoves[a] > 0){
      increase++;
    }
    if(lastMoves[a] < 0){
      decrease++;
    }
  }
}
//if(low[0] > 0 && low[1] == 0){
  //if(low[0] >= 0 + 4 && low[0] <= 23 - 4){
     //calValues[low[0]] = muxValues[low[0]];
    // low[0] = 0;
  //}
//}

/*for(int a = 0; a < 5; a++){

if(bLow[a] + 1 == bLow[a+1] ||bLow[a] - 1 == bLow[a - 1]){

low[a] = bLow[a];

}

else{

if(bLow[a] != 0){

group[bLow[a]] = 0;

}

bLow[a] = 0;

}

}*/

if(done == true){
/*if(lTtime + 50 >= lTclock && lTtime > 0 && lTclock > 0){

if(low[2] != 0){
  for(int a = 0; a < 10; a++){
    if(low[a] >= lTouch + 3 && lTouch != 0 || low[a] <= lTouch - 3 && lTouch != 0){
      //for(int b = 0; b < 24; b++){
      //if(group[b] == muxValues[low[a]]){
        //group[b] = 0;
      //}
      //}
      low[a] = 0;
    }
  }
}
}*/
//Serial.println(lTtime);

for(int a = 0; a < 24; a++){

if (group[a] > currentHighestGroup){

currentHighestGroup = group[a];

highestPoint = a;
//Serial.println();
//Serial.print("Highest Point: ");
//Serial.print(highestPoint);
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

//Serial.print("Coefficient 5: ");

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

if( s == true){
  Serial.println();
  Serial.println("Starting Sensitivity Troubleshooting");
  for (int a = 1; a < 24; a ++){
    Serial.println();
    Serial.print(a);
    Serial.print(" = ");
    Serial.print(adjustedDiff[a]); 
  }
}

for(int a = 0; a < 24; a++){

if (adjustedDiff[a] > adjustedDiff[touch]){

touch = a;

}


}

for (int a = 0; a < 24; a++){
  if (adjustedDiff[a] > adjustedDiff[secondTouch] && adjustedDiff[a] != adjustedDiff[touch]){
secondTouch = a;
}
}

if(secondTouch < 50){
Serial.println();
Serial.print("secondary touch: ");
Serial.print(secondTouch);
}

Serial.println();
Serial.print("Last Touch: ");
Serial.print(lTouch);

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

//Serial.println();
//Serial.print("Secondary touch: ");
//Serial.print(calculatedReplace);





lTrend = touch - lTouch;


currentDiff = adjustedDiff[touch];

int difference = lTouch - touch; 
int rdifference = sqrt(sq(difference));


if(lTtime + 50 >= lTclock && lTtime > 0 && lTclock > 0){
///int closeTouch = 0;
int closeGroup[] = {0,0,0,0,0};
//int highestClose = 0;
//int closeCoef[] = {0,0,0,0,0,0};
//int calHighestClose = 0;
//int adjCalClose[] = {0,0,0,0,0};
//int adjCloseGroup[] = {0,0,0,0,0};
int closeTouch = 0;
int rcloseTouch = 0;

closeGroup[0] = lTouch - 2;
closeGroup[1] = lTouch - 1;
closeGroup[2] = lTouch;
closeGroup[3] = lTouch + 1;
closeGroup[4] = lTouch + 2;

for (int a = 0; a < 5; a++){
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

if(touch < 24 && noPrint == false){
//Serial.println();

//Serial.print("Touch's Difference: ");
//Serial.print(adjustedDiff[touch]);

Serial.println();

Serial.print("Touch at: ");

if(subHalf == true){
  Serial.print(touch + .5);
  yCoordinate[0] = touch + .5;
}

if(addHalf == true){
  Serial.print(touch + 1.5);
  yCoordinate[0] = touch + 1.5;
}

if(subHalf == false && addHalf == false){
Serial.print(touch + 1);
yCoordinate[0] = touch + 1;
}

lTouch = touch;

}

}

lTtime = millis();

times++;
}

}

/*
if(yCoordinate[0] != 0){

yCoordinate[1] = 2;

radio.stopListening();

if(radio.write(yCoordinate, sizeof(yCoordinate))){

Serial.println("Sent y Coordinate");

radio.startListening();

}

}*/



if(lTtime + 2000 <= lTclock && lTtime > 0 && lTclock > 0){
lTouch = 0;
}


if (Touch == true){

fingerDown = true;

}

/*

bool count1 = false;

bool count2 = false;

bool count3 = false;

bool count4 = false;

bool count5 = false;

bool count6 = false;

bool count7 = false;

bool count8 = false;

bool count9 = false;

bool count10 = false;

bool count11 = false;

bool count12 = false;

bool count13 = false;

bool count14 = false;

bool count15 = false;

bool count16 = false;

bool count17 = false;

bool count18 = false;

bool count19 = false;

bool count20 = false;

bool count21 = false;

bool count22 = false;

bool count23 = false;

bool count24 = false;

for (int z:touchedValues){

if (z == 1) count1 = true

}

*/

// int Touch = X1 + X2 + X3 + X4 + X5 + X6 + X7 + X8 + X9 + X10

// + X11 + X12 + X13 + X14 + X15 + X16 + X17 + X18 + X19 + X20 +

// X21 + X22 + X23 + X24;

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

s = false;
n = true;
t = false;
c = false;
d = false;
r = false;
wrote = false;

}

if (input == 't'){

t = true;

n = false;

c = false;

d = false;

r = false;

wrote = false;

}

if (input == 'd'){

dPrinted = false;

d = true;

n = false;

c = false;

t = false;

r = false;

wrote = false;

}

if (input == 'c'){

cPrinted = false;

c = true;

n = false;

t = false;

d = false;

r = false;

wrote = false;

}

if(input == 's'){
  s = true;
  n = false;
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

//input = 'O';

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

if (t == true){

if (CurrentTime - BaseTime >= OutputInterval /* && pushed == false*/){

if (Touch == true){

Serial.println();

Serial.print(F("Touch detected at: "));

/*for (int g:touchedValues){

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

}*/

if (Y1 == 1){

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

if (c == true && cPrinted == false){

calVals1();

calVals2();

calVals3();

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

Serial.print(F("The sensors have not been calibrated hence there is no difference available!"));

dPrinted = true;

}

}

}

if (r == true){

if(CurrentTime - rStart >= rIntrvl && CurrentTime > rStart){
lTouch = 0;

n = true;

r = false;

reset = true;

Serial.println();

Serial.print("Ending self-touch adjustment!");

adjusted = true;


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

}

for(int a=0; a<24; a++){

if(muxValues[a] <= calValues[a] - 2){

calValues[a] = muxValues[a];

}

//if(muxValues[a] > calValues[a]){

//calValues[a] = muxValues[a];

//}

}

}

lTclock = millis();
radio.startListening();
}

//MAKE lightSetting A VALUE THAT DOES NOT ALWAYS DEPEND ON DarkTotal

/* have a calibration button that takes avg of all values

sets it as the base for different times of the day when the values

may differ. dark ( night ): 17-19, light ( day ): 22-24

CAN PUT BASETIME = millis after calibration is executed to avoid the non printing glitch

*/

// THE auto recalibration glitch is most likely due to expired being 0 on the first approach

