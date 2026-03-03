/**
 * Trigger und Aktionen des Zustands "Aussortierungsbereich".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "aussortierungsbereich.h"
#include "../weichezustand/weichezustand.h"
#include "../rutschezustand/rutschezustand.h"
#include "../ende_kalibrierung/ende_kalibrierung_lang_ae.h"

void Aussortierungsbereich::entry() {
    action->enteredAussortierungsbereich();
}

void Aussortierungsbereich::exit() {
    data->zeitStarten();
}

TriggerProcessingState Aussortierungsbereich::s_lsw_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich: lsw_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) Weichezustand;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Aussortierungsbereich::s_lsr1_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich: lsr1_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) Rutschezustand;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Aussortierungsbereich::s_lse1_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich: lse1_ub" << endl;

    data->setSortTyp(SortTyp::AUSWERFER);
    exitPointDefaultLeaveState();
    new(this) EndeKalibrierungLangAE;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}