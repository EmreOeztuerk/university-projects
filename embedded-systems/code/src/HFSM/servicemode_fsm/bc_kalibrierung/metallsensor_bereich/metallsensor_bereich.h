/**
 * Zustand "MetallsensorBereich" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef METALLSENSOR_BEREICH_H
#define METALLSENSOR_BEREICH_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class MetallsensorBereich : public BCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_metal_aus() override;
};

#endif