/**
 * @file FehlerbehandlungBaseState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Basisklasse für Zustände in der Fehlerbehandlung.
 *
 * Steuert den Workflow bei Fehlern (Anzeige, Quittierung, Behebung).
 */

#ifndef ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGBASESTATE_H
#define ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGBASESTATE_H

#include "../ContextData.h"
#include "../Actions.h"
#include "../subcommon/TriggerProcessingState.h"
#include "../common/Types.h"
#include <iostream>

class FehlerbehandlungBaseState {
protected:
    ContextData *data;
    Actions *action;

public:
    virtual ~FehlerbehandlungBaseState() {
    };

    void setData(ContextData *data);

    void setAction(Actions *action);

    virtual void initFehlerbehandlungStateMachines();

    virtual bool isPseudoStartState() { return false; };
    virtual bool isPseudoEndState() { return false; };

    virtual void enterByDefaultEntryPoint() { entry(); };
    virtual void enterByDeepHistoryEntryPoint() { entry(); };

    virtual void enterViaPseudoStart();

    virtual void enterViaDeepHistory();

    virtual void exitExplicitToEStop1();

    virtual void exitExplicitToEStop2();

    virtual void leavingState() { exit(); };

    virtual TriggerProcessingState handleDefaultExit(const TriggerProcessingState &handled);

    virtual void entry() {
    };

    virtual void exit() {
    };

    virtual TriggerProcessingState ec_rtsch_voll() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState ec_ws_unb() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState ec_ws_vrln() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState ec_abstand_ne() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState t_toleranzzeit() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_lsr_nub() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_lsa_nub() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_lsa2_ub() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_lsh_nub() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_lsw_nub() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_lse1_nub() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_lse2_nub() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_st_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_st1_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_st2_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_st1_lg() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_st2_lg() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_rt1_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_rt2_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_e1_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_e2_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_e1_ng() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_e2_ng() { return TriggerProcessingState::pending; }

    virtual TriggerProcessingState s_partner_ok();


    virtual void showState() {
    }
};

#endif //ESEP_2025WISE_TEAM_1_3_FEHLERBEHANDLUNGBASESTATE_H