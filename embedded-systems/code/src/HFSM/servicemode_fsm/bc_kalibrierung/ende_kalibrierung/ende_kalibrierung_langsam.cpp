/**
 * Trigger und Aktionen des Zustands "EndeKalibrierungLangsam".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "ende_kalibrierung_langsam.h"
#include "../anfang_kalibrierung/anfang_kalibrierung_schnell.h"

void EndeKalibrierungLangsam::entry() {
    action->enteredEndeKalibrierungLangsam();
    data->set(data->kalibrierungLangFertig, true);
}

void EndeKalibrierungLangsam::exit() {
    exitPointDefaultLeaveState();
    new(this) AnfangKalibrierungSchnell;
    entryPointDefaultEnter();
}