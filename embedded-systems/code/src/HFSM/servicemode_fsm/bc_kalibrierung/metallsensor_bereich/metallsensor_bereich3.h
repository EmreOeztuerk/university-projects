/**
 * Zustand "MetallsensorBereich3" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef METALLSENSOR_BEREICH3_H
#define METALLSENSOR_BEREICH3_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class MetallsensorBereich3 : public BCKalibrierung_BaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_metal_aus() override;
};

#endif