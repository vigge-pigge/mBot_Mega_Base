#ifndef STATUS_LEDS_HPP
#define STATUS_LEDS_HPP

#include <Arduino.h>
#include <MeRGBLed.h>

class StatusLEDs {
private:
    MeRGBLed ledLeft;
    MeRGBLed ledRight;
    uint8_t pinLeft;
    uint8_t pinRight;

public:
    // A13 för vänster, A14 för höger. Fyra LEDs per sida.
    StatusLEDs(uint8_t leftPin = A14, uint8_t rightPin = A13, uint8_t ledsPerModule = 4);

    void begin();

    // Styr båda modulerna samtidigt
    void setColor(uint8_t r, uint8_t g, uint8_t b);

    // Styr enskild modul
    void setLeftColor(uint8_t r, uint8_t g, uint8_t b);
    void setRightColor(uint8_t r, uint8_t g, uint8_t b);

    void clear();

    // Färdiga statuslägen
    void showCrash();
    void showCrashLeft();
    void showCrashRight();
    void showDriving();
    void showIdle();
    void showProximityAlert();
    void showProximityAlertLeft();
    void showProximityAlertRight();
};

#endif