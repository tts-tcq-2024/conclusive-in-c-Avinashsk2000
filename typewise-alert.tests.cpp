#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "typewise-alert.h"

// Mocking functions to capture output
class MockAlert {
public:
    MOCK_METHOD(void, sendToController, (BreachType), ());
    MOCK_METHOD(void, sendToEmail, (BreachType), ());
};

// Global instance of the mock class
MockAlert mockAlert;

// Redirects the actual alert functions to use the mock implementations
void sendToController(BreachType breachType) {
    mockAlert.sendToController(breachType);
}

void sendToEmail(BreachType breachType) {
    mockAlert.sendToEmail(breachType);
}

// Tests
TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    EXPECT_EQ(inferBreach(20, 50, 100), TOO_LOW);
    EXPECT_EQ(inferBreach(150, 50, 100), TOO_HIGH);
    EXPECT_EQ(inferBreach(75, 50, 100), NORMAL);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertSendsEmailForHighTemperature) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = PASSIVE_COOLING;

    EXPECT_CALL(mockAlert, sendToEmail(TOO_HIGH)).Times(1);

    checkAndAlert(TO_EMAIL, batteryChar, 36);
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertSendsControllerForLowTemperature) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = HI_ACTIVE_COOLING;

    EXPECT_CALL(mockAlert, sendToController(TOO_LOW)).Times(1);

    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
}

TEST(TypeWiseAlertTestSuite, NoAlertForNormalTemperature) {
    BatteryCharacter batteryChar;
    batteryChar.coolingType = MED_ACTIVE_COOLING;

    EXPECT_CALL(mockAlert, sendToController(::testing::_)).Times(0);
    EXPECT_CALL(mockAlert, sendToEmail(::testing::_)).Times(0);

    checkAndAlert(TO_EMAIL, batteryChar, 39);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
