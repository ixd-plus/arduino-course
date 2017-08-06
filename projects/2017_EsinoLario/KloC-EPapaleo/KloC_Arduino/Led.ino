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
void Led::SendLevel() const {
  analogWrite(this->pin, this->level);
}

Led::Led(const int pin) {
  this->pin = pin;
  pinMode(pin, OUTPUT);
  this->level = 0;
}

void Led::SetAndSendLevel(const int lvl) {
  this->level = lvl;
  this->SendLevel();
}

void Led::UpdateAndSendLevel(const int lvl) {
  this->level += lvl;
  this->SendLevel();
}

void Led::TurnOn() {
  this->level = 255;
  this->SendLevel();
}

void Led::TurnOff() {
  this->level = 0;
  this->SendLevel();
}

