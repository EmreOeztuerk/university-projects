/**
 * @file BetriebszustandBaseState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Basisklasse für die Zustände im Betriebszustand.
 *
 * Schnittstelle für die Zustände, die während des laufenden Betriebs (Werkstückbearbeitung) aktiv sind.
 */

#ifndef BETRIEBSZUSTAND_BASESTATE_H
#define BETRIEBSZUSTAND_BASESTATE_H

#include "../ContextData.h"
#include "../Actions.h"
#include "../subcommon/TriggerProcessingState.h"
#include "../common/Types.h"
#include "orthfsm/RegionLSA/RegionLSABaseState.h"
#include "orthfsm/RegionLSH/RegionLSHBaseState.h"
#include "orthfsm/RegionLSHbisLSW/RegionLSHBisLSWBaseState.h"
#include "orthfsm/RegionLSWbisLSE/RegionLSWBisLSEBaseState.h"
#include "orthfsm/RegionWarnung/RegionWarnungBasestate.h"


#include <iostream>
using namespace std;

class BetriebszustandBaseState {
protected:
    ContextData *data;
    Actions *action;
    RegionLSABaseState *regionLSA;
    RegionLSHBaseState *regionLSH;
    RegionLSHBisLSWBaseState *regionLSHbisLSW;
    RegionLSWBisLSEBaseState *regionLSWbisLSE;
    RegionWarnungBaseState *regionWarnung;

public:
    virtual ~BetriebszustandBaseState() {
        delete regionLSA;
        delete regionLSH;
        delete regionLSHbisLSW;
        delete regionLSWbisLSE;
        delete regionWarnung;
    };

    // --------------------------------------------------------
    // Konfiguration
    // --------------------------------------------------------
    virtual void initBetriebszustandStateMachines();

    void setData(ContextData *data);

    void setAction(Actions *action);

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
    // --------------------------------------------------------
    // Trigger der FSM "Betriebszustand"
    // --------------------------------------------------------
    // Sensoren
    virtual TriggerProcessingState s_lsa1_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsa2_ub() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_lsh_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsh_nub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsw_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsr_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lse_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lse_nub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lse1_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lse2_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lse2_nub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_metal_an() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_e1_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_e2_g() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_e1_ng() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState s_e2_ng() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState cmd_rtsch_nv() { return TriggerProcessingState::pending; };
    // Übergabe / Festo
    virtual TriggerProcessingState s_uebergabe_ws() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_uebergabe_ws_fertig() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_festo2_bereit() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_festo2_nbereit() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_festo1_frei() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_festo2_frei() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_festo1_stoppen() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_ws_an_festo2() { return TriggerProcessingState::pending; };


    // Fehler
    virtual TriggerProcessingState ec_vbdng_ub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState ec_ws_vrln() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState ec_ws_unb() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState ec_abstand_ne() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState ec_rtsch_voll() { return TriggerProcessingState::pending; };
    // Timer
    virtual TriggerProcessingState t_lsa_lsh() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_lsh_lsm() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_lsw_lse() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_lsw_lsr() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_lse1_lsa2() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_lsa_q1() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_toleranzzeit_lsa() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_toleranzzeit_lsh() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_toleranzzeit_lsw() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_toleranzzeit_lse() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_lsa_nub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsw_nub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lsr_nub() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_lse1_nub() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState t_lsh_lsw() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_q1_lsh() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState t_ping_pong() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState cmd_festos_frei() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState cmd_rtsch_voll() { return TriggerProcessingState::pending; };

    virtual TriggerProcessingState s_partner_ok_1();

    virtual TriggerProcessingState s_partner_ok_2();

    // Tick für zyklische Aktionen der inneren FSM
    virtual TriggerProcessingState tick() { return TriggerProcessingState::pending; };
    // Just for illustration / Debug
    virtual void showState() {
    };
};

#endif // BETRIEBSZUSTAND_BASESTATE_H