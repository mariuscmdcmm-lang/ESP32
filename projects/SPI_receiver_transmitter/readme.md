# SPI Communication between two ESP32 boards

This repository contains two projects designed to implement SPI communication between two ESP32 development boards (Master and Slave).

## Note on project naming
Please note that the project folders are still named `uart_receiver` and `uart_transmitter` to maintain existing build configurations and save compilation time. Despite these names, **both projects have been fully reconfigured to use the SPI protocol.**

## SPI Connection Architecture
* **Master:** Controls the SPI bus and sends commands to the slave.
* **Slave:** Listens for incoming data via SPI and executes actions (e.g., LED control).

### Pin Configuration
| Signal | GPIO Pin |
| :--- | :--- |
| **MOSI** | 23 |
| **MISO** | 19 |
| **CLK** | 18 |
| **CS** | 15 |

## Requirements
* ESP-IDF framework.
* Two ESP32 boards.
* Physical connection (ensure MISO to MISO, MOSI to MOSI, etc.).

## Usage
1. Flash `uart_transmitter` onto the Master ESP32.
2. Flash `uart_receiver` onto the Slave ESP32.
3. The Master will automatically send commands to toggle the LED on the Slave.

---
*Developed as an educational project to study the SPI protocol.*