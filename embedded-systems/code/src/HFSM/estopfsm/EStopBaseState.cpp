/**
* @file EStopBaseState.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Basisklasse EStopBaseState.
 */

#include "estopbasestate.h"
#include "estop1aktiv.h"
#include "estop2aktiv.h"

void EStopBaseState::enterViaPseudoStart() {
    // entry to state machine
    cout << "Sub Initial Transition taken" << endl;
    // Action
    new(this) EStop1Aktiv;
    enterByDefaultEntryPoint(); // Entry of initial state
}

void EStopBaseState::enterViaDeepHistory() {
    // Idee: Wenn der aktuelle State ein Pseudo-Start- oder Pseudo-End-State ist,
    // gibt es effektiv keine sinnvolle History -> starte "frisch" über den Pseudo-Start.
    if (isPseudoStartState() || isPseudoEndState()) {
        std::cout << "EStop Sub-FSM: no deep history, using pseudo start" << std::endl;
        enterViaPseudoStart();
    } else {
        std::cout << "EStop Sub-FSM: entering via deep history" << std::endl;
        enterByDeepHistoryEntryPoint();
    }
}

void EStopBaseState::enterExplicitToEStop1Aktiv() {
    std::cout << "enterExplicitToEStop1Aktiv():aufgerufen" << std::endl;
    // Transition action
    new(this) EStop1Aktiv;
    enterByDefaultEntryPoint();
}

void EStopBaseState::enterExplicitToEStop2Aktiv() {
    // Transition action
    new(this) EStop2Aktiv;
    enterByDefaultEntryPoint();
}