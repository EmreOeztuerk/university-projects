/**
 * @file RegionLSWBisLSEWartenAufErlaubnisVonF2.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands WartenAufErlaubnisVonF2.
 */

#include "RegionLSWBisLSEWartenAufErlaubnisVonF2.h"
#include "RegionLSWBisLSEWartenAufLSE.h"
#include <iostream>
using namespace std;

void RegionLSWBisLSEWartenAufErlaubnisVonF2::entry() {
    cout << "[LSW_BIS_LSE] State: WartenAufErlaubnisVonF2::entry()" << endl;
    data->enteredWartenAufErlaubnisVonF2 = true;
    action->enteredRegionLSWBisLSEWartenAufErlaubnisVonF2();
    action->cmd_mtr_stop();

    data->lsa_timer.pausiere();
    data->lsh_timer.pausiere();
    data->lsw_timer.pausiere();
    data->lse_timer.pausiere();
}

void RegionLSWBisLSEWartenAufErlaubnisVonF2::exit() {
    cout << "[LSW_BIS_LSE] State: WartenAufErlaubnisVonF2::exit()" << endl;
    if (data->enteredHoheMessen) {
        action->cmd_mtr_rechts_l();
    } else { action->cmd_mtr_rechts_s(); }

    data->lsa_timer.starte(
        data->t_lse1_lsa2_s,
        data->t_lse1_lsa2_l,
        [this]() {
            action->t_lse1_lsa2();
            //std::cout << "-> TIMER: 100% Weg LE1_LSA2 erreicht." << std::endl;
        },
        [this]() {
            action->t_toleranzzeit_lsa();
            //std::cout << "-> FEHLER LSE1_LSA2: WS verloren!" << std::endl;
        }
    );
    data->enteredWartenAufErlaubnisVonF2 = false;

    data->lsa_timer.fortsetzen();
    data->lsh_timer.fortsetzen();
    data->lsw_timer.fortsetzen();
    data->lse_timer.fortsetzen();
}

TriggerProcessingState RegionLSWBisLSEWartenAufErlaubnisVonF2::s_festo2_bereit() {
    cout << "[LSW_BIS_LSE] WartenAufErlaubnisVonF2::s_festo2_bereit() -> WartenAufLSE" << endl;
    data->loescheWS_F1();
    action->s_ws_an_festo2();
    leavingState();
    new(this) RegionLSWBisLSEWartenAufLSE;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void RegionLSWBisLSEWartenAufErlaubnisVonF2::showState() {
    cout << "    Region LSW_BIS_LSE: WartenAufErlaubnisVonF2" << endl;
}