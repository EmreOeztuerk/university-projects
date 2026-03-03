/**
 * Trigger und Aktionen des Zustands "Rutschezustand".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "rutschezustand.h"
#include "../anfang_kalibrierung/anfang_kalibrierung.h"

void Rutschezustand::entry() {
    action->enteredRutschezustand();
    data->setSortTyp(SortTyp::WEICHE);
}

TriggerProcessingState Rutschezustand::s_lsr1_nub() {
    cout << "Service Mode BCKalibrierung Rutschezustand: lsr1_nub" << endl;

    if (data->sortTyp == SortTyp::WEICHE) {
        exitPointDefaultLeaveState();
        new(this) AnfangKalibrierung;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}