/* ** Magnets game ** by Davide Tomì
 * Project made at the wikicourse "Design and program interactive objects", held by Marco Lurati and Giovanni Profeta in Esino Lario (Italy), on July 2017.
 */

#include <Adafruit_NeoPixel.h>

int valA;
int valB;
bool goalA, goalB;
int intensita_defaultA,intensita_defaultB;
const int margine = 30;

int puntiA;
int puntiB;

Adafruit_NeoPixel punteggio = Adafruit_NeoPixel(12, 9, NEO_GRB + NEO_KHZ800);


void setup() {
 Serial.begin (9600);

 Serial.println(F("- Magnets Game iniziato -"));
 

 punteggio.begin();
 
 resettaPunteggio();

 while(millis() < 5000) {
    intensita_defaultA = analogRead(A0);
    intensita_defaultB = analogRead(A1);
 }
 

 Serial.print(F("intensita_defaultA: "));
 Serial.println(intensita_defaultA);
 Serial.print(F("intensita_defaultB: "));
 Serial.println(intensita_defaultB);

  showStart();
}

void loop() {
  valA = analogRead(A0);
  valB = analogRead(A1);
  
  //Serial.println (valB);
  
  if (valA < (intensita_defaultA - margine) && goalA == LOW) {
    puntiA++;
    Serial.println("Goal A!");
    goalA = HIGH;
    aggiornaPunteggio();
    delay(1000);
  } else if (valA > (intensita_defaultB - margine)) {
    goalA = LOW;
  }

  if(valB < (intensita_defaultB - margine) && goalB == LOW) {
    puntiB++;
    Serial.println("Goal B!");
    goalB = HIGH;
    aggiornaPunteggio();
    delay(1000);
  } else if (valB > (intensita_defaultB - margine)) {
    goalB = LOW;
  }

  if(puntiA == 6 || puntiB == 6) {
    resettaPunteggio();
    if(puntiA < puntiB){
      Serial.println(F("B è il vincitore")); 
    } else {
      Serial.println(F("A è il vincitore")); 
    }
    puntiA = 0;
    puntiB = 0;
  }
  
  delay (100);
}

void resettaPunteggio(){
 // spegni i led del punteggio
 punteggio.clear();
 punteggio.show();
 
}

void showStart() {
  for(int i=0; i<12; i++) {
     punteggio.setPixelColor(i, punteggio.Color(0,200,0));
     punteggio.show();
     delay(300);
  }
  for(int i=0; i<12; i++) {
     punteggio.setPixelColor(i, punteggio.Color(0,0,0));
     punteggio.show();
     delay(300);
  }
  for(int i=0; i<12; i++) {
     punteggio.setPixelColor(i, punteggio.Color(0,200,0));
     punteggio.show();
     
  }
  delay(100);
  for(int i=0; i<12; i++) {
     punteggio.setPixelColor(i, punteggio.Color(0,0,0));
     punteggio.show();
     
  }
  delay(100);
}


void aggiornaPunteggio(){
  
  resettaPunteggio();

  for(int i=0; i < puntiA; i++) {
    punteggio.setPixelColor(5-i, punteggio.Color(255,50,0));
  }

  for(int i=6; i<(6+puntiB); i++) {
    punteggio.setPixelColor(i, punteggio.Color(50,70,255));
  }

  punteggio.show();
}

