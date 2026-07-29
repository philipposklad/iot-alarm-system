#include"sensor.h"

//Constructors
  sensor::sensor()
  {
    //Does nothing, used with new[], needs to set parameters, impossible with new[]
  }
  sensor::sensor(int d, int typ, int trig, int vcc, int mod)
  {
    setAll(d, typ, trig, vcc, mod);
  }

//Getters
  int sensor::getId(){return id;}
  int sensor::getType(){return type;}
  int sensor::getTriggerPin(){return triggerPin;}
  int sensor::getVccPin(){return vccPin;}
  int sensor::getMode(){return mode;}
  int sensor::getTriggerFlag(){return triggerFlag;}
  int sensor::getNotificationState(){return notificationState;}
//Setters
  void sensor::setAll(int d, int typ, int trig, int vcc, int mod)
  {
    id=d;
    type=typ;
    triggerPin=trig;
    vccPin=vcc;
    mode=mod;
    triggerFlag=false;
    pinMode(triggerPin,INPUT);
    digitalWrite(vccPin,LOW); //digitalWrite before pinMode to start with LOW value when pinMode is called
    pinMode(vccPin,OUTPUT);
    warmUp_begin=0;
    warmUp_end=0;
    detect_begin=0;
    detect_end=0;
    notification_begin=0;
    notification_end=0;
    alarm_notification_begin=0;
    alarm_notification_end=0;
    setNotificationState(IDLE);
    if(mode!=SENSOR_OFF)//If sensor starts on warm up is needed
      warmUp();
    else
      warmUpFlag=0;
  }
  void sensor::setId(int d){id=d;}
  void sensor::setType(int typ){type=typ;}
  void sensor::setTriggerPin(int trig){triggerPin=trig;}
  void sensor::setVccPin(int vcc){vccPin=vcc;}
  void sensor::setMode(int mod)
  {
    if(mode==mod) //If mode doesn't change return
      return; 
    setNotificationState(IDLE); //Everytime mode changes, notification state returns to idle
    if(mode==SENSOR_OFF)//If old mode is off, needs warm up
    {
      powerOn();
      warmUp();
    }
    mode=mod;
    if(mode==SENSOR_OFF)//If new mode is off
    {
      if(warmUpFlag)//If warm up is in progress, cancel warm up and turn off
        warmUpFlag=0;
    }
  }
  void sensor::setNotificationState(int newState){notificationState=newState;}
//Functions
  void sensor::powerOn()
  {
    if(digitalRead(vccPin)==HIGH)
      return;
    digitalWrite(vccPin,HIGH);
  }
  void sensor::powerOff()
  {
    if(digitalRead(vccPin)==LOW)
      return;
    digitalWrite(vccPin,LOW);
  }
  void sensor::turnOn()
  {
    setMode(ARMED);
  }

  void sensor::turnOff()
  {
    setMode(SENSOR_OFF);
  }

  int sensor::detect()
  {
    if(mode==SENSOR_OFF)//Sensor is off
      return NOALARM;
    if(warmUpFlag)
    {
      if(warmUpDuration()>WARMUPDURATION)//Triggers during warm up should not count as alarm
        warmUpFlag=0;
      return NOALARM;
    }

    if(triggerFlag)//If sensor has recently triggered check if pulse has ended
    {
      if(detectDuration()>PULSEDURATION)
        triggerFlag=false;
      return NOALARM;
    }
    else//If sensor has not triggered recenly
      if((type==NEGATIVETRIGGER && !digitalRead(triggerPin)) || (type==POSITIVETRIGGER && digitalRead(triggerPin)))//If sensor triggers
      {
        detect_begin=millis();
       triggerFlag=true;
       return ALARM;
      }
    return NOALARM;
  }

  unsigned long sensor::detectDuration()
  {
    detect_end=millis();
    return detect_end-detect_begin;
  }
  unsigned long sensor::warmUpDuration()
  {
    warmUp_end=millis();
    return warmUp_end-warmUp_begin;
  }
  void sensor::warmUp()
  {
    warmUp_begin=millis();
    warmUpFlag=true;
  }
  void sensor::notificationTimestamp()
  {
    notification_begin=millis();
  }
  unsigned long sensor::notificationDuration()
  {
    notification_end=millis();
    return notification_end-notification_begin;
  }
  void sensor::alarmNotificationTimestamp()
  {
    alarm_notification_begin=millis();
  }
  unsigned long sensor::alarmNotificationDuration()
  {
    alarm_notification_end=millis();
    return alarm_notification_end-alarm_notification_begin;
  }
