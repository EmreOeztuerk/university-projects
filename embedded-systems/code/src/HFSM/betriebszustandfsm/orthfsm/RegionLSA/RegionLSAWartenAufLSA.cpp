/**
 * @file RegionLSAWartenAufLSA.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands WartenAufLSA.
 */

#include "RegionLSAWartenAufLSA.h"
#include "RegionLSABeginnsortierprozessF1.h"
#include "RegionLSABeginnsortierprozessF2.h"

#include <iostream>
using namespace std;

void RegionLSAWartenAufLSA::entry() {
    cout << "[LSA] State: WartenAufLSA::entry()" << endl;
    action->enteredRegionLSAWartenAufLSA();
    if (data->gFesto1()) {
        cout << "[LSA] Wechsel in Beginnsortierprozess" << endl;
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
        //return TriggerProcessingState::consumed;
    }
    if (data->gFesto2() && !data->gF2Voll()) {
        action->cmd_mtr_rechts_s();


        cout << "[LSA] WartenAufLSA::s_lse1_ub() aufgerufen" << endl;
        leavingState();
        new(this) RegionLSABeginnSortierprozessF2;
        enterByDefaultEntryPoint();
    }
}

TriggerProcessingState RegionLSAWartenAufLSA::s_lsa1_ub() {
    cout << "[LSA] WartenAufLSA::s_lsa_ub() aufgerufen, festo1:" << data->gFesto1() << endl;
    if (data->gFesto1()) {
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
        //action->cmd_mtr_rechts_s();
        cout << "[LSA] WartenAufLSA::s_lsa_ub() aufgerufen" << endl;

        leavingState();
        new(this) RegionLSABeginnSortierprozessF1;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}

TriggerProcessingState RegionLSAWartenAufLSA::s_lse1_ub() {
    if (data->gFesto2() && !data->gF2Voll()) {
        action->cmd_mtr_rechts_s();


        cout << "[LSA] WartenAufLSA::s_lse1_ub() aufgerufen" << endl;
        leavingState();
        new(this) RegionLSABeginnSortierprozessF2;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}

TriggerProcessingState RegionLSAWartenAufLSA::s_lsa2_ub() {
    cout << "[LSA] RegionLSAWartenAufLSA::s_lsa_ub() -> F-Exit nueWS" << endl;
    data->setFehler(Fehlerart::neuWS, FehlerOrt::LSA);

    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSAWartenAufLSA::showState() {
    cout << "    Region LSA: WartenAufLSA" << endl;
}