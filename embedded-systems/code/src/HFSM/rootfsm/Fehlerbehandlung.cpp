/**
 * @file Fehlerbehandlung.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Fehlerbehandlungs-Zustands.
 */

#include "Fehlerbehandlung.h"
#include "EStop.h"
#include "Betriebszustand.h"
#include "Ruhezustand.h"


#include <iostream>

void Fehlerbehandlung::enterByDefaultEntryPoint() {
    entry();
    fbstatemachine->enterViaPseudoStart();
    std::cout << "enterByDefaultEntryPoint: Fehlerbehandlung" << std::endl;
}

void Fehlerbehandlung::entry() {
    action->cmd_mtr_stop();
}

void Fehlerbehandlung::exit() {
    action->cmd_led_amp_rot_aus();
}

void Fehlerbehandlung::handleDefaultExit(const TriggerProcessingState &handled) {
    if (handled == TriggerProcessingState::endstatereached) {
        leavingState();
        new(this) Betriebszustand;
        enterByDeepHistoryEntryPoint();
    }
}

void Fehlerbehandlung::leavingState() {
    fbstatemachine->leavingState();
    exit();
}

void Fehlerbehandlung::s_e1_g() {
    std::cout << "Fehlerbehandlung::E1_G called" << std::endl;
    leavingState();
    new(this) EStop;
    enterByE1EntryPoint();
}

void Fehlerbehandlung::s_e2_g() {
    std::cout << "Fehlerbehandlung::E2_G called" << std::endl;
    leavingState();
    new(this) EStop;
    enterByE2EntryPoint();
}

void Fehlerbehandlung::s_lsr_nub() {
    TriggerProcessingState processingstate = fbstatemachine->s_lsr_nub();
    std::cout << "[Debug Root Fehlerbehandlung] s_lsr_nub" << std::endl;
    handleDefaultExit(processingstate);
}

void Fehlerbehandlung::s_st_g() {
    TriggerProcessingState processingstate = fbstatemachine->s_st_g();
    std::cout << "[Debug Root Fehlerbehandlung] s_st1_g" << std::endl;
    handleDefaultExit(processingstate);
}

void Fehlerbehandlung::s_rt1_g() {
    TriggerProcessingState processingstate = fbstatemachine->s_rt1_g();
    std::cout << "[Debug Root Fehlerbehandlung] s_rt1_g" << std::endl;
    handleDefaultExit(processingstate);
}

void Fehlerbehandlung::s_lsa2_ub() {
    TriggerProcessingState processingstate = fbstatemachine->s_lsa2_ub();
    std::cout << "[Debug Root Fehlerbehandlung] s_lsa2_ub" << std::endl;
    handleDefaultExit(processingstate);
}

void Fehlerbehandlung::s_partner_ok_1() {
    leavingState();
    new(this) Betriebszustand;
    enterByDeepHistoryEntryPoint();
}

void Fehlerbehandlung::s_partner_ok_2() {
    leavingState();
    new(this) Betriebszustand;
    enterByDeepHistoryEntryPoint();
}

void Fehlerbehandlung::showState() {
    std::cout << "State: Fehlerbehandlung" << std::endl;
}