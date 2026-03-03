/**
 * @file Betriebszustand.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Betriebszustands.
 */

#include "Betriebszustand.h"
#include "Ruhezustand.h"
#include "Fehlerbehandlung.h"
#include "EStop.h"
//#include "../betriebszustandfsm/AufWSWarten.h"

#include <iostream>
using namespace std;

void Betriebszustand::enterByDefaultEntryPoint() {
    entry();
    bzstatemachine->enterViaPseudoStart();
}

void Betriebszustand::enterByDeepHistoryEntryPoint() {
    entry();
    bzstatemachine->enterViaDeepHistory();
}

void Betriebszustand::entry() {
    cout << "State: Betriebszustand rein" << endl;
    //    action->enterBetriebszustand();

    action->cmd_led_amp_grn_an();
    action->cmd_led_amp_rot_aus();
}

void Betriebszustand::exit() {
    cout << "State: Betriebszustand raus" << endl;
    action->cmd_led_amp_grn_aus();
    action->cmd_mtr_stop();
}

void Betriebszustand::leavingState() {
    //    if (bzfsm) {
    bzstatemachine->leavingState();
    //    }
    exit();
}

// ---------------------------------------------------------
// handleDefaultExit: Sub-FSM Exit auswerten
// ---------------------------------------------------------
void Betriebszustand::handleDefaultExit(const TriggerProcessingState &handled) {
    // F-Exit -> Fehlerbehandlung
    if (handled == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        cout << "ROOT: Explicit Exit 'F' -> Fehlerbehandlung" << endl;
        data->lsa_timer.pausiere();
        data->lsh_timer.pausiere();
        data->lsw_timer.pausiere();
        data->lse_timer.pausiere();
        data->lsr_timer.pausiere();
        if (data->getLetzterFehler().ort != FehlerOrt::ANDEREMACHINE) {
            action->cmd_partner_fehler_melden();
            cout << "ROOT: fehler an Partner gemeldet" << endl;
        }
        leavingState();
        new(this) Fehlerbehandlung;
        enterByDefaultEntryPoint();
    }
}

// ---------------------------------------------------------
// Root Trigger Delegation
// ---------------------------------------------------------

void Betriebszustand::s_stp_g() {
    std::cout << "Betriebszustand::ST_G called" << std::endl;
    leavingState();
    new(this) Ruhezustand;
    enterByDefaultEntryPoint();
}

void Betriebszustand::s_lsa1_ub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsa1_ub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lsa2_ub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsa2_ub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lsa_nub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsa_nub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lsh_ub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsh_ub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lsh_nub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsh_nub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lsw_ub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsw_ub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lsw_nub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsw_nub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lsr_ub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsr_ub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lsr_nub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lsr_nub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lse1_ub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lse1_ub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lse1_nub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lse1_nub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lse2_ub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lse2_ub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_lse2_nub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_lse2_nub();
    handleDefaultExit(ps);
}

void Betriebszustand::s_uebergabe_ws() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_uebergabe_ws();
    handleDefaultExit(ps);
}

void Betriebszustand::s_uebergabe_ws_fertig() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_uebergabe_ws_fertig();
    handleDefaultExit(ps);
}

void Betriebszustand::s_festo2_bereit() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_festo2_bereit();
    handleDefaultExit(ps);
}

void Betriebszustand::s_festo2_nbereit() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_festo2_nbereit();
    handleDefaultExit(ps);
}

void Betriebszustand::s_festo2_frei() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_festo2_frei();
    handleDefaultExit(ps);
}

void Betriebszustand::s_festo1_frei() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_festo1_frei();
    handleDefaultExit(ps);
}

void Betriebszustand::s_festo1_stoppen() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_festo1_stoppen();
    handleDefaultExit(ps);
}

void Betriebszustand::s_ws_an_festo2() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->s_ws_an_festo2();
    handleDefaultExit(ps);
}

void Betriebszustand::ec_vbdng_ub() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->ec_vbdng_ub();
    handleDefaultExit(ps);
}

void Betriebszustand::ec_ws_vrln() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->ec_ws_vrln();
    handleDefaultExit(ps);
}

void Betriebszustand::ec_ws_unb() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->ec_ws_unb();
    handleDefaultExit(ps);
}

