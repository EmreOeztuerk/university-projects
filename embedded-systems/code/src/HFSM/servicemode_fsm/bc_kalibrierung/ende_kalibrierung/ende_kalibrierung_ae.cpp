/**
 * Trigger und Aktionen des Zustands "EndeKalibrierungAE".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "ende_kalibrierung_ae.h"

void EndeKalibrierungAE::entry() {
    action->enteredEndeKalibrierungAE();
    //data->zeitSpeichern(data->t_lsw_lse, "");
}

void EndeKalibrierungAE::exit() {
    data->zeitStarten();
}

TriggerProcessingState EndeKalibrierungAE::s_lse2_nub() {
    cout << "Service Mode BCKalibrierung EndeKalibrierungAE: lse2_nub" << endl;

    if (data->kalibrierungFertig) {
        exitPointDefaultLeaveState();
        entryPointDefaultEnter();
        return TriggerProcessingState::endstatereached;
    }
    return TriggerProcessingState::pending;
}