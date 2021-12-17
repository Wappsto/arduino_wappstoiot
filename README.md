# arduino_wappstoiot

A general rule that you should be aware of, is that that "name" for device and value object have extra functionality.

The library will ask Wappsto if an device/value exist with that name before creating it, so it will link to the right value. If nothing have this name a new will be created. If a parent have multiple children with the same name the library will choose the first available.

Because of this it is adviced not to give identical names to children, example. two devices under the same network, or two values under the same device.


## Initialize Wappsto:
Wappsto needs a reference to WiFiClientSecure when created, example.
```
WiFiClientSecure client;
Wappsto wappsto(&client);
```

## Configure Wappsto identity for your implementation
For your implementation to identify itself towards Wappsto it needs a network id, the server CA certificate, and the client certificate/key - all generated by Wappsto.

### Generate wappsto_config.h
As a help to generate this you can use the python script included in this repository in the folder generate_config_header. 
For a first time run, you may need to install the required libraries 
```
pip3 install -r requirements.txt
```
To generate a header file, use the following command - and copy the file wappsto_config.h to your Arduino sketch folder. For other commands check the --help parameter.
```
python3 main.py --type arduino
```


### Set the config and connect

```
#include "wappsto_config.h"
...
wappsto.config(network_uuid, ca, client_crt, client_key);
if(wappsto.connect()) {
    // Connected
} else {
    // Failed to connect
}
```

## Enable debug output from Wappsto
By setting this you can see debug info on the serial port from the library.
```
wappsto.setLog(true);
```

## Create your network:
```
myNetwork = wappsto.createNetwork("Network Name");
```

## Create a device:
```
DeviceDescription_t myDeviceDescription = {
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
See better description of values somewhere...

The PERMISSION_e parameter tells the library if it should create report and/or control for this value.
* READ -> report state
* WRITE -> control state
* READ_WRITE -> report and control state

The first time a value is created a number will have the value NA, and the string/blob will be empty, for both control and report.
If the value exist, the data of the value will not change. If you want to value to update when the device reboots, you have to call report/control.

### Create a number value:
```
ValueNumber_t myNumberValueParameters = {.min = 0, .max = 100, .step = 1, .unit = "", .si_conversion = ""};
myNumberValue = myDevice->createValueNumber("Value Number Name", "value type", READ_WRITE, &myNumberValueParameters);
```

### Create a string value:
```
ValueString_t myStringValueParameters = {.max = 200, .encoding = ""};
myStringValue = myDevice->createValueString("Value String Name", "value type", READ_WRITE, &myStringValueParameters);
```

### Create a blob value:
```
ValueBlob_t myBlobValueParameters = {.max = 200, .encoding = ""};
myBlobValue = myDevice->createValueBlob("Value Blob Name", "value type", READ_WRITE, &myBlobValueParameters);
```


## Send a value report:
```
int myInt = 123;
double myDouble = 42.7;

myNumberValue.report("987");    // You can send numbers as a string you format
myNumberValue.report(myInt);    // Report the number as an int
myNumberValue.report(myDouble); // Report the number as a double


myStringValue.report("Example string");

myBlobValue.report("A5FF2C");

```

## Set a control value:
Normally the control value will only be updated from "outside" through wappsto.com, but you may need to set a current value when booting or in other situations.
Setting this is similr to report:
```
int myInt = 123;
double myDouble = 42.7;

myNumberValue.control("987");    // You can send numbers as a string you format
myNumberValue.control(myInt);    // Set control as an int
myNumberValue.control(myDouble); // Set control as a double


myStringValue.control("Example string");

myBlobValue.control("A5FF2C");

```

## Receive a value control request:
You can choose if you want the control data translated to a double, or if you want the string directly.

```
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
```
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

```
String ctrlData = myNumberValue.getControlData();

double ctrlDataNumber = myNumberValue.getControlNumberData();

String ctrlTime = myNumberValue.getControlTimestamp();

String reportData = myNumberValue.getReportData();

double reportDataNumber = myNumberValue.getReportNumberData();

String reportTime = myNumberValue.getReportTimestamp();
```

