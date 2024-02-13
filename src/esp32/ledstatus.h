#ifndef LEDSTATUS_H
#define LEDSTATUS_H

class LedStatusClass{
protected:
    uint8_t _state;
public:
    LedStatusClass(void);
    void setState(uint8_t state);
    void loop(void);
};

extern LedStatusClass LedStatus;

#endif