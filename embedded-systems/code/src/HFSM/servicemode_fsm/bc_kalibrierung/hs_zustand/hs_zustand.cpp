/**
 * Trigger und Aktionen des Zustands "HSzustand".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "hs_zustand.h"
#include "../werkstueck_laeuft/werkstueck_laeuft.h"

void HSzustand::entry() {
    action->enteredHSzustand();
    //data->zeitSpeichern(data->t_lsa_lsh, "");
}

TriggerProcessingState HSzustand::s_lsh_nub() {
    cout << "Service Mode BCKalibrierung HSzustand: lsh_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}