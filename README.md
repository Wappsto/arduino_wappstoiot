_This is an early version of the library, so the interface may still change at certain points. We will try to keep the changes to a minimum, if possible._

[![Seluxit A/S](https://img.shields.io/badge/Seluxit_A/S-1f324d.svg?logo=data:image/x-icon;base64,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)](https://seluxit.com)
[![Wappsto](https://img.shields.io/badge/Wappsto-1f324d.svg?logo=data:image/x-icon;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAACXBIWXMAAAEuAAABLgF7cRpNAAAEM0lEQVR4nO1bwY3bMBCcyysBEpw6sDo4P4LgflEHUQm6DlyCSrgOTiU43yAPpgOVoOtATh55bh6iHJm3uyJFOQxwGmCB2CR3h0tyuVxfbogIrxlvUhNIjc0BqQmkxuaA1ARSY3NAagKpsTkgNYHU2BwgfF8BOAIgRloABwD59el5I8fAqQXP+YhhTi9BRFPZE1FL/miIKHN0/EvJLAdftHaOZx3u5PsAZSN6V+k/klX4jsryhcqmqALIx0oVybWnYc5nB4RsIw0l/f+TH9EQEW6I6C2A30Jw+QqgAdDbz4UNJjuh/8n2af3jVxD2AAyAW6H9GQNfYz9nGPh+Efq/G7c/h1pZhaPi2WvFhLkzfyQ5INfCmBxEVAiNMVuxU8gskbkY5RN/OBQxiVAD4EFo29ltmEXoH5FhuMelbf9guSwDEd0L3vFdwUdlZY6RKw/S8xLtmE4lE8bfazEgJKI3Cslm4cTX1FsKOnLtfDwGkjUKWZ8z6spB0RfqVGmXQiPfBRrJSN+uRYCuStHTUniA7Rg9ZuoAydt5oKGc5Gjtez1q1123YPK5oOtAROfXoBFiZBEYUzs75sS03WKI5trNkEFOdE4ASvxNynxRCN8bABePIc7roWfNZwsbZdxaR2gqDaOrH9unHbnsrqdlRrVjRcQ7liM6oorgwS3s+Xr2WbWYtNZ3UmtG/Kns52x7B4sImdvWhdKuHZeYXZiPfdwB3RVIZCRH9V9E9FNoW3LduWIYvd20j/sWODLR8nNg1HXRQ74Z3gP4wHx/wvCMDY34LjjuF3N0HWAERWUkkbGQ6osK8TUFibO5+MRsGw6habEktaB/inolW2L6O5VF5yZSvimT/76iHa94xtUDuDiwwzq/A2QAPintH7FODSEHX7Z7MTfOAUZQWiymc6lbKmzAtkn2Q1Ao9i/AOaAFH7FjA2ED4M6j3x1iKjwDOK4ncIFVOD8Nc35i0uJKOfcSqgh73u8aqSZomO9uMZSlQ7EH8CS0/bDC4SnCHnfMDNc5xAFA+DEYn7ccnq2+0v5b4hEaFP3u/xHKNuJy+DZwK0rvALc4ohVB1rAp6tAUSYmEb37OxZER3PnW4gR7fhmRqr9iIqcpKwRlPtVibTJaVqmV2H2ComS3kMbMKeQwtxqS44j8XpZGGS9OhPRdJ46ZI8NViTql/xoFzYz4NJZovrDKjVN/nJn7aYyLnDvwmVaJdQqave0rFVYN+EhfgE9/jWptZjVyZSUONGzJmvRtS7QsqdHiCFmbteVwIHnn5ZodHyJzk5tD7WFDkjrS9mzM8SFRRBBoPPTPSRNhv5jTf00SaxVRQPr1KMHL+dcg0dF1/laoJPl2cOHt/FASOQ2edYn0NFw31RUm7kplbblBr7Pc8hB9N0Tbf5p61dgckJpAamwOSE0gNTYHpCaQGpsDUhNIjVfvgD9WFsGCdX/VsgAAAABJRU5ErkJggg==)](https://wappsto.com)

## HW target setup

The library currently has examples for two HW targets:
* ESP32 - see examples/esp32_wappsto/esp32_wappsto.ino for setup details
    * arduino_wappstoiot support ESP32 library versions 1.0.x and 2.0.x
* Wio Terminal - see examples/wio_wappsto/wio_wappsto.ino or wio_tft_wappsto.ino
    * This board requires multiple libraries and an WiFi firmware upgrade to work

If you want to use your own hardware, use these examples as reference. Note that UTC time is required, so NTP or an alternative time implementation is required.


# arduino_wappstoiot - library description

## Prerequisites

### An account on Wappsto
It is also assumes you have created a account on www.wappsto.com, if not please create one first.

### Arduino IDE installed on your PC
If you do not have this, see https://www.arduino.cc/en/Guide

#### Install ArduinoJson library
From the library manager you need to install ArduinoJson library - see https://arduinojson.org/v6/doc/installation/ for more details.
Latest tested version: 6.21.3

#### Install this wappsto library
To install this library in Arduino Ide, download this project as a zip-file, click "Code" and choose "Download ZIP".
Then choose "Sketch"->"Include Library"->"Add .Zip Library..." See https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries for details.


### Generate certificates and id for a new device
For your physical device to identify itself towards Wappsto it needs a network id, the server CA certificate, and the client certificate/key - all these has to be generated on Wappsto.com and placed in (if matching the examples) called wappsto_config.h.

#### Generate wappsto_config.h
The easiest way is to download the header file from a Wapp.

On https://wappsto.com go to the store and install the Wapp "IoT Certificate Manager" (https://wappsto.com/store/application/iot_certificate_manager).

Open the Wapp and select: "Header file (used in WappstoIoT for Arduino)" and click download.

#### Using python script (deprecated, old way)

As a help to generate this you can use the python script included in this repository in the folder generate_config_header.
For a first time run, you may need to install the required libraries
```
cd <path to downloaded arduino_wappstoiot>
cd generate_config_header
pip install -r requirements.txt
```
To generate a header file:

1. Go to the `generate_config_header` folder:
```
cd generate_config_header
```
2. Use the command:
```
python main.py --type arduino
```
3. Then you will be asked to login using email and password for your user on Wappsto.
4. Copy the newly generated file `wappsto_config.h` to your Arduino sketch folder. Note if generated correctly it should have a valid UUID (a string similar, but different, to this `"d7fafe76-b020-4594-8f2a-aae11c6b6589"` defined for the `const char* network_uuid =` line.


Note, if you have both pyhton2 and python3 installed you may need to use
```
pip3 install -r requirements.txt
python3 main.py --type arduino
```

If you get the error
```
ImportError: cannot import name 'soft_unicode' from 'markupsafe' 
```
It is due to a change in dependency for another library, it can be fixed using this command.
```
pip install -U MarkupSafe==0.20
```


#### Claiming and ownership - Not allowed to access values

If you get a "not allowed to access" values on the device you have created, it is because you either have not claimed it, or the ownership have been reset. 
Since you would be the manfactorer you will still see the device on your list of networks, you can see it is online, but you will not be able to see values or control the device.

To claim a device, go to the "IoT Devices" tab on https://wappsto.com/devices click the "+ Add an IoT device" button in the top right corner, and enter the network UUID int the box.

If you click the delete button for a network, it will remove the ownership of the device, so it can be claimed by another user. Once a device have been claimed it can not be claimed by another.


# This Wappsto library

## Required structure

The network id is generated by www.wappsto.com and is linked to the certificates.

```
├── "Network Name"
│   └── "Device 1 name"
│   |   └── "Value 1 name"
│   |   └── "Value 2 name"
|   |   ...
│   └── "Device 2 name"
│       └── "Value 1 name"
│       └── "Value 2 name"
...

```

Note that that "name" for device and value object have extra functionality, and rules.
* A network must not have have devices with identical names.
* A single device must not have values with identical names. (as the example above it is allowed to use the same name under a different device.)

The library will ask Wappsto if an device/value exist with that name before creating it, so it will link to the right value. If nothing have this name a new will be created. If a parent have multiple children with the same name the library will choose the first available.

If you change a name a new will be created, but but the old will not be delete. This you will have to do yourself, eg. using https://wappsto.com/devices

## Initialize Wappsto:
Wappsto needs a reference to WiFiClientSecure when created, example.
```c
WiFiClientSecure client;
Wappsto wappsto(&client);
```

### Set the config and connect

```c
#include "wappsto_config.h"
...
wappsto.config(network_uuid, ca, client_crt, client_key);
if(wappsto.connect()) {
    // Connected
} else {
    // Failed to connect
}
```

## Optional config parameters, log and ping
Besides the mandatory commands you can also set ping interval and log level:
```c
wappsto.config(network_uuid, ca, client_crt, client_key, ping interval in minutes, log level);
```
* The ping interval will send a short package from the device to wappsto to keep the connection alive. If your device rarely sends data, it might be a good idea to add this to avoid timeout on the connection.
* The log level can print information from the wappsto library to the serial debug port - the following levels are possible:
```
   VERBOSE
   INFO
   WARNING
   ERROR
   NO_LOGS <- Default
```

## Create your network:
```c
myNetwork = wappsto.createNetwork("Network Name");
```

## Create a device:
```c
DeviceDescription_t myDeviceDescription = {
    .name = "Device name",
    .product = "Product name",
    .manufacturer = "Company name",
    .description = "Description of the product",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};

myDevice = myNetwork->createDevice("Device Name", &myDeviceDescription);
```

## Values
Values is probably what you are mostly interessted in, and can be one of these 3:

* Number - integers or decimals, these will be logged and shown as a graph.
* String - a human readable string (UTF-8)
* Blob - data, for example a base64 encoded image, hex values, etc.
* Xml - a complete xml document

### Read and write data

Each value can have one or two data points:

* Report: Data read on the device and reported to the server [READ]
* Control: Data sent from the server to the device to control it [WRITE]

Your value may be one of them or both.

### Value parameters
To explain the parameters, we will use an example for a temperature value placed in a living room.

* name: An name for the value, here "Living room"
* type: Is a help for the UI to find values of the right type, here "temperature"
* The PERMISSION_e parameter tells the library if it should create report and/or control for this value.
    * READ -> report state
    * WRITE -> control state
    * READ_WRITE -> report and control state
* min: (Number only) lowest number (to be used by UI)
* max: For number highest numer, for string/blob maximum length
* step: (Number only) Step size for a number, for example 1 for integers, and 0.1 for decimals
* unit: (Numbers only) is there a unit for this number, in the temperature example it is °C

The first time a value is created a number will have the value NA, and the string/blob will be empty, for both control and report.
If the value exist, the data of the value will not change. If you want to value to update when the device reboots, you have to call report/control.

### Create a number value:
```c
ValueNumber_t myNumberValueParameters = {   .name = "Living room",
                                            .type = "temperature",
                                            .permission = READ_WRITE,
                                            .min = -20,
                                            .max = 100,
                                            .step = 0.1,
                                            .unit = "°C",
                                            .si_conversion = ""};

myNumberValue = myDevice->createNumberValue(&myNumberValueParameters);
```

### Create a string value:
```c
ValueString_t myStringValueParameters = { .name = "Value String Name",
                                          .type = "value type",
                                          .permission = READ_WRITE,
                                          .max = 200,
                                          .encoding = ""};

myStringValue = myDevice->createStringValue("Value String Name", "value type", READ_WRITE, &myStringValueParameters);
```

### Create a blob value:
```c
ValueBlob_t myBlobValueParameters =  { .name = "Value Blob Name",
                                       .type = "value type",
                                       .permission = READ_WRITE,
                                       .max = 200,
                                       .encoding = ""};

myBlobValue = myDevice->createBlobValue(&myBlobValueParameters);
```

### Create a xml value:
```c
ValueXml_t myXmlValueParameters =  { .name = "Value Xml Name",
                                     .type = "value type",
                                     .permission = READ_WRITE,
                                     .xsd = "test",
                                     .namespace = "test"};

myXmlValue = myDevice->createXmlValue(&myXmlValueParameters);
```


## Send a value report:
```c
int myInt = 123;
double myDouble = 42.7;

myNumberValue.report("987");    // You can send numbers as a string you format
myNumberValue.report(myInt);    // Report the number as an int
myNumberValue.report(myDouble); // Report the number as a double


myStringValue.report("Example string");

myBlobValue.report("A5FF2C");

```

## Set a control value:
Normally the control value will only be updated from wappsto.com, but you may need to set a current value when booting or in other situations.
Setting this is similar to report:
```c
int myInt = 123;
double myDouble = 42.7;

myNumberValue.control("987");    // You can send numbers as a string you format
myNumberValue.control(myInt);    // Set control as an int
myNumberValue.control(myDouble); // Set control as a double


myStringValue.control("Example string");

myBlobValue.control("A5FF2C");

```

## Add wappsto in your loop function
For Wappsto to be able to receive data (control, refresh, and pings), it has to be included in your Arduino loop function - if not you will never receive any callbacks with data, and the connection may experience a timeout.
It is also recommended not to block the loop for extended periods of time.
...
wappsto.dataAvailable();
...

## Receive a value control request:
You can choose if you want the control data translated to a double, or if you want the string directly.

```c
// Receive control with a number (double)
void controlNumberCallback(Value *value, double data, String timestamp)
{
    // handle control request
}

// Receive control with a string
void controlStringCallback(Value *value, String data, String timestamp)
{
    // handle control request
}

...
myNumberValue->onControl(&controlNumberCallback);
myStringValue->onControl(&controlStringCallback);
```

## Receive a value refresh request (report value only):
```c
void refreshNumberCallback(Value *value)
{
    // handle refresh request
}
...
myNumberValue->onRefresh(&refreshNumberCallback);

```

## Getting last data and timestamp from value
You can access the last received/send data and timestamp by using these functions.
For instance after a new boot, the last controlled state set in Wappsto.com will be retrieved, so your program and start with that state.

_Note_ retriveing numberData from an empty value will return a 0;

```c
String ctrlData = myNumberValue.getControlData();

double ctrlDataNumber = myNumberValue.getControlNumberData();

String ctrlTime = myNumberValue.getControlTimestamp();

String reportData = myNumberValue.getReportData();

double reportDataNumber = myNumberValue.getReportNumberData();

String reportTime = myNumberValue.getReportTimestamp();
```

For futher examples see the code in the example folder, or see 'File -> Examples -> Wappsto' in Arduino IDE.
