
#pragma once

#include "TempControl.h"
#include "BPLSettings.h"

class ParasiteTempController
{
public:
    ParasiteTempController(){}
    void run();
    void init();

    String getSettings(void);
    bool updateSettings(String json);
    
    void setTemperatureRange(float lower,float upper);

    char getMode();
    uint32_t getTimeElapsed();

    // the following interface are for web(JSON)
    int getTemp(){ return(int)(_currentTemp * 100.0);}
    int getLowerBound(){return(int)(_settings->setTemp * 100.0);}
    int getUpperBound(){return(int)(_settings->maxIdleTemp * 100.0);}

    float getTemperature(){ return _currentTemp;}
    float getSetTemperature(){return _settings->setTemp;}

    static Actuator *cooler;

protected:
    ParasiteTempControlSettings *_settings;
    
    bool _validSetting;
    float _currentTemp;
    uint32_t _lastSwitchedTime;
    uint32_t _lastSensorValidTime;

    void _setCooling(bool cool);
    bool checkSettings(void);
};

extern ParasiteTempController parasiteTempController;
