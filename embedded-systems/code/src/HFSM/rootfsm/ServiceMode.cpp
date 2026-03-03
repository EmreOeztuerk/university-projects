/**
* @file ServiceMode.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des ServiceMode-Zustands.
 */

#include "serviceMode.h"

#include <iostream>

void ServiceMode::enterByDefaultEntryPoint() {
    entry();
}

void ServiceMode::exitExplicitToEStop1() {
    exit();
    new(this) EStop;
    enterByE1EntryPoint();
}

void ServiceMode::exitExplicitToEStop2() {
    exit();
    new(this) EStop;
    enterByE2EntryPoint();
}

void ServiceMode::exitExplicitToBetriebszustand() {
    exit();
    new(this) Betriebszustand;
    enterByDefaultEntryPoint();
}

void ServiceMode::exitExplicitToRuhezustand() {
    exit();
    new(this) Ruhezustand;
    exitExplicitToRuhezustand();
}


void ServiceMode::handleDefaultExit(const TriggerProcessingState &handled) {
    switch (handled) {
        case TriggerProcessingState::explicitExitViaE1:
            new(this) EStop;
            enterByE1EntryPoint();
            break;
        case TriggerProcessingState::explicitExitViaE2:
            new(this) EStop;
            enterByE2EntryPoint();
            break;
        case TriggerProcessingState::explicitExitViaR:
            new(this) Ruhezustand;
            exitExplicitToRuhezustand();
            break;
        default:
            break;
    }
}

void ServiceMode::leavingState() {
    exit();
}

void ServiceMode::entry() {
    std::cout << "Delta::" << __func__ << " std::called" << endl;
    action->cmd_led_amp_glb_an();
}

void ServiceMode::exit() {
    action->cmd_led_amp_glb_aus();
}

void ServiceMode::s_st1_g() {
    //	TriggerProcessingState processingstate = servicemode_statemachine->s_st1_g();
    servicemode_statemachine->s_st1_g();
    new(this) BCKalibrierung_BaseState;
    enterByDefaultEntryPoint();
}

void ServiceMode::s_stp_g() {
    if (data->kalibrierungFertig) {
        TriggerProcessingState processingstate = servicemode_statemachine->s_stp_g();
        handleDefaultExit(processingstate);
    }
}

void ServiceMode::s_nvbdn() {
    //	TriggerProcessingState processingstate = servicemode_statemachine->s_nvbdn();
    servicemode_statemachine->s_nvbdn();
    leavingState();
}

void ServiceMode::s_e1_g() {
    leavingState();
    new(this) EStop;
    enterByE1EntryPoint();
    //TriggerProcessingState processingstate = servicemode_statemachine->s_e1_g();
    //handleDefaultExit(processingstate);
}

void ServiceMode::s_e2_g() {
    leavingState();
    new(this) EStop;
    enterByE2EntryPoint();
    //TriggerProcessingState processingstate = servicemode_statemachine->s_e2_g();
    //handleDefaultExit(processingstate);
}

void ServiceMode::showState() {
    std::cout << "State: ServiceMode" << std::endl;
}