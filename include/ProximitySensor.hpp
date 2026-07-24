#ifndef PROXIMITY_SENSOR_HPP
#define PROXIMITY_SENSOR_HPP

#include <Arduino.h>

class ProximitySensor {
private:
    uint8_t pinLeft;
    uint8_t pinRight;
    uint8_t pinMiddle;

public:
    // Standardportar
    ProximitySensor(uint8_t leftPin = A6, uint8_t rightPin = A8, uint8_t middlePin = A7);

    // Måste kallas i setup() för att initiera sensorerna
    void begin();

    // Kollar enskilda sensorer
    bool isLeftDetected();
    bool isRightDetected();
    bool isMiddleDetected();
};

#endif