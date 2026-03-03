/**
* @file BetriebszustandPseudoEndState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Endzustand für den Betriebszustand.
 *
 * Markiert das reguläre Ende des Betriebszustands.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_BETRIEBSZUSTANDPSEUDOENDSTATE_H
#define ESEP_2025WISE_TEAM_1_3_BETRIEBSZUSTANDPSEUDOENDSTATE_H

#include "BetriebszustandBaseState.h"
#include <iostream>

class BetriebszustandPseudoEndState : public BetriebszustandBaseState {
public:
    bool isPseudoEndState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "[Betriebszustand] Endzustand" << std::endl; }; // Just for illustration!
};

#endif //ESEP_2025WISE_TEAM_1_3_BETRIEBSZUSTANDPSEUDOENDSTATE_H