#ifndef LINE_SENSOR_HPP
#define LINE_SENSOR_HPP

#include <Arduino.h>

class LineSensor {
private:
    uint8_t pinLeft;
    uint8_t pinRight;

public:
    // Starttröskelvärde för linjesensorerna: 500
    LineSensor(uint8_t leftPin = A9, uint8_t rightPin = A10);

    void begin();

    // Läser de exakta analoga värdena (0-1023) från linjesensorerna
    int readLeftRaw();
    int readRightRaw();

    // Returnerar true om sensorn är över den svarta linjen
    bool isLeftOnLine();
    bool isRightOnLine();

};

#endif