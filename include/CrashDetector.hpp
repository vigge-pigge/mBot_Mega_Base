#ifndef CRASH_DETECTOR_HPP
#define CRASH_DETECTOR_HPP

#include <Arduino.h>

class CrashDetector {
private:
    uint8_t pinLeft;
    uint8_t pinRight;

public:
    // Standardportar
    CrashDetector(uint8_t leftPin = A11, uint8_t rightPin = A12);

    // Måste kallas i setup() för att initiera switcharna
    void begin();

    // Kollar enskilda switchar
    bool isLeftPressed();
    bool isRightPressed();

    // Returnerar true om någon av switcharna är aktiverad
    bool isCrashDetected();
};

#endif