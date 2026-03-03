/**
 * Trigger und Aktionen des Zustands "Rutschezustand3".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "rutschezustand3.h"
#include "../anfang_kalibrierung/anfang_kalibrierung_schnell.h"

void Rutschezustand3::entry() {
    action->enteredRutschezustand3();
    data->setSortTyp(SortTyp::WEICHE);
}

TriggerProcessingState Rutschezustand3::s_lsr1_nub() {
    cout << "Service Mode BCKalibrierung Rutschezustand3: lsr1_nub" << endl;

    if (data->sortTyp == SortTyp::WEICHE) {
        exitPointDefaultLeaveState();
        new(this) AnfangKalibrierungSchnell;
        entryPointDefaultEnter();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}