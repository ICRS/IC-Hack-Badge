# IC-Hack-Badge

Repository for how to use and program the IC Hack PCB badges. They are functionally very similar to the [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/), and any guides for that board will likely work for this one too.

Microcontrollers are small but flexible computers that are great at interacting with the real world. We hope that by giving everyone a powerful microcontroller like the RP2040, they are able to give embedded programming a go and make something cool!

Kits of sensors and electronics are available from ICRS to help build whatever you have in mind. ICRS can also provide knowledge and help with anything relating to hardware projects and the badges.

![hacker-front](/Images/hacker-front.png)

The badges were designed using [KiCad](https://www.kicad.org/) and all the design files and schematics are available in the [/Hardware](/Hardware/) folder.

## Contents

- [Features](#features)
- [Pinout](#pinout)
- [Writing Code](#writing-code)
- [Uploading Code & Boot Mode](#uploading-code--boot-mode)
- [Resetting Your Badge](#resetting-your-badge)
- [Running From Battery](#running-from-battery)
- [Credits](#credits)

## Features

The features of the board are listed below:

- RP2040: Dual Arm Cortex M0+ cores up to 133MHz
- 264kB RAM
- 2MB Flash
- 24*5 Pixel LED Matrix
- Light up LED logo
- CR2450 Battery holder
- USB C

## Pinout

The functions of each pin are shown here from the back side.

![pinout](/Images/pinout_bottom.png)

These pins can be connected to using standard Dupont 2.54mm jumper cables.

There are also pins that connect to things internally on the board:

| **Pin Number** | **Function**         |
|----------------|----------------------|
| 4              | Logo H               |
| 5              | Logo C               |
| 6              | Logo I               |
| 15             | Display serial data  |
| 16             | Display enable       |
| 17             | Display serial clock |
| 18             | Display latch        |

## Writing Code

There are several ways of programming the board using either C/C++ or MicroPython.

- [Arduino](#arduino)
- [MicroPython](#micropython)
- [VS Code & C/C++ SDK](#vs-code--cc-sdk)

### Arduino

Arduino is an open source platform for programming microcontrollers in C++. An [Arduino library](https://github.com/ICRS/IC-Hack-Badge-Arduino) has been written for the board that makes using the built in LED matrix easy.

To get started, [download Arduino](https://www.arduino.cc/en/software).

Once you have it running go to `file > preferences` and add the following link to `Additional boards manager URLs`: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

![arduino_preferences](/Images/arduino_preferences.png)

You then need to install support for the RP2040 by going to the board manager tab on the left, searching for "Raspberry Pi Pico" and installing the one by Earle F. Philhower, III.

![arduino_board_manager](/Images/arduino_board_manager.png)

Go to the board selector in the top, and set the current board to the `Raspberry Pi Pico`.

![arduino_board_select](/Images/arduino_board_select.png)
![arduino_board_and_port](/Images/arduino_board_and_port.png)


Finally, we need to install the library made for this board by going to the library manager tab on the left, searching for IC Hack Badge, and installing the most recent version. 

![arduino_library_manager](/Images/arduino_library_manager.png)

You can then check out example code in `File > Examples > IC Hack Badge`, or start writing your own code.

To program the badge, put the board in [boot mode](#uploading-code--boot-mode) and select the port it is connected to through the same menu where you chose the current board. You may need to check the `Show all ports` box. Then press the upload arrow in the top left.

### MicroPython

To use MicroPython first upload the interpreter to the badge using the [latest firmware from Raspberry Pi](https://micropython.org/download/rp2-pico/rp2-pico-latest.uf2), which you can upload using the instructions in [Uploading Code & Boot Mode](#uploading-code--boot-mode).

You can then connect to the serial port using a program such as PuTTY on Windows, or Minicom on Linux, and start executing code in the MicroPython shell.

![micropython_shell](/Images/micropython_shell.png)

Here we are printing "Hello World" over USB, and then setting pin 4 high, which turns on the H LEDs on the badge's logo. The mapping of pins to their functions is shown in the [Pinout](#pinout) section.

You can use the [MicroPico](https://marketplace.visualstudio.com/items?itemName=paulober.pico-w-go) VS Code extension, or [Thonny](https://thonny.org/) to write programs that you can then upload to the badge.

See the [documentation by Raspberry Pi](https://www.raspberrypi.com/documentation/microcontrollers/micropython.html) for more info and APIs.

Currently there is no MicroPython code for writing to the LED matrix, but both the Arduino library and the default firmware have implementations in C++ that could be ported to Python.

### VS Code & C/C++ SDK

The C/C++ SDK is the official way released by Raspberry Pi for programming the RP2040 microcontroller. It offers more depth and features than Arduino, but lacks Arduino's abstractions and libraries, that can make using it harder.

Setting it up can be a bit more challenging than using Arduino too (especially on Windows), and we recommend following [Raspberry Pi's guides](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html) if you want to do so.

The firmware flashed to all the badges was written using the C/C++ SDK, and can be found in the [/SDK](/SDK/) folder for reference. Sorry for the spaghetti, blame Haotian -_-

## Uploading Code & Boot Mode

Plug the board into a computer using the USB C connector. Then in order to program the board it needs to be put into "boot mode". This is done by **holding down** both the reset button (**RST**) on the top of the board **and** the boot button (**BOOT**) on the bottom of the board. Then **release RST**, wait a bit and **release BOOT**.

This will make the board show up as a USB drive that you can drag and drop compiled uf2 binaries into.

## Resetting Your Badge
The default firmware is [firmware.uf2](/firmware.uf2) and you can upload it to your badge with a USB C cable after putting it in [boot mode](#uploading-code--boot-mode).

## Running From Battery

Coin cells are able to supply a maximum of around 10mA continuously (ideally <2mA), but the RP2040 when running at full speed can draw well over 20mA. This means that the power consumption needs to be lowered by changing the CPU frequency and voltage so it can be run on the battery.

This can be done in C/C++ using the following lines:

```C
#include "pico/stdlib.h"
#include "hardware/vreg.h"

void main() {
  vreg_set_voltage(VREG_VOLTAGE_0_90);
  set_sys_clock_khz(18000, false);
  ...
}
```

This lowers the core voltage to 0.9V, and the core frequency to 18MHz which draws around 6mA. This was found to be a good balance of low power and stablility, however it means the USB is disabled since it requires a minimum of 48MHz.

> [!NOTE]
> The battery should last for well over 24 hours with the default firmware, so it can be left on for the whole hackathon.

Using the pico extras repo in the C/C++ SDK (sadly not in Arduino) the chip can be put to sleep which only draws 0.7mA. You can find out more about pico_sleep [here](https://github.com/raspberrypi/pico-extras/tree/master).

When running MicroPython the RP2040 consumes too much power for the battery, so you should use USB instead.

## Credits

Hardware: Ben Smith

Artwork: Anna Votin and Kevin Wang

Firmware: Haotian Wu (+ Ben Smith)
