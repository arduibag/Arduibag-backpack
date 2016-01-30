/*
  Arduibag.cpp - Library for Arduibag connected backpack for bikers.
  Created by Michaël d'AURIA, 2016-30-01
  Released into the public domain.
*/

#include <Arduino.h>
#include <Arduibag.h> //Arduibag library

//Screen configuration
#define CLK 8  
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

//Temperature sensor configuration
#define DHTPIN 12
#define DHTTYPE DHT11   // DHT 11 sensor type

//Realtime clock configuration
#define RSDA 4
#define RSCL 5

//Hardware setup
//Display configuration
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

//Realtime clock configuration
DS3231  rtc(SDA, SCL);
Time ti;
Time ti_init;

//Temperature sensor configuration
DHT dht(DHTPIN, DHTTYPE);

Arduibag::Arduibag(bool LANG)
{
  _LANG = LANG;
}

//----------------------------------
// HARDWARE INITIALIZATION
//----------------------------------
void Arduibag::begin() {

   
   
  // LED Matrix Setup  
  matrix.begin();
  
  // DHT temperature sensor Setup
  dht.begin();
  
  //Realtime clock Setup
  rtc.begin();

  //keep current time for travel time
  ti_init = rtc.getTime();
}

//----------------------------------
// CLEAR SCREEN
//----------------------------------
void Arduibag::clearScreen() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));
}

//----------------------------------
// DISPLAY LOGO
//----------------------------------
void Arduibag::displayLogo() {
  short x, y, c=0;
  int color;

  matrix.fillScreen(matrix.Color333(0, 0, 0));
  for (y=0;y<16;y++) {
      for (x=0;x<32;x++) {
        color = pgm_read_word_near(Logo + c);
        //if(color==0x07E3) color = matrix.Color333(6, 0, 0);
        matrix.drawPixel(x , y, color);
      c++;
      }
   }
   
  delay(3000);
}

//----------------------------------
// BLUETOOTH SYNCHRONIZATION
//----------------------------------
void Arduibag::waitRemote() {
  

  matrix.fillScreen(matrix.Color333(0, 0, 0));
     
  matrix.drawRoundRect(1, 2, 30, 13, 1, matrix.Color333(1, 1, 1));
  
  //Bluetooth logo
  matrix.fillRoundRect(6, 4, 7, 9, 1, matrix.Color333(0, 0, 1));
  matrix.drawFastVLine(9, 5 , 7, matrix.Color333(3, 3, 3));
  matrix.drawLine(7, 6, 8, 7, matrix.Color333(3, 3, 3));
  matrix.drawLine(10, 6, 11, 7, matrix.Color333(3, 3, 3));
  matrix.drawPixel(10, 8, matrix.Color333(3, 3, 3));
  matrix.drawLine(8, 9, 7, 10, matrix.Color333(3, 3, 3));
  matrix.drawLine(11, 9, 10, 10, matrix.Color333(3, 3, 3));
  
  //Level BT 0
  matrix.drawCircle(16, 8, 1, matrix.Color333(3, 3, 3));
  
  while(!Serial.available()) {
    matrix.fillRect(18, 4, 8, 9, matrix.Color333(0, 0, 0));
    delay(100);
    
    //Level BT 1
    matrix.drawPixel(18, 6, matrix.Color333(3, 3, 3));
    matrix.drawFastVLine(19, 7 , 3, matrix.Color333(3, 3, 3));
    matrix.drawPixel(18, 10, matrix.Color333(3, 3, 3));
    delay(100);
    
    //Level BT 2
    matrix.drawLine(20, 5, 21, 6, matrix.Color333(3, 3, 3));
    matrix.drawFastVLine(22, 7 , 3, matrix.Color333(3, 3, 3));
    matrix.drawLine(21, 10, 20, 11, matrix.Color333(3, 3, 3));
    delay(100);
    
    //Level BT3
    matrix.drawLine(22, 4, 24, 6, matrix.Color333(3, 3, 3));
    matrix.drawFastVLine(25, 7 , 3, matrix.Color333(3, 3, 3));
    matrix.drawLine(24, 10, 22, 12, matrix.Color333(3, 3, 3));
    delay(100);
  }
  //Send a character to exit the loop
  Serial.write('a');
  matrix.fillRect(1, 2, 30, 13, matrix.Color333(0, 0, 0));

}

