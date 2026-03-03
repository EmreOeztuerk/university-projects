/**
 * Zustand "Aussortierungsbereich4" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef AUSSORTIERUNGSBEREICH4_H
#define AUSSORTIERUNGSBEREICH4_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class Aussortierungsbereich4 : public BCKalibrierung_BaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_lsw_ub() override;

    TriggerProcessingState s_lsr2_ub() override;

    TriggerProcessingState s_lse2_ub() override;
};

#endif