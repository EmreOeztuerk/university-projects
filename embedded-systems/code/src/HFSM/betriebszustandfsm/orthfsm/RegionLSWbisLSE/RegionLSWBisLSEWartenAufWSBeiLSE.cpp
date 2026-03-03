/**
 * @file RegionLSWBisLSEWartenAufWSBeiLSE.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenAufWSBeiLSE.
 */

#include "RegionLSWBisLSEWartenAufWSBeiLSE.h"
#include "RegionLSWBisLSEWartenBisWSEntfernt.h"
#include "RegionLSWBisLSEWartenAufErlaubnisVonF2.h"
#include "RegionLSWBisLSEWartenAufLSE.h"
#include <iostream>
using namespace std;

void RegionLSWBisLSEWartenAufWSBeiLSE::entry() {
    cout << "[LSW_BIS_LSE] State: WartenAufWSBeiLSE::entry()" << endl;
    action->enteredRegionLSWBisLSEWartenAufWSBeiLSE();
}


TriggerProcessingState RegionLSWBisLSEWartenAufWSBeiLSE::s_lse1_ub() {
    cout << "[LSW_BIS_LSE] WartenAufWSBeiLSE::s_lse1_ub()" << endl;

    if (data->gFesto1() && data->gF2Bereit()) {
        cout << "[LSW_BIS_LSE] WartenAufWSBeiLSE::s_lse1_ub() -> WartenAufLSE" << endl;

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
        data->loescheWS_F1();
        action->s_ws_an_festo2();
        leavingState();
        new(this) RegionLSWBisLSEWartenAufLSE;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    } else if (data->gFesto1() && !data->gF2Bereit()) {
        cout << "[LSW_BIS_LSE] WartenAufWSBeiLSE::s_lse1_ub() -> WartenAufErlaubnisVonF2" << endl;

        leavingState();
        new(this) RegionLSWBisLSEWartenAufErlaubnisVonF2;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }
    // Wenn keine Guard passt -> nicht zuständig
    return TriggerProcessingState::pending;
}

TriggerProcessingState RegionLSWBisLSEWartenAufWSBeiLSE::s_lse2_ub() {
    cout << "[LSW_BIS_LSE] WartenAufWSBeiLSE::s_lse2_ub() " << endl;

    // Guard 2: Wir sind Festo2 (oder behandeln F2-Ende)
    if (data->gFesto2()) {
        cout << "[LSW_BIS_LSE] WartenAufWSBeiLSE::s_lse2_ub() -> WartenBisWSEntfernt" << endl;


        leavingState();
        new(this) RegionLSWBisLSEWartenBisWSEntfernt;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }

    // Wenn keine Guard passt -> nicht zuständig
    return TriggerProcessingState::pending;
}

TriggerProcessingState RegionLSWBisLSEWartenAufWSBeiLSE::t_toleranzzeit_lse() {
    // Timeout im Endbereich => verlorenWS
    cout << "[LSW] WartenAufWSBeiLSE::t_toleranzzeit_lse() -> Fehlerbehandlung verlorenWS" << endl;
    data->setFehler(Fehlerart::verlorenWS, FehlerOrt::LSE1);
    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSWBisLSEWartenAufWSBeiLSE::showState() {
    cout << "    Region LSW_BIS_LSE: WartenAufWSBeiLSE" << endl;
}