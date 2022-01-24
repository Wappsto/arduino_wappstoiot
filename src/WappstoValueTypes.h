
#include "Wappsto.h"

ValueNumber_t defaultTemperatureParameter = { \
    .name = "Temperature",
    .type = "temperature",
    .permission = READ,
    .min = -20,
    .max = 110,
    .step = 0.1,
    .unit = "°C",
    .si_conversion = ""
};

ValueNumber_t defaultLatitudeParameter = { \
    .name = "Latitude",
    .type = "latitude",
    .permission = READ,
    .min = -90,
    .max = 90,
    .step = 0.000001,
    .unit = "°N",
    .si_conversion = ""
};

ValueNumber_t defaultLongitudeParameter = { \
    .name = "Longitude",
    .type = "longitude",
    .permission = READ,
    .min = -180,
    .max = 180,
    .step = 0.000001,
    .unit = "°E",
    .si_conversion = ""
};

ValueNumber_t defaultBooleanParameter = { \
    .name = "Boolean",
    .type = "boolean",
    .permission = READ,
    .min = 0,
    .max = 1,
    .step = 1,
    .unit = "",
    .si_conversion = ""
};

ValueBlob_t defaultColorParameter = { \
    .name = "Color",
    .type = "color",
    .permission = READ_WRITE,
    .max = 8,
    .encoding = "color_int"
};
