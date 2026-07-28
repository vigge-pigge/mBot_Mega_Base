#include "LineSensor.hpp"

LineSensor::LineSensor(uint8_t leftPin, uint8_t rightPin)
    : pinLeft(leftPin), pinRight(rightPin) {}

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
    return readLeftRaw() < 500;
}

bool LineSensor::isRightOnLine() {
    return readRightRaw() < 500;
}