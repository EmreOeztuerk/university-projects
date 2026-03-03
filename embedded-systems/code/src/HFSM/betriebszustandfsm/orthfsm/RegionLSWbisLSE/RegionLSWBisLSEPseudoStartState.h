/**
 * @file RegionLSWBisLSEPseudoStartState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Startzustand für die Region LSW bis LSE.
 *
 * Initialisiert die Region und wechselt je nach Anlagenkonfiguration (Festo 1 oder 2)
 * in den passenden Wartezustand.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEPSEUDOSTARTSTATE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEPSEUDOSTARTSTATE_H

#include "RegionLSWBisLSEBaseState.h"

class RegionLSWBisLSEPseudoStartState : public RegionLSWBisLSEBaseState {
public:
    bool isPseudoStartState() { return true; };
    void enterByDeepHistoryEntryPoint() override { enterViaPseudoStart(); };
    void showState() override { std::cout << "RegionLSWBisLSEBaseState: RegionLSWBisLSE Startzustand" << std::endl; };
};

#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEPSEUDOSTARTSTATE_H