/**
 * @file RegionLSAPseudoEndState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Pseudo-Endzustand der Region LSA.
 *
 * Markiert das Ende des Ablaufs in dieser orthogonalen Region.
 * Dient technisch oft als Platzhalter oder Reset-Punkt für die Region.
 */

#ifndef REGIONLSAPSEUDOENDSTATE_H
#define REGIONLSAPSEUDOENDSTATE_H

#include "RegionLSABaseState.h"
#include <iostream>

class RegionLSAPseudoEndState : public RegionLSABaseState {
public:
    bool isPseudoEndState() override { return true; };

    void enterViaDeepHistory() override { enterViaPseudoStart(); };

    void showState() override { std::cout << "State: RegionLSA End State" << std::endl; }; // Just for illustration!
};

#endif //REGIONLSAPSEUDOENDSTATE_H