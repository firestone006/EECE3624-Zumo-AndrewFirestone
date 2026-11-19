
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
