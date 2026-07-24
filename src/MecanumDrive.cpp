#include "MecanumDrive.hpp"
#include <MeMegaPi.h>

// Initierar motorerna med bara ett port-värde per motor
MecanumDrive::MecanumDrive(uint8_t portFL, uint8_t portFR, 
                           uint8_t portRL, uint8_t portRR)
    : motorFL(portFL),
      motorFR(portFR),
      motorRL(portRL),
      motorRR(portRR) {}

void MecanumDrive::moveForward(int speed) {
    motorFL.run(-speed);
    motorFR.run(speed);
    motorRL.run(-speed);
    motorRR.run(speed);
}

void MecanumDrive::moveBackward(int speed) {
    moveForward(-speed);
}

void MecanumDrive::strafeLeft(int speed) {
    motorFL.run(speed);
    motorFR.run(speed);
    motorRL.run(-speed);
    motorRR.run(-speed);
}

void MecanumDrive::strafeRight(int speed) {
    motorFL.run(-speed);
    motorFR.run(-speed);
    motorRL.run(speed);
    motorRR.run(speed);
}

void MecanumDrive::rotateLeft(int speed) {
    motorFL.run(speed);
    motorFR.run(speed);
    motorRL.run(speed);
    motorRR.run(speed);
}

void MecanumDrive::rotateRight(int speed) {
    motorFL.run(-speed);
    motorFR.run(-speed);
    motorRL.run(-speed);
    motorRR.run(-speed);
}

void MecanumDrive::diagonalFL(int speed) {
    motorFL.run(0);
    motorFR.run(speed);
    motorRL.run(-speed);
    motorRR.run(0);
}

void MecanumDrive::diagonalFR(int speed) {
    motorFL.run(-speed);
    motorFR.run(0);
    motorRL.run(0);
    motorRR.run(speed);
}

void MecanumDrive::diagonalRL(int speed) {
    motorFL.run(speed);
    motorFR.run(0);
    motorRL.run(0);
    motorRR.run(-speed);
}

void MecanumDrive::diagonalRR(int speed) {
    motorFL.run(0);
    motorFR.run(-speed);
    motorRL.run(speed);
    motorRR.run(0);
}

void MecanumDrive::stop() {
    motorFL.stop();
    motorFR.stop();
    motorRL.stop();
    motorRR.stop();
}

void MecanumDrive::driveVector(int vy, int vx, int omega) {
    int speedFL =  vy + vx + omega;
    int speedFR = -vy + vx + omega;
    int speedRL =  vy - vx + omega;
    int speedRR = -vy - vx + omega;

    speedFL = constrain(speedFL, -255, 255);
    speedFR = constrain(speedFR, -255, 255);
    speedRL = constrain(speedRL, -255, 255);
    speedRR = constrain(speedRR, -255, 255);

    motorFL.run(speedFL);
    motorFR.run(speedFR);
    motorRL.run(speedRL);
    motorRR.run(speedRR);
}
