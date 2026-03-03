/**
 * @file RegionLSWBisLSEWartenAufLSE.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenAufLSE.
 */

#include "RegionLSWBisLSEWartenAufLSE.h"
#include "RegionLSWBisLSEWartenAufWSBeiLSE.h"
#include <iostream>
using namespace std;

void RegionLSWBisLSEWartenAufLSE::entry() {
    cout << "[LSW_BIS_LSE] State: WartenAufLSE::entry()" << endl;
    action->enteredRegionLSWBisLSEWartenAufLSE();
}

TriggerProcessingState RegionLSWBisLSEWartenAufLSE::t_lsw_lse() {
    cout << "[LSW_BIS_LSE] WartenAufLSE::t_lsw_lse() -> WartenAufWS_BeiLSE" << endl;
    leavingState();
    new(this) RegionLSWBisLSEWartenAufWSBeiLSE;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState RegionLSWBisLSEWartenAufLSE::s_lse1_ub() {
    if (data->gFesto1()) {
        // neuWS am Ende im falschen Zustand -> Fehler
        cout << "[LSW] WartenAufLSE::s_lse1_ub() -> Fehlerbehandlung neuWS" << endl;
        data->setFehler(Fehlerart::neuWS, FehlerOrt::LSE1);
        leavingState();
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    }
    return TriggerProcessingState::pending;
}

TriggerProcessingState RegionLSWBisLSEWartenAufLSE::s_lse2_ub() {
    if (data->gFesto2()) {
        cout << "[LSW] WartenAufLSE::s_lse2_ub() -> Fehlerbehandlung neuWS" << endl;
        data->setFehler(Fehlerart::neuWS, FehlerOrt::LSE2);
        leavingState();
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    }
    return TriggerProcessingState::pending;
}

void RegionLSWBisLSEWartenAufLSE::showState() {
    cout << "    Region LSW_BIS_LSE: WartenAufLSE" << endl;
}