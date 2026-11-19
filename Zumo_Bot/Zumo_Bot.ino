/*
  Simple character read.  This is looking for an "H" or and "L"
  and will set the LED tied to the given pin on or off.

  "SoftwareSerial" is a library that does a bit-bang USART interface on 
  any two digital GPIOs on an arduino.  This allows you to leave the 
  primary USART for debugging.

  This routine setups a USART on pins 2 and 4 named "bluetooth".  It then
  setups up this uart at 9600 baud.  In the loop, it checks to see if a
  character has arrived, and then reads it.

  The LED13 (built in LED) is blinked to show the program is running.  This
  blink can be moved inside the bluetooth if statement for debugging, or
  the normal serial pipe can be used.
*/

#include <SoftwareSerial.h>
#include <Wire.h>
#include <ZumoShield.h>

ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

unsigned char lastByte = 'S';

const int TURN_SPEED = 200;
int SPEED = 400;

// HC-05
SoftwareSerial bluetooth(2,4);  // RX, TX

void waitForButtonAndCountDown()
{
  button.waitForButton();

  // play audible countdown
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}

void setup() {
  // Setup the HC-05 bluetooth device
  // (HC)=(Uno).  VCC=5V, GND=GND, TX=RX(pin2),
  // RX=DNC! (possible damage if 5V Uno TX drives to HC-05)
  bluetooth.begin(9600);
  waitForButtonAndCountDown();
}

void loop() {
  if (bluetooth.available()) {  
    lastByte = (unsigned char) bluetooth.read();
  }

  if(lastByte == 'S'){
    motors.setSpeeds(0, 0);
  }
  if (lastByte == 'R'){
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
  }
  if (lastByte == 'L'){
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
  }
  if (lastByte == 'F'){
    motors.setSpeeds(SPEED, SPEED);
  }
  if (lastByte == 'B'){
    motors.setSpeeds(-SPEED, -SPEED);
  }
  if (lastByte == 'X') {
    if (SPEED == 400) {
      SPEED = 200;
      Serial.println("Half speed");
    }
    else {
      SPEED = 400;
      Serial.println("Full speed");
    }

    lastByte = 'S';
  }
}
