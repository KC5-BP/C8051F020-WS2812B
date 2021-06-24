# Ws2812b monitoring + Issues
Examples of Headers and C files for monitoring Ws2812b neopixels with a Silabs C8051F020.

![Kaiminus (french name) : Pic made the 8th Mai 2018](https://pbs.twimg.com/media/DcrguXRWsAAZQeq?format=jpg&name=large)


## Actual Issues :
1. I try to optimize as much as I can ! *"CAN"* because Keil uVision is a good IDE, but the 
free version is very limited to a certain point. \
For example :
    * I want to improve the reading of a struct initialisation such as :
      * **xdata offsetText Offset = {0, 0};** \
  And I want to make it clearer by writing :
      * xdata offsetText Offset = (offsetText) {**.column = 0**, **.line = 0**}; \
  But Keil takes the dot before one struct member as an error syntax. 
  So lots of little details like this blocks me to improve my code.
1. led_Show() : Error when using it in matrix_Print().
1. getPixelStatusFromChar() : Can't implement my function without getting 
   error about space overflow.

## Formats
- 00-Archives ;
    - Represents the first version/first throw of the process. With usage either with a
      leds strip or a matrix. They work, but they are not as completed as files 
      in refFiles folder.
- 00-Pics ;
    - For the moment, it only contains differences between structure format. In a future, 
      I will add some pics that I'd take from resulting display, like a showcase 
      (as show for example, with upper picture).
- refFiles ;
    - Separated into led_manip and mx_manip sub-folders (mx_manip using led_manip).\
      At first, mx_ and led_ were fused (as said before) because it was the beginning.\
      Then, I made a massive clean up and split this file into one 
      containing the LEDs control and the other will use the led monitoring, 
      but with a specific arrangement.
