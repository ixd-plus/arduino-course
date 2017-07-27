#include <Servo.h>
int pot;
int pos;

Servo servo1;

void setup() {
  servo1.attach(9);
}

void loop() {
  pot = analogRead(A0);
  pos = map(pot, 0, 1023, 0,180);
  servo1.write(pos);
}
