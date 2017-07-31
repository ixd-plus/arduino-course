void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  int pot = analogRead(A0);
  Serial.println(pot);

  if (pot > 500) {
    digitalWrite(13, HIGH);
  } else if (pot < 500) {
    digitalWrite(13, LOW);
  }

  delay(200);
}
