/**
 * Zustand "WCKalibrierung" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef WC_KALIBRIERUNG_BASESTATE_H
#define WC_KALIBRIERUNG_BASESTATE_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
//#include "../../servicemode_basestate.h"

#include <iostream>
using namespace std;

class WCKalibrierung_BaseState {
protected:
    ContextData *data;
    Actions *action;

public:
    virtual ~WCKalibrierung_BaseState() {
    };

    virtual void initWCKalibrierungStateMachines() {
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
    /* Lichtschranke Hoehenmesser */
    virtual TriggerProcessingState s_lsh_ub() { return TriggerProcessingState::pending; };

    /* Lichtschranke Ausfahrt */
    virtual TriggerProcessingState s_lsa1_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsa1_nub() { return TriggerProcessingState::pending; };

    /* Metallsensor */
    virtual TriggerProcessingState s_metal_an() { return TriggerProcessingState::pending; };
};

#endif