#ifndef HumiditySensor_H
#define HumiditySensor_H
#include "Brewpi.h"
#include "EnvironmentSensor.h"


class HumiditySensor
{
public:
    HumiditySensor(int8_t cal=0){
        _hCal =cal;
    }
    virtual ~HumiditySensor(){}
    virtual unsigned char humidity(){
        return 0xFF;
    }
protected:
    int8_t _hCal;
};
extern HumiditySensor nullHumiditySensor;

class HumiditySensorEnvironment:public HumiditySensor{
public:
    HumiditySensorEnvironment(EnvironmentSensor* sensor,int8_t cal=0):HumiditySensor(cal){
        _sensor=sensor;
    }
  
    virtual ~HumiditySensorEnvironment(){}
    unsigned char humidity(){
        if(!_sensor->isConnected()){
            return 0xFF;
        }
        uint8_t h = _sensor->humidity();
        return (uint8_t) (h + _hCal);

    }
protected:
    EnvironmentSensor *_sensor;
};

#endif