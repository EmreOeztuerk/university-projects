/**
 * Trigger und Aktionen des Zustands "WerkstueckLaeuft".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_laeuft.h"

void WerkstueckLaeuftWC::entry() {
    action->enteredWerkstueckLaeuftWC();
    data->zeitStarten();
}

TriggerProcessingState WerkstueckLaeuftWC::s_lsh_ub() {
    cout << "Service Mode WCKalibrierung WerkstueckLaeuft: lsh_ub" << endl;

    exitPointDefaultLeaveState();
    if (data->letztesMal) {
        //data->zeitSpeichern(data->t_lsa_lsh_lsw, "");
        return TriggerProcessingState::endstatereached;
    } else {
        new(this) HSzustandWC;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
}