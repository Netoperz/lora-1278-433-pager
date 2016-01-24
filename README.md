# lora-1278-433-pager
An Arduino based pager with Nokia 5110 Display, receiving data via DRF1278DM

I used an Olimexino Nano, as it already operates at 3.3v, so no levelshifter is needed for the display and it has an SD-Card slot "on board". As it is based on a Leonardo it also has a full 2nd Serial connection, so software serial is not needed.

Button one is used to switch backlight on and off, and reset/delete all messages after a long press
Button two is used to see new Messages and iterate through new ones

Connections:

  Dev | Pins
  ----------|--------------
  A beeper | GND and Pin 6
  A Button | GND and Pin 15
  A Button | GND and Pin 17

  Nokia 5110 LCD | to:
  ---------|-----------
    CE  | Pin 10
    RST | Pin 9
    DC  | Pin 11
    CLK | Pin 13
    DIN | Pin 12
    BL  | Pin 8
    VCC | 3.3V
    GND | GND
  
  DRF1278DM LORAmote | to:
  -------------|------------
    GND | GND
    VCC | 5V
    RXD | Pin 1
    TXD | PIN 0
    SET | Pin 2 (unused right now, should be used to trigger wake up interrupt)
  
