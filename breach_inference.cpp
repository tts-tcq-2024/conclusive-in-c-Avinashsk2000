#include "typewise-alert.h"
 
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
