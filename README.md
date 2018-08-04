# MusicHall
### First prototype for Music Hall sound installation

## Equipment:
* Arduino Uno
* MB1260 XL-MaxSonar-EZL0
* Raspberry with PureData
* Audio speaker

## Set-up:
* Connect HC RS04 to Arduino (follow data sheet instructions)
* Download and install MusicHall Arduino sketch
* Download PureData patch and open it on Raspberry side
* Connect Arduino to Raspberry with USB cable
* Connect audio speaker to Raspberry built-in analog audio outlet
* From PureData patch, find out correct USB port where Arduino is connected
using the _devices_ object and put in the _open <port>_ object
* Hit the _open <port>_ object
* Hit the _tgl_ object connected to _pd dsp $1_ message to enable audio computing
* Have fun!

## Arduino Side
* music-hall-arduino.ino directory

## Raspberry Side
* music-hall-pd directory


### NOTE: you can use your computer instead of the Raspberry PI
