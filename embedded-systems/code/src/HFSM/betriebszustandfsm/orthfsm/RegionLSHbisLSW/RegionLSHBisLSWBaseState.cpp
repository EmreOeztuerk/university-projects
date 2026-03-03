/**
 * @file RegionLSHBisLSWBaseState.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Basisklasse RegionLSHBisLSWBaseState.
 */

#include "RegionLSHBisLSWBaseState.h"
#include "RegionLSHBisLSWWartenAufLSW.h"

void RegionLSHBisLSWBaseState::enterViaPseudoStart() {
    std::cout << "[LSH_BIS_LSW] Initial Transition taken" << std::endl;
    new(this) RegionLSHBisLSWWartenAufLSW;
    enterByDefaultEntryPoint();
}

void RegionLSHBisLSWBaseState::enterViaDeepHistory() {
    enterByDeepHistoryEntryPoint();
}