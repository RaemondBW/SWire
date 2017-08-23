# SWire
The goal of SWire is to be a drag and drop Software I2C replacement for the Arduino Wire Library.


## Why do I need this?
* You need a software implementation of I2C because, your hardware requires two I2C busses and you only have one.
* For some reason you have decided to use a alternate pins from the hardware I2C Pins.


## Why this library?
* I got frustrated by the lack of support and debugability of the other Assembly written Software I2C Libraries available on Github.
* [XantolI2C](https://github.com/xantorohara/XantoI2C), on which this library was built, frustratingly didn't implement the Wire.h interface so was not an easy drag and drop replacement.


## Usage
1. Clone or Download (from [here](https://github.com/RaemondBW/SWire/archive/master.zip)) this Repository from github to your Arduino/libraries folder
2. Include it in the source of your Arduino project,
```#include <SWire.h>```
3. Replace all instances of Wire from your project with SWire calls.
4. Replace your Wire.begin() with SWire.begin(<SDA Pin>, <SCL Pin>)
5. If you are still confused, check out the example project.
