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
#include<QueueList.h>
#include<Time.h>
#include<TimeLib.h>

// Declaring a constant in the global namespace, that's not a great idea, huh?
// LIBRARY WRITER, clean up after you've used them, thank you. Or at least document it.
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

/**
 * Identifies a LED attached to the Arduino board.
 */
class Led {
private:
  int pin;
  int level;
  void SendLevel() const;
public:
  /**
   * Constructs a new LED instance.
   * 
   * @param The pin where the LED is attached to.
   */
  Led(const int);
  /**
   * Sets the light level of the led to the specified one and sends the update.
   * 
   * @param The new light level. The values must be between 0 and 255.
   */
  void SetAndSendLevel(const int);
  /**
   * Updates the light level of the led of the specified delta and sends the update.
   * 
   * @param The delta between the current light level and the new one.
   */
  void UpdateAndSendLevel(const int);
  /**
   * Turns on the led, setting its light level to the highest value possible.
   */
  void TurnOn();
  /**
   * Turns off the led, setting its light level to the lowest value possible.
   */
  void TurnOff();
};

/**
 * Identifies a digital (0-1) button connected to the Arduino board.
 */
class Button {
private:
  int pin;
public:
  /**
   * Constructs a new button instance.
   * 
   * @param The pin the button is attached to.
   */
  Button(const int);
  /**
   * Reads the value currently supplied by the button.
   * 
   * <p>The value returned is either 0 (not pressed) or 1 (pressed)</p>
   * 
   * <p>This method is deprecated: consider using {@link #IsPressed()} instead.</p>
   * 
   * @return An integer (either 0 or 1).
   */
  [[deprecated("Replaced by IsPressed, which offers a more adequate solution")]]
  int Read() const;
  /**
   * Reads the value currently supplied by the button.
   * 
   * @return True if the button is pressed, false otherwise.
   */
   bool IsPressed() const;
   /**
    * Converts this object into a boolean.
    */
   operator bool() const;
};

/**
 * Identifies an analog potentiometer connected to the Arduino board.
 */
class Potentiometer {
private:
  int pin;
public:
  /**
   * Constructs a new potentiometer instance.
   * 
   * @param The pin the potentiometer is attached to.
   */
  Potentiometer(const int);
  /**
   * Reads the value given by the potentiometer.
   * 
   * @return The potentiometer's value.
   */
  unsigned int Read();
};

namespace Mode {
  /**
   * Identifies the current KloC mode.
   */
  enum Mode {
    /**
     * Indicates that KloC is currently working as a normal clock.
     */
    CLOCK,
    /**
     * Indicates that KloC is currently working as a timer, aka in count-down mode.
     */
    TIMER,
    /**
     * Indicates that KloC is now receiving commands through a serial communication.
     */
    SERIAL,
    /**
     * Indicates that KloC is currently in standby mode.
     */
    STANDBY
  };

  /**
   * Displays the mode on the LED mode indicator.
   * 
   * @param The mode to display.
   * @param The first of the two leds.
   * @param The second of the two leds.
   */
  void SwitchModeToLed(const enum Mode, class Led, class Led);
  /**
   * Cycles the current mode, moving onto the next one.
   * 
   * @param A pointer to the current mode, which will be used to set the new mode.
   */
  void CycleMode(enum Mode*);
  /**
   * Cycles the current mode and displays the new one on the led indicator.
   * 
   * @param A pointer to the current mode, which will be used to set the new mode.
   * @param The first of the two leds.
   * @param The second of the two leds.
   */
  void CycleModeAndUpdateLeds(enum Mode*, class Led, class Led);
  /**
   * Gets the mode that matches the given string.
   * 
   * <p>If no mode matches, then {@code STANDBY} is returned.
   * 
   * @param The enum mode.
   * @return The mode that matches the given string.
   */
  enum Mode GetModeFromString(const String);
}

namespace Display {
  /**
   * Identifies what the KloC is currently displaying on its led strip.
   */
  enum Display {
    /**
     * The clock is currently counting seconds.
     */
    SECONDS,
    /**
     * The clock is currently counting minutes.
     */
    MINUTES
  };

  /**
   * Identifies an array which maps the current minute to the led that should be turned on.
   * 
   * Array-based access is deprecated, you should use {@link #GetLedMatchingMinute(const int)} instead.
   */
  // TODO Move to std::unordered_map<int, short int> (Not available in Arduino)
  short int minuteToLed[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8,
                             9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15,
                             16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19};

  /**
   * Identifies an array which maps the current minute to the color of the led turned on.
   * 
   * Array-based access is deprecated, you should use {@link #GetMatchingLedColor(const int)} instead.
   */
  uint32_t colors[] = {0, 0, 0};

  /**
   * Gets the led that should be turned on according to the current minute.
   * 
   * @param The current minute.
   * @return The led that should be turned on.
   */
  short int GetLedMatchingMinute(const int);
  /**
   * Gets the color of the turned on led according to the current minute.
   * 
   * @param The current minute.
   * @return The led color.
   */
  uint32_t GetMatchingLedColor(const int);
}

namespace SerialPort {
  /**
   * Identifies if the current session can communicate through the serial port with a companion app.
   */
  bool hasSerial{false};

  /**
   * Queue used to store and manage all the various incoming messages from the serial port.
   */
  class QueueList<String> incomingMessagesQueue{QueueList<String>()};

  /**
   * Returns if the Arduino controller can communicate through a serial port.
   * 
   * <p>Before attempting to communicate through the serial port, you should check the result value of
   * this function.</p>
   * 
   * <p>It is suggested to use this function instead of accessing {@code hasSerial} directly.</p>
   * 
   * @return If there's somebody listening on the serial port.
   */
  bool CanCommunicateWithSerial();
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


