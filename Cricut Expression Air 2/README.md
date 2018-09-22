Some pictures from inside a Cricut Expression Air 2.

I didn't want to completely disassemble it, so the circuit board view isn't very good...

You can make out a 64 pin TQFP chip, house marked "FJ3091 1650C6V", most likely a PIC24FJ128GB106 or similar.
I'm going to assume this is the master MCU, as there is also another 14 pin SOIC chip (likely a PIC16LF1823) in the back with a PIC programming header next to it, much like the Cricut Mini.

The Bluetooth UART module is also made by Microchip, so there's a running theme here.

X/Y axis drive motors are nearly the same as the Mini, not sure on the pen/cutter heads... I'm assuming some kind of servo, it's not a solenoid though.

Shouldn't be an issue adapting TeensyCNC to this model as well, only catch is going to be the head up/down mech.
