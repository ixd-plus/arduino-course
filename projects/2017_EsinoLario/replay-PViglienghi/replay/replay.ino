/* ** Replay ** by Patrick Viglienghi
 * Project made at the wikicourse "Design and program interactive objects", held by Marco Lurati and Giovanni Profeta in Esino Lario (Italy), on July 2017.
 */

const int pinlead1 = 4;
const int pinlead2 = 2;
const int pinlead3 = 8;
const int pinbutton=7;
const int pinspeaker=11;
const int pinbutton2=12;
int interruttore2;
int sens;
int pot;
int interruttore;
int stepon = 0;
int f1=261;
int f2=293;
int f3=329;


void setup() {
  Serial.begin(9600);
  pinMode(pinlead1, OUTPUT);
  pinMode(pinlead2, OUTPUT);
  pinMode(pinlead3, OUTPUT);
  pinMode(pinspeaker, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  pinMode(pinbutton,INPUT);
  pinMode(pinbutton2,INPUT);
  play();
}
void loop() {
  sens = analogRead(A0);
  pot = analogRead(A2);
  interruttore  =digitalRead(pinbutton);
 interruttore2 =digitalRead(pinbutton2);
  sens=map(sens,0,1023,100,5000);
  

  if(interruttore == HIGH){
    play();
 }
  else
 {
    noTone(pinspeaker);
 }
   
 if(interruttore2 == HIGH){
   readsensor();
   }
 
    
}
  

void play(){

  if(stepon == 3)
   {
    stepon = 1;
  }  
    else 
    {
      stepon = stepon+1;
      }
      
  if(stepon == 1){
    tone(pinspeaker,f1);
    digitalWrite(pinlead1,HIGH);
    digitalWrite(pinlead2,LOW);
    digitalWrite(pinlead3,LOW);
  }
  else if (stepon == 2){
    tone(pinspeaker,f2);
    digitalWrite(pinlead2,HIGH);
    digitalWrite(pinlead1,LOW);
    digitalWrite(pinlead3,LOW);
  }
  else if (stepon == 3){
    tone(pinspeaker,f3);
    digitalWrite(pinlead3,HIGH);
    digitalWrite(pinlead2,LOW);
    digitalWrite(pinlead1,LOW);
  }
    delay(pot);

}

  
  void readsensor() {

  tone(pinspeaker,sens);
  Serial.println(sens);
    
  if(stepon == 1) {
    f1 =sens;
  }
  if(stepon == 2) {
    f2 = sens;
  }
  if(stepon == 3) {
    f3 = sens;
 }

 delay(500);
}

