/**
 * Ende-Pseudozustand in WCKalibrierung
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "wc_kalibrierung_basestate/wc_kalibrierung_basestate.h"

#ifndef WC_KALIBRIERUNG_END_H
#define WC_KALIBRIERUNG_END_H

class WCKalibrierung_PseudoEndState : public WCKalibrierung_BaseState {
public:
    bool isPseudoEndState() { return true; };
};

#endif