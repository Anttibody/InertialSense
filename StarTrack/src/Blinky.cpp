//
// Created by kylemallory on 3/3/23.
//

#include <Blinky.h>

const std::vector<int> Blinky::SOLID_OFF = { Blinky::Colors::OFF };

const std::vector<int> Blinky::SOLID_RED = { Blinky::Colors::RED };
const std::vector<int> Blinky::SOLID_YELLOW = { Blinky::Colors::YELLOW };
const std::vector<int> Blinky::SOLID_GREEN = { Blinky::Colors::GREEN };
const std::vector<int> Blinky::SOLID_CYAN = { Blinky::Colors::CYAN };
const std::vector<int> Blinky::SOLID_BLUE = { Blinky::Colors::BLUE };
const std::vector<int> Blinky::SOLID_MAGENTA = { Blinky::Colors::MAGENTA };
const std::vector<int> Blinky::SOLID_WHITE = { Blinky::Colors::WHITE };

const std::vector<int> Blinky::BLINK_RED = { Blinky::Colors::RED, Blinky::Colors::OFF };
const std::vector<int> Blinky::BLINK_YELLOW = { Blinky::Colors::YELLOW, Blinky::Colors::OFF };
const std::vector<int> Blinky::BLINK_GREEN = { Blinky::Colors::GREEN, Blinky::Colors::OFF };
const std::vector<int> Blinky::BLINK_CYAN = { Blinky::Colors::GREEN, Blinky::Colors::OFF };
const std::vector<int> Blinky::BLINK_BLUE = { Blinky::Colors::BLUE, Blinky::Colors::OFF };
const std::vector<int> Blinky::BLINK_MAGENTA = { Blinky::Colors::MAGENTA, Blinky::Colors::OFF };
const std::vector<int> Blinky::BLINK_WHITE = { Blinky::Colors::WHITE, Blinky::Colors::OFF };

const std::vector<int> Blinky::CYCLE_RED_WHITE = { Blinky::Colors::RED, Blinky::Colors::WHITE };
const std::vector<int> Blinky::CYCLE_RED_YELLOW = { Blinky::Colors::RED, Blinky::Colors::YELLOW };
const std::vector<int> Blinky::CYCLE_RGB = { Blinky::Colors::RED, Blinky::Colors::GREEN, Blinky::Colors::BLUE };
const std::vector<int> Blinky::CYCLE_RGB_OFF = { Blinky::Colors::RED, Blinky::Colors::GREEN, Blinky::Colors::BLUE, Blinky::Colors::OFF, Blinky::Colors::OFF };


void Blinky::initGPIO(std::string gpio_host) {
    // initialize GPIO
    if (pigpioStatus >= 0)
        pigpio_stop(pigpioStatus);

    pigpioStatus = pigpio_start(gpio_host.c_str(), NULL);

    if (pigpioStatus < 0) {
        spdlog::error("Unable to initialize piGPIO.  Is this application running on a PI, and with elevated privileges (sudo)?");
        return;
    }

    setLedGPIO(r_gpio, g_gpio, b_gpio);
    setButtonGPIO(btn_gpio, btn_active, debounce_ms);
}

void Blinky::setLedGPIO(int gpio_red, int gpio_green, int gpio_blue) {
    r_gpio = gpio_red;
    g_gpio = gpio_green;
    b_gpio = gpio_blue;

    // LED, RED Channel
    set_mode(pigpioStatus, r_gpio, PI_OUTPUT);
    set_pull_up_down(pigpioStatus, r_gpio, PI_PUD_DOWN); // off by default

    // LED, GREEN Channel
    set_mode(pigpioStatus, g_gpio, PI_OUTPUT);
    set_pull_up_down(pigpioStatus, g_gpio, PI_PUD_DOWN); // off by default

    // LED, BLUE Channel
    set_mode(pigpioStatus, b_gpio, PI_OUTPUT);
    set_pull_up_down(pigpioStatus,b_gpio, PI_PUD_DOWN); // off by default
};
void Blinky::setButtonGPIO(int gpio_btn, int active_state, int ms_debounce) {
    btn_gpio = gpio_btn;
    btn_active = active_state;
    debounce_ms = ms_debounce;

    // Button
    set_mode(pigpioStatus, btn_gpio, PI_INPUT);
    set_pull_up_down(pigpioStatus, btn_gpio, (btn_active == PI_HIGH) ? PI_PUD_DOWN : PI_PUD_UP);
};


void Blinky::setLED(int mask) {
    if (pigpioStatus < 0)
        return;

    static int last_mask = 0;

    if (mask != last_mask) {

        // toggle RED GPIO
        gpio_write(pigpioStatus, r_gpio, mask & 0x01);
        // toggle GREEN GPIO
        gpio_write(pigpioStatus, g_gpio, (mask >> 1) & 0x01);
        // toggle BLUE GPIO
        gpio_write(pigpioStatus, b_gpio, (mask >> 2) & 0x01);

        spdlog::trace("Period: {:0.3f}, Color: {:0d}, Mask: {:02x}", rates[rate_i], colors[color_i], mask);
        last_mask = mask;
    }
}

void Blinky::setPattern(const std::vector<double>& rates_, const std::vector<int>& colors_) {
    if (pigpioStatus < 0)
        initGPIO();

    rates = rates_;
    rate_i = 0;

    colors = colors_;
    color_i = 0;

    next_transition = std::chrono::steady_clock::now();
}

void Blinky::setPattern(double rate, const std::vector<int>& v) {
    rates = { rate };
    setPattern(rates, v);
}

/**
 * Blinks the LED in the specified color (a 3-bit mask), at the specified rate.
 * @param color
 * @param rate
 */
void Blinky::update() {
    if ((colors.size() > 0) && (std::chrono::steady_clock::now() >= next_transition)) {
        // first let's push out a current color to the LED and then move to our next
        color_i = (color_i + 1) % colors.size();

        // now let's set the duration until the next transition
        rate_i = (rate_i + 1) % rates.size();
        next_transition = std::chrono::steady_clock::now() + std::chrono::milliseconds((int)(rates[rate_i] * 1000 / colors.size()));
    }
    setLED(colors[color_i]);
}

void Blinky::basicBlink(double rate, char on, char off) {
    rates = { rate };
    colors = { on, off };
    setPattern(rates, colors);
}

bool Blinky::isButtonPressed() {
    static auto last_transition = std::chrono::steady_clock::now();
    static bool last_state = !btn_active;

    auto now = std::chrono::steady_clock::now();
    bool state = gpio_read(pigpioStatus, btn_gpio);

    if (state != last_state) {
        if (now - last_transition > std::chrono::milliseconds(debounce_ms)) {
            // we've crossed the threshold, so we make the last_state our active state
            last_state = state;
            // spdlog::info("Button change detected: {}", last_state == BUTTON_ACTIVE ? "ON" : "OFF");
        }
    } else {
        last_transition = now;
    }

    return last_state == btn_active;
}


