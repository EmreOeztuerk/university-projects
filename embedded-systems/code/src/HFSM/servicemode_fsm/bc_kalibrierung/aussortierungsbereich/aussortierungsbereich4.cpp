/**
 * Trigger und Aktionen des Zustands "Aussortierungsbereich4".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "aussortierungsbereich4.h"
#include "../weichezustand/weichezustand4.h"
#include "../rutschezustand/rutschezustand4.h"
#include "../werkstueck_am_ende/werkstueck_am_ende2.h"

void Aussortierungsbereich4::entry() {
    action->enteredAussortierungsbereich4();
}

void Aussortierungsbereich4::exit() {
    data->zeitStarten();
}

TriggerProcessingState Aussortierungsbereich4::s_lsw_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich4: lsw_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) Weichezustand4;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Aussortierungsbereich4::s_lsr2_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich4: lsr2_ub" << endl;

    exitPointDefaultLeaveState();
    new(this) Rutschezustand4;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Aussortierungsbereich4::s_lse2_ub() {
    cout << "Service Mode BCKalibrierung Aussortierungsbereich4: lse2_ub" << endl;

    exitPointDefaultLeaveState();
    data->setSortTyp(SortTyp::AUSWERFER);
    new(this) WerkstueckAmEnde2;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}