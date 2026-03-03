/**
 * Zustand "WerkstueckLaeuft6" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef WERKSTUECK_LAEUFT6_H
#define WERKSTUECK_LAEUFT6_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class WerkstueckLaeuft6 : public BCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsh_ub() override;

    TriggerProcessingState s_metal_an() override;
};

#endif