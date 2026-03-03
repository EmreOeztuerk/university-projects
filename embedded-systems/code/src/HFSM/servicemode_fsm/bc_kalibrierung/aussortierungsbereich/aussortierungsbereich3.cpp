/**
 * Trigger und Aktionen des Zustands "Aussortierungsbereich3".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "aussortierungsbereich3.h"
#include "../weichezustand/weichezustand3.h"
#include "../rutschezustand/rutschezustand3.h"
#include "../ende_kalibrierung/ende_kalibrierung_ae.h"

void Aussortierungsbereich3::entry() {
    action->enteredAussortierungsbereich3();
}

void Aussortierungsbereich3::exit() {
    data->zeitStarten();
}

TriggerProcessingState Aussortierungsbereich3::s_lsw_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich3: lsw_ub" << endl;

    if (data->sortTyp == SortTyp::WEICHE) {
        exitPointDefaultLeaveState();
        new(this) Weichezustand3;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}

TriggerProcessingState Aussortierungsbereich3::s_lsr1_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich3: lsr1_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) Rutschezustand3;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Aussortierungsbereich3::s_lse1_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich3: lse1_ub" << endl;

    exitPointDefaultLeaveState();
    data->setSortTyp(SortTyp::AUSWERFER);
    new(this) EndeKalibrierungAE;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}