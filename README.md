# BrewPiLessGx

BrewPiLessGx is [BrewPiLess](https://github.com/vitotai/BrewPiLess) running on ESP32_2432S032C, which is a ESP32 with a touch enabled TFT. Built on top of [lvgl](https://lvgl.io), flexbile display and touch based GUI are implemented.

# Feature

- TFT with touch enabled
    - Set control mode and temperature on the screen.
    - Select and connect Network on the screen.
- Customized display. Display information you need.
    - Two built-in skin available. one for simple BrewPi information, and the other have all available information.
    - It can be configured to show only hydrometer information, thus used as a iSpindel, Pill ,or Tilt reapeater.

# Screen
The UI screens
- Main infomraton screen
- Temperature Control
    to set control mode and temperature.
- Network Settings
    to view current settings, to enable/disable AP mode, an to connect to a WiFi network.

# Hardware
The hardware is a 3.2" smart display type 24320S032C, bought from [Aliexpress](https://www.aliexpress.com/item/1005005138982767.html).

- 240x320 16bit TFT, driven by ILI9431
- Capative touch, GT911 based.
- Three hardware pins available, they are
    - 22: OneWire for DS18B20
    - 21: actuator. Cooling/Heating
    - 35: Pressure transducer

If more actuators are needed, DS2413 based one-wire relays might be an option.


Most functionalities of BrewPiLess are supported, but due to limitation of the hardware not all of them are available.


# Difference between BrewPiLess

- No OTA update, due to memory limitation
- the filemanager is access at standard port
- NO traditional I2C LCD support

# Operation
The main display is customizable, defined by a JSON file. The syntax and format is described in the [document](doc/skin.md).

There are two built-in skins. The first one is "brewpi" for who just use traditional BrewPi functionality. 

![BrewPi Screen](doc/img/main_brewpi.jpg)

To access network control, tap the bars of wireless icons at the right top corner to bring this screen. Information of network settings are shown. Basic control, including select a WiFi network, are available. Guessing the IP address is no longer necessary.

![Network Setting](doc/img/network_setting.jpg)

To setup control mode and target temperature, tap the "Target" string. 

![Temperature Control](doc/img/tempcontrol.jpg).

The second built-in skin provides all available information of BrewPiLess.

![Information Screen](doc/img/main_information.jpg)

Tap the same wireless icon for accessing network settings, and the area of BeerTemp, BeeerSet, FridgeTemp, and FridgeSet for Temperature control.

To input Original Gravity, tap the area of OG, ATT, and ABV. To input current gravity, tap around the gravity.

The main screen and tapping area are all customizable. For example, display only information of Pill to serve as a Pill gateway is possible.

![Pill Information](doc/img/main_pill.jpg)