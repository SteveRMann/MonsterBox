#define SKETCH "ServoSweep.ino"

const int servoPin = D1;

#include <Servo.h>
Servo myservo;

int holePosition;

void setup() {
  Serial.begin( 115200 );
  Serial.println();
  Serial.println(SKETCH);


  /*
    Specs in https://www.servocity.com/hs-645mg-servo/
    pin: the number of the pin that the servo is attached to
    min: pulse width, in us, corresponding to the 0 degree angle on the servo (defaults to 544)
    max: pulse width, in us, corresponding to the 180 degree angle on the servo (defaults to 2400)

    Use ServoManualSweep.ino to calibrate these end points.
  */
  //myservo.attach(servoPin, 400, 2400);    //PWM range.
  myservo.attach(servoPin, 625, 2400);    //PWM range.

  myservo.write(0);                       //Start at zero
  holePosition = 0;
  delay(5000);
}


void loop() {
  dropEgg();
  delay(2000);
}

void dropEgg() {
  printf("holePosition = %d\n", holePosition);
  if (holePosition == 0) {
    myservo.write(90);
    delay(500);
    myservo.write(180);
    holePosition = 180;
    delay(1000);
  } else {
    myservo.write(90);
    delay(500);
    myservo.write(0);
    holePosition = 0;
    delay(1000);
  }
}
