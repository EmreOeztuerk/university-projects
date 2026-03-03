/**
 * @file RegionLSHWartenAufLSH.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands WartenAufLSH.
 */

#include "RegionLSHWartenAufLSH.h"
#include "RegionLSHWartenAufWSBeiLSH.h"
#include <iostream>
using namespace std;

void RegionLSHWartenAufLSH::entry() {
    cout << "[LSH] State: WartenAufLSH::entry()" << endl;
    action->enteredRegionLSHWartenAufLSH();
}

TriggerProcessingState RegionLSHWartenAufLSH::t_lsa_lsh() {
    cout << "[LSH] WartenAufLSH::t_lsa_lsh() -> WartenAufWSBeiLSH" << endl;
    leavingState();
    new(this) RegionLSHWartenAufWSBeiLSH;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState RegionLSHWartenAufLSH::s_lsh_ub() {
    cout << "[LSH] WartenAufLSH::s_lsh_ub() -> Fehlerbehandlung neuWS" << endl;
    if (!data->enteredWartenAufErlaubnisVonF2)action->cmd_mtr_rechts_l();
    leavingState();
    data->setFehler(Fehlerart::neuWS, FehlerOrt::LSH);
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSHWartenAufLSH::showState() {
    cout << "    Region LSH: WartenAufLSH" << endl;
}