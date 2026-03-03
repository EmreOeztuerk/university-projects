/**
 * Trigger und Aktionen des Zustands "EndeKalibrierungLangAE".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "ende_kalibrierung_lang_ae.h"
#include "../ende_kalibrierung/ende_kalibrierung_langsam.h"

void EndeKalibrierungLangAE::entry() {
    action->enteredEndeKalibrierungLangAE();
}

void EndeKalibrierungLangAE::exit() {
    //data->zeitSpeichern(data->t_lsw_lse, "");
}

TriggerProcessingState EndeKalibrierungLangAE::s_lse1_nub() {
    cout << "Service Mode BCKalibrierung EndeKalibrierungLangAE: lse1_nub" << endl;

    exitPointDefaultLeaveState();
    new(this) EndeKalibrierungLangsam;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}