/**
 * @file RegionWarnungWarnsignalAktivieren.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WarnsignalAktivieren.
 */

#include "RegionWarnungWarnsignalAktivieren.h"
#include "RegionWarnungWartenAufWarnung.h"
#include <iostream>
using namespace std;

void RegionWarnungWarnsignalAktivieren::entry() {
    cout << "[Warnung] State: WarnsignalAktivieren::entry()" << endl;
    action->enteredRegionWarnungWarnsignalAktivieren();
    action->cmd_led_amp_gelb_b();

    if (!(data->rutsche_voll)) {
        cout << "[Warnung] WarnsignalAktivieren:: rutsche_voll == false -> WartenAufWarnung" << endl;
        leavingState();
        new(this) RegionWarnungWartenAufWarnung;
        enterByDefaultEntryPoint();
    }
}

void RegionWarnungWarnsignalAktivieren::exit() {
    cout << "[Warnung] State: WarnsignalAktivieren::exit()" << endl;

    action->cmd_led_amp_glb_aus();
}

TriggerProcessingState RegionWarnungWarnsignalAktivieren::cmd_rtsch_nv() {
    // Interpretiere dieses Event als "Rutsche nicht mehr voll" (RTSCH_NV)
    cout << "[Warnung] WarnsignalAktivieren::ec_rtsch_voll() -> WartenAufWarnung" << endl;
    leavingState();
    new(this) RegionWarnungWartenAufWarnung;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void RegionWarnungWarnsignalAktivieren::showState() {
    cout << "    Region Warnung: WarnsignalAktivieren" << endl;
}