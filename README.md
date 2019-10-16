# room-monitor-arduino

Proximity sensor for an [Ardunio Uno Wifi Rev 2](https://store.arduino.cc/usa/arduino-uno-wifi-rev2) to check to see if a person is present within a designated area. This can be useful if you want to check if someone is at their desk, in the bathroom, doing laundry, or in any room of your choosing.

## Getting Started

Follow these steps to get this software running on your Arduino.

### Prerequisites

- [Arduino Uno Wifi Rev 2](https://store.arduino.cc/usa/arduino-uno-wifi-rev2)
- [Infared Proximity Sensor](https://www.sparkfun.com/products/8958)
- [JST to Breadboard Jumper](https://www.sparkfun.com/products/13685)
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)

### Dependencies

- [ArduinoHttpClient](https://github.com/arduino-libraries/ArduinoHttpClient)
- [ArduinoJson](https://arduinojson.org/?utm_source=meta&utm_medium=library.properties)
- [SharpIR](https://github.com/qub1750ul/Arduino_SharpIR)
- [WifiNINA](https://www.arduino.cc/en/Reference/WiFiNINA)

### Installing

Download repository.

```bash
git clone git@github.com:BruceBC/room-monitor-arduino.git
cd ~/room-monitor-arduino/proximitySensor && touch arduino_secrets.h
cd ~/room-monitor-arduino/proximitySensorSimulator && touch arduino_secrets.h
```

Open each of the `arduino_secrets.h` files and add the following:

```C
#define SECRET_SSID "" // Wifi Router Name
#define SECRET_PASS "" // Wifi Password
#define SERVER_ADDRESS "" // http://<subdomain>.ngrok.io or http://localhost
#define PORT 80 // Port (80 by default)
#define HARDWARE_ID "" // Unique id to allow app to link to hardware data (guid recommended)
#define PATH "/hardware" // Url path (/hardware by default)
```

Make sure to fill out variables appropriately.

### Usage

Use with an infared proximity sensor by navigating to the proximitySensor folder and open proximitySensor.ino in the Arduino IDE.

```bash
cd ~/room-monitor-arduino/proximitySensor
```

Simulate an infared proximity sensor by navigating to the proximitySensorSimulator folder and open proximitySensorSimulator.ino in the Arduino IDE.

```bash
cd ~/room-monitor-arduino/proximitySensorSimulator
```

## Server

Needs to be running in the background.

[Websocket Server](https://github.com/BruceBC/room-socket-elixir)

## Apps

Companion app to visualize the data of whether someone is present within a room.

[iOS](https://github.com/BruceBC/room-monitor-ios)
