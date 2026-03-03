/**
 * Start-Pseudozustand in WCKalibrierung
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#include "wc_kalibrierung_basestate/wc_kalibrierung_basestate.h"

#ifndef WC_KALIBRIERUNG_START_H
#define WC_KALIBRIERUNG_START_H

class WCKalibrierung_PseudoStartState : public WCKalibrierung_BaseState {
public:
    bool isPseudoStartState() { return true; };
};

#endif