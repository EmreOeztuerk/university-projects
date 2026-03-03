/**
 * @file RegionLSHBisLSWWSAussortieren.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WSAussortieren.
 */

#include "RegionLSHbisLSWWSAussortieren.h"
#include "RegionLSHBisLSWWartenAufLSW.h"
#include <iostream>
using namespace std;

void RegionLSHBisLSWWSAussortieren::entry() {
    cout << "[LSH_BIS_LSW] State: WS_Aussortieren::entry()" << endl;
    action->enteredRegionLSHBisLSWWSAussortieren();
    action->cmd_aussortieren();
    data->werkstueckVerlaesstAnlage(); // loescht werkstuech, die asussortiert wurde    data->zeitStarten();
    if (data->festo2) {
        if (!data->erwarteteTypenQueue.empty()) {
            WerkstueckTyp erwartet = data->erwarteteTypenQueue.front();

            if (erwartet != WerkstueckTyp::UNBEKANNT) {
                std::cout << "[F2] Mismatch -> (WERKSTUECK_ANDERS)" << std::endl;
                action->wsAnders();
            } else {
                data->erwarteteTypenQueue.pop_front();
            }
        }
    }
    data->lsr_timer.starte(
        data->t_lsw_lsr_s,
        data->t_lsw_lsr_l,
        [this]() {
            action->t_lsw_lsr();
        },
        [this]() {
        }
    );
    data->enteredAussortieren = true;
    data->zeitStarten();
    //action->cmd_mtr_rechts_s();
    //data->set(data->motorSchnell, true);
}

TriggerProcessingState RegionLSHBisLSWWSAussortieren::s_lsr_ub() {
    cout << "[LSH_BIS_LSW] State: WS_Aussortieren::s_lsr_ub()" << endl;
    if (data->gFesto1()) data->loescheWS_F1();
    if (data->gFesto2()) data->loescheWS_F2();
    data->zeitSpeichern("Reisezeit_LSW_LSR");
    leavingState();
    new(this) RegionLSHBisLSWWartenAufLSW;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState RegionLSHBisLSWWSAussortieren::t_lsw_lsr() {
    cout << "[LSH_BIS_LSW] State: WS_Aussortieren::t_lsw_lsr()" << endl;
    data->setFehler(Fehlerart::verlorenWS, FehlerOrt::LSR);


    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSHBisLSWWSAussortieren::showState() {
    cout << "    Region LSH_BIS_LSW: WS_Aussortieren" << endl;
}