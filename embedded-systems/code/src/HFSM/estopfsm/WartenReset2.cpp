/**
 * @file WartenReset2.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenReset2.
 */

#include "wartenreset2.h"
#include "estop1aktiv.h"
#include "estop2aktiv.h"
#include "EStopPseudoEndState.h"

#include <iostream>

using namespace std;

void WartenReset2::entry() {
    cout << "current State:WartenReset2" << endl;
    action->enteredWartenReset2();
};

TriggerProcessingState WartenReset2::s_rt2_g() {
    cout << "WartenReset2::RT2_G called" << endl;
    leavingState();

    // Transition-Aktion: Daten zurücksetzen
    data->reset_abgeschlossen = data->datenprozessLoeschen();

    new(this) EStopPseudoEndState;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::endstatereached;
}

TriggerProcessingState WartenReset2::s_e1_g() {
    cout << "WartenReset2::E1_G called" << endl;
    leavingState();
    // Transition action
    new(this) EStop1Aktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState WartenReset2::s_e2_g() {
    cout << "WartenReset2::E2_G called" << endl;
    leavingState();
    // Transition action
    new(this) EStop2Aktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void WartenReset2::showState() {
    cout << "current State:WartenReset2" << endl;
}