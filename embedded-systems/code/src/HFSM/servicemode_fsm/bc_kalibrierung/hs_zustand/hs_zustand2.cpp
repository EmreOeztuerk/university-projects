/**
 * Trigger und Aktionen des Zustands "HSzustand2".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "hs_zustand2.h"
#include "../werkstueck_laeuft/werkstueck_laeuft3.h"

void HSzustand2::entry() {
    action->enteredHSzustand2();
    //data->zeitSpeichern(data->t_lsa_lsh, "");
}

TriggerProcessingState HSzustand2::s_lsh_nub() {
    cout << "Service Mode BCKalibrierung HSzustand2: lsh_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft3;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}