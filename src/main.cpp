#include <Arduino.h>
#include "MecanumDrive.hpp"
#include "CrashDetector.hpp"
#include "ProximitySensor.hpp"
#include "StatusLEDs.hpp"

// ==================== PIN-KONFIGURATION ====================
#define CRASH_LEFT_PIN A11
#define CRASH_RIGHT_PIN A12
#define PROXIMITY_LEFT_PIN A6
#define PROXIMITY_MIDDLE_PIN A7
#define PROXIMITY_RIGHT_PIN A8
#define STATUS_LED_LEFT_PIN A14
#define STATUS_LED_RIGHT_PIN A13

// ==================== TIDSKONSTANTER ====================
#define WATCHDOG_MS 300        // Inget kommando pa 300 ms -> nodstopp
#define CRASH_BACKUP_MS 600    // Backtid efter krock
#define CRASH_ROTATE_MS 500    // Rotationstid efter krock
#define MAX_FORWARD_PROX 40    // Max framhastighet nar avstandssensor reagerar

// ==================== OBJEKT ====================
MecanumDrive robotDrive;
CrashDetector crashDetector(CRASH_LEFT_PIN, CRASH_RIGHT_PIN);
ProximitySensor proximitySensor(PROXIMITY_LEFT_PIN, PROXIMITY_RIGHT_PIN, PROXIMITY_MIDDLE_PIN, 400);
StatusLEDs statusLEDs(STATUS_LED_LEFT_PIN, STATUS_LED_RIGHT_PIN);

// ==================== TILLSTAND ====================
String serialBuffer = "";
unsigned long lastCommandTime = 0;
bool commandReceived = false;   // Watchdogen aktiveras forst efter forsta kommandot

// Tolkar ett komplett kommando som avslutats med radbrytning
void handleCommand(String cmd) {
    cmd.trim();
    if (cmd.length() == 0) return;

    if (cmd == "STOPP") {
        robotDrive.stop();
        statusLEDs.showIdle();
        Serial.println("OK: STOPP");
    }
    else if (cmd.startsWith("M ")) {
        int s1 = cmd.indexOf(' ');
        int s2 = cmd.indexOf(' ', s1 + 1);
        if (s2 == -1) { Serial.println("ERR: anvandning M <vanster> <hoger>"); return; }

        int l = cmd.substring(s1 + 1, s2).toInt();
        int r = cmd.substring(s2 + 1).toInt();
        l = constrain(l, -255, 255);
        r = constrain(r, -255, 255);

        // Sakkerhet: bromsa framfarten om avstandssensorerna ser nagot
        if (proximitySensor.isLeftDetected() || proximitySensor.isMiddleDetected()
            || proximitySensor.isRightDetected()) {
            if (l > MAX_FORWARD_PROX) l = MAX_FORWARD_PROX;
            if (r > MAX_FORWARD_PROX) r = MAX_FORWARD_PROX;
        }

        // Mecanum-mappning (se driveVector i MecanumDrive.cpp):
        // positiva varden = framAT. Funktion: l=r=100 <=> moveForward(100)
        int vy    = -(l + r) / 2;
        int omega = (r - l) / 2;
        robotDrive.driveVector(vy, 0, omega);
        statusLEDs.showDriving();
    }
    else {
        Serial.println("ERR: okant kommando: " + cmd);
        return;   // okant kommando raknas inte som livstecken
    }

    lastCommandTime = millis();
    commandReceived = true;
}

void setup() {
    Serial.begin(115200);

    crashDetector.begin();
    proximitySensor.begin();
    statusLEDs.begin();
    robotDrive.stop();
    statusLEDs.showIdle();

    delay(500);
    Serial.println("MEGAPI_READY");
}

void loop() {
    // 1. LAS SERIEKOMMANDON (rad för rad)
    while (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '\n') {
            handleCommand(serialBuffer);
            serialBuffer = "";
        } else if (c != '\r') {
            serialBuffer += c;
        }
    }

    // 2. KROCKDETEKTOR – hogsta prioritet, overstyr alltid Pi:ns kommandon
    if (crashDetector.isLeftPressed() || crashDetector.isRightPressed()) {
        bool leftHit = crashDetector.isLeftPressed();
        Serial.println("ALARM: CRASH_DETECTED");
        robotDrive.stop();
        statusLEDs.showCrash();
        delay(300);
        robotDrive.moveBackward(50);
        delay(CRASH_BACKUP_MS);
        if (leftHit) { robotDrive.rotateRight(80); } else { robotDrive.rotateLeft(80); }
        delay(CRASH_ROTATE_MS);
        robotDrive.stop();
        statusLEDs.showIdle();

        // Töm ev. gamla kommandon som hunnit samlas under manövern
        serialBuffer = "";
        while (Serial.available() > 0) Serial.read();
        commandReceived = false;   // kraver ett nytt kommando innan karning igen
    }

    // 3. WATCHDOG – tystnad fran Pi:nn = nodstopp
    if (commandReceived && (millis() - lastCommandTime > WATCHDOG_MS)) {
        robotDrive.stop();
        statusLEDs.showIdle();
        commandReceived = false;
    }

    delay(10);
}