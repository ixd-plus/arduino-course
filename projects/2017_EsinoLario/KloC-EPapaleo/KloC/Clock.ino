/*
 * KloC Arduino Controller
 * Copyright (C) 2017  TheSilkMiner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact information:
 * E-mail: thesilkminer <at> outlook <dot> com
 */
void TryReadCurrentTimeMillisFromSerial();
void HandleWhatWeShouldDisplay(enum Display::Display*, int*);
void ResetStrip(int*);
int GetTimeToDisplay(const time_t, const enum Display::Display);
void DisplayValueOnStrip(const int);
unsigned long long int ParseTime(const String);

void HandleClock() {
  static enum Display::Display whatWeShouldDisplay = Display::MINUTES;
  static int previousTimeToDisplay = 0;
  
  TryReadCurrentTimeMillisFromSerial();
  time_t currentTimeMillis = now();

  HandleWhatWeShouldDisplay(&whatWeShouldDisplay, &previousTimeToDisplay);
  const int timeToDisplay = GetTimeToDisplay(currentTimeMillis, whatWeShouldDisplay);
  if (previousTimeToDisplay == timeToDisplay) return;

  Serial.print(":CLOCK>DISP>VAL>");
  Serial.println(timeToDisplay);
  DisplayValueOnStrip(timeToDisplay);
  previousTimeToDisplay = timeToDisplay;
}

void HandleWhatWeShouldDisplay(enum Display::Display* disp, int* previousTimeToDisplay) {
  static long int toggleDelay = 0;
  if (toggleDelay > 0) {
    --toggleDelay;
    return;
  }
  if (!!timerConfirmButton.Read()) {
    switch(*disp) {
      case Display::SECONDS:
        *disp = Display::MINUTES;
        break;
      case Display::MINUTES:
        *disp = Display::SECONDS;
        break;
    }
    Serial.print(":CLOCK>DISP>");
    Serial.println(*disp);
    ResetStrip(previousTimeToDisplay);
    toggleDelay = 10000L;
  }
}

void ResetStrip(int* i) {
  for (int i = 0; i < LED_STRIP_LEDS; ++i) ledStrip.setPixelColor(i, ledStrip.Color(0, 0, 0));
  ledStrip.show();
  *i = 0;
}

int GetTimeToDisplay(const time_t time, const enum Display::Display what) {
  switch(what) {
    case Display::SECONDS:
    default:
      return second(time);
    case Display::MINUTES:
      return minute(time);
  }
}

void DisplayValueOnStrip(const int value) {
  if (value <= 0 || value >= 60) {
    for (int i = 0; i < LED_STRIP_LEDS; ++i) ledStrip.setPixelColor(i, ledStrip.Color(0, 0, 0));
    ledStrip.show();
  }
  short int led = Display::minuteToLed[value];
  uint32_t color = Display::colors[value % 3];
  ledStrip.setPixelColor(led, color);
  for (int i = 0; i < led; ++i) ledStrip.setPixelColor(i, Display::colors[2]);
  ledStrip.show();
}

void TryReadCurrentTimeMillisFromSerial() {
  if (Serial.available() > 0) {
    while(true) {
      byte in = '\0';
      while (in != '`') {
        in = Serial.read();
        if (in == 0xFF) continue; 
      }
      String time = Serial.readStringUntil(';');
      unsigned long long int seconds = ParseTime(time);
      setTime(seconds);
      break;
    }
    Serial.println(":GOTTEN");
  }
}

unsigned long long int ParseTime(const String givenTime) {
  char time[givenTime.length() + 5];
  givenTime.toCharArray(time, sizeof(time));
  return atol(time);
}

