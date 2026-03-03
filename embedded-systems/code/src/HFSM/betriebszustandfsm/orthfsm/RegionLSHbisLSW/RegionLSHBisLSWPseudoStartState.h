/**
 * @file RegionLSHBisLSWPseudoStartState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Startzustand der Region LSH bis LSW.
 *
 * Initialisiert die Region und wechselt in den Zustand `RegionLSHBisLSWKomplettTypErkennen`.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSHBISLSWPSEUDOSTARTSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSHBISLSWPSEUDOSTARTSTATE_H

#include "RegionLSHBisLSWBaseState.h"

class RegionLSHBisLSWPseudoStartState : public RegionLSHBisLSWBaseState {
public:
    bool isPseudoStartState() { return true; };
    void enterByDeepHistoryEntryPoint() override { enterViaPseudoStart(); };
    void showState() { std::cout << "RegionLSHBisLSWBaseState: RegionLSHBisLSW Startzustand" << std::endl; };
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSHBISLSWPSEUDOSTARTSTATE_H