/**
 * Trigger und Aktionen des Zustands "Weichezustand".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "weichezustand.h"
#include "../ende_kalibrierung/ende_kalibrierung_lang_ae.h"

void Weichezustand::entry() {
    action->enteredWeichezustand();
    action->cmd_wch_an(); // durchlassen
}

TriggerProcessingState Weichezustand::s_lse1_ub() {
    cout << "Service Mode BCKalibrierung Weichezustand: lse1_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) EndeKalibrierungLangAE;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}