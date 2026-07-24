#ifndef MECANUM_DRIVE_HPP
#define MECANUM_DRIVE_HPP

#include <Arduino.h>
#include <MeMegaPiDCMotor.h>

class MecanumDrive {
private:
    MeMegaPiDCMotor motorFL; // Fram Vänster
    MeMegaPiDCMotor motorFR; // Fram Höger
    MeMegaPiDCMotor motorRL; // Bak Vänster
    MeMegaPiDCMotor motorRR; // Bak Höger

public:
    // Vi använder de virtuella portarna 9 och 10 för att komma åt "Slot 2"
    MecanumDrive(uint8_t portFL = 10, uint8_t portFR = 1, 
                 uint8_t portRL = 2, uint8_t portRR = 9);

    void moveForward(int speed);
    void moveBackward(int speed);
    void strafeLeft(int speed);
    void strafeRight(int speed);
    void rotateLeft(int speed);
    void rotateRight(int speed);
    void diagonalFL(int speed);
    void diagonalFR(int speed);
    void diagonalRL(int speed);
    void diagonalRR(int speed);
    void stop();

    void driveVector(int vy, int vx, int omega);
};

#endif
