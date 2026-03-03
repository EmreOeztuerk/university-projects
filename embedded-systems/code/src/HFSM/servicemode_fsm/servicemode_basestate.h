/**
 * Gesamt-FSM Service Mode
 *
 * @author: Team 1.3
 * @version: 2
 * @date: 20. December 2025
 */
#ifndef SERVICEMODE_BASESTATE_H
#define SERVICEMODE_BASESTATE_H

#include "../subcommon/TriggerProcessingState.h"
#include "../contextdata.h"
#include "../actions.h"
#include "../../HAL/sorter/sorttype.hpp"
#include "bc_kalibrierung/bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"
#include "wc_kalibrierung/wc_kalibrierung_basestate/wc_kalibrierung_basestate.h"

#include <iostream>
using namespace std;

class Servicemode_BaseState {
protected:
    ContextData *data;
    Actions *action;
    BCKalibrierung_BaseState *bckalib_basestate;
    WCKalibrierung_BaseState *wckalib_basestate;

public:
    virtual ~Servicemode_BaseState() {
        delete bckalib_basestate;
        delete wckalib_basestate;
    };

    virtual void initServicemodeStateMachines();

    void setData(ContextData *data);

    void setAction(Actions *action);

    virtual bool isPseudoStartState() { return false; };
    virtual bool isPseudoEndState() { return false; };

    virtual void entryPointDefaultEnter() { entry(); };

    virtual void exitExplicitToEStop1();

    virtual void exitExplicitToEStop2();

    virtual void exitExplicitToBetriebszustand();

    virtual void exitExplicitToRuhezustand();

    virtual void exitPointDefaultLeaveState() { exit(); };

    // entry/exit-actions
    virtual void entry();

    virtual void exit();

    // Trigger Service Mode
    virtual TriggerProcessingState s_st1_g() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_stp_g() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_nvbdn() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_e1_g() { return TriggerProcessingState::explicitExitViaE1; };
    virtual TriggerProcessingState s_e2_g() { return TriggerProcessingState::explicitExitViaE2; };

    // Trigger untere Zustaende
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