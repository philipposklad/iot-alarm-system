#ifndef SENSOR_H
#define SENSOR_H
#ifndef ARDUINO_H
#define ARDUINO_H
#include<Arduino.h>
#endif
const unsigned long PULSEDURATION = 10000;
const unsigned long WARMUPDURATION = 40000;
const unsigned long NOTIFICATIONDURATION = 120000;
const unsigned long ALARMNOTIFICATIONDURATION = 120000;
const int POSITIVETRIGGER = 0;
const int NEGATIVETRIGGER = 1;
const int NOALARM = 0;
const int ALARM = 1;
const int SENSOR_OFF = 0;
const int ARMED = 1;
const int DETECT = 2;
const int DETECTANDSMS = 3;
const int TEST = 4;
const int IDLE = 0;
const int SENSOR_TRIGGERED = 1;
const int NOTIFY_SENT = 2;
const int ALARM_TRIGGERED = 3;
const int ALARM_NOTIFY_SENT = 4;

class sensor
{
    //Variables
    int id;
    int type;
    int triggerPin;
    int vccPin;
    int mode;
    bool triggerFlag;    
    unsigned long detect_begin;
    unsigned long detect_end;
    bool warmUpFlag;
    unsigned long warmUp_begin;
    unsigned long warmUp_end;
    int notificationState;
    unsigned long notification_begin;
    unsigned long notification_end;
    unsigned long alarm_notification_begin;
    unsigned long alarm_notification_end;
  public:
    //Constructor
    sensor();
    sensor(int d, int typ, int trig, int vcc, int mod);
    //Functions
    void powerOn();
    void powerOff();
    void turnOn();
    void turnOff();
    int detect();
    unsigned long detectDuration();
    unsigned long warmUpDuration();
    unsigned long notificationDuration();
    unsigned long alarmNotificationDuration();
    void warmUp();
    void notificationTimestamp();
    void alarmNotificationTimestamp();
    //Getters
    int getId();
    int getType();
    int getTriggerPin();
    int getVccPin();
    int getMode();
    int getTriggerFlag();
    int getNotificationState();
    //Setters
    void setAll(int d, int typ, int trig, int vcc, int mod);//Used after new[] to set the parameters
    void setId(int d);
    void setType(int typ);
    void setTriggerPin(int trig);
    void setVccPin(int vcc);
    void setMode(int mod);
    void setNotificationState(int newState);
};
#endif