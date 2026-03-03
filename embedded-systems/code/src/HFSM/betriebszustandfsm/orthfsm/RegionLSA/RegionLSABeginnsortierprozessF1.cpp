/**
* @file RegionLSABeginnsortierprozessF1.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von BeginnSortierprozessF1.
 */

#include "RegionLSABeginnsortierprozessF1.h"
#include "RegionLSANaechsteWSErlaubt.h"
#include <iostream>
using namespace std;

void RegionLSABeginnSortierprozessF1::entry() {
    cout << "[LSA] State: BeginnSortierprozessF1::entry()" << endl;
    action->enteredRegionLSABeginnSortierprozessF1();
    action->cmd_led_q1_aus();
}

TriggerProcessingState RegionLSABeginnSortierprozessF1::t_lsa_q1() {
    cout << "[LSA] BeginnSortierprozessF1::t_lsa_q1() -> NaechsteWSErlaubt" << endl;
    leavingState();
    new(this) RegionLSANaechsteWSErlaubt;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState RegionLSABeginnSortierprozessF1::t_lsa_lsh() {
    cout << "[LSA] BeginnSortierprozessF1::t_lsa_q1() -> NaechsteWSErlaubt" << endl;
    leavingState();
    new(this) RegionLSANaechsteWSErlaubt;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}


TriggerProcessingState RegionLSABeginnSortierprozessF1::s_lsa1_ub() {
    cout << "[LSA] BeginnSortierprozessF1::s_lsa_ub() -> Fehelernehandlung" << endl;
    data->setFehler(Fehlerart::neuWS, FehlerOrt::LSA1);
    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSABeginnSortierprozessF1::showState() {
    cout << "    Region LSA: BeginnSortierprozessF1" << endl;
}