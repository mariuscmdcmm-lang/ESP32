# ESP-NOW Project
In this project an ESP32 board sends data to another esp32 via ESP-NOW Protocol.

## How to use
1. Flash esp_now_b project first in your "slave" esp32, and search the MAC Address in the monitor.
2. **IMPORTANT**: Go to esp-now/esp_now_a/main/main.c file and edit the 14th line with your second esp32 board mac address, and save.
3. Flash esp_now_a in your "master" esp32 board . 
4. Connect both esp32 to your computer and see how esp32 "slave" prints in monitor a message received from the ESP32 A.
5. Feel free to edit the code acording to your requirements.

`AMPFUSION`
*Developed by Maryo*


