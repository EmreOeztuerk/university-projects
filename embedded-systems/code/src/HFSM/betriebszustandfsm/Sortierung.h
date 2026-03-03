/**
 * @file Sortierung.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand Sortierung (Haupt-Betriebsphase).
 *
 * In diesem Zustand ist die orthogonale Region aktiv (LSA, LSH, LSW, etc.),
 * um mehrere Werkstücke gleichzeitig zu verarbeiten.
 */

#ifndef SORTIERUNG_H
#define SORTIERUNG_H

#include "BetriebszustandBaseState.h"

class Sortierung : public BetriebszustandBaseState {
public:
    // Entry/Exit
    void entry() override;

    //void exit()  override;

    void enterByDefaultEntryPoint() override;

    void enterByDeepHistoryEntryPoint() override;

    void leavingState() override;

    TriggerProcessingState handleDefaultExit();

    TriggerProcessingState cmd_festos_frei() override;

    // --------------------------------------------------------
    // Trigger der FSM "Sortierung"
    // --------------------------------------------------------
    // Sensoren
    virtual TriggerProcessingState s_lsa1_ub() override;

    virtual TriggerProcessingState s_lsa2_ub() override;

    virtual TriggerProcessingState s_lsh_ub() override;

    virtual TriggerProcessingState s_lsh_nub() override;

    virtual TriggerProcessingState s_lsw_ub() override;

    virtual TriggerProcessingState s_lsr_ub() override;

    virtual TriggerProcessingState s_lse_ub() override;

    virtual TriggerProcessingState s_lse_nub() override;

    virtual TriggerProcessingState s_lse1_ub() override;

    virtual TriggerProcessingState s_lse2_ub() override;

    virtual TriggerProcessingState s_lse2_nub() override;

    virtual TriggerProcessingState s_metal_an() override;

    virtual TriggerProcessingState s_e1_g() override;

    virtual TriggerProcessingState s_e2_g() override;

    //virtual TriggerProcessingState s_e1_ng() override;
    //virtual TriggerProcessingState s_e2_ng() override;
    // Übergabe / Festo
    virtual TriggerProcessingState s_uebergabe_ws() override;
    virtual TriggerProcessingState s_uebergabe_ws_fertig() override;
    virtual TriggerProcessingState s_festo2_bereit() override;

    virtual TriggerProcessingState s_festo2_nbereit() override;

    virtual TriggerProcessingState s_festo2_frei() override;

    virtual TriggerProcessingState s_festo1_frei() override;

    virtual TriggerProcessingState s_festo1_stoppen() override;


    // Fehler
    // virtual TriggerProcessingState ec_vbdng_ub()   override;
    // virtual TriggerProcessingState ec_ws_vrln()    override;
    // virtual TriggerProcessingState ec_ws_unb()     override;
    // virtual TriggerProcessingState ec_abstand_ne() override;
    virtual TriggerProcessingState ec_rtsch_voll() override;

    // Timer
    virtual TriggerProcessingState t_lsa_lsh() override;

    virtual TriggerProcessingState t_lsh_lsm() override;

    virtual TriggerProcessingState t_lsw_lse() override;

    virtual TriggerProcessingState t_lsw_lsr() override;

    virtual TriggerProcessingState t_lse1_lsa2() override;

    virtual TriggerProcessingState t_lsa_q1() override;

    virtual TriggerProcessingState t_toleranzzeit_lsa() override;

    virtual TriggerProcessingState t_toleranzzeit_lsh() override;

    virtual TriggerProcessingState t_toleranzzeit_lsw() override;

    virtual TriggerProcessingState t_toleranzzeit_lse() override;

    ///virtual TriggerProcessingState s_lsa_nub() override;
    //virtual TriggerProcessingState s_lsw_nub() override;
    //virtual TriggerProcessingState s_lsr_nub() override;
    //virtual TriggerProcessingState s_lse1_nub() override;

    virtual TriggerProcessingState t_lsh_lsw() override;

    virtual TriggerProcessingState t_q1_lsh() override;

    //virtual TriggerProcessingState t_ping_pong() override;


    virtual TriggerProcessingState cmd_rtsch_voll() override;

    virtual TriggerProcessingState cmd_rtsch_nv() override;

    void showState() override;
};

#endif // SORTIERUNG_H