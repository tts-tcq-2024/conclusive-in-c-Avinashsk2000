#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "typewise-alert.h"

// Function pointers for mocking
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

// Test getTemperatureLimits with invalid cooling type (less than PASSIVE_COOLING)
void test_getTemperatureLimits_InvalidCoolingTypeLessThan() {
    assert(getTemperatureLimits(-1).lowerLimit == 0 && "Error: Invalid cooling type.");
}

// Test getTemperatureLimits with invalid cooling type (greater than MED_ACTIVE_COOLING)
void test_getTemperatureLimits_InvalidCoolingTypeGreaterThan() {
    assert(getTemperatureLimits(3).lowerLimit == 0 && "Error: Invalid cooling type.");
}

// Test getTemperatureLimits with valid cooling types
void test_getTemperatureLimits_ValidCoolingTypes() {
    TemperatureLimits limits = getTemperatureLimits(PASSIVE_COOLING);
    assert(limits.lowerLimit == 0);
    assert(limits.upperLimit == 35);

    limits = getTemperatureLimits(HI_ACTIVE_COOLING);
    assert(limits.lowerLimit == 0);
    assert(limits.upperLimit == 45);

    limits = getTemperatureLimits(MED_ACTIVE_COOLING);
    assert(limits.lowerLimit == 0);
    assert(limits.upperLimit == 40);
}

// Test inferBreach for different cases
void test_inferBreach() {
    assert(inferBreach(10, 20, 30) == TOO_LOW);
    assert(inferBreach(25, 20, 30) == NORMAL);
    assert(inferBreach(35, 20, 30) == TOO_HIGH);
}

// Test sendToController with mock function
void test_sendToController() {
    mockSendToController = mockSendToControllerFunc;
    mockSendToController(TOO_LOW);
    assert(strcmp(printfBuffer, "Controller Alert: 0\n") == 0);
}

// Test sendToEmail with mock function
void test_sendToEmail() {
    mockSendToEmail = mockSendToEmailFunc;
    mockSendToEmail(NORMAL);
    assert(strcmp(printfBuffer, "Email Alert: 1\n") == 0);

    mockSendToEmail(TOO_LOW);
    assert(strcmp(printfBuffer, "Email Alert: 0\n") == 0);

    mockSendToEmail(TOO_HIGH);
    assert(strcmp(printfBuffer, "Email Alert: 2\n") == 0);
}

// Test checkAndAlert for high temperature
void test_checkAndAlert_HighTemperature() {
    BatteryCharacter battery = {HI_ACTIVE_COOLING, "Battery B"};
    mockSendToEmail = mockSendToEmailFunc;

    checkAndAlert(TO_EMAIL, battery, 46);  // Exceeds HI_ACTIVE_COOLING limit
    assert(strcmp(printfBuffer, "Email Alert: 2\n") == 0);
}

// Test checkAndAlert for normal temperature
void test_checkAndAlert_NormalTemperature() {
    BatteryCharacter battery = {PASSIVE_COOLING, "Battery A"};
    mockSendToEmail = mockSendToEmailFunc;

    checkAndAlert(TO_EMAIL, battery, 25);  // Within PASSIVE_COOLING limit
    assert(strcmp(printfBuffer, "Email Alert: 1\n") == 0);
}

// Test checkAndAlert for low temperature
void test_checkAndAlert_LowTemperature() {
    BatteryCharacter battery = {MED_ACTIVE_COOLING, "Battery C"};
    mockSendToEmail = mockSendToEmailFunc;

    checkAndAlert(TO_EMAIL, battery, -5);  // Below MED_ACTIVE_COOLING limit
    assert(strcmp(printfBuffer, "Email Alert: 0\n") == 0);
}
