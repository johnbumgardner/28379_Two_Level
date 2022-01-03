# Two Level DC-AC Converter Controller
> Supporting FREEDM Systems Center at NCSU on WBG Semicondtor Applications

This code employes State Vector Pulse Width Modulation to control a Two Level Gallium Nitride FET controlled Inverter to deliver current to a three-phase motor.
For this, I chose to use a digital switching implementation - simply toggling pins. The Microcontroller I used was the F28379D LaunchPad™ development kit manufactured
by Texas Instruments.  Also I used the many tools provided by TI, namely ControlSuite, C2000Ware, and Code Composer Studio.

![](http://www.ti.com/diagrams/med_launchxl-f28379d_angle.jpg)

## Installation

Windows Only

1. Install Code Composer Studio, ControlSuite, and C2000Ware.
2. Clone this repository in local directory.
3. Open this project through Code Composer Studio


## Usage example

Universities, organizations, and companies, around the world, are studying the unique properties of wide-bandgap semiconductor electronic devices.
As a result of their unique properties, these materials (Silicon Carbide and Gallium Nitride) are ideal in applicaitons unsuitable for 
tradition semiconductor materials, like Silicon and Gallium Arsenide, such as higher voltages, temperatures, and frequencies. For these properties,
these devices are especially suited for RF and Power applications.  

This code was designed to work on a 2 level (6 FETs) Gallium Nitride device that converted direct current into alternating current. The program accepts an 
internally defined input that specifies the angle and the magnitude of the desired motor voltage, then determines what state vectors must be outputted,
and for what length of time as well. This code/algoithms implemented could be used on a multitude of other switch-mode power electronic converters utilizing
a variety of semiconductor materials.  

This current build is for RAM only - primarily what I use to test and debug on the MCU. Using CCS it is easy to create a version to utilize Flash memory.


## Release History

* 0.1.0
    * The first proper release

## Meta



[https://github.com/johnbumgardner/github-link](https://github.com/johnbumgardner)
