#include <gtest/gtest.h>
#include <gmock/gmock.h> // For mocking
#include "typewise-alert.h"

// Mocking functions to capture output
class MockAlert {
public:
    MOCK_METHOD(void, sendToController, (BreachType), ());
    MOCK_METHOD(void, sendToEmail, (BreachType), ());
};

// Global instance of the mock class
MockAlert mockAlert;

// Override the original functions with mocks
void sendToControllerMock(BreachType breachType) {
    mockAlert.sendToController(breachType);
}

void sendToEmailMock(BreachType breachType) {
    mockAlert.sendToEmail(breachType);
}

// Tests
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

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachWithHiActiveCooling) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = HI_ACTIVE_COOLING;

    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 44), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 45), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 46), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachWithMedActiveCooling) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = MED_ACTIVE_COOLING;

    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 39), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 40), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(batteryChar.coolingType, 41), TOO_HIGH);
}
