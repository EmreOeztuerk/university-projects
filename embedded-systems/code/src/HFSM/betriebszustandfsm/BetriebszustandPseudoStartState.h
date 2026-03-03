/**
 * @file BetriebszustandPseudoStartState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Startzustand des Betriebszustands.
 *
 * Initialisiert die Sub-FSM (oft direkt in den Zustand `AufWSWarten` oder `Sortierung`).
 */

#ifndef ESEP_2025WISE_TEAM_1_3_BETRIEBSZUSTANDPSEUDOSTARTSTATE_H
#define ESEP_2025WISE_TEAM_1_3_BETRIEBSZUSTANDPSEUDOSTARTSTATE_H

#include "BetriebszustandBaseState.h"

class BetriebszustandPseudoStartState : public BetriebszustandBaseState {
public:
    bool isPseudoStartState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "[Betriebszustand] Pseudo Startzustand" << std::endl; };
};

#endif //ESEP_2025WISE_TEAM_1_3_BETRIEBSZUSTANDPSEUDOSTARTSTATE_H