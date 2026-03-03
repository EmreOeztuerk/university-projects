/**
 * Zustand "AnfangKalibrierungSchnell" der FSM "Servicemode".
 *
 * @author: Team 1.3
 * @version: 0.0
 * @date: 10. December 2025
 */
#ifndef ANFANG_KALIBRIERUNG_SCHNELL_H
#define ANFANG_KALIBRIERUNG_SCHNELL_H

#include "../../../subcommon/TriggerProcessingState.h"
#include "../../../contextdata.h"
#include "../../../actions.h"
#include "../bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class AnfangKalibrierungSchnell : public BCKalibrierung_BaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsa1_ub() override;
};

#endif