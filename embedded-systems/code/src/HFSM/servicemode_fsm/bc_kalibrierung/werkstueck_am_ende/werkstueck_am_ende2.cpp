/**
 * Trigger und Aktionen des Zustands "WerkstueckAmEnde2".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_am_ende2.h"

void WerkstueckAmEnde2::entry() {
    action->enteredWerkstueckAmEnde2();
    data->set(data->kalibrierungFertig, true);
    action->cmd_mtr_stop();
    //data->zeitSpeichern(data->t_lsw_lse, "");
}

TriggerProcessingState WerkstueckAmEnde2::s_lse2_nub() {
    if (data->kalibrierungFertig) {
        entryPointDefaultEnter();
        return TriggerProcessingState::endstatereached;
    }
    return TriggerProcessingState::pending;
}