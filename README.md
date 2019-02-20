# An Arduino Driver from the TFMiniPlus Lidar system.

Readme and documentation is WIP

Library is currently being developed under the initial-api branch.

Current state is:

* Can read the distance / sensor tempreture / signal strength from the device 
* Can Request the firmware version from the device

What needs to be done to consider the api complete is:

 * Be able to reset the system
 * Be able to set the frame rate
 * Be able to trigger manual detection
 * Be able to set the output format between milimeter, centimeters or Pixhawk 
 * Be able to set the baud rate
 * Be able to enable / disable the device
 * Be able to restore to factory settings
 * Be able to save the configuration onto the device
