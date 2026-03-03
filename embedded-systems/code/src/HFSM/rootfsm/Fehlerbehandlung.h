/**
 * @file Fehlerbehandlung.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition des Fehlerbehandlungs-Zustands.
 *
 * Wird aktiv bei Fehlern (z.B. Werkstück verschwunden).
 * Verwaltet Quittierung und Behebung über Sub-FSM (`fehlerbehandlungfsm`).
 */

#ifndef FEHLERBEHANDLUNG_H
#define FEHLERBEHANDLUNG_H

#include "BaseState.h"
#include "../fehlerbehandlungfsm/FehlerbehandlungBaseState.h"
#include "../fehlerbehandlungfsm/FehlerbehandlungPseudoStartState.h"

class Fehlerbehandlung : public BaseState {
private:
    FehlerbehandlungPseudoStartState *fbfsm;

public:
    void entry() override;

    void exit() override;

    void handleDefaultExit(const TriggerProcessingState &handled) override;

    void enterByDefaultEntryPoint() override;

    void leavingState() override;

    // Triggers
    void s_e1_g() override;

    void s_e2_g() override;

    void s_rt1_g() override;

    void s_lsr_nub() override;

    void s_lsa2_ub() override;

    void s_st_g() override;


    void s_partner_ok_1() override;

    void s_partner_ok_2() override;

    void showState() override;
};

#endif /* FEHLERBEHANDLUNG_H */