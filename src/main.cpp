#include <Arduino.h>
#include "MecanumDrive.hpp"
#include "CrashDetector.hpp"
#include "ProximitySensor.hpp"
#include "StatusLEDs.hpp"

MecanumDrive robotDrive;
CrashDetector crashDetector(A11, A12);
ProximitySensor proximitySensor(A6, A8, A7);
StatusLEDs statusLEDs(A14, A13);

void setup() {
    Serial.begin(115200);
    proximitySensor.begin();
    crashDetector.begin();
    statusLEDs.begin();
    Serial.println("--- mBot Mega Styrsystem: Drivning + Krockdetektor + Avståndssensor + LEDs Startat ---");
    statusLEDs.showIdle();
    delay(2000);
}

void loop() {
    int normalSpeed = 100; // Normal hastighet
    int slowSpeed = 50;   // Långsam hastighet när avståndssensorerna är aktiverade
    int currentSpeed = normalSpeed;

    // Säkerhetskontroll 1: Om någon av krockdetektorerna är aktiverade, stoppa roboten och backa lite
    bool leftDetected = crashDetector.isLeftPressed();
    bool rightDetected = crashDetector.isRightPressed();

    if (leftDetected || rightDetected) {
        Serial.print("Krockdetektor aktiverad! ");
        robotDrive.stop(); // Stoppa roboten omedelbart
        delay(500); // Vänta lite innan backning
        robotDrive.moveBackward(slowSpeed); // Backa lite för att undvika hinder
        delay(1000); // Backa i 1 sekund
        robotDrive.stop(); // Stoppa roboten efter backning
        delay(500); // Vänta lite innan nästa åtgärd
        if (leftDetected && rightDetected) {
            Serial.print("Båda switcharna tryckta. ");
            statusLEDs.showCrash();
            robotDrive.rotateRight(100); // Rotera åt 180 grader höger för att undvika hinder
            delay(1800); // Rotera i 1.8 sekunder
            robotDrive.stop();
        } else if (leftDetected) {
            Serial.print("Vänster switch tryckt. ");
            statusLEDs.showCrashLeft();
            robotDrive.rotateRight(100); // Rotera 90 grader åt höger för att undvika hinder
            delay(900); // Rotera i 0.9 sekunder
            robotDrive.stop();
        } else if (rightDetected) {
            Serial.print("Höger switch tryckt. ");
            statusLEDs.showCrashRight();
            robotDrive.rotateLeft(100); // Rotera 90 grader åt vänster för att undvika hinder
            delay(900); // Rotera i 0.9 sekunder
            robotDrive.stop();
        }
        Serial.println();
    } else {
        Serial.println("Ingen krockdetektor aktiverad.");
    }

    delay(200); // Läs av krockdetektorerna var 200 ms

    // Säkerhetskontroll 2: Om någon av avståndssensorerna är aktiverade, sänk hastigheten
    bool leftProximity = proximitySensor.isLeftDetected();
    bool rightProximity = proximitySensor.isRightDetected();
    bool middleProximity = proximitySensor.isMiddleDetected();

    if (leftProximity || rightProximity || middleProximity) {
        Serial.print("Avståndssensor aktiverad!");
        if (leftProximity) {
            Serial.print("Vänster sensor aktiverad.");
            if (!crashDetector.isLeftPressed()) { // Om ingen krockdetektor är aktiverad, visa varning
                statusLEDs.showProximityAlertLeft();
            }
        }
        if (rightProximity) {
            Serial.print("Höger sensor aktiverad.");
            if (!crashDetector.isRightPressed()) { // Om ingen krockdetektor är aktiverad, visa varning
                statusLEDs.showProximityAlertRight();
            }
        }
        if (middleProximity) {
            Serial.print("Mittensensor aktiverad.");
            if (!crashDetector.isLeftPressed() && !crashDetector.isRightPressed()) { // Om ingen krockdetektor är aktiverad, visa varning
                statusLEDs.showProximityAlert();
            }
        }
        Serial.println();

        currentSpeed = slowSpeed; // Sätt en lägre hastighet när avståndssensorerna är aktiverade
    } else {
        Serial.println("Ingen avståndssensor aktiverad.");
    }

    delay(200); // Läs av avståndssensorerna var 200 ms

    // LED-statusindikering

    // LED 1. Om vi kör
    if (currentSpeed > 0 && !leftProximity && !rightProximity && !middleProximity) {
        statusLEDs.showDriving();
    } else if (currentSpeed == 0){
        // LED 2. Om vi är stilla
        statusLEDs.showIdle();
    }

    delay(200); // Läs av LED-status var 200 ms

    robotDrive.moveForward(currentSpeed);
/*
    // 1. Framåt
    Serial.println("Kör framåt...");
    robotDrive.moveForward(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);
    
    // 2. Glid Vänster (Strafe)
    Serial.println("Glider åt vänster...");
    robotDrive.strafeLeft(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);

    // 3. Glid Höger (Strafe)
    Serial.println("Glider åt höger...");
    robotDrive.strafeRight(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);

    // 4. Rotera Vänster
    Serial.println("Roterar vänster...");
    robotDrive.rotateLeft(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);

    // 5. Rotera Höger
    Serial.println("Roterar höger...");
    robotDrive.rotateRight(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);

    // 6. Diagonal Fram Vänster
    Serial.println("Kör diagonalt framåt vänster...");
    robotDrive.diagonalFL(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);

    // 7. Diagonal Fram Höger
    Serial.println("Kör diagonalt framåt höger...");
    robotDrive.diagonalFR(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);

    // 8. Diagonal Bak Vänster
    Serial.println("Kör diagonalt bakåt vänster...");
    robotDrive.diagonalRL(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);

    // 9. Diagonal Bak Höger
    Serial.println("Kör diagonalt bakåt höger...");
    robotDrive.diagonalRR(currentSpeed);
    delay(2000);
    robotDrive.stop();
    delay(1000);

    // 10. Bakåt
    Serial.println("Kör bakåt...");
    robotDrive.moveBackward(currentSpeed);
    delay(2000);
    robotDrive.stop();

    delay(4000);
*/
}