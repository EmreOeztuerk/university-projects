/**
 * @file RZBaseState.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Basisklasse für alle Zustände innerhalb des Ruhezustands.
 *
 * Definiert die abstrakte Schnittstelle für die Sub-FSM des Ruhezustands (z.B. Wartezustand, Betriebsbereit).
 */

#ifndef RZBASESTATE_HPP
#define RZBASESTATE_HPP

#include "../subcommon/TriggerProcessingState.h"
#include "../ContextData.h"
#include "../Actions.h"

using namespace std;

class RZBaseState {
protected:
    ContextData *data;
    Actions *action;
    //RZBaseState* rzfsm;
public:
    virtual ~RZBaseState() {
    };

    virtual void initRZStateMachines() {
    };

    void setData(ContextData *data);

    void setAction(Actions *action);

    virtual bool isPseudoStartState() { return false; };
    virtual bool isPseudoEndState() { return false; };

    virtual void enterByDefaultEntryPoint() { entry(); };

    virtual void enterViaPseudoStart();

    virtual void leavingState() { exit(); };


    // entry/exit-actions
    virtual void entry() {
    };

    virtual void exit() {
    };

    // Trigger
    virtual TriggerProcessingState s_st1_g() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_st2_g() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_st1_lg() {
        //    	   std::cout << "[Debug Root RZBaseState] s_st1_lg" << std::endl;
        //    	   return TriggerProcessingState::explicitExitViaS;
        return TriggerProcessingState::pending;
    };
    virtual TriggerProcessingState s_st2_lg() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_rt1_g() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_rt2_g() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_e1_g() { return TriggerProcessingState::pending; };

    virtual void showState() = 0;
};
#endif //RZBASESTATE_HPP