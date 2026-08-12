#ifndef PROXIMITY_SENSOR_HPP
#define PROXIMITY_SENSOR_HPP

#include <Arduino.h>

class ProximitySensor {
private:
    uint8_t pinLeft;
    uint8_t pinRight;
    uint8_t pinMiddle;
    const int threshold; // Fast tröskelvärde för att avgöra om hinder är närvarande

public:
    // Standardportar. Starttröskelvärde: 400
    ProximitySensor(uint8_t leftPin = A6, uint8_t rightPin = A8, uint8_t middlePin = A7, int proximityThreshold = 400);

    // Måste kallas i setup() för att initiera sensorerna
    void begin();

    // Läser de exakta analoga värdena (0-1023) från avståndssensorerna
    int readLeftRaw();
    int readRightRaw();
    int readMiddleRaw();

    // Kollar enskilda sensorer (TRUE om hinder detekteras)
    bool isLeftDetected();
    bool isRightDetected();
    bool isMiddleDetected();
};

#endif