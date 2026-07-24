#include "StatusLEDs.hpp"

StatusLEDs::StatusLEDs(uint8_t pinLeft, uint8_t pinRight, uint8_t ledsPerModule)
    : ledLeft(0, ledsPerModule), ledRight(0, ledsPerModule),
      pinLeft(pinLeft), pinRight(pinRight) {}

void StatusLEDs::begin() {
    ledLeft.setpin(pinLeft);
    ledRight.setpin(pinRight);
    clear();
}

void StatusLEDs::setLeftColor(uint8_t r, uint8_t g, uint8_t b) {
    ledLeft.setColor(0, r, g, b);
    ledLeft.show();
}

void StatusLEDs::setRightColor(uint8_t r, uint8_t g, uint8_t b) {
    ledRight.setColor(0, r, g, b);
    ledRight.show();
}

void StatusLEDs::setColor(uint8_t r, uint8_t g, uint8_t b) {
    setLeftColor(r, g, b);
    setRightColor(r, g, b);
}

void StatusLEDs::clear() {
    setColor(0, 0, 0);
}

void StatusLEDs::showCrash() {
    setColor(255, 0, 0); // Röd
}

void StatusLEDs::showCrashLeft() {
    setLeftColor(255, 0, 0); // Röd
    //setRightColor(0, 0, 0); // Avstängd
}

void StatusLEDs::showCrashRight() {
    //setLeftColor(0, 0, 0); // Avstängd
    setRightColor(255, 0, 0); // Röd
}

void StatusLEDs::showDriving() {
    setColor(0, 255, 0); // Grön
}

void StatusLEDs::showIdle() {
    setColor(0, 0, 255); // Blå
}

void StatusLEDs::showProximityAlert() {
    setColor(255, 255, 0); // Gul
}

void StatusLEDs::showProximityAlertLeft() {
    setLeftColor(255, 255, 0); // Gul
    //setRightColor(0, 0, 0); // Avstängd
}

void StatusLEDs::showProximityAlertRight() {
    //setLeftColor(0, 0, 0); // Avstängd
    setRightColor(255, 255, 0); // Gul
}