/**
 * @file EStop2Aktiv.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von EStop2Aktiv.
 */

#include "EStop2Aktiv.h"
#include "EStopsAktiv.h"
#include "reseterlaubt.h"
#include "estopbasestate.h"

#include <iostream>
using namespace std;

void EStop2Aktiv::entry() {
    action->enteredEStopsAktiv();
    data->Fehlermeldung("E2");
};


TriggerProcessingState EStop2Aktiv::s_e2_ng() {
    cout << "EStop2Aktiv::E2_NG called" << endl;
    leavingState();
    // Transition action
    new(this) ResetErlaubt;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState EStop2Aktiv::s_e1_g() {
    cout << "EStop2Aktiv::E1_G called" << endl;
    leavingState();
    // Transition action
    new(this) EStopsAktiv;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void EStop2Aktiv::showState() {
    std::cout << "State: EStop2Aktiv" << std::endl;
}