/**
* @file StartTasterWarten2.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von StartTasterWarten2.
 */

#include "StartTasterWarten2.hpp"

void StartTasterWarten2::entry() {
    action->enteredStartTasterWarten2();
    std::cout << "  Sub-State von Ruhezustand: StartTasterWarten2" << std::endl;
}

TriggerProcessingState StartTasterWarten2::s_st2_lg() {
    std::cout << "StarttasterWarten2::ST2_LG called" << std::endl;
    leavingState();

    return TriggerProcessingState::explicitExitViaS;
}

void StartTasterWarten2::showState() {
    std::cout << "  Sub-State von Ruhezustand: StartTasterWarten2" << std::endl;
}