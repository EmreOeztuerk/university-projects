/**
 * @file RegionWarnungPseudoEndState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Endzustand der Warnregion.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONWARNUNGPSEUDOENDSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONWARNUNGPSEUDOENDSTATE_H

#include "RegionWarnungBaseState.h"
#include <iostream>

class RegionWarnungPseudoEndState : public RegionWarnungBaseState {
public:
    bool isPseudoEndState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "State: RegionLSWBisLSE End State" << std::endl; };
    // Just for illustration!
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONWARNUNGPSEUDOENDSTATE_H