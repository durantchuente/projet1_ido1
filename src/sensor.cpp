#include "sensor.hpp"

Sensor::Sensor(){};
Sensor::Sensor(String name, int pin){
    this->name = name;
    this->pin = pin;
};

float Sensor::getData(){
    return this->data;
};

byte Sensor::getPin(){
    return this->pin;
};

bool Sensor::verificationNormalRange(int min, int max){
    if (data>=min && data<=max)
        return true;
    return false;
}
String Sensor::getName(){
    return this->name;
};
void Sensor::setData(float data){
    this->data = data;
};