// ** Home Meteo Alarm **  by Fabio Biffi
// Project made at the wikicourse "Design and program interactive objects", held by Marco Lurati and Giovanni Profeta in Esino Lario (Italy), on July 2017.


#include <Time.h>
#include <TimeLib.h>

#include <Ultrasonic.h>   //nuova libreria
Ultrasonic sensore(12,13);
int dist=10;                      //distanza iniziale  misurata dal sensore
int alto=0;
time_t tempoDistanzaMassima;
time_t tempoDistanzaMinima;
bool tempoMinimoImpostato;  
 
void setup() {
  pinMode(8,OUTPUT);  
  Serial.begin(9600);            //istruzione per vedere a monitor:
 }

void loop() {
 dist = sensore.distanceRead();

 if (dist == 10)
    {
    Serial.println ("NON PIOVE!!!!") ;
    tempoDistanzaMassima = now();
    }  
                                              
  if(dist <= 5){
    Serial.println ("Allarme in corso");
    if (!tempoMinimoImpostato) {
      tempoDistanzaMinima = now();
      tempoMinimoImpostato = true;
    }
    Serial.print(tempoDistanzaMassima);
    Serial.print(" --> ");
    Serial.println(tempoDistanzaMinima);
    if(minute(tempoDistanzaMassima) - minute(tempoDistanzaMinima) <= 2)
    {
      digitalWrite(8,HIGH); 
      delay(20);
      
      
      digitalWrite(8,LOW); 
      delay(20);

      tone(A0,955,1500);
      delay(200);
    }
    
    } 
    else if (dist < 6) {
    tempoMinimoImpostato = false;
    tempoDistanzaMinima = NULL;
    }
  Serial.println(dist);
  Serial.print("---> ");
  Serial.println(alto);
  delay(200);

    }
