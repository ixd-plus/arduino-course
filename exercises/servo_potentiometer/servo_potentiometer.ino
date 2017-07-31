#include <Servo.h>

Servo servo;
int pot = 0;  // potentiometer value
int pos = 0;  // servomotor position

void setup() {
    servo.attach(9);
}

void loop() {

  pot = analogRead(A0);   // 0-1023
  pos = map(pot, 0, 1023, 0, 180);
  servo.write(pos);    // 0-180

  delay(10);
}
