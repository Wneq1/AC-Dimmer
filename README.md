# Power-Regulator

Power regulator for arduino 
![IMG_0336](https://user-images.githubusercontent.com/127328405/233417921-0a248f8b-8eb7-490d-a4a8-28f2506e8195.JPG)
In today's project, we will focus on building a power regulator using an Arduino microcontroller and a thyristor. Power regulators are commonly used to control the amount of electrical energy supplied to loads such as lamps, heaters, or motors, in order to adjust their brightness, temperature, or speed. By building your own power regulator using Arduino and a thyristor, you can create a cost-effective and customized solution for controlling AC loads in DIY projects or home automation systems. IF you wish to built one by yourselfe in this file, you can find a finished project in KiCad and short code in Arduino.

WARNING: THIS PROJECT INVOLVES HIGH VOLTAGES THAT CAN BE DANGEROUS TO LIFE AND HEALTH. 
YOU PERFORM THIS PROJECT AT YOUR OWN RISK AND RESPONSIBILITY.

To complete this project you will need:
Arduino or other microcontroller
* Fuse
* Resistors
    * 68k x2
    * 33k x2
    * 360 x1
    * 330 x2
    * 10k x5
    * 39 x1
* Capacitors
    * 0.01uF x1
    * 100nF x1
    * 25uF x1
* Optocoupler PC817
* Hi-link
* LM7805 (5V regulator)
* LED
* Optotriac MOC3041
* Pushbuttons
* OLED Display
* Thyristor BT138

Our system consists of several modules: 
* power module
* actuator module
* measurement module
* logic module

![image](https://user-images.githubusercontent.com/127328405/233717977-a205c8f1-172e-4290-bee5-1d3509b0191a.png)

Short description of this project:

In AC-powered devices, power is closely related to the area under the sine wave applied to the input of our controlled system (e.g., a light bulb), and it is this area that can be regulated. However, in order to do this, we need a reference point. Since the signal present in the power socket is sinusoidal in nature, our reference point will be the zero crossing of the waveform, which will define how long our load will be energized. The longer the duration, the higher the power.

Now, let's do some math. Let's say the frequency of the power grid is 50 Hz, which means the period is f=1/T T=1/f=1/50=20[ms]. In each cycle, there are two peak values of the sine wave, so we reach the peak every 10 ms. If we manage to turn on the light bulb at the very beginning, it will receive 100% of the available power. If we turn it on halfway through (5 ms), it will receive 50% of the power, and if we turn it on towards the end of the 10 ms period, it will remain off (0% power).

Short description:
The waveform appearance for 100% power:
![image](https://user-images.githubusercontent.com/127328405/233792444-77caff8e-5803-43e4-a006-3274114af943.png)
The waveform appearance for 50% power:
![image](https://user-images.githubusercontent.com/127328405/233792453-679abb82-182c-4c3c-9561-93fae93011a5.png)




Below are some illustrative photos:
![IMG_0333](https://user-images.githubusercontent.com/127328405/233417970-14f5ca37-7357-4c78-9306-d3bf5964af4e.JPG)
![IMG_0334](https://user-images.githubusercontent.com/127328405/233417983-43a7cb51-7ace-44d9-91cd-f096ca21e5a4.JPG)
