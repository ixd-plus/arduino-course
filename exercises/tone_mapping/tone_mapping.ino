int pot;    // potentiometer value
int freq;   // tone frequence

void setup() {
  pinMode(3, OUTPUT);   // speaker pin
  Serial.begin(9600);
}

void loop() {
  pot = analogRead(A0);   // analog pin reading, value between 0-1023
  freq = map(pot, 0, 1023, 2000, 50);   // map the potentiometer value to specific frequences
  
  Serial.print(pot);
  Serial.print(" become: ");
  Serial.println(freq);
  
  tone(3, freq, 100);
  delay(300);
}

