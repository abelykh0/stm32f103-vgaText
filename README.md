## stm32f103-vgaText
VGA text demo on STM32F103 "bluepill"

![Board](https://raw.githubusercontent.com/abelykh0/stm32f103-vgaText/master/doc/Board.jpg)

## What it can do
* Display 38x37 characters
* Use PS/2 keyboard to type and move cursor

![Screenshot](https://raw.githubusercontent.com/abelykh0/stm32f103-vgaText/master/doc/Screenshot.jpg)

## Instructions

| Hardware      |    Qty|
| ------------- | -----:|
| [STM32F103 "Blue Pill"](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill) board | 1
| VGA connector | 1
| PS/2 Keyboard | 1
| Resistors 470 Ohm | 3
| Resistors 680 Ohm | 3
| Resistors 2.2 KOhm | 2
| Resistors 3.3 KOhm | 2
| Breadboard | 1
| Jumper wires | 20
| ST-Link v2 or clone | 1

Software: Install free IDE [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html). I am using Windows 10, however STMicroelectronics claims that it also supports Linux and Mac.

How to connect wires:

| PIN | Description | Connect To | Output |
| --- | ----------- | ---------- | ------ |
| PA0 | Red 1 | Resistor 470 Ohm | VGA red (1)
| PA1 | Red 2 | Resistor 680 Ohm | VGA red (1)
| PA2 | Green 1 | Resistor 470 Ohm | VGA green (2)
| PA3 | Green 2 | Resistor 680 Ohm | VGA green (2)
| PA4 | Blue 1 | Resistor 470 Ohm | VGA blue (3)
| PA5 | Blue 2 | Resistor 680 Ohm | VGA blue (3)
| PB0 | HSync | | VGA HSync (13)
| PB6 | VSync | | VGA VSync (14)
| PB10 | CLK | Resistor 2K2 to keyboard CLK and resistor 3K3 to GND
| PB11 | DATA | Resistor 2K2 to keyboard DATA and resistor 3K3 to GND
| G | Ground | | VGA Ground (5,6,7,8,10)

## Overview
The STM32F103 is a Cortex-M3 microcontroller that has neither a video controller, nor enough RAM for a framebuffer at any reasonable resolution.

This demo works around this to produce an acceptable quality 800x600 video with 64 colors to display 38x37 (1,406) 8x16 pixel characters. It uses three timers and a GPIO port. In addition to VGA output, the demo also gets input from a PS/2 keyboard.
The release build is using about half of the 20K available RAM.

## Implementation Details
* Timer TIM4 is used to generate vertical sync signal
* Timer TIM2 is used as a "shock absorber" to make the VGA stable
* Timer TIM3 is used to generate horizontal sync signal
* The code that reads the PS/2 keyboard input is running inside PendSV interrupt
* Video memory contains 2 byte characters (unfortunately, if using 1 byte, only 127 characters can be supported because of limitation of the "drawing" assembly function) and 4 byte "attributes". The attribute is an address to a 128 byte lookup table containing (4 * 16) normal colors and (4 * 16) inverted colors (to show cursor)
* File settings.h contains definitions for default colors (background and font), timing for VGA mode (set to 800x600 @ 56 Hz), as well as number of character columns and rows
