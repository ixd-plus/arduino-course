// Arduino counts from 1 to 10 and from 10 to 1

int num = 1;
int dir;

void setup() {
	Serial.begin(9600);

}

void loop() {
	if (num == 10) {
		dir = -1;
		Serial.print("dir = -1");
	}
	if (num == 0) {
		dir = 1;
		Serial.print("dir = -1");
	}

	num = num + dir;
	Serial.print(num);
	delay(1000);
}
