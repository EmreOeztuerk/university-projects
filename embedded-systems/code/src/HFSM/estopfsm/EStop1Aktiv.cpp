/**
* @file EStop1Aktiv.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von EStop1Aktiv.
 */

#include "reseterlaubt.h"
#include "estopsaktiv.h"
#include "estop1aktiv.h"


#include <iostream>
using namespace std;

void EStop1Aktiv::entry() {
    cout << "current State: EStop1Aktiv" << endl;
    action->enteredEStop1Aktiv();
    data->Fehlermeldung("E1");
};


TriggerProcessingState EStop1Aktiv::s_e1_ng() {
    cout << "EStop1Aktiv::E1_NG called" << endl;
    leavingState();
    // Transition action
    new(this) ResetErlaubt;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState EStop1Aktiv::s_e2_g() {
    cout << "EStop1Aktiv::E2_G called" << endl;
    leavingState();
    // Transition action
    new(this) EStopsAktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed; // Transition not completely handled.
}


void EStop1Aktiv::showState() {
    cout << "current State: EStop1Aktiv" << endl;
}