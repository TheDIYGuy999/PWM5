# This is an Arduino "PWM5" Solar Charge Controller

It was originally invented by Julian Ilett: https://www.youtube.com/watch?v=AobcNhLG_Xw&list=PLjzGSu1yGFjWa5BEHgX5UUrSexppett3W

This Arduino version uses a similar layout as arduined.eu: https://www.arduined.eu/arduino-solar-charge-controller/

After I designed and built a couple of own MPPT solar chargers (see my other Repositories here), I wanted to try this "classic" PWM design.
It works fine, but of course you can't use it for nominal voltages below 12V (or at least it would be very inefficient in this case). Check out the included PCB files, if you want to build your own PWM5 controller.

## Features / Improvements:
- Programmable with Arduino IDE
- Input voltage 12 - 22V (36 cell solar panel)
- Output voltage 10.8 - 14.4V (6 cell 12V lead acid battery)
- Suitable for solar palels up to 100W (according to Julian Ilett)
- Using the 3.3V, 8MHz Arduino Pro Mini, which requires about 5mA (instead of 20mA for the 5V, 16MHz version)
- IRLZ44N logic level MOSFET used, because the charge pump can't deliver enough voltage with 3.3V supply
- Working frequencies: 15kHz (charge pump), 2kHz (PWM)
- Structurized the software, added serial output, improved the LED to be similar with Julians design
- Readings and calculations are based on vcc, which is read automatically
- Much simpler differential control algorithm
- Schematic better structurized, easier to understand
- Minor component value changes
- Eagle schematic and board file available
- Gerber files available (allows you to order your own boards easily)
- This controller is COMMON NEGATIVE
- An anti backfeed diode on the solar panel side is required

New in V 1.0:
- Initial commit, tested with my 10W and 20W solar panels, charging my 12V, 6.6Ah Lead Acid Gel battery

## Usage

See pictures
![](https://github.com/TheDIYGuy999/PWM5/blob/master/1.jpg)
![](https://github.com/TheDIYGuy999/PWM5/blob/master/2.jpg)
![](https://github.com/TheDIYGuy999/PWM5/blob/master/3.jpg)
![](https://github.com/TheDIYGuy999/PWM5/blob/master/Top.png)
![](https://github.com/TheDIYGuy999/PWM5/blob/master/Bottom.png)
![](https://github.com/TheDIYGuy999/PWM5/blob/master/Schematic.png)

Also have a look at the pdf schematic.

(c) 2018 TheDIYGuy999
