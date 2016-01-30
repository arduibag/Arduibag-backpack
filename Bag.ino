//-----------------------------------------------------------------
//
// ArduiBAG - Michaël d'AURIA
// 30/01/2016
//
// Bag part
//
//-----------------------------------------------------------------

#include <Arduibag.h>

//Langage configuration
// 0: US
// 1: FR
#define LANG 0

//Arduibag configuration
Arduibag aBag(LANG);

// Global variable for actions
bool day = 0; //night or day
char action;

//----------------------------------
// SETUP
//----------------------------------

void setup() {
  
  //-------------------------------
  // Hardware setup
  // ------------------------------
  
  // Serial communication setup
  Serial.begin(9600);
  
  //Arduibag setup
  aBag.begin(); 

  //-------------------------------
  // Software initialization
  //-------------------------------
    
  //Display Logo
  aBag.displayLogo();
  
  //Bluetooth synchronization
  //Wainting for the remote
  aBag.waitRemote();
  
  //Night & Day chose
  day = aBag.chooseNightDay();
  
  // display Ready message
  aBag.displayReady();  
  
  
}


//----------------------------------
// MAIN LOOP
//----------------------------------

void loop() {

  if (Serial.available())action = Serial.read();
 
  switch (action) {
    
    case 'a':
      aBag.clearScreen();
      Serial.write('0');
      //upA=0;
      //upB=0;
      action = -1;
    break;
    
    // LEFT
    case 'A':
      aBag.displayFlecheG();
    break;

    //RIGHT
    case 'B':
      aBag.displayFlecheD();
    break;
    
    //DOWN
    case 'C':
      aBag.displaySTOP();
    break;
    
    //UP
    case 'D':
      aBag.displayAnimMario();
    break;

    case 'E':
      aBag.displayRun();
    break;
      
    default:
      if (day) aBag.clearScreen(); else aBag.displayRun();
      
  }

}
