#include"microcontroller.h"
//Constructor
microcontroller::microcontroller()
{
  warmUp_begin=millis();
  warmUp_end=millis();
  preAlarm_begin=millis();
  preAlarm_end=millis();
  alarm_begin=millis();
  alarm_end=millis();
  alarmState=ALARM_OFF;
  sirenState=SIREN_OFF;
  warmUpFlag=false;
  preAlarmSiren=new siren(1,4);    //Power is on D2 aka GPIO 4
  alarmSiren=new siren(0,5); //Power is on D1 aka GPIO 5
  pad=new keypad();
  sensorModes=new int[numOfDevices];
    for(int i=0;i<numOfDevices;i++)
      sensorModes[i]=ARMED;   //Default armed state is armed for all sensors but can change in case of outside unreliable sensor or damaged sensor so no false alarms are triggered
    sensors=new sensor[numOfDevices];
    sensors[0].setAll(0,NEGATIVETRIGGER,14,16,SENSOR_OFF); //Common sensor power is D0 aka GPIO 16, trigger is D5 aka GPIO 14
    sensors[1].setAll(1,NEGATIVETRIGGER,12,16,SENSOR_OFF); //Common sensor power is D0 aka GPIO 16, trigger is D6 aka GPIO 12
    sensors[2].setAll(2,NEGATIVETRIGGER,13,16,SENSOR_OFF); //Common sensor power is D0 aka GPIO 16, trigger is D7 aka GPIO 13
}

microcontroller::~microcontroller() 
{
  delete[] sensors;
  delete[] sensorModes;
  delete preAlarmSiren;
  delete alarmSiren;
  delete pad;
}
//Getter
bool microcontroller::getAlarmState(){return alarmState;}
int microcontroller::getSirenState(){return sirenState;}
int* microcontroller::getSensorModes(){return sensorModes;}
sensor* microcontroller::getSensors(){return sensors;}
siren* microcontroller::getAlarmSiren(){return alarmSiren;}
siren* microcontroller::getPreAlarmSiren(){return preAlarmSiren;}
//Setter
void microcontroller::setAlarmState(bool al){if(al==alarmState) return; alarmState=al;}
void microcontroller::setSirenState(int sir){sirenState=sir;}
void microcontroller::setSerial(HardwareSerial* srl)
{
  controllerSerial=srl;
  controllerSerial->println("Microcontroller connected to serial");
}
void microcontroller::setWarmUpFlag(bool warm){if(warm!=warmUpFlag) warmUpFlag=warm;}

//Functions
void microcontroller::powerOn()
{
  for(int i=0;i<numOfDevices;i++)
    getSensors()[i].powerOn();
}
void microcontroller::powerOff()
{
  for(int i=0;i<numOfDevices;i++)
    getSensors()[i].powerOff();
}
void microcontroller::alarmOff()
{
  alarmSirenOff();
  preAlarmSirenOff();
  setAlarmState(false);
  disarmAll();
  warmUpOver();
  powerOff();
  synchronizeKeypad();
}
void microcontroller::alarmOn()
{
  setAlarmState(true);
  armAll();
  warmUp();
  powerOn();
  synchronizeKeypad();
}
void microcontroller::synchronizeKeypad()
{
  if (pad->getIsArmed() != alarmState)
    pad->setIsArmed(alarmState);
}
void microcontroller::control()
{
  pad->checkInput();
  Serial.print("Keypad Input: ");
  Serial.println(pad->getCurrentInput());
  if(alarmState==false && pad->getIsArmed()==true)
    alarmOn();
  else if(alarmState==true && pad->getIsArmed()==false)
    alarmOff();
  
  warmUpTimeOut();  //Check if warm up has finished (if alarm is warming up)
  if(warmUpFlag)    //If alarm is warming up, stop here, no detections, no sirens
    return;

  
  if(getAlarmState()) //If alarm is on
  {
    int signal = detect();  //Normally detect is only needed if alarm is not sounding and no detect has occured but it is used in every loop to update the notification states of all sensors
    if(!preAlarmSiren->isSounding() && !alarmSiren->isSounding())  //If no alarm is sounding, no detect has occured
    {
      if(signal==ALARM) //If sensors detect ALARM
      {
        preAlarmSirenOn();
      }
      else if(signal==DETECT) //If sensors detect DETECT
      {

      }
      //If NOALARM
    }
    else if(preAlarmSiren->isSounding() && !alarmSiren->isSounding()) //If only pre-alarm is sounding, detect has occured
    {
      if(preAlarmDuration()>=PREALARMDURATION)
      {
        alarmSirenOn();
      }
    }
    else if(alarmSiren->isSounding()) //If alarm is sounding
    {
      if(signal==ALARM)       //If alarm keeps being detected, refresh stop contdown (don't stop alarm)
        alarmSirenOn();
      if(alarmDuration()>=ALARMDURATION)
      {
        alarmSirenOff();
        preAlarmSirenOff();
      }
    }

  }
  else                //if alarm is off
 {
  //unnecessary
  //alarmSirenOff();
  //preAlarmSirenOff();
  //disarmAll();
 }
}

