#ifndef SIREN_H
#define SIREN_H
#ifndef ARDUINO_H
#define ARDUINO_H
#include<Arduino.h>
#endif

class siren
{
    int id;
    int vccPin;
  public:
    siren();
    siren(int d, int vcc);
  //Getters
    int getId();
    int getVccPin();
  //Setters
    void setId(int d);
    void setVccPin(int vcc);
  //Functions
    bool isSounding();
    void soundAlarm();
    void stopAlarm();
};
#endif