void Betriebszustand::ec_abstand_ne() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->ec_abstand_ne();
    handleDefaultExit(ps);
}

void Betriebszustand::ec_rtsch_voll() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->ec_rtsch_voll();
    handleDefaultExit(ps);
}

void Betriebszustand::cmd_rtsch_nv() {
    TriggerProcessingState ps = bzstatemachine->cmd_rtsch_nv();
    handleDefaultExit(ps);
}

void Betriebszustand::cmd_festos_frei() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->cmd_festos_frei();
    handleDefaultExit(ps);
}

void Betriebszustand::t_lsa_lsh() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_lsa_lsh();
    handleDefaultExit(ps);
}

void Betriebszustand::t_lsh_lsw() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_lsh_lsw();
    handleDefaultExit(ps);
}

void Betriebszustand::t_lsw_lse() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_lsw_lse();
    handleDefaultExit(ps);
}

void Betriebszustand::t_lsw_lsr() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_lsw_lsr();
    handleDefaultExit(ps);
}

void Betriebszustand::t_lse1_lsa2() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_lse1_lsa2();
    handleDefaultExit(ps);
}

void Betriebszustand::t_q1_lsh() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_q1_lsh();
    handleDefaultExit(ps);
}

void Betriebszustand::t_lsa_q1() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_lsa_q1();
    handleDefaultExit(ps);
}

void Betriebszustand::t_toleranzzeit_lsa() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_toleranzzeit_lsa();
    handleDefaultExit(ps);
}

void Betriebszustand::t_toleranzzeit_lsh() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_toleranzzeit_lsh();
    handleDefaultExit(ps);
}

void Betriebszustand::t_toleranzzeit_lsw() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_toleranzzeit_lsw();
    handleDefaultExit(ps);
}

void Betriebszustand::t_toleranzzeit_lse() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_toleranzzeit_lse();
    handleDefaultExit(ps);
}

void Betriebszustand::t_ping_pong() {
    //    if (!bzfsm) return;
    TriggerProcessingState ps = bzstatemachine->t_ping_pong();
    handleDefaultExit(ps);
}

void Betriebszustand::s_e1_g() {
    std::cout << "Betriebszustand::E1_G called" << std::endl;
    leavingState();
    new(this) EStop;
    enterByE1EntryPoint();
}

void Betriebszustand::s_e2_g() {
    std::cout << "Betriebszustand::E2_G called" << std::endl;
    leavingState();
    new(this) EStop;
    enterByE2EntryPoint();
}

void Betriebszustand::s_partner_fehler_1() {
    cout << "ROOT: s_partner_fehler_1-> Fehlerbehandlung" << endl;

    if (data->getLetzterFehler().ort != FehlerOrt::ANDEREMACHINE) {
        action->cmd_partner_fehler_melden();
        cout << "ROOT: fehler an Partner gemeldet" << endl;
    }
    data->lsa_timer.pausiere();
    data->lsh_timer.pausiere();
    data->lsw_timer.pausiere();
    data->lse_timer.pausiere();
    data->lsr_timer.pausiere();
    leavingState();
    new(this) Fehlerbehandlung;
    enterByDefaultEntryPoint();
}

void Betriebszustand::s_partner_ok_1() {
    TriggerProcessingState ps = bzstatemachine->t_ping_pong();
    handleDefaultExit(ps);
}

void Betriebszustand::s_partner_fehler_2() {
    cout << "ROOT: s_partner_fehler_2 -> Fehlerbehandlung" << endl;

    if (data->getLetzterFehler().ort != FehlerOrt::ANDEREMACHINE) {
        action->cmd_partner_fehler_melden();
        cout << "ROOT: fehler an Partner gemeldet" << endl;
    }
    data->lsa_timer.pausiere();
    data->lsh_timer.pausiere();
    data->lsw_timer.pausiere();
    data->lse_timer.pausiere();
    data->lsr_timer.pausiere();
    leavingState();
    new(this) Fehlerbehandlung;
    enterByDefaultEntryPoint();
}

void Betriebszustand::s_partner_ok_2() {
    TriggerProcessingState ps = bzstatemachine->t_ping_pong();
    handleDefaultExit(ps);
}

void Betriebszustand::showState() {
    std::cout << "State: Betriebszustand" << std::endl;
}