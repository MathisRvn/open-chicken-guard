#include <Servo.h>

#define MOTOR_BTN_PIN 6

Servo servo;

// 0 -> clockwise
// 180 -> backwise
// 90 -> stop

int getInterrupt () {
  return digitalRead(MOTOR_BTN_PIN);  
}

void stopMotor () {
 servo.write(90);  
}
void clockWise () {
 servo.write(0);  
}
void backWise () {
 servo.write(180);  
}

void openDoor () {
  
  if(getInterrupt() == 0) {
    // A ouvrir
    bool up = false;
    while (up == false) {
      wdt_reset();
      clockWise();
      if (getInterrupt() == 1) {
        up = true;  
        stopMotor();
      }
      delay(1);  
    }  
  } // Sinon la porte est déjà ouverte
  
  stopMotor();
}


void closeDoor () {
  
  if (getInterrupt() == 1) {
    backWise();
    sleepLong(getMotorCloseDelay());
    stopMotor();
  } // Sinon la porte est déjà fermé

  stopMotor();
  
}
