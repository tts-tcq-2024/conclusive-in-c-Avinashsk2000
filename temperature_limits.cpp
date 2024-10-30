#include "typewise-alert.h"
#include <stdio.h>   // Include for fprintf and stderr
#include <stdlib.h>  // Include for exit and EXIT_FAILURE

// Temperature limits lookup table for each cooling type
static const TemperatureLimits coolingLimits[] = {
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
