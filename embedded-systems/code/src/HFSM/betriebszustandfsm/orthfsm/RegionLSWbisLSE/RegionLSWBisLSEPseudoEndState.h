/**
 * @file RegionLSWBisLSEPseudoEndState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Endzustand der Region LSW bis LSE.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEPSEUDOENDSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEPSEUDOENDSTATE_H

#include "RegionLSABaseState.h"
#include <iostream>

class RegionLSWBisLSEPseudoEndState : public RegionLSWBisLSEBaseState {
public:
    bool isPseudoEndState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "State: RegionLSWBisLSE End State" << std::endl; };
    // Just for illustration!
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEPSEUDOENDSTATE_H