#include <ProximitySensor.hpp>

ProximitySensor::ProximitySensor(uint8_t leftPin, uint8_t rightPin, uint8_t middlePin)
    : pinLeft(leftPin), pinRight(rightPin), pinMiddle(middlePin) {}

void ProximitySensor::begin() {
    pinMode(pinLeft, INPUT);
    pinMode(pinRight, INPUT);
    pinMode(pinMiddle, INPUT);
}

bool ProximitySensor::isLeftDetected() {
    // Kollar om vänster sensor är aktiverad
    return digitalRead(pinLeft) == LOW;
}

bool ProximitySensor::isRightDetected() {
    // Kollar om höger sensor är aktiverad
    return digitalRead(pinRight) == LOW;
}

bool ProximitySensor::isMiddleDetected() {
    // Kollar om mittensensor är aktiverad
    return digitalRead(pinMiddle) == LOW;
}