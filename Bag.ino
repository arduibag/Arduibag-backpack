//-----------------------------------------------------------------
//
// ArduiBAG - Michaël d'AURIA
// 20/05/2014
//
// Bag part
//
//-----------------------------------------------------------------

#include <Adafruit_GFX.h>   // Core graphics library for LED Matrix
#include <RGBmatrixPanel.h> // Hardware-specific library LED led Matrix
#include <SoftwareSerial.h> // Serial library
#include <avr/pgmspace.h> // Memory library
#include <Sprites.h> //Sprite inclusion
#include <DHT.h> //Temperature sensor DHT11
#include <DS3231.h> // Real Time clock DS3231

//Realtime clock configuration
DS3231  rtc(SDA, SCL);
Time ti;
Time ti_init;

//Temperature sensor configuration
#define DHTPIN 12
#define DHTTYPE DHT11   // DHT 11 sensor type
DHT dht(DHTPIN, DHTTYPE);

//Screen configuration
#define CLK 8  
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// Global variable for actions
unsigned char action = 12;
char upA = 0;
char upB = 0;
bool day = 0; //night or day

//----------------------------------
// SETUP
//----------------------------------

void setup() {
  
  //-------------------------------
  // Hardware initialization
  // ------------------------------
  
  // Serial communication for DEBUG
  Serial.begin(9600);
   
  // LED Matrix Setup  
  matrix.begin();
  
  // DHT temperature sensor Setup
  dht.begin();
  
  //Realtime clock Setup
  rtc.begin();

  //keep current time for travel time
  ti_init = rtc.getTime();

  //-------------------------------
  // Software initialization
  //-------------------------------
    
  //Display Logo
  afficheLogo();
  
  //Bluetooth synchronization
  //Wainting for the remote
  waitRemote();
  
  //Night & Day chose
  day = choseNightDay();
  
  // display Ready message
  afficheReady();  
  
  
}


//----------------------------------
// MAIN LOOP
//----------------------------------

void loop() {
 
  switch (action) {
    
    case 0:
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      Serial.write('0');
      upA=0;
      upB=0;
      action = -1;
    break;
    
    // LEFT
    case 1:
      afficheFlecheG();
    break;

    //RIGHT
    case 2:
      afficheFlecheD();
    break;
    
    //DOWN
    case 3:
      afficheSTOP();
    break;
    
    //UP
    case 4:
      afficheAnimMario();
    break;

    case 5:
      afficheCourse();
    break;
    
    case 6:
      //afficheAnimation();
    break;
    
    case 7:
      //nothing
    break;
    
    case 10:
      //do Nothing ! :)
    break;
      
    default:
      if (day) matrix.fillScreen(matrix.Color333(0, 0, 0)); else afficheCourse();
      
  }

  if (Serial.available())lanceFonction(Serial.read());

}

//----------------------------------
// FUNCTIONS (for a future library !)
//----------------------------------



//----------------------------------
// DISPLAY LOGO
//----------------------------------
void afficheLogo() {
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
void waitRemote() {
  
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
// NIGHT or DAY CHOSE
//----------------------------------
bool choseNightDay() {

  short c=1, a, x;
 
 //Display Night & Day chose
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
        afficheDanger();
      }
      if (a=='D') {
        Serial.write('0');
        afficheInfos();
      }
    }
    
  }
}

