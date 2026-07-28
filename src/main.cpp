#include <Arduino.h>
#include "MecanumDrive.hpp"

#define DRIVE_SPEED 100

MecanumDrive robotDrive;

void setup() {
    Serial.begin(115200);
    delay(500);
    robotDrive.stop();
    Serial.println("mBot Mega: Serial Reciever Redo!");
    Serial.println("Kommandon: FRAM, BACK, VANSTER, HOGER, STOPP");
}

void processCommand(String cmd) {
    cmd.trim();
    cmd.toUpperCase();

    if (cmd == "FRAM") {
        robotDrive.moveForward(DRIVE_SPEED);
        Serial.println("Roboten kör framåt");
    } else if (cmd == "BACK") {
        robotDrive.moveBackward(DRIVE_SPEED);
        Serial.println("Roboten kör bakåt");
    } else if (cmd == "VANSTER") {
        robotDrive.rotateLeft(DRIVE_SPEED);
        Serial.println("Roboten roterar vänster");
    } else if (cmd == "HOGER") {
        robotDrive.rotateRight(DRIVE_SPEED);
        Serial.println("Roboten roterar höger");
    } else if (cmd == "STOPP") {
        robotDrive.stop();
        Serial.println("Roboten stoppad");
    } else {
        Serial.print("Okänt kommando: ");
        Serial.println(cmd);
    }
}

void loop() {
    if (Serial.available() > 0) {
        String incomingCommand = Serial.readStringUntil('\n');
        processCommand(incomingCommand);
    }
}

