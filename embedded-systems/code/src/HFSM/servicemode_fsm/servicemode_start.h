/**
 * Start-Pseudozustand in Servicemode
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "servicemode_basestate.h"

#ifndef SERVICEMODE_START_H
#define SERVICEMODE_START_H

class ServiceMode_PseudoStartState : public Servicemode_BaseState {
public:
    bool isPseudoStartState() { return true; };
};

#endif