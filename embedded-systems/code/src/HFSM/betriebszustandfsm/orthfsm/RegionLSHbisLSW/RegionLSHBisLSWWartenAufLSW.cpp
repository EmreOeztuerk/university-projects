/**
 * @file RegionLSHBisLSWWartenAufLSW.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenAufLSW.
 */

#include "RegionLSHBisLSWWartenAufLSW.h"
#include "RegionLSHBisLSWWartenAufWSBeiLSW.h"
#include <iostream>
using namespace std;

void RegionLSHBisLSWWartenAufLSW::entry() {
    cout << "[LSH_BIS_LSW] State: WartenAufLSW::entry()" << endl;
    action->enteredRegionLSHBisLSWWartenAufLSW();
    data->enteredAussortieren = false;
}

TriggerProcessingState RegionLSHBisLSWWartenAufLSW::t_lsh_lsw() {
    cout << "[LSH_BIS_LSW] WartenAufLSW::t_lsh_lsw() -> WartenAufWS_BeiLSW" << endl;
    leavingState();
    new(this) RegionLSHBisLSWWartenAufWSBeiLSW;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState RegionLSHBisLSWWartenAufLSW::s_metal_an() {
    cout << "[LSH_BIS_LSW] WartenAufWS::s_metal_an() -> F-Exit (neuWS, LSH_BIS_LSW)" << endl;
    data->setFehler(Fehlerart::neuWS, FehlerOrt::LSH_BIS_LSW);
    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

TriggerProcessingState RegionLSHBisLSWWartenAufLSW::s_lsw_ub() {
    cout << "[LSH_BIS_LSW] WartenAufWS::s_lsw_ub() -> F-Exit (neuWS, LSH_BIS_LSW)" << endl;

    data->setFehler(Fehlerart::neuWS, FehlerOrt::LSH_BIS_LSW);
    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSHBisLSWWartenAufLSW::showState() {
    cout << "    Region LSH_BIS_LSW: WartenAufLSW" << endl;
}