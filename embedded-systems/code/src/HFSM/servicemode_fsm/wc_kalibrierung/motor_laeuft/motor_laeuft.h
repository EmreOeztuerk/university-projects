/**
 * Zustand "MotorLaeuft" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef MOTOR_LAEUFT_H
#define MOTOR_LAEUFT_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../wc_kalibrierung_basestate/wc_kalibrierung_basestate.h"
#include "../werkstueck_laeuft/werkstueck_laeuft.h"

class MotorLaeuftWC : public WCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsa1_nub() override;
};

#endif