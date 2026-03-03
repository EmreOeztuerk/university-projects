/**
 * @file RegionLSHBasestate.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Basisklasse RegionLSHBaseState.
 */

#include "RegionLSHBaseState.h"
#include "RegionLSHWartenAufLSH.h"

void RegionLSHBaseState::enterViaPseudoStart() {
    std::cout << "[LSH] Initial Transition taken" << std::endl;
    new(this) RegionLSHWartenAufLSH;
    enterByDefaultEntryPoint();
}

void RegionLSHBaseState::enterViaDeepHistory() {
    enterByDeepHistoryEntryPoint();
}