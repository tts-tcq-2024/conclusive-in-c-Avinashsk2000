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

// Send alert to the controller
void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

// Send alert via email
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
    }
}

// Handle alert based on alert target
void handleAlert(AlertTarget alertTarget, BreachType breachType) {
    if (alertTarget == TO_CONTROLLER) {
        sendToController(breachType);
    } else if (alertTarget == TO_EMAIL) {
        sendToEmail(breachType);
    }
}

// Main function to check and alert based on temperature and cooling type
void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    handleAlert(alertTarget, breachType);
}