//----------------------------------
// DISPLAY READY MESSAGE
//----------------------------------
void afficheReady() {

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
void afficheInfos() {
    
    while (Serial.read()!='a') {  
        afficheTime();
        delay(2000);
        afficheTravelTime();
        delay(2000);
        afficheTemperature(1);
        delay(2000);
        afficheHumidity();
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
void afficheTravelTime() {
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
void afficheTime() {
      
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
void afficheTemperature(bool captor) {      
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
void afficheHumidity() {
     
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
void afficheDanger() {
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
      matrix.print("RALENTIR!");
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

void displayMoon(short on) {
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

void displaySun(short on) {
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

void afficheFlecheG() {
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

void afficheFlecheD() {
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

void afficheSTOP() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  //mySerial.write('T'); //LED OFF
  delay(300);
  matrix.fillScreen(matrix.Color333(1, 0, 0));
  matrix.setCursor(5, 5);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(6,6,6));
  matrix.print("STOP");
  Serial.write('S'); //LED 1&2 ON RED
  delay(300);
  action = 10;
}

//----------------------------------
// BLUE TRIANGLE - WALK ?
//----------------------------------

void afficheMarche() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  delay(300);
  matrix.drawTriangle(16, 3, 7, 12, 25, 12, matrix.Color333(0, 0, 7));
  matrix.drawTriangle(16, 4, 9, 11, 23, 11, matrix.Color333(0, 0, 7));
  delay(300);
}

//----------------------------------
// GREEN ANIMATED TRIANGLE - RUN ?
//----------------------------------

void afficheCourse() {
  
  if ((upB<20) or (upB>30 and upB<40) or (upB>50 and upB<60) or (upB>70 and upB<80) or (upB>90 and upB<=100)) {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.fillTriangle(16, upA, 7, (upA+9), 25, (upA+9), matrix.Color333(0, 0, 7));
    matrix.fillTriangle(16, upA+13, 7, (upA+22), 25, (upA+22), matrix.Color333(0, 0, 7));
    matrix.fillTriangle(16, upA+26, 7, (upA+35), 25, (upA+35), matrix.Color333(0, 0, 7));
    delay(10);
  } else {
    delay(20);
  }
  
  
  if (upA == -12) {
    upA=0;
    upB++;
   } else {
     upA--;
   }
           
   if (upB==20 and upA==0) afficheTime();
   if (upB==40 and upA==0) afficheTravelTime();
   if (upB==60 and upA==0) afficheTemperature(1);
   if (upB==80 and upA==0) afficheHumidity();

   if (upB > 100) {
     upB = 0;
   }
}

//----------------------------------
// SPRITE - Display SPRITE
//----------------------------------
/*
void afficheSprite(short s) {
  short x, y, c=0;
  int  color;
  if (upB>32)upB=-16;

  matrix.fillRect(upB-2, 0, 2, 16, matrix.Color333(0, 0, 0));
   
  for (y=0;y<16;y++) {
    for (x=0;x<16;x++) {
      /if (s==1) color = pgm_read_word_near(sprite_1 + c);
      if (s==2) color = pgm_read_word_near(sprite_2 + c);
      if (s==3) color = pgm_read_word_near(sprite_3 + c);
      matrix.drawPixel(x + upB, y, color);
      c++;
    }
  }
  
  upB += 2;
  delay(100);
}*/

void afficheMario(short s) {
   short x, y, c=0;
   int  color;
   if (upB<-54)upB=32;
   
   matrix.setTextWrap(false);
   matrix.fillScreen(matrix.Color333(0, 0, 0));
   //Serial.write('0'); //LED OFF
    
    for (y=0;y<16;y++) {
      for (x=0;x<16;x++) {
        if (s==1) color = pgm_read_word_near(mario1 + c);
        if (s==2) color = pgm_read_word_near(mario2 + c);
        matrix.drawPixel(x+upB , y+1, color);
      c++;
      }
    }
    
    upB -= 2;
    
    matrix.setCursor(x+4+upB, 8);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(6,1,0));
    matrix.print("MERCI!");
    
    delay(100);
    Serial.write('M'); //LED 1&2 MAGENTA
    //action = 10;
}

//----------------------------------
// SPRITE ANIMATION - ???
//----------------------------------
/*
void afficheAnimation() {
  if (upA>4)upA=0;
  if (upA==0) afficheSprite(1);
  if (upA==1) afficheSprite(2);
  if (upA==2) afficheSprite(3);
  if (upA==3) afficheSprite(2);
  upA++;
}*/

void afficheAnimMario() {
  if (upA>2)upA=0;
  if (upA==0) afficheMario(1);
  if (upA==1) afficheMario(2);
  upA++;
}


//----------------------------------
// ACTION LAUNCHER
//----------------------------------

void lanceFonction (size_t texte) {

  char caract = char(texte);

  switch (caract) {
    
    case 'a':
      action = 0;
    break; 
    
    case 'A':
      action = 1;
    break;

    case 'B':
      action = 2;
    break;
  
    case 'C':
      action = 3;
    break;
  
    case 'D':
      upB=32;
      action = 4;
    break;
    
    case 'E':
      action = 5;
    break;

    case 'F':
      action = 6;
    break;
    
    //Mobile application only
    case 'M':
      action = 7;
    break;
    
    default:
      action = 0;
   }
}

//----------------------------------
// DISPLAY TEXT
//----------------------------------

void afficheTexte(size_t texte) {
  
   // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));
    
  // draw some text!
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  
  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print(char(texte));
}
