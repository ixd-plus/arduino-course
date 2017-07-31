#include <Servo.h>

Servo servo1;
int pos = 0;    // servomotor position

void setup() {
  servo1.attach(9);
  Serial.begin(9600);
}

void loop() {

  for(int i=0; i<180; i=i+1) {
    Serial.println(i);
    servo1.write(i);
    delay(100);
  }

  for(int i=180; i>0; i--) {
    Serial.println(i);
    servo1.write(i);
    delay(100);
  }

}

