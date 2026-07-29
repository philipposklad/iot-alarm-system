#include"siren.h"
//Constructor
  siren::siren(){}
  siren::siren(int d, int vcc)
  {
    id=d;
    vccPin=vcc;
    digitalWrite(vccPin,LOW); //Writing first so siren starts on LOW
    pinMode(vccPin,OUTPUT);
  }

//Getters
  int siren::getId(){return id;}
  int siren::getVccPin(){return vccPin;}

//Setters
  void siren::setId(int d){id=d;}
  void siren::setVccPin(int vcc){vccPin=vcc;}

//Functions
  bool siren::isSounding()
  {
    if(digitalRead(vccPin)==HIGH)
      return true;
    else return false;
  }
  
  void siren::soundAlarm()
  {
    if(digitalRead(vccPin)==HIGH)
      return;
    digitalWrite(vccPin,HIGH);
  }

  void siren::stopAlarm()
  {
    if(digitalRead(vccPin)==LOW)
      return;
    digitalWrite(vccPin,LOW);
  }


