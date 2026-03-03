/**
 * @file Sortierung.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands Sortierung.
 *
 * Leitet Events an alle aktiven orthogonalen Regionen weiter.
 */

#include "Sortierung.h"
#include "AufWSWarten.h"

// Orthogonale Regionen
#include "orthfsm/RegionWarnung/RegionWarnungBaseState.h"
#include "orthfsm/RegionLSA/RegionLSABaseState.h"
#include "orthfsm/RegionLSH/RegionLSHBaseState.h"
#include "orthfsm/RegionLSHBisLSW/RegionLSHBisLSWBaseState.h"
#include "orthfsm/RegionLSWBisLSE/RegionLSWBisLSEBaseState.h"

#include <iostream>
using namespace std;

void Sortierung::entry() {
    action->enteredSortierung();
    if (data->gFesto1() && data->enteredHoheMessen == false && data->enteredWartenAufErlaubnisVonF2 == false) {
        action->cmd_mtr_rechts_s();
    }
    cout << "[Betriebszustand] Sortierung::entry()" << endl;
}

void Sortierung::enterByDefaultEntryPoint() {
    entry();
    // Alle Regionen starten
    regionLSA->enterViaPseudoStart();
    regionLSH->enterViaPseudoStart();
    regionLSHbisLSW->enterViaPseudoStart();
    regionLSWbisLSE->enterViaPseudoStart();
    regionWarnung->enterViaPseudoStart();
}

void Sortierung::enterByDeepHistoryEntryPoint() {
    entry();
    if (data->enteredHoheMessen == true) {
        action->cmd_mtr_rechts_l();
        data->lsa_timer.fortsetzen();
        data->lsh_timer.fortsetzen();
        data->lsw_timer.fortsetzen();
        data->lse_timer.fortsetzen();
        data->lsr_timer.fortsetzen();
    } else if (data->enteredWartenAufErlaubnisVonF2 == false && data->enteredWartenBisWSEntfernt == false) {
        action->cmd_mtr_rechts_s();
        data->lsa_timer.fortsetzen();
        data->lsh_timer.fortsetzen();
        data->lsw_timer.fortsetzen();
        data->lse_timer.fortsetzen();
        data->lsr_timer.fortsetzen();
    }
    // Deep-History der Regionen wiederherstellen
    regionLSA->enterViaDeepHistory();
    regionLSH->enterViaDeepHistory();
    regionLSHbisLSW->enterViaDeepHistory();
    regionLSWbisLSE->enterViaDeepHistory();
    regionWarnung->enterViaDeepHistory();
}


void Sortierung::leavingState() {
    regionLSA->leavingState();
    regionLSH->leavingState();
    regionLSHbisLSW->leavingState();
    regionLSWbisLSE->leavingState();
    regionWarnung->leavingState();
    exit();
}

TriggerProcessingState Sortierung::handleDefaultExit() {
    TriggerProcessingState processingstate = TriggerProcessingState::pending;


    if (regionLSA->isPseudoEndState() && regionLSH->isPseudoEndState() && regionLSHbisLSW->isPseudoEndState() &&
        regionLSWbisLSE->isPseudoEndState() && regionWarnung->isPseudoEndState()) {
        leavingState();
        new(this) AufWSWarten;
        enterByDefaultEntryPoint();
        processingstate = TriggerProcessingState::consumed;
    }
    return processingstate;
}

