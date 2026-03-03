/**
 * @file RegionLSHBisLSWPseudoEndState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Endzustand der Region LSH bis LSW.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSHBISLSWPSEUDOENDSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSHBISLSWPSEUDOENDSTATE_H

#include "RegionLSHBisLSWBaseState.h"
#include <iostream>

class RegionLSHBisLSWPseudoEndState : public RegionLSHBisLSWBaseState {
public:
    bool isPseudoEndState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "State: RegionLSA End State" << std::endl; }; // Just for illustration!
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSHBISLSWPSEUDOENDSTATE_H