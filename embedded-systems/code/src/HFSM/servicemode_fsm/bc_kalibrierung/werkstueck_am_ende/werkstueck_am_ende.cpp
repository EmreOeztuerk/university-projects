/**
 * Trigger und Aktionen des Zustands "WerkstueckAmEnde".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "werkstueck_am_ende.h"
#include "../ende_kalibrierung/ende_kalibrierung_langsam.h"

void WerkstueckAmEnde::entry() {
    action->enteredWerkstueckAmEnde();
    action->cmd_mtr_stop();
    //data->zeitSpeichern(data->t_lsw_lse, "");
}

TriggerProcessingState WerkstueckAmEnde::s_lse2_nub() {
    new(this) EndeKalibrierungLangsam;
    entryPointDefaultEnter();
    return TriggerProcessingState::consumed;
}