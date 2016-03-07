# hortus
hortus is an [Arduino](http://arduino.cc) project using a soil moisture sensor to detect when the plants of a garden require water.

### Schematic

Parts list:
- Arduino Microcontroller
- Piezo Buzzer
- Alphanumeric 7-Segment 4 Digit Display (TM1637)
- YL-69 Soil Moisture Sensor
- YL-38 Bridge

![](http://i.imgur.com/DbyK3cd.png)

- Display CLK and DIO pins go to pins 8 and 9 respectively
- Buzzer VCC pin goes to pin 4
- YL-38 Bridge VCC pin goes to pin 3
- YL-38 Bridge A0 pin goes to A5

### Compiling

Use PlatformIO to build this project.