/**
 * Zustand "EndeKalibrierungAE" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef ENDE_KALIBRIERUNG_AE_H
#define ENDE_KALIBRIERUNG_AE_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class EndeKalibrierungAE : public BCKalibrierung_BaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_lse2_nub() override;
};

#endif