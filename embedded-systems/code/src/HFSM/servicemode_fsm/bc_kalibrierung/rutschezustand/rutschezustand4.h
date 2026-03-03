/**
 * Zustand "Rutschezustand4" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef RUTSCHEZUSTAND4_H
#define RUTSCHEZUSTAND4_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class Rutschezustand4 : public BCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsr2_nub() override;
};

#endif