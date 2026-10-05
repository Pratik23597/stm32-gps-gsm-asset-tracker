# GPS/GSM Asset Tracking and Safety System on STM32F407

C-DAC PG Diploma in Embedded Systems Design - project (2022), Pratik Sukare

A tracking unit built on the **STM32F407 Discovery** board that determines its position with a **NEO-6M GPS** module and talks to its owner over SMS and voice calls through a **Quectel M66 GSM** modem. An **ESP8266 (NodeMCU)** forwards location data to a web server via MQTT. The firmware is written in C on the STM32 HAL (STM32CubeIDE).

## Operating modes

The owner selects a mode by SMS. Commands are only accepted from an authorised phone number.

**Vehicle mode (`CAR_Mode`)**
- `Location@` - replies with the current position as a Google Maps link
- `CAR_ON` - switches the ignition relay remotely
- Ignition-tamper input - sends a tamper alert SMS when the ignition line becomes active
- `SAFETY_OFF` - leaves vehicle mode

**Personal-safety mode (`PC_Mode`, parent-child)**
- SOS push button - sends an emergency SMS with a Google Maps link to four contacts and places a voice call to the first contact
- `SAFETY_OFF` - leaves the mode

## How it works

- **GPS:** NEO-6M on a UART, received with **DMA** (started from a push-button interrupt); the `$GPRMC` sentence is parsed for latitude/longitude, converted to decimal degrees and formatted as a Google Maps link
- **GSM:** Quectel M66 driven with AT commands over UART - text mode (`AT+CMGF`), read (`AT+CMGR`), send (`AT+CMGS`), delete (`AT+CMGDA`), unsolicited-result control (`AT+CNMI`) and voice call (`ATD`)
- **Cloud:** location messages are sent over a third UART to an ESP8266, which publishes them via MQTT to a web server / database
- **Debug:** modem replies and parsed data are mirrored to a debug UART; on-board LEDs indicate state

## Hardware

STM32F407 Discovery, NEO-6M GPS with antenna, Quectel M66 GSM module, ESP8266 NodeMCU, relay module, push buttons, breadboard and wiring.

## Repository layout

| Path | Contents |
|---|---|
| `firmware/Project.ioc` | STM32CubeMX pin and peripheral configuration |
| `firmware/Core/Src/main.c` | Application code (GPS parsing, GSM command handling, modes) |
| `firmware/Core/` | CubeMX-generated init code, interrupt handlers, startup |
| `firmware/Drivers/` | ST HAL and CMSIS drivers |

## Build

Open the `firmware` folder as a project in **STM32CubeIDE** and build for STM32F407VGTx. Before flashing, replace the placeholder phone numbers (`XXXXXXXXX1`...`4`) in `main.c` with real numbers.

## Possible extensions

Vehicle speed tracking, a more compact MCU/PCB, accident detection with a vibration sensor, and pattern-based alerts from location history.

## License

MIT - see [LICENSE](LICENSE). Third-party code (e.g. ST HAL/CMSIS drivers, libraries) keeps its own license.

## Author

Pratik Sukare - [LinkedIn](https://www.linkedin.com/in/pratiksukare) | pratiksukare235@gmail.com
