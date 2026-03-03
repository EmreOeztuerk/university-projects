/**
 * Trigger und Aktionen des Zustands "MotorLaeuft".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "initialisierung.h"
#include "../anfang_kalibrierung/anfang_kalibrierung.h"

void Initialisierung::entry() {
    action->enteredInitialisierung();
    data->set(data->kalibrierungFertig, false);
    data->set(data->kalibrierungLangFertig, false);
}

void Initialisierung::exit() {
    exitPointDefaultLeaveState();
    new(this) AnfangKalibrierung;
    entryPointDefaultEnter();
}