void microcontroller::armSensor(int sensorNum)
{
  getSensors()[sensorNum].setMode(sensorModes[sensorNum]);
}
void microcontroller::disarmSensor(int sensorNum)
{
  getSensors()[sensorNum].turnOff();
}
void microcontroller::armAll()
{
  int i;
  for(i=0;i<numOfDevices;i++)
    armSensor(i);
}
void microcontroller::disarmAll()
{
  int i;
  for(i=0;i<numOfDevices;i++)
    disarmSensor(i);
}
void microcontroller::detectSensor(int sensorNum)
{
  getSensorModes()[sensorNum]=DETECT;
  if(alarmState==true)
    getSensors()[sensorNum].setMode(DETECT);
}
void microcontroller::activateSensor(int sensorNum)
{
  getSensorModes()[sensorNum]=ARMED;
  if(alarmState==true)
    getSensors()[sensorNum].turnOn();
}
void microcontroller::deactivateSensor(int sensorNum)
{
  getSensorModes()[sensorNum]=SENSOR_OFF;
  if(alarmState==true)
    getSensors()[sensorNum].turnOff();
}

int microcontroller::detect()
  {
    int alarmFlag=NOALARM;
    int i;
    for(i=0;i<numOfDevices;i++)
    {
      if((getSensors()[i].getMode()==ARMED || getSensors()[i].getMode()==DETECTANDSMS) && getSensors()[i].getNotificationState()==NOTIFY_SENT && alarmSiren->isSounding())   //If armed sensor (or detect&sms sensor) has already detected alarm and the siren is sounding, advance to alarm triggered state
          getSensors()[i].setNotificationState(ALARM_TRIGGERED);
      
      if(getSensors()[i].detect()==ALARM)                                       //If sensor detects alarm advance state, prepare to return detect, low priority
      {
        if(getSensors()[i].getNotificationState()==IDLE)
        getSensors()[i].setNotificationState(SENSOR_TRIGGERED);
        if(getSensors()[i].getMode()==ARMED)                                    //If armed sensor detects alarm, prepare to return alarm, high priority, no returning detect after alarm has occured
          alarmFlag=ALARM;
        else if(alarmFlag==NOALARM)
        alarmFlag=DETECT;
      }
    }
  return alarmFlag;    
  }

void microcontroller::alarmSirenOn()
{
  alarmSiren->soundAlarm();
  alarm_begin=millis();
}
void microcontroller::alarmSirenOff()
{
  alarmSiren->stopAlarm();
}
void microcontroller::preAlarmSirenOn()
{
  preAlarmSiren->soundAlarm();
  preAlarm_begin=millis();
}
void microcontroller::preAlarmSirenOff()
{
  preAlarmSiren->stopAlarm();
}
unsigned long microcontroller::warmUpDuration()
{
    warmUp_end=millis();
    return warmUp_end-warmUp_begin;
}
void microcontroller::warmUp()
  {
    warmUp_begin=millis();
    warmUpFlag=true;
  }
void microcontroller::warmUpTimeOut()
{
  if(warmUpFlag && warmUpDuration()>=ALARMWARMUPDURATION)
    warmUpOver();
}
void microcontroller::warmUpOver()
{
  setWarmUpFlag(false);
}
unsigned long microcontroller::preAlarmDuration()
{
    preAlarm_end=millis();
    return preAlarm_end-preAlarm_begin;
}
unsigned long microcontroller::alarmDuration()
{
    alarm_end=millis();
    return alarm_end-alarm_begin;
}