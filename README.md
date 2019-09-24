# Arduino Library for the TFMiniPlus Lidar system.

First of all ... i want to thank Jimi Hindrix, Eddie Hazel, The Doors and so many more for their moral support during the writing of this library. They shall remain an inspiration to this generation and all the generations to come. 

Now ... let's go straight to business !

This library is meant to be used with the TFMini plus not to be confused with it's previous successor the TFMini "totally didn't happen to me". You can find the documentation provided by the manufacturer in the [documenataion folder](/documentation)

## How to add to the Arduino libraries

To use the library you need to get the latestest release [here](https://github.com/senegalo/TFMiniPlus/releases/latest) and in your Arduino ide: Scketch > Include Library > Add .zip library.

## Initialization whithin your sketch

First start by including the library to make the class available inside your sketch  

```
#include "TFMiniPlus.h"
```

Then define a variable of type `TFMiniPlus` that shall forever hold the class. and in your setup method initialize a `Serial` port of your choice and pass it as a reference to the `begin` method. Notice that the default baud rate of the device is `115200` so unless you changed it via the configuration commands and saved it use the default. You should have something like that at the end:

``` c++
TFMiniPlus tfmini;

void setup(){
    Serial.begin(115200);

    tfmini.begin(&Serial);
}
```

## Data read mode

The TFMiniPlus operates by continuously sending data with a preset frame rate via the configuration "_default is 50 frames/second_". The data frame contains 3 pieces of information.

* The Distance in mm or cm "_cm by default_"
* The signal strength which represents how confident the sensor is into that reading. It goes from of **0** to **65535**. The documentation says that if the value is less than **100** or equal to **65535** then basically the reading is completely unreliable and it will just return a distance of **0**. 
* The internal sensor temperature in Celsius because ... why not !

To read the data frame use the `readData()` method which returns true when it has successfully captured a frame and validated it's checksum. After the data is read use the utility functions `getDistance()`, `getSignalStrength()` and `getSensorTemperature()` to access the respective data. A more complete example below:

``` c++
void loop() {
  // read the data frame sent by the mini
  if (tfmini.readData()) {
    // Distance "default in CM"
    tfmini.getDistance();

    // Sensor temperature in celsius
    tfmini.getSensorTemperature();

    // Signal Strength
    tfmini.getSignalStrength();
  }
}
```

## Sending Commands

You can control many aspects of this device like the baud rate and the data frames frequency. Every time you send a command to the TFMini it responds with a data frame either to affirm that the configuration has been applied or not or to return data like in the `getVersion()` method. So as a rule of thumb most configuration methods will return a **boolean** representing whether the operation was success or not unless they have some data to return.

### Get Version

The TFMini's internal firmware in case you want to update it or just display it in a ... i don't know what you might use it for :D nonetheless there is a method to get it.

```c++
void setup(){
  // Initialization as mentioned above

  // Get firmware version
  tfmini.getVersion(); //Ouputs as a String "1.2.3" or "Error" 
}
```

### System reset

I am not fully sure when you might want to reset you TFMini but there is a method for it. It will basically _restarts_ your TFMini in case it's stuck or something. Also to note here don't confuse this method with the `restoreFactorySettings()` method which will reset all your saved settings to the defaults.

```c++
void setup(){
  // Initialization as mentioned above

  // resets the tfmini
  tfmini.systemReset(); //Ouputs a boolean   
}
```

### Frame Rate

With this method you control how frequent the TFMini should scan and send you back the data. The default value is **100 frames/second**. You can also set the frame rate to **0** and trigger the detection manually with the `triggerDetection()` described below. The range is from **0** to **1000**

```c++
void setup(){
  // Initialization as mentioned above

  // Set the frame rate to 100
  tfmini.setFrameRate(100); //Ouputs a boolean   
}
```

### Trigger Detection

When you set the frame rate to **0** the TFMini will stop sending you data unless you call the `triggerDetection()` method. This method is unlike the other command methods as it doesn't output anything. you should call it and then call `readData()` afterwards to capture the data frame.

```c++
void loop() {
  //Trigger the detection
  tfmini.triggerDetection();

  // read the data frame sent by the mini
  if (tfmini.readData()) {
    //...
  }
}
```

### Change the output format

The TFMini supports 3 types but the library supports only 2 of them. It can return the distance in **milimeters**, **centimeters** and **Pixhawk**. The library supports only the first 2. To change the output format call the `setMeasurementTo()` method with on of the following predefined macros `TFMINI_MEASUREMENT_CM` or `TFMINI_MEASUREMENT_MM`. By default the TFMini outputs the distance in **centimeters**.

```c++
void setup(){
  // Initialization as mentioned above

  // set the measurement to mm
  tfmini.setFrameRate(TFMINI_MEASUREMENT_MM); //Ouputs a boolean   
}
```

### Change the baud rate

The TFMini by default operates with a baud rate of **115200**. You can change that by calling the setBaudRate method. Don't forget to reinitialize your `Serial` port afterwards to be able to communicate with the TFMini. The documentation dictates that only the standard baud rates are supported so don't go crazy !

```c++
void setup(){
  // Initialization as mentioned above

  // Change the baud rate to 9600
  tfmini.setBaudRate(9600); //Ouputs a boolean   
}
```

### Enable / Disable the device

You can enable and disable the TFMini by calling the `setEnabled()` method. 


```c++
void setup(){
  // Initialization as mentioned above

  // disable the TFMini
  tfmini.setEnabled(false); //Ouputs a boolean   
}
```

### Restore factory Settings

When the worlds turn it's back on you. Rest to the factory settings. this will wip out all your custom configuration and sets back the defaults. To do so call the `restoreFactorySettings()` method.

```c++
void setup(){
  // Initialization as mentioned above

  // Restore factory settings
  tfmini.restoreFactorySettings(); //Ouputs a boolean   
}
```

### Save settings

Basically persists the changes you made in the configuration to the TFMini itself. So when power goes out and back again you don't have to configure it again. To save the current settings use the `saveSettings()` method.

```c++
void setup(){
  // Initialization as mentioned above

  // set the measurement to mm
  tfmini.setFrameRate(TFMINI_MEASUREMENT_MM); 

  // persist the settings to the TFMini
  tfmini.saveSettings() // Outputs a boolean    
}
```
