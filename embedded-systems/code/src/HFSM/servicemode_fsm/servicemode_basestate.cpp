/**
 * Trigger und Aktionen des Basiszustands "Service Mode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "servicemode_basestate.h"
#include "../rootfsm/Betriebszustand.h"
#include "../rootfsm/Ruhezustand.h"
#include "../rootfsm/EStop.h"

void Servicemode_BaseState::setData(ContextData *data) {
    this->data = data;
}

void Servicemode_BaseState::setAction(Actions *action) {
    this->action = action;
}

void Servicemode_BaseState::initServicemodeStateMachines() {
    bckalib_basestate = new BCKalibrierung_BaseState();
    wckalib_basestate = new WCKalibrierung_BaseState();

    bckalib_basestate->initBCKalibrierungStateMachines();
    wckalib_basestate->initWCKalibrierungStateMachines();
}

void Servicemode_BaseState::exitExplicitToEStop1() {
    cout << "Service Mode Base State: Ausgang zum E-Stop 1." << endl;
    exit();
    new(this) EStop;
    //	enterByE1EntryPoint();
    entryPointDefaultEnter();
}

void Servicemode_BaseState::exitExplicitToEStop2() {
    cout << "Service Mode Base State: Ausgang zum E-Stop 2." << endl;
    exit();
    new(this) EStop;
    //	enterByE2EntryPoint();
    entryPointDefaultEnter();
}

void Servicemode_BaseState::exitExplicitToBetriebszustand() {
    cout << "Service Mode Base State: Ausgang zum Betriebszustand." << endl;
    exit();
    new(this) Betriebszustand;
    entryPointDefaultEnter();
}

void Servicemode_BaseState::exitExplicitToRuhezustand() {
    cout << "Service Mode Base State: Ausgang zum Ruhezustand." << endl;
    exit();
    new(this) Ruhezustand;
    entryPointDefaultEnter();
}

void Servicemode_BaseState::entry() {
    action->cmd_led_amp_glb_an();
}

void Servicemode_BaseState::exit() {
    action->cmd_led_amp_glb_aus();
}