# Soarer PS/2 to USB keyboard converter
https://deskthority.net/workshop-f7/xt-at-ps2-terminal-to-usb-converter-with-nkro-t2510.html

You need
- ATMega32U4 Microcontroller/Arduino. E.g. Arduino Pro Micro
- PS/2 female connector

### Hardware
PS/2 -> Arduino
- Data (green) -> PD0 (Pro Micro: 3)
- CLK (white) -> PD1 (Pro Micro: 2)
- 5V (red) -> 5V
- GND (black) -> GND

#### Arduino Pro Micro pinout
http://www.pighixxx.com/test/wp-content/uploads/2016/07/pro_micro_pinout_v1_0_red.png

### Firmware
https://deskthority.net/workshop-f7/xt-at-ps2-terminal-to-usb-converter-with-nkro-t2510.html

Soarer_Converter_v1.12_update.zip: https://deskthority.net/resources/file/8295

#### Flashing firmware
https://sourceforge.net/projects/winavr/
```
avrdude -p m32u4 -b 57600 -P com5 -c avr109 -U flash:w:Soarer_at2usb_v1.12_atmega32u4.hex
```
Bootloader mode: RST to GND two times. You have couple of seconds to start flashing. Serial port can be different than in normal mode.


### Testing/Debugging
hid_listen.exe: https://www.pjrc.com/teensy/hid_listen.html

### Settings/Tools/Docs
https://deskthority.net/workshop-f7/xt-at-ps2-terminal-to-usb-converter-with-nkro-t2510.html

Soarer_Converter_v1.10.zip: https://deskthority.net/resources/file/6142

## Links
- https://geekhack.org/index.php?topic=17458.0
- http://www.computer-engineering.org/ps2protocol/
- http://www.computer-engineering.org/ps2keyboard/scancodes2.html

## Keyboard controller
- https://geekhack.org/index.php?topic=50437.0
- https://deskthority.net/workshop-f7/soarer-s-keyboard-controller-firmware-t6767.html
Soarer_Controller_v1.20_beta4.zip

