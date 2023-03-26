# Counterek
## Arduino based low-powered counter device with a display.
![Fig0 Showcase](showcase.jpg)
## Intro
Device meant to count anything and display the number on the display. In this case used to count number of repetitions of certain physical activities during the workout. 

## Technical Stuff
Based on Atmega168P 5V@16MHz, since that’s what I had lying around and the project is not too complicated to justify usage of something more powerful like ESP32 or any STM32 IC.
Display is 4 digit TM1637 Display. 

The initial assumptions of counting seems to be obvious and easy to implement. However, the details _Counts_. Device is powered by a battery since the usage of a wall adapter is not always possible and for such a low consumption device, not necessary. A lot of attention went to making the devices as efficient as possible in terms of consumed power. Its default state is the sleep state during which it consumed as stated on the Fig1 below. 0.09mA is enough to keep this device in use for years. Battery consideration below. 
![Fig1 Consumption during sleep](sleep.jpg)
During operation, which in my best case scenario is 1hr a day (at most) it consumes more as on the Fig2. 
![Fig2 Normal usage](normal.jpg)
This current is much higher. This comes mostly from display which lights up only required digits (without 0s before number) but is quite a hungry device for battery powered applications. Fortunately after 30s of inactivity it is turned off and Arduino goes to sleep after that. 

# Interrupts
To wake up from sleep interrupts are used. Hardware interrupts connected on pins 2 and 3 which are two pins which can be used for this purposes. one of them goes to the reset button which simply resets counter to 0. It also adds this number to previously saved number in EEPROM - just out of curiosity. Second pin is connected to the simple plate which is triggered by moving magnet. This is actually part of counting. This solution is the simplest and most effective. More sophisticated solutions were considered. Diode with photosensor taken from old printer would be a great option, but LED in such case require a constant supply of power to provide interrupts - this would decrease battery operation time. Distance sensor was also considered since in my case the distance to moving object could trigger the counter. However, its software implementation was not that easy feature a lot of measurement noise. Simplest solution is the bestest solution.
![Fig3 Inside of device](inside.jpg)

# Used battery and operation time on one charge. 
Battery used is a Li-Ion cell found in other equipment which was not working anymore. Considering its size it can be said it is a 300 - 400 mAh in capacity cell. It is charged by common TP4056 IC equipped with undervoltage protection. 
Atmega168 is powered directly from it. To save power, the linear regulator and red power LED were removed from Arduino Pro Mini development board. This is possible since the Arduino can handle the raw battery voltage which goes from 3 to 4.2V. Only one issue was discovered: when battery is near 3V the Atmega168 is already not functioning but the display still works, which makes an impressions of halted operation. Where in reality, the controller is not working anymore and display just displays last demanded number. 
Considering the time of operation device should work in sleep mode in one charge about: 138 days, assuming worst case scenario for battery capacity (300mAh). Up to this point the real life test are being conducted. 

With ease classical 18650 could be used. Even cheaper option or recycled cell will feature a 2Ah of capacity and work for years. It is a good idea but with such, there would not be any reason to update the firmware and introduce new features ;).

## Sources
1) https://www.youtube.com/watch?v=urLSDi7SD8M
2) https://www.microchip.com/en-us/product/ATmega168

