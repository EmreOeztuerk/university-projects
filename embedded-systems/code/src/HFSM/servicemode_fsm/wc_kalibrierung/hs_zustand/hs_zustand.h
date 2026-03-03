/**
 * Zustand "HSzustand" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef HS_ZUSTAND_H
#define HS_ZUSTAND_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../ContextData.h"
#include "../../../actions.h"
#include "../wc_kalibrierung_basestate/wc_kalibrierung_basestate.h"

class HSzustandWC : public WCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_metal_an() override;
};

#endif