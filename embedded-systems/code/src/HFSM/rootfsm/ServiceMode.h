/**
* @file ServiceMode.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition des Service-Mode-Zustands.
 *
 * Ein Wartungsmodus, der für Kalibrierung und Tests verwendet wird.
 * Wird durch langes Drücken der Starttaste im Ruhezustand erreicht.
 */

#ifndef SERVICEMODE_H
#define SERVICEMODE_H

#include "BaseState.h"
#include "Betriebszustand.h"
#include "EStop.h"
#include "Ruhezustand.h"
#include "../servicemode_fsm/bc_kalibrierung/bc_kalibrierung_basestate/bc_kalibrierung_basestate.h"

class ServiceMode : public BaseState {
public:
    void enterByDefaultEntryPoint() override;

    virtual void exitExplicitToEStop1() override;

    virtual void exitExplicitToEStop2() override;

    void exitExplicitToBetriebszustand() override;

    void exitExplicitToRuhezustand() override;

    void handleDefaultExit(const TriggerProcessingState &handled) override;

    void leavingState() override;

    // entry/exit-actions
    void entry() override;

    void exit() override;

    // Trigger Service Mode
    void s_st1_g() override;

    void s_stp_g() override;

    void s_nvbdn() override; // nicht verbunden
    void s_e1_g() override;

    void s_e2_g() override;

    // Trigger untere Zustaende
    /* Lichtschranke Anfang */
    void s_lsa1_ub() {
    };

    void s_lsa1_nub() {
    };

    void s_lsa2_ub() {
    };

    void s_lsa2_nub() {
    };

    /* Lichtschranke Hoehenmesser */
    void s_lsh_ub() {
    };

    void s_lsh_nub() {
    };

    /* Metallsensor */
    void s_metal_an() {
    };
    //	void ms_aus();

    /* Lichtschranke Rutsche */
    void s_lsr1_ub() {
    };

    void s_lsr1_nub() {
    };

    void s_lsr2_ub() {
    };

    void s_lsr_nub() {
    };

    /* Lichtschranke Weiche */
    void s_lsw_ub() {
    };

    void s_lsw_nub() {
    };

    /* Lichtschranke Ende */
    void s_lse1_ub() {
    };

    void s_lse1_nub() {
    };

    void s_lse2_ub() {
    };

    void s_lse2_nub() {
    };

    void showState() override;
};

#endif /* SERVICEMODE_H */