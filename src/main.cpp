/* Demonstration of the ESP32 waking up from a deep sleep
 * This is based on:  https://lastminuteengineers.com/esp32-deep-sleep-wakeup-sources/
 * I modified to use a GPIO on a ESP32-S3 Dev kit 1
 * I have the NeoPixel flash White on each wakeup
 * I code the wakeup condition to different colors on the NeoPixel
 * because the Windows driver plays around with serial control pins when it comes up.
 * This causes the board to reboot.
 * https://docs.espressif.com/projects/esptool/en/latest/esp32/advanced-topics/boot-mode-selection.html
 * 
 * I tested Wakeup on Timer and Wakeup EXT0 which is one GIO pin.
 * I pulled the pin to ground by a resistor and used a pushbutton to 3.3V to
 * trigger the wakeup.
*/

#include <Arduino.h>
#include "NeoPixelScale.h"

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex

#define uS_TO_S_FACTOR 1000000ULL // Micro seconds to seconds
#define TIME_TO_SLEEP_SEC 5       // Sleep time (in seconds)

RTC_DATA_ATTR volatile int bootCount = 0;  // volatile tells compiler to read the value each time from memory

// Looks up the reason that the ESP32 was awoken and outputs it
// on both serial and by a color on the NeoPixel
// On Windows, I had the CPU reboot when it reconnects with the serial on USB.
// therefore it always reported Wakeup Not Caused by Sleep and boot count of zero.
// If I just connect with power, not USB, the color code works to report the correct boot type
// One explanation:  https://docs.espressif.com/projects/esptool/en/latest/esp32/advanced-topics/boot-mode-selection.html
void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    pinMode(LED_BUILTIN, OUTPUT);
    NeoPixel::DisplayColor(LED_BUILTIN, NeoPixel::White);
    delay(500);

    Serial.println("***** wakeup reason  " + String(wakeup_reason));
    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        NeoPixel::DisplayColor(LED_BUILTIN, NeoPixel::Green);
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        NeoPixel::DisplayColor(LED_BUILTIN, NeoPixel::Orange);
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        NeoPixel::DisplayColor(LED_BUILTIN, NeoPixel::Blue);
        Serial.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        NeoPixel::DisplayColor(LED_BUILTIN, NeoPixel::Violet);
        Serial.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        NeoPixel::DisplayColor(LED_BUILTIN, NeoPixel::Cyan);
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        NeoPixel::DisplayColor(LED_BUILTIN, NeoPixel::Red);
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
    delay(500);
    NeoPixel::DisplayColor(LED_BUILTIN, NeoPixel::Black);
}

void setup()
{
    Serial.begin(115200);
    delay(1000); // Take some time to open up the Serial Monitor

    // Increment boot number and print it every reboot
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    // Print the wakeup reason for ESP32
    print_wakeup_reason();

    /*
    First we configure the wake up source
    We set our ESP32 to wake up for an external trigger.

    There are two types for ESP32, ext0 and ext1 .
    ext0 uses RTC_IO to wakeup thus requires RTC peripherals
    to be on while ext1 uses RTC Controller so doesn't need
    peripherals to be powered on.
    Note that using internal pullups/pulldowns also requires
    RTC peripherals to be turned on.
    */
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_1, 1); // 1 = High, 0 = Low

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_SEC * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP_SEC) + " Seconds");

    // If you were to use ext1, you would use it like
    // esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void loop()
{
    // This is not going to be called
}
