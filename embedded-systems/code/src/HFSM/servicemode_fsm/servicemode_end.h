/**
 * Ende-Pseudozustand in Servicemode
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "servicemode_basestate.h"

#ifndef SERVICEMODE_END_H
#define SERVICEMODE_END_H

class ServiceMode_PseudoEndState : public Servicemode_BaseState {
public:
    bool isPseudoEndState() { return true; };
};

#endif