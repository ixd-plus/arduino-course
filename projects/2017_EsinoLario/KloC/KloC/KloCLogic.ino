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
void CheckButtonForMode();
void DeferModeCall();
void HandleClock();
void HandleTimer();
void HandleSerial();
void CheckForWakeUpCall();

void Setup() {
  Serial.begin(RATE_SPEED);
  ledStrip.begin();
  ledStrip.clear();
  ledStrip.show();
  Display::colors[0] = ledStrip.Color(255, 128, 0);
  Display::colors[1] = ledStrip.Color(255, 0, 128);
  Display::colors[2] = ledStrip.Color(255, 255, 255);
  Mode::CycleModeAndUpdateLeds(&currentMode, firstModeLed, secondModeLed);
  while (!Serial);
}

void Loop() {
  CheckButtonForMode();
  DeferModeCall();
}

void CheckButtonForMode() {
  static long int toggleDelay = 0;
  if (toggleDelay > 0) {
    --toggleDelay;
    return;
  }
  if (modeSwitchButton) {
    Mode::CycleModeAndUpdateLeds(&currentMode, firstModeLed, secondModeLed);
    ledStrip.clear();
    ledStrip.show();
    toggleDelay = 100000L;
  }
}

void DeferModeCall() {
  switch(currentMode) {
    case Mode::CLOCK:
      HandleClock();
      break;
    case Mode::TIMER:
      HandleTimer();
      break;
    case Mode::SERIAL:
      HandleSerial();
      break;
    case Mode::STANDBY:
    default:
      CheckForWakeUpCall();
  }
}

void CheckForWakeUpCall() {
  if (Serial.available() > 0) {
    while(true) {
      byte in = '\0';
      while (in != '`') {
        in = Serial.read();
        if (in == 0xFF) continue; 
      }
      String call = Serial.readStringUntil(';');
      if (!String("WAKEUP").equals(call)) continue;
      Serial.println(":WOKEN");
      Mode::CycleMode(&currentMode);
      Mode::SwitchModeToLed(currentMode, firstModeLed, secondModeLed);
      break;
    }
  }
}

