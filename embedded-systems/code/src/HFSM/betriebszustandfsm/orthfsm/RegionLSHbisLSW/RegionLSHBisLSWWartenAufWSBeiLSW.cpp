/**
 * @file RegionLSHBisLSWWartenAufWSBeiLSW.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenAufWSBeiLSW.
 */

#include "RegionLSHBisLSWWartenAufWSBeiLSW.h"
#include "RegionLSHBisLSWKomplettTypErkennen.h"
#include "RegionLSHBisLSWWSDurchlauf.h"
#include "RegionLSHBisLSWWSAussortieren.h"
#include <iostream>
using namespace std;

void RegionLSHBisLSWWartenAufWSBeiLSW::entry() {
    cout << "[LSH_BIS_LSW] State: WartenAufWS_BeiLSW::entry()" << endl;
    action->enteredRegionLSHBisLSWWartenAufWSBeiLSW();
}

TriggerProcessingState RegionLSHBisLSWWartenAufWSBeiLSW::s_metal_an() {
    cout << "[LSH_BIS_LSW] WartenAufWS_BeiLSW::s_metal_an() -> KomplettTypErkennen" << endl;
    leavingState();
    new(this) RegionLSHBisLSWKomplettTypErkennen;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState RegionLSHBisLSWWartenAufWSBeiLSW::t_toleranzzeit_lsw() {
    cout << "[LSH] WartenAufWSBeiLSW::t_toleranzzeit_lsw() -> Fehlerbehandlung verlorenWS" << endl;
    data->setFehler(Fehlerart::verlorenWS, FehlerOrt::LSW);
    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

TriggerProcessingState RegionLSHBisLSWWartenAufWSBeiLSW::s_lsw_ub() {
    data->ueberpruefeReihenfolge();

    // 1) Durchlauf:
    // LSW_UB[[typ == naechteTyp] || [[Festo1]&& [typ != naechteTyp] & [!rutsche2_voll]]] -> WS_Durchlauf
    if (data->gLSWUBDurchlauf()) {
        leavingState();
        new(this) RegionLSHBisLSWWSDurchlauf;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }

    // 2) Aussortieren:
    // LSW_UB[[[typ != naechteTyp] &&  [!rutsche_voll]] || [Festo2],[!gleicheTyp]] -> WS_Aussortieren
    if (data->gLSWUBAussortieren()) {
        leavingState();
        new(this) RegionLSHBisLSWWSAussortieren;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }

    // 3) Fehlerfall hat Priorität:
    // LSW_UB[[typ != naechteTyp] && [rutsche2_voll]] /sendFehler(rutscheVoll, LSR) -> F-Exit

    if (data->gLSWUBFehlerRutsche2Voll()) {
        cout << "[LSH] WartenAufWSBeiLSW::s_lsw_ub() -> Fehlerbehandlung rutscheVoll" << endl;
        data->setFehler(Fehlerart::rutscheVoll, FehlerOrt::LSR);
        leavingState();
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    }


    // Wenn keine Guard passt → nicht zuständig
    return TriggerProcessingState::pending;
}

void RegionLSHBisLSWWartenAufWSBeiLSW::showState() {
    cout << "    Region LSH_BIS_LSW: WartenAufWS_BeiLSW" << endl;
}