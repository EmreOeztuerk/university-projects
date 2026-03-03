/**
 * @file RegionWarnungBasestate.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Basisklasse für die Warnungs-Region.
 *
 * Diese orthogonale Region läuft parallel zum Transportprozess und kümmert sich
 * um die Anzeige von Warnungen (z.B. Ampeln, die auf Fehler hinweisen).
 */

#ifndef REGIONWARNUNGBASESTATE_H
#define REGIONWARNUNGBASESTATE_H

//#ifndef REGION_WARNUNG_BASESTATE_H
//#define REGION_WARNUNG_BASESTATE_H

#include "../../../ContextData.h"
#include "../../../Actions.h"
//#include "../../../Context.h"
#include "../../../subcommon/TriggerProcessingState.h"

#include <iostream>
using namespace std;

class RegionWarnungBaseState {
protected:
    ContextData *data;
    Actions *action;

public:
    virtual ~RegionWarnungBaseState() {
    };

    void setData(ContextData *data) { this->data = data; };
    void setAction(Actions *action) { this->action = action; };

    virtual bool isPseudoStartState() { return false; };
    virtual bool isPseudoEndState() { return false; };

    virtual void enterByDefaultEntryPoint() { entry(); };
    virtual void enterByDeepHistoryEntryPoint() { entry(); };

    virtual void enterViaPseudoStart();

    virtual void enterViaDeepHistory();

    virtual void leavingState() { exit(); };

    virtual void entry() {
    };

    virtual void exit() {
    };

    // Trigger für diese Region
    // (RTSCH_VOLL / RTSCH_NV werden hier über ec_rtsch_voll() abgebildet)
    //virtual TriggerProcessingState ec_rtsch_voll() { return TriggerProcessingState::pending; }
    virtual TriggerProcessingState tick() { return TriggerProcessingState::pending; }
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
    //virtual TriggerProcessingState s_e1_ng() { return TriggerProcessingState::pending; }
    //virtual TriggerProcessingState s_e2_ng() { return TriggerProcessingState::pending; }
    // Übergabe / Festo
    virtual TriggerProcessingState s_uebergabe_ws() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_uebergabe_ws_fertig() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_festo2_bereit() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_festo2_nbereit() { return TriggerProcessingState::pending; };
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
    virtual TriggerProcessingState cmd_rtsch_nv() { return TriggerProcessingState::pending; };

    // Just for illustration!
    virtual void showState() {
    }
};

#endif // REGION_WARNUNG_BASESTATE_H