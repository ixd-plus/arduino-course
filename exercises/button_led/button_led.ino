int button;   // value of the button

void setup() {
  pinMode(2, INPUT);    // button pin (in pull-down configuration)
  pinMode(13, OUTPUT);  // pin of the led on the Arduino board
}

void loop() {
  button = digitalRead(2);

  if(button == HIGH) {
    digitalWrite(13, HIGH);
  }
  else if(button == LOW) {
    digitalWrite(13, LOW);
  }

  delay(100);
}

