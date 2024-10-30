#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>  

typedef enum {
    PASSIVE_COOLING,
    HI_ACTIVE_COOLING,
    MED_ACTIVE_COOLING
} CoolingType;

typedef enum {
    NORMAL,
    TOO_LOW,
    TOO_HIGH
} BreachType;

// Struct to define temperature limits for each cooling type
typedef struct {
    double lowerLimit;
    double upperLimit;
} TemperatureLimits;

BreachType inferBreach(double value, double lowerLimit, double upperLimit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);

typedef enum {
    TO_CONTROLLER,
    TO_EMAIL
} AlertTarget;

typedef struct {
    CoolingType coolingType;
    char brand[48];
} BatteryCharacter;

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);
void sendToController(BreachType breachType);
void sendToEmail(BreachType breachType);

TemperatureLimits getTemperatureLimits(CoolingType coolingType);
void handleAlert(AlertTarget alertTarget, BreachType breachType);

