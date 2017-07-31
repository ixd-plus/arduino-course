int intensity = 0;
int dir = 1;

void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(intensity == 255) {
    dir = -1;
  }
  if(intensity == 0) {
    dir = 1;
  }
  intensity = intensity + dir;
  analogWrite(3, intensity);   // 0-255

  Serial.println(intensity);
  delay(10);
}
