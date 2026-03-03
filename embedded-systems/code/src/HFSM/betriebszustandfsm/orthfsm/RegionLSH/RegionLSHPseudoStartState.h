/**
 * @file RegionLSHPseudoStartState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Startzustand für die Region LSH.
 *
 * Initialisiert die Region und wechselt in den Zustand `RegionLSHWartenAufWSBeiLSH`.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSHPSEUDOSTARTSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSHPSEUDOSTARTSTATE_H

#include "RegionLSHBaseState.h"

class RegionLSHPseudoStartState : public RegionLSHBaseState {
public:
    bool isPseudoStartState() { return true; };
    void enterByDeepHistoryEntryPoint() override { enterViaPseudoStart(); };
    void showState() override { std::cout << "RegionLSHBaseState: RegionLSH Startzustand" << std::endl; };
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSHPSEUDOSTARTSTATE_H