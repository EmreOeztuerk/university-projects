/**
 * Trigger und Aktionen des Zustands "Wartezustand".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "wartezustand.h"

void Servicemode_Wartezustand::entry() {
    action->enteredServicemode_Wartezustand();
}

TriggerProcessingState Servicemode_Wartezustand::s_st1_g() {
    cout << "Service Mode Wartezustand: st1_gedrueckt" << endl;

    exitPointDefaultLeaveState();
    new(this) BCKalibrierung_BaseState;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Servicemode_Wartezustand::s_stp_g() {
    cout << "Service Mode Wartezustand: stp1_gedrueckt" << endl;

    exitPointDefaultLeaveState();
    new(this) Ruhezustand;
    entryPointDefaultEnter();
    return TriggerProcessingState::endstatereached;
}