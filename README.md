TeensyCNC, CNC controller using a Teensy 3.x!

Currently only 2 axis, but easily expanded.
Made to hack my Wife's Cricut Mini, because Provo Craft sucks and wants you to pay them to be creative (and now that they've discontinued it and basically bricked it!)
I want to cut/draw what I want, not something that's in THEIR clipart library AND have to pay for it as well!!!

Besides that, it's also a fun project! :D

Connection information:

<b>Original PIC24FJ64GB002 (Master PIC - handles USB interface, command queing/excution, motion control):</b>

Description | Pin | Pin | Description
----------- | --- | --- | -----------
--- | 1 | 28 | ---
--- | 2 | 27 | Ground
--- | 3 | 26 | motorYb
--- | 4 | 25 | motorXb
--- | 5 | 24 | motorXa
motorYa |  6 | 23 | ---
--- | 7 | 22 | USB Data -
Ground | 8 | 21 | USB Data +
--- | 9 | 20 | ---
Power button | 10 | 19 | ---
Head solenoid | 11 | 18 | ---
Load button | 12 | 17 | ---
--- | 13 | 16 | ---
--- | 14 | 15 | ---

<b>Original PIC16LF1823 (Slave PIC -  handles counting quadratic motor encoder pulses, passes the data to master PIC via SPI):</b>
Note: this is a 14 pin chip, and the board has a 20 pin location.

Description | Pin | Pin | Description
----------- | --- | --- | -----------
--- | 1 | 14 | Ground
encoderXb | 2 | 13 | ---
encoderXa | 3 | 12 | ---
--- | 4 | 11 | encoderYa
--- | 5 | 10 | ---
--- | 6 | 9 | ---
LED enable | 7 | 8 | encoderYb

<b>Connections to the Teensy:</b>

Connection at Cricut | Connection at Teensy
-------------------- | --------------------
motorXa | Teensy pin 21 (PTD6, PWM output)
motorXb | Teensy pin 20 (PTD5, PWM output)
motorYa | Teensy pin 23 (PTC2, PWM output)
motorYb | Teensy pin 22 (PTC1, PWM output)
encoderYa | Teensy pin 17 (PTB1, interrupt input)
encoderYb | Teensy pin 16 (PTB0, interrupt input)
encoderXa | Teensy pin 11 (PTC6, interrupt input)
encoderXb | Teensy pin 12 (PTC7, interrupt input)
Load button | Teensy pin 14 (PTD1, interrupt input)
Head solenoid | Teensy pin 13 (PTC5, Output)
LED Enable | Teensy +3.3v
Ground | Teensy Ground (may be grounded via USB B connector)

<b>Note:</b>
Power button is currently unused and unconnected. Future use?

For USB connection, I used a chopped off USB MicroB cable and soldered the wires to the pins on the bottom of the board.

Motor connection (from left to right, looking at the connector - not needed, just for reference):

Pin | Description
--- | -----------
1 | motor -
2 | motor +
3 | sensor com
4 | A
5 | LED +
6 | B
