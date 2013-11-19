/*
    FILE:       idDHTLib.h
    VERSION:    0.0.3
    PURPOSE:    Interrupt driven Lib for DHT11 and DHT22 for mbed.
    LICENCE:    GPL v3 (http://www.gnu.org/licenses/gpl.html)
    DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
    DATASHEET: http://www.adafruit.com/datasheets/DHT22.pdf
    
    Based on idDHT11 library: https://github.com/niesteszeck/idDHT11
    Based on DHTLib library: http://playground.arduino.cc/Main/DHTLib
    Based on code proposed: http://forum.arduino.cc/index.php?PHPSESSID=j6n105kl2h07nbj72ac4vbh4s5&topic=175356.0
    Mbed port of Arduino library: https://github.com/niesteszeck/idDHTLib
    
    Changelog:
        v 0.0.1
            fork from idDHT11 lib
            change names to idDHTLib
            added DHT22 functionality
        v 0.0.2
            Optimization on shift var (pylon from Arduino Forum)
        v 0.0.3
            Timing correction to finally work properly on DHT22
            (Dessimat0r from Arduino forum)
 */

#ifndef idDHTLib_H__
#define idDHTLib_H__

#define IDDHTLIB_VERSION "0.0.3"
#include "mbed.h"

// state codes
#define IDDHTLIB_OK         0
#define IDDHTLIB_ACQUIRING      1
#define IDDHTLIB_ACQUIRED       2
#define IDDHTLIB_RESPONSE_OK        3

// error codes
#define IDDHTLIB_ERROR_CHECKSUM     -1
#define IDDHTLIB_ERROR_TIMEOUT      -2
#define IDDHTLIB_ERROR_ACQUIRING    -3
#define IDDHTLIB_ERROR_DELTA        -4
#define IDDHTLIB_ERROR_NOTSTARTED   -5

#define IDDHTLIB_CHECK_STATE        if(state == STOPPED)            \
                        return status;          \
                    else if(state != ACQUIRED)      \
                        return IDDHTLIB_ERROR_ACQUIRING;
                                    
class idDHTLib
{
public:
    idDHTLib(PinName pin, void (*isrCallback_wrapper)());
    void init(PinName pin, void (*isrCallback_wrapper)());
    void dht11Callback();
    void dht22Callback();
    int acquire();
    int acquireAndWait();
    float getCelsius();
    float getFahrenheit();
    float getKelvin();
    double getDewPoint();
    double getDewPointSlow();
    float getHumidity();
    bool acquiring();
    int getStatus();
    
private:
    Timer t;
    void (*isrCallback_wrapper)(void);
    pFunctionPointer_t  handler;
    enum states{RESPONSE=0,DATA=1,ACQUIRED=2,STOPPED=3,ACQUIRING=4};
    volatile states state;
    volatile int status;
    volatile char bits[5];
    volatile char cnt;
    volatile char idx;
//    volatile int us;
    
    DigitalInOut inOut;
    InterruptIn intIn;
    PinName pin;
    volatile float hum;
    volatile float temp;
    void isrCallback(bool dht22);
};
#endif // idDHTLib_H__