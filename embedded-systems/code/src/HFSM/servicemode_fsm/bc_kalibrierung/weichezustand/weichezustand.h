/**
 * Zustand "Weichezustand" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef WEICHEZUSTAND_H
#define WEICHEZUSTAND_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class Weichezustand : public BCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lse1_ub() override;
};

#endif