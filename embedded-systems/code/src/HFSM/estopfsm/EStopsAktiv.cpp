/**
* @file EStopsAktiv.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von EStopsAktiv.
 */

#include "estopsaktiv.h"
#include "estop1aktiv.h"
#include "estop2aktiv.h"

#include <iostream>
using namespace std;

void EStopsAktiv::entry() {
    action->enteredEStopsAktiv();
    data->Fehlermeldung("E");
};


TriggerProcessingState EStopsAktiv::s_e1_ng() {
    cout << "EStopsAktiv::E1_NG called" << endl;
    leavingState();
    // Transition action
    new(this) EStop2Aktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState EStopsAktiv::s_e2_ng() {
    cout << "EStopsAktiv::E2_NG called" << endl;
    leavingState();
    // Transition action
    new(this) EStop1Aktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}


void EStopsAktiv::showState() {
    cout << "current State: Working" << endl;
}