<p align="center">
  <img src="https://github.com/A-N-Ulab/OTSI-cyberpet/assets/131162335/a7465629-5936-4026-bd4e-a9c7b908ea3b">
</p>
<p align="center">
  <img alt="Static Badge" src="https://img.shields.io/badge/have%20fun%20while%20making%20otsi-yellow">
  <img alt="Static Badge" src="https://img.shields.io/badge/ANUlab&Glinek-Otsi-blue">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/A-N-Ulab/OTSI-cyberpet">
  <img alt="GitHub commit activity (branch)" src="https://img.shields.io/github/commit-activity/w/A-N-Ulab/OTSI-cyberpet/main">
  <img alt="Static Badge" src="https://img.shields.io/badge/License-CC_BY_4.0-blue">
</p>

<p align="center">
  <img src="https://github.com/A-N-Ulab/OTSI-cyberpet/assets/131162335/705cb7bc-523f-4a10-80e8-7dd7ab574440" height="auto" width="270">
  <img src="https://github.com/A-N-Ulab/OTSI-cyberpet/assets/131162335/80723e43-d20f-4c51-9b04-db50c9392800" height="auto" width="270">
  <img src="https://github.com/A-N-Ulab/OTSI-cyberpet/assets/131162335/ad5e6828-a9dd-4269-b79f-161b0dead243" height="auto" width="270">
</p>

# About this project
### Story
It all started when [cyberdeck cafe](https://cyberdeck.cafe/) announced a contest for cyberpet based on heltec development board. And fast forward 6 months my cyberpet is ALMOST done.

### Credits
Otsi's graphics are based on axolotl design made by [witch](https://dinopixel.com/pixel-art/purple-axolotl/17015)   
Main menu is based on a desing by [upirr](https://github.com/upiir/arduino_oled_menu)   
This project wouldn't be possible without awesome cyberdeck cafe's community. [Cyberdeck cafe website](https://cyberdeck.cafe/), [Cyberdeck cafe discord](https://discord.gg/cyberdeck-cafe-607459933784637460)

### Disclaimers
THIS PROJECTS IS WORK IN PROGRESS but updates will be in 2ish weeks, i need a break   
* There are a lot of things that can and probably will change, to get latest cad files, code and instructions download lates [release](https://github.com/A-N-Ulab/OTSI-cyberpet/releases/latest)
* soldering otsi is not beginner friendly, have it in mind before you start this project

# Build your own
### Needed items
Detailed information about needed items is in the newest release in pdf called *__list of items needed__*
| Name | Quantity(pieces) | info |
| ------------- | ------------- | ------------- |
| heltec LoRa v3 | 1 | - |
| Mx switch | 3 | i'm using brown ones |
| Antena for lora | 1 | IPX to SMA cable and SMA antena |
| Keycap | 3 | - |
| 18650 battery cell | 1 | - |
| 18650 battery holder | 1 | 76x20x19mm |
| RTC DS1302 | 1 | - |
| CR2032 | 1 | - |
| DHT11 | 1 | temperature and humidity sensor |
| ec11 rotary encoder | 1 | rotary encoder with button |
| Rocker switch | 1 | cutoutsize of 13.5x9mm |
| photo resistor 5-10kohm | 1 | - |
| 3mm LED | 2 | one blue LED and one red LED |
| TP4056 | 1 | with USB C |
| push button | 1 | 6x6mm |
| USB C cable| 1 | USB A to USB C|
| 1W led | 1 | max heatsink diameter 20mm |
| I2C 0.96inch OLED | 1 | - |
| 10k resistor | 5 | - |
| M2 threaded inserts | 6 | - |
| M2x8mm screws | 6 | - |

### Case
Theoretically you could design your own case but I'm providing my case design. There are 5 stl's:  
* Antena holder - print without supports
* Knob - print without supports
* Cover - pritn with supports
* Two Main body files, one with holes for m2 threded inserts one with holes just for screws - both are print without supports    
Additionaly there are 2 files (.f3d and .step) that contain whole Otsi design

### Schematic and soldering
More accurate schematic is avaiable in latest release in pfg called *__schematic__*  
![schematic image](https://github.com/A-N-Ulab/OTSI-cyberpet/assets/131162335/9042c696-bbe9-449c-8ecf-2df62a072ed3)      
Solder everything according to the schematic provided, for additional help in latest release there is a pdf file named *__photos__* that have, as name sugests, all the photos from my soldering process.

### Software
Before uploading otsi's code you are going to have to change 2 variables:    
<img width="400" src="https://github.com/A-N-Ulab/OTSI-cyberpet/assets/131162335/06644cf0-b99a-4477-9333-134a14bbfa56">     
Set those to variables to WiFi network (ssid and password) that you always want to connect to, your *__home wifi__*     
After that you are ready to upload code to your heltec or from now your own Otsi cyberpet   

### Usage
To go to main many or chose any option press enoder button, button number 1 is a return, everything else will be descibed on otsi itself.  
To charge Otsi use port on the top. To program Otsi use USB cable.  
NEVER PLUG IN OTSI TO USB WHEN POWER SWITCH IS TURNED ON
