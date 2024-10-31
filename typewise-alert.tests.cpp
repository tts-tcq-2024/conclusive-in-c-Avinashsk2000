// test_alert_handling.cpp
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "typewise-alert.h"

// Buffers for capturing outputs for controller and email alerts
char printfBuffer[256];

// Mock implementation of output functions
void mockOutputToController(unsigned short header, BreachType breachType) {
    snprintf(printfBuffer, sizeof(printfBuffer), "Header: %x, Breach Type: %d\n", header, breachType);
}

void mockOutputToEmail(const char* recipient, const char* message) {
    snprintf(printfBuffer, sizeof(printfBuffer), "To: %s\n%s\n", recipient, message);
}

// Test for `sendToController` using the mock output function
void test_sendToController() {
    // Substitute real output with mock function
    outputToController = mockOutputToController;

    // Test for TOO_LOW breach type
    sendToController(TOO_LOW);
    assert(strcmp(printfBuffer, "Header: feed, Breach Type: 0\n") == 0);

    // Test for TOO_HIGH breach type
    sendToController(TOO_HIGH);
    assert(strcmp(printfBuffer, "Header: feed, Breach Type: 2\n") == 0);
}

// Test for `sendToEmail` using the mock output function
void test_sendToEmail() {
    // Substitute real email output with mock function
    outputToEmail = mockOutputToEmail;

    // Test for NORMAL breach type
    sendToEmail(NORMAL);
    assert(strcmp(printfBuffer, "To: a.b@c.com\nHi, the temperature is normal\n") == 0);

    // Test for TOO_LOW breach type
    sendToEmail(TOO_LOW);
    assert(strcmp(printfBuffer, "To: a.b@c.com\nHi, the temperature is too low\n") == 0);

    // Test for TOO_HIGH breach type
    sendToEmail(TOO_HIGH);
    assert(strcmp(printfBuffer, "To: a.b@c.com\nHi, the temperature is too high\n") == 0);
}

// Test for `inferBreach` boundary cases
void test_inferBreach_BoundaryCases() {
    assert(inferBreach(5, 10, 20) == TOO_LOW);    // TOO_LOW case
    assert(inferBreach(15, 10, 20) == NORMAL);    // NORMAL within range
    assert(inferBreach(25, 10, 20) == TOO_HIGH);  // TOO_HIGH case
    assert(inferBreach(10, 10, 20) == NORMAL);    // Boundary check at lower limit
    assert(inferBreach(20, 10, 20) == NORMAL);    // Boundary check at upper limit
}

// Test `classifyTemperatureBreach` for each cooling type and boundary conditions
void test_classifyTemperatureBreach_CoolingTypes() {
    assert(classifyTemperatureBreach(PASSIVE_COOLING, -5) == TOO_LOW);
    assert(classifyTemperatureBreach(PASSIVE_COOLING, 25) == NORMAL);
    assert(classifyTemperatureBreach(PASSIVE_COOLING, 40) == TOO_HIGH);

    assert(classifyTemperatureBreach(HI_ACTIVE_COOLING, -5) == TOO_LOW);
    assert(classifyTemperatureBreach(HI_ACTIVE_COOLING, 30) == NORMAL);
    assert(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50) == TOO_HIGH);

    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, -5) == TOO_LOW);
    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, 30) == NORMAL);
    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, 45) == TOO_HIGH);
}

// Test `checkAndAlert` for different alert targets using mock functions
void test_checkAndAlert_Controller() {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;

    // Substitute real controller output with mock function
    outputToController = mockOutputToController;

    // Test low temperature case
    checkAndAlert(TO_CONTROLLER, batteryChar, -5);
    assert(strcmp(printfBuffer, "Header: feed, Breach Type: 0\n") == 0);  // Expect TOO_LOW alert

    // Test high temperature case
    checkAndAlert(TO_CONTROLLER, batteryChar, 40);
    assert(strcmp(printfBuffer, "Header: feed, Breach Type: 2\n") == 0);  // Expect TOO_HIGH alert
}

void test_checkAndAlert_Email() {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = HI_ACTIVE_COOLING;

    // Substitute real email output with mock function
    outputToEmail = mockOutputToEmail;

    // Test high temperature case
    checkAndAlert(TO_EMAIL, batteryChar, 46);
    assert(strcmp(printfBuffer, "To: a.b@c.com\nHi, the temperature is too high\n") == 0);  // Expect TOO_HIGH alert

    // Test normal temperature case
    checkAndAlert(TO_EMAIL, batteryChar, 30);
    assert(strcmp(printfBuffer, "To: a.b@c.com\nHi, the temperature is normal\n") == 0);  // Expect NORMAL alert
}
