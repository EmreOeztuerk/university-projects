/**
 * Trigger und Aktionen des Zustands "HSzustand3".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "hs_zustand3.h"
#include "../werkstueck_laeuft/werkstueck_laeuft4.h"

void HSzustand3::entry() {
    action->enteredHSzustand3();
    //data->zeitSpeichern(data->t_lsa_lsh, "");
}

TriggerProcessingState HSzustand3::s_lsh_nub() {
    cout << "Service Mode BCKalibrierung HSzustand3: lsh_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft4;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}