/**
 * Trigger und Aktionen des Zustands "Aussortierungsbereich2".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "aussortierungsbereich2.h"
#include "../weichezustand/weichezustand2.h"
#include "../werkstueck_am_ende/werkstueck_am_ende.h"

void Aussortierungsbereich2::entry() {
    action->enteredAussortierungsbereich2();
}

void Aussortierungsbereich2::exit() {
    data->zeitStarten();
}

TriggerProcessingState Aussortierungsbereich2::s_lsw_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich2: lsw_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) Weichezustand2;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Aussortierungsbereich2::s_lse2_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich2: lse2_ub" << endl;

    exitPointDefaultLeaveState();
    data->setSortTyp(SortTyp::AUSWERFER);
    new(this) WerkstueckAmEnde;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}