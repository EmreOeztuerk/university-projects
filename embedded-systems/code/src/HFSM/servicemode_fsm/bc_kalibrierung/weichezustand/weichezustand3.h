/**
 * Zustand "Weichezustand3" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef WEICHEZUSTAND3_H
#define WEICHEZUSTAND3_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class Weichezustand3 : public BCKalibrierung_BaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_lse1_ub() override;
};

#endif