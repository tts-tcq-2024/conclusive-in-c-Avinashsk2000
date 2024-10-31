#include "typewise-alert.h"
#include <cstdio>

// Array defining limits for each cooling type
TemperatureLimits coolingLimits[] = {
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

// Function to infer breach level based on temperature and limits
BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if (value < lowerLimit) {
        return TOO_LOW;
    }
    if (value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

// Function to send alert to the controller
void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("Header: %x, Breach Type: %d\n", header, breachType);
}

// Function to send alert via email
void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    if (breachType == TOO_LOW) {
        printf("To: %s\nHi, the temperature is too low\n", recipient);
    } else if (breachType == TOO_HIGH) {
        printf("To: %s\nHi, the temperature is too high\n", recipient);
    } else {
        printf("To: %s\nTemperature is normal\n", recipient);
    }
}

// Function to handle alerts based on alert target and breach type
void handleAlert(AlertTarget target, BreachType breachType) {
    switch (target) {
        case TO_CONTROLLER:
            sendToController(breachType);
            break;
        case TO_EMAIL:
            sendToEmail(breachType);
            break;
        default:
            fprintf(stderr, "Error: Invalid alert target.\n");
            exit(EXIT_FAILURE);
    }
}

// Main alert check function that verifies limits and triggers alerts
void checkAndAlert(AlertTarget target, BatteryCharacter batteryChar, double temperature) {
    TemperatureLimits limits = getTemperatureLimits(batteryChar.coolingType);
    BreachType breachType = inferBreach(temperature, limits.lowerLimit, limits.upperLimit);
    handleAlert(target, breachType);
}
