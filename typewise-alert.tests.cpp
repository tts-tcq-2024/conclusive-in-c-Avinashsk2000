#include <gtest/gtest.h>
#include "typewise-alert.h"

// Use a flag to track calls for testing
bool controller_called = false;
bool email_called = false;
BreachType last_breach_type;

void sendToControllerMock(BreachType breachType) {
    controller_called = true;
    last_breach_type = breachType;
}

void sendToEmailMock(BreachType breachType) {
    email_called = true;
    last_breach_type = breachType;
}

TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    EXPECT_EQ(inferBreach(20, 50, 100), TOO_LOW);
    EXPECT_EQ(inferBreach(150, 50, 100), TOO_HIGH);
    EXPECT_EQ(inferBreach(75, 50, 100), NORMAL);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachWithPassiveCooling) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;

    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 34), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 35), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 36), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertForController) {
    controller_called = false; // Reset the flag before test
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;
    double temperatureInC = 36;

    // Directly call the mock function
    sendToController = sendToControllerMock; // Override the original function
    checkAndAlert(TO_CONTROLLER, batteryChar, temperatureInC);
    
    EXPECT_TRUE(controller_called);
    EXPECT_EQ(last_breach_type, TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertForEmail) {
    email_called = false; // Reset the flag before test
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;
    double temperatureInC = 34;

    // Directly call the mock function
    sendToEmail = sendToEmailMock; // Override the original function
    checkAndAlert(TO_EMAIL, batteryChar, temperatureInC);
    
    EXPECT_TRUE(email_called);
    EXPECT_EQ(last_breach_type, NORMAL);
}

// Include other tests for different cooling types...

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
