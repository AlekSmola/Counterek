# Counterek
## Arduino based low-powered counter device with display.

## Intro
Device meant to count anything and display the numebr on the display. In this case used to count number of repetitions of certain physical activities in the gym. 

## Technical Stuff
Based on Atmega168P 5V@16MHz, since thats what I had lying around and project is not too complicated to justify usage of something more powerful like ESP32 or any STM32 IC.
Diplay is 4 digit TM1637 Display. 

The initial assumptions of counting seems to be obvious and easy to implment. However, the details _Counts_. Device is powered by a battery since the usage of a wall adapter is not always possible and for such a low consumption device, not neccesary. A lot of attention went to making the devices as efficient in terms of cconsumped power. Its default state is the sleep state during which it cosnumed as stated on the Fig below. During operation, which in baset case sceneario for my usage is 1hr a day at most it consumes more asd on the Fig2. 
