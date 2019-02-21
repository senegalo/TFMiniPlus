# An Arduino Driver from the TFMiniPlus Lidar system.

Readme and documentation is WIP

Library is currently being developed under the initial-api branch.

Current state is:

* can read the distance / sensor tempreture / signal strength from the device 
* can Request the firmware version from the device
* can reset the system
* can set the frame rate
* can trigger manual detection
* can be able set the output format between milimeter or centimeters 
* can set the baud rate
* can enable / disable the device
* can restore to factory settings
* can save the configuration onto the device

What needs to be done to consider the api complete is:

* Several optimization parts in the code
* change the setEnabled method to take a boolean instead of a constant
* create the keywords file
* package for arduino 
