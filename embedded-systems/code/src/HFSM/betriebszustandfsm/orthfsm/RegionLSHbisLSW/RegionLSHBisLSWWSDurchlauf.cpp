/**
 * @file RegionLSHBisLSWWSDurchlauf.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WSDurchlauf.
 */

#include "RegionLSHBisLSWWSDurchlauf.h"
#include "REGIONLSHBISLSWWARTENAUFLSW.H"
#include <iostream>
using namespace std;

void RegionLSHBisLSWWSDurchlauf::entry() {
    cout << "[LSH_BIS_LSW] State: WS_Durchlauf::entry()" << endl;
    action->enteredRegionLSHBisLSWWSDurchlauf();

    Werkstueck ws = data->werkstueckListe.front();
    bool aussortiert = ws.istAusschuss();
    int wsTyp2 = 0;

    if (aussortiert && data->gFesto1()) {
        wsTyp2 = Signal::WERKSTUECK_UNBEKANNT;
    } else {
        wsTyp2 = data->getTypCodeForSending();
    }

    if (wsTyp2 != -1) {
        // Code: S_UEBERGABE_WS, Value: wsTyp2

        action->wsTypSenden(wsTyp2);
        std::cout << "[F1 -> F2] WS Erwartet bei F2: " << wsTyp2 << std::endl;
    } else {
        std::cerr << "[F1 ERROR] WS Erwartet nicht an F2 gesendet!" << std::endl;
    }
    data->lse_timer.starte(
        data->t_lsw_lse_s,
        data->t_lsw_lse_l,
        [this]() {
            action->t_lsw_lse();
            //std::cout << "-> TIMER: 100% Weg LSW_LSE erreicht." << std::endl;
        },
        [this]() {
            action->t_toleranzzeit_lse();
            //std::cout << "-> FEHLER LSW_LSE: WS verloren!" << std::endl;
        }
    );

    if (!aussortiert && data->festo1) {
        data->advanceOrder();
    } else if (data->gFesto2()) {
        data->advanceOrder();
    }

    action->cmd_durchlassen();
    data->werkstueckVerlaesstAnlage();
    leavingState();
    new(this) RegionLSHBisLSWWartenAufLSW;
    enterByDefaultEntryPoint();
}

void RegionLSHBisLSWWSDurchlauf::showState() {
    cout << "    Region LSH_BIS_LSW: WS_Durchlauf" << endl;
}