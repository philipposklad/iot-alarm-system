#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <WiFiClientSecure.h>

const char DEVICE_LOGIN_NAME[]  = "";


const char SSID[]               = "";// Network SSID (name)
const char PASS[]               = "";// Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = "";//SECRET_DEVICE_KEY;    // Secret device password

/////////////////////////////////////////////////////////////////////


void onSensor1Change();
void onSensor2Change();
void onSensor3Change();
void onAlarmChange();

int sensor_1;
int sensor_2;
int sensor_3;
bool alarm;
bool state1Alarm;
bool state1Detect;
bool state2Alarm;
bool state2Detect;
bool state3Alarm;
bool state3Detect;

void initProperties()
{

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(sensor_1, READWRITE, ON_CHANGE, onSensor1Change);
  ArduinoCloud.addProperty(sensor_2, READWRITE, ON_CHANGE, onSensor2Change);
  ArduinoCloud.addProperty(sensor_3, READWRITE, ON_CHANGE, onSensor3Change);
  ArduinoCloud.addProperty(alarm, READWRITE, ON_CHANGE, onAlarmChange);
  ArduinoCloud.addProperty(state1Alarm, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(state1Detect, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(state2Alarm, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(state2Detect, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(state3Alarm, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(state3Detect, READ, ON_CHANGE, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
