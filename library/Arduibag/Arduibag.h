/*
  Arduibag.h - Library for Arduibag connected backpack for biker.
  Created by Michaël d'AURIA - 2016-30-01
  Released into the public domain.
*/
#ifndef Arduibag_h
#define Arduibag_h

#include "Arduino.h"

#include <Adafruit_GFX.h>   // Core graphics library for LED Matrix
#include <RGBmatrixPanel.h> // Hardware-specific library LED led Matrix
#include <SoftwareSerial.h> // Serial library
#include <avr/pgmspace.h> // Memory library
#include <Sprites.h> //Sprite inclusion
#include <DHT.h> //Temperature sensor DHT11
#include <DS3231.h> // Real Time clock DS3231

class Arduibag
{
  public:
    Arduibag(bool LANG);
    void begin();
    void clearScreen();
    void displayLogo();
    void waitRemote();
    bool chooseNightDay();
    void displayReady();
    void displayInfos();
    void displayTravelTime();
    void displayTime();
    void displayTemperature(bool captor);
    void displayHumidity();
    void displayDanger();
    void displayMoon(short on);
    void displaySun(short on);
    void displayFlecheG();
    void displayFlecheD();
    void displaySTOP();
    void displayWalk();
    void displayRun();
    void displayMario(short s);
    void displayAnimMario();
    void lanceFunction (size_t texte);
    void displayTexte(size_t texte);

  private:
	bool _LANG;
    unsigned char _action;
    char _upA;
    char _upB;
};

#endif