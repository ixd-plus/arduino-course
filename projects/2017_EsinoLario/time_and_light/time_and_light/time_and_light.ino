/* ** Time and light ** by Mario Gionfini
 * Project made at the wikicourse "Design and program interactive objects", held by Marco Lurati and Giovanni Profeta in Esino Lario (Italy), on July 2017.
 */

#include <Servo.h>;

    Servo myservo;
  
  int pos= 0;  // variabile posizione servo
  int intensita;
  int intensita_max;
  int pos_max;
  int start;
void setup() {

  Serial.begin(9600);
  
  myservo.attach(9);
  
  pinMode(A0, INPUT);  // luce
  // pinMode( , OUTPUT);   // led
  pinMode(10, INPUT); 
  } 

void loop() {
 
 
  intensita = analogRead(A0);
  start = digitalRead(10);
  Serial.println(intensita);

  if(start == HIGH) {
    pos=0;
  }
  
  myservo.write(pos);    //  0-180

  if(intensita > intensita_max) {
    intensita_max = intensita;
    pos_max = pos;
    Serial.println(pos_max);
  }

  
  if(pos == 110) { 
    myservo.write(pos_max);
    Serial.println("Posizione max");
      
  } else {
    pos++;
  }
  delay(200); 

  
}
 
