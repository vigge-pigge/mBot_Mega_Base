#ifndef LINE_SENSOR_HPP
#define LINE_SENSOR_HPP

#include <Arduino.h>

class LineSensor {
private:
    uint8_t pinLeft;
    uint8_t pinRight;
    int threshold; // Tröskelvärde för att avgöra om sensorn ser linjen

public:
    // Starttröskelvärde för linjesensorerna: 500
    LineSensor(uint8_t leftPin = A9, uint8_t rightPin = A10, int lineThreshold = 500);

    void begin();

    // Läser de exakta analoga värdena (0-1023) från linjesensorerna
    int readLeftRaw();
    int readRightRaw();

    // Returnerar true om sensorn är över den svarta linjen
    bool isLeftOnLine();
    bool isRightOnLine();

    // Ändra tröskelvärdet dynamiskt
    void setThreshold(int newThreshold);
};

#endif