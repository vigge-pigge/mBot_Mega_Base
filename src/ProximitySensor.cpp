#include <ProximitySensor.hpp>

ProximitySensor::ProximitySensor(uint8_t leftPin, uint8_t rightPin, uint8_t middlePin, int proximityThreshold)
    : pinLeft(leftPin), pinRight(rightPin), pinMiddle(middlePin), threshold(proximityThreshold) {}

void ProximitySensor::begin() {
    pinMode(pinLeft, INPUT);
    pinMode(pinRight, INPUT);
    pinMode(pinMiddle, INPUT);
}

int ProximitySensor::readLeftRaw() {
    return analogRead(pinLeft);
}

int ProximitySensor::readRightRaw() {
    return analogRead(pinRight);
}

int ProximitySensor::readMiddleRaw() {
    return analogRead(pinMiddle);
}

bool ProximitySensor::isLeftDetected() {
    // Returnerar true om vänster sensor läser värde under threshold (hinder närvarande)
    return readLeftRaw() < threshold;
}

bool ProximitySensor::isRightDetected() {
    // Returnerar true om höger sensor läser värde under threshold (hinder närvarande)
    return readRightRaw() < threshold;
}

bool ProximitySensor::isMiddleDetected() {
    // Returnerar true om mitten sensor läser värde under threshold (hinder närvarande)
    return readMiddleRaw() < threshold;
}

void ProximitySensor::setThreshold(int newThreshold) {
    threshold = newThreshold;
}