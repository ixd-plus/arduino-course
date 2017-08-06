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
bool AttemptConnection();

void HandleSerial() {
  if (!AttemptConnection()) {
    Mode::CycleModeAndUpdateLeds(&currentMode, firstModeLed, secondModeLed);
    Mode::CycleModeAndUpdateLeds(&currentMode, firstModeLed, secondModeLed);
    return;
  }
}

bool AttemptConnection() {
  static bool connected = false;
  
  if (connected) return true;
  
  if (SerialPort::CanCommunicateWithSerial()) {
    return true;
  } else {
    Serial.println(":SYN");
    auto counter = 0;
    auto estabilished = false;
    while (true) {
      if (estabilished || counter >= 5) {
        if (!estabilished) Serial.println(":CONNECTION>FAILED");
        break;
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
}

