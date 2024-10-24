#include <Arduino.h>
#include <WiFi.h>
#include <ESP32HomeKit.h>

/*
	This example provides a basic connection to HomeKit.
	You can't read/write values at the moment but it's coming I promise.
*/

void homekit_setup(void)
{

	HAPAccessory hapAccessory("ESP32 Fan");
	hapAccessory.Setup("EspFan01", "Espressif", "001122334455", "0.0.1", "1.0.0", HAP_ACCESSORY_SENSOR);

	HAPTemperatureSensor hapTemperatureSensor("Temperature", 10.0);
	hapAccessory.AddService(hapTemperatureSensor.Service);

	hapAccessory.Register();
	HAPCore hapCore("111-22-333", "ES32");
	hapCore.Start();
}