// --------------------------------------------------------
// EXIT aus Sortierung
// --------------------------------------------------------
TriggerProcessingState Sortierung::cmd_festos_frei() {
    cout << "[Betriebszustand] Sortierung::cmd_festos_frei() -> AufWS_Warten" << endl;
    leavingState();
    new(this) AufWSWarten;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Sortierung::s_festo1_frei() {
    return TriggerProcessingState::pending;
}

TriggerProcessingState Sortierung::s_festo1_stoppen() {
    if (data->gFesto1()) {
        cout << "[Betriebszustand] Sortierung::s_festo1_stoppen() -> AufWS_Warten" << endl;
        leavingState();
        new(this) AufWSWarten;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}

TriggerProcessingState Sortierung::s_festo2_frei() {
    if (data->gFesto2()) {
        cout << "[Betriebszustand] Sortierung::s_festo2_frei() -> AufWS_Warten" << endl;
        leavingState();
        new(this) AufWSWarten;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}

TriggerProcessingState Sortierung::s_lsa1_ub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lsa1_ub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lsa1_ub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lsa1_ub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lsa1_ub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lsa1_ub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lsa2_ub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lsa2_ub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lsa2_ub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lsa2_ub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lsa2_ub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lsa2_ub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lsh_ub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lsh_ub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lsh_ub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lsh_ub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lsh_ub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lsh_ub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lsh_nub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lsh_nub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lsh_nub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lsh_nub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lsh_nub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lsh_nub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lsw_ub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lsw_ub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lsw_ub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lsw_ub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lsw_ub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lsw_ub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lsr_ub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lsr_ub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lsr_ub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lsr_ub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lsr_ub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lsr_ub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lse_ub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lse_ub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lse_ub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lse_ub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lse_ub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lse_ub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lse_nub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lse_nub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lse_nub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lse_nub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lse_nub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lse_nub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lse1_ub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lse1_ub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lse1_ub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lse1_ub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lse1_ub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lse1_ub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lse2_ub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lse2_ub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lse2_ub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lse2_ub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lse2_ub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lse2_ub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_lse2_nub() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_lse2_nub();
    TriggerProcessingState processingRegionLSH = regionLSH->s_lse2_nub();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_lse2_nub();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_lse2_nub();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_lse2_nub();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_metal_an() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_metal_an();
    TriggerProcessingState processingRegionLSH = regionLSH->s_metal_an();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_metal_an();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_metal_an();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_metal_an();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_e1_g() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_e1_g();
    TriggerProcessingState processingRegionLSH = regionLSH->s_e1_g();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_e1_g();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_e1_g();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_e1_g();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_e2_g() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_e2_g();
    TriggerProcessingState processingRegionLSH = regionLSH->s_e2_g();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_e2_g();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_e2_g();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_e2_g();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_uebergabe_ws() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_uebergabe_ws();
    TriggerProcessingState processingRegionLSH = regionLSH->s_uebergabe_ws();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_uebergabe_ws();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_uebergabe_ws();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_uebergabe_ws();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_uebergabe_ws_fertig() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_uebergabe_ws_fertig();
    TriggerProcessingState processingRegionLSH = regionLSH->s_uebergabe_ws_fertig();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_uebergabe_ws_fertig();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_uebergabe_ws_fertig();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_uebergabe_ws_fertig();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_festo2_bereit() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_festo2_bereit();
    TriggerProcessingState processingRegionLSH = regionLSH->s_festo2_bereit();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_festo2_bereit();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_festo2_bereit();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_festo2_bereit();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::s_festo2_nbereit() {
    TriggerProcessingState processingRegionLSA = regionLSA->s_festo2_nbereit();
    TriggerProcessingState processingRegionLSH = regionLSH->s_festo2_nbereit();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->s_festo2_nbereit();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->s_festo2_nbereit();
    TriggerProcessingState processingRegionWarnung = regionWarnung->s_festo2_nbereit();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::ec_rtsch_voll() {
    TriggerProcessingState processingRegionLSA = regionLSA->ec_rtsch_voll();
    TriggerProcessingState processingRegionLSH = regionLSH->ec_rtsch_voll();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->ec_rtsch_voll();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->ec_rtsch_voll();
    TriggerProcessingState processingRegionWarnung = regionWarnung->ec_rtsch_voll();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_lsa_lsh() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_lsa_lsh();
    TriggerProcessingState processingRegionLSH = regionLSH->t_lsa_lsh();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_lsa_lsh();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_lsa_lsh();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_lsa_lsh();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_lsh_lsm() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_lsh_lsm();
    TriggerProcessingState processingRegionLSH = regionLSH->t_lsh_lsm();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_lsh_lsm();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_lsh_lsm();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_lsh_lsm();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_lsw_lse() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_lsw_lse();
    TriggerProcessingState processingRegionLSH = regionLSH->t_lsw_lse();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_lsw_lse();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_lsw_lse();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_lsw_lse();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_lsw_lsr() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_lsw_lsr();
    TriggerProcessingState processingRegionLSH = regionLSH->t_lsw_lsr();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_lsw_lsr();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_lsw_lsr();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_lsw_lsr();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}


TriggerProcessingState Sortierung::t_lse1_lsa2() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_lse1_lsa2();
    TriggerProcessingState processingRegionLSH = regionLSH->t_lse1_lsa2();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_lse1_lsa2();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_lse1_lsa2();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_lse1_lsa2();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_lsa_q1() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_lsa_q1();
    TriggerProcessingState processingRegionLSH = regionLSH->t_lsa_q1();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_lsa_q1();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_lsa_q1();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_lsa_q1();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_toleranzzeit_lsa() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_toleranzzeit_lsa();
    TriggerProcessingState processingRegionLSH = regionLSH->t_toleranzzeit_lsa();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_toleranzzeit_lsa();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_toleranzzeit_lsa();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_toleranzzeit_lsa();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_toleranzzeit_lsh() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_toleranzzeit_lsh();
    TriggerProcessingState processingRegionLSH = regionLSH->t_toleranzzeit_lsh();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_toleranzzeit_lsh();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_toleranzzeit_lsh();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_toleranzzeit_lsh();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_toleranzzeit_lsw() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_toleranzzeit_lsw();
    TriggerProcessingState processingRegionLSH = regionLSH->t_toleranzzeit_lsw();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_toleranzzeit_lsw();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_toleranzzeit_lsw();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_toleranzzeit_lsw();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}

TriggerProcessingState Sortierung::t_toleranzzeit_lse() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_toleranzzeit_lse();
    TriggerProcessingState processingRegionLSH = regionLSH->t_toleranzzeit_lse();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_toleranzzeit_lse();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_toleranzzeit_lse();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_toleranzzeit_lse();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}


TriggerProcessingState Sortierung::t_lsh_lsw() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_lsh_lsw();
    TriggerProcessingState processingRegionLSH = regionLSH->t_lsh_lsw();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_lsh_lsw();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_lsh_lsw();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_lsh_lsw();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}


TriggerProcessingState Sortierung::t_q1_lsh() {
    TriggerProcessingState processingRegionLSA = regionLSA->t_q1_lsh();
    TriggerProcessingState processingRegionLSH = regionLSH->t_q1_lsh();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->t_q1_lsh();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->t_q1_lsh();
    TriggerProcessingState processingRegionWarnung = regionWarnung->t_q1_lsh();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}


TriggerProcessingState Sortierung::cmd_rtsch_voll() {
    TriggerProcessingState processingRegionLSA = regionLSA->cmd_rtsch_voll();
    TriggerProcessingState processingRegionLSH = regionLSH->cmd_rtsch_voll();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->cmd_rtsch_voll();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->cmd_rtsch_voll();
    TriggerProcessingState processingRegionWarnung = regionWarnung->cmd_rtsch_voll();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}


TriggerProcessingState Sortierung::cmd_rtsch_nv() {
    TriggerProcessingState processingRegionLSA = regionLSA->cmd_rtsch_nv();
    TriggerProcessingState processingRegionLSH = regionLSH->cmd_rtsch_nv();
    TriggerProcessingState processingRegionLSHbisLSW = regionLSHbisLSW->cmd_rtsch_nv();
    TriggerProcessingState processingRegionLSWbisLSE = regionLSWbisLSE->cmd_rtsch_nv();
    TriggerProcessingState processingRegionWarnung = regionWarnung->cmd_rtsch_nv();
    TriggerProcessingState processingStateDefault = handleDefaultExit();


    if (processingStateDefault == TriggerProcessingState::expliciteexitfehlerbehandlung) {
        return TriggerProcessingState::expliciteexitfehlerbehandlung;
    } else if (processingStateDefault == TriggerProcessingState::consumed) {
        return TriggerProcessingState::consumed;
    } else {
        if (processingRegionLSA == TriggerProcessingState::pending && processingRegionLSH ==
            TriggerProcessingState::pending &&
            processingRegionLSHbisLSW == TriggerProcessingState::pending && processingRegionLSWbisLSE ==
            TriggerProcessingState::pending &&
            processingRegionWarnung == TriggerProcessingState::pending) {
            return TriggerProcessingState::pending;
        } else if (processingRegionLSA == TriggerProcessingState::expliciteexitfehlerbehandlung || processingRegionLSH
                   == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSHbisLSW == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionLSWbisLSE == TriggerProcessingState::expliciteexitfehlerbehandlung ||
                   processingRegionWarnung == TriggerProcessingState::expliciteexitfehlerbehandlung) {
            leavingState();
            //new(this) Fehlerbehandlung;
            //enterByDefaultEntryPoint();
            return TriggerProcessingState::expliciteexitfehlerbehandlung;
        } else {
            return TriggerProcessingState::consumed;
        }
    }
}


void Sortierung::showState() {
    cout << "  Betriebszustand-Substate: Sortierung" << endl;
}