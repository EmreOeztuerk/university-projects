/**
 * Zustand "Wartezustand" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef WARTEZUSTAND_HPP
#define WARTEZUSTAND_HPP

#include "../subcommon/TriggerProcessingState.h"
#include "../contextdata.h"
#include "../actions.h"
#include "servicemode_basestate.h"
#include "bc_kalibrierung/bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"
#include "../rootfsm/Ruhezustand.h"

class Servicemode_Wartezustand : public Servicemode_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_st1_g() override;

    TriggerProcessingState s_stp_g() override;
};

#endif