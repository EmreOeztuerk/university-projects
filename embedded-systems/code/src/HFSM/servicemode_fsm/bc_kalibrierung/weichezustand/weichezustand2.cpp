/**
 * Trigger und Aktionen des Zustands "Weichezustand2".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "weichezustand2.h"
#include "../werkstueck_am_ende/werkstueck_am_ende.h"

void Weichezustand2::entry() {
    action->enteredWeichezustand2();
    action->cmd_wch_an(); // durchlassen
}

TriggerProcessingState Weichezustand2::s_lse2_ub() {
    cout << "Service Mode BCKalibrierung Weichezustand2: lse2_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckAmEnde;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}