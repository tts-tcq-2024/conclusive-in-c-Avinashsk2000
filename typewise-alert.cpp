#include "typewise-alert.h"
#include <stdio.h>
#include <string.h>

// Temperature limit struct to define bounds for each cooling type
typedef struct {
    double lowerLimit;
    double upperLimit;
} TemperatureLimits;

// Temperature limits lookup table for each cooling type
const TemperatureLimits coolingLimits[] = {
    {0, 35},  // PASSIVE_COOLING
    {0, 45},  // HI_ACTIVE_COOLING
    {0, 40}   // MED_ACTIVE_COOLING
};

// Retrieve temperature limits based on cooling type
TemperatureLimits getTemperatureLimits(CoolingType coolingType) {
    return coolingLimits[coolingType];
}

// Infer breach type based on limits
BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if (value < lowerLimit) {
        return TOO_LOW;
    }
    if (value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

// Classify temperature breach based on cooling type and temperature
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    TemperatureLimits limits = getTemperatureLimits(coolingType);
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

// Handle alert based on alert target
void handleAlert(AlertTarget alertTarget, BreachType breachType) {
    switch (alertTarget) {
        case TO_CONTROLLER:
            sendToController(breachType);
            break;
        case TO_EMAIL:
            sendToEmail(breachType);
            break;
    }
}

// Main function to check and alert based on temperature and cooling type
void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    handleAlert(alertTarget, breachType);
}

// Send alert to the controller
void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

// Send alert via email
void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    switch (breachType) {
        case TOO_LOW:
            printf("To: %s\n", recipient);
            printf("Hi, the temperature is too low\n");
            break;
        case TOO_HIGH:
            printf("To: %s\n", recipient);
            printf("Hi, the temperature is too high\n");
            break;
        case NORMAL:
            printf("To: %s\n", recipient);
            printf("Hi, the temperature is normal\n");
            break;
    }
}
