#include <CrashDetector.hpp>

CrashDetector::CrashDetector(uint8_t leftPin, uint8_t rightPin)
    : pinLeft(leftPin), pinRight(rightPin) {}

void CrashDetector::begin() {
    pinMode(pinLeft, INPUT);
    pinMode(pinRight, INPUT);
}

bool CrashDetector::isLeftPressed() {
    // Kollar om vänster switch är aktiverad (LOW betyder att den är tryckt)
    return digitalRead(pinLeft) == LOW;
}

bool CrashDetector::isRightPressed() {
    // Kollar om höger switch är aktiverad (LOW betyder att den är tryckt)
    return digitalRead(pinRight) == LOW;
}

bool CrashDetector::isCrashDetected() {
    return isLeftPressed() || isRightPressed();
}