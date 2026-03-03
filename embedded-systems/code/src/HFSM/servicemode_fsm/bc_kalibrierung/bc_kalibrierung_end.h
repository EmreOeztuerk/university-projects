/**
 * Ende-Pseudozustand in BCKalibrierung
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

#ifndef BC_KALIBRIERUNG_END_H
#define BC_KALIBRIERUNG_END_H

class BCKalibrierung_PseudoEndState : public BCKalibrierung_BaseState {
public:
    bool isPseudoEndState() { return true; };
};

#endif