/**
 * Zustand "Initialisierung" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef INITIALISIERUNG_H
#define INITIALISIERUNG_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class Initialisierung : public BCKalibrierung_BaseState {
public:
    void entry() override;

    void exit() override;
};

#endif