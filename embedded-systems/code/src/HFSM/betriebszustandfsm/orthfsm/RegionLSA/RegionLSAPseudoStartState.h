/**
 * @file RegionLSAPseudoStartState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Startzustand der Region LSA.
 *
 * Initialisiert die Region und wechselt in den Zustand `RegionLSAWartenAufWSBeiLSA`.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSAPSEUDOSTARTSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSAPSEUDOSTARTSTATE_H

#include "RegionLSABaseState.h"

class RegionLSAPseudoStartState : public RegionLSABaseState {
public:
    bool isPseudoStartState() override { return true; };
    void enterByDeepHistoryEntryPoint() override { enterViaPseudoStart(); };
    void showState() override { std::cout << "RegionLSABaseState: RegionLSA Startzustand" << std::endl; };
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSAPSEUDOSTARTSTATE_H