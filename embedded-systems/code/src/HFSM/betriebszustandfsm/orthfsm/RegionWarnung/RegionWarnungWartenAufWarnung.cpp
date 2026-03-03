/**
 * @file RegionWarnungWartenAufWarnung.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenAufWarnung.
 */

#include "RegionWarnungWartenAufWarnung.h"
#include "RegionWarnungWarnsignalAktivieren.h"
#include <iostream>
using namespace std;

void RegionWarnungWartenAufWarnung::entry() {
    cout << "[Warnung] State: WartenAufWarnung::entry()" << endl;
    action->enteredRegionWarnungWartenAufWarnung();
    if (data->rutsche_voll) {
        cout << "[Warnung] RegionWarnungWartenAufWarnung:: rutsche_voll == true -> WartenAufWarnung" << endl;

        leavingState();
        new(this) RegionWarnungWarnsignalAktivieren;
        enterByDefaultEntryPoint();
    }
}

TriggerProcessingState RegionWarnungWartenAufWarnung::ec_rtsch_voll() {
    cout << "[Warnung] WartenAufWarnung::ec_rtsch_voll() -> WarnsignalAktivieren" << endl;
    leavingState();
    new(this) RegionWarnungWarnsignalAktivieren;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void RegionWarnungWartenAufWarnung::showState() {
    cout << "    Region Warnung: WartenAufWarnung" << endl;
}