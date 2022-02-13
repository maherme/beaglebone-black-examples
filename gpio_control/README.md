# GPIO control examples

Here you can find some examples about configuring and controlling the GPIO of the BeagleBone Black:

- [led_user_control.c](led_user_control.c): in this file you can control the four user led of the BBB. You can compile this application using ```make led```.
- [counter_7seg.c](counter_7seg.c): in this file you can control a seven segment display. You can compile this application using ```make 7seg```.

  The connection between BBB and the seven segment display is as follow:
  | BeagleBone Black | 7 Display             |
  |:----------------:|:---------------------:|
  | P8-7 (GPIO 66)   | Segment A (pin 7)     |
  | P8-8 (GPIO 67)   | Segment B (pin 6)     |
  | P8-9 (GPIO 69)   | Segment C (pin 4)     |
  | P8-10 (GPIO 68)  | Decimal Point (pin 5) |
  | P8-11 (GPIO 45)  | Segment D (pin 2)     |
  | P8-12 (GPIO 44)  | Segment E (pin 1)     |
  | P8-14 (GPIO 26)  | Segment F (pin 9)     |
  | P8-16 (GPIO 46)  | Segment G (pin 10)    |
  
- [button_7seg.c](button_7seg.c): in this file you control a seven segment display using an external button. You can compile this application using ```make button7seg```.

  For connecting the button you have to use P9-23 (GPIO 49) pin and a 3.3V pin.
  
- [counter_4dig7seg.c](counter_4dig7seg.c): in this file you control a four seven segment display. You can compile this application using ```make 4dig7seg```.

  The connection between BBB and the four seven segment display is as follow:
  | BeagleBone Black | 4 Digit 7 Display     |
  |:----------------:|:---------------------:|
  | P8-7 (GPIO 66)   | Segment A (pin 11)    |
  | P8-8 (GPIO 67)   | Segment B (pin 7)     |
  | P8-9 (GPIO 69)   | Segment C (pin 4)     |
  | P8-10 (GPIO 68)  | Decimal Point (pin 3) |
  | P8-11 (GPIO 45)  | Segment D (pin 2)     |
  | P8-12 (GPIO 44)  | Segment E (pin 1)     |
  | P8-14 (GPIO 26)  | Segment F (pin 10)    |
  | P8-16 (GPIO 46)  | Segment G (pin 5)     |
  | P9-15 (GPIO 48)  | Digit 1 (pin 12)      |
  | P9-23 (GPIO 49)  | Digit 2 (pin 9)       |
  | P9-30 (GPIO 112) | Digit 3 (pin 8)       |
  | P9-27 (GPIO 115) | Digit 4 (pin 6)       |

- [print_lcd.c](print_lcd.c): in this file you control a 2x16 LCD (HD44780). You can compile this application using ```make lcd```.

  The connection between BBB and the LCD is as follow:
  | BeagleBone Black | 2x16 LCD (HD44780)    |
  |:----------------:|:---------------------:|
  | P8-7 (GPIO 66)   | RS (pin 4)            |
  | P8-8 (GPIO 67)   | RW (pin 5)            |
  | P8-9 (GPIO 69)   | Enable (pin 6)        |
  | P8-10 (GPIO 68)  | Data 4 (pin 11)       |
  | P8-11 (GPIO 45)  | Data 5 (pin 12)       |
  | P8-12 (GPIO 44)  | Data 6 (pin 13)       |
  | P8-14 (GPIO 26)  | Data 7 (pin 14)       |
