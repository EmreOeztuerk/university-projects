/**
 * @file RegionLSABaseState.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Basisklasse RegionLSABaseState.
 */

#include "RegionLSABaseState.h"
#include "RegionLSAWartenAufLSA.h"

void RegionLSABaseState::enterViaPseudoStart() {
    std::cout << "[Betriebszustand] RegionLSA Initial Transition taken" << std::endl;
    new(this) RegionLSAWartenAufLSA;
    enterByDefaultEntryPoint();
}

void RegionLSABaseState::enterViaDeepHistory() {
    enterByDeepHistoryEntryPoint();
}