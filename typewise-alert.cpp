#include "typewise-alert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Temperature limits lookup table for each cooling type
const TemperatureLimits coolingLimits[] = {
    {0, 35},  // PASSIVE_COOLING
    {0, 45},  // HI_ACTIVE_COOLING
    {0, 40}   // MED_ACTIVE_COOLING
};

// Function to retrieve temperature limits based on cooling type
TemperatureLimits getTemperatureLimits(CoolingType coolingType) {
    if (coolingType < PASSIVE_COOLING || coolingType > MED_ACTIVE_COOLING) {
        fprintf(stderr, "Error: Invalid cooling type.\n");
        exit(EXIT_FAILURE);
    }
    return coolingLimits[coolingType];
}

// Function to infer breach type based on limits
BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if (value < lowerLimit) {
        return TOO_LOW;
    }
    if (value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

// Function to classify temperature breach based on cooling type and temperature
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    TemperatureLimits limits = getTemperatureLimits(coolingType);
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

// Function to send alert to the controller
void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("Header: %x, Breach Type: %d\n", header, breachType);
}

// Function to send alert via email
void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    const char* messages[] = {
        "Hi, the temperature is normal",   // NORMAL (0)
        "Hi, the temperature is too low",  // TOO_LOW (1)
        "Hi, the temperature is too high"  // TOO_HIGH (2)
    };

    // Check that breachType is within bounds of the messages array
    if (breachType >= NORMAL && breachType <= TOO_HIGH) {
        printf("To: %s\n", recipient);
        printf("%s\n", messages[breachType]);
    } else {
        fprintf(stderr, "Error: Invalid breach type for email notification.\n");
    }
}

// Function to handle alert based on alert target
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

// Example usage
int main() {
    // Simulate a battery character
    BatteryCharacter batteryChar;
    batteryChar.coolingType = HI_ACTIVE_COOLING;
    strncpy(batteryChar.brand, "SampleBrand", sizeof(batteryChar.brand) - 1);
    batteryChar.brand[sizeof(batteryChar.brand) - 1] = '\0'; // Ensure null termination

    // Simulate temperature readings
    double temperatureInC = 50.0; // Example temperature
    AlertTarget alertTarget = TO_EMAIL; // Example alert target

    // Check and send alert
    checkAndAlert(alertTarget, batteryChar, temperatureInC);
    return 0;
}
