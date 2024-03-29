#ifndef SENSOR_HPP
    #define SENSOR_HPP

#include <Arduino.h>

class Sensor
{
    private:
        String name;
        byte pin;
        float data;

    public:
        Sensor();
        Sensor(String name, int pin);

        float getData();
        byte getPin();
        String getName();
        bool verificationNormalRange(int min, int max);
        void setData(float data);
};

#endif