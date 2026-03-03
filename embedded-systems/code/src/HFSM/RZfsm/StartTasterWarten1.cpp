/**
 * @file StartTasterWarten1.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von StartTasterWarten1.
 */

#include "StartTasterWarten1.hpp"


void StartTasterWarten1::entry() {
    action->enteredStartTasterWarten1();
    std::cout << "  Sub-State von Ruhezustand: StartTasterWarten1" << std::endl;
}

TriggerProcessingState StartTasterWarten1::s_st1_lg() {
    std::cout << "StarttasterWarten1::ST1_LG called" << std::endl;
    leavingState();
    return TriggerProcessingState::explicitExitViaS;
}

void StartTasterWarten1::showState() {
    std::cout << "  Sub-State von Ruhezustand: StartTasterWarten1" << std::endl;
}