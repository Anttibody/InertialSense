//
// Created by kylemallory on 3/3/23.
//

#ifndef STARTRACK_BLINKY_H
#define STARTRACK_BLINKY_H

#include <iostream>
#include <vector>
#include <chrono>

#include <spdlog/spdlog.h>
#include <pigpiod_if2.h>


/**
 * Simple class that handles blinking a single Tri-color RGB LED -- I know what you thinking?...
 * Does this really need its own class?  Probably not, but let's have fun with it anyway!
 *
 * It works like this:
 *   Initialize with the GPIO pins used to drive the 3 LED channels.
 *   A "color" bit mask represents which of the state of each of the LED channels.
 *   Call setPattern() to specify a the rates of change, and the "color" masks that are changed to.
 *   Call update() as frequently as possible.
 *
 *   update() will cycle between
 */

class Blinky {

private:
    int r_gpio, g_gpio, b_gpio;

    std::chrono::time_point<std::chrono::steady_clock> next_transition;
    std::vector<double> rates;
    std::vector<int> colors;
    int rate_i, color_i;

    int btn_gpio;
    int btn_active = PI_LOW;
    int debounce_ms = 100;

    int pigpioStatus = PI_NOT_INITIALISED;

public:

    enum Colors {
        OFF = 0x00,
        RED = 0x01,
        GREEN = 0x02,
        YELLOW = 0x03,
        BLUE = 0x04,
        MAGENTA = 0x05,
        CYAN = 0x06,
        WHITE = 0x07
    };


    Blinky(int gpio_red, int gpio_green, int gpio_blue, int gpio_btn) : r_gpio(gpio_red), g_gpio(gpio_green), b_gpio(gpio_blue), btn_gpio(gpio_btn) { };
    int getPiGPIOInstance() { return pigpioStatus; }
    void initGPIO(std::string gpio_host="127.0.0.1"); // allow late initialization (after constructor)
    void setLedGPIO(int gpio_red, int gpio_green, int gpio_blue);
    void setButtonGPIO(int gpio_btn, int active_state, int ms_debounce);

    void setLED(int mask);

    void setPattern(const std::vector<double>& rates_, const std::vector<int>& colors);
    void setPattern(double rate, const std::vector<int>& colors);
    void update();

    bool isButtonPressed();

    /** helper/utility functions for ease of use **/
    void basicBlink(double rate, char on, char off);

    const static std::vector<int> SOLID_OFF;

    const static std::vector<int> SOLID_RED;
    const static std::vector<int> SOLID_YELLOW;
    const static std::vector<int> SOLID_GREEN;
    const static std::vector<int> SOLID_CYAN;
    const static std::vector<int> SOLID_BLUE;
    const static std::vector<int> SOLID_MAGENTA;
    const static std::vector<int> SOLID_WHITE;

    const static std::vector<int> BLINK_RED;
    const static std::vector<int> BLINK_YELLOW;
    const static std::vector<int> BLINK_GREEN;
    const static std::vector<int> BLINK_CYAN;
    const static std::vector<int> BLINK_BLUE;
    const static std::vector<int> BLINK_MAGENTA;
    const static std::vector<int> BLINK_WHITE;

    const static std::vector<int> CYCLE_RED_WHITE;
    const static std::vector<int> CYCLE_RED_YELLOW;
    const static std::vector<int> CYCLE_RGB;
    const static std::vector<int> CYCLE_RGB_OFF;


};


#endif //STARTRACK_BLINKY_H