//----------------------------------
// NIGHT or DAY SELECTION
//----------------------------------
bool Arduibag::chooseNightDay() {

  short c=1, a, x;
 
 //Display Night & Day selection
  matrix.drawRoundRect(1, 2, 30, 13, 1, matrix.Color333(1, 1, 1));
  displayMoon(1);
  displaySun(1);
    
  while(c) {
    //Arrows
    matrix.fillTriangle (12, 8, 14, 6, 14, 10, matrix.Color333(2, 2, 2));
    matrix.fillRect(16, 6, 3, 5, matrix.Color333(0, 0, 0));
    Serial.write('A');
    delay(500);
    matrix.fillTriangle (18, 8, 16, 6, 16, 10, matrix.Color333(2, 2, 2));
    matrix.fillRect(12, 6, 3, 5, matrix.Color333(0, 0, 0));
    Serial.write('B');
    delay(500);
    
    if (Serial.available()){
      a = Serial.read();
      if(a=='A') {
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.drawRoundRect(1, 2, 30, 13, 1, matrix.Color333(1, 1, 1));
        matrix.fillTriangle (12, 8, 14, 6, 14, 10, matrix.Color333(6, 6, 6));
        for (x=1; x<4; x++) {
          displayMoon(0);
          Serial.write('A');
          delay(100);
          displayMoon(1);
          Serial.write('0');
          delay(100);
        }
        return 0;
      }
      if(a=='B') {
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.drawRoundRect(1, 2, 30, 13, 1, matrix.Color333(1, 1, 1));
        matrix.fillTriangle (18, 8, 16, 6, 16, 10, matrix.Color333(6, 6, 6));
        for (x=1; x<4; x++) {
          displaySun(0);
          Serial.write('B');
          delay(100);
          displaySun(1);
          Serial.write('0');
          delay(100);
        }
        return 1;
      }
      if (a=='C') {
        Serial.write('0');
        displayDanger();
      }
      if (a=='D') {
        Serial.write('0');
        displayInfos();
      }
    }
    
  }
}

//----------------------------------
// DISPLAY READY MESSAGE
//----------------------------------
void Arduibag::displayReady() {

  char a;
    
  //Ready to fight !
    matrix.setTextSize(1);
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    
    for (a=0;a<8;a++){
      matrix.setCursor(1, 5);
      matrix.setTextColor(matrix.Color333(0, a, 0));
      matrix.print("Ready");
      delay(80-(a*10));
    }
    delay(500);
    for (a=7;a>=0;a--){
      matrix.setCursor(1, 5);
      matrix.setTextColor(matrix.Color333(0, a, 0));
      matrix.print("Ready");
      delay(80-(a*10));
    }
}

//----------------------------------
// DISPLAY INFOS
//----------------------------------
void Arduibag::displayInfos() {
    
    while (Serial.read()!='a') {  
        displayTime();
        delay(2000);
        displayTravelTime();
        delay(2000);
        displayTemperature(1);
        delay(2000);
        displayHumidity();
        delay(2000);
    }
    
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.drawRoundRect(1, 2, 30, 13, 1, matrix.Color333(1, 1, 1));
    displayMoon(1);
    displaySun(1);
}

//----------------------------------
// DISPLAY TRAVEL TIME
//----------------------------------
void Arduibag::displayTravelTime() {
  char ti_range;
  
    ti = rtc.getTime();
  
    ti_range = (rtc.getUnixTime(ti) - rtc.getUnixTime(ti_init)) / 60;
  
        matrix.fillScreen(matrix.Color333(0, 0, 0));
       
        matrix.drawCircle(6, 8, 4, matrix.Color333(1, 1, 1));
        matrix.fillTriangle(6, 8, 6, 5, 8, 6, matrix.Color333(0, 1, 1));
        
        matrix.setCursor(14, 5);
        matrix.setTextSize(1);
        matrix.setTextColor(matrix.Color333(1, 1, 0));
        if(ti_range<10)matrix.print("0");
        matrix.print(ti_range, DEC);
        matrix.print("\""); 
}

//----------------------------------
// DISPLAY TIME
//----------------------------------
void Arduibag::displayTime() {
      
        // Display Time
        ti = rtc.getTime();
        matrix.fillScreen(matrix.Color333(0, 0, 0)); 
        matrix.setCursor(1, 5);
        matrix.setTextSize(1);
        matrix.setTextColor(matrix.Color333(3, 0, 2));
        if(ti.hour<10)matrix.print(" ");
        matrix.print(ti.hour, DEC);
        matrix.print(":");
        if(ti.min<10)matrix.print("0");
        matrix.print(ti.min, DEC);
}

