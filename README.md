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
 * /
