#include <iostream>

#include <spdlog/spdlog.h>
#include <pigpiod_if2.h>

#include <StarTrack.h>
#include <Blinky.h>

#define LED_RED_GPIO    16
#define LED_GREEN_GPIO  20
#define LED_BLUE_GPIO   21
#define BUTTON_GPIO     26
#define BUTTON_ACTIVE   PI_LOW

Blinky blinky(LED_RED_GPIO, LED_GREEN_GPIO, LED_BLUE_GPIO, BUTTON_GPIO);
StarTrack starTrack(blinky);


int main(int argc, char **argv) {

    starTrack.initialize();
    if (gpio_read(blinky.getPiGPIOInstance(), BUTTON_GPIO) == BUTTON_ACTIVE) {
        blinky.isButtonPressed();
        usleep(100000); // sleep long enough for debounce to register before we check again.
        if (blinky.isButtonPressed()) {
            spdlog::set_level(spdlog::level::debug);
            blinky.setPattern(0.2, Blinky::CYCLE_RGB_OFF);
            // don't continue until the button is released, but show the CYCLE_RGB so the user knows we have received their command.
            while (blinky.isButtonPressed()) {
                blinky.update();
                usleep(100000);
            }
        }
    }

    starTrack.setIdleLEDPattern(1.0, Blinky::CYCLE_RED_YELLOW);
    bool connected = starTrack.connectRUG3("/dev/ttyACM0");
    if (connected)
        starTrack.configureRUG3();
    starTrack.setIdleLEDPattern(1.0, Blinky::SOLID_BLUE);
    starTrack.update();

    bool done = false;
    bool was_pressed = false;
    std::chrono::steady_clock::time_point pressed_time;
    while (!done) {
        bool pressed = blinky.isButtonPressed();
        if (pressed != was_pressed) {
            // BUTTON has TRANSITIONED (between Pressed <=> Released)
            if (was_pressed && !pressed) {  // BUTTON was RELEASED
                spdlog::debug("Button PRESS detected.");
                if (std::chrono::steady_clock::now() - pressed_time > std::chrono::milliseconds(5000)) {
                    sync();
                    spdlog::info("Reset detected. Re-Initializing StarTrack");
                    starTrack.initialize();
                    pressed = was_pressed = false;
                    starTrack.setIdleLEDPattern(1.0, Blinky::BLINK_BLUE);
                } else {
                    if (starTrack.hasAllPoints()) {
                        starTrack.setActiveLEDPattern(0.5, Blinky::BLINK_WHITE);
                        spdlog::warn("Unable to record new measurement.  All measurement points have already been recorded.  Reset to start again.");
                    } else {
                        starTrack.setIdleLEDPattern(0.5, Blinky::SOLID_YELLOW);
                        // starTrack.setStationary();
                        while (starTrack.recordPoint() == false) {
                            spdlog::debug("Waiting for point to record (waiting on INS_STATUS_STATIONARY_MODE)...");
                            starTrack.update();
                            usleep(100000);
                        }
                        starTrack.setMoving();
                        sync();
                    }
                }
                // check if we're done....
                if (starTrack.hasAllPoints()) {
                    if (starTrack.validate_measurements())
                        starTrack.setIdleLEDPattern(0.25, Blinky::BLINK_GREEN);
                    else
                        starTrack.setIdleLEDPattern(0.25, Blinky::BLINK_RED);
                }
            }
            if (pressed && !was_pressed) { // BUTTON WAS PRESSED
                spdlog::debug("Button RELEASE detected.");
                pressed_time = std::chrono::steady_clock::now();
                starTrack.setStationary();
            }
            was_pressed = pressed;
        } else if (pressed) {
            // Button is being HELD
            // spdlog::debug("Button HOLD detected.");
            if (!starTrack.isLEDActive()) {
                spdlog::debug("LED is idle...");
                starTrack.setIdleLEDPattern(1.0, Blinky::SOLID_OFF);
                if (std::chrono::steady_clock::now() - pressed_time > std::chrono::milliseconds(6000)) {
                    starTrack.setActiveLEDPattern(0.2, Blinky::CYCLE_RED_WHITE, 1.0);
                } else if (std::chrono::steady_clock::now() - pressed_time > std::chrono::milliseconds(4000)) {
                    starTrack.setActiveLEDPattern(0.5, Blinky::BLINK_WHITE, 1.0);
                } else if (std::chrono::steady_clock::now() - pressed_time > std::chrono::milliseconds(2000)) {
                    starTrack.setActiveLEDPattern(1.0, Blinky::BLINK_WHITE, 1.0);
                } else {
                }
            }
            was_pressed = pressed;
        } else {
            //starTrack.setIdleLEDPattern(1.0, Blinky::SOLID_OFF);
            // this is our IDLE state (NO button held).
        }
        starTrack.update();
    }

    pigpio_stop(blinky.getPiGPIOInstance());
    return 0;
}