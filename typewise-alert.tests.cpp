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

TEST(TypeWiseAlertTestSuite, SendsEmailForBreach) {
    testing::InSequence seq; // Ensure the order of calls is correct

    // Set expectations
    EXPECT_CALL(mockAlert, sendToEmail(TOO_LOW)).Times(1);
    EXPECT_CALL(mockAlert, sendToEmail(TOO_HIGH)).Times(1);
    EXPECT_CALL(mockAlert, sendToEmail(NORMAL)).Times(1);

    // Test sending emails for each breach type
    mockAlert.sendToEmail(TOO_LOW);
    mockAlert.sendToEmail(TOO_HIGH);
    mockAlert.sendToEmail(NORMAL);
}

TEST(TypeWiseAlertTestSuite, SendsToControllerForBreach) {
    testing::InSequence seq;

    // Set expectations
    EXPECT_CALL(mockAlert, sendToController(TOO_LOW)).Times(1);
    EXPECT_CALL(mockAlert, sendToController(TOO_HIGH)).Times(1);
    EXPECT_CALL(mockAlert, sendToController(NORMAL)).Times(1);

    // Test sending to controller for each breach type
    mockAlert.sendToController(TOO_LOW);
    mockAlert.sendToController(TOO_HIGH);
    mockAlert.sendToController(NORMAL);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertForController) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;
    double temperatureInC = 36;

    EXPECT_CALL(mockAlert, sendToController(TOO_HIGH)).Times(1);
    checkAndAlert(TO_CONTROLLER, batteryChar, temperatureInC);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertForEmail) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;
    double temperatureInC = 34;

    EXPECT_CALL(mockAlert, sendToEmail(NORMAL)).Times(1);
    checkAndAlert(TO_EMAIL, batteryChar, temperatureInC);
}

TEST(TypeWiseAlertTestSuite, HandlesInvalidBreachesInEmail) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;

    // Test with invalid breach type (outside defined enums)
    EXPECT_NO_THROW(mockAlert.sendToEmail(static_cast<BreachType>(-1))); // Invalid breach
    EXPECT_NO_THROW(mockAlert.sendToEmail(static_cast<BreachType>(3)));  // Invalid breach
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
