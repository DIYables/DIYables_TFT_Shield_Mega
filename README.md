## TFT LCD Display Library for Arduino Mega — DIYables_TFT_Shield_Mega (16-bit Parallel)
This Arduino library provides an easy-to-use interface for the 3.5-inch TFT LCD Display Shield with **16-bit parallel interface** for Arduino Mega form factor boards.
It enables fast and reliable graphics, text, and image rendering on your TFT shield.
* Fully extends the Adafruit GFX library, giving you access to all standard graphics and text functions.
* Adds optimized 16-bit parallel support for drawing shapes, displaying images, and rendering text.
* Compatible with Arduino Mega 2560, Arduino Due, Arduino Giga R1 WiFi, and other Mega form factor boards.
* 16-bit parallel bus: each pixel drawn in a single write cycle — up to 2x faster than 8-bit shields.

With this library, you can easily create colorful, high-resolution graphical interfaces for your Arduino projects using a 3.5-inch ILI9486 TFT LCD Display Shield (16-bit parallel, Mega form factor).



Product Link
----------------------------
* [DIYables TFT Shield For Arduino Mega](http://diyables.io/tft-shield-mega)

![DIYables TFT Shield For Arduino Mega](https://diyables.io/images/products/3.5-inch-tft-lcd-color-display-for-arduino-mega-mega-1.jpg)

Features  
----------------------------  
* 16-bit parallel data bus for maximum throughput
* Direct register access on Mega 2560, Due, and Giga R1 WiFi for fastest possible speed
* Works with all Arduino Mega form factor boards (Mega, Due, Giga, etc.)



Available Examples
----------------------------
* **DrawImage.ino**: Demonstrates how to display a bitmap image stored in program memory (flash) directly on the TFT screen.
* **DrawImageSDcard.ino**: Shows how to load and display an image from an SD card onto the TFT display.
* **DrawShapes.ino**: Illustrates drawing various geometric shapes (such as circles, rectangles, triangles, and more) on the TFT screen using the library’s graphics functions.
* **ShowTextAndNumber.ino**: Demonstrates how to display text strings and numbers in different fonts, sizes, and colors on the TFT display.
* **UseExternalFont.ino**: Shows how to use and display external (custom) fonts on the TFT screen for enhanced text appearance.



Tutorials
----------------------------

* [Arduino Mega - TFT LCD Display Shield](https://newbiely.com/tutorials/arduino-mega/arduino-mega-tft-lcd-display-shield)



16-bit Parallel Pin Mapping
----------------------------

| Shield Pin | Function   | Arduino Pin | Mega 2560 Port | Due Port | Giga Port |
|------------|------------|:-----------:|----------------|----------|-----------|
| DB0        | Data bit 0 | 37          | PC0            | PC5      | PJ6       |
| DB1        | Data bit 1 | 36          | PC1            | PC4      | PK6       |
| DB2        | Data bit 2 | 35          | PC2            | PC3      | PJ5       |
| DB3        | Data bit 3 | 34          | PC3            | PC2      | PK5       |
| DB4        | Data bit 4 | 33          | PC4            | PC1      | PJ4       |
| DB5        | Data bit 5 | 32          | PC5            | PD10     | PK4       |
| DB6        | Data bit 6 | 31          | PC6            | PA7      | PJ3       |
| DB7        | Data bit 7 | 30          | PC7            | PD9      | PK3       |
| DB8        | Data bit 8 | 22          | PA0            | PB26     | PJ12      |
| DB9        | Data bit 9 | 23          | PA1            | PA14     | PG13      |
| DB10       | Data bit 10| 24          | PA2            | PA15     | PG12      |
| DB11       | Data bit 11| 25          | PA3            | PD0      | PJ0       |
| DB12       | Data bit 12| 26          | PA4            | PD1      | PJ14      |
| DB13       | Data bit 13| 27          | PA5            | PD2      | PJ1       |
| DB14       | Data bit 14| 28          | PA6            | PD3      | PJ15      |
| DB15       | Data bit 15| 29          | PA7            | PD6      | PJ2       |
| CS         | Chip Select| 40          | PG1            | PC8      | PE6       |
| RS (CD)    | Cmd/Data   | 38          | PD7            | PC6      | PJ7       |
| WR         | Write      | 39          | PG2            | PC7      | PI14      |
| RD         | Read       | 44          | PL5            | PC19     | PG10      |
| RST        | Reset      | 41          | PG0            | PC9      | PK7       |


Supported and Tested Hardware
----------------------------

| Board                   | Tested | Mode                      | Speed     |
|-------------------------|:------:|---------------------------|-----------|
| Arduino Mega            |   ✅   | Direct register access    | Very fast |
| [DIYables MEGA2560 R3 *(works like Arduino Mega 2560 Rev3)*](https://diyables.io/atmega2560-board) |   ✅   | Direct register access    | Very fast |
| Arduino Due             |   ✅   | Direct register access    | Very fast |
| Arduino Giga            |   ✅   | Direct register access    | Very fast |
| Other Mega form factor  |   —    | Arduino API (digitalWrite)| Moderate  |

**Note**: This library is specifically designed for the 16-bit parallel TFT shield for Mega form factor boards. If you have an 8-bit TFT shield for Uno form factor, please use the [DIYables TFT Shield Library](https://github.com/DIYables/DIYables_TFT_Shield) instead.

