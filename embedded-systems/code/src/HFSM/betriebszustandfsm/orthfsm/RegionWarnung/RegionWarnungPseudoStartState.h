/**
 * @file RegionWarnungPseudoStartState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Startzustand der Warnregion.
 *
 * Initialisiert die Region und wechselt in den Zustand `RegionWarnungWartenAufWarnung`.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONWARNUNGPSEUDOSTARTSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONWARNUNGPSEUDOSTARTSTATE_H

#include "RegionWarnungBasestate.h"

class RegionWarnungPseudoStartState : public RegionWarnungBaseState {
public:
    bool isPseudoStartState() { return true; };
    void enterByDeepHistoryEntryPoint() override { enterViaPseudoStart(); };
    void showState() { std::cout << "RegionWarnungBasestate: RegionWarnung Startzustand" << std::endl; };
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONWARNUNGPSEUDOSTARTSTATE_H