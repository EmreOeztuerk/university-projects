/**
 * @file RegionLSHPseudoEndState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Endzustand der Region LSH (Höhenmessung).
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSHPSEUDOENDSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSHPSEUDOENDSTATE_H

#include "RegionLSABaseState.h"
#include <iostream>

class RegionLSHPseudoEndState : public RegionLSHBaseState {
public:
    bool isPseudoEndState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "State: RegionLSH End State" << std::endl; }; // Just for illustration!
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSHPSEUDOENDSTATE_H