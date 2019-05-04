## stm32f103-vgaText
VGA text demo on STM32F103 "bluepill"

## What it can do
* Display 36x37 characters
* Use PS/2 keyboard to type and move cursor

## Instructions
Software: Install free IDE [System Workbench for STM32](https://www.st.com/en/development-tools/sw4stm32.html/). I am using Windows 10, however STMicroelectronics claims that it also supports Linux and Mac.

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

This demo works around this to produce a reasonable quality 800x600 video with 64 colors to display 36x37 (1,332) 8x8 pixel characters. It uses three timers and a GPIO port. In addition to VGA output, the demo also gets input from a PS/2 keyboard.
The release build is using less than half of the 20K available RAM.

## Implementation Details
* Timer TIM4 is used to generate vertical sync signal
* Timer TIM2 is used as a "shock absorber" to make the VGA stable
* Timer TIM3 is used to generate horizontal sync signal
* The code that reads the PS/2 keyboard input is running inside SyncSV interrupt


