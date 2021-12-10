# arduino_wappstoiot

## Initialize Wappsto:
```
Wappsto wappsto(&wifi secure client);
```

## Create your network:
```
myNetwork = wappsto.createNetwork("Network Name");
```

## Create a device:
```
myDevice = myNetwork->createDevice("Device Name", "Product Name", "Manufactorer", "Description", "Protocol", "Communication");
```

## Create a number value:
```
ValueNumber_t myNumberValueParameters = {.min = 0, .max = 100, .step = 1, .unit = "", .si_conversion = ""};
myNumberValue = myDevice->createValueNumber("Value Name", "value type", READ_WRITE, &myNumberValueParameters);
```

## Create a string value:
```
ValueString_t myStringValueParameters = {.max = 200, .encoding = ""};
myStringValue = myDevice->createValueString("Value Name", "value type", READ_WRITE, &myStringValueParameters);
```

## Send a value report:
```
myNumberValue.report("123");

myStringValue.report("Example string");
```

## Receive a value control request:
```
void controlNumberCallback(Value *value, String data, String timestamp)
{
    // handle control request
}
...
myNumberValue->onControl(&controlNumberCallback);
```

## Receive a value refresh request (report value):
```
void refreshNumberCallback(Value *value)
{
    // handle refresh request
}
...
myNumberValue->onRefresh(&refreshNumberCallback);

```
