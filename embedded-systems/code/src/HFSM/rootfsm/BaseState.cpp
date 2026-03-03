/**
* @file BaseState.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der BaseState-Klasse.
 */

#include "BaseState.h"
#include "../estopfsm/EStopPseudoStartState.h"
#include "../estopfsm/EstopBaseState.h"
#include "Betriebszustand.h"
#include "../betriebszustandfsm/BetriebszustandBaseState.h"
#include "../betriebszustandfsm/BetriebszustandPseudoStartState.h"
#include "../fehlerbehandlungfsm/FEHLERBEHANDLUNGPSEUDOSTARTSTATE.H"
#include "Ruhezustand.h"
#include "../RZfsm/RZBaseState.hpp"
#include "../RZfsm/RZPseudoStartState.hpp"
#include "ServiceMode.h"
#include "../servicemode_fsm/servicemode_basestate.h"
#include "../servicemode_fsm/servicemode_start.h"


void BaseState::initSubStateMachines() {
    // Init EStop-Sub-FSM
    estopstatemachine = new EStopPseudoStartState();
    estopstatemachine->initEStopStateMachines();

    // Init Ruhezustand-Sub-FSM
    rzstatemachine = new RZPseudoStartState();
    rzstatemachine->initRZStateMachines();

    //servicemode_statemachine = new ServiceMode_PseudoStartState();
    //servicemode_statemachine->initServicemodeStateMachines();

    bzstatemachine = new BetriebszustandPseudoStartState();
    bzstatemachine->initBetriebszustandStateMachines();
    //
    fbstatemachine = new FehlerbehandlungPseudoStartState();
    fbstatemachine->initFehlerbehandlungStateMachines();
}

void BaseState::setData(ContextData *data) {
    // Set shared context data
    this->data = data;
    estopstatemachine->setData(data);
    rzstatemachine->setData(data);
    //servicemode_statemachine->setData(data);
    fbstatemachine->setData(data);
    bzstatemachine->setData(data);
}

void BaseState::setAction(Actions *action) {
    // Set action interface
    this->action = action;
    estopstatemachine->setAction(action);
    rzstatemachine->setAction(action);
    //servicemode_statemachine->setAction(action);
    fbstatemachine->setAction(action);
    bzstatemachine->setAction(action);
}

void BaseState::enterViaPseudoStart() {
    // go from pseudo-start state to first state
    cout << "Ruhezustand transition taken" << endl;
    new(this) Ruhezustand;
    enterByDefaultEntryPoint(); // Entry of initial state
}