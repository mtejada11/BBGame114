# BBGame114

BBGame114 is handheld game device that consists of an Arduino Nano processor board, an 11x4 LED matrix display, three input buttons and a piezoelectric speaker. This device can be programmed using the Arduino IDE. This GitHub repository contains the BBGame114Lib library that can be used for programming this device, and it also contains a Space Invaders-like game.

![BBGame114 build diagram](https://github.com/mtejada11/BBGame114/raw/doc/Build/BBGame114.png)
*Diagram of BBGame114 assembled on 830-point solderless breadboard*

![BBGame114 circuit diagram](https://github.com/mtejada11/BBGame114/raw/doc/Build/BBGame114%20circuit%20diagram.png)
*Circuit diagram*

# BBGame114 Arduino Library

The [BBGame114Lib.zip](https://github.com/mtejada11/BBGame114/blob/master/Lib/BBGame114Lib.zip?raw=true) library for Arduino included in this repository is a library that allows the BBGame114 device to be programmed with higher level methods/functions than the Arduino `digitalWrite` and `digitalRead` functions. See the page [BBGame114Lib Library Reference](https://github.com/mtejada11/BBGame114/wiki/BBGame114Lib-Library-Reference) page for more information on this library. To install this library in the Arduino IDE, 

To install this library in the Arduino IDE:
1. Download [BBGame114Lib.zip](https://github.com/mtejada11/BBGame114/blob/master/Lib/BBGame114Lib.zip?raw=true) from this repository
2. In Arduino IDE, go to **Sketch**  >  **Include Library**  >  **Add .ZIP Library...**
3. Find downloaded file and press OK

# BBGame114 Coding Exercises

This repository contains several coding exercises. Exercises Ex01 to Ex03b are introductory Arduino exercises that do not required the LED matrix to be assembled. Exercises Ex04 to Ex07b are exercises than can be done with a partially assembled LED matrix of 5x4 pixels. The remaining exercises require the fully assembled BBGame114 circuit with and LED matrix of 11x4 pixels.

| Number | Description |
| --- | --- |
|Ex01  	|Blink |
|Ex02  	|Sequence |
|Ex02b 	|Sequence with Button |
|Ex03  	|Text Input Output |
|Ex03b 	|Text Input Output LED Control |
|Ex04  	|Matrix Test |
|Ex05  	|Matrix Test with Lib |
|Ex06  	|Numeric Counter |
|Ex07  	|Set Pixels |
|Ex07b 	|Set Pixels with Photoresistor |
|Ex08  	|Scrolling Text |
|Ex09  	|Thousand Counter |
|Ex09b 	|Thousand Counter with Button |
|Ex10a 	|Game Elements (Ship) |
|Ex10b 	|Game Elements (Laser) |
|Ex10c 	|Full Game |
|Ex10d 	|Full Game with Score |
|Ex11a 	|Light Meter |
|Ex11b 	|Light Meter Bar Graph |
