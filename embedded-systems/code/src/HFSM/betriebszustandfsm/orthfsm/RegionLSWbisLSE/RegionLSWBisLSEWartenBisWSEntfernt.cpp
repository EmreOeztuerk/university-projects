/**
 * @file RegionLSWBisLSEWartenBisWSEntfernt.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenBisWSEntfernt.
 */

#include "RegionLSWBisLSEWartenBisWSEntfernt.h"
#include "RegionLSWBisLSEWartenAufLSE.h"
#include <iostream>
using namespace std;

void RegionLSWBisLSEWartenBisWSEntfernt::entry() {
    cout << "[LSW_BIS_LSE] State: WartenBisWSEntfernt::entry()" << endl;
    data->enteredWartenBisWSEntfernt = true;
    action->enteredRegionLSWBisLSEWartenBisWSEntfernt();
    action->cmd_mtr_stop();
    action->s_festo2_nbereit();
    data->lsa_timer.pausiere();
    data->lsh_timer.pausiere();
    data->lsw_timer.pausiere();
    data->lse_timer.pausiere();
}

void RegionLSWBisLSEWartenBisWSEntfernt::exit() {
    cout << "[LSW_BIS_LSE] State: WartenBisWS_Entfernt::exit()" << endl;
    action->cmd_mtr_rechts_s();
    data->enteredWartenBisWSEntfernt = false;
    //action->s_festo2_bereit();
}

TriggerProcessingState RegionLSWBisLSEWartenBisWSEntfernt::s_lse2_nub() {
    cout << "[LSW_BIS_LSE] WartenBisWSEntfernt::s_lse2_nub() -> WartenAufLSE" << endl;
    data->loescheWS_F2();

    data->lsa_timer.fortsetzen();
    data->lsh_timer.fortsetzen();
    data->lsw_timer.fortsetzen();
    data->lse_timer.fortsetzen();

    leavingState();
    new(this) RegionLSWBisLSEWartenAufLSE;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void RegionLSWBisLSEWartenBisWSEntfernt::showState() {
    cout << "    Region LSW_BIS_LSE: WartenBisWSEntfernt" << endl;
}