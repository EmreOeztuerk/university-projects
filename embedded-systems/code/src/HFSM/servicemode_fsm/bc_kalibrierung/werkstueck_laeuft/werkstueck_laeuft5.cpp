/**
 * Trigger und Aktionen des Zustands "WerkstueckLaeuft5".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_laeuft5.h"
#include "../werkstueck_laeuft/werkstueck_laeuft6.h"

void WerkstueckLaeuft5::entry() {
    action->enteredWerkstueckLaeuft5();
    //data->zeitSpeichern(data->t_lse1_lsa2, "");
}

TriggerProcessingState WerkstueckLaeuft5::s_lsa2_nub() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft5: lsa2_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft6;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}