/**
* @file StartTasterWarten22.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von StartTasterWarten22.
 */

#include "StartTasterWarten22.hpp"


void StartTasterWarten22::entry() {
    action->enteredStartTasterWarten22();
    std::cout << "  Sub-State von Ruhezustand: StartTasterWarten22" << std::endl;
}

TriggerProcessingState StartTasterWarten22::s_st2_g() {
    std::cout << "StartTasterWarten22::ST2_G called" << std::endl;
    leavingState();
    return TriggerProcessingState::explicitExitViaB;
}

void StartTasterWarten22::showState() {
    std::cout << "  Sub-State von Ruhezustand: StartTasterWarten22" << std::endl;
}