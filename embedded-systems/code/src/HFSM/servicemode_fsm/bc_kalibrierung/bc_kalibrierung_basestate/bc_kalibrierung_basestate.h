/**
 * Zustand "BCKalibrierung" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef BC_KALIBRIERUNG_BASESTATE_H
#define BC_KALIBRIERUNG_BASESTATE_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
//#include "../../servicemode_basestate.h"

#include <iostream>
using namespace std;

class BCKalibrierung_BaseState {
protected:
    ContextData *data;
    Actions *action;

public:
    virtual ~BCKalibrierung_BaseState() {
    };

    virtual void initBCKalibrierungStateMachines() {
    };

    void setData(ContextData *data);

    void setAction(Actions *action);

    virtual bool isPseudoStartState() { return false; };
    virtual bool isPseudoEndState() { return false; };

    virtual void entryPointDefaultEnter() { entry(); };
    virtual void exitPointDefaultLeaveState() { exit(); };

    // entry/exit-actions
    virtual void entry() {
    };

    virtual void exit() {
    };

    // Trigger
    /* Lichtschranke Anfang */
    virtual TriggerProcessingState s_lsa1_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsa1_nub() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_lsa2_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsa2_nub() { return TriggerProcessingState::pending; };

    /* Lichtschranke Hoehenmesser */
    virtual TriggerProcessingState s_lsh_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsh_nub() { return TriggerProcessingState::pending; };

    /* Metallsensor */
    virtual TriggerProcessingState s_metal_an() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_metal_aus() { return TriggerProcessingState::pending; };

    /* Lichtschranke Rutsche */
    virtual TriggerProcessingState s_lsr1_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsr1_nub() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_lsr2_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsr2_nub() { return TriggerProcessingState::pending; };

    /* Lichtschranke Weiche */
    virtual TriggerProcessingState s_lsw_ub() { return TriggerProcessingState::pending; };

    /* Lichtschranke Ende */
    virtual TriggerProcessingState s_lse1_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lse1_nub() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_lse2_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lse2_nub() { return TriggerProcessingState::pending; };
};

#endif