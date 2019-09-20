# T60SVP

This is a PIC24-based application capable of reading the Supervisor password from an IBM ThinkPad T60 laptop. It was written specifically to run on an Explorer 16/32 development kit with the 24FJ1024GB610 PIM, but the code is portable to pretty much any platform.

The T60, and other ThinkPad laptops store the supervisor password on a AT24RF08 1KB EPPROM. The T60, specifically, uses an Atmel AT8356908 which behaves exactly like the AT24RF08.

Access to the EEPROM is through the I2C bus which is accessible by soldering two wires onto the board at specific vias.

Here are the VIAs in question (located on the motherboard, on the left bottom side of the keyboard):

![SDA/SCL](https://wallyoz.smugmug.com/photos/1171479448_Upm7U-L.jpg)

# Usage

1. Connect wires from the motherboard to SDA1/SCL1 on the Explorer 16/32 (P56/P57)

1. Connect a ground on the Explorer 16/32 to the silver GPU/i945 heatsink cover, which is grounded

1. Connect the Serial terminal on the Explorer 16/32 to your computer via a Micro-B USB cable

1. Power on the computer and hit F1 to try to enter the BIOS

1. Once you're sitting at the supervisor password screen, go ahead and open a serial console to the Explorer board at 115200 baud

1. Run "scan" to ensure that you're getting responses from the I2C slaves

1. Run "password" to dump the password from the ROM. The password is stored in scancode, but the utility automatically converts it to ASCII

If all goes well, your should see something like this (forgive the picture of a monitor):

![Output](https://i.imgur.com/mKfmTn1.jpg)
