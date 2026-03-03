/**
 * Zustand "HSzustand" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef HS_ZUSTAND_H
#define HS_ZUSTAND_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class HSzustand : public BCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsh_nub() override;
};

#endif