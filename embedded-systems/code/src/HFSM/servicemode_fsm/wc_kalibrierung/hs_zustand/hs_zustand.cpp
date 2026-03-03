/**
 * Trigger und Aktionen des Zustands "HSzustand".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "hs_zustand.h"
#include "../anfang_wc_kalibrierung/anfang_wc_kalibrierung.h"

void HSzustandWC::entry() {
    action->enteredHSzustandWC();
    //data->zeitSpeichern(data->t_lsa_lsh_lsw, "");
    data->set(data->letztesMal, true);
}

TriggerProcessingState HSzustandWC::s_metal_an() {
    cout << "Service Mode WCKalibrierung HSzustand: ms_an" << endl;

    exitPointDefaultLeaveState();
    new(this) AnfangWCKalibrierung;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}