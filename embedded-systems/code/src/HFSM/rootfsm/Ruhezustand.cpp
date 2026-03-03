/**
* @file Ruhezustand.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Ruhezustands.
 *
 * Leitet Events an die Sub-FSM (RZfsm) weiter und prüft auf Transitionen
 * in andere Root-Zustände (z.B. nach ServiceMode oder Betriebszustand).
 */


#include <iostream>
#include "Ruhezustand.h"
#include "EStop.h"
#include "Ruhezustand.h"
#include "Betriebszustand.h"
#include "ServiceMode.h"
#include "../RZfsm/Wartezustand.hpp"


void Ruhezustand::enterByDefaultEntryPoint() {
    entry();
    rzstatemachine->enterViaPseudoStart();
    std::cout << "enterByDefaultEntryPoint: Ruhezustand" << std::endl;
}

// ---------------------------------------------------------
// Explicit Exit Handlung
// ---------------------------------------------------------
void Ruhezustand::handleDefaultExit(const TriggerProcessingState &handled) {
    switch (handled) {
        //  Betriebszustand
        case TriggerProcessingState::explicitExitViaB: {
            //std::cout << "ROOT: Explicit Exit 'B' -> Betriebszustand" << std::endl;

            bool reset_abgeschlossen = data->istReset_abgeschlossen();;

            // reset_abgeschlossen
            if (reset_abgeschlossen) {
                //rzstatemachine->exit();
                leavingState();
                new(this) Betriebszustand;
                enterByDefaultEntryPoint();
            } else {
                // !reset_abgeschlossen
                //rzstatemachine->exit();
                leavingState();
                new(this) Betriebszustand;
                enterByDeepHistoryEntryPoint();
            }
        }
        break;

        //  ServiceMode
        case TriggerProcessingState::explicitExitViaS:
            //std::cout << "ROOT: Explicit Exit 'S' -> ServiceMode" << std::endl;
            //rzstatemachine->exit();
            leavingState();
            // std::cout << "[Debug Root Ruhezustand] new(this) ServiceMode" << std::endl;
            new(this) ServiceMode;
            // std::cout << "[Debug Root Ruhezustand] exit to ServiceMode" << std::endl;
            enterByDefaultEntryPoint();
            break;

        case TriggerProcessingState::explicitExitViaE1:
            //std::cout << "ROOT: Explicit Exit 'E1' -> ESTOP" << std::endl;
            //rzstatemachine->exit();
            leavingState();
            new(this) EStop;
            enterByE1EntryPoint();
            //enterByDefaultEntryPoint();
            break;

        default:
            //std::cout << "[Debug Root Ruhezustand] default case" << std::endl;
            break;
    }
}

//in SUb ist leavingState
void Ruhezustand::leavingState() {
    // if (rzfsm) {
    rzstatemachine->leavingState();
    //   delete rzfsm;
    // rzfsm = nullptr;
}


void Ruhezustand::entry() {
    //action->enteredRuhezustand();
    std::cout << "State: Ruhezustand" << std::endl;
    action->cmd_led_amp_grn_aus();
    action->cmd_led_amp_rot_aus();
    action->cmd_led_amp_glb_aus();
    action->cmd_led_q1_aus();
}

// ---------------------------------------------------------
// TRIGGERS
// ---------------------------------------------------------

void Ruhezustand::s_stp_g() {
    action->cmd_mtr_rechts_s();
}

void Ruhezustand::s_st1_lg() {
    //if (!rzfsm) return; // return TriggerProcessingState::pending;

    action->cmd_mtr_rechts_l();
    /*
    TriggerProcessingState processingstate = rzstatemachine->s_st1_lg();
    //std::cout << "[Debug Root Ruhezustand] s_st1_lg" << std::endl;
    handleDefaultExit(processingstate);*/
}

void Ruhezustand::s_st2_lg() {
    action->cmd_mtr_rechts_l();
    /// if (!rzfsm) return;
    /* TriggerProcessingState processingstate = rzstatemachine->s_st2_lg();
    handleDefaultExit(processingstate);*/
}

void Ruhezustand::s_st1_g() {
    //if (!rzfsm) return;
    TriggerProcessingState processingstate = rzstatemachine->s_st1_g();
    handleDefaultExit(processingstate);
}

void Ruhezustand::s_st2_g() {
    //if (!rzfsm) return;
    TriggerProcessingState processingstate = rzstatemachine->s_st2_g();
    handleDefaultExit(processingstate);
}

void Ruhezustand::s_rt1_g() {
    //if (!rzfsm) return;
    TriggerProcessingState processingstate = rzstatemachine->s_rt1_g();
    handleDefaultExit(processingstate);
}

void Ruhezustand::s_rt2_g() {
    //if (!rzfsm) return;
    TriggerProcessingState processingstate = rzstatemachine->s_rt2_g();
    handleDefaultExit(processingstate);
}

void Ruhezustand::s_e1_g() {
    std::cout << "State: Ruhezustand, Signal E1_G" << std::endl;
    //TriggerProcessingState processingstate = rzfsm->s_e1_g();

    //handleDefaultExit(processingstate);

    leavingState();
    new(this) EStop;
    enterByE1EntryPoint();
}

void Ruhezustand::s_e2_g() {
    std::cout << "State: Ruhezustand, Signal E2_G" << std::endl;
    //TriggerProcessingState processingstate = rzfsm->s_e1_g();

    //handleDefaultExit(processingstate);

    leavingState();
    new(this) EStop;
    enterByE2EntryPoint();
}