//----------------------------------
// DISPLAY TEMPERATURE
//----------------------------------
void Arduibag::displayTemperature(bool captor) {      
      int t;
      // Read temperature as Celsius (the default)
      if (captor) {
          t = dht.readTemperature()-2; //light blue 
          //-2° for my personnal sensor
      } else {
        t = rtc.getTemp()-1; //green
      }
      //Temperature (°C)
      
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(9, 5);
      matrix.setTextSize(1);
      matrix.setTextColor(matrix.Color333(0, 2, captor));
      matrix.drawCircle(22, 6, 1, matrix.Color333(0, 2, captor));
      matrix.print(t);
      matrix.setCursor(25, 5);
      matrix.print("C");
      matrix.drawRoundRect(3, 2, 3, 10, 1, matrix.Color333(1, 1, 1));
      matrix.fillCircle (4, 11, 2, matrix.Color333(1, 1, 1));
      matrix.fillCircle (4, 11, 1, matrix.Color333(1, 0, 0));
      matrix.drawFastVLine(4, 6, 4, matrix.Color333(1, 0, 0));     
}

//----------------------------------
// DISPLAY HUMIDITY
//----------------------------------
void Arduibag::displayHumidity() {
     
      //Read humidity
      int h = dht.readHumidity();
      
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(12, 5);
      matrix.setTextSize(1);
      matrix.setTextColor(matrix.Color333(3, 3, 3));
      matrix.print(h);
      matrix.print("%"); 
      matrix.fillCircle (6, 9, 3, matrix.Color333(0, 1, 1));
      matrix.fillTriangle (6, 3, 4, 7, 8, 7, matrix.Color333(0, 1, 1));
      matrix.drawPixel(4, 8, matrix.Color333(0, 0, 1));
      matrix.drawPixel(5, 7, matrix.Color333(0, 0, 1));
      matrix.drawPixel(4, 9, matrix.Color333(0, 0, 1));
}

//----------------------------------
// DISPLAY DANGER
//----------------------------------
void Arduibag::displayDanger() {
  short inc;
  
  matrix.setTextWrap(false);

  while(Serial.read()!='a'){
    
    for (inc=0;inc<5;inc++) {
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      delay(500);
      matrix.fillTriangle (15, 0, 6, 15, 24, 15, matrix.Color333(7, 0, 0));
      matrix.fillTriangle (15, 3, 9, 13, 21, 13, matrix.Color333(7, 3, 0));
      matrix.fillTriangle (15, 6, 12, 11, 18, 11, matrix.Color333(0, 0, 0));
      delay(500);
    }
    
    for (inc=0;inc<130;inc++) {
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(32-inc, 1);
      matrix.setTextSize(2);
      matrix.setTextColor(matrix.Color333(6,1,0));
      if(_LANG)matrix.print("RALENTIR!"); else matrix.print("SLOW DOWN!");
      delay(10);
    }
  }
  
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.drawRoundRect(1, 2, 30, 13, 1, matrix.Color333(1, 1, 1));
      displayMoon(1);
      displaySun(1);
}


//----------------------------------
// DISPLAY MOON
//----------------------------------

void Arduibag::displayMoon(short on) {
  //Moon
  if (on) {
    matrix.fillCircle (7, 8 , 4, matrix.Color333(0, 1, 1));
    matrix.fillCircle (9, 8 , 2, matrix.Color333(0, 0, 0));
  } else {
    matrix.fillRect(3, 3, 8, 10, matrix.Color333(0, 0, 0));   
  }
}

//---------------a-------------------
// DISPLAY SUN
//----------------------------------

void Arduibag::displaySun(short on) {
  //Sun
  if (on) {
    matrix.drawFastVLine(24, 4, 9, matrix.Color333(2, 1, 0));
    matrix.drawFastHLine(20, 8, 9, matrix.Color333(2, 1, 0));
    matrix.drawLine(21, 5, 27, 11, matrix.Color333(2, 1, 0));
    matrix.drawLine(21, 11, 27, 5, matrix.Color333(2, 1, 0));
    matrix.fillCircle (24, 8 , 2, matrix.Color333(6, 6, 0));
  } else {
    matrix.fillRect(20, 4, 9, 9, matrix.Color333(0, 0, 0));
  }
}

//----------------------------------
// LEFT ARROW - BLINKER
//----------------------------------

void Arduibag::displayFlecheG() {
  
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  Serial.write('0'); //LED OFF
  delay(300);
  matrix.fillTriangle(5, 8, 10, 3, 10, 13, matrix.Color333(6, 1, 0));
  matrix.fillRect(10, 6, 17, 6, matrix.Color333(6, 1, 0));
  Serial.write('L'); //LED 1 ON YELLOW
  delay(300);
 
}

//----------------------------------
// RIGHT ARROW - BLINKER
//----------------------------------

