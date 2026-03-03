/**
 * @file Ruhezustand.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition des Ruhezustands (Idle).
 *
 * Der Zustand, in dem sich die Maschine nach dem Start oder Reset befindet.
 * Enthält eine eigene Sub-FSM (`RZfsm`).
 */

#ifndef RUHEZUSTAND_H
#define RUHEZUSTAND_H

#include "BaseState.h"
#include "../RZfsm/RZBaseState.hpp"

class Ruhezustand : public BaseState {
private:
    RZBaseState *rzfsm;

public:
    void enterByDefaultEntryPoint() override;


    // void exit() override;
    void leavingState() override;

    void handleDefaultExit(const TriggerProcessingState &handled) override;

    void entry() override;

    // Trigger
    //void  s_st_g() override;
    void s_st1_g() override;

    void s_st2_g() override;

    void s_st1_lg() override;

    void s_st2_lg() override;

    void s_rt1_g() override;

    void s_rt2_g() override;

    void s_e1_g() override;

    void s_e2_g() override;

    void s_stp_g() override;

    //void showState() override;
};

#endif /* RUHEZUSTAND_H */