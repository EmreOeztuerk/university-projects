/**
 * Trigger und Aktionen des Zustands "Weichezustand4".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "weichezustand4.h"
#include "../werkstueck_am_ende/werkstueck_am_ende2.h"

void Weichezustand4::entry() {
    action->enteredWeichezustand4();
    action->cmd_wch_an();
}

TriggerProcessingState Weichezustand4::s_lse2_ub() {
    cout << "Service Mode BCKalibrierung Weichezustand4: lse2_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) WerkstueckAmEnde2;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}