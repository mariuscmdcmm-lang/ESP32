#Sleep Modes
This is a simple test about using `esp_sleep.h` library.

##About
I'm using a timer and an external gpio interrupttion to wakeup the esp32 and increment an integer.

##Used methods
1. esp_sleep_enable_timer_wakeup(uint64_t time_in_us)
2. esp_sleep_enable_ext0_wakeup(gpio_num, level)
3. esp_deep_sleep_start()

*Developed by Maryo* . 
