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
namespace Mode {
  void SwitchModeToLed(const Mode mode, const Led* first, const Led* second) {
    switch (mode) {
      case CLOCK:
        first->TurnOn();
        second->TurnOff();
        break;
      case TIMER:
        first->TurnOff();
        second->TurnOn();
        break;
      case SERIAL:
        first->TurnOn();
        second->TurnOn();
        break;
      case STANDBY:
      default:
        first->TurnOff();
        second->TurnOff();
        break;
    }
  }

  void CycleMode(enum Mode* mode) {
    switch(*mode) {
      case CLOCK:
        *mode = TIMER;
        break;
      case TIMER:
        *mode = SERIAL;
        break;
      case SERIAL:
        *mode = STANDBY;
        break;
      case STANDBY:
        *mode = CLOCK;
        break;
    }
    Serial.print(":MODE>");
    Serial.println(*mode == CLOCK? "CLOCK" : *mode == TIMER? "TIMER" : *mode == SERIAL? "SERIAL" : "STANDBY");
  }
}

