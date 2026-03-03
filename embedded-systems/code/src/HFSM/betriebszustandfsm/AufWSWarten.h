/**
 * @file AufWSWarten.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf Werkstück.
 *
 * Die Anlage ist in Betrieb, aber es befindet sich aktuell kein Werkstück in der Bearbeitungszone,
 * oder es wird auf ein neues Werkstück am Einlauf gewartet.
 */

#ifndef AUFWS_WARTEN_H
#define AUFWS_WARTEN_H

#include "BetriebszustandBaseState.h"

class AufWSWarten : public BetriebszustandBaseState {
public:
    // Entry / Exit
    void entry() override;

    void exit() override;

    void enterByDefaultEntryPoint() override;

    //void enterByDeepHistoryEntryPoint() override;

    //void leavingState() override;
    // Trigger aus dem Sortier-/Sensorbereich
    TriggerProcessingState s_lsa1_ub() override;

    TriggerProcessingState s_ws_an_festo2() override;

    void showState() override;
};

#endif // AUFWS_WARTEN_H