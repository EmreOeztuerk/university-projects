/**
 * Zustand "AnfangWCKalibrierung" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef ANFANG_WC_KALIBRIERUNG_H
#define ANFANG_WC_KALIBRIERUNG_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../wc_kalibrierung_basestate/wc_kalibrierung_basestate.h"
//#include "../../../servicemode_fsm/wc_kalibrierung/motor_laeuft/motor_laeuft.h"

class AnfangWCKalibrierung : public WCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsa1_ub() override;
};

#endif