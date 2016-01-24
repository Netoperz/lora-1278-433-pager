# lora-1278-433-pager
An Arduino based pager with Nokia 5110 Display, receiving data via DRF1278DM

I used an Olimexino Nano, as it already operates at 3.3v, so no levelshifter is needed for the display and it has an SD-Card slot "on board". As it is based on a Leonardo it also has a full 2nd Serial connection, so software serial is not needed.

Button one is used to switch backlight on and off, and reset/delete all messages after a long press
Button two is used to see new Messages and iterate through new ones

Connections:
  A beeper to GND and Pin 6
  A Button to GND and Pin 15
  A Button to GND and Pin 17
  Nokia 5110 LCD to:
    CE  on Pin 10
    RST on Pin 9
    DC  on Pin 11
    CLK on Pin 13
    DIN on Pin 12
    BL  on Pin 8
    VCC to 3.3V
    GND to GND
  
  DRF1278DM LORAmote to:
    GND on GND
    VCC on 5V
    RXD on Pin 1
    TXD on PIN 0
    SET on Pin 2 (unused right now, should be used as wake up interrupt)
  
