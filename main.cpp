#include "typewise-alert.h"
 
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
