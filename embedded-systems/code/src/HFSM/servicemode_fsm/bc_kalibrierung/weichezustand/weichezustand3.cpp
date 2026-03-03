/**
 * Trigger und Aktionen des Zustands "Weichezustand3".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "weichezustand3.h"
#include "../ende_kalibrierung/ende_kalibrierung_ae.h"

void Weichezustand3::entry() {
    action->enteredWeichezustand3();
    action->cmd_wch_an();
}

void Weichezustand3::exit() {
    action->cmd_wch_aus();
}

TriggerProcessingState Weichezustand3::s_lse1_ub() {
    cout << "Service Mode BCKalibrierung Weichezustand2: lse1_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) EndeKalibrierungAE;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}