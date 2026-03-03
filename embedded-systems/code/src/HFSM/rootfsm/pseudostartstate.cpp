/**
* @file pseudostartstate.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des PseudoStartStates.
 */

#include "pseudostartstate.h"

bool PseudoStartState::isPseudoStartState() {
    return true;
}

void PseudoStartState::showState() {
    std::cout << "State: Pseudo Start State" << std::endl;
}