#ifndef MICROCONTROLLER_H
#define MICROCONTROLLER_H
#include"sensor.h"
#include"siren.h"
#include"keypad.h"

const int numOfDevices = 3;

//Constants
const int ALL_OFF = 0;
const int ALL_ARMED = 1;
const int ARMED_STATE = 2;
const int CURRENT_STATE = 3;
const bool ALARM_OFF = 0;
const bool ALARM_ON = 1;
const int SIREN_OFF = 0;
const int PRE_SIREN_ON = 1;
const int SIREN_ON = 2;
const int SIREN_EMAIL =3;
const unsigned long ALARMWARMUPDURATION = 30000;
const unsigned long PREALARMDURATION = 15000;
const unsigned long ALARMDURATION = 300000;

class microcontroller 
{
    unsigned long warmUp_begin;
    unsigned long warmUp_end;
    unsigned long preAlarm_begin;
    unsigned long preAlarm_end;
    unsigned long alarm_begin;
    unsigned long alarm_end;
    HardwareSerial* controllerSerial;
    sensor* sensors;
    int* sensorModes;
    siren* preAlarmSiren;
    siren* alarmSiren;
    keypad* pad;
    bool alarmState;
    bool warmUpFlag;
    int sirenState;
  public:
    microcontroller();
    ~microcontroller();
//Getters
    bool getAlarmState();
    int getSirenState();
    int* getSensorModes();
    sensor* getSensors();
    siren* getAlarmSiren();
    siren* getPreAlarmSiren();
//Setters
    void setAlarmState(bool al);
    void setSirenState(int sir);
    void setSerial(HardwareSerial* srl);
    void setWarmUpFlag(bool warm);
//Functions
    void alarmOn();
    void alarmOff();
    int detect();
    void control();
    void armSensor(int sensorNum);
    void disarmSensor(int sensorNum);
    void armAll();
    void disarmAll();
    void detectSensor(int sensorNum);
    void activateSensor(int sensorNum);
    void deactivateSensor(int sensorNum);
    void alarmSirenOn();
    void alarmSirenOff();
    void preAlarmSirenOn();
    void preAlarmSirenOff();
    unsigned long warmUpDuration();
    unsigned long preAlarmDuration();
    unsigned long alarmDuration();
    void warmUp();
    void warmUpTimeOut();
    void warmUpOver();
    void powerOn();
    void powerOff();
    void synchronizeKeypad();
};




#endif