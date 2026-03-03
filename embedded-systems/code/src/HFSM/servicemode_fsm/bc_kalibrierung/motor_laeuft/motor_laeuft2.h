/**
 * Zustand "MotorLaeuft2" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef MOTOR_LAEUFT2_H
#define MOTOR_LAEUFT2_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class MotorLaeuft2 : public BCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsa2_ub() override;
};

#endif