/**
 * @file RegionLSHBisLSWKomplettTypErkennen.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Typerkennung.
 */

#include "RegionLSHBisLSWKomplettTypErkennen.h"
#include "RegionLSHBisLSWWSDurchlauf.h"
#include "RegionLSHBisLSWWSAussortieren.h"
#include <iostream>
using namespace std;

void RegionLSHBisLSWKomplettTypErkennen::entry() {
    cout << "[LSH_BIS_LSW] State: KomplettTypErkennen::entry()" << endl;
    action->enteredRegionLSHBisLSWKomplettTypErkennen();
    data->ueberpruefeReihenfolge();
    data->hatMetall = false;
}

TriggerProcessingState RegionLSHBisLSWKomplettTypErkennen::s_lsw_ub() {
    cout << "[LSH_BIS_LSW] KomplettTypErkennen::s_lsw_ub() -> WS_Durchlauf (TODO: Guards & WS_Aussortieren)" << endl;

    bool gut = data->gTypGleichNaechster();

    if ((gut || data->gFesto1()) && (data->gTypUngleichNaechster() && !data->gRutsche2Voll())) {
        leavingState();
        new(this) RegionLSHBisLSWWSDurchlauf();
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }

    if ((gut && data->gRutscheVoll()) || (data->gFesto2() && data->gGleicheTyp())) {
        leavingState();
        new(this) RegionLSHBisLSWWSAussortieren();
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }

    if (gut && data->gRutsche2Voll()) {
        leavingState();
        data->setFehler(Fehlerart::rutscheVoll, FehlerOrt::LSR);
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    }

    // Wenn keine Guard passt -> nicht zuständig
    return TriggerProcessingState::pending;
}

void RegionLSHBisLSWKomplettTypErkennen::showState() {
    cout << "    Region LSH_BIS_LSW: KomplettTypErkennen" << endl;
}