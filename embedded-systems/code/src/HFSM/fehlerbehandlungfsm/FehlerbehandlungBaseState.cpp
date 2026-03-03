/**
 * @file FehlerbehandlungBaseState.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Basisklasse FehlerbehandlungBaseState.
 */

#include "FehlerbehandlungBaseState.h"
#include "AnstehendUnquittiert.h"
#include "../estopfsm/EStop1Aktiv.h"
#include "../estopfsm/EStop2Aktiv.h"

void FehlerbehandlungBaseState::setData(ContextData *data) {
    this->data = data;
}

void FehlerbehandlungBaseState::setAction(Actions *action) {
    this->action = action;
}

void FehlerbehandlungBaseState::initFehlerbehandlungStateMachines() {
}

void FehlerbehandlungBaseState::enterViaPseudoStart() {
    std::cout << "[FehlerbehandlungBaseState] Fängt mit Anstehend Unquittiert an." << std::endl;
    new(this) AnstehendUnquittiert;
    enterByDefaultEntryPoint();
}

void FehlerbehandlungBaseState::enterViaDeepHistory() {
    if (isPseudoStartState() || isPseudoEndState()) {
        enterViaPseudoStart();
    } else {
        enterByDeepHistoryEntryPoint();
    }
}

void FehlerbehandlungBaseState::exitExplicitToEStop1() {
    std::cout << "[Fehlerbehandlung] E-Stop 1 gedrückt." << std::endl;
    new(this) EStop1Aktiv;
    enterByDefaultEntryPoint();
}

void FehlerbehandlungBaseState::exitExplicitToEStop2() {
    std::cout << "[Fehlerbehandlung] E-Stop 1 gedrückt." << std::endl;
    new(this) EStop2Aktiv;
    enterByDefaultEntryPoint();
}

TriggerProcessingState FehlerbehandlungBaseState::handleDefaultExit(const TriggerProcessingState &handled) {
    return TriggerProcessingState::pending;
};

TriggerProcessingState FehlerbehandlungBaseState::s_partner_ok() {
    data->setFehler(Fehlerart::keinFehler, FehlerOrt::DEFAULT);
    return TriggerProcessingState::consumed;
}