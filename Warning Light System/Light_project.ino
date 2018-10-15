/*
* Created by Nathaniel Mitchell - Limmtec Pty Ltd
*                                 (C) 2014
* 
* Uses Digital IOs to programmtically turn on/off relays (which then turn external relays on and off
*
* Based on sample programs from
*                               - 8ch relay - Freetronics
*                               - Watchdog - Freetronics
*                               - 
*                               - 
*/

// Set the PIN locations
const int pinWatchdog = 0;
const int pinSwitch1 = 2;
const int pinSwitch2 = 3;
const int pinSwitch3 = 4;
const int pinSwitch4 = 5;
const int pinSwitch5 = 6;
const int pinSwitch6 = 7;
const int pinSwitch7 = 8;
const int pinSwitch8 = 9;
const int pinStatusLED = 11;

// Used for reading the switch input
int varSwitch1 = 0;
int varSwitch2 = 0;
int varSwitch3 = 0;
int varSwitch4 = 0;
int varSwitch5 = 0;
int varSwitch6 = 0;
int varSwitch7 = 0;
int varSwitch8 = 0;

// Used to find out if the switch was pressed and is toggled to it's opposite state.
boolean varSwitchStatus1 = false;
boolean varSwitchStatus2 = false;
boolean varSwitchStatus3 = false;
boolean varSwitchStatus4 = false;
boolean varSwitchStatus5 = false;
boolean varSwitchStatus6 = false;
boolean varSwitchStatus7 = false;
boolean varSwitchStatus8 = false;

// What was the previous loop's button state variables
boolean varSwitchPrevious1 = false;
boolean varSwitchPrevious2 = false;
boolean varSwitchPrevious3 = false;
boolean varSwitchPrevious4 = false;
boolean varSwitchPrevious5 = false;
boolean varSwitchPrevious6 = false;
boolean varSwitchPrevious7 = false;
boolean varSwitchPrevious8 = false;

// Used for the LED "breathing" status light.
const int varBreatheStart = 32; // At what brightness should the LED be started at
const int varBreatheStep = 15; // How many steps between last brightness and new. Should be smaller than the Breathing Start amount
const int varBreatheDelay = 40; // How many milliseconds to wait between each step in the breath
boolean varBreatheDirection = true; // A toggle to work out are we breathing IN or OUT
int varBreatheCount = 0; // A counter for the breathing

// A variable for which relays to turn on/off
int varCurrentRelayConf = 0;

// Include the wire headers
#include "Wire.h"

// Set's the relay shield ADDRESS
#define Relay1_ADDR  0x20

// Applies a TTL high, waits 20ms then resets the line to low
void FUNC_watchdog() {
    digitalWrite(pinWatchdog, HIGH);
    delay(20);
    digitalWrite(pinWatchdog, LOW);
}

// Applies the bit configuration to 
void FUNC_relay(int varValue)
{
  Wire.beginTransmission(Relay1_ADDR);
  Wire.write(0x12);        // Select GPIOA
  Wire.write(varValue);  // Send value to bank A
  Wire.endTransmission();
}

void setup() {
  
  Serial.begin( 38400 );
  varBreatheCount = varBreatheStart;
  
  // Configure the pins
  pinMode(pinWatchdog, OUTPUT);
  pinMode(pinStatusLED, OUTPUT);
  pinMode(pinSwitch1, INPUT_PULLUP);
  pinMode(pinSwitch2, INPUT_PULLUP);
  pinMode(pinSwitch3, INPUT_PULLUP);
  pinMode(pinSwitch4, INPUT_PULLUP);
  pinMode(pinSwitch5, INPUT_PULLUP);
  pinMode(pinSwitch6, INPUT_PULLUP);
  pinMode(pinSwitch7, INPUT_PULLUP);
  pinMode(pinSwitch8, INPUT_PULLUP);
    
  // Wake up & Reset the relay shield
  Wire.begin(); // Wake up I2C bus
  Wire.beginTransmission(Relay1_ADDR);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // Set all of bank A to outputs
  Wire.endTransmission();
}

