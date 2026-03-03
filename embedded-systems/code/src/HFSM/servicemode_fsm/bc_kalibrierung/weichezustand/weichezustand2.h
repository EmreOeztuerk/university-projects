/**
 * Zustand "Weichezustand2" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef WEICHEZUSTAND2_H
#define WEICHEZUSTAND2_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class Weichezustand2 : public BCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lse2_ub() override;
};

#endif