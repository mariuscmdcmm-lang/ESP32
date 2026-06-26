# ESP32 WiFi Server Template

A basic template for an ESP32 WiFi server, perfect for starting your own IoT projects.

## Compilation
Simply compile and flash this project onto your ESP32 board to get started.

## Peripherals
* **Potentiometer:** Connected to pin 34 (ensure it is powered by 3.3V).
* **LEDs:** * LED 1: Pin 2 (with a 330Ω resistor).
    * LED 2: Pin 22 (with a 330Ω resistor).

## Capabilities
* **Sensor Monitoring:** Displays the potentiometer value on the webpage in real-time.
* **Control:** * Toggle the LEDs via the buttons on the webpage.
    * Use a slider to send values (0-100) from the webpage to the ESP32 (visible in the serial monitor).
* **Real-time:** The webpage updates the values automatically without needing to refresh the page.
