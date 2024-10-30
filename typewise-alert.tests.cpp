#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "typewise-alert.h"

// Mocking class to capture output
class MockAlert {
public:
    MOCK_METHOD(void, sendToController, (BreachType), ());
    MOCK_METHOD(void, sendToEmail, (BreachType), ());
};

MockAlert mockAlert;

// Test for getTemperatureLimits with invalid cooling type
TEST(TypeWiseAlertTestSuite, GetTemperatureLimitsInvalidCoolingType) {
    EXPECT_EXIT(getTemperatureLimits(static_cast<CoolingType>(-1)), 
                ::testing::ExitedWithCode(EXIT_FAILURE), "Error: Invalid cooling type.");
}

// Test for getTemperatureLimits with valid cooling types
TEST(TypeWiseAlertTestSuite, GetTemperatureLimitsValidCoolingTypes) {
    TemperatureLimits limits = getTemperatureLimits(PASSIVE_COOLING);
    EXPECT_EQ(limits.lowerLimit, 0); // Corrected from 'limits.lower' to 'limits.lowerLimit'
    EXPECT_EQ(limits.upperLimit, 35); // Corrected from 'limits.upper' to 'limits.upperLimit'

    limits = getTemperatureLimits(HI_ACTIVE_COOLING);
    EXPECT_EQ(limits.lowerLimit, 0); // Corrected
    EXPECT_EQ(limits.upperLimit, 45); // Corrected

    limits = getTemperatureLimits(MED_ACTIVE_COOLING);
    EXPECT_EQ(limits.lowerLimit, 0); // Corrected
    EXPECT_EQ(limits.upperLimit, 40); // Corrected
}

// Test inferBreach for TOO_LOW, TOO_HIGH, and NORMAL cases
TEST(TypeWiseAlertTestSuite, InferBreachCases) {
    EXPECT_EQ(inferBreach(10, 20, 30), TOO_LOW);     // Test TOO_LOW
    EXPECT_EQ(inferBreach(25, 20, 30), NORMAL);      // Test NORMAL
    EXPECT_EQ(inferBreach(35, 20, 30), TOO_HIGH);    // Test TOO_HIGH
}

// Test sendToController with breach type
TEST(TypeWiseAlertTestSuite, SendToControllerWithBreachType) {
    EXPECT_CALL(mockAlert, sendToController(TOO_LOW)).Times(1);
    sendToController(TOO_LOW);
}

// Test sendToEmail with valid breach types
TEST(TypeWiseAlertTestSuite, SendToEmailValidBreachTypes) {
    EXPECT_CALL(mockAlert, sendToEmail(NORMAL)).Times(1);
    sendToEmail(NORMAL);

    EXPECT_CALL(mockAlert, sendToEmail(TOO_LOW)).Times(1);
    sendToEmail(TOO_LOW);

    EXPECT_CALL(mockAlert, sendToEmail(TOO_HIGH)).Times(1);
    sendToEmail(TOO_HIGH);
}

// Test sendToEmail with invalid breach type
TEST(TypeWiseAlertTestSuite, SendToEmailInvalidBreachType) {
    EXPECT_EXIT(sendToEmail(static_cast<BreachType>(-1)), 
                ::testing::ExitedWithCode(EXIT_FAILURE), "Error: Invalid breach type for email notification.");
}

// Test handleAlert with valid and invalid alert targets
TEST(TypeWiseAlertTestSuite, HandleAlertValidAndInvalidTargets) {
    EXPECT_CALL(mockAlert, sendToController(TOO_HIGH)).Times(1);
    handleAlert(TO_CONTROLLER, TOO_HIGH);

    EXPECT_CALL(mockAlert, sendToEmail(TOO_LOW)).Times(1);
    handleAlert(TO_EMAIL, TOO_LOW);

    EXPECT_EXIT(handleAlert(static_cast<AlertTarget>(-1), TOO_HIGH), 
                ::testing::ExitedWithCode(EXIT_FAILURE), "Error: Invalid alert target.");
}

// Test checkAndAlert for high temperature
TEST(TypeWiseAlertTestSuite, CheckAndAlertHighTemperature) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = HI_ACTIVE_COOLING;

    EXPECT_CALL(mockAlert, sendToEmail(TOO_HIGH)).Times(1);
    checkAndAlert(TO_EMAIL, batteryChar, 46);  // Exceeds HI_ACTIVE_COOLING limit
}

// Test checkAndAlert for normal temperature
TEST(TypeWiseAlertTestSuite, CheckAndAlertNormalTemperature) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;

    EXPECT_CALL(mockAlert, sendToEmail(NORMAL)).Times(1);
    checkAndAlert(TO_EMAIL, batteryChar, 25);  // Within PASSIVE_COOLING limit
}

// Test checkAndAlert for low temperature
TEST(TypeWiseAlertTestSuite, CheckAndAlertLowTemperature) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = MED_ACTIVE_COOLING;

    EXPECT_CALL(mockAlert, sendToEmail(TOO_LOW)).Times(1);
    checkAndAlert(TO_EMAIL, batteryChar, -5);  // Below MED_ACTIVE_COOLING limit
}
