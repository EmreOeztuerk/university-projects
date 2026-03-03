/**
 * @file RegionLSWBisLSEBaseState.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Basisklasse RegionLSWBisLSEBaseState.
 */

#include "RegionLSWBisLSEBaseState.h"
#include "RegionLSWBisLSEWartenAufLSE.h"

void RegionLSWBisLSEBaseState::enterViaPseudoStart() {
    std::cout << "[LSW_BIS_LSE] Initial Transition taken" << std::endl;
    new(this) RegionLSWBisLSEWartenAufLSE;
    enterByDefaultEntryPoint();
}

void RegionLSWBisLSEBaseState::enterViaDeepHistory() {
    enterByDeepHistoryEntryPoint();
}