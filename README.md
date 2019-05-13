# BBGame114

BBGame114 is handheld game device that consists of an Arduino Nano processor board, an 11x4 LED matrix display, three input buttons and a piezoelectric speaker. This device can be programmed using the Arduino IDE. This GitHub repository contains the BBGame114Lib library that can be used for programming this device, and it also contains a Space Invaders-like game.

<img src="https://github.com/mtejada11/BBGame114/raw/doc/Build/BBGame114.png" width="800" alt="BBGame114 build diagram">
*Diagram of BBGame114 assembled on 830-point solderless breadboard*

<img src="https://github.com/mtejada11/BBGame114/raw/doc/Build/BBGame114%20circuit%20diagram.png" width="800" alt="BBGame114 circuit diagram">
*Circuit diagram*

# BBGame114 Arduino Library

The [BBGame114Lib.zip](https://github.com/mtejada11/BBGame114/blob/master/Lib/BBGame114Lib.zip?raw=true) library for Arduino included in this repository is a library that allows the BBGame114 device to be programmed with higher level methods/functions than the Arduino `digitalWrite` and `digitalRead` functions. See the page [BBGame114Lib Library Reference](https://github.com/mtejada11/BBGame114/wiki/BBGame114Lib-Library-Reference) page for more information on this library. 

# BBGame114 Coding Exercises

This repository contains several coding exercises. Exercises Ex01 to Ex03b are introductory Arduino exercises that do not required the LED matrix to be assembled. Exercises Ex04 to Ex07b are exercises than can be done with a partially assembled LED matrix of 5x4 pixels. The remaining exercises require the fully assembled BBGame114 circuit with and LED matrix of 11x4 pixels.

| Number | Description |
| --- | --- |
|Ex01  	|[Blink](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex01_Blink/Ex01_Blink.ino) |
|Ex02  	|[Sequence](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex02_Sequence/Ex02_Sequence.ino) |
|Ex02b 	|[Sequence with Button](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex02b_Sequence_with_Button/Ex02b_Sequence_with_Button.ino) |
|Ex03  	|[Text Input Output](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex03_Text_Input_Output/Ex03_Text_Input_Output.ino) |
|Ex03b 	|[Text Input Output LED Control](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex03b_Text_Input_Output_LED_Control/Ex03b_Text_Input_Output_LED_Control.ino) |
|Ex04  	|[Matrix Test](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex04_Matrix_Test/Ex04_Matrix_Test.ino) |
|Ex05  	|[Matrix Test with Lib](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex05_Matrix_Test_with_Lib/Ex05_Matrix_Test_with_Lib.ino) |
|Ex06  	|[Numeric Counter](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex06_Numeric_Counter/Ex06_Numeric_Counter.ino) |
|Ex07  	|[Set Pixels](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex07_Set_Pixels/Ex07_Set_Pixels.ino) |
|Ex07b 	|[Set Pixels with Photoresistor](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex07b_Set_Pixels_with_Photoresistor/Ex07b_Set_Pixels_with_Photoresistor.ino) |
|Ex08  	|[Scrolling Text](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex08_Scrolling_Text/Ex08_Scrolling_Text.ino) |
|Ex09  	|[Thousand Counter](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex09_Thousand_Counter/Ex09_Thousand_Counter.ino) |
|Ex09b 	|[Thousand Counter with Button](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex09b_Thousand_Counter_with_Button/Ex09b_Thousand_Counter_with_Button.ino) |
|Ex10a 	|[Game Elements (Ship)](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex10a_Game_Elements/Ex10a_Game_Elements.ino) |
|Ex10b 	|[Game Elements (Laser)](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex10b_Game_Elements/Ex10b_Game_Elements.ino) |
|Ex10c 	|[Full Space Invaders-like Game](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex10c_Full_Game/Ex10c_Full_Game.ino) |
|Ex10d 	|[Full Space Invaders-like Game with Score](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex10d_Full_Game_with_Score/Ex10d_Full_Game_with_Score.ino) |
|Ex10e 	|[Full Space Invaders-like Game with Custom Levels](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex10e_Full_Game_with_Custom_Levels/Ex10e_Full_Game_with_Custom_Levels.ino) |
|Ex11a 	|[Light Meter](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex11a_Light_Meter/Ex11a_Light_Meter.ino) |
|Ex11b 	|[Light Meter Bar Graph](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex11b_Light_Meter_Bar_Graph/Ex11b_Light_Meter_Bar_Graph.ino) |
|Ex12 	|[NeoPixels](https://github.com/mtejada11/BBGame114/blob/master/Exercises/Ex12b_NeoPixels42/Ex12b_NeoPixels42.ino) |