/*
#include <Arduino.h>
#include "MecanumDrive.hpp"
#include "CrashDetector.hpp"
#include "ProximitySensor.hpp"
#include "StatusLEDs.hpp"
#include "LineSensor.hpp" // <-- Lade till LineSensor-biblioteket!

// ==================== PIN-KONFIGURATION ====================
#define CRASH_LEFT_PIN A11
#define CRASH_RIGHT_PIN A12
#define PROXIMITY_LEFT_PIN A6
#define PROXIMITY_MIDDLE_PIN A7
#define PROXIMITY_RIGHT_PIN A8
#define LINE_LEFT_PIN A9          // <-- Ny pin för Vänster linjesensor
#define LINE_RIGHT_PIN A10        // <-- Ny pin för Höger linjesensor
#define STATUS_LED_LEFT_PIN A14
#define STATUS_LED_RIGHT_PIN A13

// ==================== HASTIGHETSKONSTANTER ====================
#define NORMAL_SPEED 100
#define SLOW_SPEED 50

// ==================== TIDSKONSTANTER ====================
#define CRASH_DETECTION_TIMEOUT 500      
#define PROXIMITY_CHECK_TIMEOUT 200      
#define LINE_CHECK_TIMEOUT 100           // ms mellan linje-checkar
#define SENSOR_LOOP_DELAY 50             

// ==================== ROTATION-TIMING ====================
#define ROTATE_90_DEGREES 900            
#define ROTATE_180_DEGREES 1800          

// ==================== ROBOT STATE-MACHINE ====================
enum RobotState {
    STATE_IDLE,
    STATE_DRIVING,
    STATE_CRASH_DETECTED,
    STATE_PROXIMITY_ALERT,
    STATE_LINE_DETECTED          // <-- Nytt state för linjerna
};

// ==================== GLOBALA OBJEKT ====================
MecanumDrive robotDrive;
CrashDetector crashDetector(CRASH_LEFT_PIN, CRASH_RIGHT_PIN);
ProximitySensor proximitySensor(PROXIMITY_LEFT_PIN, PROXIMITY_RIGHT_PIN, PROXIMITY_MIDDLE_PIN, 400);
StatusLEDs statusLEDs(STATUS_LED_LEFT_PIN, STATUS_LED_RIGHT_PIN);
LineSensor lineSensors(LINE_LEFT_PIN, LINE_RIGHT_PIN); // <-- Skapade linjesensor-objektet

// ==================== STATE-VARIABLER ====================
RobotState currentState = STATE_IDLE;
unsigned long lastCrashCheck = 0;
unsigned long lastProximityCheck = 0;
unsigned long lastLineCheck = 0;

void handleCrash(bool leftPressed, bool rightPressed) {
    robotDrive.stop();
    delay(500);
    robotDrive.moveBackward(SLOW_SPEED);
    delay(1000);
    robotDrive.stop();
    delay(500);
    
    if (leftPressed && rightPressed) {
        Serial.println("Krockdetektor: BÅDA sidor träffade - roterar 180°");
        statusLEDs.showCrash();
        robotDrive.rotateRight(100);
        delay(ROTATE_180_DEGREES);
    } else if (leftPressed) {
        Serial.println("Krockdetektor: VÄNSTER sida träffade - roterar 90° höger");
        // statusLEDs.showCrashLeft(); // Antar att funktionen finns i din StatusLEDs.hpp
        robotDrive.rotateRight(100);
        delay(ROTATE_90_DEGREES);
    } else if (rightPressed) {
        Serial.println("Krockdetektor: HÖGER sida träffade - roterar 90° vänster");
        // statusLEDs.showCrashRight();
        robotDrive.rotateLeft(100);
        delay(ROTATE_90_DEGREES);
    }
    
    robotDrive.stop();
    currentState = STATE_IDLE;
}

void updateProximitySensors() {
    bool leftDetected = proximitySensor.isLeftDetected();
    bool rightDetected = proximitySensor.isRightDetected();
    bool middleDetected = proximitySensor.isMiddleDetected();
    
    if (leftDetected || rightDetected || middleDetected) {
        currentState = STATE_PROXIMITY_ALERT;
    } else {
        if (currentState == STATE_PROXIMITY_ALERT) {
            currentState = STATE_DRIVING;
        }
    }
}

void updateLineSensors() {
    int rawLeft = lineSensors.readLeftRaw();   // Hämtar råvärdet (0-1023)
    int rawRight = lineSensors.readRightRaw(); // Hämtar råvärdet (0-1023)
    
    bool leftOnLine = lineSensors.isLeftOnLine();
    bool rightOnLine = lineSensors.isRightOnLine();

    // FELSÖKNINGS-UTSKRIFT: Berättar exakt vad roboten känner av
    Serial.print("Linje Vänster (A9): ");
    Serial.print(rawLeft);
    Serial.print(" | Linje Höger (A10): ");
    Serial.println(rawRight);

    if (leftOnLine || rightOnLine) {
        currentState = STATE_LINE_DETECTED;
        robotDrive.stop();
        Serial.print(">>> LINJE UPPTÄCKT! Vänster triggad: ");
        Serial.print(leftOnLine);
        Serial.print(" Höger triggad: ");
        Serial.println(rightOnLine);
        
        delay(1000); 
        currentState = STATE_IDLE;
    }
}
void setup() {
    Serial.begin(115200);
    delay(100);
    
    proximitySensor.begin();
    crashDetector.begin();
    statusLEDs.begin();
    lineSensors.begin(); // <-- Starta linjesensorerna
    
    Serial.println("System redo. Alla sensorer är aktiva!");
    statusLEDs.showIdle();
    currentState = STATE_IDLE;
}

void loop() {
    unsigned long now = millis();
    
    // 1. KROCKDETEKTOR (Högsta prioritet)
    if (now - lastCrashCheck >= CRASH_DETECTION_TIMEOUT) {
        lastCrashCheck = now;
        bool leftCrash = crashDetector.isLeftPressed();
        bool rightCrash = crashDetector.isRightPressed();
        if (leftCrash || rightCrash) {
            currentState = STATE_CRASH_DETECTED;
            handleCrash(leftCrash, rightCrash);
        }
    }
    
    // 2. LINJESENSORER (Näst högsta prioritet)
    if (now - lastLineCheck >= LINE_CHECK_TIMEOUT && currentState != STATE_CRASH_DETECTED) {
        lastLineCheck = now;
        updateLineSensors();
    }

    // 3. AVSTÅNDSSENSORER
    if (now - lastProximityCheck >= PROXIMITY_CHECK_TIMEOUT && 
        currentState != STATE_CRASH_DETECTED && 
        currentState != STATE_LINE_DETECTED) {
        lastProximityCheck = now;
        updateProximitySensors();
    }
    
    // ========== MOTORKONTROLL ==========
    if (currentState == STATE_CRASH_DETECTED || currentState == STATE_LINE_DETECTED) {
        // Logiken hanteras i respektive funktionsanrop (handleCrash / updateLineSensors)
    } else if (currentState == STATE_PROXIMITY_ALERT) {
        robotDrive.moveForward(SLOW_SPEED);
    } else if (currentState == STATE_DRIVING) {
        statusLEDs.showDriving();
        robotDrive.moveForward(NORMAL_SPEED);
    } else if (currentState == STATE_IDLE) {
        statusLEDs.showIdle();
        robotDrive.stop();
        
        // Ta bort kommentarstecknen nedan för att få roboten att köra automatiskt när den är idle
        // currentState = STATE_DRIVING; 
    }
    
    delay(SENSOR_LOOP_DELAY);
}
*/