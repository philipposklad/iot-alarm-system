#include"keypad.h"

//Constructor
  keypad::keypad()
  {
    input_begin=millis();
    no_key_begin=millis();
    isArmed=false;
    keyPressed=false;
    currentInput="";
    armCode="4321";
    disarmCode="1234";
  }
  keypad::keypad(String arm, String disarm)
  {
    input_begin=millis();
    no_key_begin=millis();
    isArmed=false;
    keyPressed=false;
    currentInput="";
    armCode=arm;
    disarmCode=disarm;
  }
//Getters
  String keypad::getCurrentInput(){return currentInput;}
  String keypad::getArmcode(){return armCode;}
  String keypad::getDisarmCode(){return disarmCode;}
  bool keypad::getIsArmed(){return isArmed;}
//Setters
  void keypad::setCurrentInput(String str){currentInput=str;}
  void keypad::setArmCode(String str){armCode=str;}
  void keypad::setDisarmCode(String str){disarmCode=str;}
  void keypad::setIsArmed(bool alarm){isArmed=alarm;}
//Functions
  unsigned long keypad::inputDuration()
  {
    input_end=millis();
    return input_end-input_begin;
  }
  char keypad::mapAnalogToKey(int analogValue)  //Translate integer values to characters
  {
        if(analogValue < 20) return '\0';
        if(analogValue < 72) return '1';
        if(analogValue < 115) return '2';
        if(analogValue < 168) return '3';
        if(analogValue < 260) return '4';
        if(analogValue < 373) return '5';
        if(analogValue < 480) return '6';
        if(analogValue < 630) return '7';
        if(analogValue < 750) return '8';
        if(analogValue < 830) return '9';
        if(analogValue < 890) return '*';
        if(analogValue < 950) return '0';
        if(analogValue < 1024) return '#';
        return '\0';
  }
  void keypad::checkTimeout()
  {
      if(inputDuration()>INPUTDURATION)
        currentInput="";
  }
  void keypad::checkInput()
  {
      int analogValue = analogRead(analogInPin);  //Read value from Analog In
      char key = mapAnalogToKey(analogValue);     //Translate value to key character
      if(currentInput.length()>=CODESIZE)         //Empty input string if size is bigger than code size
        currentInput="";
      checkTimeout();                             //If enough time has passed since last pressed key, empty input string

      if(key != '\0'  &&  keyPressed == false)    //Pressing a key to give input token
      {
            input_begin = millis(); // Last button press time
            currentInput += key;      // Add key to input string
            keyPressed=true;        //Key pressed
            no_key_begin=millis();  //Countdown to re-accept keys begins
            // Check if code arms or disarms alarm
            if(currentInput == disarmCode)
            {
                isArmed = false;
                currentInput = "";
            }
            else if(currentInput == armCode)
            {
                isArmed = true;
                currentInput = "";
            }
      }
      else if(key != '\0' &&  keyPressed == true) //Key has already been pressed and is still being pressed, input inactive until key is released for NOKEYDURATION ms
      {
        no_key_begin=millis();  //Countdown to re-accept keys renews
      }
      else if(key == '\0'  &&  keyPressed == false) //No key being pressed, input is active to accept next token
      {

      }
      else if(key == '\0' &&  keyPressed == true) //No key being pressed, key has been pressed, unlock input to accept next token if NOKEYDURATION ms has passed
      {
        checkNoKeyTimeout();  //Check if countdown to re-accept keys has passed
      }
  }
  void keypad::checkNoKeyTimeout()
  {
    if(noKeyDuration() > NOKEYDURATION)
      keyPressed=false;
  }
  unsigned long keypad::noKeyDuration()
  {
    no_key_end=millis();
    return no_key_end - no_key_begin;
  }



