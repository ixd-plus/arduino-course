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
bool BeginSerialCommunication();
void CheckSerialForMessages();
void CheckButtonForMode();
void CheckSerialForMode();
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
  while (!Serial);
  firstModeLed.TurnOn();
  secondModeLed.TurnOn();
  if (BeginSerialCommunication()) {
    for (auto i = 0; i < 3; ++i ) {
      firstModeLed.TurnOff();
      secondModeLed.TurnOff();
      delay(100);
      firstModeLed.TurnOn();
      secondModeLed.TurnOn();
      delay(100); 
    }
  } else {
    for (auto i = 0; i < 3; ++i) {
      firstModeLed.TurnOff();
      secondModeLed.TurnOff();
      delay(100);
      firstModeLed.TurnOn();
      delay(100);
      secondModeLed.TurnOn();
      delay(150);
      firstModeLed.TurnOff();
      delay(100);
    }
  }
  firstModeLed.TurnOff();
  secondModeLed.TurnOff();
  delay(300);
  Mode::CycleModeAndUpdateLeds(&currentMode, firstModeLed, secondModeLed);
}

bool BeginSerialCommunication() {
  Serial.println(":SYN");
  auto counter = 0;
  auto estabilished = false;
  while (true) {
    if (estabilished || counter >= 30) {
      if (!estabilished) Serial.println(":CONNECTION>FAILED");
      break; // Wait half a minute maximum, then break out and wait for a SYN/ACK request from the Java controller.
    }
    if (Serial.available() > 0) {
      while(true) {
        byte in = '\0';
        while (in != '`') {
          in = Serial.read();
          if (in == 0xFF) continue; 
        }
        String call = Serial.readStringUntil(';');
        if (!String("ACK").equals(call)) continue;
        Serial.println(":CONNECTION>ESTABILISHED");
        estabilished = true;
        break;
      }
    }
    ++counter;
    delay(1000); // Wait one second after every call
  }
  SerialPort::hasSerial = estabilished;
  return estabilished;
}

void Loop() {
  CheckSerialForMessages();
  CheckButtonForMode();
  CheckSerialForMode();
  DeferModeCall();
}

void CheckSerialForMessages() {
  if (!SerialPort::CanCommunicateWithSerial()) return;
  if (currentMode == Mode::STANDBY) return;
  if (Serial.available() <= 0) return;
  byte in = '\0';
  while (in != '`') {
    in = Serial.read();
    if (in == 0xFF) continue; 
  }
  auto call = Serial.readStringUntil(';');
  SerialPort::incomingMessagesQueue.push(call);
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

void CheckSerialForMode() {
  if (!SerialPort::CanCommunicateWithSerial()) return;
  if (currentMode == Mode::STANDBY) return;
  if (SerialPort::incomingMessagesQueue.count() <= 0) return;
  const auto message = SerialPort::incomingMessagesQueue.peek();
  if (!message.startsWith("MODE")) return;
  SerialPort::incomingMessagesQueue.pop();
  const auto newMode = message.substring(String("MODE>").length());
  const auto mode = Mode::GetModeFromString(newMode);
  while (currentMode != mode) {
    Mode::CycleModeAndUpdateLeds(&currentMode, firstModeLed, secondModeLed);
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
  if (!SerialPort::CanCommunicateWithSerial()) return;
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
      Mode::CycleModeAndUpdateLeds(&currentMode, firstModeLed, secondModeLed);
      break;
    }
  }
}

