#include <stdlib.h>
#include "microcontroller.h"
#include "thingProperties.h"
////////////////////////////////////////////////////////
microcontroller A;
void setup() {
  Serial.begin(9600);  // opens serial port, sets data rate to 9600 bps
  delay(2000);
  Serial.println(" Starting program");
  A.setSerial(&Serial);
  // Defined in thingProperties.h
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  delay(3000);
  ArduinoCloud.update();
  onAlarmChange();
  onSensor1Change();
  onSensor2Change();
  onSensor3Change();
  synchronizeStates();
}

////////////////////////////////////////////////////////
void loop() {

  ArduinoCloud.update();  //Read and write on server variables
  synchronizeAlarm();     //Write on server alarm variable if alarm state changed locally
  A.control();            //The function to control the behavior of the alarm locally
  checkStates();
}
////////////////////////////////////////////////////////
//Local alarm variable change
void synchronizeAlarm() {
  if (alarm != A.getAlarmState())
    alarm = A.getAlarmState();  //If alarm changes locally, the cloud alarm variable needs to change too
}
//Local sensor states change
void synchronizeStates() {
  state1Detect = A.getSensors()[0].getNotificationState() == NOTIFY_SENT;
  state1Alarm = A.getSensors()[0].getNotificationState() == ALARM_NOTIFY_SENT;
  state2Detect = A.getSensors()[1].getNotificationState() == NOTIFY_SENT;
  state2Alarm = A.getSensors()[1].getNotificationState() == ALARM_NOTIFY_SENT;
  state3Detect = A.getSensors()[2].getNotificationState() == NOTIFY_SENT;
  state3Alarm = A.getSensors()[2].getNotificationState() == ALARM_NOTIFY_SENT;
}
////////////////////////////////////////////////////////
//IoT variable changes
void onAlarmChange() {
  if (alarm == A.getAlarmState())
    return;
  if (alarm == true)
    A.alarmOn();
  if (alarm == false)
    A.alarmOff();
}

//Since Sensor1 is READ_WRITE variable, onSensor1Change() is executed every time a new value is received from IoT Cloud.
void onSensor1Change() {
  if (numOfDevices >= 1) {
    if (sensor_1 == SENSOR_OFF)
      A.deactivateSensor(0);
    if (sensor_1 == ARMED)
      A.activateSensor(0);
    if (sensor_1 == DETECT)
      A.detectSensor(0);
  }
}

//Since Sensor2 is READ_WRITE variable, onSensor2Change() is executed every time a new value is received from IoT Cloud.
void onSensor2Change() {
  if (numOfDevices >= 2) {
    if (sensor_2 == SENSOR_OFF)
      A.deactivateSensor(1);
    if (sensor_2 == ARMED)
      A.activateSensor(1);
    if (sensor_2 == DETECT)
      A.detectSensor(1);
  }
}

//Since Sensor3 is READ_WRITE variable, onSensor3Change() is executed every time a new value is received from IoT Cloud.
void onSensor3Change() {
  if (numOfDevices >= 3) {
    if (sensor_3 == SENSOR_OFF)
      A.deactivateSensor(2);
    if (sensor_3 == ARMED)
      A.activateSensor(2);
    if (sensor_3 == DETECT)
      A.detectSensor(2);
  }
}
void printnotificationdebug(int sensorNum, int notificationState, int mode) {
  Serial.print("Sensor: ");
  Serial.print(sensorNum);
  Serial.print("\tMode: ");
  Serial.print(mode);
  Serial.print("\tNotification State: ");
  Serial.println(notificationState);
}

void checkStates() {
  for (int i = 0; i < numOfDevices; i++)
    checkState(i);
  synchronizeStates();
}
void checkState(int sensorNum) {
  //printnotificationdebug(sensorNum, A.getSensors()[sensorNum].getNotificationState(), A.getSensors()[sensorNum].getMode());
  if (A.getSensors()[sensorNum].getNotificationState() == SENSOR_TRIGGERED)  //If sensor has triggered send email and advance notification state
  {
    if (A.getAlarmSiren()->isSounding() && (A.getSensors()[sensorNum].getMode() == ARMED || A.getSensors()[sensorNum].getMode() == DETECTANDSMS))  //If sensor mode is armed or d&s AND siren is sounding, then skip to alarm triggered state
      A.getSensors()[sensorNum].setNotificationState(ALARM_TRIGGERED);
    else {
      //send email
      A.getSensors()[sensorNum].setNotificationState(NOTIFY_SENT);
      A.getSensors()[sensorNum].notificationTimestamp();
    }
  }
  if (A.getSensors()[sensorNum].getNotificationState() == NOTIFY_SENT)  //If email has been sent, when notification duration passes return to idle state
  {
    if (A.getSensors()[sensorNum].notificationDuration() > NOTIFICATIONDURATION)
      A.getSensors()[sensorNum].setNotificationState(IDLE);
  }
  if (A.getSensors()[sensorNum].getNotificationState() == ALARM_TRIGGERED)  //If alarm has triggered (only armed can trigger but detect&sms also reach this state without triggering), send alarm notification email and sms and advance state
  {
    //send email
    A.getSensors()[sensorNum].setNotificationState(ALARM_NOTIFY_SENT);
    A.getSensors()[sensorNum].alarmNotificationTimestamp();
  }
  if (A.getSensors()[sensorNum].getNotificationState() == ALARM_NOTIFY_SENT)  //If alarm notification has been sent, if alarm notification duration has passed return to alarm triggered state if alarm is sounding or to idle if not
  {
    if (A.getSensors()[sensorNum].alarmNotificationDuration() > ALARMNOTIFICATIONDURATION) {
      if (A.getAlarmSiren()->isSounding())
        A.getSensors()[sensorNum].setNotificationState(ALARM_TRIGGERED);
      else
        A.getSensors()[sensorNum].setNotificationState(IDLE);
    }
  }
  printnotificationdebug(sensorNum, A.getSensors()[sensorNum].getNotificationState(), A.getSensors()[sensorNum].getMode());
}



