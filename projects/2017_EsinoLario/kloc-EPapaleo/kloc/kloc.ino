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
#include<Adafruit_NeoPixel.h>
#include<Time.h>
#include<TimeLib.h>

// Declaring a constant in the global namespace, that's not a great idea, huh?
// Let's remove it here, it doesn't seem to be used anywhere else...
#ifdef SERIAL
# undef SERIAL
#endif

#define MODE_LED_FIRST_PIN 5
#define MODE_LED_SECOND_PIN 6
#define MODE_BUTTON_PIN 2
#define TIMER_CONFIRM_BUTTON_PIN 4
#define POTENTIOMETER_PIN A0
#define LED_STRIP_PIN 10

#define LED_STRIP_LEDS 20

#define RATE_SPEED 9600

class Led {
private:
  int pin;
  int level;
  void SendLevel() const;
public:
  Led(const int);
  void SetAndSendLevel(const int);
  void UpdateAndSendLevel(const int);
  void TurnOn();
  void TurnOff();
};

class Button {
private:
  int pin;
public:
  Button(const int);
  int Read() const;
};

class Potentiometer {
private:
  int pin;
public:
  Potentiometer(const int);
  unsigned int Read();
};

namespace Mode {
  enum Mode {
    CLOCK, TIMER, SERIAL, STANDBY
  };

  void SwitchModeToLed(const Mode, const Led*, const Led*);
  void CycleMode(enum Mode*);
}

namespace Display {
  enum Display {
    SECONDS, MINUTES
  };

  short int minuteToLed[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8,
                             9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15,
                             16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19};

  uint32_t colors[] = {0, 0, 0};
}

class Led firstModeLed = Led(MODE_LED_FIRST_PIN);
class Led secondModeLed = Led(MODE_LED_SECOND_PIN);
class Button modeSwitchButton = Button(MODE_BUTTON_PIN);
enum Mode::Mode currentMode = Mode::STANDBY;

class Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(LED_STRIP_LEDS, LED_STRIP_PIN);

class Potentiometer timerChooserPotentiometer = Potentiometer(POTENTIOMETER_PIN);
class Button timerConfirmButton = Button(TIMER_CONFIRM_BUTTON_PIN);

void Setup();
void Loop();

void setup() {
  Setup();
}

void loop() {
  Loop();
}