void Arduibag::displayFlecheD() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  Serial.write('0'); //LED OFF
  delay(300);
  matrix.fillTriangle(26, 8, 21, 3, 21, 13, matrix.Color333(6, 1, 0));
  matrix.fillRect(5, 6, 17, 6, matrix.Color333(6, 1, 0));
  Serial.write('R'); //LED 2 ON YELLOW
  delay(300);
}

//----------------------------------
// STOP MESSAGE - STOP
//----------------------------------

void Arduibag::displaySTOP() {
  matrix.fillScreen(matrix.Color333(1, 0, 0));
  matrix.setCursor(5, 5);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(6,6,6));
  matrix.print("STOP");
  Serial.write('S'); //LED 1&2 ON RED
  while(!Serial.available());
  //_action = 10;
}

//----------------------------------
// BLUE TRIANGLE - WALK ?
//----------------------------------

void Arduibag::displayWalk() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  delay(300);
  matrix.drawTriangle(16, 3, 7, 12, 25, 12, matrix.Color333(0, 0, 7));
  matrix.drawTriangle(16, 4, 9, 11, 23, 11, matrix.Color333(0, 0, 7));
  delay(300);
}

//----------------------------------
// BLUE ANIMATED TRIANGLE - RUN ?
//----------------------------------

void Arduibag::displayRun() {
  
  if ((_upB<20) or (_upB>30 and _upB<40) or (_upB>50 and _upB<60) or (_upB>70 and _upB<80) or (_upB>90 and _upB<=100)) {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.fillTriangle(16, _upA, 7, (_upA+9), 25, (_upA+9), matrix.Color333(0, 0, 7));
    matrix.fillTriangle(16, _upA+13, 7, (_upA+22), 25, (_upA+22), matrix.Color333(0, 0, 7));
    matrix.fillTriangle(16, _upA+26, 7, (_upA+35), 25, (_upA+35), matrix.Color333(0, 0, 7));
    delay(10);
  } else {
    delay(20);
  }
  
  
  if (_upA == -12) {
    _upA=0;
    _upB++;
   } else {
     _upA--;
   }
           
   if (_upB==20 and _upA==0) displayTime();
   if (_upB==40 and _upA==0) displayTravelTime();
   if (_upB==60 and _upA==0) displayTemperature(1);
   if (_upB==80 and _upA==0) displayHumidity();

   if (_upB > 100) {
     _upB = 0;
   }
}

//----------------------------------
// SPRITE - Display SPRITE
//----------------------------------

void Arduibag::displayMario(short s) {
   short x, y, c=0;
   int  color;
   if (_upB<-54)_upB=32;
   
   matrix.setTextWrap(false);
   matrix.fillScreen(matrix.Color333(0, 0, 0));
   //Serial.write('0'); //LED OFF
    
    for (y=0;y<16;y++) {
      for (x=0;x<16;x++) {
        if (s==1) color = pgm_read_word_near(mario1 + c);
        if (s==2) color = pgm_read_word_near(mario2 + c);
        matrix.drawPixel(x+_upB , y+1, color);
      c++;
      }
    }
    
    _upB -= 2;
    
    matrix.setCursor(x+4+_upB, 8);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(6,1,0));
    if (_LANG)matrix.print("MERCI!"); else matrix.print("THANKS!");
    
    delay(100);
    Serial.write('M'); //LED 1&2 MAGENTA
    //_action = 10;
}

//----------------------------------
// SPRITE ANIMATION - ???
//----------------------------------

void Arduibag::displayAnimMario() {
  if (_upA>2)_upA=0;
  if (_upA==0) displayMario(1);
  if (_upA==1) displayMario(2);
  _upA++;
}


//----------------------------------
// ACTION LAUNCHER
//----------------------------------

void Arduibag::lanceFunction (size_t texte) {

  char caract = char(texte);

  switch (caract) {
    
    case 'a':
      _action = 0;
    break; 
    
    case 'A':
      _action = 1;
    break;

    case 'B':
      _action = 2;
    break;
  
    case 'C':
      _action = 3;
    break;
  
    case 'D':
      _upB=32;
      _action = 4;
    break;
    
    case 'E':
      _action = 5;
    break;

    case 'F':
      _action = 6;
    break;
    
    //Mobile application only
    case 'M':
      _action = 7;
    break;
    
    default:
      _action = 0;
   }
}

//----------------------------------
// DISPLAY TEXT
//----------------------------------

void Arduibag::displayTexte(size_t texte) {
  
   // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));
    
  // draw some text!
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  
  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print(char(texte));
}


