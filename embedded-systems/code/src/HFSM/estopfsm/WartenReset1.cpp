/**
 * @file WartenReset1.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenReset1.
 */

#include "wartenreset1.h"
#include "estop1aktiv.h"
#include "estop2aktiv.h"
#include "EStopPseudoEndState.h"
#include "estopbasestate.h"


#include <iostream>
using namespace std;

void WartenReset1::entry() {
    cout << "current State: WartenReset1" << endl;
    action->enteredWartenReset1();
};

TriggerProcessingState WartenReset1::s_rt1_g() {
    cout << "WartenReset1::RT2_G called" << endl;
    leavingState();

    // Transition-Aktion: Daten zurücksetzen
    data->reset_abgeschlossen = data->datenprozessLoeschen();

    new(this) EStopPseudoEndState;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::endstatereached;
}


TriggerProcessingState WartenReset1::s_e1_g() {
    cout << "WartenReset1::E1_G called" << endl;
    leavingState();
    // Transition action
    new(this) EStop1Aktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState WartenReset1::s_e2_g() {
    cout << "WartenReset1::E2_G called" << endl;
    leavingState();
    // Transition action
    new(this) EStop2Aktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void WartenReset1::showState() {
    cout << "current State: WartenReset1" << endl;
}