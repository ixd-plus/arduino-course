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
void GetTimerAndStartProcess(bool*, time_t*, time_t*, int*);
bool CheckTimer(bool*, const time_t*, const int);
void DisplayValueOnStrip(const int, const int);
void Blink(bool*);

void HandleTimer() {
  static long int toggleDelay = 0;
  static bool hasTimer = false;
  static bool hasBlinked = false;
  static bool hasReached = false;
  static time_t startTime = 0;
  static int countdownValue = 0;
  static time_t endTime = 0;
  if (hasTimer) {
    if (!hasReached) hasReached = CheckTimer(&hasBlinked, &endTime, countdownValue);
    if (!hasReached) return;
    if (!hasBlinked) {
      Blink(&hasBlinked);
      return;
    }
    hasTimer = false;
    hasReached = false;
    hasBlinked = false;
    startTime = 0;
    countdownValue = 0;
    endTime = 0;
    Serial.println(":TIMER>ENDED");
    return;
  }
  if (toggleDelay > 0) {
    --toggleDelay;
    return;
  }
  if (!!timerConfirmButton.Read()) {
    GetTimerAndStartProcess(&hasTimer, &startTime, &endTime, &countdownValue);
    toggleDelay = 100000L;
  }
}

void GetTimerAndStartProcess(bool* hasTimer, time_t* startTime, time_t* endTime, int* value) {
  const unsigned int read = timerChooserPotentiometer.Read();
  const unsigned int val = (read > 1023 - 5)? 0 : map(read, 5, 1023, 5 * 60, 10);
  if (val == 0) return;
  *hasTimer = true;
  *startTime = now();
  *endTime = *startTime + val;
  *value = val;
  Serial.print(":TIMER>");
  Serial.print(*startTime);
  Serial.print(">");
  Serial.print(val);
  Serial.print(">");
  Serial.print(*endTime);
  Serial.println();
}

bool CheckTimer(bool* hasBlinked, const time_t* endTime, const int val) {
  const time_t nowTime = now();
  const time_t difference = *endTime - nowTime;
  Serial.print(":TIMER>DIFF>");
  Serial.println(difference);
  if (difference <= 0) {
    *hasBlinked = false;
    return true;
  } else {
    DisplayValueOnStrip(difference, val);
    return false;
  }
}

void DisplayValueOnStrip(const int difference, const int val) {
  static const uint32_t onColor = ledStrip.Color(0, 128, 255);
  static const uint32_t offColor = ledStrip.Color(0, 0, 0);
  
  const int led = map(difference, 0, val, 0, LED_STRIP_LEDS - 1);
  Serial.print(":TIMER>DISPLAY>");
  Serial.print(difference);
  Serial.print(">");
  Serial.println(val);
  for (int i = 0; i < LED_STRIP_LEDS; ++i) {
    ledStrip.setPixelColor(i, i <= led? onColor : offColor);
  }
  ledStrip.show();
}

void Blink(bool* hasBlinked) {
  static int blinkAmount = 0;
  static int counter = 0;
  static bool on = true;
  if (blinkAmount > 20 * 4) {
    *hasBlinked = true;
    blinkAmount = 0;
    for (int i = 0; i < LED_STRIP_LEDS; ++i) ledStrip.setPixelColor(i, ledStrip.Color(0, 0, 0));
    ledStrip.show();
    return;
  }
  if (counter >= 0) {
    --counter;
    return;
  }
  Serial.print(":TIMER>BLINK>");
  Serial.print(blinkAmount);
  Serial.print(">");
  Serial.println(on);
  ++blinkAmount;

  for (int i = 0; i < LED_STRIP_LEDS; ++i) {
    if (blinkAmount == 20 * 4 - 1) continue;
    switch(blinkAmount % 4) {
      case 1:
        ledStrip.setPixelColor(i, ledStrip.Color(255, 0, 0));
        break;
      case 2:
        ledStrip.setPixelColor(i, ledStrip.Color(0, 255, 0));
        break;
      case 3:
        ledStrip.setPixelColor(i, ledStrip.Color(0, 0, 255));
        break;
      case 0:
        if (on) ledStrip.setPixelColor(i, ledStrip.Color(255, 255, 255));
        else ledStrip.setPixelColor(i, ledStrip.Color(0, 0, 0));
        break;
    }
  }
  ledStrip.show();
  if (blinkAmount % 4 == 0) on = !on;
  counter = 30000;
}