void loop() {
  
// Reset the watchdog timer
  FUNC_watchdog();
  
// Read all the button status  
  varSwitch1 = digitalRead(pinSwitch1);
  varSwitch2 = digitalRead(pinSwitch2);
  varSwitch3 = digitalRead(pinSwitch3);
  varSwitch4 = digitalRead(pinSwitch4);
  varSwitch5 = digitalRead(pinSwitch5);
  varSwitch6 = digitalRead(pinSwitch6);
  varSwitch7 = digitalRead(pinSwitch7);
  varSwitch8 = digitalRead(pinSwitch8);
  
// If the button is no longer pressed, reset the previous state variable to false so the button can be "pressed" again. Otherwise it is ignored
  if (varSwitch1 == HIGH) {
    varSwitchPrevious1 = false;
  }
  
  if (varSwitch2 == HIGH) {
    varSwitchPrevious2 = false;
  }
  
  if (varSwitch3 == HIGH) {
    varSwitchPrevious3 = false;
  }
  
  if (varSwitch4 == HIGH) {
    varSwitchPrevious4 = false;
  }
  
  if (varSwitch5 == HIGH) {
    varSwitchPrevious5 = false;
  }
  
  if (varSwitch6 == HIGH) {
    varSwitchPrevious6 = false;
  }
  
  if (varSwitch7 == HIGH) {
    varSwitchPrevious7 = false;
  }
  
  if (varSwitch8 == HIGH) {
    varSwitchPrevious8 = false;
  }
  
// Check to see if the button was pressed (and that the previous loop the button wasn't pressed [IE so holding the button doesnt constantly cycle the relays on and oof).
// If it was, flip the status then process to turn on/off the relay
  if (varSwitch1 == LOW && varSwitchPrevious1 == false) {
    varSwitchPrevious1 = true;
    varSwitchStatus1 = !varSwitchStatus1;
    if (varSwitchStatus1 == true) {
      varCurrentRelayConf = varCurrentRelayConf + 1;
    }
    else
    {
      varCurrentRelayConf = varCurrentRelayConf - 1;
    }
  }
  if (varSwitch2 == LOW && varSwitchPrevious2 == false) {
    varSwitchPrevious2 = true;
    varSwitchStatus2 = !varSwitchStatus2;
    if (varSwitchStatus2 == true) {
      varCurrentRelayConf = varCurrentRelayConf + 2;
    }
    else
    {
      varCurrentRelayConf = varCurrentRelayConf - 2;
    }
  }
  if (varSwitch3 == LOW && varSwitchPrevious3 == false) {
    varSwitchPrevious3 = true;
    varSwitchStatus3 = !varSwitchStatus3;
    if (varSwitchStatus3 == true) {
      varCurrentRelayConf = varCurrentRelayConf + 4;
    }
    else
    {
      varCurrentRelayConf = varCurrentRelayConf - 4;
    }
  }
  if (varSwitch4 == LOW && varSwitchPrevious4 == false) {
    varSwitchPrevious4 = true;
    varSwitchStatus4 = !varSwitchStatus4;
    if (varSwitchStatus4 == true) {
      varCurrentRelayConf = varCurrentRelayConf + 8;
    }
    else
    {
      varCurrentRelayConf = varCurrentRelayConf - 8;
    }
  }
  if (varSwitch5 == LOW && varSwitchPrevious5 == false) {
    varSwitchPrevious5 = true;
    varSwitchStatus5 = !varSwitchStatus5;
    if (varSwitchStatus5 == true) {
      varCurrentRelayConf = varCurrentRelayConf + 16;
    }
    else
    {
      varCurrentRelayConf = varCurrentRelayConf - 16;
    }
  }
  if (varSwitch6 == LOW && varSwitchPrevious6 == false) {
    varSwitchPrevious6 = true;
    varSwitchStatus6 = !varSwitchStatus6;
    if (varSwitchStatus6 == true) {
      varCurrentRelayConf = varCurrentRelayConf + 32;
    }
    else
    {
      varCurrentRelayConf = varCurrentRelayConf - 32;
    }
  }
  if (varSwitch7 == LOW && varSwitchPrevious7 == false) {
    varSwitchPrevious7 = true;
    varSwitchStatus7 = !varSwitchStatus7;
    if (varSwitchStatus7 == true) {
      varCurrentRelayConf = varCurrentRelayConf + 64;
    }
    else
    {
      varCurrentRelayConf = varCurrentRelayConf - 64;
    }
  }
  if (varSwitch8 == LOW && varSwitchPrevious8 == false) {
    varSwitchPrevious8 = true;
    varSwitchStatus8 = !varSwitchStatus8;
    if (varSwitchStatus8 == true) {
      varCurrentRelayConf = varCurrentRelayConf + 128;
    }
    else
    {
      varCurrentRelayConf = varCurrentRelayConf - 128;
    }
  }

// Write the current relay configruation to the relay board
  FUNC_relay(varCurrentRelayConf);
  
  Serial.println(" 1    2    3    4    5    6    7    8    Value");
  if (varSwitchStatus1 == LOW) {
    Serial.print("Off  ");
  }
  else
  {
    Serial.print(" On  ");
  }
  if (varSwitchStatus2 == LOW) {
    Serial.print("Off  ");
  }
  else
  {
    Serial.print(" On  ");
  }
  if (varSwitchStatus3 == LOW) {
    Serial.print("Off  ");
  }
  else
  {
    Serial.print(" On  ");
  }
  if (varSwitchStatus4 == LOW) {
    Serial.print("Off  ");
  }
  else
  {
    Serial.print(" On  ");
  }
  if (varSwitchStatus5 == LOW) {
    Serial.print("Off  ");
  }
  else
  {
    Serial.print(" On  ");
  }
  if (varSwitchStatus6 == LOW) {
    Serial.print("Off  ");
  }
  else
  {
    Serial.print(" On  ");
  }
  if (varSwitchStatus7 == LOW) {
    Serial.print("Off  ");
  }
  else
  {
    Serial.print(" On  ");
  }
  if (varSwitchStatus8 == LOW) {
    Serial.print("Off  ");
  }
  else
  {
    Serial.print(" On  ");
  }
  Serial.println(varCurrentRelayConf);
  Serial.println(" ");
  Serial.println("Direction   BreatheCount");
  if (varBreatheDirection == true) {
    Serial.print("Forwards       ");
  }
  else
  {
    Serial.print("Backwards      ");
  }
  Serial.println(varBreatheCount);
  delay(100);
  Serial.write(27);   //Print "esc"
  Serial.print("[2J");

 if (varBreatheDirection == true) {
    varBreatheCount = varBreatheCount + varBreatheStep;
    if (varBreatheCount >= 250) {
      varBreatheCount = varBreatheCount - varBreatheStep;
      varBreatheDirection = false;
      analogWrite(pinStatusLED, varBreatheCount);
      delay(varBreatheDelay);
    }
    else
    {
      analogWrite(pinStatusLED, varBreatheCount);
      delay(varBreatheDelay);
    }
  }
  else
  {
    varBreatheCount = varBreatheCount - varBreatheStep;
    if (varBreatheCount == varBreatheStart) {
      varBreatheCount = varBreatheCount + varBreatheStep;
      varBreatheDirection = true;
      analogWrite(pinStatusLED, varBreatheCount);
      delay(varBreatheDelay);
    }
    else
    {
      analogWrite(pinStatusLED, varBreatheCount);
      delay(varBreatheDelay);
    }
  }
}
