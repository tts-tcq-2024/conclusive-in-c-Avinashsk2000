#include "typewise-alert.h"
#include <stdio.h>

// Helper function to output to the controller (testable)
void outputToController(unsigned short header, BreachType breachType) {
    printf("Header: %x, Breach Type: %d\n", header, breachType);
}

// Function to send an alert to the controller by calling the helper
void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    outputToController(header, breachType);
}

// Function to send an alert via email
void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    const char* messages[] = {
        "Hi, the temperature is normal",     // NORMAL (0)
        "Hi, the temperature is too low",    // TOO_LOW (1)
        "Hi, the temperature is too high"    // TOO_HIGH (2)
    };
    if (breachType >= NORMAL && breachType <= TOO_HIGH) {
        printf("To: %s\n", recipient);
        printf("%s\n", messages[breachType]);
    } else {
        fprintf(stderr, "Error: Invalid breach type for email notification.\n");
    }
}

// Function to handle alerts based on the target
void handleAlert(AlertTarget alertTarget, BreachType breachType) {
    if (alertTarget == TO_CONTROLLER) {
        sendToController(breachType);
    } else if (alertTarget == TO_EMAIL) {
        sendToEmail(breachType);
    } else {
        fprintf(stderr, "Error: Invalid alert target.\n");
    }
}

// Function to check and alert based on temperature and cooling type
void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    handleAlert(alertTarget, breachType);
}
