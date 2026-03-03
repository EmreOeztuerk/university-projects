/**
 * @file RegionLSAWartenAufWSBeiLSA.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands WartenAufWSBeiLSA.
 */

#include "RegionLSAWartenAufWSBeiLSA.h"
#include "RegionLSABeginnSortierprozessF2.h"
#include <iostream>
using namespace std;

void RegionLSAWartenAufWSBeiLSA::entry() {
    cout << "[LSA] State: WartenAufWSBeiLSA::entry()" << endl;
    action->enteredRegionLSAWartenAufWSBeiLSA();
    if (data->FehlerVerlorenBeiLAS2) {
        data->FehlerVerlorenBeiLAS2 = false;
        cout << "[LSA] RegionLSAWartenAufWSBeiLSA::VerlorenWS gefunden -> RegionLSABeginnSortierprozessF2" << endl;
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

        leavingState();

        new(this) RegionLSABeginnSortierprozessF2;
        enterByDefaultEntryPoint();
        data->addWS_F2();
    }
}

TriggerProcessingState RegionLSAWartenAufWSBeiLSA::s_lsa2_ub() {
    cout << "[LSA] WartenAufWSBeiLSA::s_lsa2_ub()" << endl;
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

    leavingState();
    data->addWS_F2();
    if (data->festo1Frei) action->s_festo1_stoppen();
    new(this) RegionLSABeginnSortierprozessF2;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState RegionLSAWartenAufWSBeiLSA::t_toleranzzeit_lsa() {
    cout << "[LSA] WartenAufWSBeiLSA::t_toleranzzeit() -> F-Exit (TODO: sendFehler(verlorenWS, LSA2))" << endl;
    data->setFehler(Fehlerart::verlorenWS, FehlerOrt::LSA2);
    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSAWartenAufWSBeiLSA::showState() {
    cout << "    Region LSA: WartenAufWSBeiLSA" << endl;
}