#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "typewise-alert.h"

// Function pointers for mocking alert functions
void (*mockSendToController)(BreachType) = NULL;
void (*mockSendToEmail)(BreachType) = NULL;

// Buffer for capturing printed output
char printfBuffer[256];

// Mock printf function to capture output for testing
int mockPrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int printedLength = vsnprintf(printfBuffer, sizeof(printfBuffer), format, args);
    va_end(args);
    return printedLength;
}

// Mock functions for sendToController and sendToEmail
void mockSendToControllerFunc(BreachType breachType) {
    snprintf(printfBuffer, sizeof(printfBuffer), "Controller Alert: %d\n", breachType);
}

void mockSendToEmailFunc(BreachType breachType) {
    snprintf(printfBuffer, sizeof(printfBuffer), "Email Alert: %d\n", breachType);
}

// Test inferBreach for boundary cases
void test_inferBreach_BoundaryCases() {
    // Test TOO_LOW
    assert(inferBreach(5, 10, 20) == TOO_LOW);
    // Test NORMAL within range
    assert(inferBreach(15, 10, 20) == NORMAL);
    // Test TOO_HIGH
    assert(inferBreach(25, 10, 20) == TOO_HIGH);
    // Test at exact lower limit
    assert(inferBreach(10, 10, 20) == NORMAL);
    // Test at exact upper limit
    assert(inferBreach(20, 10, 20) == NORMAL);
}

// Test classifyTemperatureBreach with various cooling types and temperature ranges
void test_classifyTemperatureBreach_CoolingTypes() {
    // Testing PASSIVE_COOLING for TOO_LOW, NORMAL, and TOO_HIGH
    assert(classifyTemperatureBreach(PASSIVE_COOLING, -5) == TOO_LOW);
    assert(classifyTemperatureBreach(PASSIVE_COOLING, 25) == NORMAL);
    assert(classifyTemperatureBreach(PASSIVE_COOLING, 40) == TOO_HIGH);

    // Testing HI_ACTIVE_COOLING for TOO_LOW, NORMAL, and TOO_HIGH
    assert(classifyTemperatureBreach(HI_ACTIVE_COOLING, -5) == TOO_LOW);
    assert(classifyTemperatureBreach(HI_ACTIVE_COOLING, 30) == NORMAL);
    assert(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50) == TOO_HIGH);

    // Testing MED_ACTIVE_COOLING for TOO_LOW, NORMAL, and TOO_HIGH
    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, -5) == TOO_LOW);
    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, 30) == NORMAL);
    assert(classifyTemperatureBreach(MED_ACTIVE_COOLING, 45) == TOO_HIGH);
}

// Test for sending to controller using the mock function pointer
void test_sendToController_Mock() {
    // Set the mock function pointer and call
    mockSendToController = mockSendToControllerFunc;
    mockSendToController(TOO_LOW);
    assert(strcmp(printfBuffer, "Controller Alert: 0\n") == 0);
}

// Test for sending to email using the mock function pointer
void test_sendToEmail_Mock() {
    // Set the mock function pointer and call
    mockSendToEmail = mockSendToEmailFunc;
    mockSendToEmail(TOO_HIGH);
    assert(strcmp(printfBuffer, "Email Alert: 2\n") == 0);
}

// Test for checkAndAlert with mocked email function
void test_checkAndAlert_Email() {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = HI_ACTIVE_COOLING;

    // Set up mock functions
    mockSendToEmail = mockSendToEmailFunc;
    
    // Test high temperature case
    checkAndAlert(TO_EMAIL, batteryChar, 46);
    assert(strcmp(printfBuffer, "Email Alert: 2\n") == 0);  // Expect TOO_HIGH alert

    // Test normal temperature case
    checkAndAlert(TO_EMAIL, batteryChar, 30);
    assert(strcmp(printfBuffer, "Email Alert: 1\n") == 0);  // Expect NORMAL alert
}

// Test for checkAndAlert with mocked controller function
void test_checkAndAlert_Controller() {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;

    // Set up mock functions
    mockSendToController = mockSendToControllerFunc;

    // Test low temperature case
    checkAndAlert(TO_CONTROLLER, batteryChar, -5);
    assert(strcmp(printfBuffer, "Controller Alert: 0\n") == 0);  // Expect TOO_LOW alert
}
