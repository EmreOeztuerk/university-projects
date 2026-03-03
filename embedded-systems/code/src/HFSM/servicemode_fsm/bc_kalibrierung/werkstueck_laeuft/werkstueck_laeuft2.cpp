/**
 * Trigger und Aktionen des Zustands "WerkstueckLaeuft2".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_laeuft2.h"
#include "werkstueck_laeuft3.h"

void WerkstueckLaeuft2::entry() {
    action->enteredWerkstueckLaeuft2();
    //data->zeitSpeichern(data->t_lse1_lsa2, "");
}

TriggerProcessingState WerkstueckLaeuft2::s_lsa2_nub() {
    cout << "Service Mode BCKalibrierung WerkstueckLaeuft2: lsa2_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft3;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}