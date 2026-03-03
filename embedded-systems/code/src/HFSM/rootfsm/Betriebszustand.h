/**
 * @file Betriebszustand.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition des Betriebszustands (Running).
 *
 * Der Hauptzustand für die normale Werkstückbearbeitung.
 * Enthält eine komplexe Sub-FSM (`betriebszustandfsm`) mit orthogonalen Regionen.
 */

#ifndef BETRIEBSZUSTAND_H
#define BETRIEBSZUSTAND_H

#include "BaseState.h"
#include "../subcommon/TriggerProcessingState.h"

// Sub-FSM
//#include "../betriebszustandfsm/BetriebszustandBaseState.h"

class Betriebszustand : public BaseState {
private:
public:
    // EntryPoints
    void enterByDefaultEntryPoint() override;

    void enterByDeepHistoryEntryPoint() override;

    // Root Entry/Exit
    void entry() override;

    void exit() override;

    void leavingState() override;

    // Auswertung Rückgabewert der Sub-FSM
    void handleDefaultExit(const TriggerProcessingState &handled) override;

    // --------------------------------------------------------
    // Root Trigger: delegieren an bzfsm
    // --------------------------------------------------------
    void s_stp_g() override;

    void s_lsa1_ub() override;

    void s_lsa2_ub() override;

    void s_lsa_nub() override;

    void s_lsh_ub() override;

    void s_lsh_nub() override;

    void s_lsw_ub() override;

    void s_lsw_nub() override;

    void s_lsr_ub() override;

    void s_lsr_nub() override;

    void s_lse1_ub() override;

    void s_lse1_nub() override;

    void s_lse2_ub() override;

    void s_lse2_nub() override;

    void s_e1_g() override;

    void s_e2_g() override;

    void s_uebergabe_ws() override;

    void s_uebergabe_ws_fertig() override;

    void s_festo2_bereit() override;

    void s_festo2_nbereit() override;

    void s_festo2_frei() override;

    void s_festo1_frei() override;

    void s_festo1_stoppen() override;

    void s_ws_an_festo2() override;

    void ec_vbdng_ub() override;

    void ec_ws_vrln() override;

    void ec_ws_unb() override;

    void ec_abstand_ne() override;

    void ec_rtsch_voll() override;

    void cmd_rtsch_nv();

    void cmd_festos_frei() override;

    void s_partner_fehler_1() override;

    void s_partner_ok_1() override;

    void s_partner_fehler_2() override;

    void s_partner_ok_2() override;

    void t_lsa_lsh() override;

    void t_lsh_lsw() override;

    void t_lsw_lse() override;

    void t_lsw_lsr() override;

    void t_lse1_lsa2() override;

    void t_q1_lsh() override;

    void t_lsa_q1() override;

    void t_toleranzzeit_lsa() override;

    void t_toleranzzeit_lsh() override;

    void t_toleranzzeit_lsw() override;

    void t_toleranzzeit_lse() override;

    void t_ping_pong() override;

    void showState() override;
};

#endif /* BETRIEBSZUSTAND_H */