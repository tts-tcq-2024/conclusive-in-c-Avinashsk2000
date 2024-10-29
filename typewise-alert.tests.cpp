#include <gtest/gtest.h>
#include "typewise-alert.h"

// Global variables to track state in mock functions
bool controller_called = false;
bool email_called = false;
BreachType last_breach_type;

// Function pointer types for mocking
void (*sendToControllerPtr)(BreachType) = sendToController; 
void (*sendToEmailPtr)(BreachType) = sendToEmail;

// Mock functions
void sendToControllerMock(BreachType breachType) {
    controller_called = true;
    last_breach_type = breachType;
}

void sendToEmailMock(BreachType breachType) {
    email_called = true;
    last_breach_type = breachType;
}

// Test Suite
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
    last_breach_type = NORMAL; // Reset the breach type before test

    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;
    double temperatureInC = 36;

    // Use the mock function for the test
    sendToControllerPtr = sendToControllerMock; // Point to mock
    checkAndAlert(TO_CONTROLLER, batteryChar, temperatureInC);
    
    EXPECT_TRUE(controller_called);
    EXPECT_EQ(last_breach_type, TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertForEmail) {
    email_called = false; // Reset the flag before test
    last_breach_type = NORMAL; // Reset the breach type before test

    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;
    double temperatureInC = 34;

    // Use the mock function for the test
    sendToEmailPtr = sendToEmailMock; // Point to mock
    checkAndAlert(TO_EMAIL, batteryChar, temperatureInC);
    
    EXPECT_TRUE(email_called);
    EXPECT_EQ(last_breach_type, NORMAL);
}

// Main function to run the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); // Correct initialization
    return RUN_ALL_TESTS();
}
