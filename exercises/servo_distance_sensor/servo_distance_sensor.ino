#include <Ultrasonic.h>   // install the "Ultrasonic" library from the library manager
#include <Servo.h>

Servo servo;
Ultrasonic sensor(12,13);

int dist = 0;   // sensor distance
int pos = 0;  // servo position, 0 - 180 degrees

void setup() {
  servo.attach(9);
  Serial.begin(9600);
}

void loop() {

  dist = sensor.distanceRead();
  pos = map(dist, 0, 255, 0, 180);
  
  Serial.println(pos);
  servo.write(pos);
  
  delay(300);
}
