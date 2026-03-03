/**
 * @file RegionLSANaechsteWSErlaubt.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von NaechsteWSErlaubt.
 */

#include "RegionLSANaechsteWSErlaubt.h"
#include "RegionLSABeginnSortierprozessF1.h"
#include <iostream>
using namespace std;

void RegionLSANaechsteWSErlaubt::entry() {
    cout << "[LSA] State: NaechsteWSErlaubt::entry()" << endl;
    action->cmd_led_q1_an();
}

TriggerProcessingState RegionLSANaechsteWSErlaubt::s_lsa1_ub() {
    cout << "[LSA] State RegionLSANaechsteWSErlaubt::s_lsa_ub() -> BeginnSortierprozessF1" << endl;
    data->lsh_timer.starte(
        data->t_lsa_lsh_s,
        data->t_lsa_lsh_l,
        [this]() {
            action->t_lsa_lsh();
            //std::cout << "-> TIMER: 100% Weg LSA_LSH erreicht." << std::endl;
        },
        [this]() {
            action->t_toleranzzeit_lsh();
            //std::cout << "-> FEHLER LSA_LSH: WS verloren BEI LSH!" << std::endl;
        }
    );
    data->addWS_F1();
    leavingState();
    new(this) RegionLSABeginnSortierprozessF1;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void RegionLSANaechsteWSErlaubt::showState() {
    cout << "    Region LSA: NaechsteWSErlaubt" << endl;
}