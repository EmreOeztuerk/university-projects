/**
 * @file RegionLSABeginnsortierprozessF2.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von BeginnSortierprozessF2.
 */

#include "RegionLSABeginnsortierprozessF2.h"
#include "RegionLSAWartenAufLSA.h"
#include "RegionLSAWartenAufWSBeiLSA.h"
#include <iostream>

using namespace std;

void RegionLSABeginnSortierprozessF2::entry() {
    cout << "[LSA] State: BeginnSortierprozessF2::entry()" << endl;
    action->enteredRegionLSABeginnSortierprozessF2();
}

TriggerProcessingState RegionLSABeginnSortierprozessF2::t_lse1_lsa2() {
    cout << "[LSA] BeginnSortierprozessF2::t_lse1_lsa2() -> WartenAufWSBeiLSA" << endl;
    leavingState();
    new(this) RegionLSAWartenAufWSBeiLSA;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}


TriggerProcessingState RegionLSABeginnSortierprozessF2::s_lsa2_ub() {
    cout << "[LSA] BeginnSortierprozessF2::s_lsa_ub() -> F-Exit nueWS" << endl;
    data->setFehler(Fehlerart::neuWS, FehlerOrt::LSA);

    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSABeginnSortierprozessF2::showState() {
    cout << "    Region LSA: BeginnSortierprozessF2" << endl;
}