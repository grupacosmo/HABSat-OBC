# Remote Camera Controlling System (rcc-sys)
## Description
It's a project that aims to introduce our OBC team to microcontrollers and freeRTOS real-time operating system.

## Hardware
* STM32F446RE MCU
* LCM1602 LCD
* DS1307 RTC
* BME280 Temperature, pressure and humidity sensors
* Waveshare 3947 MicroSD card reader
* ESP8266 WiFi module
* OV7670 Camera with servomechanism that allows it to rotate 

## Status
This project has been officially closed.

What's done:
* Button input recognition done with interrupts
* Blinking of MCU's LED.
* Measuring time
* Measuring weather conditions
* Displaying the information on the LCD

Some of the initial plans turned out not to deliver practical enough skills for our team. More specifically:
* WiFi module - we should move our resources for learning long range data transmission methods
* Camera - should be handled by AI team.

Furthermore, the main goal of this project has been achieved, as we have aquired the skills that we wanted.

Considering the above, we have decided to abandon this project and move onto the new one that derives from rcc-sys with the goal of sending the High-altitude balloon into the stratosphere. 

## Required tools
* CMake
* GNU Arm Embedded Toolchain
