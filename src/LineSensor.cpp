#include "LineSensor.hpp"

LineSensor::LineSensor(uint8_t leftPin, uint8_t rightPin, int lineThreshold)
    : pinLeft(leftPin), pinRight(rightPin), threshold(lineThreshold) {}

void LineSensor::begin() {
    pinMode(pinLeft, INPUT);
    pinMode(pinRight, INPUT);
}

int LineSensor::readLeftRaw() {
    return analogRead(pinLeft);
}

int LineSensor::readRightRaw() {
    return analogRead(pinRight);
}

bool LineSensor::isLeftOnLine() {
    return readLeftRaw() < threshold;
}

bool LineSensor::isRightOnLine() {
    return readRightRaw() < threshold;
}

void LineSensor::setThreshold(int newThreshold) {
    threshold = newThreshold;
}