#ifndef KEYPAD_H
#define KEYPAD_H
#ifndef ARDUINO_H
#define ARDUINO_H
#include<Arduino.h>
#endif
const unsigned long INPUTDURATION = 5000;
const unsigned long NOKEYDURATION = 50;
const int CODESIZE = 4;
const int analogInPin = A0;
class keypad
{
    String currentInput;
    String armCode;
    String disarmCode;
    bool isArmed;
    bool keyPressed;
    unsigned long input_begin;
    unsigned long input_end;
    unsigned long no_key_begin;
    unsigned long no_key_end;
  public:
  //Constructor
    keypad();
    keypad(String arm, String disarm);
  //Getters
    String getCurrentInput();
    String getArmcode();
    String getDisarmCode();
    bool getIsArmed();
  //Setters
    void setCurrentInput(String str);
    void setArmCode(String str);
    void setDisarmCode(String str);
    void setIsArmed(bool alarm);
  //Functions
    unsigned long inputDuration();
    unsigned long noKeyDuration();
    void checkNoKeyTimeout();
    char mapAnalogToKey(int analogValue);
    void checkTimeout();
    void checkInput();
};
#endif