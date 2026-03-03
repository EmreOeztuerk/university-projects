/**
 * Trigger und Aktionen des Zustands "HSzustand4".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "hs_zustand4.h"
#include "../werkstueck_laeuft/werkstueck_laeuft6.h"

void HSzustand4::entry() {
    action->enteredHSzustand4();
    //data->zeitSpeichern(data->t_lsa_lsh, "");
}

TriggerProcessingState HSzustand4::s_lsh_nub() {
    cout << "Service Mode BCKalibrierung HSzustand4: lsh_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckLaeuft6;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}