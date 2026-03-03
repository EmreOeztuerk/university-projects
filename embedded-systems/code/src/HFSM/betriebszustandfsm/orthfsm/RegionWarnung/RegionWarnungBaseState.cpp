/**
 * @file RegionWarnungBaseState.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Basisklasse RegionWarnungBasestate.
 */

#include "RegionWarnungBasestate.h"
#include "RegionWarnungWartenAufWarnung.h"

void RegionWarnungBaseState::enterViaPseudoStart() {
    std::cout << "[Warnung] Initial Transition taken" << std::endl;
    new(this) RegionWarnungWartenAufWarnung;
    enterByDefaultEntryPoint();
}

void RegionWarnungBaseState::enterViaDeepHistory() {
    enterByDeepHistoryEntryPoint();
}