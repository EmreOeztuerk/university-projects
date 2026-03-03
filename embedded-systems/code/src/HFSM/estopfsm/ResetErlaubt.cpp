/**
 * @file ResetErlaubt.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von ResetErlaubt.
 */

#include "../actions.h"
#include "reseterlaubt.h"
#include "estop1aktiv.h"
#include "estop2aktiv.h"
#include "wartenreset1.h"
#include "wartenreset2.h"


#include <iostream>
using namespace std;

void ResetErlaubt::entry() {
    std::cout << "State: ResetErlaubt" << std::endl;
    action->enteredResetErlaubt();
    action->cmd_led_rt_an();
};

TriggerProcessingState ResetErlaubt::s_e1_g() {
    cout << "ResetErlaubt::E1_G called" << endl;
    leavingState();
    // Transition action
    new(this) EStop1Aktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState ResetErlaubt::s_e2_g() {
    cout << "ResetErlaubt::E2_G called" << endl;
    leavingState();
    // Transition action
    new(this) EStop2Aktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState ResetErlaubt::s_rt1_g() {
    cout << "ResetErlaubt::RT1_G called" << endl;
    leavingState();
    // Transition action
    new(this) WartenReset2;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState ResetErlaubt::s_rt2_g() {
    cout << "ResetErlaubt::RT2_G called" << endl;
    leavingState();
    // Transition action
    new(this) WartenReset1;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void ResetErlaubt::showState() {
    std::cout << "State: ResetErlaubt" << std::endl;
}