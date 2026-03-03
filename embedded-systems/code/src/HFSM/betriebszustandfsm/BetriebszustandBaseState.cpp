//
// Created by emreo on 09.12.2025.
//

#include "BetriebszustandBaseState.h"
#include "orthfsm/RegionLSA/RegionLSAPseudoStartState.h"
#include "orthfsm/RegionLSH/RegionLSHPseudoStartState.h"
#include "orthfsm/RegionLSHbisLSW/RegionLSHBisLSWPseudoStartState.h"
#include "orthfsm/RegionLSWbisLSE/RegionLSWBisLSEPseudoStartState.h"
#include "orthfsm/RegionWarnung/RegionWarnungPseudoStartState.h"
#include "AufWSWarten.h"
#include "../estopfsm/EStop1Aktiv.h"
#include "../estopfsm/EStop2Aktiv.h"

void BetriebszustandBaseState::initBetriebszustandStateMachines() {
    regionLSA = new RegionLSAPseudoStartState();
    regionLSH = new RegionLSHPseudoStartState();
    regionLSHbisLSW = new RegionLSHBisLSWPseudoStartState();
    regionLSWbisLSE = new RegionLSWBisLSEPseudoStartState();
    regionWarnung = new RegionWarnungPseudoStartState();
}

void BetriebszustandBaseState::setData(ContextData *data) {
    this->data = data;
    regionLSA->setData(data);
    regionLSH->setData(data);
    regionLSHbisLSW->setData(data);
    regionLSWbisLSE->setData(data);
    regionWarnung->setData(data);
}

void BetriebszustandBaseState::setAction(Actions *action) {
    this->action = action;
    regionLSA->setAction(action);
    regionLSH->setAction(action);
    regionLSHbisLSW->setAction(action);
    regionLSWbisLSE->setAction(action);
    regionWarnung->setAction(action);
}

void BetriebszustandBaseState::enterViaPseudoStart() {
    std::cout << "[Betriebszustand] Fängt mit AufWSWarten an." << std::endl;
    new(this) AufWSWarten;
    enterByDefaultEntryPoint();
}

void BetriebszustandBaseState::enterViaDeepHistory() {
    if (isPseudoStartState() || isPseudoEndState()) {
        enterViaPseudoStart();
    } else {
        enterByDeepHistoryEntryPoint();
    }
}

void BetriebszustandBaseState::exitExplicitToEStop1() {
    std::cout << "[Fehlerbehandlung] E-Stop 1 gedrückt." << std::endl;
    new(this) EStop1Aktiv;
    enterByDefaultEntryPoint();
}

void BetriebszustandBaseState::exitExplicitToEStop2() {
    std::cout << "[Fehlerbehandlung] E-Stop 2 gedrückt." << std::endl;
    new(this) EStop2Aktiv;
    enterByDefaultEntryPoint();
}

TriggerProcessingState BetriebszustandBaseState::handleDefaultExit(const TriggerProcessingState &handled) {
    return TriggerProcessingState::pending;
};

TriggerProcessingState BetriebszustandBaseState::s_partner_ok_1() {
    return TriggerProcessingState::consumed;
}

TriggerProcessingState BetriebszustandBaseState::s_partner_ok_2() {
    return TriggerProcessingState::consumed;
}