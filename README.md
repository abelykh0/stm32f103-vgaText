# stm32f103-vgaText
VGA text demo on STM32F103 "bluepill"

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
