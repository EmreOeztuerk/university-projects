/**
 * @file EStop.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des EStop-Zustands.
 */

#include "EStop.h"
#include "Ruhezustand.h"
#include "../Actions.h"
#include "../estopfsm/EStopBaseState.h"
#include "../subcommon/TriggerProcessingState.h"

#include <iostream>

void EStop::enterByDeepHistoryEntryPoint() {
    entry();
    estopstatemachine->enterViaDeepHistory();
}

void EStop::enterByE1EntryPoint() {
    std::cout << "enterByE1EntryPoint() aufgeruft" << std::endl;
    entry();
    estopstatemachine->enterExplicitToEStop1Aktiv();
}

void EStop::enterByE2EntryPoint() {
    entry();
    estopstatemachine->enterExplicitToEStop2Aktiv();
}

void EStop::handleDefaultExit(const TriggerProcessingState &processingstate) {
    if (processingstate == TriggerProcessingState::endstatereached) {
        std::cout << "[estop] Ampellicht aus." << std::endl;
        // Sub-state finished
        action->cmd_led_amp_rot_aus();
        action->cmd_led_amp_glb_aus();
        estopstatemachine->exit(); // just call own exit.
        new(this) Ruhezustand; // Wechsel zum Ruhezustand
        enterByDefaultEntryPoint();
    }
}

// --- Entry / Exit Actions ---
void EStop::entry() {
    std::cout << "State: EStop" << std::endl;
    action->enteredEStop();

    // Aktionen beim Eintritt
    action->cmd_mtr_stop();
    action->cmd_wch_aus();
    action->cmd_led_amp_grn_aus();
    action->cmd_led_amp_rot_an();
    action->cmd_led_amp_gelb_b();
    action->cmd_led_rt_aus();
    action->cmd_led_q1_aus();
}

void EStop::leavingState() {
    estopstatemachine->leavingState();
    exit();
}

// --- Trigger Events ---
void EStop::s_e1_g() {
    TriggerProcessingState processingstate = estopstatemachine->s_e1_g();
    handleDefaultExit(processingstate);
}

void EStop::s_e2_g() {
    TriggerProcessingState processingstate = estopstatemachine->s_e2_g();
    handleDefaultExit(processingstate);
}

void EStop::s_e1_ng() {
    TriggerProcessingState processingstate = estopstatemachine->s_e1_ng();
    handleDefaultExit(processingstate);
}

void EStop::s_e2_ng() {
    TriggerProcessingState processingstate = estopstatemachine->s_e2_ng();
    handleDefaultExit(processingstate);
}

void EStop::s_rt1_g() {
    TriggerProcessingState processingstate = estopstatemachine->s_rt1_g();
    handleDefaultExit(processingstate);
}

void EStop::s_rt2_g() {
    TriggerProcessingState processingstate = estopstatemachine->s_rt2_g();
    handleDefaultExit(processingstate);
}


void EStop::showState() {
    std::cout << "State: EStop" << std::endl;
}